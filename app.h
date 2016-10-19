#ifndef __APP_H__
#define __APP_H__

#include<SDL_ttf.h>
#include"menu.h"
#include"duck.h"

#define KEY (event.key.keysym.sym)
#define DRAW_EVENT SDL_USEREVENT
#define START_DUCKS 3
#define MAX_DUCKS 6
#define SPAWN .45
#define TRIES 5
#define SPAWN_EVENT SDL_JOYBUTTONDOWN

class app
{
public:
  app(const char * filename);
  ~app();
  void event_loop();
  void quit() { _quit = 1; }
  void pause();
  void restart();
  int get_score() {return _score;}
  int get_killed() {return _ducks_killed;}
  int get_level() {return _level;}
  int get_misses() {return _misses;}
  
private:
  void mousebuttondown(int x, int y);
  void keydown(SDL_Keycode sym);
  void draw();
  void sdl_init();
  void check_ducks();
  
  // Members: 
  SDL_Window * _window;
  SDL_Renderer * _rend;
  
  int _app_w, _app_h;
  int _frame_delay;
  int _quit;
  int _menu_height;
  menu * _menu;
  SDL_Texture * _background;
  SDL_Texture * _grass;
  SDL_Rect _grass_dest;
  // game
  duck ** _ducks;
  int _score;
  int _ducks_killed;
  SDL_Texture * _alive;
  SDL_Texture * _dead;
  int _num_ducks;
  int _level;
  int _misses;
  int _play;
};

Uint32 signal_draw_event(Uint32 interval, void * data);
Uint32 signal_spawn_event(Uint32 interval, void * data);

#endif
