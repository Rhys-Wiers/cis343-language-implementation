#pragma once
#include <string>
#include <variant>

enum class TokenType {
	// single char
	LeftParen,
	RightParen,
	LeftBrace,
	RightBrace,
	Comma,
	Dot,
	Plus,
	Minus,
	Star,
	Slash,
	SemiColon,

	// one or two char
	Bang,
	Equal,
	Less,
	Greater,
	EqualEqual,
	BangEqual,
	LessEqual,
	GreaterEqual,

	// literals
	Number,
	String,
	Identifier,

	// keywords
	AND,
	CLASS,
	ELSE,
	FALSE,
	FOR,
	IF,
	NIL,
	OR,
	PRINT,
	RETURN,
	TRUE,
	WHILE,

	EoF
};

using Literal = std::variant<std::monostate, double, std::string>;

struct Token {
	TokenType type;
	std::string lexeme;
	Literal literal;
	int line;
};
