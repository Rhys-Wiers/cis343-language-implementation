#include <error.h>
#include <iostream>

namespace {
bool had_error_flag = false;
}

void report_error(int line, const std::string &message) {
	std::cerr << "[line " << line << "] Error: " << message << "\n";
	had_error_flag = true;
}

bool had_error() { return had_error_flag; }

void reset_error() { had_error_flag = false; }
