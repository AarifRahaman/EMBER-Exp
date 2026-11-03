#!/usr/bin/env bash
set -euo pipefail

# Small helper: decide which C/C++ files to check
get_cpp_files() {
    local event="${GITHUB_EVENT_NAME:-push}"
    local base_ref="${GITHUB_BASE_REF:-main}"

    if [ "$event" = "pull_request" ] && [ -n "$base_ref" ]; then
        echo "PR event → only changed C/C++ files relative to base branch: $base_ref" >&2
        git fetch origin "$base_ref" --depth=1
        git diff --name-only "origin/$base_ref...HEAD" -- \
            '*.h' '*.hpp' '*.c' '*.cc' '*.cpp' '*.cxx' '*.tpp'
    else
        echo "Push event → all tracked C/C++ files" >&2
        git ls-files '*.h' '*.hpp' '*.c' '*.cc' '*.cpp' '*.cxx' '*.tpp'
    fi
}

run_clang_format() {
    files=$(get_cpp_files)

    if [ -z "$files" ]; then
        echo "No C/C++ files to check, skipping clang-format."
        exit 0
    fi

    clang-format --version
    echo "Running clang-format --dry-run -Werror on:"
    echo "$files"

    for f in $files; do
        if [ ! -f "$f" ]; then
            echo "Skipping missing file: $f (not found)"
            continue
        fi
        clang-format --dry-run -Werror "$f"
    done
}

run_cppcheck() {
    files=$(get_cpp_files)

    if [ -z "$files" ]; then
        echo "No C/C++ files to check, skipping cppcheck."
        exit 0
    fi

    echo "Running cppcheck on:"
    echo "$files"

    cppcheck \
        --enable=all \
        --error-exitcode=1 \
        --std=c++20 \
        --inline-suppr \
        --quiet \
        --suppress=missingIncludeSystem \
        --suppress=*:*build/* \
        $files
}

case "${1:-}" in
    format)
        run_clang_format
        ;;
    cppcheck)
        run_cppcheck
        ;;
    *)
        echo "Usage: $0 {format|cppcheck}"
        exit 1
        ;;
esac
