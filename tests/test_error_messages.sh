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

# Error message tests: compare minishell stderr outputs against expected substrings

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

PASSED=0
FAILED=0

run_error_test() {
    local cmd="$1"
    local expected="$2"

    # capture minishell stderr (normalize prompts and blank lines)
    ms_out=$(echo -e "$cmd\nexit" | NO_COLOR=1 $SHELL 2>&1 | sed 's/\[minishell\]> //g' | sed '/^$/d')
    # capture bash stderr for reference
    bash_out=$(bash -c "$cmd" 2>&1)

    if echo "$ms_out" | grep -qi "$expected"; then
        echo -e "${GREEN}✓${NC} [$expected] $cmd"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗${NC} [$expected] $cmd"
        echo "   minishell stderr: $ms_out"
        echo "   bash stderr: $bash_out"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

# inline list of test cases (format: "cmd|expected")
entries=(
    "nonexistentcommand123|command not found"
    "invalidcmd456|command not found"
    "ls /nonexistentpath|No such file or directory"
    "cat nonexistent_file|No such file or directory"
    "mkdir /root/forbidden|Permission denied"
    "ls /root|Permission denied"
    "cat /etc/shadow|Permission denied"
    "echo hello > /root/testfile|Permission denied"
)

for entry in "${entries[@]}"; do
    IFS='|' read -r cmd expect <<< "$entry"
    run_error_test "$cmd" "$expect"
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
    echo -e "${GREEN}✓ All error message tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some error message tests failed${NC}"
    exit 1
fi
