# Gramática LL(1) da MiniLang

## Símbolo inicial

<program>

## Produções

<program> -> <block> EndOfFile

<block> -> LeftBrace <stmt_list> RightBrace

<stmt_list> -> <statement> <stmt_list>
             | ε

<statement> -> <assignment>
             | <read_stmt>
             | <write_stmt>
             | <if_stmt>
             | <while_stmt>

<assignment> -> Identifier Assign <expr> Semicolon

<read_stmt> -> KwRead LeftParen Identifier RightParen Semicolon

<write_stmt> -> KwWrite LeftParen <expr> RightParen Semicolon

<if_stmt> -> KwIf LeftParen <condition> RightParen
             KwThen <block> <else_opt>

<else_opt> -> KwElse <block>
            | ε

<while_stmt> -> KwWhile LeftParen <condition> RightParen
                KwDo <block>

<condition> -> <expr> <relop> <expr>

<relop> -> Less
          | LessEqual
          | Greater
          | GreaterEqual
          | Equal
          | NotEqual

<expr> -> <term> <expr_tail>

<expr_tail> -> Plus <term> <expr_tail>
             | Minus <term> <expr_tail>
             | ε

<term> -> <factor> <term_tail>

<term_tail> -> Star <factor> <term_tail>
             | Slash <factor> <term_tail>
             | Percent <factor> <term_tail>
             | ε

<factor> -> Number
          | Identifier
          | LeftParen <expr> RightParen

## Conjuntos FIRST

| Não terminal | FIRST |
|---|---|
| `<program>` | { LeftBrace } |
| `<block>` | { LeftBrace } |
| `<stmt_list>` | { Identifier, KwRead, KwWrite, KwIf, KwWhile, ε } |
| `<statement>` | { Identifier, KwRead, KwWrite, KwIf, KwWhile } |
| `<assignment>` | { Identifier } |
| `<read_stmt>` | { KwRead } |
| `<write_stmt>` | { KwWrite } |
| `<if_stmt>` | { KwIf } |
| `<else_opt>` | { KwElse, ε } |
| `<while_stmt>` | { KwWhile } |
| `<condition>` | { Number, Identifier, LeftParen } |
| `<relop>` | { Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual } |
| `<expr>` | { Number, Identifier, LeftParen } |
| `<expr_tail>` | { Plus, Minus, ε } |
| `<term>` | { Number, Identifier, LeftParen } |
| `<term_tail>` | { Star, Slash, Percent, ε } |
| `<factor>` | { Number, Identifier, LeftParen } |

## Conjuntos FOLLOW

| Não terminal | FOLLOW |
|---|---|
| `<program>` | { EndOfFile } |
| `<block>` | { EndOfFile, KwElse, Identifier, KwRead, KwWrite, KwIf, KwWhile, RightBrace } |
| `<stmt_list>` | { RightBrace } |
| `<statement>` | { Identifier, KwRead, KwWrite, KwIf, KwWhile, RightBrace } |
| `<assignment>` | { Identifier, KwRead, KwWrite, KwIf, KwWhile, RightBrace } |
| `<read_stmt>` | { Identifier, KwRead, KwWrite, KwIf, KwWhile, RightBrace } |
| `<write_stmt>` | { Identifier, KwRead, KwWrite, KwIf, KwWhile, RightBrace } |
| `<if_stmt>` | { Identifier, KwRead, KwWrite, KwIf, KwWhile, RightBrace } |
| `<else_opt>` | { Identifier, KwRead, KwWrite, KwIf, KwWhile, RightBrace } |
| `<while_stmt>` | { Identifier, KwRead, KwWrite, KwIf, KwWhile, RightBrace } |
| `<condition>` | { RightParen } |
| `<relop>` | { Number, Identifier, LeftParen } |
| `<expr>` | { Semicolon, RightParen, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual } |
| `<expr_tail>` | { Semicolon, RightParen, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual } |
| `<term>` | { Plus, Minus, Semicolon, RightParen, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual } |
| `<term_tail>` | { Plus, Minus, Semicolon, RightParen, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual } |
| `<factor>` | { Star, Slash, Percent, Plus, Minus, Semicolon, RightParen, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual } |

## Verificação LL(1)

A gramática não apresenta conflitos LL(1) nas alternativas das produções.

As alternativas de `<statement>` possuem conjuntos FIRST disjuntos. O mesmo ocorre em `<factor>`, `<expr_tail>`, `<term_tail>` e `<else_opt>`.

