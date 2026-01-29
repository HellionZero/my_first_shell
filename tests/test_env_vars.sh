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

# Test suite for environment variables and expansions

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

SHELL="$(dirname "$0")/../minishell"
PASSED=0
FAILED=0

test_env() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    output=$(echo -e "$input\nexit" | NO_COLOR=1 $SHELL 2>&1 | \
        sed 's/\[minishell\]> //g' | \
        grep -v "^exit$" | \
        grep -v "^$" | \
        tr '\n' ' ')
    
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

test_no_match() {
    local test_name="$1"
    local input="$2"
    local forbidden="$3"
    
    output=$(echo -e "$input\nexit" | NO_COLOR=1 $SHELL 2>&1 | \
        sed 's/\[minishell\]> //g' | \
        grep -v "^exit$" | \
        grep -v "^$")
    
    if ! echo "$output" | grep -q "$forbidden"; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗${NC} $test_name"
        echo "   Should NOT contain: $forbidden"
        echo "   Got: $output"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

echo ""
echo "========================================="
echo "   Environment Variables Test Suite"
echo "========================================="
echo ""

# Basic Environment Variables
echo -e "${BLUE}Basic Environment Variables:${NC}"
test_env "env - shows PATH" "env | grep PATH\nexit" "PATH="
test_env "env - shows HOME" "env | grep HOME\nexit" "HOME="
test_env "env - shows USER" "env | grep USER\nexit" "USER="
test_env "env - shows SHELL variable" "env | grep SHELL\nexit" "SHELL="

# SHLVL Tests
echo ""
echo -e "${BLUE}SHLVL Tests:${NC}"
test_env "SHLVL - incremented" "env | grep SHLVL\nexit" "SHLVL="
test_env "SHLVL - echo expansion" "echo \$SHLVL\nexit" "[0-9]"

# Export and Variable Setting
echo ""
echo -e "${BLUE}Export and Variable Setting:${NC}"
test_env "export - new variable" "export MYVAR=hello\nenv | grep MYVAR\nexit" "MYVAR=hello"
test_env "export - multiple variables" "export VAR1=a\nexport VAR2=b\nenv | grep VAR\nexit" "VAR1=a.*VAR2=b"
test_env "export - override existing" "export PATH=/test\necho \$PATH\nexit" "/test"
test_env "export - empty value" "export EMPTY=\nenv | grep EMPTY\nexit" "EMPTY="
test_env "export - with spaces in value" "export SPACED=\"hello world\"\nenv | grep SPACED\nexit" "SPACED=hello world"

# Variable Expansion
echo ""
echo -e "${BLUE}Variable Expansion:${NC}"
test_env "expansion - simple variable" "export TEST=value\necho \$TEST\nexit" "value"
test_env "expansion - in double quotes" "export VAR=test\necho \"value: \$VAR\"\nexit" "value: test"
test_env "expansion - concatenation" "export X=hello\necho \${X}world\nexit" "helloworld"
test_env "expansion - undefined variable" "echo \$UNDEFINED_VAR_XYZ\nexit" "^$"

# Exit Status Variable ($?)
echo ""
echo -e "${BLUE}Exit Status Variable (\$?):${NC}"
test_env "\$? - after success" "true\necho \$?\nexit" "0"
test_env "\$? - after command" "echo test\necho \$?\nexit" "0"
test_env "\$? - in command" "export VAR=\$?\nenv | grep VAR\nexit" "VAR="

# Unset Tests
echo ""
echo -e "${BLUE}Unset Tests:${NC}"
test_env "unset - remove variable" "export TOREMOVE=value\nunset TOREMOVE\nenv | grep TOREMOVE\nexit" "^$"
test_no_match "unset - variable gone" "export TESTVAR=val\nunset TESTVAR\nenv\nexit" "TESTVAR=val"
test_env "unset - multiple variables" "export A=1\nexport B=2\nunset A\nunset B\nenv | grep -E \"^A=|^B=\"\nexit" "^$"

# Variable in Commands
echo ""
echo -e "${BLUE}Variables in Commands:${NC}"
test_env "variable in echo" "export MSG=hello\necho \$MSG world\nexit" "hello world"
test_env "variable in pipe" "export WORD=test\necho \$WORD | cat\nexit" "test"
test_env "variable in redirect" "export FILE=/tmp/env_test.txt\necho content > \$FILE\ncat \$FILE\nexit" "content"

# HOME Variable
echo ""
echo -e "${BLUE}HOME Variable:${NC}"
test_env "HOME - exists" "env | grep HOME\nexit" "HOME="
test_env "HOME - cd without args" "export HOME=/tmp\ncd\npwd\nexit" "/tmp"

# PWD and OLDPWD
echo ""
echo -e "${BLUE}PWD and OLDPWD:${NC}"
test_env "PWD - current directory" "env | grep PWD\nexit" "PWD="
test_env "PWD - after cd" "cd /tmp\nenv | grep PWD\nexit" "PWD=/tmp"
test_env "OLDPWD - set after cd" "cd /tmp\ncd /\nenv | grep OLDPWD\nexit" "OLDPWD=/tmp"

# Environment Persistence
echo ""
echo -e "${BLUE}Environment Persistence:${NC}"
test_env "persistence - across commands" "export PERSIST=value\necho test\nenv | grep PERSIST\nexit" "PERSIST=value"

# Special Cases
echo ""
echo -e "${BLUE}Special Cases:${NC}"
test_env "invalid identifier - with special char" "export VAR-NAME=test\nexit" "not a valid identifier"
test_env "invalid identifier - starts with number" "export 123VAR=test\nexit" "not a valid identifier"
test_env "empty export list" "export\nexit" "="

# Variable Expansion Edge Cases
echo ""
echo -e "${BLUE}Variable Expansion Edge Cases:${NC}"
test_env "expansion - multiple in one line" "export A=1\nexport B=2\necho \$A \$B\nexit" "1 2"
test_env "expansion - nested quotes" "export VAR=test\necho '\$VAR' \"\$VAR\"\nexit" "\\\$VAR test"
test_env "expansion - with operators" "export X=val\necho \$X && echo done\nexit" "val.*done"

# Cleanup
rm -f /tmp/env_test.txt 2>/dev/null

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
    echo -e "${GREEN}✓ All environment variable tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
