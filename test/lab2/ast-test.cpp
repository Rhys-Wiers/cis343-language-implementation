// ---------------- begin AI code ----------------
#include "../../src/ast-printer.h"
#include <iostream>
#include <memory>

// ---------- helpers ----------
template <typename T, typename... Args> ExprPtr make(Args &&...args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

Token tok(TokenType type, const std::string &lexeme) {
	return Token{type, lexeme, std::monostate{}, 1};
}

ExprPtr lit(Literal v) { return make<LiteralExpr>(std::move(v)); }
ExprPtr group(ExprPtr e) { return make<GroupingExpr>(std::move(e)); }
ExprPtr unary(Token op, ExprPtr e) {
	return make<UnaryExpr>(std::move(op), std::move(e));
}
ExprPtr binary(ExprPtr l, Token op, ExprPtr r) {
	return make<BinaryExpr>(std::move(l), std::move(op), std::move(r));
}

int failures = 0;

void check(const std::string &name, const Expr &tree,
		   const std::string &expected) {
	AstPrinter printer;
	std::string actual = printer.print(tree);
	bool ok = actual == expected;
	if (!ok)
		++failures;
	std::cout << (ok ? "PASS " : "FAIL ") << name << "\n"
			  << "  expected: " << expected << "\n"
			  << "  actual:   " << actual << "\n";
}

int main() {
	// ---------- Test 1: primary and unary ----------
	// Every literal kind, both unary operators, and nested unary.
	check("literal number", *lit(42.0), "42");
	check("literal string", *lit(std::string("hi")),
		  "hi"); // "hi" if you skip quotes
	check("literal true", *lit(true), "true");
	check("literal false", *lit(false), "false");
	check("literal nil", *lit(std::monostate{}), "nil");
	check("unary minus", *unary(tok(TokenType::Minus, "-"), lit(3.0)), "(- 3)");
	check("unary bang", *unary(tok(TokenType::Bang, "!"), lit(true)),
		  "(! true)");
	check("nested unary",
		  *unary(tok(TokenType::Bang, "!"),
				 unary(tok(TokenType::Bang, "!"), lit(false))),
		  "(! (! false))");

	// ---------- Test 2: arithmetic (term and factor) ----------
	// 1 + 2 * 3 - 4 / 5 % 6
	// factor binds tighter, and operators of equal level group left to right:
	// ((1 + (2 * 3)) - ((4 / 5) % 6))
	ExprPtr arithmetic =
		binary(binary(lit(1.0), tok(TokenType::Plus, "+"),
					  binary(lit(2.0), tok(TokenType::Star, "*"), lit(3.0))),
			   tok(TokenType::Minus, "-"),
			   binary(binary(lit(4.0), tok(TokenType::Slash, "/"), lit(5.0)),
					  tok(TokenType::Remainder, "%"), lit(6.0)));
	check("arithmetic", *arithmetic, "(- (+ 1 (* 2 3)) (% (/ 4 5) 6))");

	// the assignment's example: -123 * (45.67)
	ExprPtr assignment = binary(unary(tok(TokenType::Minus, "-"), lit(123.0)),
								tok(TokenType::Star, "*"), group(lit(45.67)));
	check("assignment example", *assignment, "(* (- 123) (group 45.67))");

	// ---------- Test 3: comparison, equality, logic, grouping ----------
	// (1 < 2) == true && (3 >= 4) != nil || ((5 <= 6) && (7 > 8))
	ExprPtr logic = binary(
		binary(binary(binary(lit(1.0), tok(TokenType::Less, "<"), lit(2.0)),
					  tok(TokenType::EqualEqual, "=="), lit(true)),
			   tok(TokenType::Ampersands, "&&"),
			   binary(binary(lit(3.0), tok(TokenType::GreaterEqual, ">="),
							 lit(4.0)),
					  tok(TokenType::BangEqual, "!="), lit(std::monostate{}))),
		tok(TokenType::Verts, "||"),
		group(
			binary(binary(lit(5.0), tok(TokenType::LessEqual, "<="), lit(6.0)),
				   tok(TokenType::Ampersands, "&&"),
				   binary(lit(7.0), tok(TokenType::Greater, ">"), lit(8.0)))));
	check("logic", *logic,
		  "(|| (&& (== (< 1 2) true) (!= (>= 3 4) nil)) "
		  "(group (&& (<= 5 6) (> 7 8))))");

	std::cout << (failures == 0 ? "\nAll tests passed.\n"
								: "\nSome tests failed.\n");
	return failures;
}
// ---------------- end AI code ----------------
