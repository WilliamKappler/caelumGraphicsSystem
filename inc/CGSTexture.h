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

#ifndef CGSTEXTURE_H
#define CGSTEXTURE_H

#include "CGS.h"
#include "CGSTextureHole.h"

// These preprocessor directives are extremely dangerous and should only be used
// on fully tested programs. Each disables a portion of type, size, or sanity
// checking within textures. It is strongly suggested you leave these commented
// unless you are fully aware of what you are doing AND have tested your program
// in-depth and received no CGS warning. However, disabling these can give a
// substantial performance boost for programs utilizing a substantial amount of
// direct texture reads and/or writes.

// Uncomment to disable boundary checks on texture access, ie, that the texel is
// allocated and within the given texture range. Also disables checks that the 
// channel ID is 4 or less. No checks are implemented for channels outside those 
// used but 4 or less (they will all point to the beginning of the texel, so 
// R or B).

//#define DISABLE_TEXTURE_TEXEL_BOUNDARY_CHECKS

// Uncomment to disable texture validity checks in address functions. This will
// cause invalid textures to return non-NULL addresses (low memory) if called on
// textures without valid internal memory. Accessing these will crash your
// program. However, it is not officially required that NULL be returned in the
// first place, and should not be depended upon; it is merely to assist debugging,
// and access functions have officially undefined behavior without valid internal
// memory to access.

//#define DISABLE_TEXTURE_ACCESS_VALIDITY_CHECKS

// Uncomment this to disable general usage warnings. These are usually not related
// to failures and typically indicate the system has auto-corrected bad user
// behavior. This feature is not considered dangerous and can reduce console
// clutter, but has little impact on performance.

//#define DISABLE_TEXTURE_USAGE_DIAGNOSTIC_WARNINGS

// END DIRECTIVES ================================================================

// Please scroll down for actual class, these defines are absurdly large.

// This enum provides usage information for CGS on how to deal with the texture.
// To an extent, this can be changed via setType( ).
//
// - VIDEO_DYNAMIC: Informs the backend that this texture is likely to change its
// size, meaning it should use allocation methods allowing this.
// - VIDEO_FIXED: Informs the backend that this texture is unlikely to change,
// meaning it can use allocation methods which do not support size changes.
// Reallocations are likely much more expensive with VIDEO_FIXED.
enum class TextureType : uint8_t
{
	VIDEO_DYNAMIC,
	VIDEO_FIXED,
	VIDEO_IMMUTABLE,
	LOCAL_ONLY
};

// Please see https://www.opengl.org/wiki/Texture#Theory for info on dimensions.
// Within CGS, array textures are treated as a texture of the next higher order.
// Meaning, a 1D texture array behaves identically to a 2D texture.
enum class TextureDimensionality : GLenum
{
	TEX_INVALID = 0,
	// Basic dimensions
	TEX_1D = GL_TEXTURE_1D,
	TEX_2D = GL_TEXTURE_2D,
	TEX_3D = GL_TEXTURE_3D
	// Special types - future
//	TEX_2D_RECT = GL_TEXTURE_RECTANGLE, // 2D interface
//	TEX_CUBE =  GL_TEXTURE_CUBE_MAP, // 3D interface, fixed to 6 z levels
//	TEX_ARRAY_1D = GL_TEXTURE_1D_ARRAY, // 2D interface
//	TEX_ARRAY_2D = GL_TEXTURE_2D_ARRAY // 3D interface
};

