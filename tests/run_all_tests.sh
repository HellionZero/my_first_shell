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

# Master test runner - executes all test suites

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

TOTAL_PASSED=0
TOTAL_FAILED=0
SUITES_PASSED=0
SUITES_FAILED=0

echo ""
echo "========================================="
echo "    MINISHELL - Complete Test Suite"
echo "========================================="
echo ""

# Compile first
echo -e "${BLUE}[1/7]${NC} Compiling minishell..."
make -C .. > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Compilation failed${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Compilation successful${NC}"
echo ""

# Run test suites
run_suite() {
    local suite_name="$1"
    local suite_script="$2"
    
    echo -e "${BLUE}Running: $suite_name${NC}"
    echo "========================================="
    
    bash "$suite_script"
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        SUITES_PASSED=$((SUITES_PASSED + 1))
        echo -e "${GREEN}✓ $suite_name completed successfully${NC}"
    else
        SUITES_FAILED=$((SUITES_FAILED + 1))
        echo -e "${RED}✗ $suite_name had failures${NC}"
    fi
    
    echo ""
    return $exit_code
}

# Execute all test suites
echo -e "${BLUE}[2/7]${NC} Lexer Tests"
echo "========================================="
run_suite "Lexer Tests" "test_lexer.sh"

echo -e "${BLUE}[3/7]${NC} Parser/AST Tests"
echo "========================================="
run_suite "Parser Tests" "test_parser.sh"

echo -e "${BLUE}[4/7]${NC} Heredoc Tests"
echo "========================================="
run_suite "Heredoc Tests" "test_heredoc.sh"

echo -e "${BLUE}[5/7]${NC} Signal & Exit Tests"
echo "========================================="
run_suite "Signal & Exit Tests" "test_signals.sh"

echo -e "${BLUE}[6/7]${NC} Signal State Management Tests"
echo "========================================="
run_suite "Signal State Tests" "test_signal_states.sh"

echo -e "${BLUE}[7/15]${NC} Integration Tests"
echo "========================================="
run_suite "Integration Tests" "test_integration.sh"

echo -e "${BLUE}[8/16]${NC} Builtin Commands Tests"
echo "========================================="
run_suite "Builtin Tests" "test_builtins.sh"

echo -e "${BLUE}[9/16]${NC} Echo Builtin Tests"
echo "========================================="
run_suite "Echo Tests" "test_echo.sh"

echo -e "${BLUE}[10/16]${NC} Redirection Tests"
echo "========================================="
run_suite "Redirection Tests" "test_redirections.sh"

echo -e "${BLUE}[11/16]${NC} Pipe Tests"
echo "========================================="
run_suite "Pipe Tests" "test_pipes.sh"

echo -e "${BLUE}[12/18]${NC} External Commands Tests"
echo "========================================="
run_suite "External Command Tests" "test_external_commands.sh"

echo -e "${BLUE}[13/18]${NC} Exit Status Tests"
echo "========================================="
run_suite "Exit Status Tests" "test_exit_status.sh"

echo -e "${BLUE}[14/18]${NC} Error Message Tests"
echo "========================================="
run_suite "Error Message Tests" "test_error_messages.sh"

echo -e "${BLUE}[15/18]${NC} Logical Operators Tests"
echo "========================================="
run_suite "Logical Operators Tests" "test_logical.sh"

echo -e "${BLUE}[14/16]${NC} Environment Variables Tests"
echo "========================================="
run_suite "Environment Variables Tests" "test_env_vars.sh"

echo -e "${BLUE}[15/16]${NC} File Operations Tests"
echo "========================================="
run_suite "File Operations Tests" "test_files.sh"

echo -e "${BLUE}[16/16]${NC} Segfault Detection Tests"
echo "========================================="
run_suite "Segfault Tests" "test_segfault.sh"

echo -e "${BLUE}[17/17]${NC} Memory Leak Tests"
echo "========================================="
run_suite "Memory Leak Tests" "test_leaks.sh"

# Final summary
echo ""
echo "========================================="
echo "           FINAL SUMMARY"
echo "========================================="
echo ""
echo "Test Suites Run: $((SUITES_PASSED + SUITES_FAILED))"
echo -e "${GREEN}Suites Passed:   $SUITES_PASSED${NC}"
echo -e "${RED}Suites Failed:   $SUITES_FAILED${NC}"
echo ""

if [ $SUITES_FAILED -eq 0 ]; then
    echo -e "${GREEN}════════════════════════════════════════${NC}"
    echo -e "${GREEN}   ✓ ALL TEST SUITES PASSED!${NC}"
    echo -e "${GREEN}════════════════════════════════════════${NC}"
else
    echo -e "${RED}════════════════════════════════════════${NC}"
    echo -e "${RED}   ✗ SOME TEST SUITES FAILED${NC}"
    echo -e "${RED}════════════════════════════════════════${NC}"
fi

# Cleanup
echo ""
echo "========================================="
echo "           CLEANUP"
echo "========================================="
echo ""

echo -e "${BLUE}Removing test artifacts...${NC}"
rm -f /tmp/test_*.txt
rm -f test_*.txt
rm -f testfile*.txt
rm -f output*.txt
rm -f input*.txt
rm -f append*.txt
rm -f heredoc*.txt
rm -f newfile*.txt
rm -f tmpfile*.txt
cd "$(dirname "$0")"
rm -f test_*.txt
rm -f testfile*.txt
rm -f output*.txt
rm -f input*.txt
rm -f append*.txt
rm -f heredoc*.txt
rm -f newfile*.txt
rm -f tmpfile*.txt
rm -f count.txt
rm  -f file.txt
rm  -f out.txt
rm -f file
rm -f a
rm -f b
rm -f c
rm -f d
rm -f e
rm -f f
rm -f g
rm -f h
rm -f i
rm -f j
rm -f out.txt
rm -f result.txt
rm -f *.tmp
cd - > /dev/null
echo -e "${GREEN}✓ Test files cleaned${NC}"

echo -e "${BLUE}Running make fclean...${NC}"
make -C .. fclean > /dev/null 2>&1
echo -e "${GREEN}✓ Build artifacts cleaned${NC}"

echo ""
if [ $SUITES_FAILED -eq 0 ]; then
    exit 0
else
    exit 1
fi
