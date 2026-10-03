#include "token.h"
#include <iomanip>
#include <sstream>

std::string to_string(TokenType type) {
	switch (type) {
	case TokenType::LeftParen:
		return "LEFT_PAREN";
	case TokenType::RightParen:
		return "RIGHT_PAREN";
	case TokenType::LeftBrace:
		return "LEFT_BRACE";
	case TokenType::RightBrace:
		return "RIGHT_BRACE";
	case TokenType::Comma:
		return "COMMA";
	case TokenType::Dot:
		return "DOT";
	case TokenType::Plus:
		return "PLUS";
	case TokenType::Minus:
		return "MINUS";
	case TokenType::Star:
		return "STAR";
	case TokenType::Slash:
		return "SLASH";
	case TokenType::SemiColon:
		return "SEMI_COLON";
	case TokenType::Colon:
		return "COLON";
	case TokenType::Remainder:
		return "REMAINDER";
	case TokenType::Bang:
		return "BANG";
	case TokenType::Equal:
		return "EQUAL";
	case TokenType::Less:
		return "LESS";
	case TokenType::Greater:
		return "GREATER";
	case TokenType::EqualEqual:
		return "EQUAL_EQUAL";
	case TokenType::BangEqual:
		return "BANG_EQUAL";
	case TokenType::LessEqual:
		return "LESS_EQUAL";
	case TokenType::GreaterEqual:
		return "GREATER_EQUAL";
	case TokenType::Ampersands:
		return "AMPERSANDS";
	case TokenType::Verts:
		return "VERTS";
	case TokenType::Number:
		return "NUMBER";
	case TokenType::String:
		return "STRING";
	case TokenType::Identifier:
		return "IDENTIFIER";
	case TokenType::Class:
		return "CLASS";
	case TokenType::Else:
		return "ELSE";
	case TokenType::False:
		return "FALSE";
	case TokenType::For:
		return "FOR";
	case TokenType::If:
		return "IF";
	case TokenType::Nil:
		return "NIL";
	case TokenType::Return:
		return "RETURN";
	case TokenType::True:
		return "TRUE";
	case TokenType::While:
		return "WHILE";
	case TokenType::EoF:
		return "EOF";
	}

	return "";
}

// begin AI code
std::string to_string(const Literal &literal) {
	if (std::holds_alternative<double>(literal)) {
		std::ostringstream out;
		out << std::setprecision(15) << std::get<double>(literal);
		std::string text = out.str();
		// print 3 as 3.0, but leave 3.14 and 1e+21 alone
		if (text.find_first_of(".eEni") == std::string::npos) {
			text += ".0";
		}
		return text;
	}
	if (std::holds_alternative<std::string>(literal)) {
		return std::get<std::string>(literal);
	}
	return "null";
}
// end AI code
