#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace rookforge {
enum class Color : std::uint8_t { White, Black };
enum class Piece : std::int8_t { Empty, Pawn, Knight, Bishop, Rook, Queen, King };
struct Move { int from{-1}; int to{-1}; Piece promotion{Piece::Empty}; friend bool operator==(const Move&, const Move&) = default; };
struct Undo { Piece captured{}; int ep{}; int halfmove{}; int fullmove{}; unsigned castling{}; Color turn{}; std::uint64_t hash{}; };
class Board {
 public:
  Board();
  static Board fromFen(const std::string& fen);
  std::string fen() const;
  std::vector<Move> legalMoves() const;
  bool makeMove(Move move, Undo& undo);
  void unmakeMove(Move move, const Undo& undo);
  bool inCheck(Color color) const;
  bool attacked(int square, Color by) const;
  Color turn() const { return turn_; }
  Piece at(int square) const { return squares_.at(static_cast<std::size_t>(square)); }
  std::uint64_t hash() const { return hash_; }
  int halfmoveClock() const { return halfmove_; }
  static int square(const std::string& name);
  static std::string squareName(int square);
 private:
  std::array<Piece,64> squares_{};
  Color turn_{Color::White};
  unsigned castling_{15};
  int ep_{-1}, halfmove_{}, fullmove_{1};
  std::uint64_t hash_{};
  void refreshHash();
  std::vector<Move> pseudoMoves() const;
};
std::string moveUci(Move move);
}
