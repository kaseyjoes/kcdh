#ifndef __IMAGE_H__
#define __IMAGE_H__


#include<iostream>
#include<stdlib.h>
#include<assert.h>
#include<SDL.h>
#include<SDL_image.h>

using namespace std;

class image
{
 public:

  // Methods: 
  image(const char * filename, SDL_Renderer * rend, int width, 
	int height, int srcx, int srcy);
  image(const char * filename, SDL_Renderer * rend, int width, int height);
  image(SDL_Texture * texture, int width,
	int height, int srcx, int srcy);
  virtual ~image();
  void move(int newx, int newy);
  void set_dx(int dx) {_dx = dx;}
  void set_dy(int dy) {_dy = dy;}
  void set_center(int x, int y);
  int collide(image * other);
  int check_corners(image * other);
  int point_in(int x, int y);
  virtual void draw(SDL_Renderer * rend);
  void drawhoriz(SDL_Renderer * rend);
  bool is_valid(int app_w, int app_h);
  bool click(int x, int y);
  int get_x() {return _dest.x;}
  int get_y() {return _dest.y;}
  
  void set_size(int w, int h){ _dest.w = w;
                               _dest.h = h; }
  
  
 protected:
  // Members:
  SDL_Rect _src, _dest;
  SDL_Texture * _texture;
  int _dx, _dy;
  
};



#endif
