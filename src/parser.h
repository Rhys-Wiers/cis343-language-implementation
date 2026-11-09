#pragma once

#include "expression.h"
#include "statement.h"
#include "token.h"
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// recursive descent parser: tokens -> expression tree
class Parser {
  public:
	explicit Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

	// Returns nullptr when there is an error
	ExprPtr parse() {
		try {
			ExprPtr expr = expression();
			if (!is_at_end())
				throw error(peek(), "Expected end of expression.");
			return expr;
		} catch (const ParseError &) {
			return nullptr;
		}
	}
	bool had_error() const { return had_error_; }

  private:
	// define parse error type, with same behavior as runtime error type
	struct ParseError : std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	std::vector<Token> tokens_;
	size_t current_ = 0; // next token to consume
	bool had_error_ = false;

	// grammar rules, in order of precedence
	ExprPtr expression() { return logic_or(); }

	// logic_or -> logic_and ( "||" logic_and )*
	ExprPtr logic_or() {
		ExprPtr expr = logic_and();
		while (match({TokenType::Verts})) { // ||
			Token op = previous();
			ExprPtr right = logic_and();
			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}
		return expr;
	}

	// logic_and -> equality ( "&&" equality )*
	ExprPtr logic_and() {
		ExprPtr expr = equality();
		while (match({TokenType::Ampersands})) { // &&
			Token op = previous();
			ExprPtr right = equality();
			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}
		return expr;
	}

	// equality -> comparison ( ( "!=" | "==" ) comparison )*
	ExprPtr equality() {
		ExprPtr expr = comparison();
		while (match({TokenType::BangEqual, TokenType::EqualEqual})) {
			Token op = previous();
			ExprPtr right = comparison();
			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}
		return expr;
	}

	// comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
	ExprPtr comparison() {
		ExprPtr expr = term();
		while (match({TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual})) {
			Token op = previous();
			ExprPtr right = term();
			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}
		return expr;
	}

	// term -> factor ( ( "-" | "+" ) factor )*
	ExprPtr term() {
		ExprPtr expr = factor();
		while (match({TokenType::Minus, TokenType::Plus})) {
			Token op = previous();
			ExprPtr right = factor();
			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}
		return expr;
	}

	// factor -> unary ( ( "/" | "*" | "%" ) unary )*
	ExprPtr factor() {
		ExprPtr expr = unary();
		while (
			match({TokenType::Slash, TokenType::Star, TokenType::Remainder})) {
			Token op = previous();
			ExprPtr right = unary();
			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}
		return expr;
	}

	// unary -> ( "!" | "-" ) unary | primary
	ExprPtr unary() {
		if (match({TokenType::Bang, TokenType::Minus})) {
			Token op = previous();
			ExprPtr right = unary();
			return std::make_unique<UnaryExpr>(op, std::move(right));
		}
		return primary();
	}

	// primary -> true | false | nil | NUMBER | STRING | "(" expression ")"
	ExprPtr primary() {
		if (match({TokenType::False}))
			return std::make_unique<LiteralExpr>(Value{false});
		if (match({TokenType::True}))
			return std::make_unique<LiteralExpr>(Value{true});
		if (match({TokenType::Nil}))
			return std::make_unique<LiteralExpr>(Value{});

		if (match({TokenType::Number, TokenType::String}))
			return std::make_unique<LiteralExpr>(previous().literal);

		if (match({TokenType::LeftParen})) {
			ExprPtr expr = expression();
			consume(TokenType::RightParen, "Expected ')' after expression.");
			return std::make_unique<GroupingExpr>(std::move(expr));
		}

		throw error(peek(), "Expected expression.");
	}

	// ---------------- Helpers ----------------

	// check if the next token is a specified token
	// consumes it if so
	bool match(std::initializer_list<TokenType> types) {
		for (TokenType type : types) {
			if (check(type)) {
				advance();
				return true;
			}
		}
		return false;
	}

	// consume next token
	// throws if it isn't the expected type
	const Token &consume(TokenType type, const std::string &message) {
		if (check(type))
			return advance();
		throw error(peek(), message);
	}

	// check if we are at the end and return type of next token if not
	bool check(TokenType type) const {
		if (is_at_end())
			return false;
		return peek().type == type;
	}

	// if we aren't at the end then increment current
	const Token &advance() {
		if (!is_at_end())
			current_++;
		return previous();
	}

	bool is_at_end() const { return peek().type == TokenType::EoF; }
	const Token &peek() const { return tokens_[current_]; }
	const Token &previous() const { return tokens_[current_ - 1]; }

	// errors for things that went wrong
	// prints the message and returns the exception for the caller to throw
	ParseError error(const Token &token, const std::string &message) {
		had_error_ = true;
		if (token.type == TokenType::EoF)
			std::cerr << "[line " << token.line << "] Error at end: " << message << "\n";
		else
			std::cerr << "[line " << token.line << "] Error at '" << token.lexeme << "': " << message << "\n";
		return ParseError(message);
	}
};
