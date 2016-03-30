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

#include "CGSTexture.h"
#include "CGS.h"
#include "CGSMesh.h"
#include "CGSMeshToTextureAdapter.h"

AssocArray< TextureFormat, uint8_t > _init_TEXTURE_FORMAT_CHANNELS( )
{
	AssocArray< TextureFormat, uint8_t > r;

	// 1 component
	r[ TextureFormat::R8_UNORM ] = 1;
	r[ TextureFormat::R16_UNORM ] = 1;
	r[ TextureFormat::R8_SNORM ] = 1;
	r[ TextureFormat::R16_SNORM ] = 1;
	r[ TextureFormat::R16F ] = 1;
	r[ TextureFormat::R32F ] = 1;
	r[ TextureFormat::R8UI ] = 1;
	r[ TextureFormat::R16UI ] = 1;
	r[ TextureFormat::R32UI ] = 1;
	r[ TextureFormat::R8I ] = 1;
	r[ TextureFormat::R16I ] = 1;
	r[ TextureFormat::R32I ] = 1;

	// 2 component
	r[ TextureFormat::RG8_UNORM ] = 2;
	r[ TextureFormat::RG16_UNORM ] = 2;
	r[ TextureFormat::RG8_SNORM ] = 2;
	r[ TextureFormat::RG16_SNORM ] = 2;
	r[ TextureFormat::RG16F ] = 2;
	r[ TextureFormat::RG32F ] = 2;
	r[ TextureFormat::RG8UI ] = 2;
	r[ TextureFormat::RG16UI ] = 2;
	r[ TextureFormat::RG32UI ] = 2;
	r[ TextureFormat::RG8I ] = 2;
	r[ TextureFormat::RG16I ] = 2;
	r[ TextureFormat::RG32I ] = 2;

	// 3 component
	r[ TextureFormat::RGB8_UNORM ] = 3;
	r[ TextureFormat::RGB16_UNORM ] = 3;
	r[ TextureFormat::RGB8_SNORM ] = 3;
	r[ TextureFormat::RGB16_SNORM ] = 3;
	r[ TextureFormat::RGB16F ] = 3;
	r[ TextureFormat::RGB32F ] = 3;
	r[ TextureFormat::RGB8UI ] = 3;
	r[ TextureFormat::RGB16UI ] = 3;
	r[ TextureFormat::RGB32UI ] = 3;
	r[ TextureFormat::RGB8I ] = 3;
	r[ TextureFormat::RGB16I ] = 3;
	r[ TextureFormat::RGB32I ] = 3;

	// 4 component
	r[ TextureFormat::RGBA8_UNORM ] = 4;
	r[ TextureFormat::RGBA16_UNORM ] = 4;
	r[ TextureFormat::RGBA8_SNORM ] = 4;
	r[ TextureFormat::RGBA16_SNORM ] = 4;
	r[ TextureFormat::RGBA16F ] = 4;
	r[ TextureFormat::RGBA32F ] = 4;
	r[ TextureFormat::RGBA8UI ] = 4;
	r[ TextureFormat::RGBA16UI ] = 4;
	r[ TextureFormat::RGBA32UI ] = 4;
	r[ TextureFormat::RGBA8I ] = 4;
	r[ TextureFormat::RGBA16I ] = 4;
	r[ TextureFormat::RGBA32I ] = 4;

	return r;
}

AssocArray< TextureFormat, uint8_t > _init_TEXTURE_FORMAT_CHANNEL_SIZE( )
{
	AssocArray< TextureFormat, uint8_t > r;

	// 8 bit per channel
	r[ TextureFormat::R8_UNORM ] = 1;
	r[ TextureFormat::RG8_UNORM ] = 1;
	r[ TextureFormat::RGB8_UNORM ] = 1;
	r[ TextureFormat::RGBA8_UNORM ] = 1;
	r[ TextureFormat::R8_SNORM ] = 1;
	r[ TextureFormat::RG8_SNORM ] = 1;
	r[ TextureFormat::RGB8_SNORM ] = 1;
	r[ TextureFormat::RGBA8_SNORM ] = 1;
	r[ TextureFormat::R8UI ] = 1;
	r[ TextureFormat::RG8UI ] = 1;
	r[ TextureFormat::RGB8UI ] = 1;
	r[ TextureFormat::RGBA8UI ] = 1;
	r[ TextureFormat::R8I ] = 1;
	r[ TextureFormat::RG8I ] = 1;
	r[ TextureFormat::RGB8I ] = 1;
	r[ TextureFormat::RGBA8I ] = 1;

	// 16 bit per channel
	r[ TextureFormat::R16_UNORM ] = 2;
	r[ TextureFormat::RG16_UNORM ] = 2;
	r[ TextureFormat::RGB16_UNORM ] = 2;
	r[ TextureFormat::RGBA16_UNORM ] = 2;
	r[ TextureFormat::R16_SNORM ] = 2;
	r[ TextureFormat::RG16_SNORM ] = 2;
	r[ TextureFormat::RGB16_SNORM ] = 2;
	r[ TextureFormat::RGBA16_SNORM ] = 2;
	r[ TextureFormat::R16F ] = 2;
	r[ TextureFormat::RG16F ] = 2;
	r[ TextureFormat::RGB16F ] = 2;
	r[ TextureFormat::RGBA16F ] = 2;
	r[ TextureFormat::R16UI ] = 2;
	r[ TextureFormat::RG16UI ] = 2;
	r[ TextureFormat::RGB16UI ] = 2;
	r[ TextureFormat::RGBA16UI ] = 2;
	r[ TextureFormat::R16I ] = 2;
	r[ TextureFormat::RG16I ] = 2;
	r[ TextureFormat::RGB16I ] = 2;
	r[ TextureFormat::RGBA16I ] = 2;

	// 32 bit per channel
	r[ TextureFormat::R32F ] = 4;
	r[ TextureFormat::RG32F ] = 4;
	r[ TextureFormat::RGB32F ] = 4;
	r[ TextureFormat::RGBA32F ] = 4;
	r[ TextureFormat::R32UI ] = 4;
	r[ TextureFormat::RG32UI ] = 4;
	r[ TextureFormat::RGB32UI ] = 4;
	r[ TextureFormat::RGBA32UI ] = 4;
	r[ TextureFormat::R32I ] = 4;
	r[ TextureFormat::RG32I ] = 4;
	r[ TextureFormat::RGB32I ] = 4;
	r[ TextureFormat::RGBA32I ] = 4;

	return r;
}

