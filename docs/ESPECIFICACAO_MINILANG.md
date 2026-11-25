# Especificação inicial da MiniLang

## 1. Características gerais

- Linguagem imperativa.
- Valores inteiros não negativos.
- Escopo global para variáveis.
- Comandos: atribuição, leitura, escrita, `if-then-else` e `while-do`.
- Blocos delimitados por `{` e `}`.
- Comandos terminados por `;`.

## 2. Palavras reservadas

`read`, `write`, `if`, `then`, `else`, `while`, `do`.

Palavras reservadas são reconhecidas antes de identificadores. A linguagem diferencia maiúsculas de minúsculas.

## 3. Tokens

| Categoria | Lexemas |
|---|---|
| Identificador | letra ou `_`, seguido de letras, dígitos ou `_` |
| Número inteiro | uma ou mais casas decimais (`0` a `9`) |
| Aritméticos | `+`, `-`, `*`, `/`, `%` |
| Relacionais | `<`, `<=`, `>`, `>=`, `==`, `!=` |
| Atribuição | `=` |
| Delimitadores | `(`, `)`, `{`, `}`, `;` |
| Comentários | `// ...` e `/* ... */` |
| Fim de arquivo | EOF |

Os números são reconhecidos lexicalmente como sequências de dígitos. A validação do limite numérico e da semântica fica para etapas posteriores.

## 4. Sintaxe proposta

Exemplo de programa:

```text
{
    read(x);
    read(y);

    if (x > y) then {
        write(x);
    } else {
        write(y);
    }
}
```

## 5. Gramática inicial (EBNF)

```ebnf
program       = block ;
block         = "{" , { statement } , "}" ;

statement     = assignment
              | read_stmt
              | write_stmt
              | if_stmt
              | while_stmt ;

assignment    = identifier , "=" , expression , ";" ;
read_stmt     = "read" , "(" , identifier , ")" , ";" ;
write_stmt    = "write" , "(" , expression , ")" , ";" ;
if_stmt       = "if" , "(" , condition , ")" , "then" , block ,
                [ "else" , block ] ;
while_stmt    = "while" , "(" , condition , ")" , "do" , block ;

condition     = expression , relop , expression ;
relop         = "<" | "<=" | ">" | ">=" | "==" | "!=" ;

expression   = term , { ("+" | "-") , term } ;
term          = factor , { ("*" | "/" | "%") , factor } ;
factor        = integer | identifier | "(" , expression , ")" ;

identifier    = (letter | "_") , { letter | digit | "_" } ;
integer       = digit , { digit } ;
```

Esta gramática é uma especificação inicial legível. A versão formal efetivamente usada pelo parser LL(1), com não-terminais auxiliares, eliminação de recursão à esquerda e fatoração quando necessária, deverá ser registrada em `docs/grammar.md`.

## 6. Decisões e pontos a confirmar

1. A sintaxe de `read` e `write` foi definida com parênteses.
2. Os blocos usam chaves.
3. Atribuições e chamadas `read`/`write` terminam com ponto e vírgula.
4. `else` é opcional na proposta.
5. Foram propostos comentários de linha (`//`) e de bloco (`/* ... */`).
6. Foram propostos os operadores `%`, `==` e `!=`; confirme se fazem parte da linguagem esperada.
7. A expressão relacional foi definida como duas expressões separadas por um operador relacional.
8. Não foi incluído operador unário nem número negativo, respeitando a descrição de inteiros não negativos.

