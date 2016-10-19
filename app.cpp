#include"app.h"
#include"menu.h"

//   - constructor
app::app(const char * filename)
{
  int i;
  char token[1024];
  char tokenb[1024];
  ifstream fin(filename);
  assert(fin.is_open());
  
  // APPOPTIONS
  fin>>token;
  fin>>token;  fin>>_app_w;  //   APPW: 800
  fin>>token;  fin>>_app_h;  //   APPH: 600
  fin>>token;  fin>>_menu_height;  //   BUTTONH: 50
  fin>>token;  fin>> _frame_delay;  //   FRAMEDELAY: 50
  fin>>token; fin>>token;// background
  srand(time(0));
  sdl_init();

  _background = IMG_LoadTexture(_rend, token);
  if (! _background)
    {
      cerr<<"Could not load image: "<<IMG_GetError()<<endl;
      exit(1);
    }
  fin>>token; fin>>token;
  _grass = IMG_LoadTexture(_rend, token);
  if (! _grass)
    {
      cerr<<"Could not load image: "<<IMG_GetError()<<endl;
      exit(1);
    }
  
  _quit = 0;
  


  _menu = new menu(fin, _rend, this, _menu_height, _app_w, _app_h);
  assert(_menu);
  _ducks = new duck*[MAX_DUCKS];
  assert(_ducks);
  
  _num_ducks = START_DUCKS;
  _level = 1;
  fin>>token; // DUCKOPTIONS
  fin>>token;
  fin>>token;// filename
  fin>>tokenb;
  fin>>tokenb;// filename
  _alive = IMG_LoadTexture(_rend, token);
  if (! _alive)
    {
      cerr<<"Could not load image: "<<IMG_GetError()<<endl;
      exit(1);
    }

  _dead = IMG_LoadTexture(_rend, tokenb);
  if (! _dead)
    {
      cerr<<"Could not load image: "<<IMG_GetError()<<endl;
      exit(1);
    }

  for (i = 0; i < MAX_DUCKS; i++)
    {
      _ducks[i] = new duck(_alive, _dead, _rend, _app_w, _app_h);
      assert(_ducks[i]);
    }
  
  _score = 0;
  _ducks_killed = 0;
  _misses = TRIES;
  _play = 1;
  for (i = 0; i < _num_ducks; i++)
    _ducks[i]->init();
  SDL_QueryTexture(_grass, 0, 0, &_grass_dest.w, &_grass_dest.h);
  _grass_dest.x = 0;
  _grass_dest.y = 510;
  _grass_dest.w *= 2;
  _grass_dest.h += 50;
  
}

//   - destructor
app::~app()
{
  int i;
  for (i = 0; i < MAX_DUCKS; i++)
    delete _ducks[i];
  
  delete [] _ducks;
  delete _menu;
  SDL_DestroyRenderer(_rend);
  SDL_DestroyWindow(_window);
  TTF_Quit();
  SDL_VideoQuit();
  SDL_Quit();
}

//   - event loop
void app::event_loop()
{
  int i;
  SDL_Event event;
  _quit = 0;
  SDL_AddTimer(_frame_delay, signal_draw_event, 0);
  SDL_AddTimer(_frame_delay*20, signal_spawn_event, 0);
  
  while (! _quit)
    {
      if (SDL_WaitEvent(&event))
	{
	  if (event.type == SDL_QUIT)
	    _quit = 1;
	  if (event.type == SDL_MOUSEBUTTONDOWN)
	    mousebuttondown(event.button.x, event.button.y);
	  if (event.type == SDL_KEYDOWN)
	    keydown(KEY);
	  if (event.type == DRAW_EVENT)
	    draw();
	  if (event.type == SPAWN_EVENT)
	    check_ducks();
	}
      if (_level == 1 && _ducks_killed >= 20)
	{
	  _level++;
	  _num_ducks++;
	  _ducks[_num_ducks-1]->init();
	}
      else if (_level == 2 && _ducks_killed >= 50)
	{
	  _level++;
	  _ducks[_num_ducks]->init();
	  _num_ducks++;
	}
      
      if (! _play)
	{
	  for (i = 0; i < _num_ducks; i++)
	    _ducks[i]->stop();
	  _play = -1;
	}
      
      if (! _misses)
	{
	  _misses = TRIES;
	  _play = 0;
	}
    }
}