AssocArray< TextureFormat, GLenum > _init_TEXTURE_FORMAT_GL_TYPE( )
{
	AssocArray< TextureFormat, GLenum > r;

	// Unsigned byte
	r[ TextureFormat::R8_UNORM ] = GL_UNSIGNED_BYTE;
	r[ TextureFormat::RG8_UNORM ] = GL_UNSIGNED_BYTE;
	r[ TextureFormat::RGB8_UNORM ] = GL_UNSIGNED_BYTE;
	r[ TextureFormat::RGBA8_UNORM ] = GL_UNSIGNED_BYTE;
	r[ TextureFormat::R8I ] = GL_UNSIGNED_BYTE;
	r[ TextureFormat::RG8I ] = GL_UNSIGNED_BYTE;
	r[ TextureFormat::RGB8I ] = GL_UNSIGNED_BYTE;
	r[ TextureFormat::RGBA8I ] = GL_UNSIGNED_BYTE;

	// Signed byte
	r[ TextureFormat::R8_SNORM ] = GL_BYTE;
	r[ TextureFormat::RG8_SNORM ] = GL_BYTE;
	r[ TextureFormat::RGB8_SNORM ] = GL_BYTE;
	r[ TextureFormat::RGBA8_SNORM ] = GL_BYTE;
	r[ TextureFormat::R8UI ] = GL_BYTE;
	r[ TextureFormat::RG8UI ] = GL_BYTE;
	r[ TextureFormat::RGB8UI ] = GL_BYTE;
	r[ TextureFormat::RGBA8UI ] = GL_BYTE;

	// Unsigned short (16 bit)
	r[ TextureFormat::R16_UNORM ] = GL_UNSIGNED_SHORT;
	r[ TextureFormat::RG16_UNORM ] = GL_UNSIGNED_SHORT;
	r[ TextureFormat::RGB16_UNORM ] = GL_UNSIGNED_SHORT;
	r[ TextureFormat::RGBA16_UNORM ] = GL_UNSIGNED_SHORT;
	r[ TextureFormat::R16UI ] = GL_UNSIGNED_SHORT;
	r[ TextureFormat::RG16UI ] = GL_UNSIGNED_SHORT;
	r[ TextureFormat::RGB16UI ] = GL_UNSIGNED_SHORT;
	r[ TextureFormat::RGBA16UI ] = GL_UNSIGNED_SHORT;

	// Signed short (16 bit)
	r[ TextureFormat::R16_SNORM ] = GL_SHORT;
	r[ TextureFormat::RG16_SNORM ] = GL_SHORT;
	r[ TextureFormat::RGB16_SNORM ] = GL_SHORT;
	r[ TextureFormat::RGBA16_SNORM ] = GL_SHORT;
	r[ TextureFormat::R16I ] = GL_SHORT;
	r[ TextureFormat::RG16I ] = GL_SHORT;
	r[ TextureFormat::RGB16I ] = GL_SHORT;
	r[ TextureFormat::RGBA16I ] = GL_SHORT;

	// Half float
	r[ TextureFormat::R16F ] = GL_HALF_FLOAT;
	r[ TextureFormat::RG16F ] = GL_HALF_FLOAT;
	r[ TextureFormat::RGB16F ] = GL_HALF_FLOAT;
	r[ TextureFormat::RGBA16F ] = GL_HALF_FLOAT;

	// Unsigned int (32 bit)
	r[ TextureFormat::R32UI ] = GL_UNSIGNED_INT;
	r[ TextureFormat::RG32UI ] = GL_UNSIGNED_INT;
	r[ TextureFormat::RGB32UI ] = GL_UNSIGNED_INT;
	r[ TextureFormat::RGBA32UI ] = GL_UNSIGNED_INT;

	// Signed int (32 bit)
	r[ TextureFormat::R32I ] = GL_INT;
	r[ TextureFormat::RG32I ] = GL_INT;
	r[ TextureFormat::RGB32I ] = GL_INT;
	r[ TextureFormat::RGBA32I ] = GL_INT;

	// Float
	r[ TextureFormat::R32F ] = GL_FLOAT;
	r[ TextureFormat::RG32F ] = GL_FLOAT;
	r[ TextureFormat::RGB32F ] = GL_FLOAT;
	r[ TextureFormat::RGBA32F ] = GL_FLOAT;

	return r;
}

AssocArray< TextureDimensionality, uint8_t > _init_TEXTURE_DIMENSIONALITY_DIMENSIONS( )
{
	AssocArray< TextureDimensionality, uint8_t > r;

	// Basic
	r[ TextureDimensionality::TEX_1D ] = 1;
	r[ TextureDimensionality::TEX_2D ] = 2;
	r[ TextureDimensionality::TEX_3D ] = 3;

	// Advanced
	/*
	r[ TextureDimensionality::TEX_2D_RECT ] = 2;
	r[ TextureDimensionality::TEX_CUBE ] = 3;
	r[ TextureDimensionality::TEX_ARRAY_1D ] = 2;
	r[ TextureDimensionality::TEX_ARRAY_2D ] = 3;
	 */

	return r;
}

const char* CGSTexture::TEXTURE_UNIFORM_PREFIX = "textureUnitRange";
const AssocArray< TextureFormat, uint8_t > CGSTexture::TEXTURE_FORMAT_CHANNELS
		= _init_TEXTURE_FORMAT_CHANNELS( );
const AssocArray< TextureFormat, uint8_t > CGSTexture::TEXTURE_FORMAT_CHANNEL_SIZE
		= _init_TEXTURE_FORMAT_CHANNEL_SIZE( );
const AssocArray< TextureFormat, GLenum > CGSTexture::TEXTURE_FORMAT_GL_TYPE
		= _init_TEXTURE_FORMAT_GL_TYPE( );
const AssocArray< TextureDimensionality, uint8_t > CGSTexture::TEXTURE_DIMENSIONALITY_DIMENSIONS
		= _init_TEXTURE_DIMENSIONALITY_DIMENSIONS( );

const uint8_t CGSTexture::TEXTURE_STANDARD_ORDERING_OFFSET_RED = 0;
const uint8_t CGSTexture::TEXTURE_STANDARD_ORDERING_OFFSET_GREEN = 1;
const uint8_t CGSTexture::TEXTURE_STANDARD_ORDERING_OFFSET_BLUE = 2;
const uint8_t CGSTexture::TEXTURE_STANDARD_ORDERING_OFFSET_ALPHA = 3;

const uint8_t CGSTexture::TEXTURE_MODIFIED_ORDERING_OFFSET_RED = 2;
const uint8_t CGSTexture::TEXTURE_MODIFIED_ORDERING_OFFSET_GREEN = 1;
const uint8_t CGSTexture::TEXTURE_MODIFIED_ORDERING_OFFSET_BLUE = 0;
const uint8_t CGSTexture::TEXTURE_MODIFIED_ORDERING_OFFSET_ALPHA = 3;

