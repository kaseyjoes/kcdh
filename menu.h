#ifndef __MENU_H__
#define __MENU_H__

#include"text_image.h"
class app;
#include<iostream>
#include<fstream>
#include<iomanip>
#include<sstream>

using namespace std;

#define NUM_BUTTONS 6
#define PAUSE 0
#define RESTART 1
#define QUIT 2
#define KILL_DUCKS 3
#define SCORE 4
#define MISS 5

class menu
{
 public:
  menu(ifstream & fin, SDL_Renderer * rend, app * theapp, 
       int menu_height, int app_w, int app_h);
  ~menu();
  void click(int x, int y);
  void draw(SDL_Renderer * rend);
  void toggle_paused();
  int get_paused() {return _paused;}
 private:
  
  void read_font(ifstream & fin);
  text_image * read_button(ifstream & fin, SDL_Renderer * rend, 
			    int button_w, int button_h);
  text_image ** _buttons;
  app * _the_app;
  TTF_Font * _font;
  int _pt_size;
  SDL_Color _color;
  int _paused;
  SDL_Texture * _background;
  SDL_Texture * _button;
  
  // game
  int _ducks_killed;
  int _score;
  int _misses;
};


#endif
