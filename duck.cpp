#include"duck.h"


duck::duck(SDL_Texture * alive, SDL_Texture * dead, SDL_Renderer * rend, 
	   int app_w, int app_h)
{
  _frame_count = 0;
  _state = PRESP;
  _flip_left = 0;
  _dx = rand() % MAX_SPEED;
  _dy = - DY;
  _pos_x = rand() % app_w;
  if (_pos_x < 0)
    _pos_x = 0;
  _pos_y = app_h - ADUCK_H;
  
  _app_w = app_w;
  _app_h = app_h;
  _shot = new image(dead, DDUCK_W, DDUCK_H, 0, 0);
  assert(_shot);
  
  _fall = new image(dead, DDUCK_W, DDUCK_H, DDUCK_W + 1, 0);
  assert(_fall);
  
  _fly = new animate(alive, FRAMES, ADUCK_W, ADUCK_H, 0, 0);
  assert(_fly);

  _fly->set_size(DEST_W, DEST_H);
  _fall->set_size(DEST_W, DEST_H);
  _shot->set_size(DEST_W, DEST_H);
  
}

duck::~duck()
{
  delete _shot;
  delete _fall;
  delete _fly;
}

int duck::shoot(int x, int y)
{
  if (_fly->click(x, y) && _state == ALIVE)
    {
      _state = SHOT;
      _frame_count = 0;
      return 1;
    }
  return 0;
}


void duck::draw(SDL_Renderer * rend)
{
  if (_state != DEAD)
    {
      if (_state == ALIVE)
	{
	  _fly->draw(rend, _frame_count++, _flip_left);
	  if (_frame_count == ALIVEF)
	    _frame_count = 0;
	}
      else if (_state == SHOT)
	{
	  _shot->draw(rend);
	  _frame_count++;
	  if (_frame_count == SHOTF)
	    {
	      _state = FALL;
	      _frame_count = 0;
	      _dy = 5;
	    }
	}
      else if (_state == FALL)
	{
	  _fall->draw(rend);
	  _frame_count++;
	  if (_frame_count == FALLF)
	    {
	      _state = DEAD;
	      _frame_count = 0;
	    }
	}
    }
}

void duck::move(int app_w, int app_h)
{
  if ((double) rand() / (double) RAND_MAX <= MOVE)
    {
      _dx = rand() % MAX_SPEED;
      if ((double) rand() / (double) RAND_MAX <= FLIPD)
	flip_duck();
    }
  if (_dx < 0)
    flip_duck(LEFT);
  else
    flip_duck(RIGHT);

    
  if (_state == ALIVE)
    {
      if (!_fly->is_valid(app_w, app_h))
	_dx = -_dx;
      _pos_x += _dx;
      _pos_y += _dy;
      if (_pos_y < 0)
	_state = AWAY;
    }
  else if (_state == FALL)
    {
      _flip_left = 0;
      _dx = 0;
      _dy *= 1.3;
      _pos_y += _dy;
    }
  else if (_state == SHOT)
    {
      _dx = 0;
      _dy = 0;
    }
  else if (_state == DEAD || _state == AWAY)
    _pos_x = _pos_y = OFFSCREEN;
  
  _fly->move(_pos_x, _pos_y);
  _shot->move(_pos_x, _pos_y);
  _fall->move(_pos_x, _pos_y);
    
}

void duck::flip_duck()
{
  if (_flip_left)
    _flip_left = 0;
  else
    _flip_left = 1;
  
  _dx = -_dx;
}

void duck::flip_duck(int dir)
{
  if (dir == LEFT)
    _flip_left = 1;
  else
    _flip_left = 0;
}

void duck::reset()
{
  if (_state != PRESP)
    {
      _state = DEAD;
      _flip_left = 0;
      _dx = rand() % MAX_SPEED;
      _dy = - DY;
      _pos_x = rand() % _app_w;
      _pos_y = _app_h - ADUCK_H - 5;
    }
}

void duck::start()
{
  if (_state != PRESP)
    {
      _state = ALIVE;
      _fly->start();
    }
}
