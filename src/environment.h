#pragma once

#include "token.h"
#include "value.h"
#include <memory>
#include <string>
#include <unordered_map>

class Environment {
  public:
	Environment();

	explicit Environment(std::shared_ptr<Environment> enclosing);

	void define(const std::string &name, Value value);
	Value get(const Token &name) const;
	void assign(const Token &name, Value value);

  private:
	std::shared_ptr<Environment> enclosing_;
	std::unordered_map<std::string, Value> values_;
};
