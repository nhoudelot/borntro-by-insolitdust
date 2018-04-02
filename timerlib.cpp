#include <SDL/SDL.h>

#include "timerlib.h"

TIMER::TIMER()
{
  StartTime = SDL_GetTicks();
}

TIMER::~TIMER()
{
}

long long TIMER::GetCount()
{
  return (SDL_GetTicks() - StartTime) * 1000;
}