
#include <cmath>
#include <SDL/SDL.h>
#include "classes.h"
#include "vector2d.h"
#include "game.h"
#include "viewport.h"

template <typename T, int N>
Vector2d<T, N> Vector2d<T, N>::operator+(Vector2d<T, N> other) const {
  return Vector2d<T, N>(_x + other._x, _y + other._y);
}

template <typename T, int N>
Vector2d<T, N> Vector2d<T, N>::operator-(Vector2d<T, N> other) const {
  return Vector2d<T, N>(_x - other._x, _y - other._y);
}

template <typename T, int N>
uint8_t Vector2d<T, N>::nearestDirection(uint8_t directions) const {
  double alpha = atan2(_x, _y);
  alpha += M_PI - (M_PI / (double)directions);
  return (uint8_t)floor(alpha * directions / (2.0 * M_PI));
}

template <typename A, typename B>
B conv(A a) { assert(false); }

template <>
SDL_Rect conv<PhysicalPosition, SDL_Rect>(PhysicalPosition p) {
  SDL_Rect r;
  r.x = p.getX();
  r.y = p.getY();
  return r;
}

template <>
VirtualPosition conv<PhysicalPosition, VirtualPosition>(PhysicalPosition p) {
  Viewport& vp = Game::getInstance().getViewport();

  return VirtualPosition(
      p.getX() * vp.getVirtualSize().getX() / (double)vp.getPhysicalWidth(),
      p.getY() * vp.getVirtualSize().getY() / (double)vp.getPhysicalHeight()
      );
}

template <>
PhysicalPosition conv<VirtualPosition, PhysicalPosition>(VirtualPosition p) {
  Viewport& vp = Game::getInstance().getViewport();

  return PhysicalPosition(
      p.getX() * vp.getPhysicalWidth() / (double)vp.getVirtualSize().getX(),
      p.getY() * vp.getPhysicalHeight() / (double)vp.getVirtualSize().getY()
      );
}

template <>
VirtualPosition conv<SDL_MouseButtonEvent&, VirtualPosition>(SDL_MouseButtonEvent& p) {
  return conv<PhysicalPosition, VirtualPosition>(
      PhysicalPosition(p.x, p.y)
  );
}

template <>
VirtualPosition conv<SDL_MouseMotionEvent&, VirtualPosition>(SDL_MouseMotionEvent& p) {
  return conv<PhysicalPosition, VirtualPosition>(
      PhysicalPosition(p.x, p.y)
  );
}

