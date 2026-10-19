#pragma once
#include "rookforge/board.hpp"
#include <atomic>
#include <chrono>
namespace rookforge {
struct SearchResult { Move best{}; int score{}; std::uint64_t nodes{}; int depth{}; };
SearchResult search(Board board, int maxDepth, std::chrono::milliseconds limit=std::chrono::milliseconds::max(), const std::atomic_bool* cancel=nullptr, std::uint64_t nodeLimit=0);
}