Nas produções que possuem alternativa ε, seus conjuntos FIRST não vazios são disjuntos dos respectivos conjuntos FOLLOW.

Portanto, a gramática pode ser analisada com um único símbolo de antecipação (*lookahead*), característica de uma gramática LL(1).

O Parser implementado no projeto utiliza descida recursiva (*Top-Down*). A tabela preditiva LL(1) é apresentada como documentação formal da gramática, não como uma tabela consultada diretamente pelo código.

## Tabela Preditiva LL(1)

A tabela abaixo apresenta as produções selecionadas para cada combinação de não terminal e token de antecipação (*lookahead*). As combinações não listadas representam entradas de erro.

| Não terminal | Lookahead | Produção selecionada |
|---|---|---|
| `<program>` | LeftBrace | `<program> -> <block> EndOfFile` |
| `<block>` | LeftBrace | `<block> -> LeftBrace <stmt_list> RightBrace` |
| `<stmt_list>` | Identifier, KwRead, KwWrite, KwIf, KwWhile | `<stmt_list> -> <statement> <stmt_list>` |
| `<stmt_list>` | RightBrace | `<stmt_list> -> ε` |
| `<statement>` | Identifier | `<statement> -> <assignment>` |
| `<statement>` | KwRead | `<statement> -> <read_stmt>` |
| `<statement>` | KwWrite | `<statement> -> <write_stmt>` |
| `<statement>` | KwIf | `<statement> -> <if_stmt>` |
| `<statement>` | KwWhile | `<statement> -> <while_stmt>` |
| `<assignment>` | Identifier | `<assignment> -> Identifier Assign <expr> Semicolon` |
| `<read_stmt>` | KwRead | `<read_stmt> -> KwRead LeftParen Identifier RightParen Semicolon` |
| `<write_stmt>` | KwWrite | `<write_stmt> -> KwWrite LeftParen <expr> RightParen Semicolon` |
| `<if_stmt>` | KwIf | `<if_stmt> -> KwIf LeftParen <condition> RightParen KwThen <block> <else_opt>` |
| `<else_opt>` | KwElse | `<else_opt> -> KwElse <block>` |
| `<else_opt>` | Identifier, KwRead, KwWrite, KwIf, KwWhile, RightBrace | `<else_opt> -> ε` |
| `<while_stmt>` | KwWhile | `<while_stmt> -> KwWhile LeftParen <condition> RightParen KwDo <block>` |
| `<condition>` | Number, Identifier, LeftParen | `<condition> -> <expr> <relop> <expr>` |
| `<relop>` | Less | `<relop> -> Less` |
| `<relop>` | LessEqual | `<relop> -> LessEqual` |
| `<relop>` | Greater | `<relop> -> Greater` |
| `<relop>` | GreaterEqual | `<relop> -> GreaterEqual` |
| `<relop>` | Equal | `<relop> -> Equal` |
| `<relop>` | NotEqual | `<relop> -> NotEqual` |
| `<expr>` | Number, Identifier, LeftParen | `<expr> -> <term> <expr_tail>` |
| `<expr_tail>` | Plus | `<expr_tail> -> Plus <term> <expr_tail>` |
| `<expr_tail>` | Minus | `<expr_tail> -> Minus <term> <expr_tail>` |
| `<expr_tail>` | Semicolon, RightParen, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual | `<expr_tail> -> ε` |
| `<term>` | Number, Identifier, LeftParen | `<term> -> <factor> <term_tail>` |
| `<term_tail>` | Star | `<term_tail> -> Star <factor> <term_tail>` |
| `<term_tail>` | Slash | `<term_tail> -> Slash <factor> <term_tail>` |
| `<term_tail>` | Percent | `<term_tail> -> Percent <factor> <term_tail>` |
| `<term_tail>` | Plus, Minus, Semicolon, RightParen, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual | `<term_tail> -> ε` |
| `<factor>` | Number | `<factor> -> Number` |
| `<factor>` | Identifier | `<factor> -> Identifier` |
| `<factor>` | LeftParen | `<factor> -> LeftParen <expr> RightParen` |

### Verificação da tabela

Cada combinação de não terminal e lookahead possui no máximo uma produção.

As produções alternativas não apresentam conflitos nas entradas da tabela. Portanto, a tabela preditiva confirma que a gramática é LL(1).

EOF é o marcador de fim de arquivo e é consumido após a análise de `<block>`, conforme a produção inicial.

