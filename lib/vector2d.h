
#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <stdint.h>
#include <SDL/SDL.h>

template <typename T>
class Vector2d {
  T _x, _y;

  public:
    Vector2d<T>(T x, T y) : _x(x), _y(y) { }

    Vector2d<T> operator+(Vector2d<T> other);

    T x() { return _x; }
    T y() { return _y; }
};

typedef Vector2d<int32_t> InternalPosition;

/*
 * conv
 */

template <typename A, typename B>
B conv(A a);

template <>
SDL_Rect conv<InternalPosition, SDL_Rect>(InternalPosition p);

#endif // VECTOR2D_H