CGSTexture::CGSTexture( )
{
	textureStartAddress = NULL;
	memset( offsetTextureAddressTable, 0, sizeof( offsetTextureAddressTable ) );

	textureHandle = 0;

	staticTexture = false;
	initialUpload = true;
	deleteWhenUnused = false;
	modified = true;
	storageModified = true;

	allowOverAllocation = false;
	pixelFormat = TextureFormat::INVALID;
	dimensionality = TextureDimensionality::TEX_INVALID;
	numMipMaps = 0;
	channelsPerTexel = 0;
	xTexels = 0;
	yTexels = 0;
	zTexels = 0;
	xTexelsAlloc = 0;
	yTexelsAlloc = 0;
	zTexelsAlloc = 0;
	channelSize = 0;
	cellSize = 0;
	rowSize = 0;
	sheetSize = 0;
}

CGSTexture::~CGSTexture( )
{
	for( auto i = meshAdapters.begin( ); i != meshAdapters.end( ); ++i )
	{
		(*i)->_invalidate( );
	}

	if( textureHandle )
		glDeleteTextures( 1, &textureHandle );

	if( textureStartAddress )
		delete[] textureStartAddress;
}

uint32_t CGSTexture::getUsers( )
{
	return meshAdapters.size( );
}

bool CGSTexture::_allocateTexture(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint16_t& _xTexels,
		const uint16_t& _yTexels,
		const uint16_t& _zTexels,
		const bool& _allowOverAlloc,
		const uint16_t& _xTexelsAlloc,
		const uint16_t& _yTexelsAlloc,
		const uint16_t& _zTexelsAlloc,
		uint8_t& new_channelsPerTexel,
		uint16_t& new_xTexels,
		uint16_t& new_yTexels,
		uint16_t& new_zTexels,
		uint16_t& new_xTexelsAlloc,
		uint16_t& new_yTexelsAlloc,
		uint16_t& new_zTexelsAlloc,
		uint8_t& new_channelSize,
		uint8_t& new_cellSize,
		uint32_t& new_rowSize,
		uint32_t& new_sheetSize,
		uint8_t* & new_textureStartAddress,
		GLuint& new_textureHandle )
{
	// This function implementation assumes that when called (it is pure virtual,
	// so child classes must implement it) that the values of _xTexels, _yTexels,
	// and _zTexels make sense for the dimensionality of the texture.
	size_t bytesToAllocate;

	// Find/calculate sizes per channel and texel/cell
	new_channelsPerTexel = TEXTURE_FORMAT_CHANNELS.find( _format )->second;
	new_channelSize = TEXTURE_FORMAT_CHANNEL_SIZE.find( _format )->second;
	new_cellSize = new_channelsPerTexel * new_channelSize;

	// Calculate row size
	if( _allowOverAlloc && _xTexelsAlloc >= _xTexels )
	{
		new_xTexels = _xTexels;
		if( _xTexelsAlloc == _xTexels )
		{
			new_xTexelsAlloc = 0;
		}
		else
		{
			new_xTexelsAlloc = _xTexelsAlloc;
		}
		new_rowSize = new_cellSize * _xTexelsAlloc;
	}
	else
	{
		if( _allowOverAlloc && _xTexelsAlloc && _xTexelsAlloc < _xTexels )
		{
			U::log( "Warning: Texture over-allocation requested, but allocation size in X dimension smaller than requested size." );
			U::log( " - Allocate X: ", _xTexelsAlloc );
			U::log( " - Texture X size: ", _xTexels );
			U::log( " - Requested texture X used for allocation." );
		}

		new_xTexels = _xTexels;
		new_xTexelsAlloc = 0;
		new_rowSize = new_cellSize * _xTexels;
	}

	// Ensure row size is multiple of 4.
	// See https://www.opengl.org/wiki/Pixel_Transfer#Pixel_layout
	new_rowSize += new_rowSize % 4;

	// Calculate sheet size
	if( _allowOverAlloc && _yTexelsAlloc >= _yTexels )
	{
		new_yTexels = _yTexels;
		if( _yTexelsAlloc == _yTexels )
		{
			new_yTexelsAlloc = 0;
		}
		else
		{
			new_yTexelsAlloc = _yTexelsAlloc;
		}
		new_sheetSize = new_rowSize * _yTexelsAlloc;
	}
	else
	{
		if( _allowOverAlloc && _yTexelsAlloc && _yTexelsAlloc < _zTexels )
		{
			U::log( "Warning: Texture over-allocation requested, but allocation size in Y dimension smaller than requested size." );
			U::log( " - Allocate Y: ", _yTexelsAlloc );
			U::log( " - Texture Y size: ", _yTexels );
			U::log( " - Requested texture Y used for allocation." );
		}

		new_yTexels = _yTexels;
		new_yTexelsAlloc = 0;
		new_sheetSize = new_rowSize * _yTexels;
	}

	// Calculate overall texture size
	if( _allowOverAlloc && _zTexelsAlloc >= _zTexels )
	{
		new_zTexels = _zTexels;
		if( _zTexelsAlloc == _zTexels )
		{
			new_zTexelsAlloc = 0;
		}
		else
		{
			new_zTexelsAlloc = _zTexelsAlloc;
		}
		bytesToAllocate = new_sheetSize * _zTexelsAlloc;
	}
	else
	{
		if( _allowOverAlloc && _zTexelsAlloc && _zTexelsAlloc < _zTexels )
		{
			U::log( "Warning: Texture over-allocation requested, but allocation size in Z dimension smaller than requested size." );
			U::log( " - Allocate Z: ", _zTexelsAlloc );
			U::log( " - Texture Z size: ", _zTexels );
			U::log( " - Requested texture Z used for allocation." );
		}

		new_zTexels = _zTexels;
		new_zTexelsAlloc = 0;
		bytesToAllocate = new_sheetSize * _zTexels;
	}

	// Allocate memory
	new_textureStartAddress = new uint8_t[bytesToAllocate];

	// Zero it, as CGS specifies it will be zero'd
	memset( new_textureStartAddress, 0, bytesToAllocate );

	// Get a new texture handle
	glGenTextures( 1, &new_textureHandle );

	// Texture storage is expected to be set up by the CALLING IMPLEMENTATION of
	// this function, since it varies with dimensionality.
	return true;
}

