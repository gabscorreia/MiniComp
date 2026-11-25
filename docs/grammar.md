# Gramática LL(1) da MiniLang

## Símbolo inicial

<program>

## Produções

<program> -> <block>

<block> -> LEFT_BRACE <stmt_list> RIGHT_BRACE

<stmt_list> -> <statement> <stmt_list>
             | ε

<statement> -> <assignment>
             | <read_stmt>
             | <write_stmt>
             | <if_stmt>
             | <while_stmt>

<assignment> -> IDENTIFIER ASSIGN <expr> SEMICOLON

<read_stmt> -> READ LEFT_PAREN IDENTIFIER RIGHT_PAREN SEMICOLON

<write_stmt> -> WRITE LEFT_PAREN <expr> RIGHT_PAREN SEMICOLON

<if_stmt> -> IF LEFT_PAREN <condition> RIGHT_PAREN
             THEN <block> <else_opt>

<else_opt> -> ELSE <block>
            | ε

<while_stmt> -> WHILE LEFT_PAREN <condition> RIGHT_PAREN
                DO <block>

<condition> -> <expr> <relop> <expr>

<relop> -> LESS
          | LESS_EQUAL
          | GREATER
          | GREATER_EQUAL
          | EQUAL
          | NOT_EQUAL

<expr> -> <term> <expr_tail>

<expr_tail> -> PLUS <term> <expr_tail>
             | MINUS <term> <expr_tail>
             | ε

<term> -> <factor> <term_tail>

<term_tail> -> MULTIPLY <factor> <term_tail>
             | DIVIDE <factor> <term_tail>
             | MODULO <factor> <term_tail>
             | ε

<factor> -> NUMBER
          | IDENTIFIER
          | LEFT_PAREN <expr> RIGHT_PAREN
