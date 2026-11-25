#pragma once

#include "expression.h"
#include <memory>
#include <vector>

// Forward declarations
class Stmt;
class ExpressionStmt;
class PrintStmt;
class VariableStmt;
class Block;

using StmtPtr = std::unique_ptr<Stmt>;

class StmtVisitor {
  public:
	virtual ~StmtVisitor() = default;
	virtual void visit_expression_stmt(const ExpressionStmt &stmt) = 0;
	virtual void visit_print_stmt(const PrintStmt &stmt) = 0;
	virtual void visit_var_stmt(const VariableStmt &stmt) = 0;
	virtual void visit_block(const Block &stmt) = 0;
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

class VariableStmt : public Stmt {
  public:
	const Token name;
	const std::unique_ptr<Expr> initializer;

	VariableStmt(Token name, std::unique_ptr<Expr> initalizer);

	void accept(StmtVisitor &visito) const override;
};

class Block : public Stmt {
  public:
	explicit Block(std::vector<std::unique_ptr<Stmt>> statements);

	const std::vector<std::unique_ptr<Stmt>> statements;

	void accept(StmtVisitor &visitor) const override;
};
