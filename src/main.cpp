#include "error.h"
#include "scanner.h"
#include "token.h"
// #include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
// #include <ratio>
// #include <signal.h>
#include <string>
#include <unistd.h>
#include <vector>

/* signal handler to detect ctrl+c
void signal_handler(int signum) {
	if (signum == SIGINT) {
		const char msg[] = "\nEscape character detected. Exiting...\n";
		write(STDOUT_FILENO, msg, sizeof(msg) - 1);
		_exit(0);
	}
} */
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
	// std::cerr << "Error: Scanner not implemented." << '\n';
	return 0;
}

void enter_repl() {
	// register signal handler for ctrl+c
	// signal(SIGINT, signal_handler);

	// enter REPL mode

	std::string line;

	// read input
	while (true) {
		std::cout << "> " << std::flush;
		if (!std::getline(std::cin, line)) {
			std::cout << '\n';
			break;
		}

		run(line);
		reset_error();

		// echo input
		// std::cout << "Your input was: " << input << '\n';
		// std::cerr << "Error: Scanner not implemented." << '\n';
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
