#include "ast-printer.h"
#include <iomanip>
#include <sstream>
#include <variant>

std::string AstPrinter::print(const Expr &expression) {
	// start the recursion by calling accept on expression
	return to_string(expression.accept(*this));
}

Value AstPrinter::visit_binary(const BinaryExpr &expression) {
	// parenthesize using the operator's lexeme and both children
	std::vector<const Expr *> children = {expression.left.get(), expression.right.get()};
	return parenthesize(expression.op.lexeme, children);
}

Value AstPrinter::visit_unary(const UnaryExpr &expression) {
	// parenthesize using the operator's lexeme and the one child
	std::vector<const Expr *> child = {expression.right.get()};
	return parenthesize(expression.op.lexeme, child);
}

Value AstPrinter::visit_grouping(const GroupingExpr &expression) {
	// parenthesize using the fixed name "group" and the inner expression
	std::vector<const Expr *> expressionession = {expression.expr.get()};
	return parenthesize("group", expressionession);
}

Value AstPrinter::visit_literal(const LiteralExpr &expression) {
	return to_string(expression.val);
}

std::string AstPrinter::parenthesize(const std::string &name, const std::vector<const Expr *> &children) {
	// build string with "(name child1 child2)" format
	std::string str = "(" + name;
	// call accept of each child
	for (const Expr *c : children) {
		str.append(" ");
		str.append(to_string(c->accept(*this)));
	}
	return str + ")";
}
