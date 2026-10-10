#include "ast-printer.h"
#include <iomanip>
#include <sstream>
#include <variant>

std::string AstPrinter::print(const Expr &expression) {
	// start the recursion by calling accept on expression
	return expression.accept(*this);
}

std::string AstPrinter::visit_binary(const BinaryExpr &expression) {
	// parenthesize using the operator's lexeme and both children
	std::vector<const Expr *> children = {expression.left.get(),
										  expression.right.get()};
	return parenthesize(expression.op.lexeme, children);
}

std::string AstPrinter::visit_unary(const UnaryExpr &expression) {
	// parenthesize using the operator's lexeme and the one child
	std::vector<const Expr *> child = {expression.right.get()};
	return parenthesize(expression.op.lexeme, child);
}

std::string AstPrinter::visit_grouping(const GroupingExpr &expression) {
	// parenthesize using the fixed name "group" and the inner expression
	std::vector<const Expr *> expressionession = {expression.expr.get()};
	return parenthesize("group", expressionession);
}

// ------------------ Begin AI code ---------------------
std::string AstPrinter::visit_literal(const LiteralExpr &expression) {
	// check which alternative the variant holds
	// monostate -> nil, bool -> true/false, double -> formatted number,
	// string -> its text
	const Literal &v = expression.val;

	if (std::holds_alternative<std::monostate>(v)) {
		return "nil";
	}
	if (std::holds_alternative<bool>(v)) {
		return std::get<bool>(v) ? "true" : "false";
	}
	if (std::holds_alternative<double>(v)) {
		std::ostringstream out;
		out << std::setprecision(15) << std::get<double>(v);
		return out.str();
	}
	return std::get<std::string>(v);
	// otherwise it must be a string
	// return std::get<std::string>(v), optionally wrapped in quotes
}
// ------------------ End AI code ---------------------

// my formatter breaks this line weirdly but it works
std::string
AstPrinter::parenthesize(const std::string &name,
						 const std::vector<const Expr *> &children) {
	// build string with "(name child1 child2)" format
	std::string str = "(" + name;
	// call accept of each child
	for (const Expr *c : children) {
		str.append(" ");
		str.append(c->accept(*this));
	}
	return str + ")";
}
