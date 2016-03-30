/*	Copyright (c) 2015-2016 William Kappler

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
	REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
	AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
	INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
	LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
	OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
	PERFORMANCE OF THIS SOFTWARE. */

#ifndef CGSVECTORS_H
#define CGSVECTORS_H

//#define __STDC_LIMIT_MACROS // Required, but now set via cmake/build command
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <type_traits>

// These are effectively C++ versions of GLSL's vector classes, intended to be
// used for storing similar such data. These possess many of the same qualities
// as GLSL's vectors, such as:
// * The ability to access elements as xyzw, rgba, and/or stpq, as desired.
// * The ability to be constructed with mixed constructors directly, like:
// vec4( 3, vec2( 4, 5 ), 5 ).
// 
// There are, however, some differences, largely due to how C++ works:
// * Types are cast more implicitly than in GLSL - you can input ints to vec,
// which is the floating-point vector. This is simply how C++ works with floats.
// * normalize( ) and length( ) are member functions.
// * You cannot access more than one component at a time (or do swizzling, which
// requires that), such as v4.yyzz. This is not possible in C++. However, a
// function might be added to allow such to an extent.
//
// These classes violate my normal naming convention (upper-first) in order
// to better replicate GLSL's usage of them. They are, effectively, primitives.
// As such, certain types might be aliased to more "proper" types following
// my conventions.
//
// The raw vector templates should not be used. At least typedef them to a proper
// vector type first. That's why they are prefixed with _.

// XY - RG - ST
template< class T, size_t E = 2 >
class _Vector2
{
public:
	T d[E];
	
	// Each of these refers to a cell of the d array.
	T& x( ) { return this->d[0]; }
	T& y( ) { return this->d[1]; }
	
	T& r( ) { return this->d[0]; }
	T& g( ) { return this->d[1]; }
	
	T& s( ) { return this->d[0]; }
	T& t( ) { return this->d[1]; }
	
	_Vector2( )
	{}
	
	_Vector2( const T& _a, const T& _b )
	{
		d[0] = _a;
		d[1] = _b;
	}
	
	_Vector2( const _Vector2< T, 2 >& _in )
	{
		d[0] = _in[0];
		d[1] = _in[1];
	}
	
	T& operator[]( const size_t _i )
	{
		assert( _i < E );
		return d[ _i ];
	}
	
	const T& operator[]( const size_t _i ) const
	{
		assert( _i < E );
		return d[ _i ];
	}
	
	// Must overload assignment - C++ does not like reference members with default operator=.
	_Vector2< T, E >& operator=( const _Vector2< T, E >& _source )
	{
		for( size_t i = 0; i < E; ++i )
		{
			d[ i ] = _source.d[ i ];
		}
		return *this;
	}
	
	// Size-agnostic math functions - these work for _vec3 and _vec4 as well
	// 
	// Even though they explicitly return _Vector2, the internal data is
	// actually larger than 2 if these are called on a _Vector3 or _Vector4.
	// The _Vector2 with E>2 is then silently copy-constructed to a
	// _Vector3 or _Vector4 as needed, with the proper type.
	//
	// Sneaky, I know.
	
	// Scalar
	_Vector2< T, E > operator+( const T& _other ) const
	{
		_Vector2< T, E > temp;
		for( size_t i = 0; i < E; ++i )
		{
			temp[ i ] = d[ i ] + _other;
		}
		return temp;
	}
	
	_Vector2< T, E > operator-( const T& _other ) const
	{
		return *this + ( - _other );
	}
	
	_Vector2< T, E > operator*( const T& _other ) const
	{
		_Vector2< T, E > temp;
		for( size_t i = 0; i < E; ++i )
		{
			temp[ i ] = d[ i ] * _other;
		}
		return temp;
	}
	
	_Vector2< T, E > operator/( const T& _other ) const
	{
		_Vector2< T, E > temp;
		for( size_t i = 0; i < E; ++i )
		{
			temp[ i ] = d[ i ] / _other;
		}
		return temp;
	}
	
