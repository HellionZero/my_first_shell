# Relatório de Problemas do Builtin Echo

## Resumo dos Testes
**Total de testes:** 52  
**Passaram:** 38  
**Falharam:** 14  

## Problemas Identificados

### 1. ❌ **FALSO POSITIVO: echo -n funciona corretamente**
Os testes indicaram que `echo -n` não estava funcionando, mas na verdade **está funcionando perfeitamente**.

**Problema:** O script de teste estava filtrando incorretamente as linhas de saída, removendo o prompt `[my_shell]>` junto com a saída do `echo -n` (que não tem newline, então aparece na mesma linha do próximo prompt).

**Evidência:**
```bash
$ printf 'echo -n hello\n' | ./my_shell 2>&1 | tail -2 | od -c
# Saída: hello[my_shell]> exit
# O "hello" está lá, sem newline!
```

**Status:** ✅ echo -n funciona corretamente

---

### 2. ⚠️ **BUG REAL: Tabs dentro de aspas duplas estão sendo removidos**

**Problema:** Quando um tab literal é inserido dentro de aspas duplas, ele é completamente removido do output.

**Teste falhado:**
```bash
bash:     echo "hello	world"  →  "hello	world"  (preserva tab)
my_shell: echo "hello	world"  →  "helloworld"    (remove tab)
```

**Causa raiz:** No arquivo [src/lexer/lexer_quotes.c](src/lexer/lexer_quotes.c), função `extract_quote_content()`:

```c
while (input[*i] && input[*i] != quote_char)
{
    if (quote_char == '"' && input[*i] == '\\' && input[*i + 1])
        (*i)++;  // Pula o próximo caractere após backslash
    (*i)++;
}
```

Quando encontra um tab (ASCII 9, `\t`), o tab está sendo pulado/ignorado em algum lugar do processamento, provavelmente porque:
- Tabs literais (não `\t` como string, mas o caractere real tab) dentro de aspas duplas devem ser preservados
- O lexer pode estar tratando tabs como delimitadores mesmo dentro de aspas

**Comportamento esperado (POSIX):**
- Dentro de aspas duplas, apenas `\"`, `\\`, `\$`, `` \` `` e `\newline` devem ser processados como escapes
- Tabs literais devem ser preservados como estão
- A string `\t` (backslash + t) também deve ser preservada literalmente

**Status:** 🐛 BUG CONFIRMADO - necessita correção

---

## Análise Adicional Necessária

### Investigar processamento de caracteres especiais
- Verificar se outros caracteres de whitespace (como `\n`, `\r`) também são afetados
- Confirmar se o problema está no lexer ou no processamento posterior
- Testar comportamento com aspas simples (que devem preservar TUDO literalmente)

### Testes adicionais recomendados
```bash
# Tabs em aspas simples
echo 'hello	world'

# Newlines preservados
echo "hello
world"

# Outros espaços em branco
echo "a  b  c"
```

---

## Recomendações

### Prioridade ALTA
1. ✅ **Corrigir script de teste** - Modificar `test_echo.sh` para capturar corretamente a saída de `echo -n`
2. 🔧 **Corrigir preservação de tabs** - Modificar o lexer para preservar tabs literais dentro de aspas duplas

### Prioridade MÉDIA
3. 📝 **Adicionar testes de regressão** - Garantir que futuras mudanças não quebrem o comportamento correto
4. 🧪 **Testar todos os caracteres especiais** - Verificar comportamento com todos os tipos de whitespace

---

## Código do Echo está CORRETO
A implementação em [src/builtins/echo.c](src/builtins/echo.c) está funcionando conforme esperado:
- ✅ Detecta flag `-n` corretamente (incluindo `-nn`, `-nnn`, etc.)
- ✅ Para de processar flags quando encontra algo que não é `-n`
- ✅ Imprime argumentos com espaços entre eles
- ✅ Suprime newline quando `-n` está presente

**Nenhuma mudança necessária no builtin echo.**