// Please see https://www.opengl.org/wiki/Image_Format for info on each format.
enum class TextureFormat : GLenum
{
	INVALID,
	AUTO,
	// Unsigned normalized integer 8 bit
	R8_UNORM = GL_R8,
	RG8_UNORM = GL_RG8,
	RGB8_UNORM = GL_RGB8,
	RGBA8_UNORM = GL_RGBA8,
	// Unsigned normalized integer 16 bit
	R16_UNORM = GL_R16,
	RG16_UNORM = GL_RG16,
	RGB16_UNORM = GL_RGB16,
	RGBA16_UNORM = GL_RGBA16,
	// Signed normalized integer 8 bit
	R8_SNORM = GL_R8_SNORM,
	RG8_SNORM = GL_RG8_SNORM,
	RGB8_SNORM = GL_RGB8_SNORM,
	RGBA8_SNORM = GL_RGBA8_SNORM,
	// Signed normalized integer 16 bit
	R16_SNORM = GL_R16_SNORM,
	RG16_SNORM = GL_RG16_SNORM,
	RGB16_SNORM = GL_RGB16_SNORM,
	RGBA16_SNORM = GL_RGBA16_SNORM,
	// Floating point 16 bit (half)
	R16F = GL_R16F,
	RG16F = GL_RG16F,
	RGB16F = GL_RGB16F,
	RGBA16F = GL_RGBA16F,
	// Floating point 32 bit (float)
	R32F = GL_R32F,
	RG32F = GL_RG32F,
	RGB32F = GL_RGB32F,
	RGBA32F = GL_RGBA32F,
	// Unsigned integer 8 bit (uint8_t)
	R8UI = GL_R8UI,
	RG8UI = GL_RG8UI,
	RGB8UI = GL_RGB8UI,
	RGBA8UI = GL_RGBA8UI,
	// Unsigned integer 16 bit (uint16_t)
	R16UI = GL_R16UI,
	RG16UI = GL_RG16UI,
	RGB16UI = GL_RGB16UI,
	RGBA16UI = GL_RGBA16UI,
	// Unsigned integer 32 bit (uint32_t)
	R32UI = GL_R32UI,
	RG32UI = GL_RG32UI,
	RGB32UI = GL_RGB32UI,
	RGBA32UI = GL_RGBA32UI,
	// Signed integer 8 bit (uint8_t)
	R8I = GL_R8I,
	RG8I = GL_RG8I,
	RGB8I = GL_RGB8I,
	RGBA8I = GL_RGBA8I,
	// Signed integer 16 bit (uint16_t)
	R16I = GL_R16I,
	RG16I = GL_RG16I,
	RGB16I = GL_RGB16I,
	RGBA16I = GL_RGBA16I,
	// Signed integer 32 bit (uint32_t)
	R32I = GL_R32I,
	RG32I = GL_RG32I,
	RGB32I = GL_RGB32I,
	RGBA32I = GL_RGBA32I
	// Packed formats currently unsupported, but will be added soon (R10G10B10A2)
};

class CGSTexture
{
public:
	// Appended to the texture unit to give the name for texture unit variables
	// which the texture will attempt to set. Essentially, textureUnitRange[N].
	const static char* TEXTURE_UNIFORM_PREFIX; // = "textureUnitRange"
	const static AssocArray< TextureFormat, uint8_t > TEXTURE_FORMAT_CHANNELS;
	const static AssocArray< TextureFormat, uint8_t > TEXTURE_FORMAT_CHANNEL_SIZE;
	const static AssocArray< TextureFormat, GLenum > TEXTURE_FORMAT_GL_TYPE;
	// Stores the number of dimensions in a given TextureDimensionality.
	const static AssocArray< TextureDimensionality, uint8_t > TEXTURE_DIMENSIONALITY_DIMENSIONS;
	
	// These constants are intended to help raw memory writes be independent of
	// channel ordering. If usesModifiedChannelOrdering( ) is true, use
	// TEXTURE_MODIFIED_x; otherwise, use TEXTURE_STANDARD_x.
	const static uint8_t TEXTURE_STANDARD_ORDERING_OFFSET_RED; // = 0
	const static uint8_t TEXTURE_STANDARD_ORDERING_OFFSET_GREEN; // = 1
	const static uint8_t TEXTURE_STANDARD_ORDERING_OFFSET_BLUE; // = 2
	const static uint8_t TEXTURE_STANDARD_ORDERING_OFFSET_ALPHA; // = 3
	
	const static uint8_t TEXTURE_MODIFIED_ORDERING_OFFSET_RED; // = 2
	const static uint8_t TEXTURE_MODIFIED_ORDERING_OFFSET_GREEN; // = 1
	const static uint8_t TEXTURE_MODIFIED_ORDERING_OFFSET_BLUE; // = 0
	const static uint8_t TEXTURE_MODIFIED_ORDERING_OFFSET_ALPHA; // = 3
	
