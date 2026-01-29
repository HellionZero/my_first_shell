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

# Lexer Test Suite - Tests tokenization of various input patterns

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

# Helper function to test lexer output
test_lexer() {
    local input="$1"
    local expected="$2"
    local description="$3"
    
    print_test "$description"
    
    # Run with DEBUG=1 to get token output, then exit
    output=$(echo -e "$input\nexit" | DEBUG=1 ../minishell 2>&1 | grep -A 100 "=== TOKENS ===" | grep -v "=== TOKENS ===" | grep -v "=== ABSTRACT SYNTAX TREE ===" | head -n 50)
    
    if echo "$output" | grep -E -q "$expected"; then
        print_pass
    else
        print_fail "Expected pattern '$expected' not found"
        echo "Output preview:"
        echo "$output" | head -n 5
    fi
}

echo ""
echo "========================================="
echo "         LEXER TEST SUITE"
echo "========================================="
echo ""

# Category 1: Basic Commands
echo -e "${BLUE}=== Basic Commands ===${NC}"
test_lexer "ls" "\[0\] WORD" "Simple command"
test_lexer "ls -la" "\[1\] WORD" "Command with argument"
test_lexer "echo hello world" "\[2\] WORD" "Multiple arguments"

# Category 2: Operators
echo ""
echo -e "${BLUE}=== Operators ===${NC}"
test_lexer "ls | grep" "PIPE" "Pipe operator"
test_lexer "cat < file.txt" "REDIR_IN" "Input redirection"
test_lexer "echo hello > output.txt" "REDIR_OUT" "Output redirection"
test_lexer "echo hello >> output.txt" "APPEND" "Append redirection"
test_lexer "cat << EOF" "HEREDOC" "Heredoc operator"
test_lexer "echo a && echo b" "AND" "Logical AND"
test_lexer "echo a || echo b" "OR" "Logical OR"

# Category 3: Quotes
echo ""
echo -e "${BLUE}=== Quotes ===${NC}"
test_lexer "echo 'hello'" "SINGLE_QUOTE" "Single quotes"
test_lexer "echo \"hello\"" "DOUBLE_QUOTE" "Double quotes"
test_lexer "echo \"hello \$USER\"" "DOUBLE_QUOTE" "Variable in double quotes"

# Category 4: Variables
echo ""
echo -e "${BLUE}=== Variables ===${NC}"
test_lexer "echo \$USER" "VARIABLE.*USER" "Simple variable"
test_lexer "echo \${HOME}" "VARIABLE.*HOME" "Braced variable"
test_lexer "echo \$?" "VARIABLE.*?" "Special variable (\$?)"
test_lexer "echo \$\$" "VARIABLE.*\\$" "Special variable (\$\$)"
test_lexer "echo \$1" "VARIABLE.*1" "Positional variable"

# Category 5: Word Parts (Composite)
echo ""
echo -e "${BLUE}=== Composite Words ===${NC}"
test_lexer "prefix\$VAR" "LITERAL.*VARIABLE" "Literal + variable"
test_lexer "\$VAR'suffix'" "VARIABLE.*SINGLE_QUOTE" "Variable + single quote"
test_lexer "pre\$VAR'mid'\${TEST}\"end\"" "LITERAL.*VARIABLE.*SINGLE_QUOTE.*VARIABLE.*DOUBLE_QUOTE" "Complex composite"

# Category 6: Edge Cases
echo ""
echo -e "${BLUE}=== Edge Cases ===${NC}"
test_lexer "ls    -la" "\[1\] WORD" "Multiple spaces"
test_lexer "   ls" "\[0\] WORD" "Leading spaces"

# Category 7: Complex Patterns
echo ""
echo -e "${BLUE}=== Complex Patterns ===${NC}"
test_lexer "cat file | grep test | wc -l" "\[5\] PIPE" "Multiple pipes"
test_lexer "cat < in.txt | grep test > out.txt" "REDIR_IN" "Mixed operators - input"
test_lexer "cat < in.txt | grep test > out.txt" "PIPE" "Mixed operators - pipe"
test_lexer "cat < in.txt | grep test > out.txt" "REDIR_OUT" "Mixed operators - output"

# Final summary
echo ""
echo "========================================="
echo "           TEST SUMMARY"
echo "========================================="
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All lexer tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
