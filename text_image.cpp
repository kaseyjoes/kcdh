#include"text_image.h"

text_image::text_image(const char * filename, SDL_Renderer * rend, int width, 
		       int height, const char * text, TTF_Font * font, 
		       SDL_Color * color)
  : image(filename, rend, width, height)
{
  _text_surface = 0;
  _text_texture = 0;
  _text = 0;
  _font = 0;
  set_text(text, rend);
  set_font(font, rend);
  set_color(color, rend);
}

text_image::text_image(SDL_Texture * texture, SDL_Renderer * rend, 
		       int width, int height, const char * text, 
		       TTF_Font * font, SDL_Color * color)
  : image(texture, width, height, 0, 0)
{
  _text_surface = 0;
  _text_texture = 0;
  _text = 0;
  _font = 0;
  set_text(text, rend);
  set_font(font, rend);
  set_color(color, rend);
}

void text_image::render(SDL_Renderer * rend)
{
  if (_text == 0 || _font == 0)
    return;
  
  if (_text_surface)
    {
      SDL_FreeSurface(_text_surface);
      SDL_DestroyTexture(_text_texture);
    }
  
  _text_surface = TTF_RenderText_Solid(_font, _text, _color);
  assert(_text_surface);
  _text_texture = SDL_CreateTextureFromSurface(rend, _text_surface);
  assert(_text_texture);
  
  SDL_QueryTexture(_text_texture, 0, 0, &_text_dest.w, &_text_dest.h);
  if (_text_dest.w > _dest.w)
    _text_dest.w = _dest.w;
  if (_text_dest.h > _dest.h)
    _text_dest.h = _dest.h;
  
  _text_dest.x = _dest.x + (_dest.w - _text_dest.w) / 2;
  _text_dest.y = _dest.y + (_dest.h - _text_dest.h) / 2;
}

void text_image::set_text(const char * text, SDL_Renderer * rend)
{
  if (_text)
    delete [] _text;

  _text = new char[strlen(text) + 1];
  assert(_text);
  
  strcpy(_text, text);
  
  render(rend);
}

void text_image::set_font(TTF_Font * font, SDL_Renderer * rend)
{
  _font = font;
  
  render(rend);
}

void text_image::set_color(SDL_Color * color, SDL_Renderer * rend)
{
  _color = *color;
  
  render(rend);
}

void text_image::draw(SDL_Renderer * rend)
{
  image::draw(rend);
  if ( _text_dest.w > _dest.w )
    _text_dest.w = _dest.w;
  if ( _text_dest.h > _dest.h )
    _text_dest.h = _dest.h; 
  
  _text_dest.x = _dest.x + (_dest.w-_text_dest.w) / 2;
  _text_dest.y = _dest.y + (_dest.h-_text_dest.h) / 2;

  SDL_RenderCopy(rend, _text_texture, 0, &_text_dest);
}

text_image::~text_image()
{
  SDL_FreeSurface(_text_surface);
  SDL_DestroyTexture(_text_texture);
  delete [] _text;
}
