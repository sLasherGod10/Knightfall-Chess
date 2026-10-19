#pragma once
#include "rookforge/board.hpp"
#include "rookforge/clock.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace rookforge {
enum class Result { Ongoing, WhiteWins, BlackWins, Draw };
class Game {
 public:
  explicit Game(Board start=Board{});
  bool play(Move move);
  bool undo(); void restart(Board start=Board{});
  bool claimThreefold() const; bool claimFiftyMove() const; bool claimDraw();
  Result result() const; std::string status() const;
  const Board& board() const { return board_; }
  const std::vector<Move>& moves() const { return moves_; }
  void resign(Color side); void offerDraw(Color side); bool acceptDraw(Color side);
  ChessClock& clock() { return clock_; }
 private:
  Board board_; std::vector<Move> moves_; std::vector<Undo> undo_; std::vector<std::uint64_t> positions_;
  std::unordered_map<std::uint64_t,int> repetitions_; Result forced_{Result::Ongoing}; bool drawOffered_{}; Color drawOfferer_{Color::White}; ChessClock clock_;
};
}
