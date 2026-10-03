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
	Colon,
	Remainder,

	// one or two char
	Bang,
	Equal,
	Less,
	Greater,
	EqualEqual,
	BangEqual,
	LessEqual,
	GreaterEqual,
	Ampersands,
	Verts,

	// literals
	Number,
	String,
	Identifier,

	// keywords
	Class,
	Else,
	False,
	For,
	If,
	Nil,
	Return,
	True,
	While,

	EoF
};

using Literal = std::variant<std::monostate, double, std::string>;

struct Token {
	TokenType type;
	std::string lexeme;
	Literal literal;
	int line;
};

std::string to_string(TokenType type);
std::string to_string(const Literal &literal);
