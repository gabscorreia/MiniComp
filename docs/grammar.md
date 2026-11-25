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
