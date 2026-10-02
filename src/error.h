#pragma once
#include <string>

void report_error(int line, const std::string &message);
bool had_error();
void reset_error();
