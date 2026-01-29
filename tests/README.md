# My Shell - Test Suite Documentation

## Overview

Comprehensive automated test suite covering lexer, parser, heredoc, signals, and integration testing for my_shell.

## Test Results

**Status:** ✅ **100% PASSING** (92/92 tests)

**Last Run:** January 8, 2026

## Test Suites

### 1. `test_lexer.sh` - Lexer Tests (27 tests)
Tests tokenization of shell input:
- **Basic Commands** (3): Simple commands, arguments, multiple arguments
- **Operators** (7): Pipe, redirections (<, >, >>), heredoc (<<), logical (&&, ||)
- **Quotes** (3): Single quotes, double quotes, variables in quotes
- **Variables** (5): Simple ($VAR), braced (${VAR}), special ($?, $$), positional ($1)
- **Composite Words** (3): Mixed literal+variable combinations
- **Edge Cases** (2): Multiple spaces, leading spaces
- **Complex Patterns** (4): Multiple pipes, mixed operators

**Run:** `bash test_lexer.sh`

### 2. `test_parser.sh` - Parser/AST Tests (18 tests)
Tests Abstract Syntax Tree generation:
- **Simple Commands** (3): Single command, commands with args
- **Pipe Commands** (2): Simple and multiple pipes
- **Redirections** (3): Input (<), output (>), append (>>)
- **Logical Operators** (3): AND (&&), OR (||), fallback patterns
- **Combined Operators** (3): Multiple redirects, pipe+redirect, complex pipelines
- **AST Structure** (4): Node types (COMMAND, PIPE, AND, OR)

**Run:** `bash test_parser.sh`

### 3. `test_heredoc.sh` - Heredoc Tests (19 tests)
Tests heredoc parsing and file descriptor management:
- **Basic Heredoc** (3): Different delimiters
- **Heredoc with Quotes** (2): Single/double quoted delimiters
- **FD Management** (2): FD initialization and tracking
- **Multiple Heredocs** (2): Sequential heredocs, heredoc + redirect
- **Heredoc in Pipes** (4): Before/after pipe combinations
- **Heredoc with Logical Operators** (4): AND/OR combinations
- **Error Cases** (2): Empty delimiter, missing delimiter

**Run:** `bash test_heredoc.sh`

### 4. `test_signals.sh` - Signal & Exit Tests (15 tests)
Tests signal handling and exit behavior:
- Exit builtin variations (no args, numeric, invalid, too many args)
- EOF (Ctrl+D) handling
- Exit code modulo 256
- Negative exit codes
- History management
- Memory leak verification

**Run:** `bash test_signals.sh`

### 5. `test_signal_states.sh` - Signal State Tests (5 tests)
Tests signal state management:
- Signal state clearing
- Exit flag propagation
- Empty line handling
- Cleanup verification
- Stability testing

**Run:** `bash test_signal_states.sh`

### 6. `test_integration.sh` - Integration Tests (8 tests)
End-to-end integration testing:
- Exit status propagation
- Whitespace handling
- Banner display
- EOF handling
- Large exit codes
- Sequential runs

**Run:** `bash test_integration.sh`

### 7. `test_leaks.sh` - Memory Leak Tests
Comprehensive memory leak detection with valgrind.

**Run:** `bash test_leaks.sh`

## Running Tests

### Run All Tests
```bash
cd tests
bash run_all_tests.sh
```

### Run Individual Test Suites
```bash
bash test_lexer.sh         # Lexer tokenization (27 tests)
bash test_parser.sh        # Parser AST generation (18 tests)
bash test_heredoc.sh       # Heredoc handling (19 tests)
bash test_signals.sh       # Signals and exit (15 tests)
bash test_signal_states.sh # Signal state management (5 tests)
bash test_integration.sh   # Integration tests (8 tests)
bash test_leaks.sh         # Memory leak detection
```

## Test Statistics

| Suite | Tests | Status |
|-------|-------|--------|
| Lexer | 27 | ✅ 100% |
| Parser | 18 | ✅ 100% |
| Heredoc | 19 | ✅ 100% |
| Signals | 15 | ✅ 100% |
| Signal States | 5 | ✅ 100% |
| Integration | 8 | ✅ 100% |
| **TOTAL** | **92** | **✅ 100%** |

## Memory Safety

**Valgrind Results:**
- definitely lost: **0 bytes** ✅
- possibly lost: **0 bytes** ✅
- still reachable: **208 KB** ⚠️ *(readline library internals - expected)*

## Requirements

- Compiled `my_shell` binary in parent directory (`../my_shell`)
- DEBUG mode support (`DEBUG=1` environment variable)
- Valgrind (for leak testing)
- Bash shell

## Color Output

Tests use color-coded output:
- 🔵 **Blue**: Test suite/category headers
- ✅ **Green**: Passed tests and success messages
- ❌ **Red**: Failed tests and error messages

## Exit Codes

- `0`: All tests passed
- `1`: One or more tests failed

## Test Implementation

All tests are implemented as bash scripts that:
1. Compile the shell with `make`
2. Run shell commands via stdin
3. Capture and validate output
4. Report results with color-coded output
5. Return appropriate exit codes

Tests use `DEBUG=1` environment variable to enable detailed lexer/parser output for validation.
