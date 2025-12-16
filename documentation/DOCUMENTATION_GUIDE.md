# Guia de Documentação do Projeto

Este arquivo serve como referência para manter a documentação organizada e consistente ao longo do desenvolvimento.

---

## Estrutura de Documentação Recomendada

```
docs/
├── README.md                    ← Visão geral do projeto
├── ARCHITECTURE.md              ← Arquitetura geral
├── SETUP.md                     ← Como compilar e rodar
├── CONTRIBUTING.md              ← Guia para colaboradores
├── API.md                       ← Documentação de funções
└── diagrams/                    ← Diagramas (opcional)
    ├── pipeline_flow.txt
    └── struct_diagram.txt
```

---

## 1. README.md (Raiz do projeto)

### O que incluir:

```markdown
# Nome do Projeto

## Descrição
- O que é
- Objetivos

## Status Atual
- [x] Estruturas de dados definidas
- [x] Funções utilitárias implementadas
- [ ] Lexer (em desenvolvimento)
- [ ] Parser
- [ ] Executor

## Como Compilar
```bash
make
```

## Como Usar
```bash
./my_shell
```

## Estrutura do Projeto
```
src/
├── lexer/
├── parser/
├── builtins/
```

## Equipe
- Pessoa A: Lexer/Parser
- Pessoa B: Builtins
```

**Dicas:**
- Seja breve
- Use checkboxes para mostrar progresso
- Inclua exemplos práticos

---

## 2. docs/ARCHITECTURE.md

### Seções Recomendadas:

1. **Visão Geral**
   - Fluxo do pipeline (Input → Lexer → Parser → Executor)
   - Diagrama de blocos (pode ser ASCII art)

2. **Estruturas de Dados**
   - Descrever cada struct (t_token, t_command)
   - Por que foram escolhidas
   - Como se relacionam

3. **Módulos**
   - Lexer: responsabilidades
   - Parser: responsabilidades
   - Builtins: responsabilidades
   - Executor: responsabilidades

4. **Decisões de Design**
   - Por que não usar t_list da libft
   - Por que funções próprias
   - Separação de responsabilidades

5. **Dependências**
   - Quais módulos dependem de quais
   - Como trabalhar em paralelo

### Formato sugerido:

```markdown
## Pipeline Flow

```
Input String
    ↓
[Lexer] → Tokens
    ↓
[Parser] → Commands
    ↓
[Executor] → Output
```

## Estruturas

### t_token
- **Propósito**: Representar unidade léxica
- **Campos**:
  - type: tipo de token
  - value: conteúdo
  - pos: posição (para erros)
  - next: próximo token
```

---

## 3. docs/API.md

### Template para cada função pública:

```markdown
## Lexer

### `token_new`
```c
t_token *token_new(t_token_type type, char *value, int pos);
```

**Descrição**: Cria novo token

**Parâmetros**:
- `type`: Tipo do token (TOKEN_WORD, TOKEN_PIPE, etc)
- `value`: Conteúdo do token (copiado)
- `pos`: Posição no input original

**Retorno**: 
- Ponteiro para novo token
- NULL se falha de alocação

**Exemplo**:
```c
t_token *token = token_new(TOKEN_WORD, "echo", 0);
```

**Notas**:
- Aloca memória com malloc
- Duplica a string value
- Inicializa next como NULL
```

### Funções a documentar:
- `token_free()`
- `token_list_free()`
- `token_add_back()`
- `command_new()`
- `command_free()`
- `command_add_back()`
- E todas as funções públicas dos módulos

---

## 4. docs/SETUP.md

### Seções:

1. **Pré-requisitos**
   - Sistema operacional
   - Compilador (gcc)
   - Bibliotecas necessárias (readline)

2. **Instalação**
   - Clonar repositório
   - Compilar libft
   - Compilar projeto

3. **Compilação**
   ```bash
   make          # Compila
   make clean    # Remove objetos
   make fclean   # Remove tudo
   make re       # Recompila tudo
   ```

4. **Estrutura de Diretórios**
   ```
   include/    - Headers
   src/        - Código fonte
   lib/        - Bibliotecas
   obj/        - Objetos compilados
   ```

5. **Troubleshooting**
   - Erros comuns
   - Soluções

---

## 5. docs/CONTRIBUTING.md

### Para colaboradores:

1. **Como Contribuir**
   - Fork do projeto
   - Criar branch
   - Fazer mudanças
   - Pull request

2. **Convenções de Código**
   - Estilo 42 (norminette)
   - Naming conventions
   - Indentação (tabs)

3. **Estrutura de Branches**
   ```
   main                  - Código estável
   feature/lexer        - Desenvolvimento lexer
   feature/builtins     - Desenvolvimento builtins
   ```

4. **Como Testar**
   - Compilar
   - Rodar testes
   - Casos de teste

5. **Trabalho em Paralelo**
   - Módulos independentes
   - Interfaces/contratos
   - Como testar isoladamente

---

## 6. Comentários no Código

### Header de arquivo:

```c
/* ************************************************************************** */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*   Brief: Funções utilitárias para manipulação de tokens                   */
/* ************************************************************************** */
```

### Comentários de função:

