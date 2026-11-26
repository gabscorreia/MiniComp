#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    const bool checkOnly = (argc == 3 &&
                            std::string(argv[1]) == "--check");

    if ((!checkOnly && argc != 2) ||
        (checkOnly && argc != 3)) {
        std::cerr << "Uso: mincomp [--check] <arquivo.min>\n";
        return 1;
    }

    const char* filename = checkOnly ? argv[2] : argv[1];

    std::ifstream input(filename);

    if (!input) {
        std::cerr << "Não foi possível abrir o arquivo: "
                  << filename << '\n';
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

        if (!parser.errors().empty()) {
            std::cerr << "\nErros sintáticos encontrados:\n";

            for (const std::string& error : parser.errors()) {
                std::cerr << error << '\n';
            }

            std::cout << "\nAST parcial gerada após recuperação.\n";
            printAST(*ast, std::cout);
            return 1;
        }

        std::cout << "\nAnálise sintática concluída.\n";
        std::cout << "AST gerada com sucesso.\n";

        printAST(*ast, std::cout);

        // Modo de verificação: não executa o programa.
        if (checkOnly) {
            return 0;
        }

        // Etapa 3: interpretação e execução.
        std::cout << "\nIniciando execução:\n";

        Interpreter interpreter(std::cin, std::cout);
        interpreter.interpret(*ast);

        std::cout << "\nExecução concluída com sucesso.\n";

    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
