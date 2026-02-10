*Este projeto foi criado como parte do currículo da 42 por lsarraci, loda-sil.*

---

## Descrição

**minishell** é uma implementação funcional de um shell Unix/Linux em C, desenvolvida como projeto educacional. O objetivo é criar um interpretador de linha de comando que reproduz as funcionalidades básicas do bash, incluindo execução de comandos, pipes, redirecionamentos, variáveis de ambiente e builtins essenciais.

### Visão Geral

O projeto implementa um pipeline completo de processamento de entrada:
1. **Lexer**: Tokeniza a entrada do usuário
2. **Parser**: Constrói uma árvore sintática (AST) a partir dos tokens
3. **Executor**: Executa os comandos de acordo com a árvore

Inclui suporte a:
- Pipes (`|`) e operadores lógicos (`&&`, `||`)
- Redirecionamentos (`>`, `>>`, `<`, `<<`)
- Variáveis de ambiente e expansão de variáveis
- Heredocs (`<<`)
- Builtins: `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`
- Gerenciamento de sinais (SIGINT, SIGQUIT)
- Histórico de comandos (readline)

---

## Instruções

### Requisitos

- GCC (ou qualquer compilador C compatível)
- readline library development files
- Make
- Linux/Unix

### Compilação

```bash
make
```

Isso compila o projeto e gera o executável `minishell`.

### Execução

```bash
./minishell
```

A aplicação será iniciada em modo interativo com um prompt esperando por comandos.

### Limpeza

```bash
make clean        # Remove arquivos objeto
make fclean       # Remove executável e arquivos objeto
make re           # Recompila tudo do zero
```

### Verificação de Memory Leaks

```bash
make leak
```

---

## Recursos

### Documentação Oficial

- [POSIX Shell Standard](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html) - Especificação oficial do shell POSIX
- [Bash Manual](https://www.gnu.org/software/bash/manual/) - Manual do Bash
- [Linux Programmer's Manual - execve](https://man7.org/linux/man-pages/man2/execve.2.html) - Documentação de system calls
- [Linux Signals](https://man7.org/linux/man-pages/man7/signal.7.html) - Gerenciamento de sinais em Unix/Linux

### Referências Técnicas

- [Writing a Unix Shell](https://www.internalpointers.com/post/writing-unix-shell-part-1) - Tutorial detalhado sobre implementação de shells
- [Abstract Syntax Tree (AST)](https://en.wikipedia.org/wiki/Abstract_syntax_tree) - Conceito fundamental do parser
- [Process Management in Unix](https://www.ibm.com/docs/en/aix/7.2?topic=management-process) - Fundamentos de fork, exec e wait
- [File Descriptors and Redirection](https://en.wikipedia.org/wiki/File_descriptor) - Conceitos de redirecionamento

### Estrutura do Projeto

```
.
├── src/                      # Código fonte principal
│   ├── main.c               # Ponto de entrada
│   ├── builtins/            # Comandos built-in (cd, echo, etc.)
│   ├── execution/           # Lógica de execução de comandos
│   ├── lexer/               # Tokenizador (análise léxica)
│   ├── parser/              # Parser (análise sintática)
│   ├── env/                 # Gerenciamento de variáveis de ambiente
│   ├── signals/             # Tratamento de sinais
│   ├── utils/               # Funções utilitárias
│   ├── display/             # Prompt e banner
│   ├── debug/               # Funções de debug
│   └── file_manager/        # Gerenciamento de arquivos
├── include/                 # Arquivos header (.h)
├── lib/                     # Bibliotecas auxiliares
│   ├── libft/              # Biblioteca padrão customizada
│   └── ft_printf/          # Implementação customizada de printf
├── tests/                   # Suite de testes
├── documentation/           # Documentação adicional
└── Makefile                 # Build system
```

---

## Uso de IA

A IA foi utilizada para:

1. **Análise e Debugging**: Revisão de código, identificação de bugs em gerenciamento de memória e lógica de execução
2. **Documentação**: Criação e estruturação de comentários no código e documentação técnica
3. **Testes**: Sugestões para casos de teste e identificação de edge cases
4. **Otimizações**: Recomendações para melhorar performance e legibilidade do código
5. **Padrões de Design**: Consultoria sobre estruturas de dados e arquitetura de software

---

## Arquitetura

### Pipeline de Execução

```
input_string
    ↓
[LEXER] → tokens
    ↓
[PARSER] → AST (Abstract Syntax Tree)
    ↓
[EXECUTOR] → execução de comandos
    ↓
exit_code
```

### Componentes Principais

- **Lexer**: Converte string de entrada em tokens (palavras, operadores, redirecionamentos)
- **Parser**: Constrói uma árvore sintática a partir dos tokens
- **Executor**: Percorre a AST executando comandos com fork/exec
- **Builtins**: Comandos implementados internamente (não requerem fork)
- **Environment**: Gerencia variáveis de ambiente e expansões

---

## Funcionalidades Implementadas

- Execução de comandos externos
- Pipes (canalização de processos)
- Redirecionamentos (>, >>, <, <<)
- Operadores lógicos (&&, ||)
- Builtins: cd, echo, env, exit, export, pwd, unset
- Variáveis de ambiente e expansão ($VAR)
- Heredocs (<<)
- Histórico de comandos (readline)
- Tratamento de sinais (SIGINT, SIGQUIT)
- Quotes (simples e duplas) e escape de caracteres

---

## Exemplos de Uso

```bash
# Comandos básicos
$ echo "Hello, World!"
Hello, World!

$ pwd
/home/user/projects/minishell

$ ls -la | grep .c

# Redirecionamentos
$ echo "test" > output.txt
$ cat < output.txt

# Variáveis de ambiente
$ export MY_VAR="Hello"
$ echo $MY_VAR
Hello

# Operadores lógicos
$ true && echo "Sucesso"
Sucesso

$ false || echo "Falhou"
Falhou

# Heredoc
$ cat << EOF
> linha 1
> linha 2
> EOF
linha 1
linha 2
```

---

## Autores

- **lsarraci** - Implementação principal
- **loda-sil** - Contribuições e testes

---

## Licença

Este projeto é parte do programa educacional da 42 School.
