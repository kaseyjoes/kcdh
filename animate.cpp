#include"animate.h"


animate::animate(SDL_Renderer * rend, char * filename, int num_frames, 
		 int width, int height, int srcx, int srcy)
{
  int i, j = 0;
  _num_frames = num_frames * FRAME_TIME;
  _frames = new image*[_num_frames];
  assert(_frames);
  
  for (i = 0; i < _num_frames; i++)
    {
      _frames[i] = new image(filename, rend, width, height, j, srcy);
      assert(_frames[i]);
      if (i % FRAME_TIME == 0 && i != 0)
	j+= width;
    }
  _collide.x = _collide.y = 0;
  _collide.w = width;
  _collide.h = height;
  _which_frame = 0;
  _animating = 0;
}

animate::animate(SDL_Texture * sheet, int num_frames, int width, 
	int height, int srcx, int srcy)
{
  int i;
  _num_frames = num_frames;
  _frames = new image*[_num_frames];
  assert(_frames);
  for (i = 0; i < _num_frames; i++)
    {
      _frames[i] = new image(sheet, width, height, i * width, srcy);
      assert(_frames[i]);
    }
  _collide.x = _collide.y = 0;
  _collide.w = width;
  _collide.h = height;
  _which_frame = 0;
  _animating = 0;
}

animate::~animate()
{
  int i;
  
  for (i = 0; i < _num_frames; i++)
    delete _frames[i];
  
  delete [] _frames;
}

void animate::start()
{
  _animating = 1;
  _which_frame = 0;
}

void animate::stop()
{
  _which_frame = 0;
  _animating = 0;
}

void animate::move(int newx, int newy)
{
  int i;
  
  for (i = 0; i < _num_frames; i++)
    _frames[_which_frame]->move(newx, newy);
}

int animate::get_animating()
{
  return _animating;
}

void animate::draw(SDL_Renderer * rend, int next_frame, int horiz)
{
  if (_animating)
    {
      if (horiz)
	_frames[_which_frame]->drawhoriz(rend);
      else
	_frames[_which_frame]->draw(rend);
      if (!next_frame)
	_which_frame++;
      if (_which_frame == _num_frames)
	_which_frame = 0;
    }
}

bool animate::click(int x, int y)
{
  return _frames[_which_frame]->click(x, y);
}


void animate::set_dx(int dx)
{
  int i;
  for (i = 0; i < _num_frames; i++)
    _frames[i]->set_dx(dx);
}

void animate::set_dy(int dy)
{
  int i;
  for (i = 0; i < _num_frames; i++)
    _frames[i]->set_dy(dy);
}

bool animate::is_valid(int app_w, int app_h)
{
  return _frames[_which_frame]->is_valid(app_w, app_h);
}

void animate::set_size(int w, int h)
{
  int i;
  for (i = 0; i< _num_frames; i++)
    _frames[i]->set_size(w, h);
}
