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

# Test suite for detecting segfaults in edge cases

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

SHELL="$(dirname "$0")/../minishell"
PASSED=0
FAILED=0
SEGFAULTS=0

echo "========================================="
echo "       Segfault Detection Suite"
echo "========================================="
echo ""

test_segfault() {
    local test_name="$1"
    local input="$2"
    local timeout_duration="${3:-2}"
    
    # Run in timeout to prevent hanging
    output=$(timeout "$timeout_duration" bash -c "echo -e '$input' | $SHELL 2>&1")
    exit_code=$?
    
    # Exit codes: 139 = segfault (128+11), 124 = timeout
    if [ $exit_code -eq 139 ] || [ $exit_code -eq 11 ]; then
        echo -e "${RED}✗ SEGFAULT${NC} $test_name"
        echo -e "   Input: ${YELLOW}$input${NC}"
        FAILED=$((FAILED + 1))
        SEGFAULTS=$((SEGFAULTS + 1))
        return 1
    elif [ $exit_code -eq 124 ]; then
        echo -e "${YELLOW}⚠ TIMEOUT${NC} $test_name (likely hanging)"
        echo -e "   Input: ${YELLOW}$input${NC}"
        FAILED=$((FAILED + 1))
        return 1
    else
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
        return 0
    fi
}

echo -e "${BLUE}Empty and NULL Tests:${NC}"
test_segfault "empty input" ""
test_segfault "only newlines" "\n\n\n"
test_segfault "only spaces" "     "
test_segfault "only tabs" "\t\t\t"
test_segfault "exit after empty" "\nexit"

echo ""
echo -e "${BLUE}Pipe Edge Cases:${NC}"
test_segfault "pipe at start" "| cat"
test_segfault "pipe at end" "echo test |"
test_segfault "double pipe" "echo test || cat"
test_segfault "multiple pipes empty" "| | |"
test_segfault "pipe with spaces" "echo   |   cat"
test_segfault "pipe no command left" " | cat"
test_segfault "pipe no command right" "echo | "
test_segfault "many pipes" "echo a | cat | cat | cat | cat | cat | cat"

echo ""
echo -e "${BLUE}Redirect Edge Cases:${NC}"
test_segfault "redirect at start" "> file"
test_segfault "redirect no file" "echo test >"
test_segfault "redirect double" "echo test >> >>"
test_segfault "redirect to nothing" "echo > "
test_segfault "multiple redirects" "echo test > a > b > c"
test_segfault "redirect input no file" "cat <"
test_segfault "heredoc no delimiter" "cat <<"
test_segfault "heredoc empty delimiter" "cat << ''"

echo ""
echo -e "${BLUE}Quote Edge Cases:${NC}"
test_segfault "unclosed single quote" "echo 'test"
test_segfault "unclosed double quote" 'echo "test'
test_segfault "nested quotes unclosed" "echo \"'test\""
test_segfault "only quotes" "''\"\""
test_segfault "empty quotes" "echo '' \"\""

echo ""
echo -e "${BLUE}Variable Edge Cases:${NC}"
test_segfault "undefined variable" "echo \$NONEXISTENT"
test_segfault "dollar alone" "echo $"
test_segfault "double dollar" "echo $$"
test_segfault "dollar with special chars" "echo \$@#%"
test_segfault "variable in quotes" "echo '\$VAR' \"\$VAR\""
test_segfault "nested expansion" "export A=\$B; echo \$A"

echo ""
echo -e "${BLUE}Logical Operators:${NC}"
test_segfault "and at start" "&& echo test"
test_segfault "and at end" "echo test &&"
test_segfault "or at start" "|| echo test"
test_segfault "or at end" "echo test ||"
test_segfault "double and" "echo && && echo"
test_segfault "mixed operators" "echo && || echo"

echo ""
echo -e "${BLUE}Command Edge Cases:${NC}"
test_segfault "command not found" "nonexistentcommand12345"
test_segfault "very long command" "$(printf 'a%.0s' {1..1000})"
test_segfault "special characters" "echo !@#$%^&*()"
test_segfault "only semicolons" ";;;"
test_segfault "multiple semicolons" "echo;;;;;echo"

echo ""
echo -e "${BLUE}Builtin Edge Cases:${NC}"
test_segfault "cd no args" "cd"
test_segfault "cd nonexistent" "cd /nonexistent/path/xyz"
test_segfault "cd too many args" "cd /tmp /home /usr"
test_segfault "export no equals" "export NOVALUE"
test_segfault "export invalid" "export 123=value"
test_segfault "unset nonexistent" "unset NONEXISTENT"
test_segfault "env with args" "env arg1 arg2"
test_segfault "pwd with args" "pwd /some/path"
test_segfault "echo empty" "echo"

echo ""
echo -e "${BLUE}Memory Stress Tests:${NC}"
test_segfault "many variables" "export A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8 I=9 J=10; env" 3
test_segfault "long string" "echo '$(printf 'x%.0s' {1..10000})'" 3
test_segfault "many redirects" "echo test >a >b >c >d >e >f >g >h >i >j" 3
test_segfault "deep pipes" "echo 1|cat|cat|cat|cat|cat|cat|cat|cat|cat|cat" 3

echo ""
echo -e "${BLUE}Combined Edge Cases:${NC}"
test_segfault "pipe and redirect" "echo test | > file"
test_segfault "redirect and pipe" "echo test > | cat"
test_segfault "quote and redirect" "echo 'test > file"
test_segfault "variable and pipe" "echo \$VAR |"
test_segfault "all operators" "echo && echo || echo | cat > file"

echo ""
echo -e "${BLUE}Exit Tests:${NC}"
test_segfault "exit no args" "exit"
test_segfault "exit with number" "exit 42"
test_segfault "exit with text" "exit abc"
test_segfault "exit negative" "exit -1"
test_segfault "exit overflow" "exit 999999999999"

echo ""
echo "========================================="
echo "              SUMMARY"
echo "========================================="
echo -e "Total Tests: $((PASSED + FAILED))"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
if [ $SEGFAULTS -gt 0 ]; then
    echo -e "${RED}⚠ SEGFAULTS DETECTED: $SEGFAULTS${NC}"
    echo ""
    exit 1
elif [ $FAILED -gt 0 ]; then
    echo -e "${YELLOW}⚠ Some tests timed out or failed${NC}"
    echo ""
    exit 1
else
    echo -e "${GREEN}✓ No segfaults detected!${NC}"
    echo ""
    exit 0
fi
