#include "lexer.h"
#include "parser.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: mincomp <arquivo.min>\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Não foi possível abrir o arquivo: "
                  << argv[1] << '\n';
        return 1;
    }

    const std::string source(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>()
    );

    try {
        // Etapa 1: análise léxica.
        Lexer lexer(source);
        const auto tokens = lexer.tokenize();

        std::cout << "Análise léxica concluída. Tokens encontrados:\n";

        for (const Token& token : tokens) {
            std::cout << token.line << ':' << token.column << "  "
                      << tokenTypeName(token.type);

            if (!token.lexeme.empty()) {
                std::cout << "  \"" << token.lexeme << '"';
            }

            std::cout << '\n';
        }

        // Etapa 2: análise sintática e geração da AST.
        Parser parser(tokens);
        auto ast = parser.parse();

        (void)ast;

        std::cout << "\nAnálise sintática concluída.\n";
        std::cout << "AST gerada com sucesso.\n";

    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
