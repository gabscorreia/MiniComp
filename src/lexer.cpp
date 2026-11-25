#include "lexer.h"

#include <cctype>
#include <stdexcept>
#include <unordered_map>

Lexer::Lexer(const std::string& source)
    : source_(source), position_(0), line_(1), column_(1) {}

bool Lexer::isAtEnd() const {
    return position_ >= source_.size();
}

char Lexer::peek() const {
    return isAtEnd() ? '\0' : source_[position_];
}

char Lexer::peekNext() const {
    return position_ + 1 >= source_.size() ? '\0' : source_[position_ + 1];
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';

    char current = source_[position_++];
    if (current == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }
    return current;
}

bool Lexer::match(char expected) {
    if (isAtEnd() || peek() != expected) return false;
    advance();
    return true;
}

void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        if (std::isspace(static_cast<unsigned char>(peek()))) {
            advance();
            continue;
        }

        if (peek() == '/' && peekNext() == '/') {
            while (!isAtEnd() && peek() != '\n') advance();
            continue;
        }

        if (peek() == '/' && peekNext() == '*') {
            const int commentLine = line_;
            const int commentColumn = column_;
            advance();
            advance();

            bool closed = false;
            while (!isAtEnd()) {
                if (peek() == '*' && peekNext() == '/') {
                    advance();
                    advance();
                    closed = true;
                    break;
                }
                advance();
            }

            if (!closed) {
                lexicalError("comentario de bloco não terminado",
                             commentLine, commentColumn);
            }
            continue;
        }

        break;
    }
}

Token Lexer::makeToken(TokenType type, std::size_t start,
                       int line, int column) const {
    return Token{type, source_.substr(start, position_ - start), line, column};
}

[[noreturn]] void Lexer::lexicalError(const std::string& message,
                                      int line, int column) const {
    throw std::runtime_error(
        "Erro léxico na linha " + std::to_string(line) +
        ", coluna " + std::to_string(column) + ": " + message);
}

Token Lexer::scanToken() {
    skipWhitespaceAndComments();

    const std::size_t start = position_;
    const int tokenLine = line_;
    const int tokenColumn = column_;

    if (isAtEnd()) {
        return Token{TokenType::EndOfFile, "", line_, column_};
    }

    const char c = advance();

    if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
        while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
            advance();
        }

        const std::string lexeme = source_.substr(start, position_ - start);
        static const std::unordered_map<std::string, TokenType> keywords = {
            {"read", TokenType::KwRead},
            {"write", TokenType::KwWrite},
            {"if", TokenType::KwIf},
            {"then", TokenType::KwThen},
            {"else", TokenType::KwElse},
            {"while", TokenType::KwWhile},
            {"do", TokenType::KwDo}
        };

        const auto found = keywords.find(lexeme);
        if (found != keywords.end()) {
            return makeToken(found->second, start, tokenLine, tokenColumn);
        }
        return makeToken(TokenType::Identifier, start, tokenLine, tokenColumn);
    }

    if (std::isdigit(static_cast<unsigned char>(c))) {
        while (std::isdigit(static_cast<unsigned char>(peek()))) {
            advance();
        }
        return makeToken(TokenType::Number, start, tokenLine, tokenColumn);
    }

    switch (c) {
        case '+': return makeToken(TokenType::Plus, start, tokenLine, tokenColumn);
        case '-': return makeToken(TokenType::Minus, start, tokenLine, tokenColumn);
        case '*': return makeToken(TokenType::Star, start, tokenLine, tokenColumn);
        case '%': return makeToken(TokenType::Percent, start, tokenLine, tokenColumn);
        case '(': return makeToken(TokenType::LeftParen, start, tokenLine, tokenColumn);
        case ')': return makeToken(TokenType::RightParen, start, tokenLine, tokenColumn);
        case '{': return makeToken(TokenType::LeftBrace, start, tokenLine, tokenColumn);
        case '}': return makeToken(TokenType::RightBrace, start, tokenLine, tokenColumn);
        case ';': return makeToken(TokenType::Semicolon, start, tokenLine, tokenColumn);

        case '/':
            return makeToken(TokenType::Slash, start, tokenLine, tokenColumn);

        case '=':
            return makeToken(match('=') ? TokenType::Equal : TokenType::Assign,
                             start, tokenLine, tokenColumn);

        case '!':
            if (match('=')) {
                return makeToken(TokenType::NotEqual, start, tokenLine, tokenColumn);
            }
            lexicalError("o caractere '!' deve ser seguido de '='",
                         tokenLine, tokenColumn);

        case '<':
            return makeToken(match('=') ? TokenType::LessEqual : TokenType::Less,
                             start, tokenLine, tokenColumn);

        case '>':
            return makeToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater,
                             start, tokenLine, tokenColumn);

        default:
            lexicalError(std::string("caractere não reconhecido: '") + c + "'",
                         tokenLine, tokenColumn);
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        Token token = scanToken();
        tokens.push_back(token);
        if (token.type == TokenType::EndOfFile) break;
    }

    return tokens;
}

const char* tokenTypeName(TokenType type) {
    switch (type) {
        case TokenType::Identifier: return "IDENTIFIER";
        case TokenType::Number: return "NUMBER";
        case TokenType::KwRead: return "READ";
        case TokenType::KwWrite: return "WRITE";
        case TokenType::KwIf: return "IF";
        case TokenType::KwThen: return "THEN";
        case TokenType::KwElse: return "ELSE";
        case TokenType::KwWhile: return "WHILE";
        case TokenType::KwDo: return "DO";
        case TokenType::Plus: return "PLUS";
        case TokenType::Minus: return "MINUS";
        case TokenType::Star: return "STAR";
        case TokenType::Slash: return "SLASH";
        case TokenType::Percent: return "PERCENT";
        case TokenType::Assign: return "ASSIGN";
        case TokenType::Equal: return "EQUAL";
        case TokenType::NotEqual: return "NOT_EQUAL";
        case TokenType::Less: return "LESS";
        case TokenType::LessEqual: return "LESS_EQUAL";
        case TokenType::Greater: return "GREATER";
        case TokenType::GreaterEqual: return "GREATER_EQUAL";
        case TokenType::LeftParen: return "LEFT_PAREN";
        case TokenType::RightParen: return "RIGHT_PAREN";
        case TokenType::LeftBrace: return "LEFT_BRACE";
        case TokenType::RightBrace: return "RIGHT_BRACE";
        case TokenType::Semicolon: return "SEMICOLON";
        case TokenType::EndOfFile: return "EOF";
    }
    return "UNKNOWN";
}
