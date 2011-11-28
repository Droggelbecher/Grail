// vim: set noexpandtab:

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <stdint.h>
#include <SDL.h>

namespace grail {

template <typename T, int N> class Vector2d;

template <typename T, int N> Vector2d<T, N> operator*(double scalar, Vector2d<T, N> v);
template <typename T, int N> Vector2d<T, N> operator*(Vector2d<T, N> v, double scalar);
template <typename T, int N> bool operator==(Vector2d<T, N> a, Vector2d<T, N> b);
template <typename T, int N> bool operator!=(Vector2d<T, N> a, Vector2d<T, N> b);
template <typename T, int N> std::ostream& operator<<(std::ostream&, Vector2d<T, N>);

/**
 * A 2-element vector of type T.
 * The N is so you can make real typedefs (instead of the alias function the
 * "typedef" keyword provides)
 *
 * Example:
 * Vector2d<int32_t, 0> is a different type than Vector2d<int32_t, 1> 
 * although they are both 2d-vectors of int32_t.
 * This can be useful for dispatching.
 */
template <typename T, int N=0>
class Vector2d {
	
	public:
		typedef T X;
		typedef T Y;
		
	private:
		X _x; Y _y;
		
	public:
		Vector2d<T, N>() : _x(0), _y(0) { }
		Vector2d<T, N>(T x, T y) : _x(x), _y(y) { }
		
		X getX() const { return _x; }
		Y getY() const { return _y; }
		void setX(X x) { _x = x; }
		void setY(Y y) { _y = y; }
		
		Vector2d<T, N> operator+(Vector2d<T, N> other) const;
		Vector2d<T, N> operator-(Vector2d<T, N> other) const;
		Vector2d<T, N> operator/(double scalar) const;
		Vector2d<T, N> operator-() const;
		
		bool fitsIn(Vector2d<T, N> other) {
			return _x <= other._x && _y <= other._y;
		}
		
		//T operator*(const Vector2d<T, N> other) { return _x * other._x + _y * other._y; }
		T cross(const Vector2d<T, N> other) { return _x * other._y - _y * other._x; }
		
		friend Vector2d<T, N> operator*<T, N>(double, Vector2d<T, N>);
		friend Vector2d<T, N> operator*<T, N>(Vector2d<T, N>, double);
		friend bool operator==<T, N>(Vector2d<T, N>, Vector2d<T, N>);
		friend bool operator!=<T, N>(Vector2d<T, N>, Vector2d<T, N>);
		
		/**
		 * Partition the unit circle into $directions equal pie slices,
		 * return the number of the slice this vector (when drawn from the circles
		 * center) lies in.
		 *
		 * Slice 0 is centered around the vector pointing straight in positive y
		 * direction (on computer screens this is normally "down").
		 * A vector pointing in positive y direction ("down"), will have direction
		 * 0, other directions follow counter-clock-wise.
		 */
		uint16_t nearestDirection(uint16_t directions, double offset=0) const;
		
		double length() const;
		
		Vector2d<T, N> getPosition() const { return *this; }
};

typedef Vector2d<int32_t, 0> VirtualPosition;
typedef VirtualPosition VirtualSize;

typedef Vector2d<int32_t, 1> PhysicalPosition;
typedef PhysicalPosition PhysicalSize;

/*
 * conv
 * Handy ultra-specialized template function to convert various formats /
 * representations of screen positions into another.
 */

template <typename A, typename B> B conv(A a);

template <> SDL_Rect conv<PhysicalPosition, SDL_Rect>(PhysicalPosition p);
template <> SDL_Rect conv<VirtualPosition, SDL_Rect>(VirtualPosition p);
template <> VirtualPosition conv<PhysicalPosition, VirtualPosition>(PhysicalPosition p);
template <> PhysicalPosition conv<VirtualPosition, PhysicalPosition>(VirtualPosition p);

template <> VirtualPosition conv<const SDL_MouseButtonEvent&, VirtualPosition>(const SDL_MouseButtonEvent& p);
template <> VirtualPosition conv<const SDL_MouseMotionEvent&, VirtualPosition>(const SDL_MouseMotionEvent& p);

} // namespace grail

#endif // VECTOR2D_H

