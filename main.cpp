#include"app.h"


int main()
{  
  app a("config.txt");
  
  a.event_loop();
  
  return 0;
}
