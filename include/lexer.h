#ifndef MINICOMP_LEXER_H
#define MINICOMP_LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    Identifier,
    Number,

    KwRead,
    KwWrite,
    KwIf,
    KwThen,
    KwElse,
    KwWhile,
    KwDo,

    Plus,
    Minus,
    Star,
    Slash,
    Percent,

    Assign,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,

    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Semicolon,

    EndOfFile
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

class Lexer {
public:
    explicit Lexer(const std::string& source);

    std::vector<Token> tokenize();

private:
    std::string source_;
    std::size_t position_;
    int line_;
    int column_;

    bool isAtEnd() const;
    char peek() const;
    char peekNext() const;
    char advance();
    bool match(char expected);
    void skipWhitespaceAndComments();
    Token makeToken(TokenType type, std::size_t start, int line, int column) const;
    Token scanToken();
    [[noreturn]] void lexicalError(const std::string& message, int line, int column) const;
};

const char* tokenTypeName(TokenType type);

#endif
