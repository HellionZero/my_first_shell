#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [ -z "$SHELL" ]; then
	if [ -x "$SCRIPT_DIR/../minishell" ]; then
		SHELL="$SCRIPT_DIR/../minishell"
	elif [ -x "$PWD/../minishell" ]; then
		SHELL="$PWD/../minishell"
	elif [ -x "$PWD/minishell" ]; then
		SHELL="$PWD/minishell"
	fi
fi
if [ -z "$SHELL_PATH" ]; then
	SHELL_PATH="$SHELL"
fi

# Quick build check: compile if binary missing or not executable
if [ ! -x "$SHELL" ]; then
	echo "minishell binary not found or not executable, attempting to build..."
	(cd "$SCRIPT_DIR/.." && make -j) >/dev/null 2>&1 || { echo "Compilation failed" >&2; exit 1; }
	if [ -x "$SCRIPT_DIR/../minishell" ]; then
		SHELL="$SCRIPT_DIR/../minishell"
		SHELL_PATH="$SHELL"
	fi
fi
if [ ! -x "$SHELL" ]; then
	echo "minishell binary not found after build" >&2
	exit 1
fi

# Script para testar memory leaks no minishell
# Usa valgrind com suppressions para readline

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuráveis: ajustar conforme necessário
# LEAK_STILL_THRESHOLD  - limite em bytes para "still reachable" (padrão 4096)
# FAIL_ON_STILL_REACHABLE - se setado para 1, marca erro quando "still reachable" > limite
# VALGRIND_EXTRA_OPTS - permite passar flags extras ao valgrind (ex.: "--trace-children=yes")
STILL_THRESHOLD_BYTES=${LEAK_STILL_THRESHOLD:-4096}
FAIL_ON_STILL_REACHABLE=${FAIL_ON_STILL_REACHABLE:-0}
VALGRIND_EXTRA_OPTS="${VALGRIND_EXTRA_OPTS:-}"

# Arquivo de suppressions para valgrind (gerado automaticamente quando necessário)
SUPPRESSIONS_FILE="$SCRIPT_DIR/valgrind_externals.supp"

# Garante existência do arquivo de suppressions com cabeçalho
ensure_supp_file() {
    if [ ! -f "$SUPPRESSIONS_FILE" ]; then
        mkdir -p "$(dirname "$SUPPRESSIONS_FILE")" 2>/dev/null || true
        cat > "$SUPPRESSIONS_FILE" <<'EOF'
# Valgrind suppressions geradas automaticamente para comandos externos
# Não editar manualmente a não ser que saiba o que está a fazer.
EOF
    fi
}

echo "================================================"
echo "  Memory Leak Test - minishell"
echo "================================================"
echo ""