void CGSTexture::_swapTextureData(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint8_t& _numMipMaps,
		const bool& _allowOverAlloc,
		const uint8_t& new_channelsPerTexel,
		const uint16_t& new_xTexels,
		const uint16_t& new_yTexels,
		const uint16_t& new_zTexels,
		const uint16_t& new_xTexelsAlloc,
		const uint16_t& new_yTexelsAlloc,
		const uint16_t& new_zTexelsAlloc,
		const uint8_t& new_channelSize,
		const uint8_t& new_cellSize,
		const uint32_t& new_rowSize,
		const uint32_t& new_sheetSize,
		uint8_t * const& new_textureStartAddress,
		const GLuint& new_textureHandle )
{
	if( textureHandle != new_textureHandle )
	{
		storageModified = true;
	}
	
	// Delete assets if necessary
	if( textureStartAddress )
	{
		delete[] textureStartAddress;
	}

	if( textureHandle )
	{
		glDeleteTextures( 1, &textureHandle );
	}

	// Set the new values
	textureStartAddress = new_textureStartAddress;
	textureHandle = new_textureHandle;
	allowOverAllocation = _allowOverAlloc;
	pixelFormat = _format;
	dimensionality = _dimensionality;
	numMipMaps = _numMipMaps;
	channelsPerTexel = new_channelsPerTexel;
	xTexels = new_xTexels;
	yTexels = new_yTexels;
	zTexels = new_zTexels;
	xTexelsAlloc = new_xTexelsAlloc;
	yTexelsAlloc = new_yTexelsAlloc;
	zTexelsAlloc = new_zTexelsAlloc;
	channelSize = new_channelSize;
	cellSize = new_cellSize;
	rowSize = new_rowSize;
	sheetSize = new_sheetSize;

	// Use RGBA or BGRA?
	useModifiedChannelOrdering = GraphicsSystem::_useModifiedChannelOrdering( _format );

	// Trigger upload and uniform updates
	modified = true;
}

void CGSTexture::_constructTextureLookupTable( )
{
	// Build channel offset array

	// First, set all to the start address. More straight-forward this way.
	offsetTextureAddressTable[ 0 ] = textureStartAddress;
	offsetTextureAddressTable[ 1 ] = textureStartAddress;
	offsetTextureAddressTable[ 2 ] = textureStartAddress;
	offsetTextureAddressTable[ 3 ] = textureStartAddress;
	offsetTextureAddressTable[ 4 ] = textureStartAddress;

	// Only 3 and 4 component formats can be affected by modified ordering. Use
	// that to our advantage to make this simpler.
	if( channelsPerTexel >= 3 && useModifiedChannelOrdering )
	{
		offsetTextureAddressTable[ 0 ] = textureStartAddress + channelSize * 2;
		offsetTextureAddressTable[ 1 ] = textureStartAddress + channelSize;
		// offsetTextureAddressTable[ 2 ] = textureStartAddress; from above

		// Alpha always last
		if( channelsPerTexel == 4 )
		{
			offsetTextureAddressTable[ 3 ] = textureStartAddress + channelSize * 3;
		}
	}
	else
	{
		// Skips 0, which is already textureStartAddress
		// This will also make alpha = textureStartAddress + channelSize * 3
		for( int i = 1; i < channelsPerTexel; ++i )
		{
			offsetTextureAddressTable[ i ] = textureStartAddress + channelSize * i;
		}
	}
}

void CGSTexture::_checkStatic( )
{
	if( staticTexture && !initialUpload )
	{
		U::log( "Warning: A texture marked as static has been written to." );
	}
	
	initialUpload = false;
}

// LINKAGE FUNCTIONS =============================================================

void CGSTexture::_attachToMesh( CGSMeshToTextureAdapter * const& _connector )
{
	meshAdapters.insert( _connector );
}

void CGSTexture::_detachFromMesh( CGSMeshToTextureAdapter * const& _connector )
{
	meshAdapters.erase( _connector );
	_connector->_invalidate( );
	
	if( !deleteWhenUnused )
	{
		return;
	}
	
	if( !getUsers( ) )
	{
		GraphicsSystem::getGlobalInstance( )->_textureRequestDeletion( this );
	}
}

void CGSTexture::_bind( const uint8_t& _unit )
{
	glActiveTexture( GL_TEXTURE0 + _unit );
	glBindTexture( ( GLenum )dimensionality, textureHandle );
}

vec3 CGSTexture::_generateTextureUsedRange( )
{
	if( !allowOverAllocation )
	{
		return vec3( 1.0, 1.0, 1.0 );
	}
	
	vec3 range;
	
	if( xTexelsAlloc && xTexels < xTexelsAlloc )
	{
		range.x( ) = (float)xTexels / xTexelsAlloc;
	}
	else
	{
		range.x( ) = 1.0;
	}
	
	if( yTexelsAlloc && yTexels < yTexelsAlloc )
	{
		range.y( ) = (float)yTexels / yTexelsAlloc;
	}
	else
	{
		range.y( ) = 1.0;
	}
	
	if( zTexelsAlloc && zTexels < zTexelsAlloc )
	{
		range.z( ) = (float)zTexels / zTexelsAlloc;
	}
	else
	{
		range.z( ) = 1.0;
	}
	
	return range;
}

void CGSTexture::_setProgramUniforms(
		CGSMeshToTextureAdapter * const& _connector,
		const vec3& range )
{	
	GLint uniformLocation = glGetUniformLocation(
			_connector->getProgramHandle( ),
			U::c( TEXTURE_UNIFORM_PREFIX, (int)_connector->getTextureUnit( ) ).c_str( ) );

	glProgramUniform3fv( _connector->getProgramHandle( ), uniformLocation, 1, range.d );
}

void CGSTexture::_updateAllAdapterUniforms( const vec3& range )
{
	for( auto i = meshAdapters.begin( ); i != meshAdapters.end( ); ++i )
	{
		_setProgramUniforms( (*i), range );
	}
}

// INTERNAL ADDRESS FUNCTIONS ====================================================

uint8_t* CGSTexture::address3D(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel )
{
	#ifndef DISABLE_TEXTURE_ACCESS_VALIDITY_CHECKS
	if( !textureStartAddress )
	{
		U::log( "Error: Invalid texture accessed." );
		return NULL;
	}
	#endif
	#ifndef DISABLE_TEXTURE_CHANNEL_RANGE_CHECKS
	if(
			_x >= xTexels
			|| _y >= yTexels
			|| _z >= zTexels
			|| _channel > 4 )
	{
		U::log( "Error: Texture access out of range." );
		U::log( " - X coord: ", _x, " ( ", xTexels, " in texture )" );
		U::log( " - Y coord: ", _y, " ( ", yTexels, " in texture )" );
		U::log( " - Z coord: ", _z, " ( ", zTexels, " in texture )" );
		U::log( " - Channel ID: ", (int)_channel );
		return NULL;
	}
	#endif

	return offsetTextureAddressTable[ _channel ]
			+ cellSize * _x
			+ rowSize * _y
			+ sheetSize * _z;
}

