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

# Heredoc Test Suite - Tests heredoc parsing and FD management

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

# Helper function to test heredoc parsing
test_heredoc() {
    local input="$1"
    local expected="$2"
    local description="$3"
    
    print_test "$description"
    
    # Run with DEBUG=1 to get AST output showing heredoc info
    output=$(echo -e "$input\nexit" | DEBUG=1 ../minishell 2>&1 | grep -A 30 "=== ABSTRACT SYNTAX TREE ===" | head -n 20)
    
    if echo "$output" | grep -q "$expected"; then
        print_pass
    else
        print_fail "Expected pattern '$expected' not found"
        echo "Output preview:"
        echo "$output" | head -n 5
    fi
}

# Helper function to test heredoc errors
test_heredoc_error() {
    local input="$1"
    local description="$2"
    
    print_test "$description"
    
    # Should produce syntax error
    output=$(echo -e "$input\nexit" | ../minishell 2>&1)
    
    if echo "$output" | grep -qi "syntax error\|parse error"; then
        print_pass
    else
        print_fail "Expected syntax error not found"
    fi
}

echo ""
echo "========================================="
echo "       HEREDOC TEST SUITE"
echo "========================================="
echo ""

# Category 1: Basic Heredoc
echo -e "${BLUE}=== Basic Heredoc ===${NC}"
test_heredoc "cat << EOF" "<<.*EOF" "Simple heredoc delimiter"
test_heredoc "cat << END" "<<.*END" "Alternative delimiter"
test_heredoc "cat << E" "<<.*E" "Single char delimiter"

# Category 2: Heredoc with Quotes
echo ""
echo -e "${BLUE}=== Heredoc with Quotes ===${NC}"
test_heredoc "cat << 'EOF'" "<<.*EOF" "Single quoted delimiter"
test_heredoc "cat << \"EOF\"" "<<.*EOF" "Double quoted delimiter"

# Category 3: Heredoc FD Management
echo ""
echo -e "${BLUE}=== Heredoc FD Management ===${NC}"
test_heredoc "cat << EOF" "fd:" "FD field present"
test_heredoc "cat << EOF" "fd:-1\|fd:[0-9]" "FD initialized"

# Category 4: Multiple Heredocs
echo ""
echo -e "${BLUE}=== Multiple Heredocs ===${NC}"
test_heredoc "cat << EOF << END" "<<.*EOF.*<<.*END" "Two heredocs in sequence"
test_heredoc "cat << EOF > out.txt" "<<.*EOF.*>.*out.txt" "Heredoc with output redirect"

# Category 5: Heredoc in Pipes
echo ""
echo -e "${BLUE}=== Heredoc in Pipes ===${NC}"
test_heredoc "cat << EOF | grep test" "Node: PIPE" "Heredoc before pipe - pipe exists"
test_heredoc "cat << EOF | grep test" "Redirects:.*EOF" "Heredoc before pipe - heredoc exists"
test_heredoc "echo test | cat << EOF" "Node: PIPE" "Heredoc after pipe - pipe exists"
test_heredoc "echo test | cat << EOF" "Redirects:.*EOF" "Heredoc after pipe - heredoc exists"

# Category 6: Heredoc with Logical Operators
echo ""
echo -e "${BLUE}=== Heredoc with Logical Operators ===${NC}"
test_heredoc "cat << EOF && echo done" "Node: AND" "Heredoc with AND - operator exists"
test_heredoc "cat << EOF && echo done" "Redirects:.*EOF" "Heredoc with AND - heredoc exists"
test_heredoc "cat << EOF || echo error" "Node: OR" "Heredoc with OR - operator exists"
test_heredoc "cat << EOF || echo error" "Redirects:.*EOF" "Heredoc with OR - heredoc exists"

# Category 7: Error Cases
echo ""
echo -e "${BLUE}=== Heredoc Error Cases ===${NC}"
test_heredoc_error "cat << ''" "Empty literal delimiter"
test_heredoc_error "cat <<" "Missing delimiter"

# Final summary
echo ""
echo "========================================="
echo "           TEST SUMMARY"
echo "========================================="
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All heredoc tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
