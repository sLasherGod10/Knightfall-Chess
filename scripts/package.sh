#!/usr/bin/env sh
set -eu
build_dir="${1:-build-release}"
cmake -S . -B "$build_dir" -DCMAKE_BUILD_TYPE=Release -DCHESS_BUILD_GUI=OFF -DCHESS_BUILD_CLI=ON -DCHESS_BUILD_TESTS=ON
cmake --build "$build_dir" --config Release
ctest --test-dir "$build_dir" --output-on-failure
echo "Core CLI build and tests succeeded. Desktop packaging is not implemented."
