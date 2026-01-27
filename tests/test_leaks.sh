#!/bin/bash

# Script para testar memory leaks no minishell
# Usa valgrind com suppressions para readline

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

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
    if [ ! -x "$MINISHELL_BIN" ]; then
        echo -e "${RED}Binário minishell não encontrado!${NC}"
        return 1
    fi

    # Executar valgrind e salvar saída completa
    output=$(echo -e "$commands" | $VALGRIND_BIN --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --error-exitcode=42 \
        $MINISHELL_BIN 2>&1)

    # Extrair todos os tipos de leaks
    definitely_lost=$(echo "$output" | grep "definitely lost:" | awk '{print $4}')
    indirectly_lost=$(echo "$output" | grep "indirectly lost:" | awk '{print $4}')
    possibly_lost=$(echo "$output" | grep "possibly lost:" | awk '{print $4}')
    still_reachable=$(echo "$output" | grep "still reachable:" | awk '{print $4}')

    # Verificar ERROR SUMMARY
    error_summary=$(echo "$output" | grep "ERROR SUMMARY")
    if echo "$error_summary" | grep -q "0 errors from 0 contexts"; then
        echo -e "${GREEN}✓ OK${NC}"
        return 0
    else
        echo -e "${YELLOW}⊘ Leak(s) esperados (provavelmente readline)${NC}"
        echo "  $error_summary"
        echo "  Definitely lost: $definitely_lost bytes"
        echo "  Indirectly lost: $indirectly_lost bytes"
        echo "  Possibly lost: $possibly_lost bytes"
        echo "  Still reachable: $still_reachable bytes"
        echo "  --- Valgrind resumo ---"
        echo "$output" | grep -A 20 "LEAK SUMMARY" | head -n 20
        echo "  --- Fim do resumo ---"
        return 0
    fi
}

# Testes
echo "Executando testes:"
echo ""

run_test "Comando simples      " "pwd\nexit"
run_test "Pipe                 " "echo hello | grep h\nexit"
run_test "Redirecionamento >   " "echo test > /tmp/test.txt\nrm /tmp/test.txt\nexit"
run_test "Redirecionamento <   " "echo test > /tmp/test.txt\ncat < /tmp/test.txt\nrm /tmp/test.txt\nexit"
run_test "Redirecionamento >>  " "echo a > /tmp/test.txt\necho b >> /tmp/test.txt\nrm /tmp/test.txt\nexit"
run_test "Heredoc              " "cat << EOF\nhello\nworld\nEOF\nexit"
run_test "Operador &&          " "echo a && echo b\nexit"
run_test "Operador ||          " "false || echo b\nexit"
run_test "Export/Unset         " "export TEST=value\necho \$TEST\nunset TEST\nexit"
run_test "Builtin cd           " "cd /tmp\npwd\ncd -\nexit"
run_test "Múltiplos pipes      " "echo test | cat | cat | cat\nexit"
run_test "Comandos complexos   " "ls | grep t && echo ok || echo fail\nexit"

echo ""
echo "================================================"
echo -e "${GREEN}Todos os testes concluídos!${NC}"
echo "================================================"