	CGSTexture( );
	virtual ~CGSTexture( ) = 0;
	
	// Will mark this texture for deletion upon having no users (ie, mesh
	// attachments). Users of the texture should cease to hold pointers to this
	// object after calling markForDeletion( ).
	inline void markForDeletion( ) { deleteWhenUnused = true; }
	
	// Return the number of objects using this object.
	uint32_t getUsers( );
	
	// Call to invoke an update during the next render. Must be called if you
	// edit the texture via its raw pointer via an access( ) function or
	// from memoryStart( ). Not required if editing via CGSTextureHole.
	inline void setModified( ) { modified = true; }
	
	// Uploads texture to video card if needed.
	virtual void _update( ) = 0;
	
	TextureDimensionality getDimensionality( ) { return dimensionality; }
	GLuint getTextureHandle( ) { return textureHandle; }
	
	// Returns the number of texels in each dimension. The minimum is 1, which
	// corresponds to dimensions the texture does not actually have (except X,
	// which always has to exist).
	// 
	// This includes the pseudo-dimension in texture arrays. Each layer in the
	// array is represented by the next order dimension; for 1D arrays, y, and
	// 2D arrays, z.
	inline const uint16_t& getTexelCountX( ) { return xTexels; }
	inline const uint16_t& getTexelCountY( ) { return yTexels; }
	inline const uint16_t& getTexelCountZ( ) { return zTexels; }
	
	// These return the size, in bytes, of the each part of the texture. These
	// functions are intended primarily for memory operations.
	//
	// This must be made very clear: these are not merely sums. For example,
	// getRowSize() is not merely getCellSize() * getTexelCountX(). The row and
	// sheet functions will include unused space within the memory the texture.
	// This is required for the memory to align with where it is expected to be.
	//
	// Note: for packed formats, channelSize == cellSize.
	inline const uint8_t& getChannelSize( ) { return channelSize; }
	inline const uint8_t& getCellSize( ) { return cellSize; }
	inline const uint32_t& getRowSize( ) { return rowSize; }
	inline const uint32_t& getSheetSize( ) { return sheetSize; }
	
	// Returns the current internal channel ordering.
	//
	// OpenGL requests that some specific formats use BGRA instead of RGBA. CGS
	// abstracts this so that if you are not using raw memory-copy operations, you
	// do not need to concern yourself with the internal ordering. However, raw
	// memory pointers are also provided for some uses, and you should check
	// channel ordering before using these, to ensure you are not confused by
	// the potential for, as an example, channel 2 being lower in memory than
	// channel 0.
	//
	// False indicates that RGB[A] ordering is used.
	// True indicates BGR[A] ordering is used.
	//
	// As far as I am aware, only 8 bit unsigned normalized uses BGR[A] ordering,
	// but this should still be checked for any non-packed format if using direct
	// memory access, as no guarantees of order are provided beyond it matching
	// this function's value.
	//
	// Textures determine which ordering to use by calling 
	// GraphicsSystem::_useModifiedChannelOrdering( TextureFormat ), but calling
	// this function is both safer and faster, since the ordering is cached in the
	// individual textures.
	inline bool usesModifiedChannelOrdering( ) { return useModifiedChannelOrdering; }
	
	// These functions only change the size of the existing texture, and cannot
	// change the pixel format or dimensionality. All existing data in the texture
	// will be lost.
	//
	// Note that a texture will maintain the largest internal memory it has been
	// allocated if it is allowed to over-allocate, which includes previous sizes
	// from resize( ), unless cropMemory( ) is explicitly called, or resize( ) is
	// called with allocation parameters. Because this behavior is IMPLICIT, if
	// your shader(s) do not support over-allocated textures, be sure to disable
	// overallocation as a whole, or texture addressing might be affected.
	//
	// See the CGS documentation for more information on how to properly use
	// overallocation. http://onathacar.fredlawl.com/index.php/Textures
	// 
	// If any of the _dTexelsAlloc variables are set to a value, reallocation will
	// be triggered regardless of if the value is larger or smaller than the
	// current allocated memory. This is NOT the same as "reserve" in STD
	// functions, as that will not eliminate existing allocated memory. These
	// WILL.
	virtual void resize(
		const uint16_t& _x,
		const uint16_t& _y = 1,
		const uint16_t& _z = 1,
		const uint16_t& _xTexelsAlloc = 0,
		const uint16_t& _yTexelsAlloc = 0,
		const uint16_t& _zTexelsAlloc = 0 ) = 0;
	
