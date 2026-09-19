#include <fstream>
#include <iostream>
#include <signal.h>
#include <string>
#include <unistd.h>

extern "C" {
#include <readline/readline.h>
}

// signal handler to detect ctrl+c
void signal_handler(int signum) {
	if (signum == SIGINT) {
		const char msg[] = "\nEscape character detected. Exiting...\n";
		write(STDOUT_FILENO, msg, sizeof(msg) - 1);
		_exit(0);
	}
}

int main(int argc, char *argv[]) {
	// register signal handler for ctrl+c
	signal(SIGINT, signal_handler);

	// check mode
	if (argc == 1) {
		// enter REPL mode
		char *raw_input;

		// read input
		while ((raw_input = readline("> ")) != nullptr) {

			std::string input(raw_input);
			free(raw_input);

			// echo input
			std::cout << "Your input was: " << input << '\n';
			std::cerr << "Error: Scanner not implemented." << '\n';
		}
	} else if (argc == 2) {
		// locate file
		std::ifstream my_file(argv[1]);

		// check that it opened
		if (!my_file.is_open()) {
			std::cerr << "Error: Could not open the file!" << '\n';
			return 1;
		}

		std::string line;

		// read each line
		while (std::getline(my_file, line)) {
			std::cout << line << std::endl;
		}

		my_file.close();
		std::cerr << "Error: Scanner not implemented." << '\n';

	} else if (argc > 2) {
		std::cout << "Too many arguments. Only one file allowed at a time."
				  << '\n';
	}

	return 0;
}
