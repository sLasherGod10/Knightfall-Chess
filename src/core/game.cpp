#include "rookforge/game.hpp"
#include <algorithm>
#include <cstdlib>
#include <utility>
namespace rookforge {
Game::Game(Board start):board_(std::move(start)){positions_.push_back(board_.hash());repetitions_[board_.hash()]=1;clock_.start(std::chrono::minutes(10));}
bool Game::play(Move m){if(result()!=Result::Ongoing)return false;auto legal=board_.legalMoves();if(std::find(legal.begin(),legal.end(),m)==legal.end())return false;Undo u;if(!board_.makeMove(m,u))return false;undo_.push_back(u);moves_.push_back(m);positions_.push_back(board_.hash());++repetitions_[board_.hash()];drawOffered_=false;clock_.completeMove(u.turn);return true;}
bool Game::undo(){if(moves_.empty())return false;--repetitions_[board_.hash()];board_.unmakeMove(moves_.back(),undo_.back());moves_.pop_back();undo_.pop_back();positions_.pop_back();forced_=Result::Ongoing;drawOffered_=false;return true;}
void Game::restart(Board start){board_=std::move(start);moves_.clear();undo_.clear();positions_.clear();repetitions_.clear();positions_.push_back(board_.hash());repetitions_[board_.hash()]=1;forced_=Result::Ongoing;drawOffered_=false;clock_.start(std::chrono::minutes(10));}
bool Game::claimThreefold()const{return repetitions_.contains(board_.hash())&&repetitions_.at(board_.hash())>=3;}
bool Game::claimFiftyMove()const{return board_.halfmoveClock()>=100;}
bool Game::claimDraw(){if(result()!=Result::Ongoing||(!claimThreefold()&&!claimFiftyMove()))return false;forced_=Result::Draw;return true;}
Result Game::result()const{if(forced_!=Result::Ongoing)return forced_;if(clock_.expired()){Color flagged=clock_.remaining(Color::White)==ChessClock::Duration::zero()?Color::White:Color::Black;Color winner=flagged==Color::White?Color::Black:Color::White;bool matingMaterial=false;for(int sq=0;sq<64;++sq){int p=static_cast<int>(board_.at(sq));if(!p||(p<0)!=(winner==Color::Black)||std::abs(p)==6)continue;if(std::abs(p)==1||std::abs(p)>=4)matingMaterial=true;}return matingMaterial?(winner==Color::White?Result::WhiteWins:Result::BlackWins):Result::Draw;}auto legal=board_.legalMoves();if(legal.empty())return board_.inCheck(board_.turn())?(board_.turn()==Color::White?Result::BlackWins:Result::WhiteWins):Result::Draw;auto repetition=repetitions_.find(board_.hash());if(board_.halfmoveClock()>=150||(repetition!=repetitions_.end()&&repetition->second>=5))return Result::Draw;int minors=0,bishops=0,bishopColor=-1;bool sameColorBishops=true;for(int sq=0;sq<64;++sq){int p=static_cast<int>(board_.at(sq));if(!p||std::abs(p)==6)continue;if(std::abs(p)==1)return Result::Ongoing;if(std::abs(p)==3){++bishops;int color=(sq%8+sq/8)%2;if(bishopColor<0)bishopColor=color;else if(color!=bishopColor)sameColorBishops=false;}else if(std::abs(p)==2)++minors;else return Result::Ongoing;}if(minors+bishops<=1||(minors==0&&bishops>0&&sameColorBishops))return Result::Draw;return Result::Ongoing;}
std::string Game::status()const{switch(result()){case Result::WhiteWins:return "White wins";case Result::BlackWins:return "Black wins";case Result::Draw:return "Draw";case Result::Ongoing:return board_.inCheck(board_.turn())?"Check":"In progress";}return "In progress";}
void Game::resign(Color side){forced_=side==Color::White?Result::BlackWins:Result::WhiteWins;}
void Game::offerDraw(Color side){drawOfferer_=side;drawOffered_=true;}
bool Game::acceptDraw(Color side){if(!drawOffered_||side==drawOfferer_)return false;forced_=Result::Draw;drawOffered_=false;return true;}
}
