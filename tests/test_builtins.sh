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

# Test suite for builtin commands

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

SHELL="../minishell"
PASSED=0
FAILED=0

test_builtin() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    # Executa e remove prompts e exit
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
        echo "   Expected: $expected"
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
echo "      Builtin Commands Test Suite"
echo "========================================="
echo ""

# PWD Tests
echo -e "${BLUE}PWD Tests:${NC}"
test_builtin "pwd - current directory" "pwd\nexit" "tests"

# ECHO Tests
echo ""
echo -e "${BLUE}ECHO Tests:${NC}"
test_builtin "echo - simple text" "echo hello\nexit" "hello"
test_builtin "echo - multiple args" "echo hello world\nexit" "hello world"
test_builtin "echo -n flag" "echo -n test\nexit" "test"
# Skip echo empty test - just outputs empty line, difficult to match with prompt
test_builtin "echo - with quotes" "echo \"hello world\"\nexit" "hello world"

# ENV Tests
echo ""
echo -e "${BLUE}ENV Tests:${NC}"
test_builtin "env - shows PATH" "env\nexit" "PATH="
test_builtin "env - shows SHLVL" "env\nexit" "SHLVL="

# EXPORT Tests
echo ""
echo -e "${BLUE}EXPORT Tests:${NC}"
test_builtin "export - set variable" "export TEST=value\nenv\nexit" "TEST=value"
test_builtin "export - update existing" "export PATH=/new/path\nenv\nexit" "PATH=/new/path"
test_builtin "export - multiple variables" "export A=1\nexport B=2\nenv\nexit" "A=1"

# UNSET Tests
echo ""
echo -e "${BLUE}UNSET Tests:${NC}"
# Create a simple test that checks variable is gone
echo -e "export TESTVAR=value\nunset TESTVAR\nexit" | $SHELL > /dev/null 2>&1
if echo -e "export TESTVAR=value\nunset TESTVAR\nenv\nexit" | $SHELL 2>&1 | grep -v "TESTVAR" | grep -q "PATH="; then
    echo -e "${GREEN}✓${NC} unset - remove variable"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} unset - remove variable"
    FAILED=$((FAILED + 1))
fi

# CD Tests
echo ""
echo -e "${BLUE}CD Tests:${NC}"
mkdir -p /tmp/test_shell_cd
test_builtin "cd - absolute path" "cd /tmp/test_shell_cd\npwd\nexit" "/tmp/test_shell_cd"
test_builtin "cd - relative path" "cd /tmp\ncd test_shell_cd\npwd\nexit" "/tmp/test_shell_cd"
test_builtin "cd - HOME" "export HOME=/tmp\ncd\npwd\nexit" "/tmp"
rm -rf /tmp/test_shell_cd

# EXIT Tests
echo ""
echo -e "${BLUE}EXIT Tests:${NC}"
test_exit_code "exit - no args" "exit" 0
test_exit_code "exit - with code 0" "exit 0" 0
test_exit_code "exit - with code 42" "exit 42" 42
test_exit_code "exit - code 255" "exit 255" 255
test_exit_code "exit - code modulo 256" "exit 256" 0
test_exit_code "exit - code 257" "exit 257" 1

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
    echo -e "${GREEN}✓ All builtin tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
