#include "value.h"
#include <iomanip>
#include <sstream>
#include <variant>

bool is_truthy(const Value &val) {
	if (std::holds_alternative<std::monostate>(val))
		return false;
	else if (auto *a = std::get_if<bool>(&val))
		return *a;
	else if (auto *b = std::get_if<double>(&val)) {
		return *b != 0.0;
	}
	return true;
}

bool is_equal(const Value &a, const Value &b) {
	if (std::holds_alternative<std::monostate>(a))
		return true;
	if (auto *da = std::get_if<double>(&a)) {
		if (auto *db = std::get_if<double>(&b))
			return *da == *db;
	}
	if (auto *sa = std::get_if<std::string>(&a)) {
		if (auto *sb = std::get_if<std::string>(&b))
			return *sa == *sb;
	}
	if (auto *ba = std::get_if<bool>(&a)) {
		if (auto *bb = std::get_if<bool>(&b))
			return *ba == *bb;
	}

	const double *num = std::get_if<double>(&a);
	const bool *boolean = std::get_if<bool>(&b);

	if (!num || !boolean) {
		num = std::get_if<double>(&b);
		boolean = std::get_if<bool>(&a);
	}

	if (num && boolean) {
		double bool_as_double = *boolean ? 1.0 : 0.0;
		return *num == bool_as_double;
	}

	return false;
}

std::string to_string(const Value &val) {
	if (std::holds_alternative<std::monostate>(val)) {
		return "nil";
	}
	if (auto *b = std::get_if<bool>(&val)) {
		return *b ? "true" : "false";
	}
	if (auto *d = std::get_if<double>(&val)) {
		std::ostringstream out;
		out << std::setprecision(15) << *d;
		std::string text = out.str();

		//------------ begin AI code ---------------
		// If the formatted string does not contain a decimal point,
		// exponent ('e'/'E'), 'nan', or 'inf', append ".0"
		if (text.find_first_of(".eEni") == std::string::npos) {
			text += ".0";
		}
		//------------ end AI code ---------------
		return text;
	}
	if (auto *s = std::get_if<std::string>(&val)) {
		return *s;
	}
	return "";
}
