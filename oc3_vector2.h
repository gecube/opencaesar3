#ifndef __OPENCAESAR3_VECTOR2_INCLUDED__
#define __OPENCAESAR3_VECTOR2_INCLUDED__

#include <oc3_math.h>

template <class T>
class Vector2
{
public:
	//! Default constructor (null vector)
	Vector2() : x(0), y(0) {}
	//! Constructor with two different values
	Vector2(T nx, T ny) : x(nx), y(ny) {}
	//! Constructor with the same value for both members
	explicit Vector2(T n) : x(n), y(n) {}
	//! Copy constructor
	Vector2(const Vector2<T>& other) : x(other.X), y(other.Y) {}

	// operators
	Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

	Vector2<T>& operator=(const Vector2<T>& other) { x = other.x; y = other.y; return *this; }

	Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(x + other.x, y + other.y); }
	Vector2<T>& operator+=(const Vector2<T>& other) { x+=other.X; y+=other.Y; return *this; }
	Vector2<T> operator+(const T v) const { return Vector2<T>(x + v, y + v); }
	Vector2<T>& operator+=(const T v) { x+=v; y+=v; return *this; }

	Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(x - other.X, y - other.Y); }
	Vector2<T>& operator-=(const Vector2<T>& other) { x-=other.X; y-=other.Y; return *this; }
	Vector2<T> operator-(const T v) const { return Vector2<T>(x - v, y - v); }
	Vector2<T>& operator-=(const T v) { x-=v; y-=v; return *this; }

	Vector2<T> operator*(const Vector2<T>& other) const { return Vector2<T>(x * other.X, y * other.Y); }
	Vector2<T>& operator*=(const Vector2<T>& other) { x*=other.X; y*=other.Y; return *this; }
	Vector2<T> operator*(const T v) const { return Vector2<T>(x * v, y * v); }
	Vector2<T>& operator*=(const T v) { x*=v; y*=v; return *this; }

	Vector2<T> operator/(const Vector2<T>& other) const { return Vector2<T>(x / other.X, y / other.Y); }
	Vector2<T>& operator/=(const Vector2<T>& other) { x/=other.X; y/=other.Y; return *this; }
	Vector2<T> operator/(const T v) const { return Vector2<T>(x / v, y / v); }
	Vector2<T>& operator/=(const T v) { x/=v; y/=v; return *this; }

	//! sort in order X, Y. Equality with rounding tolerance.
    bool operator<=(const Vector2<T>& other) const
	{
        return 	(x<other.X || math::isEqual(x, other.X)) ||
                (math::isEqual(x, other.X) && (y<other.Y || math::isEqual(y, other.Y)));
	}

	//! sort in order X, Y. Equality with rounding tolerance.
	bool operator>=(const Vector2<T>&other) const
	{
                return 	(x>other.X || math::isEqual(x, other.X)) ||
                                (math::isEqual(x, other.X) && (y>other.Y || math::isEqual(y, other.Y)));
	}

	//! sort in order X, Y. Difference must be above rounding tolerance.
	bool operator<(const Vector2<T>&other) const
	{
                return 	(x<other.X && !math::isEqual(x, other.X)) ||
                                (math::isEqual(x, other.X) && y<other.Y && !math::isEqual(y, other.Y));
	}

	//! sort in order X, Y. Difference must be above rounding tolerance.
	bool operator>(const Vector2<T>&other) const
	{
        return 	(x>other.X && !math::isEqual(x, other.X)) ||
                                (math::isEqual(x, other.X) && y>other.Y && !math::isEqual(y, other.Y));
	}

        bool operator==(const Vector2<T>& other) const { return isEqual(other, math::ROUNDING_ERROR_f32); }
        bool operator!=(const Vector2<T>& other) const { return !isEqual(other, math::ROUNDING_ERROR_f32); }

	// functions

    //! Checks if this vector equals the other one.
	/** Takes floating point rounding errors into account.
	\param other Vector to compare with.
	\return True if the two vector are (almost) equal, else false. */
    bool IsEqual(const Vector2<T>& other, float tolerance) const
	{
        return math::isEqual(x, other.X, tolerance) && math::isEqual(y, other.Y, tolerance);
	}

	Vector2<T>& set(T nx, T ny) {x=nx; y=ny; return *this; }
	Vector2<T>& set(const Vector2<T>& p) { x=p.X; y=p.Y; return *this; }

	//! Gets the length of the vector.
	/** \return The length of the vector. */
    T getLength() const { return sqrt( x*x + y*y ); }

	//! Get the squared length of this vector
	/** This is useful because it is much faster than getLength().
	\return The squared length of the vector. */
	T getLengthSQ() const { return x*x + y*y; }

	//! Get the dot product of this vector with another.
	/** \param other Other vector to take dot product with.
	\return The dot product of the two vectors. */
	T dotProduct(const Vector2<T>& other) const
	{
		return x*other.X + y*other.Y;
	}

    template< class A >
    Vector2<A> As()
    {
        return Vector2<A>( (A)x, (A)y );
    }

    template< class A >
    Vector2<A> As() const
    {
        return Vector2<A>( (A)x, (A)y );
    }

	//! Gets distance from another point.
	/** Here, the vector is interpreted as a point in 2-dimensional space.
	\param other Other vector to measure from.
	\return Distance from other point. */
	T getDistanceFrom(const Vector2<T>& other) const
	{
		return Vector2<T>(x - other.X, y - other.Y).getLength();
	}

	//! Returns squared distance from another point.
	/** Here, the vector is interpreted as a point in 2-dimensional space.
	\param other Other vector to measure from.
	\return Squared distance from other point. */
	T getDistanceFromSQ(const Vector2<T>& other) const
	{
		return Vector2<T>(x - other.X, y - other.Y).getLengthSQ();
	}

	//! rotates the point anticlockwise around a center by an amount of degrees.
	/** \param degrees Amount of degrees to rotate by, anticlockwise.
	\param center Rotation center.
	\return This vector after transformation. */
	Vector2<T>& rotateBy(float degrees, const Vector2<T>& center=Vector2<T>())
	{
        degrees *= math::DEGTORAD64;
		const float cs = cos(degrees);
		const float sn = sin(degrees);

		x -= center.X;
		y -= center.Y;

		set((T)(x*cs - y*sn), (T)(x*sn + y*cs));

		x += center.X;
		y += center.Y;
		return *this;
	}

	//! Normalize the vector.
	/** The null vector is left untouched.
	\return Reference to this vector, after normalization. */
	Vector2<T>& normalize()
	{
		float length = (float)(x*x + y*y);
        
        if (math::isEqual(length, 0.f))
			return *this;
        length = 1.f / sqrt( length );
		x = (T)(x * length);
		y = (T)(y * length);
		return *this;
	}

	//! Calculates the angle of this vector in degrees in the trigonometric sense.
	/** 0 is to the right (3 o'clock), values increase counter-clockwise.
	This method has been suggested by Pr3t3nd3r.
	\return Returns a value between 0 and 360. */
	float getAngleTrig() const
	{
		if (y == 0)
			return x < 0 ? 180 : 0;
		else
		if (x == 0)
			return y < 0 ? 270 : 90;

		if ( y > 0)
			if (x > 0)
                                return atanf((float)y/(float)x) * math::RADTODEG;
			else
                                return 180.0-atanf((float)y/-(float)x) * math::RADTODEG;
		else
			if (x > 0)
                                return 360.0-atanf(-(float)y/(float)x) * math::RADTODEG;
			else
                                return 180.0+atanf(-(float)y/-(float)x) * math::RADTODEG;
	}

	//! Calculates the angle of this vector in degrees in the counter trigonometric sense.
	/** 0 is to the right (3 o'clock), values increase clockwise.
	\return Returns a value between 0 and 360. */
	inline float getAngle() const
	{
		if (y == 0) // corrected thanks to a suggestion by Jox
			return x < 0 ? 180 : 0;
		else if (x == 0)
			return y < 0 ? 90 : 270;

		// don't use getLength here to avoid precision loss with s32 vectors
                float tmp = y /  sqrt((float)(x*x + y*y));
                tmp = atanf( sqrt(1 - tmp*tmp) / tmp) * math::RADTODEG64;

		if (x>0 && y>0)
			return tmp + 270;
		else
		if (x>0 && y<0)
			return tmp + 90;
		else
		if (x<0 && y<0)
			return 90 - tmp;
		else
		if (x<0 && y>0)
			return 270 - tmp;

		return tmp;
	}

	//! Calculates the angle between this vector and another one in degree.
	/** \param b Other vector to test with.
	\return Returns a value between 0 and 90. */
	inline float getAngleWith(const Vector2<T>& b) const
	{
		double tmp = x*b.X + y*b.Y;

		if (tmp == 0.0)
			return 90.0;

                tmp = tmp / sqrtf((float)((x*x + y*y) * (b.X*b.X + b.Y*b.Y)));
		if (tmp < 0.0)
			tmp = -tmp;

                return atanf(sqrtf(1 - tmp*tmp) / tmp) * math::RADTODEG;
	}

	//! Returns if this vector interpreted as a point is on a line between two other points.
	/** It is assumed that the point is on the line.
	\param begin Beginning vector to compare between.
	\param end Ending vector to compare between.
	\return True if this vector is between begin and end, false if not. */
	bool isBetweenPoints(const Vector2<T>& begin, const Vector2<T>& end) const
	{
		if (begin.X != end.X)
		{
			return ((begin.X <= x && x <= end.X) ||
				(begin.X >= x && x >= end.X));
		}
		else
		{
			return ((begin.Y <= y && y <= end.Y) ||
				(begin.Y >= y && y >= end.Y));
		}
	}

	//! Creates an interpolated vector between this vector and another vector.
	/** \param other The other vector to interpolate with.
	\param d Interpolation value between 0.0f (all the other vector) and 1.0f (all this vector).
	Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
	\return An interpolated vector.  This vector is not modified. */
	Vector2<T> getInterpolated(const Vector2<T>& other, T d) const
	{
		float inv = 1.0f - (float)d;
		return Vector2<T>((T)(other.X*inv + x*d), (T)(other.Y*inv + y*d));
	}

	//! Creates a quadratically interpolated vector between this and two other vectors.
	/** \param v2 Second vector to interpolate with.
	\param v3 Third vector to interpolate with (maximum at 1.0f)
	\param d Interpolation value between 0.0f (all this vector) and 1.0f (all the 3rd vector).
	Note that this is the opposite direction of interpolation to getInterpolated() and interpolate()
	\return An interpolated vector. This vector is not modified. */
	Vector2<T> getInterpolated_quadratic(const Vector2<T>& v2, const Vector2<T>& v3, const T d) const
	{
		// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
		const float inv = 1.0f - d;
		const float mul0 = inv * inv;
		const float mul1 = 2.0f * d * inv;
		const float mul2 = d * d;

		return Vector2<T> ( (T)(x * mul0 + v2.X * mul1 + v3.X * mul2),
					(T)(y * mul0 + v2.Y * mul1 + v3.Y * mul2));
	}

	//! Sets this vector to the linearly interpolated vector between a and b.
	/** \param a first vector to interpolate with, maximum at 1.0f
	\param b second vector to interpolate with, maximum at 0.0f
	\param d Interpolation value between 0.0f (all vector b) and 1.0f (all vector a)
	Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
	*/
	Vector2<T>& interpolate(const Vector2<T>& a, const Vector2<T>& b, const T d)
	{
		x = (T)((float)b.X + ( ( a.X - b.X ) * d ));
		y = (T)((float)b.Y + ( ( a.Y - b.Y ) * d ));
		return *this;
	}

	//! X coordinate of vector.
	T x;

	//! Y coordinate of vector.
	T y;
};

#endif