//      - mousebuttondown
void app::mousebuttondown(int x, int y)
{
  int i;
  if ( y <= _menu_height)
    _menu->click(x, y);
  else
    for (i = 0; i < _num_ducks; i++)
      if (_ducks[i]->shoot(x, y))
	{
	  _ducks_killed++;
	  _score += (i+1) * 100;
	}
}

//      - keydown
void app::keydown(SDL_Keycode sym)
{
  if (sym == SDLK_ESCAPE)
    _quit = 1;
}

//      - draw
void app::draw()
{
  int i;
  SDL_RenderCopy(_rend, _background, 0, 0);

  
  for (i = 0; i < _num_ducks; i++)
    {
      _ducks[i]->move(_app_w, _app_h);
      _ducks[i]->draw(_rend);
      if (_ducks[i]->get_state() == AWAY)
	{
	  _misses--;
	  _ducks[i]->init();
	}
    }
  _menu->draw(_rend);
  SDL_RenderCopy(_rend, _grass, 0, &_grass_dest);
  SDL_RenderPresent(_rend);
}

Uint32 signal_draw_event(Uint32 interval, void * data)
{
  SDL_Event e;
  
  e.type = DRAW_EVENT;
  
  SDL_PushEvent(&e);
  
  return interval;
}


void app::sdl_init()
{
    // init stuff
  if ( SDL_Init(SDL_INIT_VIDEO))
    {
      cerr<<"SDL Init failed: "<<SDL_GetError()<<endl;
      exit(1);
    }
  
  _window = SDL_CreateWindow("Life", SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED, _app_w, _app_h, 0);
  if (! _window)
    {
      cerr<<"Could not create window: "<<SDL_GetError()<<endl;
      exit(1);
    }
  
  _rend = SDL_CreateRenderer(_window, -1, 0);
  if (!_rend)
    {
      cerr<<"Could not allocate renderer: "<<SDL_GetError()<<endl;
      exit(1);
    }
  
  if (TTF_Init())
    {
      cerr<<"Could not open TTF: "<<TTF_GetError()<<endl;
      exit(1);
    }
}
Uint32 signal_spawn_event(Uint32 interval, void * data)
{
  SDL_Event e;
  
  e.type = SPAWN_EVENT;
  
  SDL_PushEvent(&e);
  
  return interval;
}

void app::check_ducks()
{
  int i;
  if ((double) rand() / RAND_MAX <= SPAWN)
    {
      for (i = 0; i< MAX_DUCKS; i++)
	if ((!_ducks[i]->get_state() && _ducks[i]->get_state() != PRESP) ||
	    _ducks[i]->get_state() == AWAY)
	  {
	    _ducks[i]->reset();
	    _ducks[i]->start();
	    break;
	  }
	} 
}

void app::pause()
{
  SDL_Event event;
  while (_menu->get_paused() && ! _quit)
    {
      if (SDL_WaitEvent(&event))
	{
	  if (event.type == SDL_QUIT)
	    _quit = 1;
	  if (event.type == SDL_MOUSEBUTTONDOWN)
	      if ( event.button.y <= _menu_height)
		_menu->click(event.button.x, event.button.y);
	}
    }
}

void app::restart()
{
  int i;
  
  for (i = 0; i < MAX_DUCKS; i++)
    _ducks[i]->reset();
  
  _num_ducks = START_DUCKS;
  
  for (i = 0; i < _num_ducks; i++)
    _ducks[i]->init();
  
  _ducks_killed = 0;
  _score = 0;
  _play = 1;
  _misses = TRIES;
}
