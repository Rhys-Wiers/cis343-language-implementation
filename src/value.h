#pragma once
#include <string>
#include <variant>

using Value = std::variant<std::monostate, double, std::string, bool>;

bool is_truthy(const Value &val);
bool is_equal(const Value &a, const Value &b);
std::string to_string(const Value &val);
