#pragma once
#include "rookforge/board.hpp"
#include <chrono>
namespace rookforge {
class ChessClock {
 public:
  using Duration=std::chrono::milliseconds;
  void start(Duration initial,Duration increment=Duration::zero());
  void resume(); void pause(); void tick(); bool expired() const;
  Duration remaining(Color side) const;
  void completeMove(Color side);
 private:
  Duration white_{},black_{},increment_{}; bool running_{}; Color active_{Color::White};
  std::chrono::steady_clock::time_point last_{};
};
}
