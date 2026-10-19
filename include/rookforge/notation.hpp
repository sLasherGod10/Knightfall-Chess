#pragma once
#include "rookforge/board.hpp"
#include <string>
#include <vector>
namespace rookforge {
Move parseUci(const Board& board,const std::string& text);
std::string san(const Board& board,Move move);
Move parseSan(const Board& board,const std::string& text);
std::string exportPgn(const Board& start,const std::vector<Move>& moves,const std::string& result="*");
std::vector<Move> importPgn(Board& start,const std::string& pgn);
}
