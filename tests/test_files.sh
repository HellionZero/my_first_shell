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

# Test suite for file operations and complex redirections

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SHELL="$SCRIPT_DIR/../minishell"
PASSED=0
FAILED=0
TEST_DIR="/tmp/test_shell_files"

setup() {
    mkdir -p "$TEST_DIR"
    cd "$TEST_DIR"
}

cleanup() {
    rm -rf "$TEST_DIR"
}

test_file_op() {
    local test_name="$1"
    local input="$2"
    local check_cmd="$3"
    
    (cd "$TEST_DIR" && echo -e "$input\nexit" | NO_COLOR=1 $SHELL > /dev/null 2>&1)
    
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

test_output() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    output=$(cd "$TEST_DIR" && echo -e "$input\nexit" | NO_COLOR=1 $SHELL 2>&1 | \
        sed 's/\[minishell\]> //g' | \
        grep -v "^exit$" | \
        grep -v "^$")
    
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

echo ""
echo "========================================="
echo "     File Operations Test Suite"
echo "========================================="
echo ""

setup

# Basic File Creation
echo -e "${BLUE}Basic File Creation:${NC}"
test_file_op "create file with echo" "echo hello > file1.txt\nexit" "grep -q 'hello' $TEST_DIR/file1.txt"
test_file_op "create file with cat" "cat > file2.txt << EOF\nline1\nline2\nEOF\nexit" "[ \$(cat $TEST_DIR/file2.txt | wc -l) -eq 2 ]"
test_file_op "create empty file" "> empty.txt\nexit" "bash -c '> $TEST_DIR/empty_bash.txt'; [ -f $TEST_DIR/empty.txt ] && [ -f $TEST_DIR/empty_bash.txt ] && cmp -s $TEST_DIR/empty.txt $TEST_DIR/empty_bash.txt"

# File Overwriting
echo ""
echo -e "${BLUE}File Overwriting:${NC}"
test_file_op "overwrite file" "echo first > over.txt\necho second > over.txt\nexit" "grep -q 'second' $TEST_DIR/over.txt && [ \$(cat $TEST_DIR/over.txt | wc -l) -eq 1 ]"
test_file_op "overwrite with empty" "echo data > over2.txt\n> over2.txt\nexit" "bash -c 'echo data > $TEST_DIR/over2_bash.txt; > $TEST_DIR/over2_bash.txt'; [ -f $TEST_DIR/over2.txt ] && [ -f $TEST_DIR/over2_bash.txt ] && cmp -s $TEST_DIR/over2.txt $TEST_DIR/over2_bash.txt"

# File Appending
echo ""
echo -e "${BLUE}File Appending:${NC}"
test_file_op "append to file" "echo line1 > append.txt\necho line2 >> append.txt\nexit" "[ \$(cat $TEST_DIR/append.txt | wc -l) -eq 2 ]"
test_file_op "append multiple times" "echo a > multi.txt\necho b >> multi.txt\necho c >> multi.txt\nexit" "[ \$(cat $TEST_DIR/multi.txt | wc -l) -eq 3 ]"
test_file_op "append to new file" "echo test >> newappend.txt\nexit" "grep -q 'test' $TEST_DIR/newappend.txt"

# Input Redirection
echo ""
echo -e "${BLUE}Input Redirection:${NC}"
test_file_op "read from file" "echo content > input.txt\ncat < input.txt > output.txt\nexit" "grep -q 'content' $TEST_DIR/output.txt"
test_file_op "read and process" "echo data > proc.txt\ngrep data < proc.txt > result.txt\nexit" "grep -q 'data' $TEST_DIR/result.txt"

# Multiple Redirections
echo ""
echo -e "${BLUE}Multiple Redirections:${NC}"
test_file_op "input and output" "echo test > in.txt\ncat < in.txt > out.txt\nexit" "grep -q 'test' $TEST_DIR/out.txt"
test_file_op "multiple outputs same cmd" "echo data > file_a.txt && echo data2 > file_b.txt\nexit" "grep -q 'data' $TEST_DIR/file_a.txt && grep -q 'data2' $TEST_DIR/file_b.txt"
test_file_op "chain redirects" "echo start > chain1.txt && cat chain1.txt > chain2.txt\nexit" "grep -q 'start' $TEST_DIR/chain2.txt"

# Heredoc to File
echo ""
echo -e "${BLUE}Heredoc to File:${NC}"
test_file_op "heredoc redirect" "cat << EOF > heredoc.txt\nline1\nline2\nline3\nEOF\nexit" "[ \$(cat $TEST_DIR/heredoc.txt | wc -l) -eq 3 ]"
test_file_op "heredoc append" "echo start > hd_append.txt\ncat << EOF >> hd_append.txt\nadded\nEOF\nexit" "grep -q 'start' $TEST_DIR/hd_append.txt && grep -q 'added' $TEST_DIR/hd_append.txt"
test_file_op "multiple heredocs" "cat << A > hd1.txt\nfirst\nA\ncat << B > hd2.txt\nsecond\nB\nexit" "grep -q 'first' $TEST_DIR/hd1.txt && grep -q 'second' $TEST_DIR/hd2.txt"

# Pipes to Files
echo ""
echo -e "${BLUE}Pipes to Files:${NC}"
test_file_op "pipe to file" "echo test | cat > piped.txt\nexit" "grep -q 'test' $TEST_DIR/piped.txt"
test_file_op "multiple pipes to file" "echo data | grep data | cat > pipe_multi.txt\nexit" "grep -q 'data' $TEST_DIR/pipe_multi.txt"
test_file_op "pipe from file" "echo content > source.txt\ncat source.txt | grep content > dest.txt\nexit" "grep -q 'content' $TEST_DIR/dest.txt"

# File Permissions
echo ""
echo -e "${BLUE}File Permissions:${NC}"
test_file_op "create with default perms" "echo test > perm.txt\nexit" "[ -f $TEST_DIR/perm.txt ] && [ -r $TEST_DIR/perm.txt ]"
test_file_op "read only file error" "echo readonly > ro.txt\nchmod 444 ro.txt\necho new > ro.txt 2>&1 || echo ok\nexit" "[ -f $TEST_DIR/ro.txt ]"

# Special Characters in Filenames
echo ""
echo -e "${BLUE}Special Characters in Filenames:${NC}"
test_file_op "filename with spaces" "echo test > \"file with spaces.txt\"\nexit" "[ -f \"$TEST_DIR/file with spaces.txt\" ]"
test_file_op "filename with dash" "echo content > -dash.txt\nexit" "[ -f $TEST_DIR/-dash.txt ]"

# Large File Operations
echo ""
echo -e "${BLUE}Large File Operations:${NC}"
test_file_op "many lines" "seq 1 100 | cat > large.txt\nexit" "[ \$(cat $TEST_DIR/large.txt | wc -l) -eq 100 ]"
test_file_op "long line" "echo $(printf 'a%.0s' {1..1000}) > longline.txt\nexit" "[ \$(cat $TEST_DIR/longline.txt | wc -c) -gt 1000 ]"

# Error Cases
echo ""
echo -e "${BLUE}Error Cases:${NC}"
test_output "read nonexistent file" "cat < /nonexistent_file_12345 || echo error\nexit" "error"
test_output "write to protected dir" "echo test > /root/protected_file || echo error\nexit" "error"

# File with Logical Operators
echo ""
echo -e "${BLUE}Files with Logical Operators:${NC}"
test_file_op "AND with file creation" "echo a > and1.txt && echo b > and2.txt\nexit" "grep -q 'a' $TEST_DIR/and1.txt && grep -q 'b' $TEST_DIR/and2.txt"
test_file_op "OR with file fallback" "cat /nonexistent123 || echo fallback > or.txt\nexit" "grep -q 'fallback' $TEST_DIR/or.txt"

# Complex Scenarios
echo ""
echo -e "${BLUE}Complex Scenarios:${NC}"
test_file_op "pipe chain with files" "cat << EOF | grep test | cat > complex.txt\nhello\ntest line\nworld\nEOF\nexit" "grep -q 'test line' $TEST_DIR/complex.txt"
test_file_op "multiple redirects in chain" "echo start > s1.txt && cat s1.txt > s2.txt && cat s2.txt > s3.txt\nexit" "grep -q 'start' $TEST_DIR/s3.txt"
test_file_op "conditional file creation" "false || echo created > cond.txt\nexit" "grep -q 'created' $TEST_DIR/cond.txt"

# Cleanup test
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
    echo -e "${GREEN}✓ All file operation tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
