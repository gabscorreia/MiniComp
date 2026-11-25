#ifndef MINICOMP_PARSER_H
#define MINICOMP_PARSER_H

#include "ast.h"

#include <string>
#include <vector>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    std::unique_ptr<BlockStmt> parse();

private:
    const std::vector<Token>& tokens_;
    std::size_t current_;

    const Token& peek() const;
    const Token& advance();
    bool check(TokenType type) const;
    const Token& consume(TokenType type, const std::string& expected);
    [[noreturn]] void syntaxError(const std::string& message) const;

    std::unique_ptr<BlockStmt> parseBlock();
    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<Stmt> parseAssignment();
    std::unique_ptr<Stmt> parseRead();
    std::unique_ptr<Stmt> parseWrite();
    std::unique_ptr<Stmt> parseIf();
    std::unique_ptr<Stmt> parseWhile();

    std::unique_ptr<Expr> parseCondition();
    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();

    bool isRelationalOperator(TokenType type) const;
};

#endif
