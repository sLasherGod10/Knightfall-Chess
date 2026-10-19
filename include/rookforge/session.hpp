#pragma once
#include "rookforge/game.hpp"
#include "rookforge/search.hpp"
#include <atomic>
#include <mutex>
#include <memory>
#include <optional>
#include <thread>
namespace rookforge {
class Session {
 public:
  Session()=default; ~Session();
  Session(const Session&)=delete; Session& operator=(const Session&)=delete;
  Game& game(){return game_;} const Game& game()const{return game_;}
  bool humanMove(Move move);
  void startSearch(int depth,ChessClock::Duration limit=ChessClock::Duration::max());
  bool pollSearch(); void cancelSearch(); void restart(Board start=Board{}); bool undo();
 private:
  struct Completed { SearchResult result; std::uint64_t hash; std::uint64_t generation; };
  Game game_; std::jthread worker_; std::shared_ptr<std::atomic_bool> cancel_; std::mutex mutex_; std::optional<Completed> completed_; std::uint64_t generation_{};
};
}