	// Access texture hole at (x, y, z, channel) given. The given address
	// must be less than the size in each dimension, and are zero-indexed. If a
	// texture does not possess a dimension, it should be given 0.
	// 
	// OpenGL samples from the bottom-left to the top-right. This is somewhat
	// annoying because other systems (including OGUI and DirectX) sample from
	// top-left to bottom-right. I suggest you just flip UVs; overall, it's the
	// most simple solution to what is otherwise a very irritating problem. This
	// just means you should make every v texture coordinate 1 - v. If using 
	// integer coordinates, use height - v. If you do this, (0, 0, z, c) is the
	// top-left.
	//
	// The FreeImage loader function assumes you are going to use reversed y UVs.
	// 
	// Single-component textures and textures using a packed format must always
	// be given a 0 for channel.
	// 
	// For unpacked textures, each channel number will always refer to the
	// following channel, no matter the internal format:
	// 0 - Red
	// 1 - Green
	// 2 - Blue
	// 3 - Alpha
	virtual CGSTextureHole hole(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel ) = 0;
	
	// Alternative to hole( ). These return a raw pointer to the location
	// requested, not a TextureHole. This is useful for memory copy operations
	// and is used internally to set up TextureHoles for the hole() functions. You
	// are expected to cast the returned pointer to the correct type before
	// reading and writing - uint8_t* is merely a placeholder type (like void*),
	// although it does happen to match the type of the most common format.
	//
	// Because of issues with OpenGL expecting various different channel orderings
	// for different format/platform combinations, you cannot assume that channel
	// 0 is the first. Channel 0 is always RED, but it is not always the first
	// channel within the texel. Some formats are requested to use BGRA, which
	// would mean 0 is actually the third component in memory. To help work around
	// this unusual behavior, CGS provides a pseudo-channel numbered 4, which
	// will always point to the first channel of the texel in memory. If you copy
	// an entire texel manually, you should request channel 4.
	//
	// If you would like to check the channel ordering, which is strongly
	// suggested for most real-world uses, call getChannelOrdering( ).
	//
	// Note that if you write to memory returned from this function, you must call
	// setModified( ) to trigger the texture to be uploaded to the video card.
	// There is no way to tell if you used the pointer to write or merely read,
	// ergo it is up to you to ensure the system is informed of what you did.
	virtual uint8_t* access(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel ) = 0;
	
	// This will return a raw pointer to the internal memory of this texture.
	// While this is useful for doing memory copies from other libraries, it
	// is dangerous in most contexts due to support for over-allocating texture
	// memory, which will cause simple memory copy operations to misalign.
	// 
	// If you choose to write directly with this function, use getRowSize( ) and
	// getSheetSize( ) for row and sheet offsets, in order to account for
	// over-allocation, which might result in these being larger than the mere
	// summation of texels.
	//
	// The same channel numbering warnings apply as with access( ).
	// 
	// As with access( ), you must setModified( ) if you change the texture.
	//
	// Also, please be aware that using this means you have ensured the data is in
	// the format this texture is specified to accept. That means
	// non-premultiplied alpha matching the internal channel ordering obtain from
	// getChannelOrdering( ). Many libraries use different formats and if you
	// are accepting textures from such a library (like Pango-Cairo, which uses
	// premult-ARGB), you must convert and swizzle on your own. CGSTexture's
	// backend does not do this by design. Helper functions for other formats are
	// provided in CGSTypeConversions.h.
	//
	// IMO (Kappler), using texture memory directly is more trouble than it is
	// worth in almost every case I can imagine. Especially with checks disabled,
	// the CGSTextureHole-based access is almost no overhead for a much
	// simpler interface.
	inline uint8_t* memoryStart( ) { return textureStartAddress; }
	
