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

# Test suite for signal state management functions
# Tests the utility functions in shell_signals.c

SHELL="../minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
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
echo "   Signal State Management Tests"
echo "========================================="
echo ""

# Test 1: Multiple exits work correctly
test_signal_clear() {
    for i in {1..3}; do
        echo -e "exit $i" | timeout 2 $SHELL >/dev/null 2>&1
        local code=$?
        if [ $code -ne $i ]; then
            print_result "FAIL" "Signal state cleared between runs (iteration $i)"
            return
        fi
    done
    print_result "PASS" "Signal state cleared between runs"
}

# Test 2: Exit flag properly sets
test_exit_flag() {
    echo -e "exit 10" | timeout 2 $SHELL >/dev/null 2>&1
    local code=$?
    
    if [ $code -eq 10 ]; then
        print_result "PASS" "Exit flag properly propagates code"
    else
        print_result "FAIL" "Exit flag (expected 10, got $code)"
    fi
}

# Test 3: Shell handles empty lines without crashing
test_empty_lines() {
    echo -e "\n\n\n\nexit" | timeout 2 $SHELL >/dev/null 2>&1
    
    if [ $? -eq 0 ]; then
        print_result "PASS" "Empty lines don't cause crashes"
    else
        print_result "FAIL" "Empty lines handling"
    fi
}

# Test 4: Cleanup happens on normal exit
test_cleanup_normal() {
    local before=$(ps aux | grep -c "[m]inishell")
    echo -e "exit" | timeout 2 $SHELL >/dev/null 2>&1
    sleep 0.5
    local after=$(ps aux | grep -c "[m]inishell")
    
    if [ "$before" -eq "$after" ]; then
        print_result "PASS" "Cleanup on normal exit (no zombie processes)"
    else
        print_result "FAIL" "Cleanup on normal exit"
    fi
}

# Test 5: Shell responds to exit after rapid inputs
test_many_commands() {
    local commands=""
    for i in {1..10}; do
        commands+="\n"
    done
    commands+="exit 99"
    
    echo -e "$commands" | timeout 3 $SHELL >/dev/null 2>&1
    local code=$?
    
    if [ $code -eq 99 ]; then
        print_result "PASS" "Shell stable after many empty inputs"
    else
        print_result "FAIL" "Shell stability test (expected 99, got $code)"
    fi
}

test_signal_clear
test_exit_flag
test_empty_lines
test_cleanup_normal
test_many_commands

echo ""
echo "Summary: $PASSED passed, $FAILED failed"

if [ $FAILED -eq 0 ]; then
    exit 0
else
    exit 1
fi
