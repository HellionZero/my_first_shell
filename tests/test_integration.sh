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

# Integration tests for my_shell
# Tests complete workflows and edge cases

SHELL="../minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

PASSED=0
FAILED=0

print_result() {
    if [ "$1" = "PASS" ]; then
        echo -e "${GREEN}✓${NC} $2"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $2"
        FAILED=$((FAILED + 1))
    fi
}

echo "========================================="
echo "   Integration Tests"
echo "========================================="
echo ""

# Test 1: Exit status propagation
test_exit_status() {
    echo -e "exit 123" | timeout 2 $SHELL >/dev/null 2>&1
    local code=$?
    
    if [ $code -eq 123 ]; then
        print_result "PASS" "Exit status propagation (123)"
    else
        print_result "FAIL" "Exit status (expected 123, got $code)"
    fi
}

# Test 2: Whitespace in exit command
test_whitespace() {
    echo -e "   exit   42   " | timeout 2 $SHELL >/dev/null 2>&1
    local code=$?
    
    if [ $code -eq 42 ]; then
        print_result "PASS" "Whitespace handling in exit"
    else
        print_result "FAIL" "Whitespace handling (expected 42, got $code)"
    fi
}

# Test 3: Banner displays
test_banner_display() {
    # Só testa o banner se for terminal interativo
    if [ -t 1 ]; then
        local output
        output=$($SHELL 2>&1 <<<'exit')
        if echo "$output" | grep -q "MINISHELL"; then
            print_result "PASS" "Banner displays on startup"
        else
            print_result "FAIL" "Banner display"
        fi
    else
        echo -e "${YELLOW}⊘${NC} Banner display - skipped (not interactive)"
    fi
}

# Test 4: Exit message shows
test_exit_message() {
    local output=$(echo -e "exit" | timeout 2 $SHELL 2>&1)
    
    if echo "$output" | grep -q "exit"; then
        print_result "PASS" "Exit message displays"
    else
        print_result "FAIL" "Exit message"
    fi
}

# Test 5: EOF handling
test_eof_handling() {
    local output=$(echo "" | timeout 2 $SHELL 2>&1)
    
    if echo "$output" | grep -q "exit"; then
        print_result "PASS" "EOF (Ctrl+D) handled correctly"
    else
        print_result "FAIL" "EOF handling"
    fi
}

# Test 6: Large exit code
test_large_exit_code() {
    echo -e "exit 1000" | timeout 2 $SHELL >/dev/null 2>&1
    local code=$?
    
    # 1000 % 256 = 232
    if [ $code -eq 232 ]; then
        print_result "PASS" "Large exit code modulo (1000 -> 232)"
    else
        print_result "FAIL" "Large exit code (expected 232, got $code)"
    fi
}

# Test 7: Zero exit code
test_zero_exit() {
    echo -e "exit 0" | timeout 2 $SHELL >/dev/null 2>&1
    local code=$?
    
    if [ $code -eq 0 ]; then
        print_result "PASS" "Zero exit code"
    else
        print_result "FAIL" "Zero exit code (got $code)"
    fi
}

# Test 8: Sequential exits
test_sequential_runs() {
    for i in {1..3}; do
        echo -e "exit $i" | timeout 2 $SHELL >/dev/null 2>&1
        if [ $? -ne $i ]; then
            print_result "FAIL" "Sequential runs (run $i)"
            return
        fi
    done
    print_result "PASS" "Sequential shell runs (3x)"
}

test_exit_status
test_whitespace
test_banner_display
test_exit_message
test_eof_handling
test_large_exit_code
test_zero_exit
test_sequential_runs

echo ""
echo "Summary: $PASSED passed, $FAILED failed"

if [ $FAILED -eq 0 ]; then
    exit 0
else
    exit 1
fi
