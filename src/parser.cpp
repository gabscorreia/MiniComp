#include "parser.h"

#include <memory>
#include <stdexcept>
#include <utility>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens), current_(0) {

    if (tokens_.empty() ||
        tokens_.back().type != TokenType::EndOfFile) {
        throw std::invalid_argument(
            "Sequência de tokens inválida: marcador EOF ausente."
        );
    }
}
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

const std::vector<std::string>& Parser::errors() const {
    return errors_;
}

bool Parser::isStatementStart(TokenType type) const {
    switch (type) {
        case TokenType::Identifier:
        case TokenType::KwRead:
        case TokenType::KwWrite:
        case TokenType::KwIf:
        case TokenType::KwWhile:
            return true;

        default:
            return false;
    }
}

// Procura um ponto seguro para retomar a análise.
void Parser::synchronize() {
    int parentheses = 0;
    int braces = 0;

    while (!check(TokenType::EndOfFile)) {

        if (parentheses == 0 &&
            braces == 0 &&
            isStatementStart(peek().type)) {
            return;
        }

        if (check(TokenType::LeftParen)) {
            ++parentheses;
        }
        else if (check(TokenType::RightParen)) {
             if (parentheses == 0) {
                 advance();
                 return;
        }
             --parentheses;
       }
        else if (check(TokenType::LeftBrace)) {
            ++braces;
        }
        else if (check(TokenType::RightBrace)) {
            if (braces == 0) {
                return;
            }
            --braces;
        }
        else if (check(TokenType::Semicolon) &&
                 parentheses == 0 &&
                 braces == 0) {
            advance();
            return;
        }

        advance();
    }
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
        try {
            auto statement = parseStatement();

            if (statement) {
                block->statements.push_back(std::move(statement));
            }
        } catch (const std::runtime_error& error) {
            errors_.push_back(error.what());
            synchronize();
        }
    }

    if (check(TokenType::EndOfFile)) {
        errors_.push_back(
            "Erro sintático na linha " +
            std::to_string(peek().line) +
            ", coluna " +
            std::to_string(peek().column) +
            ": esperado '}', encontrado EOF"
        );
    } else {
        consume(TokenType::RightBrace, "'}'");
    }

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

    std::unique_ptr<Expr> condition;

    try {
        condition = parseCondition();
    } catch (const std::runtime_error& error) {
        errors_.push_back(error.what());

        consume(TokenType::RightParen, "')'");
        consume(TokenType::KwThen, "'then'");

        parseBlock();

        if (check(TokenType::KwElse)) {
            advance();
            parseBlock();
        }

        return nullptr;
    }

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

    std::unique_ptr<Expr> condition;

    try {
        condition = parseCondition();
    } catch (const std::runtime_error& error) {
        errors_.push_back(error.what());

        consume(TokenType::RightParen, "')'");
        consume(TokenType::KwDo, "'do'");

        parseBlock();

        return nullptr;
    }

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
