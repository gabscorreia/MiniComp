#include "parser.h"

#include <memory>
#include <stdexcept>
#include <utility>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens), current_(0) {}

const Token& Parser::peek() const {
    return tokens_[current_];
}

const Token& Parser::advance() {
    const Token& currentToken = peek();

    if (!check(TokenType::EndOfFile)) {
        ++current_;
    }

    return currentToken;
}
bool Parser::check(TokenType type) const {
    return peek().type == type;
}

const Token& Parser::consume(TokenType type,
                             const std::string& expected) {
    if (check(type)) {
        return advance();
    }

    syntaxError("esperado " + expected +
                ", encontrado " + tokenTypeName(peek().type));
}

[[noreturn]] void Parser::syntaxError(
    const std::string& message) const {
    throw std::runtime_error(
        "Erro sintático na linha " +
        std::to_string(peek().line) +
        ", coluna " +
        std::to_string(peek().column) +
        ": " + message);
}

std::unique_ptr<BlockStmt> Parser::parse() {
    auto program = parseBlock();

    consume(TokenType::EndOfFile, "fim do arquivo");

    return program;
}

std::unique_ptr<BlockStmt> Parser::parseBlock() {
    consume(TokenType::LeftBrace, "'{'");

    auto block = std::make_unique<BlockStmt>();

    while (!check(TokenType::RightBrace) &&
           !check(TokenType::EndOfFile)) {
        block->statements.push_back(parseStatement());
    }

    consume(TokenType::RightBrace, "'}'");

    return block;
}

std::unique_ptr<Stmt> Parser::parseStatement() {
    switch (peek().type) {
        case TokenType::Identifier:
            return parseAssignment();

        case TokenType::KwRead:
            return parseRead();

        case TokenType::KwWrite:
            return parseWrite();

        case TokenType::KwIf:
            return parseIf();

        case TokenType::KwWhile:
            return parseWhile();

        default:
            syntaxError(
                "comando não reconhecido: " +
                std::string(tokenTypeName(peek().type))
            );
    }
}

std::unique_ptr<Stmt> Parser::parseAssignment() {
    const std::string name =
        consume(TokenType::Identifier, "identificador").lexeme;

    consume(TokenType::Assign, "'='");

    auto value = parseExpression();

    consume(TokenType::Semicolon, "';'");

    return std::make_unique<AssignStmt>(name, std::move(value));
}

std::unique_ptr<Stmt> Parser::parseRead() {
    consume(TokenType::KwRead, "'read'");
    consume(TokenType::LeftParen, "'('");

    const std::string name =
        consume(TokenType::Identifier, "identificador").lexeme;

    consume(TokenType::RightParen, "')'");
    consume(TokenType::Semicolon, "';'");

    return std::make_unique<ReadStmt>(name);
}

std::unique_ptr<Stmt> Parser::parseWrite() {
    consume(TokenType::KwWrite, "'write'");
    consume(TokenType::LeftParen, "'('");

    auto value = parseExpression();

    consume(TokenType::RightParen, "')'");
    consume(TokenType::Semicolon, "';'");

    return std::make_unique<WriteStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::parseIf() {
    consume(TokenType::KwIf, "'if'");
    consume(TokenType::LeftParen, "'('");

    auto condition = parseCondition();

    consume(TokenType::RightParen, "')'");
    consume(TokenType::KwThen, "'then'");

    auto thenBlock = parseBlock();
    std::unique_ptr<BlockStmt> elseBlock;

    if (check(TokenType::KwElse)) {
        advance();
        elseBlock = parseBlock();
    }

    return std::make_unique<IfStmt>(
        std::move(condition),
        std::move(thenBlock),
        std::move(elseBlock)
    );
}

std::unique_ptr<Stmt> Parser::parseWhile() {
    consume(TokenType::KwWhile, "'while'");
    consume(TokenType::LeftParen, "'('");

    auto condition = parseCondition();

    consume(TokenType::RightParen, "')'");
    consume(TokenType::KwDo, "'do'");

    auto body = parseBlock();

    return std::make_unique<WhileStmt>(
        std::move(condition),
        std::move(body)
    );
}

bool Parser::isRelationalOperator(TokenType type) const {
    switch (type) {
        case TokenType::Less:
        case TokenType::LessEqual:
        case TokenType::Greater:
        case TokenType::GreaterEqual:
        case TokenType::Equal:
        case TokenType::NotEqual:
            return true;

        default:
            return false;
    }
}

std::unique_ptr<Expr> Parser::parseCondition() {
    auto left = parseExpression();

    if (!isRelationalOperator(peek().type)) {
        syntaxError("operador relacional esperado");
    }

    const TokenType op = advance().type;
    auto right = parseExpression();

    return std::make_unique<BinaryExpr>(
        std::move(left), op, std::move(right)
    );
}

std::unique_ptr<Expr> Parser::parseExpression() {
    auto expression = parseTerm();

    while (check(TokenType::Plus) ||
           check(TokenType::Minus)) {
        const TokenType op = advance().type;
        auto right = parseTerm();

        expression = std::make_unique<BinaryExpr>(
            std::move(expression), op, std::move(right)
        );
    }

    return expression;
}

std::unique_ptr<Expr> Parser::parseTerm() {
    auto term = parseFactor();

    while (check(TokenType::Star) ||
           check(TokenType::Slash) ||
           check(TokenType::Percent)) {
        const TokenType op = advance().type;
        auto right = parseFactor();

        term = std::make_unique<BinaryExpr>(
            std::move(term), op, std::move(right)
        );
    }

    return term;
}

std::unique_ptr<Expr> Parser::parseFactor() {
    if (check(TokenType::Number)) {
        const std::string value = advance().lexeme;
        return std::make_unique<NumberExpr>(value);
    }

    if (check(TokenType::Identifier)) {
        const std::string name = advance().lexeme;
        return std::make_unique<VariableExpr>(name);
    }

    if (check(TokenType::LeftParen)) {
        advance();

        auto expression = parseExpression();

        consume(TokenType::RightParen, "')'");

        return expression;
    }

    syntaxError("expressão esperada");
}
