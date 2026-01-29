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

# Test suite for redirections (<, >, >>, <<)
# Note: Currently only external commands support redirections
# Builtins with redirections will be tested separately

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

SHELL="$(cd .. && pwd)/minishell"
PASSED=0
FAILED=0
TEST_DIR="/tmp/test_shell_redirects"

# Setup test directory
setup() {
    mkdir -p "$TEST_DIR"
    cd "$TEST_DIR"
    echo "test content" > input.txt
    echo "line1" > multi.txt
    echo "line2" >> multi.txt
}

# Cleanup test directory
cleanup() {
    rm -rf "$TEST_DIR"
}

test_redirect() {
    local test_name="$1"
    local input="$2"
    local check_cmd="$3"
    
    # Execute shell from TEST_DIR so relative paths work
    (cd "$TEST_DIR" && echo -e "$input" | $SHELL > /dev/null 2>&1)
    
    if eval "$check_cmd"; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗${NC} $test_name"
        echo "   Check failed: $check_cmd"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

echo ""
echo "========================================="
echo "      Redirection Test Suite"
echo "========================================="
echo ""

setup

# Output Redirection (>)
echo -e "${BLUE}Output Redirection (>) Tests:${NC}"
test_redirect "redirect output - basic" \
    "echo hello > $TEST_DIR/out1.txt\nexit" \
    "grep -q 'hello' $TEST_DIR/out1.txt"

test_redirect "redirect output - overwrite" \
    "echo first > $TEST_DIR/out2.txt\necho second > $TEST_DIR/out2.txt\nexit" \
    "[ \$(cat $TEST_DIR/out2.txt | wc -l) -eq 1 ] && grep -q 'second' $TEST_DIR/out2.txt"

test_redirect "redirect output - empty" \
    "echo -n > $TEST_DIR/out3.txt\nexit" \
    "[ -f $TEST_DIR/out3.txt ] && [ ! -s $TEST_DIR/out3.txt ]"

# Append Redirection (>>)
echo ""
echo -e "${BLUE}Append Redirection (>>) Tests:${NC}"
test_redirect "append output - basic" \
    "echo line1 > $TEST_DIR/append1.txt\necho line2 >> $TEST_DIR/append1.txt\nexit" \
    "[ \$(cat $TEST_DIR/append1.txt | wc -l) -eq 2 ]"

test_redirect "append output - multiple" \
    "echo a > $TEST_DIR/append2.txt\necho b >> $TEST_DIR/append2.txt\necho c >> $TEST_DIR/append2.txt\nexit" \
    "[ \$(cat $TEST_DIR/append2.txt | wc -l) -eq 3 ]"

test_redirect "append to new file" \
    "echo test >> $TEST_DIR/append3.txt\nexit" \
    "grep -q 'test' $TEST_DIR/append3.txt"

# Input Redirection (<)
echo ""
echo -e "${BLUE}Input Redirection (<) Tests:${NC}"
test_redirect "input redirect - basic" \
    "cat < $TEST_DIR/input.txt > $TEST_DIR/cat_out.txt\nexit" \
    "grep -q 'test content' $TEST_DIR/cat_out.txt"

test_redirect "input redirect - with command" \
    "grep 'content' < $TEST_DIR/input.txt > $TEST_DIR/grep_out.txt\nexit" \
    "grep -q 'test content' $TEST_DIR/grep_out.txt"

# Heredoc (<<)
echo ""
echo -e "${BLUE}Heredoc (<<) Tests:${NC}"
test_redirect "heredoc - basic" \
    "cat << EOF > $TEST_DIR/heredoc1.txt\nhello\nworld\nEOF\nexit" \
    "grep -q 'hello' $TEST_DIR/heredoc1.txt && grep -q 'world' $TEST_DIR/heredoc1.txt"
test_redirect "heredoc - different delimiter" \
    "cat << END > $TEST_DIR/heredoc2.txt\ntest\nEND\nexit" \
    "grep -q 'test' $TEST_DIR/heredoc2.txt"
test_redirect "heredoc - multiple lines" \
    "cat << MARKER > $TEST_DIR/heredoc3.txt\nline1\nline2\nline3\nMARKER\nexit" \
    "[ \$(cat $TEST_DIR/heredoc3.txt | wc -l) -eq 3 ]"

# Multiple Redirections
echo ""
echo -e "${BLUE}Multiple Redirections Tests:${NC}"
test_redirect "multiple outputs" \
    "echo test > $TEST_DIR/multi1.txt\necho test2 > $TEST_DIR/multi2.txt\nexit" \
    "grep -q 'test' $TEST_DIR/multi1.txt && grep -q 'test2' $TEST_DIR/multi2.txt"

test_redirect "input and output" \
    "cat < $TEST_DIR/input.txt > $TEST_DIR/inout.txt\nexit" \
    "grep -q 'test content' $TEST_DIR/inout.txt"

# Redirections with Pipes
echo ""
echo -e "${BLUE}Redirections with Pipes Tests:${NC}"
test_redirect "pipe with output redirect" \
    "echo hello | cat > $TEST_DIR/pipe1.txt\nexit" \
    "grep -q 'hello' $TEST_DIR/pipe1.txt"

test_redirect "input redirect with pipe" \
    "cat < $TEST_DIR/multi.txt | grep line1 > $TEST_DIR/pipe2.txt\nexit" \
    "grep -q 'line1' $TEST_DIR/pipe2.txt"

cleanup

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
    echo -e "${GREEN}✓ All redirection tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
