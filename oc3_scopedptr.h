// This file is part of openCaesar3.
//
// openCaesar3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// openCaesar3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with openCaesar3.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __OPENCAESAR3_SCOPEDPTR_H_INCLUDE_
#define __OPENCAESAR3_SCOPEDPTR_H_INCLUDE_

#include "oc3_requirements.h"

template <typename T>
struct ScopedPtrDeleter
{
	static inline void cleanup(T *pointer)
	{
		// Enforce a complete type.
		// If you get a compile error here, you need add destructor in class-container
		typedef char IsIncompleteType[ sizeof(T) ? 1 : -1 ];
		(void) sizeof(IsIncompleteType);

		delete pointer;
	}
};

template <typename T>
struct ScopedPtrArrayDeleter
{
	static inline void cleanup(T *pointer)
	{
		// Enforce a complete type.
		// If you get a compile error here, you need add destructor in class-container
		typedef char IsIncompleteType[ sizeof(T) ? 1 : -1 ];
		(void) sizeof(IsIncompleteType);

		delete [] pointer;
	}
};

template <typename T, typename Cleanup = ScopedPtrDeleter<T> >
class ScopedPtr
{
public:
	explicit inline ScopedPtr(T *p = 0) : d(p)
	{
	}

	inline ~ScopedPtr()
	{
		T *oldD = this->d;
		Cleanup::cleanup(oldD);
		this->d = 0;
	}

	inline T &operator*() const
	{
		_OC3_DEBUG_BREAK_IF( !d );
		return *d;
	}

	inline T *operator->() const
	{
		_OC3_DEBUG_BREAK_IF( !d );
		return d;
	}

	inline bool operator!() const
	{
		return !d;
	}

	inline operator bool() const
	{
		return isNull() ? 0 : &ScopedPtr::d;
	}

	inline T *data() const
	{
		return d;
	}

	inline bool isNull() const
	{
		return !d;
	}

	inline void reset(T *other = 0)
	{
		if (d == other)
			return;
		T *oldD = d;
		d = other;
		Cleanup::cleanup(oldD);
	}

	inline T *take()
	{
		T *oldD = d;
		d = 0;
		return oldD;
	}

	inline void swap(ScopedPtr<T, Cleanup> &other)
	{
		std::swap(d, other.d);
	}

	typedef T *pointer;

protected:
	T *d;

private:
	_OC3_DISABLE_COPY(ScopedPtr)
};

template <class T, class Cleanup>
inline bool operator==(const ScopedPtr<T, Cleanup> &lhs, const ScopedPtr<T, Cleanup> &rhs)
{
	return lhs.data() == rhs.data();
}

template <class T, class Cleanup>
inline bool operator!=(const ScopedPtr<T, Cleanup> &lhs, const ScopedPtr<T, Cleanup> &rhs)
{
	return lhs.data() != rhs.data();
}

template <typename T, typename Cleanup = ScopedPtrArrayDeleter<T> >
class ScopedArrayPtr : public ScopedPtr<T, Cleanup>
{
public:
	explicit inline ScopedArrayPtr(T *p = 0)
		: ScopedPtr<T, Cleanup>(p)
	{
	}

	inline T &operator[](int i)
	{
		return this->d[i];
	}

	inline const T &operator[](int i) const
	{
		return this->d[i];
	}

private:
	_OC3_DISABLE_COPY(ScopedArrayPtr)
};

#endif //__OPENCAESAR3_SCOPEDPTR_H_INCLUDE_