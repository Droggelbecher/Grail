
#include <cmath>
#include <SDL/SDL.h>
#include "classes.h"
#include "vector2d.h"
#include "game.h"
#include "viewport.h"

#include <iostream>
using namespace std;

template <typename T, int N>
Vector2d<T, N> Vector2d<T, N>::operator+(Vector2d<T, N> other) const {
  return Vector2d<T, N>(_x + other._x, _y + other._y);
}

template <typename T, int N>
Vector2d<T, N> Vector2d<T, N>::operator-(Vector2d<T, N> other) const {
  return Vector2d<T, N>(_x - other._x, _y - other._y);
}

template <typename T, int N>
Vector2d<T, N> Vector2d<T, N>::operator/(double scalar) const {
  return Vector2d<T, N>((T)((double)_x / scalar), (T)((double)_y / scalar));
}

template <typename T, int N>
Vector2d<T, N> Vector2d<T, N>::operator-() const {
  return Vector2d<T, N>(-_x, -_y);
}


template <typename T, int N>
uint16_t Vector2d<T, N>::nearestDirection(uint16_t directions) const {
  if(directions < 2) { return 0; }

  double alpha = atan2(_x, _y);
  //cerr << "x=" << _x << " y=" << _y << " aplha=" << (alpha / (2.0 * M_PI)) << endl;
  alpha = alpha + (M_PI / (double)directions);
  if(alpha < 0) {
    alpha += 2.0 * M_PI;
  }
  //cerr << "alpha'=" << (alpha / (2.0 * M_PI)) << endl;
  return (uint16_t)floor(alpha * directions / (2.0 * M_PI));
}

template <typename T, int N>
double Vector2d<T, N>::length() const {
  return sqrt((double)(_x * _x) + (double)(_y * _y));
}

template <typename T, int N>
Vector2d<T, N> operator*(double scalar, Vector2d<T, N> v) {
  return Vector2d<T, N>((T)((double)v._x * scalar), (T)((double)v._y * scalar));
}

template <typename T, int N>
Vector2d<T, N> operator*(Vector2d<T, N> v, double scalar) {
  return Vector2d<T, N>((T)((double)v._x * scalar), (T)((double)v._y * scalar));
}

template <typename T, int N>
bool operator==(Vector2d<T, N> a, Vector2d<T, N> b) {
  return a._x == b._x && a._y == b._y;
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
SDL_Rect conv<VirtualPosition, SDL_Rect>(VirtualPosition p) {
  return conv<PhysicalPosition, SDL_Rect>(
      conv<VirtualPosition, PhysicalPosition>(p)
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

