#pragma once
#include "expression.h"
#include "value.h"
#include <memory>

// Forward declare visitor
class Stmt;
class ExpressionStmt;
class PrintStmt;

using StmtPtr = std::unique_ptr<Stmt>;

class StmtVisitor {
  public:
	virtual ~StmtVisitor() = default;
	virtual void visit_expression_stmt(const ExpressionStmt &stmt) = 0;
	virtual void visit_print_stmt(const PrintStmt &stmt) = 0;
};

class Stmt {
  public:
	virtual ~Stmt() = default;
	virtual void accept(StmtVisitor &visitor) const = 0;
};

class ExpressionStmt : public Stmt {
  public:
	explicit ExpressionStmt(std::unique_ptr<Expr> expression)
		: expression(std::move(expression)) {}

	void accept(StmtVisitor &visitor) const override;

	std::unique_ptr<Expr> expression;
};

class PrintStmt : public Stmt {
  public:
	explicit PrintStmt(std::unique_ptr<Expr> expression)
		: expression(std::move(expression)) {}

	void accept(StmtVisitor &visitor) const override;

	std::unique_ptr<Expr> expression;
};

inline void ExpressionStmt::accept(StmtVisitor &visitor) const {
	visitor.visit_expression_stmt(*this);
}

inline void PrintStmt::accept(StmtVisitor &visitor) const {
	visitor.visit_print_stmt(*this);
}
