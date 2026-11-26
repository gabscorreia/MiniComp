#include "interpreter.h"
#include "lexer.h"
#include "parser.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

std::string runProgram(
    const std::string& source,
    const std::string& input = ""
) {
    Lexer lexer(source);
    const auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    if (!parser.errors().empty()) {
        throw std::runtime_error("Erro sintático no teste.");
    }

    std::istringstream inputStream(input);
    std::ostringstream outputStream;

    Interpreter interpreter(inputStream, outputStream);
    interpreter.interpret(*ast);

    return outputStream.str();
}

bool throwsRuntimeError(
    const std::string& source,
    const std::string& input = ""
) {
    try {
        runProgram(source, input);
    } catch (const std::runtime_error&) {
        return true;
    }

    return false;
}

int main() {
    // Precedência aritmética.
    assert(runProgram(
        "{ x = 10 + 5 * 2; write(x); }"
    ) == "20\n");

    // Associatividade à esquerda.
    assert(runProgram(
        "{ x = 20 - 5 - 3; write(x); }"
    ) == "12\n");

    // Operadores relacionais e if/else.
    assert(runProgram(
        "{ x = 15; "
        "if (x >= 10) then { write(1); } "
        "else { write(0); } }"
    ) == "1\n");

    // Laço while.
    assert(runProgram(
        "{ x = 3; "
        "while (x > 0) do { "
        "write(x); "
        "x = x - 1; "
        "} }"
    ) == "3\n2\n1\n");

    // Leitura e escrita.
    assert(runProgram(
        "{ read(x); write(x * 2); }",
        "7\n"
    ) == "14\n");

    // Operador módulo.
    assert(runProgram(
        "{ x = 10 % 3; write(x); }"
    ) == "1\n");

    // Variável não inicializada.
    assert(throwsRuntimeError(
        "{ write(x); }"
    ));

    // Divisão por zero.
    assert(throwsRuntimeError(
        "{ x = 10 / 0; write(x); }"
    ));

    // Módulo por zero.
    assert(throwsRuntimeError(
        "{ x = 10 % 0; write(x); }"
    ));

    // Entrada negativa.
    assert(throwsRuntimeError(
        "{ read(x); write(x); }",
        "-1\n"
    ));

    std::cout << "Todos os testes do interpretador passaram.\n";

    return 0;
}