uint8_t* CGSTexture::address2D(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint8_t& _channel )
{
	#ifndef DISABLE_TEXTURE_ACCESS_VALIDITY_CHECKS
	if( !textureStartAddress )
	{
		U::log( "Error: Invalid texture accessed." );
		return NULL;
	}
	#endif
	#ifndef DISABLE_TEXTURE_CHANNEL_RANGE_CHECKS
	if(
			_x >= xTexels
			|| _y >= yTexels
			|| _channel > 4 )
	{
		U::log( "Error: Texture access out of range." );
		U::log( " - X coord: ", _x, " ( ", xTexels, " in texture )" );
		U::log( " - Y coord: ", _y, " ( ", yTexels, " in texture )" );
		U::log( " - Channel ID: ", (int)_channel );
		return NULL;
	}
	#endif

	return offsetTextureAddressTable[ _channel ]
			+ cellSize * _x
			+ rowSize * _y;
}

uint8_t* CGSTexture::address1D(
		const uint16_t& _x,
		const uint8_t& _channel )
{
	#ifndef DISABLE_TEXTURE_ACCESS_VALIDITY_CHECKS
	if( !textureStartAddress )
	{
		U::log( "Error: Invalid texture accessed." );
		return NULL;
	}
	#endif
	#ifndef DISABLE_TEXTURE_CHANNEL_RANGE_CHECKS
	if(
			_x >= xTexels
			|| _channel > 4 )
	{
		U::log( "Error: Texture access out of range." );
		U::log( " - X coord: ", _x, " ( ", xTexels, " in texture )" );
		U::log( " - Channel ID: ", (int)_channel );
		return NULL;
	}
	#endif

	return offsetTextureAddressTable[ _channel ]
			+ cellSize * _x;
}

// RESIZE FUNCTIONS ==============================================================

void CGSTexture::resize(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint16_t& _xTexelsAlloc,
		const uint16_t& _yTexelsAlloc,
		const uint16_t& _zTexelsAlloc )
{
	// This assumes, like the individual allocation process functions, that the
	// values passed to this function are appropriate for the given texture.
	
	// Cannot resize static textures.
	if( staticTexture )
	{
		U::log( "Error: Invalid call to resize( ) on a static (read-only) texture ignored." );
		return;
	}
	
	// This conditional decides if the existing texture memory can be used or not.
	if( 
			// No over-allocation, new memory is needed, no matter what
			allowOverAllocation
			// Enough memory in each dimension allocated?
			&& ( _x <= xTexelsAlloc || _x <= xTexels )
			&& ( _y <= yTexelsAlloc || _y <= yTexels )
			&& ( _z <= zTexelsAlloc || _z <= zTexels )
			// Is explicit reallocation needed?
			&& ( !_xTexelsAlloc || _xTexelsAlloc == xTexelsAlloc )
			&& ( !_yTexelsAlloc || _yTexelsAlloc == yTexelsAlloc )
			&& ( !_zTexelsAlloc || _zTexelsAlloc == zTexelsAlloc ) )
	{
		memset( textureStartAddress, 0, sheetSize * _z );
		
		// X memory
		if( xTexelsAlloc || xTexels > 1 )
		{
			xTexelsAlloc = U::max( xTexelsAlloc, xTexels );
		}
		else
		{
			xTexelsAlloc = 0;
		}
		
		// Y memory
		if( yTexelsAlloc || yTexels > 1 )
		{
			yTexelsAlloc = U::max( yTexelsAlloc, yTexels );
		}
		else
		{
			yTexelsAlloc = 0;
		}
		
		// Z memory
		if( zTexelsAlloc || zTexels > 1 )
		{
			zTexelsAlloc = U::max( zTexelsAlloc, zTexels );
		}
		else
		{
			zTexelsAlloc = 0;
		}
		
		xTexels = _x;
		yTexels = _y;
		zTexels = _z;
		
		return;
	}
	
	// New variables
	uint8_t new_channelsPerTexel;
	uint16_t new_xTexels;
	uint16_t new_yTexels;
	uint16_t new_zTexels;
	uint16_t new_xTexelsAlloc;
	uint16_t new_yTexelsAlloc;
	uint16_t new_zTexelsAlloc;
	uint8_t new_channelSize;
	uint8_t new_cellSize;
	uint32_t new_rowSize;
	uint32_t new_sheetSize;
	uint8_t* new_textureStartAddress;
	GLuint new_textureHandle;

	// This is the initial allocation, so no copy is necessary.
	_allocateTexture(
			dimensionality, pixelFormat,
			_x, _y, _z, // XYZ texels
			allowOverAllocation,
			_xTexelsAlloc, _yTexelsAlloc, _zTexelsAlloc, // XYZ alloc
			new_channelsPerTexel,
			new_xTexels, new_yTexels, new_zTexels,
			new_xTexelsAlloc, new_yTexelsAlloc, new_zTexelsAlloc,
			new_channelSize,
			new_cellSize,
			new_rowSize,
			new_sheetSize,
			new_textureStartAddress,
			new_textureHandle );

	_swapTextureData(
			dimensionality,
			pixelFormat,
			numMipMaps,
			allowOverAllocation,
			new_channelsPerTexel,
			new_xTexels,
			new_yTexels,
			new_zTexels,
			new_xTexelsAlloc,
			new_yTexelsAlloc,
			new_zTexelsAlloc,
			new_channelSize,
			new_cellSize,
			new_rowSize,
			new_sheetSize,
			new_textureStartAddress,
			new_textureHandle );
	
	_constructTextureLookupTable( );
}

// TEXTURE HOLE FUNCTIONS ========================================================

CGSTextureHole CGSTexture::hole(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel )
{
	#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
	return CGSTextureHole( address3D( _x, _y, _z, _channel ), channelSize, &modified );
	#else
	return CGSTextureHole( address3D( _x, _y, _z, _channel ), &modified );
	#endif
}

// 1D TEXTURE ====================================================================

