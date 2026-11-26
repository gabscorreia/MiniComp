#include "symbol_table.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
    SymbolTable symbols;

    // Verifica variável inexistente.
    assert(!symbols.exists("x"));

    // Armazena e consulta um valor.
    symbols.set("x", 10);
    assert(symbols.exists("x"));
    assert(symbols.get("x") == 10);

    // Atualiza o valor da variável.
    symbols.set("x", 25);
    assert(symbols.get("x") == 25);

    // Verifica o erro ao consultar variável inexistente.
    bool errorOccurred = false;

    try {
        symbols.get("y");
    } catch (const std::runtime_error&) {
        errorOccurred = true;
    }

    assert(errorOccurred);

    std::cout << "Todos os testes da tabela de simbolos passaram.\n";

    return 0;
}
