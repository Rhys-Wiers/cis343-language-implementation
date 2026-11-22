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