CGSTexture1D::CGSTexture1D(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint8_t& _numMipMaps,
		const uint16_t& _xTexels,
		const bool& _allowOverAlloc,
		const uint16_t& _xTexelsAlloc,
		const bool& _staticTexture )
{
	staticTexture = _staticTexture;
	
	if( TEXTURE_DIMENSIONALITY_DIMENSIONS.find( _dimensionality )->second != 1 )
	{
		U::log( "FATAL ERROR: CGSTexture1D constructed with incorrect dimensionality." );
		throw;
	}
	
	// New variables
	uint8_t new_channelsPerTexel;
	uint16_t new_xTexels;
	uint16_t new_yTexels;
	uint16_t new_zTexels;
	uint16_t new_xTexelsAlloc;
	uint16_t new_yTexelsAlloc;
	uint16_t new_zTexelsAlloc;
	uint8_t new_channelSize;
	uint8_t new_cellSize;
	uint32_t new_rowSize;
	uint32_t new_sheetSize;
	uint8_t* new_textureStartAddress;
	GLuint new_textureHandle;

	// This is the initial allocation, so no copy is necessary.
	_allocateTexture(
			_dimensionality, _format,
			_xTexels, 1, 1, // XYZ texels
			_allowOverAlloc,
			_xTexelsAlloc, 0, 0, // XYZ alloc
			new_channelsPerTexel,
			new_xTexels, new_yTexels, new_zTexels,
			new_xTexelsAlloc, new_yTexelsAlloc, new_zTexelsAlloc,
			new_channelSize,
			new_cellSize,
			new_rowSize,
			new_sheetSize,
			new_textureStartAddress,
			new_textureHandle );

	_swapTextureData(
			_dimensionality,
			_format,
			_numMipMaps,
			_allowOverAlloc,
			new_channelsPerTexel,
			new_xTexels,
			new_yTexels,
			new_zTexels,
			new_xTexelsAlloc,
			new_yTexelsAlloc,
			new_zTexelsAlloc,
			new_channelSize,
			new_cellSize,
			new_rowSize,
			new_sheetSize,
			new_textureStartAddress,
			new_textureHandle );

	_constructTextureLookupTable( );
}

void CGSTexture1D::_update( )
{
	if( !modified || !textureStartAddress )
	{
		return;
	}

	_checkStatic( );

	// Upload range data if needed
	_updateAllAdapterUniforms( _generateTextureUsedRange( ) );
	
	GLenum textureGLType = TEXTURE_FORMAT_GL_TYPE.find( pixelFormat )->second;
	GLenum textureGLFormat = GraphicsSystem::_getGLPreferredFormat( pixelFormat );

	// Not sure how to orphan textures; glInvalidateTexImage( ) only invalidates 1
	// mipmap. As such, this orphaning will likely only work with no-mipmap images
	// such as UI elements. This should probably be improved in the future.
	if( !storageModified )
	{
		glInvalidateTexImage( textureHandle, 0 );
	}

	glBindTexture( ( GLenum )dimensionality, textureHandle );
	
	if( storageModified )
	{
		if( allowOverAllocation )
		{
			glTexStorage1D(
					( GLenum )dimensionality,
					numMipMaps + 1,
					( GLenum )pixelFormat,
					U::max( xTexels, xTexelsAlloc ) );
		}
		else
		{
			glTexStorage1D(
					( GLenum )dimensionality,
					numMipMaps + 1,
					( GLenum )pixelFormat,
					xTexels );
		}
		
		storageModified = false;
	}
	
	if( allowOverAllocation )
	{
		glTexSubImage1D(
				( GLenum )dimensionality,
				0, // Mipmap 0, the parent image
				0, // Offset
				U::max( xTexels, xTexelsAlloc ),
				textureGLFormat,
				textureGLType,
				textureStartAddress );
	}
	else
	{
		glTexSubImage1D(
				( GLenum )dimensionality,
				0, // Mipmap 0, the parent image
				0, // Offset
				xTexels,
				textureGLFormat,
				textureGLType,
				textureStartAddress );
	}
	
	// Set the sampling parameters. Currently not user-set, but will be.
	glTexParameteri( ( GLenum )dimensionality, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	
	glGenerateMipmap( ( GLenum )dimensionality );
	modified = false;
}

void CGSTexture1D::resize(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint16_t& _xTexelsAlloc,
		const uint16_t& _yTexelsAlloc,
		const uint16_t& _zTexelsAlloc )
{
	CGSTexture::resize( _x, 1, 1, _xTexelsAlloc, 0, 0 );
}

CGSTextureHole CGSTexture1D::hole(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel )
{
	#ifndef DISABLE_TEXTURE_TEXEL_BOUNDARY_CHECKS
	if( _x >= xTexels )
	{
		U::log( "Error: X coordinate out of range in 1D texture, CGSTextureHole to dead memory returned." );
		U::log( " - X coordinate: ", _x );
		U::log( " - Number of X texels: ", xTexels );

		#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, channelSize, &modified );
		#else
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, &modified );
		#endif
	}
	if( _y != 0 )
	{
		U::log( "Warning: Non-zero Y coordinate used with 1D texture. Ignored." );
	}
	if( _z != 0 )
	{
		U::log( "Warning: Non-zero Z coordinate used with 1D texture. Ignored." );
	}
	if( _channel >= 5 )
	{
		U::log( "Error: Channel ID given is out of bounds for any possible texture (greater than 4). CGSTextureHole to dead memory returned." );
		U::log( " - Channel ID: ", (int)_channel );
		
		#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, channelSize, &modified );
		#else
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, &modified );
		#endif
	}
	if( _channel >= channelsPerTexel && _channel != 4 )
	{
		U::log( "Warning: Channel ID given exceeds number of channels in texture. First channel in memory returned." );
		U::log( " - Channel ID: ", (int)_channel );
		U::log( " - Number of channels: ", channelsPerTexel );
	}
	#endif

	#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
	return CGSTextureHole( address1D( _x, _channel ), channelSize, &modified );
	#else
	return CGSTextureHole( address1D( _x, _channel ), &modified );
	#endif
}

uint8_t* CGSTexture1D::access(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel )
{
	return address1D( _x, _channel );
}