```c
/*
** Creates a new token with given type, value and position.
** 
** @param type:  Token type (TOKEN_WORD, TOKEN_PIPE, etc)
** @param value: Token content (will be duplicated)
** @param pos:   Position in original input string
** 
** @return: Pointer to new token, or NULL if allocation fails
**
** Note: Caller must free returned token with token_free()
*/
t_token *token_new(t_token_type type, char *value, int pos)
{
    // ...
}
```

---

## 7. Diagramas (Opcional mas útil)

### docs/diagrams/pipeline_flow.txt

```
User Input
    ↓
┌─────────────┐
│   LEXER     │ ← Você implementa
└──────┬──────┘
       ↓
   [Tokens]
       ↓
┌─────────────┐
│   PARSER    │ ← Você implementa
└──────┬──────┘
       ↓
  [Commands]
       ↓
┌─────────────┐
│  EXECUTOR   │ ← Outra pessoa
└─────────────┘
```

### docs/diagrams/struct_diagram.txt

```
t_token                    t_command
┌──────────────┐          ┌──────────────┐
│ type         │          │ args[]       │
│ value        │          │ infile       │
│ pos          │          │ outfile      │
│ next    ─────┼──→       │ append       │
└──────────────┘          │ next    ─────┼──→
                          └──────────────┘
```

---

## 8. Changelog (Versionamento)

### docs/CHANGELOG.md

```markdown
# Changelog

## [Unreleased]
- Estruturas de dados implementadas
- Funções utilitárias criadas

## [0.1.0] - 2025-12-16
### Added
- Struct t_token com campos type, value, pos, next
- Struct t_command com args, infile, outfile, append, next
- Funções token_new, token_free, token_add_back
- Funções command_new, command_free, command_add_back
- Builtin exit básico

### Changed
- Refatorado Makefile da libft
- Removida separação entre bonus e all

## [0.0.1] - 2025-12-09
### Added
- Setup inicial do projeto
- Estrutura de diretórios
- Makefile básico
```

---

## Ordem Recomendada de Criação

1. **README.md** (raiz) - 15 min
   - Visão geral rápida

2. **docs/ARCHITECTURE.md** - 30 min
   - Documenta decisões importantes

3. **Comentários no código** - 20 min
   - Adiciona headers nas funções principais

4. **docs/API.md** - 40 min
   - Documenta cada função pública

5. **docs/SETUP.md** - 20 min
   - Como compilar e rodar

6. **docs/CONTRIBUTING.md** - 25 min
   - Guia para colaboradores

7. **docs/CHANGELOG.md** - 10 min
   - Histórico de mudanças

**Total estimado: ~2h30 de documentação**

---

## Ferramentas Úteis

### Diagramas ASCII:
- https://asciiflow.com/
- Para desenhar fluxos

### Markdown Preview:
- VS Code: Ctrl+Shift+V
- Verifica formatação

### Template README:
- https://github.com/othneildrew/Best-README-Template

---

## Dicas Práticas

### ✅ Boas Práticas:
- Documente enquanto desenvolve (não deixe acumular)
- Seja conciso (documentation debt é real)
- Use exemplos (código > texto)
- Mantenha atualizado (doc desatualizada é pior que sem doc)
- Pense no leitor (alguém que não conhece o projeto)

### ❌ Evite:
- Over-documentation (detalhes óbvios)
- Duplicação (info em múltiplos lugares)
- Documentar código ruim (refatore primeiro)
- Jargão excessivo
- Exemplos sem contexto

---

## Checklist de Documentação

```markdown
- [ ] README.md na raiz com visão geral
- [ ] ARCHITECTURE.md com decisões de design
- [ ] API.md com todas as funções públicas
- [ ] Comentários nas funções principais
- [ ] Exemplo de uso no README
- [ ] Instruções de compilação
- [ ] Status do projeto (o que funciona/não funciona)
- [ ] Como contribuir
- [ ] Changelog atualizado
- [ ] Diagramas de fluxo (opcional)
```

---

## Manutenção da Documentação

### Quando atualizar:

1. **A cada feature nova**: Atualizar README e CHANGELOG
2. **A cada função pública**: Atualizar API.md
3. **A cada decisão importante**: Atualizar ARCHITECTURE.md
4. **A cada mudança de interface**: Atualizar docs relevantes

### Review periódico:
- Semanal: Verificar se docs estão atualizadas
- Mensal: Refatorar documentação obsoleta
- A cada milestone: Review completo

---

## Exemplos de Documentação de Qualidade

### Projetos de referência:
- Redis: https://github.com/redis/redis
- Git: https://github.com/git/git
- Linux Kernel: https://www.kernel.org/doc/

### Boas práticas observadas:
- README conciso mas completo
- Exemplos práticos abundantes
- Diagramas quando necessário
- API bem documentada
- Contributing guide claro

---

## Notas Finais

- Documentação é para **humanos**, não para máquinas
- **Qualidade > Quantidade**
- Documentação ruim pode ser pior que nenhuma documentação
- Mantenha **consistência** de estilo
- Use **versionamento** (git) para histórico
- **Teste** os exemplos da documentação
- Peça **feedback** de outros desenvolvedores

---

**Última atualização**: 2025-12-16
**Versão**: 1.0
