#ifndef MINICOMP_AST_H
#define MINICOMP_AST_H

#include "lexer.h"

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

// Classe base para todas as expressões.
struct Expr {
    virtual ~Expr() = default;
};

// Número inteiro.
struct NumberExpr : Expr {
    std::string value;

    explicit NumberExpr(const std::string& value);
};

// Referência a uma variável.
struct VariableExpr : Expr {
    std::string name;

    explicit VariableExpr(const std::string& name);
};

// Expressão binária: soma, subtração, comparação etc.
struct BinaryExpr : Expr {
    std::unique_ptr<Expr> left;
    TokenType op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> left,
               TokenType op,
               std::unique_ptr<Expr> right);
};

// Classe base para todos os comandos.
struct Stmt {
    virtual ~Stmt() = default;
};

// Bloco de comandos.
struct BlockStmt : Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
};

// Atribuição: x = expressão;
struct AssignStmt : Stmt {
    std::string name;
    std::unique_ptr<Expr> value;

    AssignStmt(const std::string& name,
               std::unique_ptr<Expr> value);
};

// Leitura: read(x);
struct ReadStmt : Stmt {
    std::string name;

    explicit ReadStmt(const std::string& name);
};

// Escrita: write(expressão);
struct WriteStmt : Stmt {
    std::unique_ptr<Expr> value;

    explicit WriteStmt(std::unique_ptr<Expr> value);
};

// Condicional: if (...) then {...} else {...}
struct IfStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<BlockStmt> thenBlock;
    std::unique_ptr<BlockStmt> elseBlock;

    IfStmt(std::unique_ptr<Expr> condition,
           std::unique_ptr<BlockStmt> thenBlock,
           std::unique_ptr<BlockStmt> elseBlock);
};

// Repetição: while (...) do {...}
struct WhileStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<BlockStmt> body;

    WhileStmt(std::unique_ptr<Expr> condition,
              std::unique_ptr<BlockStmt> body);
};

// Exibe a árvore sintática abstrata.
void printAST(const BlockStmt& ast, std::ostream& output);

#endif
