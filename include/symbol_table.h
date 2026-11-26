#ifndef MINICOMP_SYMBOL_TABLE_H
#define MINICOMP_SYMBOL_TABLE_H

#include <string>
#include <unordered_map>

class SymbolTable {
public:
    void set(const std::string& name, int value);

    int get(const std::string& name) const;

    bool exists(const std::string& name) const;

private:
    std::unordered_map<std::string, int> symbols_;
};

#endif
