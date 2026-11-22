#pragma once

#include "token.h"
#include "value.h"
#include <memory>

struct BinaryExpr;
struct UnaryExpr;
struct LiteralExpr;
struct GroupingExpr;

struct Visitor {
	virtual ~Visitor() = default;
	virtual Value visit_binary(const BinaryExpr &expr) = 0;
	virtual Value visit_unary(const UnaryExpr &expr) = 0;
	virtual Value visit_literal(const LiteralExpr &expr) = 0;
	virtual Value visit_grouping(const GroupingExpr &expr) = 0;
};

struct Expr {
	virtual ~Expr() = default;
	virtual Value accept(Visitor &v) const = 0;
};

using ExprPtr = std::unique_ptr<Expr>;

struct BinaryExpr : Expr {
	ExprPtr left;
	Token op;
	ExprPtr right;
	BinaryExpr(ExprPtr l, Token o, ExprPtr r);
	Value accept(Visitor &v) const override;
};

struct UnaryExpr : Expr {
	Token op;
	ExprPtr right;
	UnaryExpr(Token o, ExprPtr r);
	Value accept(Visitor &v) const override;
};

struct LiteralExpr : Expr {
	Value val;
	explicit LiteralExpr(Literal v);
	Value accept(Visitor &v) const override;
};

struct GroupingExpr : Expr {
	ExprPtr expr;
	explicit GroupingExpr(ExprPtr e);
	Value accept(Visitor &v) const override;
};
