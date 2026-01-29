#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [ -z "$SHELL" ]; then
	if [ -x "$SCRIPT_DIR/../minishell" ]; then
		SHELL="$SCRIPT_DIR/../minishell"
	elif [ -x "$PWD/../minishell" ]; then
		SHELL="$PWD/../minishell"
	elif [ -x "$PWD/minishell" ]; then
		SHELL="$PWD/minishell"
	else
		echo "minishell binary not found" >&2
		exit 1
	fi
fi
if [ -z "$SHELL_PATH" ]; then
	SHELL_PATH="$SHELL"
fi

# Exit status tests: compare minishell exit codes to bash for a list of commands

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

PASSED=0
FAILED=0

run_cmd_exit() {
    local cmd="$1"

    echo -e "$cmd" | NO_COLOR=1 $SHELL > /dev/null 2>&1
    ms_code=$?
    echo -e "$cmd" | bash > /dev/null 2>&1
    bash_code=$?

    if [ $ms_code -eq $bash_code ]; then
        echo -e "${GREEN}✓${NC} [$ms_code] $cmd"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗${NC} expected $bash_code got $ms_code — $cmd"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

commands=(
    "true"
    "false"
    "exit 0"
    "exit 42"
    "nonexistentcommand123"
    "ls /nonexistentpath"
    "cat nonexistent_file"
    "echo hello | grep hello"
    "bash -c 'exit 3'"
)

echo ""
echo "========================================="
echo "      Exit Status Test Suite"
echo "========================================="
echo ""

for c in "${commands[@]}"; do
    run_cmd_exit "$c"
done

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
    echo -e "${GREEN}✓ All exit status tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some exit status tests failed${NC}"
    exit 1
fi