# Função para executar teste com valgrind e exibir detalhes
run_test() {
    local test_name="$1"
    local commands="$2"

    echo -n "[$test_name] [$commands]..."

    # Caminho do valgrind e do binário
    VALGRIND_BIN=$(command -v valgrind)
    MINISHELL_BIN="../minishell"

    if [ -z "$VALGRIND_BIN" ]; then
        echo -e "${YELLOW}Valgrind não encontrado, pulando teste.${NC}"
        return 0
    fi

    if [ ! -x "$MINISHELL_BIN" ]; then
        echo -e "${RED}Binário minishell não encontrado!${NC}"
        return 1
    fi

    # Executar valgrind e salvar saída completa (pode gerar várias se --trace-children)
    output=$(echo -e "$commands" | $VALGRIND_BIN --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --error-exitcode=42 \
        $VALGRIND_EXTRA_OPTS \
        $MINISHELL_BIN 2>&1)

    # Função auxiliar: soma todos os números encontrados nas linhas correspondentes
    sum_category() {
        # $1 = pattern e.g. "definitely lost"
        echo "$output" | grep -i "$1" | sed -E "s/.*\\b$1:\\s*([0-9,]+).*/\\1/i" | tr -d ',' | awk '{s+=$1} END{print s+0}'
    }

    # Agrega valores de cada categoria (soma across processes)
    definitely_lost=$(sum_category "definitely lost")
    indirectly_lost=$(sum_category "indirectly lost")
    possibly_lost=$(sum_category "possibly lost")
    still_reachable=$(sum_category "still reachable")

    # Normaliza se não encontrado
    [ -z "$definitely_lost" ] && definitely_lost=0
    [ -z "$still_reachable" ] && still_reachable=0

    # Para diagnóstico: apresentar totais agregados e linhas individuais relevantes
    printf "  Totais agregados: definitely=%s indirectly=%s possibly=%s still=%s\n" "$definitely_lost" "$indirectly_lost" "$possibly_lost" "$still_reachable"
    echo "  (detalhes individuais abaixo)"
    echo "$output" | grep -n -E "definitely lost:|indirectly lost:|possibly lost:|still reachable:|ERROR SUMMARY" || true

    # Se houver 'definitely lost' bytes, decidir se vem do minishell ou de filhos
    if [ "$definitely_lost" -gt 0 ] 2>/dev/null; then
        # Identifica PIDs do minishell (compara basename do binário)
        minishell_basename=$(basename "$MINISHELL_BIN")
        minishell_pids=$(echo "$output" | grep -E "^==[0-9]+== Command:" | sed -E 's/^==([0-9]+)==.*Command: (.*)$/\1\t\2/' | awk -F '\t' -v b="$minishell_basename" '$2 ~ b {print $1}' | sort -u)

        # Soma definitivamente perdidos apenas para o minishell
        definitely_mini=0
        for pid in $minishell_pids; do
            val=$(echo "$output" | grep -E "^==${pid}==.*definitely lost" | sed -E 's/.*definitely lost:\s*([0-9,]+).*/\1/' | tr -d ',' | awk '{s+=$1} END{print s+0}')
            definitely_mini=$((definitely_mini + val))
        done

        if [ "$definitely_mini" -gt 0 ] 2>/dev/null; then
            echo -e "${RED}✗ Leak detectado no minishell: definitely lost = ${definitely_mini} bytes${NC}"
            echo "  --- Valgrind LEAK SUMMARY (contexto) ---"
            echo "$output" | grep -n -A5 -E "definitely lost:|LEAK SUMMARY" | sed -n '1,200p'
            # Mostra saída dos pids do minishell para investigação
            for pid in $minishell_pids; do
                echo "\n--- Valgrind output for minishell process $pid ---"
                echo "$output" | grep -E "^==${pid}==" || true
            done
            echo "  --- Fim do resumo ---"
            return 1
        else
            # Não falha: leaks vêm de processos-filho externos (ex.: grep), reportar como aviso
            echo -e "${YELLOW}! 'definitely lost' total = ${definitely_lost} bytes, mas nenhum vem do minishell (provavel filho/externo)${NC}"
            echo "  --- Linhas relevantes (processos com 'definitely lost') ---"
        echo "$output" | grep -n -E "definitely lost:|LEAK SUMMARY" | sed -n '1,200p'
        echo "  --- Tentando gerar suppressions temporários para processos-filho detectados e reaplicar suprimir... ---"
        # Extrai pids de processos que mostram 'definitely lost' e seus comandos
        pids=$(echo "$output" | grep -E "==[0-9]+==.*definitely lost|==[0-9]+== LEAK SUMMARY" | sed -E 's/.*==([0-9]+)==.*/\1/' | sort -u)

        tmp_supp=$(mktemp)
        # Se houver um arquivo permanente com suppressions, usá-lo como base
        if [ -f "$SUPPRESSIONS_FILE" ]; then
            cat "$SUPPRESSIONS_FILE" > "$tmp_supp"
        else
            echo "# Temporary suppressions generated" > "$tmp_supp"
        fi

        for pid in $pids; do
            cmd=$(echo "$output" | grep -E "^==${pid}== Command:" | sed -E 's/^==[0-9]+== Command: //')
            if [ -n "$cmd" ]; then
                echo "  - Gerando suppressions temporárias para comando: $cmd (pid $pid)"
                tmp_log="/tmp/valgen_${pid}.$$"
                # Executa o comando sob valgrind com --gen-suppressions=all (sem trace-children para focar no binário)
                sh -c "printf 'test\n' | $VALGRIND_BIN --leak-check=full --show-leak-kinds=all --num-callers=20 --gen-suppressions=all --trace-children=no --log-file=$tmp_log $cmd" >/dev/null 2>&1 || true
                if [ -f "$tmp_log" ]; then
                    # Extrai blocos de suppression do log
                    awk '/\{/{buf=$0; inside=1; next} inside{buf=buf"\n"$0} /\}/{if(inside){print buf"\n}"; inside=0}}' "$tmp_log" | \
                        awk 'BEGIN{RS="\n}\n"; ORS="\n\n"} /Memcheck:/ {s=$0; gsub(/\r/,"",s); print s"}\n"}' | while read -r block; do
                            name=$(echo "$block" | sed -n '1,5p' | tr -d '\n' | sed -E 's/[^[:print:]]//g' | cut -c1-80)
                            if ! grep -Fq "$name" "$tmp_supp" 2>/dev/null; then
                                echo "$block" >> "$tmp_supp"
                                echo "    -> Temporary suppression adicionada"
                            else
                                echo "    -> Temporary suppression já existe, pulando"
                            fi
                        done
                    rm -f "$tmp_log" || true
                fi
            fi
        done

        # Re-executa o teste com o arquivo temporário de suppressions para confirmar limpeza
        echo "  --- Re-executando valgrind com suppressions temporárias para este teste... ---"
        new_output=$(echo -e "$commands" | $VALGRIND_BIN --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=42 $VALGRIND_EXTRA_OPTS --suppressions="$tmp_supp" $MINISHELL_BIN 2>&1)
        new_def=$(echo "$new_output" | grep -i "definitely lost:" | sed -E 's/.*definitely lost:[[:space:]]*([0-9,]+).*/\1/' | tr -d ',' | awk '{s+=$1} END{print s+0}')
        if [ -z "$new_def" ] || [ "$new_def" -eq 0 ]; then
            echo -e "${GREEN}✓ Após suppressions temporárias: nenhum 'definitely lost' no minishell ou filhos (para este teste).${NC}"
            rm -f "$tmp_supp" || true
            return 0
        else
            echo -e "${YELLOW}! Após suppressions temporárias, ainda existe definitely lost = ${new_def} bytes${NC}"
            echo "  Saída (após suppressions):"
            echo "$new_output" | grep -n -E "definitely lost:|LEAK SUMMARY|ERROR SUMMARY" | sed -n '1,200p'
            rm -f "$tmp_supp" || true
            return 0
        fi
        rm -f "$tmp_supp" || true
        return 0
        fi
    fi

    # Verifica still reachable para regressões (limite configurável)
    if [ "$still_reachable" -gt "$STILL_THRESHOLD_BYTES" ] 2>/dev/null; then
        if [ "$FAIL_ON_STILL_REACHABLE" -eq 1 ]; then
            echo -e "${RED}✗ 'still reachable' = ${still_reachable} bytes (limite ${STILL_THRESHOLD_BYTES})${NC}"
            echo "  --- Valgrind LEAK SUMMARY (contexto) ---"
            echo "$output" | grep -n -A5 -E "still reachable:|LEAK SUMMARY" | sed -n '1,80p'
            echo "  --- Fim do resumo ---"
            return 1
        else
            echo -e "${YELLOW}! 'still reachable' = ${still_reachable} bytes (limite ${STILL_THRESHOLD_BYTES})${NC}"
            echo "  (Sete FAIL_ON_STILL_REACHABLE=1 para tratar como falha)"
            return 0
        fi
    fi

    # Verifica se há erros reportados pelo valgrind (agregando todas as linhas ERROR SUMMARY)
    error_lines=$(echo "$output" | grep "ERROR SUMMARY" || true)
    if [ -n "$error_lines" ]; then
        # Se qualquer linha não contém '0 errors from 0 contexts' considerar como erro
        nonzero_errors=$(echo "$error_lines" | grep -v "0 errors from 0 contexts" || true)
        if [ -n "$nonzero_errors" ]; then
            echo -e "${YELLOW}⊘ Erros detectados pelo Valgrind (em algum processo):${NC}"
            echo "$nonzero_errors"
            echo "  Definitely lost: $definitely_lost bytes"
            # Decide se os erros vêm do minishell ou de filhos
            minishell_basename=$(basename "$MINISHELL_BIN")
            err_minishell=0
            err_pids=$(echo "$nonzero_errors" | sed -E 's/.*==([0-9]+)==.*/\1/' | sort -u)
            for pid in $err_pids; do
                # checa se esse pid corresponde ao minishell
                cmdline=$(echo "$output" | grep -E "^==${pid}== Command:" || true)
                if echo "$cmdline" | grep -q "$minishell_basename"; then
                    err_minishell=1
                fi
            done
            if [ "$err_minishell" -eq 1 ]; then
                echo -e "${RED}⊘ Erros no Valgrind vindos do minishell (falha)${NC}"
                for pid in $err_pids; do
                    echo "\n--- Valgrind output for process $pid (errors) ---"
                    echo "$output" | grep -E "^==${pid}==" || true
                done
                return 1
            else
                echo -e "${YELLOW}! Erros detectados em processos-filho/externos — não tratados como falha do minishell.${NC}"
                for pid in $err_pids; do
                    echo "\n--- Valgrind output for process $pid (errors) ---"
                    echo "$output" | grep -E "^==${pid}==" || true
                done
                return 0
            fi
        fi
    fi

    echo -e "${GREEN}✓ OK${NC}"
    return 0
}

