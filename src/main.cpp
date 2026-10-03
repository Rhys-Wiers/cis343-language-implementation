#include "error.h"
#include "scanner.h"
#include "token.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unistd.h>
#include <vector>

void run(std::string &source) {
	Scanner scanner(source);

	std::vector<Token> tokens = scanner.scan_tokens();
	for (const Token &token : tokens) {
		std::cout << to_string(token.type) << " " << token.lexeme << " "
				  << to_string(token.literal) << "\n";
	}
}

int run_file(char *file) {
	// locate file
	std::ifstream my_file(file);

	// check that it opened
	if (!my_file.is_open()) {
		std::cerr << "Error: Could not open the file." << '\n';
		return 66; // return exit code that indicates the error that occurred
	}

	// read whole file
	// start AI code
	std::string content((std::istreambuf_iterator<char>(my_file)),
						std::istreambuf_iterator<char>());
	// end AI code

	Scanner scanner(content);
	std::vector<Token> tokens = scanner.scan_tokens();
	for (const Token &token : tokens) {
		std::cout << to_string(token.type) << " " << token.lexeme << " "
				  << to_string(token.literal) << "\n";
	}

	my_file.close();
	return 0;
}

void enter_repl() {
	// enter REPL mode
	bool interactive = isatty(STDIN_FILENO);

	std::string line;

	// read input
	while (true) {
		if (interactive)
			std::cout << "> " << std::flush;
		if (!std::getline(std::cin, line)) {
			if (interactive)
				std::cout << '\n';
			break;
		}

		run(line);
		std::cout << std::flush;
		reset_error();
	}
}

int main(int argc, char *argv[]) {

	// check mode
	if (argc == 1) {
		enter_repl();
	} else if (argc == 2) {
		run_file(argv[1]);
	} else if (argc > 2) {
		std::cout << "Too many arguments. Only one file allowed at a time."
				  << '\n';
	}
	return 0;
}
