#include "rookforge/session.hpp"
#include <memory>
namespace rookforge {
Session::~Session(){cancelSearch();}
bool Session::humanMove(Move move){cancelSearch();return game_.play(move);}
void Session::startSearch(int depth,ChessClock::Duration limit){cancelSearch();if(game_.result()!=Result::Ongoing)return;Board copy=game_.board();auto hash=copy.hash();auto generation=++generation_;cancel_=std::make_shared<std::atomic_bool>(false);auto token=cancel_;worker_=std::jthread([this,copy=std::move(copy),hash,generation,token,depth,limit](std::stop_token stop)mutable{if(stop.stop_requested())token->store(true);auto result=search(std::move(copy),depth,limit,token.get());std::lock_guard lock(mutex_);completed_=Completed{result,hash,generation};});}
bool Session::pollSearch(){std::optional<Completed> ready;{std::lock_guard lock(mutex_);ready.swap(completed_);}if(!ready)return false;if(worker_.joinable())worker_.join();if(ready->generation!=generation_||ready->hash!=game_.board().hash())return false;return game_.play(ready->result.best);}
void Session::cancelSearch(){++generation_;if(cancel_)cancel_->store(true);if(worker_.joinable()){worker_.request_stop();worker_.join();}cancel_.reset();std::lock_guard lock(mutex_);completed_.reset();}
void Session::restart(Board start){cancelSearch();game_.restart(std::move(start));}
bool Session::undo(){cancelSearch();return game_.undo();}
}
