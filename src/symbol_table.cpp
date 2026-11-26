#include "symbol_table.h"

#include <stdexcept>

void SymbolTable::set(const std::string& name, int value) {
    symbols_[name] = value;
}

int SymbolTable::get(const std::string& name) const {
    auto it = symbols_.find(name);

    if (it == symbols_.end()) {
        throw std::runtime_error(
            "Erro semântico: variável '" + name + "' não foi inicializada."
        );
    }

    return it->second;
}

bool SymbolTable::exists(const std::string& name) const {
    return symbols_.find(name) != symbols_.end();
}
