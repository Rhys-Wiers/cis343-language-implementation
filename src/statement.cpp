#include "statement.h"
#include <utility>

// ---------------- ExpressionStmt ----------------
ExpressionStmt::ExpressionStmt(ExprPtr expression)
	: expression(std::move(expression)) {}

void ExpressionStmt::accept(StmtVisitor &visitor) const {
	visitor.visit_expression_stmt(*this);
}

// ---------------- PrintStmt ----------------
PrintStmt::PrintStmt(ExprPtr expression)
	: expression(std::move(expression)) {}

void PrintStmt::accept(StmtVisitor &visitor) const {
	visitor.visit_print_stmt(*this);
}

// ---------------- VariableStmt ----------------
VariableStmt::VariableStmt(Token n, std::unique_ptr<Expr> i)
	: name(std::move(n)), initializer(std::move(i)) {}

void VariableStmt::accept(StmtVisitor &visitor) const {
	visitor.visit_var_stmt(*this);
}

// ---------------- VariableStmt ----------------
Block::Block(std::vector<std::unique_ptr<Stmt>> s)
	: statements(std::move(s)) {}

void Block::accept(StmtVisitor &visitor) const {
	visitor.visit_block(*this);
}
