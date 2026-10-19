# RookForge project guide

`rookforge_core` in `src/core` owns board representation, move generation, legality filtering, reversible moves, notation, game result/clock handling, and search. It has no GUI dependency. `src/cli` offers perft/divide and search commands. `src/gui` contains the optional SFML 3 board UI. `tests/test_gtest.cpp` is used when GoogleTest is installed or fetched; otherwise `tests/test_engine.cpp` provides dependency-free CTest checks.

Configure with `CHESS_BUILD_GUI=OFF` (the default) to build core-only; `CHESS_BUILD_GUI=ON` requires SFML 3.0.2 installed separately. `CHESS_BUILD_TESTS`, `CHESS_BUILD_CLI`, `CHESS_SANITIZE`, and `CHESS_FETCH_DEPS` control optional targets. Dependency fetching currently applies to GoogleTest. The current GUI uses vector primitives and needs no runtime image assets. Session cancellation and stale-result rejection are implemented in `Session`; platform clipboard and file-dialog integration remain future work.

Troubleshooting: install CMake 3.24+ and a working C++20 toolchain; Ninja is optional. For GUI support, install SFML 3.0.2 and enable `CHESS_BUILD_GUI`.
