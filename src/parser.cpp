#include "parser.h"
#include "expression.h"
#include "statement.h"
#include "token.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

std::vector<StmtPtr> Parser::parse() {
	std::vector<StmtPtr> statements;
	while (!is_at_end()) {
		StmtPtr stmt = declaration();
		if (stmt != nullptr) {
			statements.push_back(std::move(stmt));
		}
	}
	return statements;
}

bool Parser::had_error() const {
	return had_error_;
}

// ---------------- Statement Rules ----------------

StmtPtr Parser::declaration() {
	try {
		if (match({TokenType::Var})) {
			return var_declaration();
		}
		return statement();
	} catch (const ParseError &) {
		synchronize();
		return nullptr;
	}
}

StmtPtr Parser::statement() {
	try {
		if (match({TokenType::Print})) {
			return print_statement();
		} else if (match({TokenType::LeftBrace})) {
			return std::make_unique<Block>(block());
		}
		return expression_statement();
	} catch (const ParseError &) {
		synchronize();
		return nullptr;
	}
}

StmtPtr Parser::print_statement() {
	ExprPtr value = expression();
	consume(TokenType::SemiColon, "Expected ';' after value.");
	return std::make_unique<PrintStmt>(std::move(value));
}

StmtPtr Parser::expression_statement() {
	ExprPtr expr = expression();
	consume(TokenType::SemiColon, "Expected ';' after expression");
	return std::make_unique<ExpressionStmt>(std::move(expr));
}

StmtPtr Parser::var_declaration() {
	Token name = consume(TokenType::Identifier, "Expected variable name.");

	ExprPtr initializer = nullptr;
	if (match({TokenType::Equal})) {
		initializer = expression();
	}

	consume(TokenType::SemiColon, "Expected ';' after variable declaration.");
	return std::make_unique<VariableStmt>(std::move(name), std::move(initializer));
}

std::vector<StmtPtr> Parser::block() {
	std::vector<StmtPtr> statements;

	while (!check(TokenType::RightBrace) && !is_at_end()) {
		statements.push_back(declaration());
	}

	consume(TokenType::RightBrace, "Expected '}' after block.");
	return statements;
}

// ---------------- Grammar Rules ----------------

ExprPtr Parser::expression() {
	return assignment();
}

ExprPtr Parser::assignment() {
	ExprPtr expr = logic_or();
	if (match({TokenType::Equal})) {
		Token op = previous();
		ExprPtr value = assignment();
		// begin AI code
		auto *var_expr = dynamic_cast<VariableExpr *>(expr.get());
		// end AI code
		if (var_expr) {
			return std::make_unique<AssignExpr>(var_expr->name, std::move(value));
		} else {
			error(op, "Invalid variable assignment.");
		}
	}
	return expr;
}

// logic_or -> logic_and ( "||" logic_and )*
ExprPtr Parser::logic_or() {
	ExprPtr expr = logic_and();
	while (match({TokenType::Verts})) { // ||
		Token op = previous();
		ExprPtr right = logic_and();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}
	return expr;
}

// logic_and -> equality ( "&&" equality )*
ExprPtr Parser::logic_and() {
	ExprPtr expr = equality();
	while (match({TokenType::Ampersands})) { // &&
		Token op = previous();
		ExprPtr right = equality();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}
	return expr;
}

// equality -> comparison ( ( "!=" | "==" ) comparison )*
ExprPtr Parser::equality() {
	ExprPtr expr = comparison();
	while (match({TokenType::BangEqual, TokenType::EqualEqual})) {
		Token op = previous();
		ExprPtr right = comparison();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}
	return expr;
}

// comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
ExprPtr Parser::comparison() {
	ExprPtr expr = term();
	while (match({TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual})) {
		Token op = previous();
		ExprPtr right = term();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}
	return expr;
}

// term -> factor ( ( "-" | "+" ) factor )*
ExprPtr Parser::term() {
	ExprPtr expr = factor();
	while (match({TokenType::Minus, TokenType::Plus})) {
		Token op = previous();
		ExprPtr right = factor();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}
	return expr;
}

// factor -> unary ( ( "/" | "*" | "%" ) unary )*
ExprPtr Parser::factor() {
	ExprPtr expr = unary();
	while (match({TokenType::Slash, TokenType::Star, TokenType::Remainder})) {
		Token op = previous();
		ExprPtr right = unary();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}
	return expr;
}

// unary -> ( "!" | "-" ) unary | primary
ExprPtr Parser::unary() {
	if (match({TokenType::Bang, TokenType::Minus})) {
		Token op = previous();
		ExprPtr right = unary();
		return std::make_unique<UnaryExpr>(op, std::move(right));
	}
	return primary();
}

// primary -> true | false | nil | NUMBER | STRING | "(" expression ")"
ExprPtr Parser::primary() {
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

	if (match({TokenType::Identifier})) {
		return std::make_unique<VariableExpr>(previous());
	}
	throw error(peek(), "Expected expression.");
}

// ---------------- Helpers & Error Recovery ----------------

void Parser::synchronize() {
	advance();

	while (!is_at_end()) {
		if (previous().type == TokenType::SemiColon)
			return;

		if (peek().type == TokenType::Class) {
			return;
		} else if (peek().type == TokenType::Fun) {
			return;
		} else if (peek().type == TokenType::Var) {
			return;
		} else if (peek().type == TokenType::For) {
			return;
		} else if (peek().type == TokenType::If) {
			return;
		} else if (peek().type == TokenType::While) {
			return;
		} else if (peek().type == TokenType::Print) {
			return;
		} else if (peek().type == TokenType::Return) {
			return;
		}
		advance();
	}
}

bool Parser::match(std::initializer_list<TokenType> types) {
	for (TokenType type : types) {
		if (check(type)) {
			advance();
			return true;
		}
	}
	return false;
}

const Token &Parser::consume(TokenType type, const std::string &message) {
	if (check(type))
		return advance();
	throw error(peek(), message);
}

bool Parser::check(TokenType type) const {
	if (is_at_end())
		return false;
	return peek().type == type;
}

const Token &Parser::advance() {
	if (!is_at_end())
		current_++;
	return previous();
}

bool Parser::is_at_end() const {
	return peek().type == TokenType::EoF;
}

const Token &Parser::peek() const {
	return tokens_[current_];
}

const Token &Parser::previous() const {
	return tokens_[current_ - 1];
}

Parser::ParseError Parser::error(const Token &token, const std::string &message) {
	had_error_ = true;
	if (token.type == TokenType::EoF)
		std::cerr << "[line " << token.line << "] Error at end: " << message << "\n";
	else
		std::cerr << "[line " << token.line << "] Error at '" << token.lexeme << "': " << message << "\n";
	return ParseError(message);
}
