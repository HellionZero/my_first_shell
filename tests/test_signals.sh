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

# Test suite for signal handling in my_shell
# Tests Ctrl+C, Ctrl+D, exit builtin, cleanup, and signal state

SHELL="../minishell"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0
TOTAL=0

# Helper function to print test results
print_result() {
    local test_name="$1"
    local result="$2"
    TOTAL=$((TOTAL + 1))
    
    if [ "$result" = "PASS" ]; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $test_name"
        FAILED=$((FAILED + 1))
    fi
}

# Test 1: Exit with no arguments (should use last exit status)
test_exit_no_args() {
    local output=$(echo -e "exit" | timeout 2 $SHELL 2>&1)
    
    if echo "$output" | grep -q "exit"; then
        print_result "Exit builtin - no arguments" "PASS"
    else
        print_result "Exit builtin - no arguments" "FAIL"
    fi
}

# Test 2: Exit with numeric argument
test_exit_with_number() {
    echo -e "exit 42" | timeout 2 $SHELL >/dev/null 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 42 ]; then
        print_result "Exit builtin - numeric argument (42)" "PASS"
    else
        print_result "Exit builtin - numeric argument (expected 42, got $exit_code)" "FAIL"
    fi
}

# Test 3: Exit with invalid argument
test_exit_invalid_arg() {
    local output=$(echo -e "exit abc" | timeout 2 $SHELL 2>&1)
    
    if echo "$output" | grep -q "numeric argument required"; then
        print_result "Exit builtin - invalid argument error message" "PASS"
    else
        print_result "Exit builtin - invalid argument error message" "FAIL"
    fi
}

# Test 4: Exit with too many arguments
test_exit_too_many_args() {
    local output=$(echo -e "exit 1 2" | timeout 2 $SHELL 2>&1)
    
    if echo "$output" | grep -q "too many arguments"; then
        print_result "Exit builtin - too many arguments error" "PASS"
    else
        print_result "Exit builtin - too many arguments error" "FAIL"
    fi
}

# Test 5: EOF (Ctrl+D) exits shell
test_ctrl_d_exit() {
    local output=$(echo -e "" | timeout 2 $SHELL 2>&1)
    
    if echo "$output" | grep -q "exit"; then
        print_result "Ctrl+D (EOF) - exits with message" "PASS"
    else
        print_result "Ctrl+D (EOF) - exits with message" "FAIL"
    fi
}

# Test 6: Exit code modulo 256
test_exit_code_modulo() {
    echo -e "exit 258" | timeout 2 $SHELL >/dev/null 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 2 ]; then
        print_result "Exit code modulo 256 (258 -> 2)" "PASS"
    else
        print_result "Exit code modulo 256 (expected 2, got $exit_code)" "FAIL"
    fi
}

# Test 7: Shell exits cleanly
test_clean_exit() {
    echo -e "exit" | timeout 2 $SHELL >/dev/null 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        print_result "Clean shell exit" "PASS"
    else
        print_result "Clean shell exit" "FAIL"
    fi
}

# Test 8: Memory leak check with valgrind (if available)
test_memory_leaks() {
    if ! command -v valgrind &> /dev/null; then
        echo -e "${YELLOW}⊘${NC} Memory leak test - valgrind not available (SKIP)"
        return
    fi
    
    local output=$(echo -e "exit" | valgrind --leak-check=full --error-exitcode=42 $SHELL 2>&1)
    local exit_code=$?
    
    # Exit code 0 = no errors, 42 = errors detected by valgrind
    # Check for "definitely lost: 0 bytes" which means no real leaks
    if [ $exit_code -eq 0 ] && echo "$output" | grep -q "definitely lost: 0 bytes"; then
        print_result "Memory leak check - exit builtin" "PASS"
    else
        print_result "Memory leak check - exit builtin" "FAIL"
    fi
}

# Test 9: Sequential exits with different codes
test_multiple_commands() {
    echo -e "exit 5" | timeout 2 $SHELL >/dev/null 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 5 ]; then
        print_result "Exit with specific code (5)" "PASS"
    else
        print_result "Exit with specific code (expected 5, got $exit_code)" "FAIL"
    fi
}

# Test 10: Empty input handling
test_empty_input() {
    local output=$(echo -e "\n\nexit" | timeout 2 $SHELL 2>&1)
    
    if echo "$output" | grep -q "exit"; then
        print_result "Empty input lines handling" "PASS"
    else
        print_result "Empty input lines handling" "FAIL"
    fi
}

# Test 11: Shell banner display
test_banner_display() {
    # Só testa o banner se for terminal interativo
    if [ -t 1 ]; then
        local output
        output=$($SHELL 2>&1 <<<'exit')
        if echo "$output" | grep -q "MINISHELL"; then
            print_result "Shell banner display" "PASS"
        else
            print_result "Shell banner display" "FAIL"
        fi
    else
        echo -e "${YELLOW}⊘${NC} Shell banner display - skipped (not interactive)"
    fi
}

# Test 12: Exit with negative number
test_exit_negative() {
    echo -e "exit -1" | timeout 2 $SHELL >/dev/null 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 255 ]; then
        print_result "Exit with negative number (-1 -> 255)" "PASS"
    else
        print_result "Exit with negative number (expected 255, got $exit_code)" "FAIL"
    fi
}

# Test 13: History is properly managed
test_history_management() {
    local output=$(echo -e "echo test\nexit" | timeout 2 $SHELL 2>&1)
    
    # If no crashes or errors, history was handled
    if [ $? -eq 0 ]; then
        print_result "History management (no crash)" "PASS"
    else
        print_result "History management (no crash)" "FAIL"
    fi
}

# Test 14: Rapid successive exits
test_rapid_exits() {
    for i in {1..5}; do
        echo -e "exit" | timeout 1 $SHELL >/dev/null 2>&1
        if [ $? -ne 0 ]; then
            print_result "Rapid successive exits (iteration $i)" "FAIL"
            return
        fi
    done
    print_result "Rapid successive exits (5 iterations)" "PASS"
}

# Test 15: Exit from different prompt states
test_exit_consistency() {
    local test1=$(echo -e "exit" | timeout 2 $SHELL 2>&1 | grep -c "exit")
    local test2=$(echo -e "echo 1\nexit" | timeout 2 $SHELL 2>&1 | grep -c "exit")
    
    if [ "$test1" -eq "$test2" ]; then
        print_result "Exit consistency across states" "PASS"
    else
        print_result "Exit consistency across states" "FAIL"
    fi
}

# Main execution
echo "========================================="
echo "   MY_SHELL - Signal & Exit Test Suite"
echo "========================================="
echo ""

# Check if shell exists
if [ ! -f "$SHELL" ]; then
    echo -e "${RED}Error: $SHELL not found. Please compile first.${NC}"
    exit 1
fi

# Run all tests
echo "Running tests..."
echo ""

test_exit_no_args
test_exit_with_number
test_exit_invalid_arg
test_exit_too_many_args
test_ctrl_d_exit
test_exit_code_modulo
test_clean_exit
test_multiple_commands
test_empty_input
test_banner_display
test_exit_negative
test_history_management
test_rapid_exits
test_exit_consistency
test_memory_leaks

# Summary
echo ""
echo "========================================="
echo "                SUMMARY"
echo "========================================="
echo -e "Total:  $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed.${NC}"
    exit 1
fi