// 2D TEXTURE ====================================================================
CGSTexture2D::CGSTexture2D(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint8_t& _numMipMaps,
		const uint16_t& _xTexels,
		const uint16_t& _yTexels,
		const bool& _allowOverAlloc,
		const uint16_t& _xTexelsAlloc,
		const uint16_t& _yTexelsAlloc,
		const bool& _staticTexture )
{
	staticTexture = _staticTexture;
	
	if( TEXTURE_DIMENSIONALITY_DIMENSIONS.find( _dimensionality )->second != 2 )
	{
		U::log( "FATAL ERROR: CGSTexture2D constructed with incorrect dimensionality." );
		throw;
	}
	
	uint8_t new_channelsPerTexel;
	uint16_t new_xTexels;
	uint16_t new_yTexels;
	uint16_t new_zTexels;
	uint16_t new_xTexelsAlloc;
	uint16_t new_yTexelsAlloc;
	uint16_t new_zTexelsAlloc;
	uint8_t new_channelSize;
	uint8_t new_cellSize;
	uint32_t new_rowSize;
	uint32_t new_sheetSize;
	uint8_t* new_textureStartAddress;
	GLuint new_textureHandle;

	_allocateTexture(
			_dimensionality, _format,
			_xTexels, _yTexels, 1, // XYZ texels
			_allowOverAlloc,
			_xTexelsAlloc, _yTexelsAlloc, 0, // XYZ alloc
			new_channelsPerTexel,
			new_xTexels, new_yTexels, new_zTexels,
			new_xTexelsAlloc, new_yTexelsAlloc, new_zTexelsAlloc,
			new_channelSize,
			new_cellSize,
			new_rowSize,
			new_sheetSize,
			new_textureStartAddress,
			new_textureHandle );

	_swapTextureData(
			_dimensionality,
			_format,
			_numMipMaps,
			_allowOverAlloc,
			new_channelsPerTexel,
			new_xTexels,
			new_yTexels,
			new_zTexels,
			new_xTexelsAlloc,
			new_yTexelsAlloc,
			new_zTexelsAlloc,
			new_channelSize,
			new_cellSize,
			new_rowSize,
			new_sheetSize,
			new_textureStartAddress,
			new_textureHandle );

	_constructTextureLookupTable( );
}

void CGSTexture2D::_update( )
{
	if( !modified || !textureStartAddress )
	{
		return;
	}

	_checkStatic( );
	
	// Upload range data if needed
	_updateAllAdapterUniforms( _generateTextureUsedRange( ) );

	GLenum textureGLType = TEXTURE_FORMAT_GL_TYPE.find( pixelFormat )->second;
	GLenum textureGLFormat = GraphicsSystem::_getGLPreferredFormat( pixelFormat );

	// Not sure how to orphan textures; glInvalidateTexImage( ) only invalidates 1
	// mipmap. As such, this orphaning will likely only work with no-mipmap images
	// such as UI elements. This should probably be improved in the future.
	if( !storageModified )
	{
		glInvalidateTexImage( textureHandle, 0 );
	}

	glBindTexture( ( GLenum )dimensionality, textureHandle );
	
	if( storageModified )
	{
		if( allowOverAllocation )
		{
			glTexStorage2D(
					( GLenum )dimensionality,
					numMipMaps + 1,
					( GLenum )pixelFormat,
					U::max( xTexels, xTexelsAlloc ),
					U::max( yTexels, yTexelsAlloc ) );
		}
		else
		{
			glTexStorage2D(
					( GLenum )dimensionality,
					numMipMaps + 1,
					( GLenum )pixelFormat,
					xTexels,
					yTexels );
		}
		
		storageModified = false;
	}
	
	if( allowOverAllocation )
	{
		glTexSubImage2D(
				( GLenum )dimensionality,
				0, // Mipmap 0, the parent image
				0, 0, // Offset
				U::max( xTexels, xTexelsAlloc ),
				U::max( yTexels, yTexelsAlloc ),
				textureGLFormat,
				textureGLType,
				textureStartAddress );
	}
	else
	{
		glTexSubImage2D(
				( GLenum )dimensionality,
				0, // Mipmap 0, the parent image
				0, 0, // Offset
				xTexels, yTexels, // Size
				textureGLFormat,
				textureGLType,
				textureStartAddress );
	}
	
	// Set the sampling parameters. Currently not user-set, but will be.
	glTexParameteri( ( GLenum )dimensionality, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( ( GLenum )dimensionality, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	glGenerateMipmap( ( GLenum )dimensionality );
	modified = false;
}

void CGSTexture2D::resize(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint16_t& _xTexelsAlloc,
		const uint16_t& _yTexelsAlloc,
		const uint16_t& _zTexelsAlloc )
{
	CGSTexture::resize( _x, _y, 1, _xTexelsAlloc, _yTexelsAlloc, 0 );
}

CGSTextureHole CGSTexture2D::hole(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel )
{
	#ifndef DISABLE_TEXTURE_TEXEL_BOUNDARY_CHECKS
	if( _x >= xTexels )
	{
		U::log( "Error: X coordinate out of range in 2D texture, CGSTextureHole to dead memory returned." );
		U::log( " - X coordinate: ", _x );
		U::log( " - Number of X texels: ", xTexels );

		#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, channelSize, &modified );
		#else
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, &modified );
		#endif
	}
	if( _y >= yTexels )
	{
		U::log( "Error: Y coordinate out of range in 2D texture, CGSTextureHole to dead memory returned." );
		U::log( " - Y coordinate: ", _y );
		U::log( " - Number of Y texels: ", yTexels );

		#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, channelSize, &modified );
		#else
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, &modified );
		#endif
	}
	if( _z != 0 )
	{
		U::log( "Warning: Non-zero Z coordinate used with 2D texture. Ignored." );
	}
	if( _channel >= 5 )
	{
		U::log( "Error: Channel ID given is out of bounds for any possible texture (greater than 4). CGSTextureHole to dead memory returned." );
		U::log( " - Channel ID: ", (int)_channel );
		
		#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, channelSize, &modified );
		#else
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, &modified );
		#endif
	}
	if( _channel >= channelsPerTexel && _channel != 4 )
	{
		U::log( "Warning: Channel ID given exceeds number of channels in texture. First channel in memory returned." );
		U::log( " - Channel ID: ", (int)_channel );
		U::log( " - Number of channels: ", channelsPerTexel );
	}
	#endif

	#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
	return CGSTextureHole( address2D( _x, _y, _channel ), channelSize, &modified );
	#else
	return CGSTextureHole( address2D( _x, _y, _channel ), &modified );
	#endif
}

uint8_t* CGSTexture2D::access(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel )
{
	return address2D( _x, _y, _channel );
}

