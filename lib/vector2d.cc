
#include <SDL/SDL.h>
#include "vector2d.h"

template <typename T>
Vector2d<T> Vector2d<T>::operator+(Vector2d<T> other) {
  return Vector2d<T>(_x + other._x, y + other._y);
}

template <typename A, typename B>
B conv(A a) { }

template <>
SDL_Rect conv<InternalPosition, SDL_Rect>(InternalPosition p) {
  SDL_Rect r;
  r.x = p.x();
  r.y = p.y();
  return r;
}

