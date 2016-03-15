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

#include "CGSTypeConversions.h"
#include "Utility.h"

uint8_t floatToUNORM8( const float& d )
{
	return U::clamp( ( d ) * UINT8_MAX + FLOAT_ADJUSTMENT, 0, UINT8_MAX );
}

uint16_t floatToUNORM16( const float& d )
{
	return U::clamp( ( d ) * UINT16_MAX + FLOAT_ADJUSTMENT, 0, UINT16_MAX );
}

uint32_t floatToUNORM32( const float& d )
{
	return U::clamp( ( d ) * UINT32_MAX + FLOAT_ADJUSTMENT, 0, UINT32_MAX );
}


float unorm8ToFloat( const uint8_t& d )
{
	return float( d ) / UINT8_MAX;
}

float unorm16ToFloat( const uint16_t& d )
{
	return float( d ) / UINT16_MAX;
}

float unorm32ToFloat( const uint32_t& d )
{
	return float( d ) / UINT32_MAX;
}


int8_t floatToSNORM8( const float& d )
{
	if( d >= 0.0 )
	{ // d is positive
		return U::clamp( ( d ) * INT8_MAX + FLOAT_ADJUSTMENT, 0, INT8_MAX );
	}
	else
	{ // d is negative
		return U::clamp( ( -d ) * INT8_MIN - FLOAT_ADJUSTMENT, INT8_MIN, 0 );
	}
}

int16_t floatToSNORM16( const float& d )
{
	if( d >= 0.0 )
	{ // d is positive
		return U::clamp( ( d ) * INT16_MAX + FLOAT_ADJUSTMENT, 0, INT16_MAX );
	}
	else
	{ // d is negative
		return U::clamp( ( -d ) * INT16_MIN - FLOAT_ADJUSTMENT, INT16_MIN, 0 );
	}
}

int32_t floatToSNORM32( const float& d )
{
	if( d >= 0.0 )
	{ // d is positive
		return U::clamp( ( d ) * INT32_MAX + FLOAT_ADJUSTMENT, 0, INT32_MAX );
	}
	else
	{ // d is negative
		return U::clamp( ( -d ) * INT32_MIN - FLOAT_ADJUSTMENT, INT32_MIN, 0 );
	}
}


float snorm8ToFloat( const int8_t& d )
{
	if( d >= 0 )
	{ // d is positive
		return (float)( d ) / INT8_MAX;
	}
	else
	{ // d is negative
		return -(float)( d ) / INT8_MIN;
	}
}

float snorm16ToFloat( const int16_t& d )
{
	if( d >= 0 )
	{ // d is positive
		return (float)( d ) / INT16_MAX;
	}
	else
	{ // d is negative
		return -(float)( d ) / INT16_MIN;
	}
}

float snorm32ToFloat( const int32_t& d )
{
	if( d >= 0 )
	{ // d is positive
		return (float)( d ) / INT32_MAX;
	}
	else
	{ // d is negative
		return -(float)( d ) / INT32_MIN;
	}
}


uint16_t floatToHalf( const float& d )
{
	U::log( "Half conversions not implemented yet. Badger Erbit about it!" );
	return 0;
}

float halfToFloat( const uint16_t& d )
{
	U::log( "Half conversions not implemented yet. Badger Erbit about it!" );
	return 0;
}
