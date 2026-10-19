// ------------- start AI code ---------------

#include "../../src/ast-printer.h"
#include "../../src/parser.h"
#include "../../src/scanner.h"
#include <iostream>
#include <string>

static int failures = 0;

// scan + parse source, compare printed tree against expected
static void check(const std::string &name, const std::string &source, const std::string &expected) {
	Parser parser(Scanner(source).scan_tokens());
	ExprPtr expr = parser.parse();
	if (!expr || parser.had_error()) {
		std::cout << "FAIL " << name << ": unexpected parse error\n";
		failures++;
		return;
	}
	std::string got = AstPrinter().print(*expr);
	if (got == expected) {
		std::cout << "ok   " << name << "\n";
	} else {
		std::cout << "FAIL " << name << "\n  expected: " << expected << "\n  got:      " << got << "\n";
		failures++;
	}
}

// parse should return nullptr and set had_error
static void check_error(const std::string &name, const std::string &source) {
	Parser parser(Scanner(source).scan_tokens());
	ExprPtr expr = parser.parse();
	if (expr == nullptr && parser.had_error()) {
		std::cout << "ok   " << name << "\n";
	} else {
		std::cout << "FAIL " << name << ": expected a parse error\n";
		failures++;
	}
}

int main() {
	// ---------- literals, unary, grouping ----------
	check("number", "42", "42");
	check("decimal", "45.67", "45.67");
	check("string", "\"hi\"", "hi");
	check("true", "true", "true");
	check("false", "false", "false");
	check("nil", "nil", "nil");
	check("unary minus", "-5", "(- 5)");
	check("unary bang", "!true", "(! true)");
	check("nested bang", "!!true", "(! (! true))");
	check("nested minus", "--5", "(- (- 5))");
	check("group", "(1)", "(group 1)");
	check("nested group", "((1))", "(group (group 1))");

	// ---------- precedence ----------
	check("factor over term", "1 + 2 * 3", "(+ 1 (* 2 3))");
	check("term after factor", "1 * 2 + 3", "(+ (* 1 2) 3)");
	check("group overrides", "(1 + 2) * 3", "(* (group (+ 1 2)) 3)");
	check("unary over factor", "-2 * 3", "(* (- 2) 3)");
	check("unary over equality", "!true == false", "(== (! true) false)");
	check("term over comparison", "1 + 2 < 4", "(< (+ 1 2) 4)");
	check("comparison over equality", "1 < 2 == true", "(== (< 1 2) true)");
	check("and over or", "true || false && false", "(|| true (&& false false))");
	check("and before or", "true && false || true", "(|| (&& true false) true)");
	check("remainder is factor", "1 % 2 + 3", "(+ (% 1 2) 3)");
	check("assignment example", "-123 * (45.67)", "(* (- 123) (group 45.67))");

	// ---------- associativity (left to right) ----------
	check("minus", "8 - 3 - 2", "(- (- 8 3) 2)");
	check("slash", "8 / 4 / 2", "(/ (/ 8 4) 2)");
	check("equality", "1 == 2 == 3", "(== (== 1 2) 3)");
	check("or", "true || false || true", "(|| (|| true false) true)");
	check("binary then unary", "1 - -2", "(- 1 (- 2))");

	// ---------- all operators together ----------
	check("full", "1 + 2 * 3 - 4 / 5 % 6", "(- (+ 1 (* 2 3)) (% (/ 4 5) 6))");
	check("logic", "(1 < 2) == true && (3 >= 4) != nil || ((5 <= 6) && (7 > 8))",
		  "(|| (&& (== (group (< 1 2)) true) (!= (group (>= 3 4)) nil)) "
		  "(group (&& (group (<= 5 6)) (group (> 7 8)))))");
	// ---------- errors ----------
	check_error("empty input", "");
	check_error("trailing operator", "1 +");
	check_error("leading operator", "* 3");
	check_error("double operator", "1 + + 2");
	check_error("missing close paren", "(1 + 2");
	check_error("empty group", "()");
	check_error("lone close paren", ")");
	check_error("trailing token", "1 2"); // fails until parse() checks is_at_end()

	std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
	return failures;
}

// ------------- end AI code --------------
