#pragma once

#include "expression.h"
#include <memory>

// Forward declarations
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
	explicit ExpressionStmt(ExprPtr expression);

	void accept(StmtVisitor &visitor) const override;

	ExprPtr expression;
};

class PrintStmt : public Stmt {
  public:
	explicit PrintStmt(ExprPtr expression);

	void accept(StmtVisitor &visitor) const override;

	ExprPtr expression;
};
