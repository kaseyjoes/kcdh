#include"image.h"


image::image(const char * filename, SDL_Renderer * rend, int width, 
	     int height, int srcx, int srcy)
{
  _texture = IMG_LoadTexture(rend, filename);
  if (! _texture)
    {
      cerr<<"Could not load image: "<<IMG_GetError()<<endl;
      exit(1);
    }
  _src.x = srcx;
  _src.y = srcy;
  _dest.x = _dest.y = 0;
  _dest.w = _src.w = width;
  _dest.h = _src.h = height;
  _dx = _dy = 0;
}

image::image(SDL_Texture * texture, int width,
	     int height, int srcx, int srcy)
{
  _texture = texture;
  _src.x = srcx;
  _src.y = srcy;
  _dest.x = _dest.y = 0;
  _dest.w = _src.w = width;
  _dest.h = _src.h = height;
  _dx = _dy = 0;
}


image::image(const char * filename, SDL_Renderer * rend, int width, int height)
{
  _texture = IMG_LoadTexture(rend, filename);
  if (! _texture)
    {
      cerr<<"Could not load image: "<<IMG_GetError()<<endl;
      exit(1);
    }
  _src.x = 0;
  _src.y = 0;
  _dest.x = _dest.y = 0;
  _dest.w = width;
  _dest.h = height;
  _dx = _dy = 0;
}

image::~image()
{
  SDL_DestroyTexture(_texture);
}


void image::move(int newx, int newy)
{
  _dest.x = newx;
  _dest.y = newy;
}

void image::set_center(int x, int y)
{
  _dest.x = x - _dest.w/2;
  _dest.y = y - _dest.h/2;
}

void image::draw(SDL_Renderer * rend)
{
  SDL_RenderCopy(rend, _texture, &_src, &_dest);
}

void image::drawhoriz(SDL_Renderer * rend)
{
  SDL_RenderCopyEx(rend, _texture, &_src, &_dest, 0, NULL, 
		   SDL_FLIP_HORIZONTAL);
}

bool image::click(int x, int y)
{
  return x >= _dest.x && x <= _dest.x + _dest.w && 
    y >= _dest.y && y <= _dest.y + _dest.h;
}

int image::collide(image * other)
{
  return check_corners(other) || other->check_corners(this);
}

int image::check_corners(image * other)
{
  return other->point_in(_dest.x, _dest.y) ||
    other->point_in(_dest.x + _dest.w, _dest.y) ||
    other->point_in(_dest.x+_dest.w,
		    _dest.y+_dest.h) ||
    other->point_in(_dest.x, _dest.y+_dest.h);
}

int image::point_in(int x, int y)
{
  if ( x < _dest.x || x > _dest.x + _dest.w )
    return 0;
  if ( y < _dest.y || y > _dest.y + _dest.h )
    return 0;
  
  return 1;
}

bool image::is_valid(int app_w, int app_h)
{
  if (_dest.x < 0)
    return false;
  if (_dest.x > app_w-_dest.w)
    return false;
  if (_dest.y < 0)
    return false;
  if (_dest.y > app_h -_dest.h)
    return false;
  
  return true;
}
