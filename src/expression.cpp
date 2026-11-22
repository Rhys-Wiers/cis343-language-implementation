#include "expression.h"
#include <utility>

// ---------------- BinaryExpr ----------------

BinaryExpr::BinaryExpr(ExprPtr l, Token o, ExprPtr r)
	: left(std::move(l)), op(std::move(o)), right(std::move(r)) {}

Value BinaryExpr::accept(Visitor &v) const {
	return v.visit_binary(*this);
}

// ---------------- UnaryExpr ----------------

UnaryExpr::UnaryExpr(Token o, ExprPtr r)
	: op(std::move(o)), right(std::move(r)) {}

Value UnaryExpr::accept(Visitor &v) const {
	return v.visit_unary(*this);
}

// ---------------- LiteralExpr ----------------

LiteralExpr::LiteralExpr(Literal v)
	: val(std::move(v)) {}

Value LiteralExpr::accept(Visitor &v) const {
	return v.visit_literal(*this);
}

// ---------------- GroupingExpr ----------------

GroupingExpr::GroupingExpr(ExprPtr e)
	: expr(std::move(e)) {}

Value GroupingExpr::accept(Visitor &v) const {
	return v.visit_grouping(*this);
}
