#!/usr/bin/env bash
# --- start AI code ---
# Runs every test in cases/ (file mode) and repl/ (REPL mode, piped stdin).
# Usage: ./test/lab1/run_tests.sh [path-to-binary]
# Optional per-test file: NAME.code containing the expected exit code.

BIN="${1:-build/myinterp}"
DIR="$(cd "$(dirname "$0")" && pwd)"

if [ ! -x "$BIN" ]; then
    echo "Binary not found: $BIN (build first, run from the repo root)"
    exit 2
fi

pass=0
fail=0

# check NAME EXPECTED_FILE ACTUAL_TEXT ACTUAL_CODE CODE_FILE
check() {
    local name="$1" expected="$2" actual="$3" code="$4" code_file="$5"
    local ok=1

    if ! diff <(printf '%s\n' "$actual") "$expected" > /tmp/lab1_diff.$$ 2>&1; then
        ok=0
    fi

    if [ -f "$code_file" ]; then
        local want
        want="$(tr -d '[:space:]' < "$code_file")"
        if [ "$code" != "$want" ]; then
            ok=0
            echo "  exit code: expected $want, got $code" >> /tmp/lab1_diff.$$
        fi
    fi

    if [ $ok -eq 1 ]; then
        echo "PASS  $name"
        pass=$((pass + 1))
    else
        echo "FAIL  $name"
        sed 's/^/    /' /tmp/lab1_diff.$$
        fail=$((fail + 1))
    fi
    rm -f /tmp/lab1_diff.$$
}

# File-mode tests: cases/NAME.txt vs cases/NAME.expected
for input in "$DIR"/cases/*.txt; do
    [ -e "$input" ] || continue
    base="${input%.txt}"
    name="$(basename "$base")"
    if [ ! -f "$base.expected" ]; then
        echo "SKIP  $name (no .expected file)"
        continue
    fi
    actual="$("$BIN" "$input" 2>&1)"
    code=$?
    check "$name" "$base.expected" "$actual" "$code" "$base.code"
done

# REPL tests: repl/NAME.in is piped to stdin, compared to repl/NAME.expected
for input in "$DIR"/repl/*.in; do
    [ -e "$input" ] || continue
    base="${input%.in}"
    name="repl/$(basename "$base")"
    if [ ! -f "$base.expected" ]; then
        echo "SKIP  $name (no .expected file)"
        continue
    fi
    actual="$("$BIN" < "$input" 2>&1)"
    code=$?
    check "$name" "$base.expected" "$actual" "$code" "$base.code"
done

echo
echo "$pass passed, $fail failed"
[ $fail -eq 0 ]
# --- end AI code ---
