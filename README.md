# RookForge Chess

RookForge is a C++20 chess-engine project with an independent board/move-generation core, search opponent, CLI, game/session APIs, notation support, and an optional SFML desktop board.

## Build

Requires CMake 3.24+ and a C++20 compiler. Build the engine and CLI without graphical dependencies:

```sh
cmake -S . -B build -DCHESS_BUILD_GUI=OFF -DCHESS_BUILD_CLI=ON -DCHESS_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

Run `build/rookforge_cli --help` (Windows: `build\rookforge_cli.exe --help`). Example: `rookforge_cli perft 4`.
For the GUI, install SFML 3.0.2 and configure with `-DCHESS_BUILD_GUI=ON`. The board supports click-to-move, legal move indicators, promotion to queen, AI toggle, restart, undo, and board flip. Keyboard shortcuts are N, U, F, and Escape.

## Current scope

The core includes FEN read/write, legal move filtering, castling, en passant, all four promotions, reversible moves, perft/divide, iterative-deepening alpha-beta search with a deadline, UCI/SAN, basic PGN import/export, repetition tracking, game results, clock accounting, and a cancellable search worker. The CLI supports `perft`, `divide`, `bestmove`, and `play`. The evaluator is material-only; this is not a strength claim.

Known gaps: the GUI is a minimal board rather than a complete desktop product. It lacks move history, clocks/settings UI, FEN/PGN dialogs and file controls, resign/draw controls, drag/drop, promotion choice, full asset packaging, and installer integration. Search evaluation is material-only and does not include piece-square tables, mobility, or king safety. PGN variations are skipped on import and comments are ignored; export uses generic tags. GoogleTest is used when installed or when `CHESS_FETCH_DEPS=ON`; otherwise CTest runs dependency-free assertions. Draw claims are available in the game API but not offered in the GUI.

## Packaging

`scripts/package-windows.ps1` builds and tests the core/CLI. It requires CMake and a working C++ toolchain. It does not currently produce the requested desktop installer. No packages have been produced or verified in this environment.

## License

MIT; see [LICENSE](LICENSE). No third-party runtime assets are currently included.
