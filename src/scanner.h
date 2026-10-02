#pragma once
#include "token.h"
#include <stddef.h>
#include <string>
#include <vector>

class Scanner {
  public:
	explicit Scanner(std::string source);
	std::vector<Token> scan_tokens();

  private:
	size_t start_ = 0;
	size_t current_ = 0;
	int line_ = 1;
	std::string source_;
	std::vector<Token> tokens_;
	void scan_token();
	void add_token(TokenType);
	void add_token(TokenType, Literal);
	char advance();
	const char peek();
	const char peek_next();
	void scan_string();
	void scan_number();
	void scan_identifier();
	bool is_at_end() const;
	bool match(char);
};
