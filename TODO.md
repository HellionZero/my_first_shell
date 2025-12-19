# TODO - My Shell Project

## Project Status Overview

**Last Updated:** December 19, 2025  
**Current Phase:** Parser Implementation  
**Test Coverage:** 31/31 tests passing (100%)  
**Code Quality:** Norminette compliant

---

## ✅ Phase 1: Infrastructure (COMPLETED)

### Base Infrastructure
- [x] Configure Makefile with libft and ft_printf
- [x] Create modular header structure (shell.h, types.h, structs.h, lexer.h)
- [x] Implement main loop with readline
- [x] Add command history
- [x] Configure signal handlers (SIGINT, SIGQUIT)
- [x] Create signal handlers for different modes (interactive, executing, heredoc)
- [x] Create startup banner
- [x] Implement colored prompt with arrow ➜
- [x] Fix prompt wrapping with readline

### Data Structures
- [x] Define e_token_type enum (11 types)
- [x] Define e_word_part_type enum (4 types)
- [x] Define s_token struct (with parts and value fields)
- [x] Define s_word_part struct (linked list)
- [x] Define s_command struct (linked list)
- [x] Implement token_utils (new, free, add_back, list_free)
- [x] Implement word_part_utils (new, free, add_back, list_free)
- [x] Implement command_utils (new, free, add_back, list_free)

---

## ✅ Phase 2: Lexer Module (COMPLETED - 100%)

### Core Lexer
- [x] Implement main lexer loop with delegation pattern
- [x] Create modular file structure (11 files)
- [x] Implement operator detection and tokenization
- [x] Implement word tokenization with composite parts
- [x] Create comprehensive debug output system

### Operators (7/7 implemented)
- [x] Pipe (|) - TOKEN_PIPE
- [x] Logical AND (&&) - TOKEN_AND
- [x] Logical OR (||) - TOKEN_OR
- [x] Redirect in (<) - TOKEN_REDIR_IN
- [x] Redirect out (>) - TOKEN_REDIR_OUT
- [x] Append (>>) - TOKEN_APPEND
- [x] Heredoc (<<) - TOKEN_HEREDOC

