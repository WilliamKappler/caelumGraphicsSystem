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

#ifndef CGSTEXTUREHOLE_H
#define	CGSTEXTUREHOLE_H

#include "CGS.h"
#include "CGSVectors.h"

// This class represents the smallest addressable component of a texture and acts
// to smooth over reading and writing to texture components.
//
// Instances of this class SHOULD NOT BE STORED. Referential classes can become
// undefined due to action at a distance if they are stored. You should, as a user
// of CGS, never even declare an object of this type.
//
// The read/write functions check only byte size. THEY DO NOT CHECK TYPE. As such,
// nothing will prevent you from calling write32UI( ) on a hole which contains
// 32F data. This can be useful for writing to normalized integer types, but
// will also allow you to unknowingly corrupt textures if you are not careful.
//
// Notes on types:
// * UNORM and SNORM: These are used to format and write floating points to these
// types. The texture should have the _UNORM or _SNORM type, respectively. Integer
// values can be written into UNORM and SNORM types (such as common 0-255 color
// values) using the UI and I types with the same size. Do not allow ints to be
// implicitly cast to floats by calling UNORM or SNORM with them, as they will be
// 'rounded' to the discrete values of [-1,] 0, or 1.
// 
// * 16F: Since half floats are not supported in C++ (at least on any arch I know
// of), this type takes and returns a float. As such, you must be aware of the
// substantial loss of precision when using 16F. There will be no warning at
// compile time like with other precision losses.

// Uncomment the following line to disable byte length checks when reading from
// and writing to textures. This of course means you will be able to use incorrect
// functions without any warning or indication your read/write went to the wrong
// place. As such, this should only be used if you are sure everything that deals
// with textures works correctly - ie, only after substantial testing.

//#define DISABLE_TEXTURE_HOLE_SIZE_CHECKS

class CGSTextureHole
{
public:
#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
	CGSTextureHole(
			uint8_t* const& address,
			const uint8_t& _channelSize,
			bool* _textureModifiedFlag );
#else
	CGSTextureHole(
			uint8_t* const& address,
			bool* _textureModifiedFlag );
#endif
	
	// If you are using this, it is probably more efficient to call the access()
	// function of the associated texture. This is here for simplicity, but
	// creating a texture hole has a fair amount of overhead that can be avoided
	// by jusing access().
	inline uint8_t* address( ) { return addressInTexture; }
	
	void write16F( const float& d );
	float read16F( );
	void write32F( const float& d );
	float read32F( );

	void write8_UNORM( const float& d );
	float read8_UNORM( );
	void write16_UNORM( const float& d );
	float read16_UNORM( );

	void write8_SNORM( const float& d );
	float read8_SNORM( );
	void write16_SNORM( const float& d );
	float read16_SNORM( );

	void write8UI( const uint8_t& d );
	uint8_t read8UI( );
	void write16UI( const uint16_t& d );
	uint16_t read16UI( );
	void write32UI( const uint32_t& d );
	uint32_t read32UI( );
	
	void write8I( const int8_t& d );
	int8_t read8I( );
	void write16I( const int16_t& d );
	int16_t read16I( );
	void write32I( const int32_t& d );
	int32_t read32I( );
	
protected:
	uint8_t* addressInTexture;
	bool* textureModifiedFlag;
	
#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
	uint8_t channelSize;
#endif
};

#endif	/* CGSTEXTUREHOLE_H */

