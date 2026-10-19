#include "rookforge/board.hpp"
#include "rookforge/search.hpp"
#include "rookforge/version.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

using namespace rookforge;
namespace {
std::uint64_t perft(Board& b,int depth){if(depth==0)return 1;std::uint64_t n=0;for(auto m:b.legalMoves()){Undo u;b.makeMove(m,u);n+=perft(b,depth-1);b.unmakeMove(m,u);}return n;}
int integer(const char* s){try{std::size_t n{};int v=std::stoi(s,&n);if(s[n]!='\0'||v<0)throw std::invalid_argument("invalid");return v;}catch(...){throw std::invalid_argument(std::string("Invalid non-negative integer: ")+s);}}
void usage(){std::cout<<"RookForge CLI "<<ROOKFORGE_VERSION<<"\nUsage:\n  rookforge_cli perft <depth> [FEN]\n  rookforge_cli divide <depth> [FEN]\n  rookforge_cli bestmove <depth> <milliseconds> [FEN]\n  rookforge_cli play <depth> <milliseconds> [FEN]\n  rookforge_cli version\n";}
}
int main(int argc,char** argv){try{if(argc<2){usage();return 2;}std::string cmd=argv[1];if(cmd=="version"){std::cout<<"RookForge "<<ROOKFORGE_VERSION<<"\n";return 0;}if(cmd=="help"||cmd=="--help"){usage();return 0;}if(cmd!="perft"&&cmd!="divide"&&cmd!="bestmove"&&cmd!="play")throw std::invalid_argument("Unknown command: "+cmd);int need=cmd=="bestmove"||cmd=="play"?4:3;if(argc<need)throw std::invalid_argument("Missing command arguments");int depth=integer(argv[2]);if(depth>12)throw std::invalid_argument("Depth over 12 is not supported by this CLI");if(cmd=="divide"&&depth<1)throw std::invalid_argument("Divide depth must be at least 1");int ms=(cmd=="bestmove"||cmd=="play")?integer(argv[3]):0;int fenIndex=need;Board b=argc>fenIndex?Board::fromFen(argv[fenIndex]):Board{};if(cmd=="perft"){std::cout<<perft(b,depth)<<"\n";return 0;}if(cmd=="divide"){std::uint64_t total=0;for(auto m:b.legalMoves()){Undo u;b.makeMove(m,u);auto n=perft(b,depth-1);b.unmakeMove(m,u);std::cout<<moveUci(m)<<": "<<n<<"\n";total+=n;}std::cout<<"Nodes searched: "<<total<<"\n";return 0;}auto r=search(b,depth,std::chrono::milliseconds(ms));std::cout<<"bestmove "<<moveUci(r.best)<<" score "<<r.score<<" depth "<<r.depth<<" nodes "<<r.nodes<<"\n";if(cmd=="play"){Undo u;b.makeMove(r.best,u);std::cout<<b.fen()<<"\n";}return 0;}catch(const std::exception& e){std::cerr<<"rookforge_cli: "<<e.what()<<"\n";return 2;}}
