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

# Pipe Test Suite - Tests pipe execution with various commands

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
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

print_info() {
    echo -e "${YELLOW}INFO:${NC} $1"
}

# Helper function to test pipe execution
test_pipe() {
    local command="$1"
    local description="$2"
    local check_type="$3"  # "output", "empty", "count"
    local expected="$4"
    
    print_test "$description"
    print_info "Command: $command"
    
    # Execute command in shell and capture output to file
    echo -e "$command\nexit" | NO_COLOR=1 ../minishell > /tmp/pipe_test_out.txt 2>&1
    
    # Extract only the output lines
    output=$(cat /tmp/pipe_test_out.txt | \
        sed 's/\[minishell\]> //g' | \
        grep -v "^exit$" | \
        grep -v "^$" | \
        head -20)
    
    if [ "$check_type" = "output" ]; then
        # Check if output contains expected string
        if echo "$output" | grep -q "$expected"; then
            print_pass
        else
            print_fail "Expected '$expected' in output"
            echo "Got: $output"
        fi
    elif [ "$check_type" = "empty" ]; then
        # Check if output is empty (for commands that should produce no output)
        if [ -z "$output" ]; then
            print_pass
        else
            print_fail "Expected empty output"
            echo "Got: $output"
        fi
    elif [ "$check_type" = "count" ]; then
        # Check line count
        count=$(echo "$output" | wc -l)
        if [ "$count" -eq "$expected" ]; then
            print_pass
        else
            print_fail "Expected $expected lines, got $count"
        fi
    elif [ "$check_type" = "nonempty" ]; then
        # Check if output is not empty
        if [ -n "$output" ]; then
            print_pass
        else
            print_fail "Expected non-empty output"
        fi
    fi
    
    rm -f /tmp/pipe_test_out.txt
}

echo ""
echo "========================================="
echo "         PIPE TEST SUITE"
echo "========================================="
echo ""

# Category 1: Simple Pipes (External | External)
echo "--- Simple Pipes (External | External) ---"
echo ""

test_pipe "ls .. | grep Makefile" \
    "Pipe: ls | grep Makefile" \
    "output" \
    "Makefile"

test_pipe "ls .. | grep test" \
    "Pipe: ls | grep test" \
    "output" \
    "test"

test_pipe "/bin/echo test | cat" \
    "Pipe: /bin/echo | cat" \
    "output" \
    "test"

test_pipe "cat ../Makefile | grep NAME" \
    "Pipe: cat | grep (search in file)" \
    "output" \
    "NAME"

echo ""

# Category 2: Builtin | External
echo "--- Builtin | External ---"
echo ""

test_pipe "pwd | cat" \
    "Pipe: pwd (builtin) | cat" \
    "nonempty" \
    ""

test_pipe "pwd | grep my_shell" \
    "Pipe: pwd | grep (filter output)" \
    "output" \
    "my_shell"

echo ""

# Category 3: External | Builtin
echo "--- External | Builtin (Not Applicable) ---"
echo ""
print_info "Note: Most builtins don't read stdin, so External | Builtin is limited"

echo ""

# Category 4: Multi-stage Pipes
echo "--- Multi-stage Pipes (3+ commands) ---"
echo ""

test_pipe "ls .. | grep test | cat" \
    "Pipe: ls | grep | cat (3 stages)" \
    "output" \
    "test"

test_pipe "cat ../Makefile | grep SRC | cat" \
    "Pipe: cat | grep | cat (3 stages)" \
    "output" \
    "SRC"

echo ""

# Category 5: Pipes with no output
echo "--- Pipes with No Match ---"
echo ""

test_pipe "ls .. | grep nonexistent_file_12345" \
    "Pipe: ls | grep (no match)" \
    "empty" \
    ""

echo ""

# Category 6: Error cases
echo "--- Error Cases ---"
echo ""

test_pipe "ls | invalidcommand" \
    "Pipe: Valid | Invalid command" \
    "output" \
    "command not found"

test_pipe "invalidcommand | cat" \
    "Pipe: Invalid | Valid command" \
    "output" \
    "command not found"

echo ""

# Category 7: Complex real-world scenarios
echo "--- Real-world Scenarios ---"
echo ""

test_pipe "ls -la .. | grep src" \
    "Pipe: ls -la | grep (with flags)" \
    "output" \
    "src"
test_pipe "cat ../Makefile | grep CFLAGS | wc -l" \
	"Pipe: cat | grep | wc -l (count lines)" \
	"output" \
	"2"
echo ""
echo "========================================="
echo "         TEST SUMMARY"
echo "========================================="
echo -e "Total: $((PASSED + FAILED))"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
