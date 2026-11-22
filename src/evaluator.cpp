#include "evaluator.h"
#include <cmath>
#include <iostream>
#include <variant>

RuntimeError::RuntimeError(const Token &op, const std::string &message)
	: std::runtime_error(message), op_(op) {}

// ---------------- Statement Evaluation ----------------

void Evaluator::evaluate(const Stmt &stmt) {
	stmt.accept(*this);
}

void Evaluator::visit_expression_stmt(const ExpressionStmt &stmt) {
	// Evaluate expression for side effects, discard returned Value
	evaluate(*stmt.expression);
}

void Evaluator::visit_print_stmt(const PrintStmt &stmt) {
	Value value = evaluate(*stmt.expression);
	std::cout << to_string(value) << '\n';
}

// ---------------- Expression Evaluation ----------------

Value Evaluator::evaluate(const Expr &expr) {
	return expr.accept(*this);
}

Value Evaluator::visit_literal(const LiteralExpr &expr) {
	return expr.val;
}

Value Evaluator::visit_grouping(const GroupingExpr &expr) {
	return evaluate(*expr.expr);
}

Value Evaluator::visit_unary(const UnaryExpr &expr) {
	Value right = evaluate(*expr.right);

	if (expr.op.type == TokenType::Minus) {
		check_number_operand(expr.op, right);
		return -std::get<double>(right);
	} else if (expr.op.type == TokenType::Bang) {
		return !is_truthy(right);
	}

	return std::monostate{};
}

Value Evaluator::visit_binary(const BinaryExpr &expr) {
	Value left = evaluate(*expr.left);
	Value right = evaluate(*expr.right);

	if (expr.op.type == TokenType::Plus) {
		if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
			return std::get<double>(left) + std::get<double>(right);
		if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
			return std::get<std::string>(left) + std::get<std::string>(right);
		throw RuntimeError(expr.op, "Operands must be two numbers or two strings.");
	} else if (expr.op.type == TokenType::Minus) {
		check_number_operands(expr.op, left, right);
		return std::get<double>(left) - std::get<double>(right);
	} else if (expr.op.type == TokenType::Star) {
		check_number_operands(expr.op, left, right);
		return std::get<double>(left) * std::get<double>(right);
	} else if (expr.op.type == TokenType::Slash) {
		check_number_operands(expr.op, left, right);
		if (std::get<double>(right) == 0.0) {
			throw RuntimeError(expr.op, "Division by zero.");
		}
		return std::get<double>(left) / std::get<double>(right);
	} else if (expr.op.type == TokenType::Remainder) {
		check_number_operands(expr.op, left, right);
		if (std::get<double>(right) == 0.0) {
			throw RuntimeError(expr.op, "Division by zero.");
		}
		return std::fmod(std::get<double>(left), std::get<double>(right));
	} else if (expr.op.type == TokenType::Greater) {
		check_number_operands(expr.op, left, right);
		return std::get<double>(left) > std::get<double>(right);
	} else if (expr.op.type == TokenType::GreaterEqual) {
		check_number_operands(expr.op, left, right);
		return std::get<double>(left) >= std::get<double>(right);
	} else if (expr.op.type == TokenType::Less) {
		check_number_operands(expr.op, left, right);
		return std::get<double>(left) < std::get<double>(right);
	} else if (expr.op.type == TokenType::LessEqual) {
		check_number_operands(expr.op, left, right);
		return std::get<double>(left) <= std::get<double>(right);
	} else if (expr.op.type == TokenType::EqualEqual) {
		return is_equal(left, right);
	} else if (expr.op.type == TokenType::BangEqual) {
		return !is_equal(left, right);
	} else if (expr.op.type == TokenType::Ampersands) {
		return is_truthy(left) && is_truthy(right);
	} else if (expr.op.type == TokenType::Verts) {
		return is_truthy(left) || is_truthy(right);
	}

	return std::monostate{};
}

// ---------------- Helpers ----------------

void Evaluator::check_number_operand(const Token &op, const Value &operand) {
	if (std::holds_alternative<double>(operand))
		return;
	throw RuntimeError(op, "Operand must be a number.");
}

void Evaluator::check_number_operands(const Token &op, const Value &left, const Value &right) {
	if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
		return;
	throw RuntimeError(op, "Operands must be numbers.");
}
