#ifndef __ANIMATE_H__
#define __ANIMATE_H__

#define FRAME_TIME 3
#include"image.h"



class animate
{
 public:
  animate(SDL_Renderer * rend, char * filename, int num_frames, 
	  int width, int height, int srcx, int srcy);
  animate(SDL_Texture * sheet, int num_frames, int width, 
	  int height, int srcx, int srcy);
  ~animate();
  void start();
  void stop();
  int get_animating();
  void move(int newx, int newy);
  void draw(SDL_Renderer * rend, int next_frame, int horiz);
  bool click(int x, int y);
  void set_dx(int dx);
  void set_dy(int dy);
  bool is_valid(int app_w, int app_h);
  void set_size(int w, int h);
 private:
  int _num_frames;
  image ** _frames;
  int _which_frame;
  int _animating;
  SDL_Rect _collide;
  
};

#endif
