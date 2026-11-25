#include "ast.h"

#include <ostream>
#include <stdexcept>
#include <utility>

NumberExpr::NumberExpr(const std::string& value)
    : value(value) {}

VariableExpr::VariableExpr(const std::string& name)
    : name(name) {}

BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left,
                       TokenType op,
                       std::unique_ptr<Expr> right)
    : left(std::move(left)),
      op(op),
      right(std::move(right)) {}

AssignStmt::AssignStmt(const std::string& name,
                       std::unique_ptr<Expr> value)
    : name(name),
      value(std::move(value)) {}

ReadStmt::ReadStmt(const std::string& name)
    : name(name) {}

WriteStmt::WriteStmt(std::unique_ptr<Expr> value)
    : value(std::move(value)) {}

IfStmt::IfStmt(std::unique_ptr<Expr> condition,
               std::unique_ptr<BlockStmt> thenBlock,
               std::unique_ptr<BlockStmt> elseBlock)
    : condition(std::move(condition)),
      thenBlock(std::move(thenBlock)),
      elseBlock(std::move(elseBlock)) {}

WhileStmt::WhileStmt(std::unique_ptr<Expr> condition,
                     std::unique_ptr<BlockStmt> body)
    : condition(std::move(condition)),
      body(std::move(body)) {}

namespace {

std::string operatorName(TokenType op) {
    switch (op) {
        case TokenType::Plus:         return "Add (+)";
        case TokenType::Minus:        return "Subtract (-)";
        case TokenType::Star:         return "Multiply (*)";
        case TokenType::Slash:        return "Divide (/)";
        case TokenType::Percent:      return "Modulo (%)";
        case TokenType::Less:         return "LessThan (<)";
        case TokenType::LessEqual:    return "LessEqual (<=)";
        case TokenType::Greater:      return "GreaterThan (>)";
        case TokenType::GreaterEqual: return "GreaterEqual (>=)";
        case TokenType::Equal:        return "Equal (==)";
        case TokenType::NotEqual:     return "NotEqual (!=)";
        default:                      return "UnknownOperator";
    }
}

void printExpr(const Expr& expr,
               const std::string& prefix,
               bool isLast,
               std::ostream& output);

void printBlock(const BlockStmt& block,
                const std::string& prefix,
                bool isLast,
                std::ostream& output);

void printStmt(const Stmt& stmt,
               const std::string& prefix,
               bool isLast,
               std::ostream& output);

void printExpr(const Expr& expr,
               const std::string& prefix,
               bool isLast,
               std::ostream& output) {
    const std::string childPrefix =
        prefix + (isLast ? "    " : "│   ");

    output << prefix << (isLast ? "└── " : "├── ");

    if (const auto* number = dynamic_cast<const NumberExpr*>(&expr)) {
        output << "Number: " << number->value << '\n';
        return;
    }

    if (const auto* variable =
            dynamic_cast<const VariableExpr*>(&expr)) {
        output << "Variable: " << variable->name << '\n';
        return;
    }

    if (const auto* binary = dynamic_cast<const BinaryExpr*>(&expr)) {
        output << operatorName(binary->op) << '\n';

        printExpr(*binary->left, childPrefix, false, output);
        printExpr(*binary->right, childPrefix, true, output);
        return;
    }

    throw std::runtime_error("Tipo de expressão desconhecido na AST.");
}

void printBlock(const BlockStmt& block,
                const std::string& prefix,
                bool isLast,
                std::ostream& output) {
    output << prefix << (isLast ? "└── " : "├── ") << "Block\n";

    const std::string childPrefix =
        prefix + (isLast ? "    " : "│   ");

    for (std::size_t i = 0; i < block.statements.size(); ++i) {
        const bool last = (i + 1 == block.statements.size());

        printStmt(*block.statements[i], childPrefix, last, output);
    }
}

void printNamedBlock(const std::string& label,
                     const BlockStmt& block,
                     const std::string& prefix,
                     bool isLast,
                     std::ostream& output) {
    output << prefix << (isLast ? "└── " : "├── ")
           << label << '\n';

    printBlock(block,
               prefix + (isLast ? "    " : "│   "),
               true,
               output);
}

void printStmt(const Stmt& stmt,
               const std::string& prefix,
               bool isLast,
               std::ostream& output) {
    const std::string childPrefix =
        prefix + (isLast ? "    " : "│   ");

    output << prefix << (isLast ? "└── " : "├── ");

    if (const auto* assignment =
            dynamic_cast<const AssignStmt*>(&stmt)) {
        output << "Assign: " << assignment->name << '\n';
        printExpr(*assignment->value, childPrefix, true, output);
        return;
    }

    if (const auto* read = dynamic_cast<const ReadStmt*>(&stmt)) {
        output << "Read: " << read->name << '\n';
        return;
    }

    if (const auto* write = dynamic_cast<const WriteStmt*>(&stmt)) {
        output << "Write\n";
        printExpr(*write->value, childPrefix, true, output);
        return;
    }

    if (const auto* conditional =
            dynamic_cast<const IfStmt*>(&stmt)) {
        output << "If\n";

        output << childPrefix << "├── Condition\n";
        printExpr(*conditional->condition,
                  childPrefix + "│   ", true, output);

        printNamedBlock("Then",
                        *conditional->thenBlock,
                        childPrefix,
                        conditional->elseBlock == nullptr,
                        output);

        if (conditional->elseBlock) {
            printNamedBlock("Else",
                            *conditional->elseBlock,
                            childPrefix,
                            true,
                            output);
        }
        return;
    }

    if (const auto* loop = dynamic_cast<const WhileStmt*>(&stmt)) {
        output << "While\n";

        output << childPrefix << "├── Condition\n";
        printExpr(*loop->condition,
                  childPrefix + "│   ", true, output);

        printNamedBlock("Body",
                        *loop->body,
                        childPrefix,
                        true,
                        output);
        return;
    }

    if (const auto* block = dynamic_cast<const BlockStmt*>(&stmt)) {
        output << "Block\n";

        for (std::size_t i = 0; i < block->statements.size(); ++i) {
            const bool last = (i + 1 == block->statements.size());

            printStmt(*block->statements[i],
                      childPrefix,
                      last,
                      output);
        }
        return;
    }

    throw std::runtime_error("Tipo de comando desconhecido na AST.");
}

} // namespace

void printAST(const BlockStmt& ast, std::ostream& output) {
    output << "\nÁrvore Sintática Abstrata (AST):\n";
    output << "Block\n";

    for (std::size_t i = 0; i < ast.statements.size(); ++i) {
        const bool last = (i + 1 == ast.statements.size());

        printStmt(*ast.statements[i], "", last, output);
    }
}
