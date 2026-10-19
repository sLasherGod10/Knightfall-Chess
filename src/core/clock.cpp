#include "rookforge/clock.hpp"
namespace rookforge {
void ChessClock::start(Duration initial,Duration increment){white_=black_=initial;increment_=increment;active_=Color::White;running_=true;last_=std::chrono::steady_clock::now();}
void ChessClock::resume(){if(!running_){last_=std::chrono::steady_clock::now();running_=true;}}
void ChessClock::pause(){tick();running_=false;}
void ChessClock::tick(){if(!running_)return;auto now=std::chrono::steady_clock::now();auto elapsed=std::chrono::duration_cast<Duration>(now-last_);(active_==Color::White?white_:black_)-=elapsed;last_=now;}
bool ChessClock::expired() const{return remaining(Color::White)<=Duration::zero()||remaining(Color::Black)<=Duration::zero();}
ChessClock::Duration ChessClock::remaining(Color side) const {auto result=side==Color::White?white_:black_;if(running_&&side==active_)result-=std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now()-last_);return result>Duration::zero()?result:Duration::zero();}
void ChessClock::completeMove(Color side){tick();if(side!=active_)return;(side==Color::White?white_:black_)+=increment_;active_=side==Color::White?Color::Black:Color::White;last_=std::chrono::steady_clock::now();}
}