### Quotes (3/3 implemented)
- [x] Single quotes (') - PART_SINGLE_QUOTE
- [x] Double quotes (") - PART_DOUBLE_QUOTE
- [x] Escape sequences in double quotes (\\", \\\\)

### Variables (4/4 types implemented)
- [x] Name variables ($USER, $PATH) - extract_name_variable()
- [x] Braced variables (${VAR}) - extract_braced_variable()
- [x] Special variables ($?, $$, $#, $!, $@, $*) - extract_special_variable()
- [x] Positional variables ($0, $1, $2, $123) - extract_positional_variable()
- [x] Dollar literal ($) - fallback handling

### Word Parts System
- [x] PART_LITERAL - plain text
- [x] PART_SINGLE_QUOTE - single quoted content
- [x] PART_DOUBLE_QUOTE - double quoted content
- [x] PART_VARIABLE - variable names
- [x] Composite word support (prefix$VAR'literal'${TEST}"quoted")

### Lexer Architecture
- [x] lexer.c - Main entry point with handle_operator()
- [x] lexer_utils.c - Helper functions
- [x] lexer_logical.c - AND, OR, PIPE operators
- [x] lexer_arrows.c - Redirection operators
- [x] lexer_words.c - Word tokenization with handle_dollar()
- [x] lexer_quotes.c - Quote extraction
- [x] lexer_variables.c - Variable extraction manager
- [x] var_extraction_utils.c - Variable type extractors
- [x] variable_utils.c - Validation functions
- [x] token_utils.c - Token management
- [x] word_utils.c - Word part management

### Debug & Testing
- [x] lexer_debug.c - Token visualization with word parts
- [x] Comprehensive test suite (27 lexer tests)
- [x] Test infrastructure with master runner
- [x] Documentation (tests/README.md)

---

## 🔄 Phase 3: Parser (IN PROGRESS - 0%)

### Basic Parser Structure
- [ ] Replace primitive parser with token-based parser
- [ ] Create AST (Abstract Syntax Tree) structure
- [ ] Define AST node types (command, pipe, redir, logical)
- [ ] Implement parser entry point (t_ast *parser(t_token *tokens))

### Command Parsing
- [ ] Extract command name from first WORD token
- [ ] Extract arguments from subsequent WORD tokens
- [ ] Expand word_parts into final strings
- [ ] Handle empty commands

### Operator Parsing
- [ ] Parse pipe operators (build pipeline)
- [ ] Parse logical AND (&&) operators
- [ ] Parse logical OR (||) operators
- [ ] Build operator precedence
- [ ] Create command sequences

### Redirection Parsing
- [ ] Parse redirect in (<) with filename
- [ ] Parse redirect out (>) with filename
- [ ] Parse append (>>) with filename
- [ ] Parse heredoc (<<) with delimiter
- [ ] Support multiple redirections per command

### Syntax Validation
- [ ] Validate operator placement
- [ ] Check for orphan operators
- [ ] Validate redirection targets
- [ ] Check for quote closure (already done in lexer)
- [ ] Error reporting with position

### Parser Testing
- [ ] Unit tests for simple commands
- [ ] Tests for pipes and redirections
- [ ] Tests for logical operators
- [ ] Tests for complex combinations
- [ ] Error handling tests

---

## ⏳ Phase 4: Executor (PENDING - 0%)

### Basic Execution
- [ ] Implement command execution from AST
- [ ] Fork and execve for external commands
- [ ] PATH search for executables
- [ ] Builtin detection and execution
- [ ] Exit code capture and storage

### Expansions
- [ ] Expand variables during execution (not in lexer)
- [ ] Expand $VAR with environment lookup
- [ ] Expand $? with last exit code
- [ ] Expand $$ with shell PID
- [ ] Expand $# with argument count
- [ ] Handle undefined variables (empty string)

### Quote Processing
- [ ] Process PART_SINGLE_QUOTE (no expansion)
- [ ] Process PART_DOUBLE_QUOTE (with expansion)
- [ ] Remove quote delimiters
- [ ] Preserve literal values

### Pipes
- [ ] Create pipe() for inter-process communication
- [ ] Set up pipe connections between commands
- [ ] Handle pipe chains (cmd1 | cmd2 | cmd3)
- [ ] Close unused file descriptors
- [ ] Wait for all processes

### Redirections
- [ ] Implement redirect in (<) with open()
- [ ] Implement redirect out (>) with open()
- [ ] Implement append (>>) with O_APPEND
- [ ] Implement heredoc (<<) with temp file or pipe
- [ ] Error handling for file operations

### Logical Operators
- [ ] Implement AND (&&) - execute right if left succeeds
- [ ] Implement OR (||) - execute right if left fails
- [ ] Short-circuit evaluation
- [ ] Exit code propagation

### Process Management
- [ ] Configure signals for child processes
- [ ] Handle SIGINT during execution
- [ ] Wait for processes (wait/waitpid)
- [ ] Clean up zombie processes
- [ ] Handle foreground/background execution

---

## ⏳ Phase 5: Builtins (PARTIAL - 1/7)

### Required Builtins
- [x] exit - Exit shell with optional code
- [ ] cd - Change directory (with -, ~, absolute, relative)
- [ ] pwd - Print working directory
- [ ] echo - Print arguments (with -n flag)
- [ ] env - Print environment variables
- [ ] export - Set environment variables
- [ ] unset - Unset environment variables

### Environment Management
- [ ] Store environment in shell structure
- [ ] Modify environment (export/unset)
- [ ] Pass environment to execve
- [ ] Handle PATH updates

---

## 📊 Testing & Quality (COMPLETED - 100%)

### Test Infrastructure
- [x] Master test runner (run_tests.sh)
- [x] Comprehensive test suite (27 tests)
- [x] Compilation tests (make, make re, make fclean)
- [x] Norminette validation
- [x] Test documentation (tests/README.md)
- [x] Color-coded output
- [x] Automated validation

### Test Coverage
- [x] Operators (7/7 tests)
- [x] Variables (9/9 tests)
- [x] Quotes (3/3 tests)
- [x] Word Parts (4/4 tests)
- [x] Edge Cases (4/4 tests)
- [ ] Parser tests (0 tests)
- [ ] Executor tests (0 tests)

### Code Quality
- [x] Norminette compliance (100%)
- [x] Modular architecture
- [x] Single Responsibility Principle
- [x] Memory management (no leaks in lexer)
- [ ] Valgrind testing
- [ ] Performance benchmarks

---

## 🎨 Advanced Features (OPTIONAL)

### Advanced Operators
- [ ] Subshells with parentheses ()
- [ ] Command grouping with braces {}
- [ ] Background execution (&)
- [ ] Command substitution $()
- [ ] Arithmetic expansion $(())

### Wildcards
- [ ] Asterisk (*) globbing
- [ ] Question mark (?) single char
- [ ] Bracket expressions ([abc])
- [ ] Tilde expansion (~)

### Job Control
- [ ] Job list tracking
- [ ] fg command (foreground)
- [ ] bg command (background)
- [ ] jobs command (list jobs)
- [ ] Process group management

### UX Improvements
- [ ] Prompt with current directory
- [ ] Prompt with git branch
- [ ] Prompt with exit code color
- [ ] Tab completion
- [ ] Persistent history (~/.my_shell_history)
- [ ] Multi-line command support
- [ ] Syntax highlighting

---

## 📝 Documentation

### Code Documentation
- [x] Lexer documentation (documentation/lexer.md)
- [x] Test documentation (tests/README.md)
- [x] Documentation guide (documentation/DOCUMENTATION_GUIDE.md)
- [ ] Parser documentation
- [ ] Executor documentation
- [ ] Architecture overview

### Project Documentation
- [x] TODO.md (this file)
- [ ] README.md for project root
- [ ] API documentation
- [ ] Usage examples

---

## 🎯 Next Immediate Steps

1. **Parser Implementation**
   - Define AST structures in structs.h
   - Implement parser entry point
   - Parse simple commands first
   - Add operator parsing
   - Test incrementally

2. **Testing**
   - Create parser test suite
   - Add integration tests
   - Test edge cases

3. **Builtins**
   - Implement cd, pwd, echo
   - Test with parser integration

4. **Executor Foundation**
   - Design executor architecture
   - Implement simple command execution
   - Add variable expansion

---

## 📈 Project Statistics

| Metric | Value |
|--------|-------|
| Total Files | 30+ |
| Lines of Code | ~2000+ |
| Test Coverage | 31/31 (100% lexer) |
| Norminette Status | ✅ All files pass |
| Compilation Status | ✅ Clean build |
| Memory Leaks | ✅ None detected (lexer) |

---

**Status Legend:**
- ✅ Completed
- 🔄 In Progress  
- ⏳ Pending
- 🎯 Next Priority
- [ ] README.md completo
- [ ] ARCHITECTURE.md (design do projeto)
- [ ] API.md (documentação de funções)
- [ ] CHANGELOG.md (histórico de versões)
- [ ] Comentários em todas as funções
- [ ] Documentar tipos de token e sumatizados
- [ ] Testes com valgrind (memória(s))
- [ ] CHANGELOG.md (histórico de versões)
- [ ] Comentários em todas as funções

## 🧪 Testes
- [ ] Testes do lexer
- [ ] Testes do parser
- [ ] Testes de builtins
- [ ] Testes de redirecionamentos
- [ ] Testes de pipes
- [ ] Testes de edge cases
- [ ] Script de testes automatizados
35%
- **Última Atualização:** 2025-12-17
- **Próximo Milestone:** Implementar Parser
- **Arquivos Criados:** 20+
- **Funções Implementadas:** 30+
- **Tipos de Token:** 11

## 🎯 Próximas Prioridades (Em Ordem)
1. **Parser** - Converter tokens em comandos executáveis
2. **Quotes** - Aspas simples e duplas no lexer
3. **Variáveis** - Expansão de $VAR no lexer
4. **Executor** - Executar comandos simples
5. **Pipes** - Comunicação entre processos
6. **Redirects** - Implementar <, >, >>, <<
7. **Builtins** - cd, pwd, echo, env, export, unset

## 📊 Estatísticas
- **Progresso Geral:** ~15%
- **Última Atualização:** 2025-12-17
- **Próximo Milestone:** Implementar Lexer

---

## Como Usar Este Arquivo
- `[x]` = Tarefa concluída
- `[ ]` = Tarefa pendente
- `🔄` = Em progresso
- `⏳` = Planejado
- `✅` = Concluído

**Comando para marcar como concluído:** Troque `[ ]` por `[x]`