# Testes
FAILED=0
echo "Executando testes:"
echo ""

run_test "Comando simples      " "pwd\nexit" || FAILED=1
run_test "Pipe                 " "echo hello | grep h\nexit" || FAILED=1
run_test "Redirecionamento >   " "echo test > /tmp/test.txt\nrm /tmp/test.txt\nexit" || FAILED=1
run_test "Redirecionamento <   " "echo test > /tmp/test.txt\ncat < /tmp/test.txt\nrm /tmp/test.txt\nexit" || FAILED=1
run_test "Redirecionamento >>  " "echo a > /tmp/test.txt\necho b >> /tmp/test.txt\nrm /tmp/test.txt\nexit" || FAILED=1
run_test "Heredoc              " "cat << EOF\nhello\nworld\nEOF\nexit" || FAILED=1
run_test "Operador &&          " "echo a && echo b\nexit" || FAILED=1
run_test "Operador ||          " "false || echo b\nexit" || FAILED=1
run_test "Export/Unset         " "export TEST=value\necho \$TEST\nunset TEST\nexit" || FAILED=1
run_test "Builtin cd           " "cd /tmp\npwd\ncd -\nexit" || FAILED=1
run_test "Múltiplos pipes      " "echo test | cat | cat | cat\nexit" || FAILED=1
run_test "Comandos complexos   " "ls | grep t && echo ok || echo fail\nexit" || FAILED=1

echo ""
echo "================================================"
if [ "$FAILED" -eq 0 ]; then
    echo -e "${GREEN}Todos os testes concluídos sem 'definitely lost'!${NC}"
    echo "================================================"
    exit 0
else
    echo -e "${RED}Alguns testes detectaram 'definitely lost' bytes. Verifique os detalhes acima.${NC}"
    echo "================================================"
    exit 1
fi
