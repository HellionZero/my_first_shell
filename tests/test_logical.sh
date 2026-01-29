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

# Test suite for logical operators (&& and ||)

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

SHELL="$(cd .. && pwd)/minishell"
PASSED=0
FAILED=0

test_logical() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    # Get output in single line for pattern matching
    output=$(echo -e "$input\nexit" | NO_COLOR=1 $SHELL 2>&1 | \
        sed 's/\[minishell\]> //g' | \
        grep -v "^exit$" | \
        grep -v "^$" | \
        tr '\n' ' ')
    
    if echo "$output" | grep -qE "$expected"; then
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

test_no_output() {
    local test_name="$1"
    local input="$2"
    local forbidden="$3"
    
    output=$(echo -e "$input\nexit" | NO_COLOR=1 $SHELL 2>&1 | \
        sed 's/\[minishell\]> //g' | \
        grep -v "^exit$" | \
        grep -v "^$" | \
        tr '\n' ' ')
    
    if ! echo "$output" | grep -q "$forbidden"; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗${NC} $test_name"
        echo "   Should NOT contain: $forbidden"
        echo "   Got: $output"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

echo ""
echo "========================================="
echo "   Logical Operators Test Suite"
echo "========================================="
echo ""

# AND (&&) Tests - Basic
echo -e "${BLUE}AND (&&) Operator - Basic Tests:${NC}"
test_logical "AND - both succeed" "echo A && echo B\nexit" "A.*B"
test_logical "AND - first fails" "invalidcmd123 && echo B\nexit" "command not found"
test_no_output "AND - second doesn't execute on fail" "invalidcmd123 && echo SHOULD_NOT_APPEAR\nexit" "SHOULD_NOT_APPEAR"
test_logical "AND - multiple commands" "echo A && echo B && echo C\nexit" "A.*B.*C"

# OR (||) Tests - Basic
echo ""
echo -e "${BLUE}OR (||) Operator - Basic Tests:${NC}"
test_logical "OR - first fails, second executes" "invalidcmd456 || echo fallback\nexit" "fallback"
test_logical "OR - first succeeds" "echo success || echo B\nexit" "success"
test_no_output "OR - second doesn't execute on success" "echo SUCCESS || echo SHOULD_NOT_APPEAR\nexit" "SHOULD_NOT_APPEAR"
test_logical "OR - multiple fallbacks" "false || false || echo third\nexit" "third"

# AND with Builtins
echo ""
echo -e "${BLUE}AND with Builtins:${NC}"
test_logical "AND - pwd and echo" "pwd && echo success\nexit" "my_shell.*success"
test_logical "AND - cd and pwd" "cd /tmp && pwd\nexit" "/tmp"
test_logical "AND - export and env" "export TESTVAR=value && env | grep TESTVAR\nexit" "TESTVAR=value"
test_logical "AND - echo with flags" "echo -n test && echo works\nexit" "testworks"

# OR with Builtins
echo ""
echo -e "${BLUE}OR with Builtins:${NC}"
test_logical "OR - cd fail fallback" "cd /nonexistent_dir_12345 || echo fallback\nexit" "fallback"
test_logical "OR - pwd success" "pwd || echo FAIL\nexit" "my_shell"
# AND with Pipes
echo ""
echo -e "${BLUE}AND with Pipes:${NC}"
test_logical "AND - pipe then and" "echo test | cat && echo success\nexit" "test.*success"
test_logical "AND - and then pipe" "echo A && echo B | cat\nexit" "A.*B"
test_logical "AND - complex pipe chain" "cd .. && pwd && ls | grep Makefile && echo final\nexit" "my_shell.*Makefile.*final"

# OR with Pipes
echo ""
echo -e "${BLUE}OR with Pipes:${NC}"
test_logical "OR - pipe success no fallback" "echo test | cat || echo FAIL\nexit" "test"
test_no_output "OR - pipe success no fallback check" "echo test | cat || echo SHOULD_NOT_APPEAR\nexit" "SHOULD_NOT_APPEAR"

# AND with Redirections
echo ""
echo -e "${BLUE}AND with Redirections:${NC}"
test_logical "AND - output redirect" "echo test > /tmp/and_test.txt && cat /tmp/and_test.txt\nexit" "test"
test_logical "AND - append redirect" "echo line1 > /tmp/append_and.txt && echo line2 >> /tmp/append_and.txt && cat /tmp/append_and.txt\nexit" "line1.*line2"
test_logical "AND - input redirect" "echo content > /tmp/input_and.txt && cat < /tmp/input_and.txt && echo done\nexit" "content.*done"

# OR with Redirections
echo ""
echo -e "${BLUE}OR with Redirections:${NC}"
test_no_output "OR - redirect success" "echo ok > /tmp/or_test.txt || echo FAIL\nexit" "FAIL"
test_logical "OR - redirect then fallback" "cat /nonexistent_file_xyz || echo fallback\nexit" "fallback"

# Mixed AND and OR
echo ""
echo -e "${BLUE}Mixed AND and OR:${NC}"
test_logical "Mixed - success AND fail OR fallback" "echo A && invalidcmd || echo C\nexit" "A.*command not found.*C"
test_logical "Mixed - fail AND skip OR execute" "invalidcmd && echo SKIP || echo execute\nexit" "command not found.*execute"
test_logical "Mixed - success OR skip AND execute" "echo A || echo SKIP && echo B\nexit" "A.*B"
test_logical "Mixed - complex chain" "echo start && invalid || echo fallback && echo end\nexit" "start.*command not found.*fallback.*end"

# AND/OR with Heredoc
echo ""
echo -e "${BLUE}AND/OR with Heredoc:${NC}"
test_logical "AND - heredoc success" "cat << EOF && echo success\ntest\nEOF\nexit" "test.*success"
test_logical "AND - heredoc pipe and" "cat << EOF | grep test && echo found\nhello\ntest line\nEOF\nexit" "test line.*found"

# Edge Cases
echo ""
echo -e "${BLUE}Edge Cases:${NC}"
test_logical "Multiple ANDs in sequence" "echo 1 && echo 2 && echo 3 && echo 4\nexit" "1.*2.*3.*4"
test_logical "Multiple ORs in sequence" "false || false || false || echo final\nexit" "final"
test_logical "true and false commands" "true && echo works\nexit" "works"
test_logical "false with OR" "false || true && echo success\nexit" "success"

# Cleanup temporary files
rm -f /tmp/and_test.txt /tmp/append_and.txt /tmp/input_and.txt /tmp/or_test.txt 2>/dev/null

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
    echo -e "${GREEN}✓ All logical operator tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
