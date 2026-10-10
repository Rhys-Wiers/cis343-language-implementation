#pragma once
#include "expression.h"
#include <string>
#include <vector>

class AstPrinter : public Visitor {
  public:
	// entry point: takes the root of a tree and returns its printed form
	std::string print(const Expr &expr);

	// one override per node type, matching Visitor exactly
	std::string visit_binary(const BinaryExpr &expr) override;
	std::string visit_unary(const UnaryExpr &expr) override;
	std::string visit_literal(const LiteralExpr &expr) override;
	std::string visit_grouping(const GroupingExpr &expr) override;

  private:
	// shared helper: "(" + name + " " + each child's printed form + ")"
	std::string parenthesize(const std::string &name,
							 const std::vector<const Expr *> &children);
};
