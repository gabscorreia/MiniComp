#include "interpreter.h"

#include <stdexcept>
#include <istream>
#include <ostream>

Interpreter::Interpreter(std::istream& input, std::ostream& output)
    : input_(input), output_(output) {}

int Interpreter::evaluate(const Expr& expression) {
    if (const auto* number = dynamic_cast<const NumberExpr*>(&expression)) {
        return std::stoi(number->value);
    }

    if (const auto* variable = dynamic_cast<const VariableExpr*>(&expression)) {
        return symbols_.get(variable->name);
    }

    if (const auto* binary = dynamic_cast<const BinaryExpr*>(&expression)) {
        const int left = evaluate(*binary->left);
        const int right = evaluate(*binary->right);

        switch (binary->op) {
            case TokenType::Plus:
                return left + right;

            case TokenType::Minus:
                return left - right;

            case TokenType::Star:
                return left * right;

            case TokenType::Slash:
                if (right == 0) {
                    throw std::runtime_error(
                        "Erro semântico: divisão por zero."
                    );
                }
                return left / right;

            case TokenType::Percent:
                if (right == 0) {
                    throw std::runtime_error(
                        "Erro semântico: módulo por zero."
                    );
                }
                return left % right;

            case TokenType::Less:
                return left < right;

            case TokenType::LessEqual:
                return left <= right;

            case TokenType::Greater:
                return left > right;

            case TokenType::GreaterEqual:
                return left >= right;

            case TokenType::Equal:
                return left == right;

            case TokenType::NotEqual:
                return left != right;

            default:
                throw std::runtime_error(
                    "Erro semântico: operador desconhecido."
                );
        }
    }

    throw std::runtime_error(
        "Erro semântico: expressão desconhecida."
    );
}

void Interpreter::execute(const Stmt& statement) {
    // Bloco de comandos.
    if (const auto* block = dynamic_cast<const BlockStmt*>(&statement)) {
        executeBlock(*block);
        return;
    }

    // Atribuição.
    if (const auto* assignment = dynamic_cast<const AssignStmt*>(&statement)) {
        const int value = evaluate(*assignment->value);
        symbols_.set(assignment->name, value);
        return;
    }

    // Leitura.
    if (const auto* read = dynamic_cast<const ReadStmt*>(&statement)) {
        int value;

        if (!(input_ >> value)) {
            throw std::runtime_error(
                "Erro de execução: não foi possível ler um número inteiro."
            );
        }

        if (value < 0) {
            throw std::runtime_error(
                "Erro de execução: somente números não negativos são permitidos."
            );
        }

        symbols_.set(read->name, value);
        return;
    }

    // Escrita.
    if (const auto* write = dynamic_cast<const WriteStmt*>(&statement)) {
        output_ << evaluate(*write->value) << '\n';
        return;
    }

    // Condicional.
    if (const auto* conditional = dynamic_cast<const IfStmt*>(&statement)) {
        if (evaluate(*conditional->condition) != 0) {
            executeBlock(*conditional->thenBlock);
        } else if (conditional->elseBlock) {
            executeBlock(*conditional->elseBlock);
        }
        return;
    }

    // Repetição.
    if (const auto* loop = dynamic_cast<const WhileStmt*>(&statement)) {
        while (evaluate(*loop->condition) != 0) {
            executeBlock(*loop->body);
        }
        return;
    }

    throw std::runtime_error(
        "Erro de execução: comando desconhecido."
    );
}

void Interpreter::executeBlock(const BlockStmt& block) {
    for (const auto& statement : block.statements) {
        execute(*statement);
    }
}

void Interpreter::interpret(const BlockStmt& program) {
    executeBlock(program);
}