	// Special
	_Vector2< T, E > operator-( ) const // Unary -
	{
		_Vector2< T, E > temp;
		for( size_t i = 0; i < E; ++i )
		{
			temp[ i ] = -d[ i ];
		}
		return temp;
	}
	
	// Vector
	_Vector2< T, E > operator+( const _Vector2< T, E >& _other ) const
	{
		_Vector2< T, E > temp;
		for( size_t i = 0; i < E; ++i )
		{
			temp[ i ] = d[ i ] + _other[ i ];
		}
		return temp;
	}
	
	_Vector2< T, E > operator-( const _Vector2< T, E >& _other ) const
	{
		return *this + ( - _other );
	}
	
	// General functions
	float length( ) const
	{
		// length = sqrt( squareSum )
		// squareSum = x^2 + y^2...
		float squareSum = 0;
		for( size_t i = 0; i < E; ++i )
		{
			squareSum += d[ i ] * d[ i ];
		}
		return sqrt( squareSum );
	}
	
	void normalize( )
	{
		static_assert( std::is_floating_point<T>::value, "Normalize cannot be called upon non-float vectors. It is only valid for vecN and dvecN" );
		
		// v_norm = v / |v|
		float mag = length( );
		for( size_t i = 0; i < E; ++i )
		{
			d[ i ] /= mag;
		}
	}
};

// XYZ - RGB - STP
template< class T, size_t E = 3 >
class _Vector3 : public _Vector2< T, E >
{
public:
	T& z( ) { return this->d[2]; }
	
	T& b( ) { return this->d[2]; }
	
	T& p( ) { return this->d[2]; }
	
	_Vector3( )
	{}
	
	_Vector3( const T& _a, const T& _b, const T& _c )
	{
		// 'this' is necessary due to some arcane template rules.
		this->d[0] = _a;
		this->d[1] = _b;
		this->d[2] = _c;
	}
	
protected:
	_Vector3( const T* const& _in )
	{
		this->d[0] = _in[0];
		this->d[1] = _in[1];
		this->d[2] = _in[2];
	}
	
public:
	_Vector3( const _Vector3< T >& _in ) : _Vector3::_Vector3( _in.d )
	{}
	
	_Vector3( const _Vector2< T, 3 >& _in ) : _Vector3::_Vector3( _in.d )
	{}
	
	// Helpful constructors allowing vec3( float, vec2( float, float ) ),
	// etc., like in GLSL.
	_Vector3( const float& _a, const _Vector2< T >& _v )
		: _Vector3( _a, _v[0], _v[1] )
	{}
	
	_Vector3( const _Vector2< T >& _v, const float& _c )
		: _Vector3( _v[0], _v[1], _c )
	{}
};

// XYZW - RGBA - STPQ
template< class T, size_t E = 4 >
class _Vector4 : public _Vector3< T, E >
{
public:
	T& w( ) { return this->d[3]; }
	
	T& a( ) { return this->d[3]; }
	
	T& q( ) { return this->d[3]; }
	
	_Vector4( )
	{}
	
	_Vector4( const T& _a, const T& _b, const T& _c, const T& _d )
	{
		this->d[0] = _a;
		this->d[1] = _b;
		this->d[2] = _c;
		this->d[3] = _d;
	}
	
protected:
	_Vector4( const T* const& _in )
	{
		this->d[0] = _in[0];
		this->d[1] = _in[1];
		this->d[2] = _in[2];
		this->d[3] = _in[3];
	}
	
public:
	_Vector4( const _Vector4< T >& _in ) : _Vector4::_Vector4( _in.d )
	{}
	
	_Vector4( const _Vector3< T, 4 >& _in ) : _Vector4::_Vector4( _in.d )
	{}
	
	_Vector4( const _Vector2< T, 4 >& _in ) : _Vector4::_Vector4( _in.d )
	{}
	