// 3D TEXTURE ====================================================================
CGSTexture3D::CGSTexture3D(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint8_t& _numMipMaps,
		const uint16_t& _xTexels,
		const uint16_t& _yTexels,
		const uint16_t& _zTexels,
		const bool& _allowOverAlloc,
		const uint16_t& _xTexelsAlloc,
		const uint16_t& _yTexelsAlloc,
		const uint16_t& _zTexelsAlloc,
		const bool& _staticTexture )
{
	staticTexture = _staticTexture;
	
	if( TEXTURE_DIMENSIONALITY_DIMENSIONS.find( _dimensionality )->second != 3 )
	{
		U::log( "FATAL ERROR: CGSTexture3D constructed with incorrect dimensionality." );
		throw;
	}
	
	uint8_t new_channelsPerTexel;
	uint16_t new_xTexels;
	uint16_t new_yTexels;
	uint16_t new_zTexels;
	uint16_t new_xTexelsAlloc;
	uint16_t new_yTexelsAlloc;
	uint16_t new_zTexelsAlloc;
	uint8_t new_channelSize;
	uint8_t new_cellSize;
	uint32_t new_rowSize;
	uint32_t new_sheetSize;
	uint8_t* new_textureStartAddress;
	GLuint new_textureHandle;

	_allocateTexture(
			_dimensionality, _format,
			_xTexels, _yTexels, _zTexels,
			_allowOverAlloc,
			_xTexelsAlloc, _yTexelsAlloc, _zTexelsAlloc,
			new_channelsPerTexel,
			new_xTexels, new_yTexels, new_zTexels,
			new_xTexelsAlloc, new_yTexelsAlloc, new_zTexelsAlloc,
			new_channelSize,
			new_cellSize,
			new_rowSize,
			new_sheetSize,
			new_textureStartAddress,
			new_textureHandle );

	_swapTextureData(
			_dimensionality,
			_format,
			_numMipMaps,
			_allowOverAlloc,
			new_channelsPerTexel,
			new_xTexels,
			new_yTexels,
			new_zTexels,
			new_xTexelsAlloc,
			new_yTexelsAlloc,
			new_zTexelsAlloc,
			new_channelSize,
			new_cellSize,
			new_rowSize,
			new_sheetSize,
			new_textureStartAddress,
			new_textureHandle );

	_constructTextureLookupTable( );
}

void CGSTexture3D::_update( )
{
	if( !modified || !textureStartAddress )
	{
		return;
	}

	_checkStatic( );

	// Upload range data if needed
	_updateAllAdapterUniforms( _generateTextureUsedRange( ) );

	GLenum textureGLType = TEXTURE_FORMAT_GL_TYPE.find( pixelFormat )->second;
	GLenum textureGLFormat = GraphicsSystem::_getGLPreferredFormat( pixelFormat );

	// Not sure how to orphan textures; glInvalidateTexImage( ) only invalidates 1
	// mipmap. As such, this orphaning will likely only work with no-mipmap images
	// such as UI elements. This should probably be improved in the future.
	if( !storageModified )
	{
		glInvalidateTexImage( textureHandle, 0 );
	}

	glBindTexture( ( GLenum )dimensionality, textureHandle );
	
	if( storageModified )
	{
		if( allowOverAllocation )
		{
			glTexStorage3D(
					( GLenum )dimensionality,
					numMipMaps + 1,
					( GLenum )pixelFormat,
					U::max( xTexels, xTexelsAlloc ),
					U::max( yTexels, yTexelsAlloc ),
					U::max( zTexels, zTexelsAlloc ) );
		}
		else
		{
			glTexStorage3D(
					( GLenum )dimensionality,
					numMipMaps + 1,
					( GLenum )pixelFormat,
					xTexels,
					yTexels,
					zTexels );
		}
		
		storageModified = false;
	}
	
	if( allowOverAllocation )
	{
		glTexSubImage3D(
				( GLenum )dimensionality,
				0, // Mipmap 0, the parent image
				0, 0, 0, // Offset
				U::max( xTexels, xTexelsAlloc ),
				U::max( yTexels, yTexelsAlloc ),
				U::max( zTexels, zTexelsAlloc ),
				textureGLFormat,
				textureGLType,
				textureStartAddress );
	}
	else
	{
		glTexSubImage3D(
				( GLenum )dimensionality,
				0, // Mipmap 0, the parent image
				0, 0, 0, // Offset
				xTexels, yTexels, zTexels,
				textureGLFormat,
				textureGLType,
				textureStartAddress );
	}
	
	// Set the sampling parameters. Currently not user-set, but will be.
	glTexParameteri( ( GLenum )dimensionality, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( ( GLenum )dimensionality, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( ( GLenum )dimensionality, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	
	glGenerateMipmap( ( GLenum )dimensionality );
	modified = false;
}

void CGSTexture3D::resize(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint16_t& _xTexelsAlloc,
		const uint16_t& _yTexelsAlloc,
		const uint16_t& _zTexelsAlloc )
{
	CGSTexture::resize( _x, _y, _z, _xTexelsAlloc, _yTexelsAlloc, _zTexelsAlloc );
}

CGSTextureHole CGSTexture3D::hole(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel )
{
	#ifndef DISABLE_TEXTURE_TEXEL_BOUNDARY_CHECKS
	if( _x >= xTexels )
	{
		U::log( "Error: X coordinate out of range in 3D texture, CGSTextureHole to dead memory returned." );
		U::log( " - X coordinate: ", _x );
		U::log( " - Number of X texels: ", xTexels );

		#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, channelSize, &modified );
		#else
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, &modified );
		#endif
	}
	if( _y >= yTexels )
	{
		U::log( "Error: Y coordinate out of range in 3D texture, CGSTextureHole to dead memory returned." );
		U::log( " - Y coordinate: ", _y );
		U::log( " - Number of Y texels: ", yTexels );

		#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, channelSize, &modified );
		#else
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, &modified );
		#endif
	}
	if( _z >= zTexels )
	{
		U::log( "Error: Z coordinate out of range in 3D texture, CGSTextureHole to dead memory returned." );
		U::log( " - Z coordinate: ", _z );
		U::log( " - Number of Z texels: ", zTexels );

		#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, channelSize, &modified );
		#else
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, &modified );
		#endif
	}
	if( _channel >= 5 )
	{
		U::log( "Error: Channel ID given is out of bounds for any possible texture (greater than 4). CGSTextureHole to dead memory returned." );
		U::log( " - Channel ID: ", (int)_channel );
		
		#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, channelSize, &modified );
		#else
		return CGSTextureHole( (uint8_t*)&invalidTextureHoleLocation, &modified );
		#endif
	}
	if( _channel >= channelsPerTexel && _channel != 4 )
	{
		U::log( "Warning: Channel ID given exceeds number of channels in texture. First channel in memory returned." );
		U::log( " - Channel ID: ", (int)_channel );
		U::log( " - Number of channels: ", channelsPerTexel );
	}
	#endif

	#ifndef DISABLE_TEXTURE_HOLE_SIZE_CHECKS
	return CGSTextureHole( address3D( _x, _y, _z, _channel ), channelSize, &modified );
	#else
	return CGSTextureHole( address3D( _x, _y, _z, _channel ), &modified );
	#endif
}

uint8_t* CGSTexture3D::access(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel )
{
	return address3D( _x, _y, _z, _channel );
}