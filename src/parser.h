#pragma once

#include "expression.h"
#include "statement.h"
#include "token.h"
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>

// recursive descent parser
class Parser {
  public:
	explicit Parser(std::vector<Token> tokens);

	// parses until EOF
	std::vector<StmtPtr> parse();
	bool had_error() const;

  private:
	// define parse error type, with same behavior as runtime error type
	struct ParseError : std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	std::vector<Token> tokens_;
	std::size_t current_ = 0; // next token to consume
	bool had_error_ = false;

	// statement rules
	StmtPtr statement();
	StmtPtr print_statement();
	StmtPtr expression_statement();

	// grammar rules, in order of precedence
	ExprPtr expression();
	ExprPtr logic_or();
	ExprPtr logic_and();
	ExprPtr equality();
	ExprPtr comparison();
	ExprPtr term();
	ExprPtr factor();
	ExprPtr unary();
	ExprPtr primary();

	// helpers & error recovery
	void synchronize();
	bool match(std::initializer_list<TokenType> types);
	const Token &consume(TokenType type, const std::string &message);
	bool check(TokenType type) const;
	const Token &advance();
	bool is_at_end() const;
	const Token &peek() const;
	const Token &previous() const;
	ParseError error(const Token &token, const std::string &message);
};
