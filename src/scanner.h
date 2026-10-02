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
	void add_token(TokenType, Literal);
	char advance();
	char peek();
	char peek_next();
	void string();
	void number();
	bool is_at_end() const;
	bool match(char);
};
