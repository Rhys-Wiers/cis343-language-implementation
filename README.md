# Hashbrown

A Lox-style language implementation in C++. **Status:** the scanner is done; AST, parser, and interpreter are next.

## Build and run

Requires a C++17 compiler.

```bash
g++ -std=c++17 -Isrc src/*.cpp -o hashbrown

./hashbrown            # REPL (Ctrl-D to exit)
./hashbrown file.hb    # scan a file
```

Both modes print one token per line as `TYPE lexeme literal`. For example, `-123 * (45.67)` gives:

```
MINUS - null
NUMBER 123 123.0
STAR * null
LEFT_PAREN ( null
NUMBER 45.67 45.67
RIGHT_PAREN ) null
EOF  null
```

## Language so far

- **Literals:** numbers (`42`, `3.14`, `1e10`), strings (`"hi"`, double quotes, may span lines, no escapes), `true`, `false`, `nil`
- **Operators:** `+ - * / %`, `< <= > >=`, `== !=`, `&& || !`, `=`
- **Punctuation:** `( ) { } , . ; :`
- **Keywords:** `class else false for if nil return true while`
- **Comments:** `# single line` and `/# multi-line #/`

## Errors

Errors go to stderr as `[line N] Error: message`, and scanning continues so several errors can be reported in one run.

## Layout

```
src/
├── main.cpp        # REPL and file mode
├── scanner.h/.cpp  # source string -> vector<Token>
├── token.h/.cpp    # TokenType, Token, Literal, to_string()
└── error.h/.cpp    # report_error(), had_error(), reset_error()
```

## Roadmap

- [ ] Expression grammar, AST nodes, AST printer
- [ ] Parser
- [ ] Interpreter

AI-assisted snippets are marked with `start AI code` / `end AI code` comments in the source.
