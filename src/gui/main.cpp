#include "rookforge/session.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>

namespace {
using namespace rookforge;
constexpr unsigned side=720, cell=90;
int squareAt(sf::Vector2i p,bool flipped){if(p.x<0||p.y<0||p.x>=static_cast<int>(side)||p.y>=static_cast<int>(side))return -1;int f=p.x/static_cast<int>(cell),r=7-p.y/static_cast<int>(cell);return flipped?(7-f)*8+(7-r):r*8+f;}
sf::Color pieceColor(Piece p){return static_cast<int>(p)>0?sf::Color(245,237,214):sf::Color(28,34,45);}
std::array<unsigned char,7> glyph(char c){switch(c){case 'A':return {14,17,17,31,17,17,17};case 'B':return {30,17,17,30,17,17,30};case 'C':return {14,17,16,16,16,17,14};case 'D':return {30,17,17,17,17,17,30};case 'E':return {31,16,16,30,16,16,31};case 'F':return {31,16,16,30,16,16,16};case 'G':return {14,17,16,23,17,17,15};case 'H':return {17,17,17,31,17,17,17};case 'I':return {14,4,4,4,4,4,14};case 'J':return {7,2,2,2,18,18,12};case 'K':return {17,18,20,24,20,18,17};case 'L':return {16,16,16,16,16,16,31};case 'M':return {17,27,21,21,17,17,17};case 'N':return {17,25,25,21,19,19,17};case 'O':return {14,17,17,17,17,17,14};case 'P':return {30,17,17,30,16,16,16};case 'Q':return {14,17,17,17,21,18,13};case 'R':return {30,17,17,30,20,18,17};case 'S':return {15,16,16,14,1,1,30};case 'T':return {31,4,4,4,4,4,4};case 'U':return {17,17,17,17,17,17,14};case 'V':return {17,17,17,17,17,10,4};case 'W':return {17,17,17,21,21,21,10};case 'Y':return {17,17,10,4,4,4,4};case '0':return {14,17,19,21,25,17,14};case '1':return {4,12,4,4,4,4,14};case '2':return {14,17,1,2,4,8,31};case '3':return {30,1,1,14,1,1,30};case ' ':return {};default:return {0,0,0,31,0,0,0};}}
void drawText(sf::RenderWindow& window,const std::string& text,float x,float y,float scale,sf::Color color){float pen=x;for(char c:text){auto bits=glyph(c);for(int r=0;r<7;++r)for(int col=0;col<5;++col)if(bits[static_cast<std::size_t>(r)]&(1u<<(4-col))){sf::RectangleShape pixel({scale,scale});pixel.setPosition({pen+col*scale,y+r*scale});pixel.setFillColor(color);window.draw(pixel);}pen+=6*scale;}}
void drawPiece(sf::RenderWindow& window,Piece p,float x,float y){if(p==Piece::Empty)return;int t=std::abs(static_cast<int>(p));sf::Color fill=pieceColor(p);sf::CircleShape body(static_cast<float>(cell)*0.29F);body.setOrigin({body.getRadius(),body.getRadius()});body.setPosition({x+cell/2.F,y+cell/2.F+5.F});body.setFillColor(fill);body.setOutlineThickness(2.F);body.setOutlineColor(sf::Color(181,143,81));window.draw(body);sf::CircleShape crown(static_cast<float>(cell)*(t==6?0.115F:0.09F));crown.setOrigin({crown.getRadius(),crown.getRadius()});crown.setPosition({x+cell/2.F,y+cell/2.F-10.F});crown.setFillColor(fill);window.draw(crown);sf::RectangleShape base({static_cast<float>(cell)*0.46F,5.F});base.setOrigin({base.getSize().x/2.F,base.getSize().y/2.F});base.setPosition({x+cell/2.F,y+cell*0.73F});base.setFillColor(sf::Color(181,143,81));window.draw(base);if(t==2||t==3||t==4||t==5){sf::RectangleShape mark({4.F,static_cast<float>(cell)*0.24F});mark.setOrigin({2.F,mark.getSize().y/2.F});mark.setPosition({x+cell/2.F,y+cell/2.F+2.F});mark.setFillColor(sf::Color(181,143,81));window.draw(mark);}}
}
int main(){using namespace rookforge;sf::RenderWindow window(sf::VideoMode({1000U,760U}),"RookForge Chess",sf::Style::Default);window.setFramerateLimit(60);Session session;bool flipped=false,ai=false;int selected=-1;Move last{};std::vector<Move> legal;
 while(window.isOpen()){
   while(const auto event=window.pollEvent()){
     if(event->is<sf::Event::Closed>())window.close();
     if(const auto* key=event->getIf<sf::Event::KeyPressed>()){if(key->code==sf::Keyboard::Key::Escape)selected=-1;else if(key->code==sf::Keyboard::Key::F)flipped=!flipped;else if(key->code==sf::Keyboard::Key::N){session.restart();selected=-1;last={};}else if(key->code==sf::Keyboard::Key::U){session.undo();selected=-1;}}
     if(const auto* mouse=event->getIf<sf::Event::MouseButtonPressed>()){
       if(mouse->button==sf::Mouse::Button::Right){selected=-1;continue;}
       if(mouse->button!=sf::Mouse::Button::Left)continue;
       if(mouse->position.x>=720&&mouse->position.x<1000&&mouse->position.y>=120&&mouse->position.y<462){int row=(mouse->position.y-120)/72;if(row==0){ai=!ai;if(ai&&session.game().board().turn()==Color::Black)session.startSearch(3,std::chrono::seconds(3));}else if(row==1){session.restart();selected=-1;last={};}else if(row==2){session.undo();selected=-1;}else if(row==3){flipped=!flipped;selected=-1;}else if(row==4){ai=false;session.cancelSearch();}continue;}
       if(mouse->position.x>=720)continue;
       int sq=squareAt(mouse->position,flipped);if(sq<0||session.game().result()!=Result::Ongoing)continue;
       const Board& board=session.game().board();
       if(selected<0){if(board.at(sq)!=Piece::Empty&&((static_cast<int>(board.at(sq))>0)==(board.turn()==Color::White))){selected=sq;legal.clear();for(auto m:board.legalMoves())if(m.from==sq)legal.push_back(m);}}
       else{auto it=std::find_if(legal.begin(),legal.end(),[&](Move m){return m.to==sq;});if(it!=legal.end()){Move m=*it;if(m.promotion!=Piece::Empty)m.promotion=Piece::Queen;if(session.humanMove(m)){last=m;selected=-1;legal.clear();if(ai)session.startSearch(3,std::chrono::seconds(3));}}else if(board.at(sq)!=Piece::Empty&&((static_cast<int>(board.at(sq))>0)==(board.turn()==Color::White))){selected=sq;legal.clear();for(auto m:board.legalMoves())if(m.from==sq)legal.push_back(m);}else selected=-1;}
     }
   }
   if(ai&&session.game().board().turn()==Color::Black&&session.game().result()==Result::Ongoing&&session.pollSearch()&&!session.game().moves().empty())last=session.game().moves().back();
   window.clear(sf::Color(19,24,32));
   drawText(window,"ROOKFORGE",765.F,28.F,4.F,sf::Color(238,225,199));drawText(window,"CHESS",765.F,63.F,3.F,sf::Color(181,143,81));
   for(int rank=0;rank<8;++rank)for(int file=0;file<8;++file){int f=flipped?7-file:file,r=flipped?rank:7-rank,sq=r*8+f;sf::RectangleShape tile({static_cast<float>(cell),static_cast<float>(cell)});tile.setPosition({static_cast<float>(file*static_cast<int>(cell)),static_cast<float>(rank*static_cast<int>(cell))});bool light=((f+r)%2)==0;tile.setFillColor(light?sf::Color(224,211,184):sf::Color(82,105,116));window.draw(tile);if(sq==last.from||sq==last.to){tile.setFillColor(sf::Color(202,164,81,100));window.draw(tile);}if(sq==selected){tile.setFillColor(sf::Color(212,171,93,115));window.draw(tile);}for(auto m:legal)if(m.to==sq){sf::CircleShape marker(session.game().board().at(sq)==Piece::Empty?8.F:27.F);marker.setOrigin({marker.getRadius(),marker.getRadius()});marker.setPosition({file*static_cast<float>(cell)+cell/2.F,rank*static_cast<float>(cell)+cell/2.F});marker.setFillColor(session.game().board().at(sq)==Piece::Empty?sf::Color(37,43,52,155):sf::Color::Transparent);if(session.game().board().at(sq)!=Piece::Empty){marker.setOutlineThickness(4.F);marker.setOutlineColor(sf::Color(160,70,57,220));}window.draw(marker);}drawPiece(window,session.game().board().at(sq),file*static_cast<float>(cell),rank*static_cast<float>(cell));}
   sf::RectangleShape rail({280.F,760.F});rail.setPosition({720.F,0.F});rail.setFillColor(sf::Color(25,31,41));window.draw(rail);
   auto button=[&](float y,bool on){sf::RectangleShape b({220.F,54.F});b.setPosition({750.F,y});b.setFillColor(on?sf::Color(181,143,81):sf::Color(43,53,66));b.setOutlineThickness(1.F);b.setOutlineColor(sf::Color(181,143,81));window.draw(b);};
   button(120.F,ai);button(192.F,false);button(264.F,false);button(336.F,false);button(408.F,false);
   drawText(window,ai?"TWO PLAYER":"PLAY AI",785.F,143.F,2.F,sf::Color(238,225,199));drawText(window,"NEW GAME",785.F,215.F,2.F,sf::Color(238,225,199));drawText(window,"UNDO",785.F,287.F,2.F,sf::Color(238,225,199));drawText(window,"FLIP BOARD",785.F,359.F,2.F,sf::Color(238,225,199));drawText(window,"TWO PLAYER",785.F,431.F,2.F,sf::Color(238,225,199));
   if(session.game().result()==Result::Ongoing){if(session.game().board().inCheck(session.game().board().turn()))drawText(window,"CHECK",785.F,510.F,3.F,sf::Color(220,113,95));drawText(window,session.game().board().turn()==Color::White?"WHITE TO MOVE":"BLACK TO MOVE",750.F,560.F,2.F,sf::Color(238,225,199));}else drawText(window,session.game().status()=="Draw"?"DRAW":"GAME OVER",750.F,560.F,2.F,sf::Color(181,143,81));
   sf::RectangleShape indicator({190.F,8.F});indicator.setPosition({765.F,93.F});indicator.setFillColor(session.game().board().turn()==Color::White?sf::Color(245,237,214):sf::Color(181,143,81));window.draw(indicator);
   window.display();
 }
 session.cancelSearch();return 0;
}
