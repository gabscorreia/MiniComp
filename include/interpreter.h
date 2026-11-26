#ifndef MINICOMP_INTERPRETER_H
#define MINICOMP_INTERPRETER_H

#include "ast.h"
#include "symbol_table.h"

#include <iosfwd>

class Interpreter {
public:
    Interpreter(std::istream& input, std::ostream& output);

    void interpret(const BlockStmt& program);

private:
    SymbolTable symbols_;
    std::istream& input_;
    std::ostream& output_;

    int evaluate(const Expr& expression);

    void execute(const Stmt& statement);
    void executeBlock(const BlockStmt& block);
};

#endif
