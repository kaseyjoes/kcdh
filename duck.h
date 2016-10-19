#ifndef __DUCK_H__
#define __DUCK_H__

#include"animate.h"

#define ADUCK_W 36
#define ADUCK_H 31
#define DDUCK_W 33
#define DDUCK_H 31
#define FRAMES 3
#define MAX_SPEED 8
#define DY 5
#define MOVE .1
#define FLIPD .5
#define OFFSCREEN 10000

#define DEST_W (ADUCK_W * 1.4)
#define DEST_H (ADUCK_H * 1.4)

#define PRESP -1
#define DEAD 0
#define ALIVE 1
#define SHOT 2
#define FALL 3
#define AWAY 4
#define ALIVEF 3
#define SHOTF 8
#define FALLF 20

#define LEFT 1
#define RIGHT 0

class duck
{
 public:
  duck(SDL_Texture * alive, SDL_Texture * dead, SDL_Renderer * rend, 
       int app_w, int app_h);
  ~duck();
  int get_state() { return _state; }
  int shoot(int x, int y);
  void reset();
  void start();
  void stop() {_state = PRESP;}
  void draw(SDL_Renderer * rend);
  void move(int app_w, int app_h);
  void init() {_state = DEAD;}
 private:
  void flip_duck(int dir);
  void flip_duck();
  
  int _state, _flip_left;
  animate * _fly;
  image * _shot, * _fall;
  int _dx, _dy;
  int _pos_x, _pos_y;
  int _app_w, _app_h;
  int _frame_count;
};



#endif
