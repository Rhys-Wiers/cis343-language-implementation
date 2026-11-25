#include "environment.h"
#include "evaluator.h"
#include <utility>

Environment::Environment() : enclosing_(nullptr) {}

Environment::Environment(std::shared_ptr<Environment> enclosing)
	: enclosing_(std::move(enclosing)) {}

void Environment::define(const std::string &name, Value value) {
	values_[name] = value;
}

Value Environment::get(const Token &name) const {
	auto it = values_.find(name.lexeme);
	if (it != values_.end()) {
		return it->second;
	}

	if (enclosing_ != nullptr) {
		return enclosing_->get(name);
	}

	throw RuntimeError(name, "Unidentified variable '" + name.lexeme + "'.");
}

void Environment::assign(const Token &name, Value value) {
	auto it = values_.find(name.lexeme);
	if (it != values_.end()) {
		it->second = std::move(value);
		return;
	}

	if (enclosing_ != nullptr) {
		enclosing_->assign(name, std::move(value));
		return;
	}

	throw RuntimeError(name, "Unidentified variable '" + name.lexeme + "'.");
}
