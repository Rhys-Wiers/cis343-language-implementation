#pragma once

#include "expression.h"
#include "statement.h"
#include "token.h"
#include "value.h"
#include <stdexcept>
#include <string>

class RuntimeError : public std::runtime_error {
  public:
	RuntimeError(const Token &op, const std::string &message);
	Token op_;
};

class Evaluator : public Visitor, public StmtVisitor {
  public:
	// Statement Evaluation
	void evaluate(const Stmt &stmt);
	void visit_expression_stmt(const ExpressionStmt &stmt) override;
	void visit_print_stmt(const PrintStmt &stmt) override;

	// Expression Evaluation
	Value evaluate(const Expr &expr);
	Value visit_literal(const LiteralExpr &expr) override;
	Value visit_grouping(const GroupingExpr &expr) override;
	Value visit_unary(const UnaryExpr &expr) override;
	Value visit_binary(const BinaryExpr &expr) override;

  private:
	void check_number_operand(const Token &op, const Value &operand);
	void check_number_operands(const Token &op, const Value &left, const Value &right);
};
