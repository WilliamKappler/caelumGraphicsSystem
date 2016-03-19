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

#ifndef CGSUTILITY_H
#define	CGSUTILITY_H

#include "CGSDepends.h"

// Windows G++ is broken and no one seems inclined to fix it, with regards to
// to_string.
// From http://stackoverflow.com/a/20861692/3995702
#ifdef _WIN32
namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm;
        stm << n;
        return stm.str( );
    }
}
#endif

namespace U
{
	//////////////////////////////////////////////////////////////////////////////
	// Concatenation/conversion function c()

	// This will catch pointers and prevent implicit casting to int, which would
	// cause errors on 64 bit.
	
	template< class T >
	inline String c( T a )
	{
		return std::to_string( a );
	}
	
	template< class T >
	inline String c( T const* a )
	{
		return c( (uintptr_t)a );
	}

	inline String c( String s )
	{
		return s;
	}

	inline String c( const char s[] )
	{
		return s;
	}

	// Forward declare to avoid issues resolving the proper template function
	template< class T, typename... Args >
	inline String c( T a, Args... args );
	template< class T, typename... Args >
	inline String c( T const* a, Args... args );
	template< typename... Args >
	inline String c( String s, Args... args );
	template< typename... Args >
	inline String c( const char s[], Args... args );

	// Actual implementations

	template< class T, typename... Args >
	inline String c( T a, Args... args )
	{
		return c( a ) + c( args... );
	}

	template< class T, typename... Args >
	inline String c( T const* a, Args... args )
	{
		return c( a ) + c( args... );
	}

	template< typename... Args >
	inline String c( String s, Args... args )
	{
		return s + c( args... );
	}

	template< typename... Args >
	inline String c( const char s[], Args... args )
	{
		return s + c( args... );
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// Pair function p()

	template< typename A, typename B >
	inline Pair<A, B> p( const A& a, const B& b )
	{
		return Pair<A, B>( a, b );
	}

	//////////////////////////////////////////////////////////////////////////////
	// Log function log()
	
	// In the future, this should provide an option to log to a file instead of
	// stdout.

	template< typename... Arguments >
	inline void log( Arguments... args )
	{
		std::cout << c( args... ) << std::endl;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// Clamping function clamp()
	
	template< typename T1, typename T2, typename T3 >
	T1 clamp( const T1& original, const T2& lower, const T3& upper )
	{
		if( original < lower )
		{
			return lower;
		}
		else if( original > upper )
		{
			return upper;
		}
		return original;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// Math functions min() and max()
	
	// WARNING: These suffer from an issue in that, if types do not match, you
	// need to acknowledge the potential for rollover. For example, 
	// U::min( (uint32_t)50, 0.4252, 525.62, 50, 32, -2356 ) = 4294964940
	// It is strongly suggested you never use these on variables with differing
	// signage, and you should avoid using them on variables of differing size.
	// 
	// I will try to find a better solution for this issue in the future.
	template< class T1, class T2 >
	inline T1 max( const T1& v1, const T2& v2 )
	{
		return ( v1 < v2 ) ? v2 : v1;
	}
	
	template< class T, typename... Arguments >
	inline T max( const T& v1, Arguments... args )
	{
		return ( v1 < max( args... ) ) ? max( args... ) : v1 ;
	}
	
	template< class T1, class T2 >
	inline T1 min( const T1& v1, const T2& v2 )
	{
		return ( v1 < v2 ) ? v1 : v2;
	}
	
	template< class T, typename... Arguments >
	inline T min( const T& v1, Arguments... args )
	{
		return ( v1 < min( args... ) ) ? v1 : min( args... ) ;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// String functions stringToInt() and stringToFloat()
	
	inline int stringToInt( const String& str ) { return std::stoi( str ); }
	inline int stringToFloat( const String& str ) { return std::stof( str ); }
};

#endif	/* CGSUTILITY_H */

