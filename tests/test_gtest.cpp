#include "rookforge/board.hpp"
#include "rookforge/notation.hpp"
#include "rookforge/search.hpp"
#include <gtest/gtest.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
using namespace rookforge;
namespace { std::uint64_t perft(Board& b,int depth){if(!depth)return 1;std::uint64_t nodes=0;for(auto move:b.legalMoves()){Undo undo;b.makeMove(move,undo);nodes+=perft(b,depth-1);b.unmakeMove(move,undo);}return nodes;} }
TEST(Board, InitialFenAndLegalMoves){Board board;EXPECT_EQ(board.fen(),"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");EXPECT_EQ(board.legalMoves().size(),20U);EXPECT_EQ(Board::fromFen(board.fen()).fen(),board.fen());}
TEST(MoveGeneration, InitialPerftReferences){Board board;EXPECT_EQ(perft(board,2),400U);EXPECT_EQ(perft(board,3),8902U);EXPECT_EQ(perft(board,5),4865609U);}
TEST(Board, MakeUnmakeRestoresPosition){Board board;auto before=board.fen();auto move=board.legalMoves().front();Undo undo;ASSERT_TRUE(board.makeMove(move,undo));board.unmakeMove(move,undo);EXPECT_EQ(board.fen(),before);}
TEST(Notation, UciAndSan){Board board;auto move=parseUci(board,"e2e4");EXPECT_EQ(san(board,move),"e4");EXPECT_EQ(moveUci(move),"e2e4");}
TEST(Search, ReturnsLegalMoveUnderNodeLimit){Board board;auto result=search(board,8,std::chrono::milliseconds::max(),nullptr,40);auto legal=board.legalMoves();EXPECT_NE(std::find(legal.begin(),legal.end(),result.best),legal.end());}
