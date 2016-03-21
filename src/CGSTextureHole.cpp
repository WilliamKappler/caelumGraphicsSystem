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
#include "CGSTextureHole.h"
#include "CGSTexture.h"
#include "CGSUtility.h"

#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
CGSTextureHole::CGSTextureHole( uint8_t* const& address, const uint8_t& _channelSize, bool* _textureModifiedFlag )
{
	addressInTexture = address;
	channelSize = _channelSize;
	textureModifiedFlag = _textureModifiedFlag;
}
#else
CGSTextureHole::CGSTextureHole( uint8_t* const& address, bool* _textureModifiedFlag )
{
	addressInTexture = address;
	textureModifiedFlag = _textureModifiedFlag;
}
#endif

#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
# define _textureHoleCheckByteSizeV( SIZE ) if( channelSize != SIZE ) \
{ \
	U::log( "Warning: Invalid texture hole write attempted." ); \
	U::log( " - Write of size ", (int)SIZE, " attempted into hole of size ", (int)channelSize ); \
	return; \
}
# define _textureHoleCheckByteSizeR( SIZE ) if( channelSize != SIZE ) \
{ \
	U::log( "Warning: Invalid texture hole write attempted." ); \
	U::log( " - Write of size ", (int)SIZE, " attempted into hole of size ", (int)channelSize ); \
	return 0; \
}
#else
# define _textureHoleCheckByteSizeV( SIZE )
# define _textureHoleCheckByteSizeR( SIZE )
#endif

// Floating point

void CGSTextureHole::write16F( const float& d )
{
	_textureHoleCheckByteSizeV( 2 );
	
	*((uint16_t*)addressInTexture) = floatToHalf( d );
	
	*textureModifiedFlag = true;
}

float CGSTextureHole::read16F( )
{
	_textureHoleCheckByteSizeR( 2 );
	
	return halfToFloat( *((uint16_t*)addressInTexture) );
}

void CGSTextureHole::write32F( const float& d )
{
	_textureHoleCheckByteSizeV( 4 );
	
	*((float*)addressInTexture) = d;
	
	*textureModifiedFlag = true;
}

float CGSTextureHole::read32F( )
{
	_textureHoleCheckByteSizeR( 4 );
	
	return *((float*)addressInTexture);
}

// Unsigned normalized

void CGSTextureHole::write8_UNORM( const float& d )
{
	_textureHoleCheckByteSizeV( 1 );
	
	*((uint8_t*)addressInTexture) = floatToUNORM8( d );
	
	*textureModifiedFlag = true;
}

float CGSTextureHole::read8_UNORM( )
{
	_textureHoleCheckByteSizeR( 1 );
	
	return unorm8ToFloat( *((uint8_t*)addressInTexture) );
}

void CGSTextureHole::write16_UNORM( const float& d )
{
	_textureHoleCheckByteSizeV( 2 );
	
	*((uint16_t*)addressInTexture) = floatToUNORM16( d );
	
	*textureModifiedFlag = true;
}

float CGSTextureHole::read16_UNORM( )
{
	_textureHoleCheckByteSizeR( 2 );
	
	return unorm16ToFloat( *((uint16_t*)addressInTexture) );
}

// Signed normalized

void CGSTextureHole::write8_SNORM( const float& d )
{
	_textureHoleCheckByteSizeV( 1 );
	
	*((int8_t*)addressInTexture) = floatToSNORM8( d );
	
	*textureModifiedFlag = true;
}

float CGSTextureHole::read8_SNORM( )
{
	_textureHoleCheckByteSizeR( 1 );
	
	return snorm8ToFloat( *((uint8_t*)addressInTexture) );
}

void CGSTextureHole::write16_SNORM( const float& d )
{
	_textureHoleCheckByteSizeV( 2 );
	
	*((int16_t*)addressInTexture) = floatToSNORM16( d );
	
	*textureModifiedFlag = true;
}

float CGSTextureHole::read16_SNORM( )
{
	_textureHoleCheckByteSizeR( 2 );
	
	return snorm16ToFloat( *((uint16_t*)addressInTexture) );
}

// Unsigned int

void CGSTextureHole::write8UI( const uint8_t& d )
{
	_textureHoleCheckByteSizeV( 1 );
	
	*((uint8_t*)addressInTexture) = d;
	
	*textureModifiedFlag = true;
}

uint8_t CGSTextureHole::read8UI( )
{
	_textureHoleCheckByteSizeR( 1 );
	
	return *((uint8_t*)addressInTexture);
}

void CGSTextureHole::write16UI( const uint16_t& d )
{
	_textureHoleCheckByteSizeV( 2 );
	
	*((uint16_t*)addressInTexture) = d;
	
	*textureModifiedFlag = true;
}

uint16_t CGSTextureHole::read16UI( )
{
	_textureHoleCheckByteSizeR( 2 );
	
	return *((uint16_t*)addressInTexture);
}

void CGSTextureHole::write32UI( const uint32_t& d )
{
	_textureHoleCheckByteSizeV( 4 );
	
	*((uint32_t*)addressInTexture) = d;
	
	*textureModifiedFlag = true;
}

uint32_t CGSTextureHole::read32UI( )
{
	_textureHoleCheckByteSizeR( 4 );
	
	return *((uint32_t*)addressInTexture);
}

// Signed int

void CGSTextureHole::write8I( const int8_t& d )
{
	_textureHoleCheckByteSizeV( 1 );
	
	*((int8_t*)addressInTexture) = d;
	
	*textureModifiedFlag = true;
}

int8_t CGSTextureHole::read8I( )
{
	_textureHoleCheckByteSizeR( 1 );
	
	return *((int8_t*)addressInTexture);
}

void CGSTextureHole::write16I( const int16_t& d )
{
	_textureHoleCheckByteSizeV( 2 );
	
	*((int16_t*)addressInTexture) = d;
	
	*textureModifiedFlag = true;
}

int16_t CGSTextureHole::read16I( )
{
	_textureHoleCheckByteSizeR( 2 );
	
	return *((int16_t*)addressInTexture);
}

void CGSTextureHole::write32I( const int32_t& d )
{
	_textureHoleCheckByteSizeV( 4 );
	
	*((int32_t*)addressInTexture) = d;
	
	*textureModifiedFlag = true;
}

int32_t CGSTextureHole::read32I( )
{
	_textureHoleCheckByteSizeR( 4 );
	
	return *((int32_t*)addressInTexture);
}