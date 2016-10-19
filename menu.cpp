#include"menu.h"
#include"app.h"


menu::menu(ifstream & fin, SDL_Renderer * rend, app * theapp, 
	   int menu_height, int app_w, int app_h)
{
  char token[1024];
  int i, button_w = app_w / NUM_BUTTONS;
  
  _the_app = theapp;
  
  _buttons = new text_image*[NUM_BUTTONS];
  assert(_buttons);
  
  fin>>token; // read out menu options

  read_font(fin);
  _background = IMG_LoadTexture(rend, (char*) "imgs/menu_bg.png");
  if (! _background)
    {
      cerr<<"Could not load image: "<<IMG_GetError()<<endl;
      exit(1);
    }
  _button = IMG_LoadTexture(rend, (char*) "imgs/button.png");
  if (! _button)
    {
      cerr<<"Could not load image: "<<IMG_GetError()<<endl;
      exit(1);
    }
  
  for (i = 0; i < NUM_BUTTONS; i++)
    {
      _buttons[i] = read_button(fin, rend, button_w, menu_height);
      _buttons[i]->set_center(i*button_w + button_w / 2, menu_height / 2);
    }

  _score = 0;
  _ducks_killed = 0;
  _misses = _the_app->get_misses();
}


void menu::read_font(ifstream & fin)
{
  char token[1024], filename[1024];
  unsigned int combined = 0x0;
  
  fin>>token; // reads out FONTFILE
  fin>>filename; // reads out the file name
  
  fin>>token; // read out FONTSIZE
  fin>>_pt_size; // read out the pt size
  
  _font = TTF_OpenFont(filename, _pt_size);
  if (! _font)
    {
      cerr<<"Could not open font: "<<TTF_GetError()<<endl;
      exit(1);
    }
  
  fin>>token; // read out FONTCOLOR
  fin>>hex>>combined;
  
  _color.b = combined & 0xFF;
  _color.g = (combined & 0xFF00) >> 8;
  _color.r = (combined & 0xFF0000) >> 16;
}

text_image * menu::read_button(ifstream & fin, SDL_Renderer * rend, 
			  int button_w, int button_h)
{
  char token[1024], text[1024];
  int i;
  char c;
  
  text_image * img;
  
  fin>>token; // reads out the button label
  
  fin>>token; // reads out the filename
  
      for (c = 'a'; c != '\"'; fin>>c); // reads out chars until "
      
      for (i = 0, fin>>c; c != '\"'; i++, fin>>c)
	text[i] = c; // store chars into text
 
      text[i] = '\0';// add a null/eos char
      
      
      
      img = new text_image(_button, rend, button_w, button_h, 
			   text, _font, &_color);

  assert(img);
  return img;
}

menu::~menu()
{
  int i;
  
  TTF_CloseFont(_font);
  
  for (i = 0; i < NUM_BUTTONS; i++)
    delete _buttons[i];
  
  delete [] _buttons;
}

// + click
void menu::click(int x, int y)
{
  int i;
  
  for (i = 0; i < NUM_BUTTONS; i++)
    if (_buttons[i]->click(x, y))
      break;

  if (i == PAUSE)
    {
      toggle_paused();
      _the_app->pause();
    }
  if (i == RESTART)
    _the_app->restart();
  if (i == QUIT)
    _the_app->quit();
}


// + draw
void menu::draw(SDL_Renderer * rend)
{
  int i;
  text_image * ti;
  _ducks_killed = _the_app->get_killed();
  _score = _the_app->get_score();
  _misses = _the_app->get_misses();
  
  ostringstream gen_text;
  ostringstream gen_textb;
  ostringstream textc;
  gen_text<<"Kills: "<<_ducks_killed;
  gen_textb<<"Score: "<<_score;
  textc<<"Tries: "<<_misses;
  
  ti = (text_image*) _buttons[KILL_DUCKS];
  ti->set_text(gen_text.str().c_str(), rend);
  ti = (text_image*) _buttons[SCORE];
  ti->set_text(gen_textb.str().c_str(), rend);

  ti = (text_image*) _buttons[MISS];
  ti->set_text(textc.str().c_str(), rend);
  
  for (i = 0; i < NUM_BUTTONS; i++)
    _buttons[i]->draw(rend);
}

void menu::toggle_paused()
{
  if (_paused)
    _paused = 0;
  else
    _paused = 1;
}
