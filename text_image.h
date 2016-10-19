#ifndef __TEXT_IMAGE_H__
#define __TEXT_IMAGE_H__

#include"image.h"
#include<SDL_ttf.h>
#include<string.h>


class text_image : public image
{
public:
  
  text_image(const char * filename, SDL_Renderer * rend, int width, int height,
	     const char * text, TTF_Font * font, SDL_Color * color);
  text_image(SDL_Texture * texture, SDL_Renderer * rend, 
			 int width, int height, const char * text, 
			 TTF_Font * font, SDL_Color * color);
  ~text_image();
  void set_text(const char * text, SDL_Renderer * rend);
  void set_font(TTF_Font * font, SDL_Renderer * rend);
  void set_color(SDL_Color * color, SDL_Renderer * rend);
  void draw(SDL_Renderer * rend);
  
  
protected:
  
  void render(SDL_Renderer * rend);
  
  char * _text;
  TTF_Font * _font;
  SDL_Color _color;
  
  SDL_Texture * _text_texture;
  SDL_Surface * _text_surface;
  SDL_Rect _text_dest;

};




#endif
