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

# Comprehensive test suite for echo builtin

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SHELL_PATH="../minishell"
PASSED=0
FAILED=0
TOTAL=0

# Função para executar teste
run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Executa o comando no shell e extrai o output
    mysh_full=$(echo -e "$command\nexit" | NO_COLOR=1 $SHELL_PATH 2>&1)
        actual=$(echo "$mysh_full" | grep -v -e '^$' -e '^\[minishell\]>' | head -n1 | sed 's/exit$//')
    
    if [ "$actual" = "$expected" ]; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $test_name"
        echo -e "   ${YELLOW}Expected:${NC} '$expected'"
        echo -e "   ${YELLOW}Got:${NC}      '$actual'"
        FAILED=$((FAILED + 1))
    fi
}

# Função para testar com bash de referência
compare_with_bash() {
    local test_name="$1"
    local command="$2"
    
    TOTAL=$((TOTAL + 1))
    
    # Executa no bash
    bash_output=$(bash -c "$command" 2>&1)
    
    # Executa no myshell e extrai apenas o output do comando
    mysh_full=$(echo -e "$command\nexit" | NO_COLOR=1 $SHELL_PATH 2>&1)
        # Extrai a primeira linha relevante do output
        mysh_output=$(echo "$mysh_full" | grep -v -e '^$' -e '^\[minishell\]>' | head -n1 | sed 's/exit$//')
    
    if [ "$bash_output" = "$mysh_output" ]; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $test_name"
        echo -e "   ${YELLOW}Bash output:${NC}   '$bash_output'"
        echo -e "   ${YELLOW}Minishell output:${NC} '$mysh_output'"
        FAILED=$((FAILED + 1))
    fi
}

echo -e "${BLUE}════════════════════════════════════════${NC}"
echo -e "${BLUE}  Testing ECHO Builtin                  ${NC}"
echo -e "${BLUE}════════════════════════════════════════${NC}\n"

# ========================================
# Testes Básicos
# ========================================
echo -e "${BLUE}Basic Tests:${NC}"

compare_with_bash "echo: simple text" "echo hello"
compare_with_bash "echo: multiple words" "echo hello world"
compare_with_bash "echo: no arguments" "echo"
compare_with_bash "echo: single space" "echo ' '"
compare_with_bash "echo: multiple spaces" "echo 'a  b  c'"
compare_with_bash "echo: tab character" "echo 'hello	world'"

echo ""

# ========================================
# Testes com flag -n
# ========================================
echo -e "${BLUE}-n Flag Tests:${NC}"

compare_with_bash "echo -n: suppress newline" "echo -n hello"
compare_with_bash "echo -n: multiple words" "echo -n hello world"
compare_with_bash "echo -n: no args" "echo -n"
compare_with_bash "echo -nn: multiple n" "echo -nn hello"
compare_with_bash "echo -nnn: three n" "echo -nnn test"
compare_with_bash "echo -nnnn: four n" "echo -nnnn test"

echo ""

# ========================================
# Testes com múltiplas flags -n
# ========================================
echo -e "${BLUE}Multiple -n Flags Tests:${NC}"

compare_with_bash "echo: -n -n text" "echo -n -n hello"
compare_with_bash "echo: -n -nn text" "echo -n -nn world"
compare_with_bash "echo: -nn -n text" "echo -nn -n test"
compare_with_bash "echo: -n -n -n text" "echo -n -n -n multiple"

echo ""

# ========================================
# Testes com flags inválidas
# ========================================
echo -e "${BLUE}Invalid Flag Tests:${NC}"

compare_with_bash "echo: -a flag (invalid)" "echo -a hello"
compare_with_bash "echo: -n followed by -a" "echo -n -a test"
compare_with_bash "echo: -na (mixed)" "echo -na test"
compare_with_bash "echo: -nx (n with invalid)" "echo -nx hello"

echo ""

# ========================================
# Testes com strings especiais
# ========================================
echo -e "${BLUE}Special String Tests:${NC}"

compare_with_bash "echo: empty string" "echo ''"
compare_with_bash "echo: quoted string" "echo 'hello world'"
compare_with_bash "echo: double quotes" 'echo "hello world"'
compare_with_bash "echo: backslash" "echo 'hello\\world'"
compare_with_bash "echo: dollar sign" "echo '$HOME'"
compare_with_bash "echo: asterisk" "echo '*'"
compare_with_bash "echo: question mark" "echo '?'"

echo ""

# ========================================
# Testes com caracteres especiais
# ========================================
echo -e "${BLUE}Special Character Tests:${NC}"

compare_with_bash "echo: exclamation" "echo 'hello!'"
compare_with_bash "echo: at sign" "echo 'user@host'"
compare_with_bash "echo: hash" "echo '#comment'"
compare_with_bash "echo: percent" "echo '100%'"
compare_with_bash "echo: ampersand" "echo 'A&B'"
compare_with_bash "echo: equals" "echo 'A=B'"
compare_with_bash "echo: plus" "echo '1+1'"
compare_with_bash "echo: minus" "echo '5-3'"

echo ""

# ========================================
# Testes com números
# ========================================
echo -e "${BLUE}Number Tests:${NC}"

compare_with_bash "echo: single number" "echo 42"
compare_with_bash "echo: negative number" "echo -42"
compare_with_bash "echo: decimal" "echo 3.14"
compare_with_bash "echo: multiple numbers" "echo 1 2 3 4 5"

echo ""

# ========================================
# Testes com -n e texto especial
# ========================================
echo -e "${BLUE}-n with Special Cases:${NC}"

compare_with_bash "echo -n: with spaces" "echo -n 'a  b  c'"
compare_with_bash "echo -n: empty string" "echo -n ''"
compare_with_bash "echo -n: dash text" "echo -n -- hello"
compare_with_bash "echo -n: -n as text" "echo -n -n"

echo ""

# ========================================
# Testes de edge cases
# ========================================
echo -e "${BLUE}Edge Case Tests:${NC}"

compare_with_bash "echo: just dash" "echo -"
compare_with_bash "echo: double dash" "echo --"
compare_with_bash "echo: -n-" "echo -n-"
compare_with_bash "echo: text starting with -n" "echo -notaflag"
compare_with_bash "echo: -n in middle" "echo hello -n world"
compare_with_bash "echo: multiple args with -n" "echo arg1 arg2 -n arg3"

echo ""

# ========================================
# Testes com espaços em branco
# ========================================
echo -e "${BLUE}Whitespace Tests:${NC}"

run_test "echo: preserve single spaces" "echo a b c" "a b c"
run_test "echo: newline at end" "echo test" "test"
run_test "echo -n: no newline" "echo -n test" "test"

echo ""

# ========================================
# Resultados
# ========================================
echo -e "${BLUE}════════════════════════════════════════${NC}"
echo -e "Total Tests: $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo -e "${BLUE}════════════════════════════════════════${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}All tests passed! ✓${NC}\n"
    exit 0
else
    echo -e "\n${RED}Some tests failed! ✗${NC}\n"
    exit 1
fi