	// CGS INTERNAL CALLS ========================================================
	void _attachToMesh( CGSMeshToTextureAdapter* const& _connector );
	void _detachFromMesh( CGSMeshToTextureAdapter* const& _connector );
	void _bind( const uint8_t& _unit );
	
protected:
	uint8_t* textureStartAddress;
	
	// Channel offsets. These are essentially textureStartAddress plus the
	// distance to a given channel. These are stored partially to avoid
	// conditional forks in the address functions and partially to abstract the
	// actual offsets, since these can differ (RGBA vs. BGRA).
	//
	// Note that there are _5_ elements, and there are 5 no matter how many
	// channels there are. Unused channels, including channel ID 4, are always
	// equal to textureStartAddress. Since there is no channel ID 4 (0-3 are
	// real channels), ID 4 is always the beginning of texel memory, no matter
	// the channel ordering. This is by design and can be utilized.
	uint8_t* offsetTextureAddressTable[5];
	
	// If true, this texture uses BGR[A]. If false, it uses the standard RGBA.
	bool useModifiedChannelOrdering;
	
	// The data format for this texture. It will be uploaded to OpenGL verbatim
	// to avoid costly repacking (TextureFormat is typed to GLenum and its values
	// alias valid OpenGL formats).
	TextureFormat pixelFormat;
	
	TextureDimensionality dimensionality;
	
	// Current handle of the texture this object represents. Mutable. 0 before
	// allocation.
	GLuint textureHandle;
	
	bool staticTexture; // Texture is read-only
	bool initialUpload; // True upon construction, false after upload
	bool deleteWhenUnused;
	bool allowOverAllocation;
	bool modified;
	bool storageModified;
	
	uint8_t numMipMaps;
	
	uint8_t channelsPerTexel;
	uint16_t xTexels;
	uint16_t yTexels;
	uint16_t zTexels;
	
	// These store the allocated texels in the texture, no matter the current real
	// size. If these are 0, then the size is equal to [dim]Texels.
	uint16_t xTexelsAlloc;
	uint16_t yTexelsAlloc;
	uint16_t zTexelsAlloc;
	
	// Each of these are in bytes and (re)calculated when the pixel format and
	// dimensions are set.
	uint8_t channelSize; // Size of each texture channel, as addressed above.
	uint8_t cellSize; // Size of each texel/pixel, the sum of channels.
	uint32_t rowSize; // The sum of cells in a row, including unused cells.
	uint32_t sheetSize; // The sum of rows in a sheet, including unused cells.
	
	// Stores links to meshes.
	UnorderedSet< CGSMeshToTextureAdapter* > meshAdapters;
	
	// These are the back-end functions for obtaining a channel address, used
	// for hole() and access(). 
	//
	// Because channel orders can change for some formats, channel 0 might not
	// be the beginning of memory. As such, the pseudo-channel 4 is provided,
	// which ALWAYS points to the first element.
	inline uint8_t* address3D(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel );
	
	inline uint8_t* address2D(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint8_t& _channel );
	
	inline uint8_t* address1D(
		const uint16_t& _x,
		const uint8_t& _channel );
	
	// Create the range vector
	vec3 _generateTextureUsedRange( );
	
	// Function that actually sets the program uniforms from the mesh.
	void _setProgramUniforms(
		CGSMeshToTextureAdapter* const& _connector,
		const vec3& range );
	
	// Function to call _setProgramUniforms( ) for each attached adapter.
	void _updateAllAdapterUniforms( const vec3& range );
	
	// This is the back-end to all allocation operations, including the
	// constructor. It calls no virtual functions and as such is safe to use
	// in constructors and destructors (why you would want to allocate in a
	// destructor is unknown to me).
	//
	// The function creates a NEW texture in OpenGL and does not write any
	// member variables. This is to allow copies from the existing memory as
	// necessary.
	bool _allocateTexture(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint16_t& _xTexels,
		const uint16_t& _yTexels,
		const uint16_t& _zTexels,
		const bool& _allowOverAlloc,
		const uint16_t& _xTexelsAlloc,
		const uint16_t& _yTexelsAlloc,
		const uint16_t& _zTexelsAlloc,
		// Written to external variables
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
		// Final texture data
		uint8_t* & new_textureStartAddress,
		GLuint& new_textureHandle );
	
