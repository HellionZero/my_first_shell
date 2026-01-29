#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [ -z "$SHELL" ]; then
	if [ -x "$SCRIPT_DIR/../minishell" ]; then
		SHELL="$SCRIPT_DIR/../minishell"
	elif [ -x "$PWD/../minishell" ]; then
		SHELL="$PWD/../minishell"
	elif [ -x "$PWD/minishell" ]; then
		SHELL="$PWD/minishell"
	else
		echo "minishell binary not found" >&2
		exit 1
	fi
fi
if [ -z "$SHELL_PATH" ]; then
	SHELL_PATH="$SHELL"
fi

# Test suite for external command execution

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

PASSED=0
FAILED=0

test_command() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    output=$(echo -e "$input\nexit" | NO_COLOR=1 $SHELL 2>&1 | \
        sed 's/\[minishell\]> //g' | \
        grep -v "^exit$" | \
        grep -v "^$")
    
    if echo "$output" | grep -q "$expected"; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗${NC} $test_name"
        echo "   Expected pattern: $expected"
        echo "   Got: $output"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

test_exit_code() {
    local test_name="$1"
    local input="$2"
    local expected_code="$3"
    
    echo -e "$input" | $SHELL > /dev/null 2>&1
    actual_code=$?
    
    if [ $actual_code -eq $expected_code ]; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗${NC} $test_name"
        echo "   Expected exit code: $expected_code"
        echo "   Got: $actual_code"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

echo ""
echo "========================================="
echo "   External Commands Test Suite"
echo "========================================="
echo ""

# Basic External Commands
echo -e "${BLUE}Basic External Commands:${NC}"
test_command "ls command" "ls /tmp\nexit" "."
test_command "cat command" "echo test | cat\nexit" "test"
test_command "grep command" "echo hello | grep hello\nexit" "hello"
test_command "wc command" "echo line1 | wc -l\nexit" "1"

# Commands with Arguments
echo ""
echo -e "${BLUE}Commands with Arguments:${NC}"
test_command "ls with args" "ls -la /tmp\nexit" "drwx"
test_command "echo piped to grep" "echo abcdef | grep abc\nexit" "abc"

# PATH Resolution
echo ""
echo -e "${BLUE}PATH Resolution:${NC}"
test_command "/bin/echo absolute path" "/bin/echo test\nexit" "test"
test_command "which command" "which ls\nexit" "/bin/ls"

# Error Cases
echo ""
echo -e "${BLUE}Error Cases:${NC}"
test_exit_code "command not found" "nonexistentcommand123\nexit" 127
test_command "invalid command error" "invalidcmd456\nexit" "command not found"

# Multiple Commands
echo ""
echo -e "${BLUE}Sequential Commands:${NC}"
test_command "multiple ls commands" "ls /tmp\nls /home\nexit" "."
test_command "mixed builtin and external" "pwd\nls /tmp\necho done\nexit" "done"

# Summary
echo ""
echo "========================================="
echo "              SUMMARY"
echo "========================================="
echo -e "Total Tests: $((PASSED + FAILED))"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All external command tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
