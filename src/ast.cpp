#include "ast.h"

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