	// Helper constructors
	_Vector4( const _Vector2< T >& _va, const float& _c, const float& _d )
		: _Vector4( _va[0], _va[1], _c, _d )
	{}
	
	_Vector4( const float& _a, const _Vector2< T >& _vb, const float& _d )
		: _Vector4( _a, _vb[0], _vb[1], _d )
	{}
	
	_Vector4( const float& _a, const float& _b, const _Vector2< T >& _vc )
		: _Vector4( _a, _b, _vc[0], _vc[1] )
	{}
	
	_Vector4( const _Vector2< T >& _va, const _Vector2< T >& _vc )
		: _Vector4( _va[0], _va[1], _vc[0], _vc[1] )
	{}
	
	_Vector4( const _Vector3< T >& _va, const float& _d )
		: _Vector4( _va[0], _va[1], _va[2], _d )
	{}
	
	_Vector4( const float& _a, const _Vector3< T >& _vb )
		: _Vector4( _a, _vb[0], _vb[1], _vb[2] )
	{}
};

// NON-MEMBER FUNCTIONS ==========================================================

// scalar * vector is not a member function. A second T is needed to avoid 
// possible mismatch between castable types.
template< class T, class T2, size_t E >
_Vector2< T, E > operator*( const T2& _scalar, const _Vector2< T, E >& _vector )
{
	_Vector2< T, E > temp;
	for( size_t i = 0; i < E; ++i )
	{
		temp[ i ] = _vector.d[ i ] * _scalar;
	}
	return temp;
}

// scalar + vector
template< class T, class T2, size_t E >
_Vector2< T, E > operator+( const T2& _scalar, const _Vector2< T, E >& _vector )
{
	return _vector + _scalar;
}

// scalar - vector
template< class T, class T2, size_t E >
_Vector2< T, E > operator-( const T2& _scalar, const _Vector2< T, E >& _vector )
{
	return ( - _vector ) + _scalar;
}

// vector
template< class T, size_t E >
T dotProduct( _Vector2< T, E > v1, _Vector2< T, E > v2 )
{
	_Vector2< T, E > temp;
	for( size_t i = 0; i < E; ++i )
	{
		temp[ i ] = v1.d[ i ] * v2.d[ i ];
	}
	return temp;
}

template< class T >
_Vector3< T, 3 > crossProduct( _Vector3< T, 3 > a, _Vector3< T, 3 > b )
{
	//             |i  j  k |    |A1*B2 - B1*A2|
	// A x B = det(|A0 A1 A2|) = |A2*B0 - B2*A0|
	//             |B0 B1 B2|    |A0*B1 - B0*A1|
	
	_Vector3< T, 3 > temp;
	temp[ 0 ] = a[ 1 ] * b[ 2 ] - b[ 1 ] * a[ 2 ];
	temp[ 1 ] = a[ 2 ] * b[ 0 ] - b[ 2 ] * a[ 0 ];
	temp[ 2 ] = a[ 0 ] * b[ 1 ] - b[ 0 ] * a[ 1 ];
	return temp;
}

// END-USER TYPEDEFS =============================================================

// float vectors, vecN
using vec2 = _Vector2< float >;
using vec3 = _Vector3< float >;
using vec4 = _Vector4< float >;

// double vectors, dvecN
using dvec2 = _Vector2< double >;
using dvec3 = _Vector3< double >;
using dvec4 = _Vector4< double >;

// bool vectors, bvecN
using bvec2 = _Vector2< bool >;
using bvec3 = _Vector3< bool >;
using bvec4 = _Vector4< bool >;

// signed integer vectors, ivecN
using ivec2 = _Vector2< int32_t >;
using ivec3 = _Vector3< int32_t >;
using ivec4 = _Vector4< int32_t >;

// unsigned integer vectors, uvecN
using uvec2 = _Vector2< uint32_t >;
using uvec3 = _Vector3< uint32_t >;
using uvec4 = _Vector4< uint32_t >;

#endif	/* CGSVECTORS_H */