	// Writes the new texture values to the old and cleans up the existing
	// texture data if necessary. Primarily a convenience function, since this is
	// done in several different places and is fairly long.
	void _swapTextureData(
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
		uint8_t* const& new_textureStartAddress,
		const GLuint& new_textureHandle );
	
	// Construct the final texture lookup table. Uses the current texture
	// format and textureStartAddress. As such, should always come after
	// _swapTextureData( ).
	void _constructTextureLookupTable( );
	
	// Returns a warning if this is a static texture that has been modified.
	void _checkStatic( );
	
	// This is a hack to deal with an unfortunate implication of how the hole( )
	// functions operate. Specifically, there is no option to NOT return one, and
	// for the sake of speed, they perform no checks on the validity of their
	// internal pointer.
	//
	// As such, I need somewhere I can direct holes if I cannot return a proper
	// one, that will not cause a segfault. That is where this variable comes in:
	// it is effectively /dev/null for texture holes.
	//
	// By definition, anything actually using this is undefined behavior.
	uint64_t invalidTextureHoleLocation;
};

class CGSTexture1D : public CGSTexture
{
public:
	CGSTexture1D(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint8_t& _numMipMaps,
		const uint16_t& _xTexels,
		const bool& _allowOverAlloc = false,
		const uint16_t& _xTexelsAlloc = 0,
		const bool& _staticTexture = false );
	
	// Base class implementations
	void _update( );

	void resize(
		const uint16_t& _x,
		const uint16_t& _y = 1,
		const uint16_t& _z = 1,
		const uint16_t& _xTexelsAlloc = 0,
		const uint16_t& _yTexelsAlloc = 0,
		const uint16_t& _zTexelsAlloc = 0 );

	CGSTextureHole hole(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel );

	uint8_t* access(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel );
	
	// No special destructor for dimensional textures
	~CGSTexture1D( ) {};
};

class CGSTexture2D : public CGSTexture
{
public:
	CGSTexture2D(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint8_t& _numMipMaps,
		const uint16_t& _xTexels,
		const uint16_t& _yTexels,
		const bool& _allowOverAlloc = false,
		const uint16_t& _xTexelsAlloc = 0,
		const uint16_t& _yTexelsAlloc = 0,
		const bool& _staticTexture = false );
	
	
	// Base class implementations
	void _update( );

	void resize(
		const uint16_t& _x,
		const uint16_t& _y = 1,
		const uint16_t& _z = 1,
		const uint16_t& _xTexelsAlloc = 0,
		const uint16_t& _yTexelsAlloc = 0,
		const uint16_t& _zTexelsAlloc = 0 );

	CGSTextureHole hole(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel );

	uint8_t* access(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel );
	
	~CGSTexture2D( ) {};
};

class CGSTexture3D : public CGSTexture
{
public:
	CGSTexture3D(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint8_t& _numMipMaps,
		const uint16_t& _xTexels,
		const uint16_t& _yTexels,
		const uint16_t& _zTexels,
		const bool& _allowOverAlloc = false,
		const uint16_t& _xTexelsAlloc = 0,
		const uint16_t& _yTexelsAlloc = 0,
		const uint16_t& _zTexelsAlloc = 0,
		const bool& _staticTexture = false );
	
	// Base class implementations
	void _update( );

	void resize(
		const uint16_t& _x,
		const uint16_t& _y = 1,
		const uint16_t& _z = 1,
		const uint16_t& _xTexelsAlloc = 0,
		const uint16_t& _yTexelsAlloc = 0,
		const uint16_t& _zTexelsAlloc = 0 );

	CGSTextureHole hole(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel );

	uint8_t* access(
		const uint16_t& _x,
		const uint16_t& _y,
		const uint16_t& _z,
		const uint8_t& _channel );
	
	~CGSTexture3D( ) {};
};

#endif	/* CGSTEXTURE_H */

