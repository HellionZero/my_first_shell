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

# Parser/AST Test Suite - Tests Abstract Syntax Tree generation

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

PASSED=0
FAILED=0

print_test() {
    echo -e "${BLUE}TEST:${NC} $1"
}

print_pass() {
    echo -e "${GREEN}✓ PASS${NC}"
    PASSED=$((PASSED + 1))
}

print_fail() {
    echo -e "${RED}✗ FAIL${NC} - $1"
    FAILED=$((FAILED + 1))
}

# Helper function to test parser AST output
test_parser() {
    local input="$1"
    local expected="$2"
    local description="$3"
    
    print_test "$description"
    
    # Run with DEBUG=1 to get AST output, then exit
    output=$(echo -e "$input\nexit" | DEBUG=1 ../minishell 2>&1 | grep -A 50 "=== ABSTRACT SYNTAX TREE ===" | head -n 30)
    
    if echo "$output" | grep -q "$expected"; then
        print_pass
    else
        print_fail "Expected pattern '$expected' not found"
        echo "Output preview:"
        echo "$output" | head -n 10
    fi
}

echo ""
echo "========================================="
echo "       PARSER/AST TEST SUITE"
echo "========================================="
echo ""

# Category 1: Simple Commands
echo -e "${BLUE}=== Simple Commands ===${NC}"
test_parser "ls" "Command: \[ls\]" "Single command"
test_parser "ls -la" "Command: \[ls, -la\]" "Command with args"
test_parser "echo hello world" "Command: \[echo, hello, world\]" "Command with multiple args"

# Category 2: Pipes
echo ""
echo -e "${BLUE}=== Pipe Commands ===${NC}"
test_parser "ls | grep" "PIPE" "Simple pipe"
test_parser "cat file | grep test | wc -l" "PIPE" "Multiple pipes"

# Category 3: Redirections
echo ""
echo -e "${BLUE}=== Redirections ===${NC}"
test_parser "cat < input.txt" "Redirects:.*<.*input.txt" "Input redirection"
test_parser "echo hello > output.txt" "Redirects:.*>.*output.txt" "Output redirection"
test_parser "echo hello >> output.txt" "Redirects:.*>>.*output.txt" "Append redirection"

# Category 4: Logical Operators
echo ""
echo -e "${BLUE}=== Logical Operators ===${NC}"
test_parser "echo a && echo b" "AND" "Logical AND"
test_parser "echo a || echo b" "OR" "Logical OR"
test_parser "false || echo backup" "OR" "OR with fallback"

# Category 5: Combined Operators
echo ""
echo -e "${BLUE}=== Combined Operators ===${NC}"
test_parser "cat < in.txt > out.txt" "Redirects:.*<.*in.txt.*>.*out.txt" "Input and output redirect"
test_parser "ls | grep test > result.txt" "Node: PIPE" "Pipe and redirect"
test_parser "cat < in.txt | grep test | wc -l > count.txt" "Node: PIPE" "Complex pipeline"

# Category 6: AST Structure
echo ""
echo -e "${BLUE}=== AST Structure ===${NC}"
test_parser "echo test" "Node: COMMAND" "Command node type"
test_parser "ls | grep" "Node: PIPE" "Pipe node type"
test_parser "echo a && echo b" "Node: AND" "AND node type"
test_parser "echo a || echo b" "Node: OR" "OR node type"

# Final summary
echo ""
echo "========================================="
echo "           TEST SUMMARY"
echo "========================================="
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All parser tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
