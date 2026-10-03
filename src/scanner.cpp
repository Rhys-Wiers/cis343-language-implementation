#include "scanner.h"
#include "error.h"
#include "token.h"
#include <cctype>
#include <stdexcept>
#include <string>
#include <unordered_map>

static std::unordered_map<std::string, TokenType> keywords = {
	{"class", TokenType::Class},   {"else", TokenType::Else},
	{"false", TokenType::False},   {"for", TokenType::For},
	{"if", TokenType::If},		   {"nil", TokenType::Nil},
	{"return", TokenType::Return}, {"true", TokenType::True},
	{"while", TokenType::While},
};

// Initialize scanner with the source that we are scanning
Scanner::Scanner(std::string source) : source_(std::move(source)) {}

namespace {
// check if a char is a number
bool is_digit(char c) { return c >= '0' && c <= '9'; }

// check if a char is a letter
bool is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// check if alhpanumeric
bool is_alphanumeric(char c) { return is_alpha(c) || is_digit(c); }
} // namespace

// check if at EOF
bool Scanner::is_at_end() const { return current_ >= source_.size(); }

// check if next char is the one specified
bool Scanner::match(char expected) {
	if (is_at_end())
		return false;
	if (source_[current_] != expected)
		return false;

	++current_;
	return true;
}

void Scanner::add_token(TokenType type) { add_token(type, std::monostate{}); }

void Scanner::add_token(TokenType type, Literal literal) {
	std::string text = source_.substr(start_, current_ - start_);
	tokens_.push_back(Token{type, text, literal, line_});
}

char Scanner::advance() {
	++current_;
	return source_[current_ - 1];
}

const char Scanner::peek() {
	if (is_at_end())
		return '\0';
	return source_[current_];
}

const char Scanner::peek_next() {
	if (current_ + 1 >= source_.size())
		return '\0';
	return source_[current_ + 1];
}

void Scanner::scan_string() {
	while (peek() != '"' && !is_at_end()) {
		if (peek() == '\n')
			++line_;
		advance();
	}

	if (is_at_end()) {
		report_error(line_, "Unterminated string.");
		return;
	}

	advance();

	std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
	add_token(TokenType::String, value);
}

void Scanner::scan_number() {
	while (is_digit(peek()))
		advance();

	if (peek() == '.' && is_digit(peek_next())) {
		advance();
		while (is_digit(peek()))
			advance();
	}
	if (peek() == 'e' || peek() == 'E') {
		advance();
		if (peek() == '+' || peek() == '-')
			advance();
		if (!is_digit(peek())) {
			report_error(line_, "Malformed exponent in number.");
			return;
		}
		while (is_digit(peek())) {
			advance();
		}
	}

	std::string text = source_.substr(start_, current_ - start_);
	try {
		add_token(TokenType::Number, std::stod(text));
	} catch (const std::out_of_range &) {
		report_error(line_, "Number literal too large.");
	}
}

void Scanner::scan_identifier() {
	while (is_alphanumeric(peek()))
		advance();

	// --- start AI code ---
	std::string text = source_.substr(start_, current_ - start_);
	auto found = keywords.find(text);
	TokenType type =
		(found != keywords.end()) ? found->second : TokenType::Identifier;
	// --- end AI code ---

	add_token(type);
}

// Scan a single token
void Scanner::scan_token() {
	char c = advance();

	// checks all important characters and adds them to list of tokens as
	// necessary
	switch (c) {
	case '(':
		add_token(TokenType::LeftParen);
		break;
	case ')':
		add_token(TokenType::RightParen);
		break;
	case '{':
		add_token(TokenType::LeftBrace);
		break;
	case '}':
		add_token(TokenType::RightBrace);
		break;
	case ',':
		add_token(TokenType::Comma);
		break;
	case '.':
		add_token(TokenType::Dot);
		break;
	case '+':
		add_token(TokenType::Plus);
		break;
	case '-':
		add_token(TokenType::Minus);
		break;
	case ';':
		add_token(TokenType::SemiColon);
		break;
	case '*':
		add_token(TokenType::Star);
		break;
	case '!':
		add_token(match('=') ? TokenType::BangEqual : TokenType::Bang);
		break;
	case '=':
		add_token(match('=') ? TokenType::EqualEqual : TokenType::Equal);
		break;
	case '<':
		add_token(match('=') ? TokenType::LessEqual : TokenType::Less);
		break;
	case '>':
		add_token(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
		break;
	case ':':
		add_token(TokenType::Colon);
		break;
	case '%':
		add_token(TokenType::Remainder);
		break;
	case '/':
		// check if it's a comment
		if (match('#')) {
			while (peek() != '#' && peek_next() != '/' && !is_at_end()) {
				if (peek() == '\n')
					++line_;
				advance();
			}
			if (!is_at_end()) {
				advance();
				advance();
			} else {
				report_error(line_, "Unterminated multiline comment.");
			}
		} else {
			add_token(TokenType::Slash);
		}
		break;
	case '#':
		while (peek() != '\n' && !is_at_end()) {
			advance();
		}
		break;
	case '"':
		scan_string();
		break;
	case '&':
		if (match('&')) {
			add_token(TokenType::Ampersands);
		} else {
			report_error(line_, "Unexpected '&'. Did you mean '&&'?");
		}
		break;
	case '|':
		if (match('|')) {
			add_token(TokenType::Verts);
		} else {
			report_error(line_, "Unexpected '|'. Did you mean '||'?");
		}
		break;
	// skip whitespace
	case ' ':
	case '\r':
	case '\t':
		break;

	// increment line counter and then skip newline
	case '\n':
		++line_;
		break;

	// catchall for unexpected tokens
	default:
		if (is_digit(c)) {
			scan_number();
		} else if (is_alpha(c)) {
			scan_identifier();
		} else {
			report_error(line_, "Unexpected Character.");
		}
		break;
	}
}

std::vector<Token> Scanner::scan_tokens() {
	// loop and scan
	while (!is_at_end()) {
		start_ = current_;
		scan_token();
	}

	tokens_.push_back(Token{TokenType::EoF, "", std::monostate{}, line_});
	return tokens_;
}
