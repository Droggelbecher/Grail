
#include "event.h"

void freeUserEventData(SDL_Event& evt) {
  if(evt.type == SDL_USEREVENT) {
    delete (Event*)(evt.user.data1);
    evt.user.data1 = 0;
  }
}
