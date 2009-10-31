
#include <cmath>
#include <SDL/SDL.h>
#include "classes.h"
#include "vector2d.h"
#include "game.h"
#include "viewport.h"

template <typename T, int N>
Vector2d<T, N> Vector2d<T, N>::operator+(Vector2d<T, N> other) {
  return Vector2d<T, N>(_x + other._x, _y + other._y);
}

template <typename T, int N>
uint8_t Vector2d<T, N>::nearestDirection(uint8_t directions) {
  double alpha = atan2(x(), y());
  alpha += M_PI - (M_PI / (double)directions);
  return (uint8_t)floor(alpha * directions / (2.0 * M_PI));
}

template <typename A, typename B>
B conv(A a) { }

template <>
SDL_Rect conv<VirtualPosition, SDL_Rect>(VirtualPosition p) {
  SDL_Rect r;
  r.x = p.x();
  r.y = p.y();
  return r;
}

template <>
VirtualPosition conv<PhysicalPosition, VirtualPosition>(PhysicalPosition p) {
  Viewport& vp = Game::getInstance().getViewport();

  return VirtualPosition(
      p.x() * vp.getVirtualWidth() / (double)vp.getPhysicalWidth(),
      p.y() * vp.getVirtualHeight() / (double)vp.getPhysicalHeight()
      );
}

template <>
PhysicalPosition conv<VirtualPosition, PhysicalPosition>(VirtualPosition p) {
  Viewport& vp = Game::getInstance().getViewport();

  return PhysicalPosition(
      p.x() * vp.getPhysicalWidth() / (double)vp.getVirtualWidth(),
      p.y() * vp.getPhysicalHeight() / (double)vp.getVirtualHeight()
      );
}


