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

// TODO list:
// * Resizable windows (by C++ command)
// * Resizable windows (by user corner dragging)
// * Window size change listeners (ie, OGUI)
// * Fullscreen+borderless support (SDL_WindowFlags)
// * Enable on/off vsync (SDL_GL_SetSwapInterval)
// * Loading mesh files (custom file format?)

#ifndef CGS_H
#define CGS_H

#ifdef _WIN32
// Must come before glew
#include <windef.h>

// Must come before OpenGL (SDL)
#include "glew.h"
//#include "glxew.h"
//#include "wglew.h"

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_syswm.h"
#else
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "SDL2/SDL_syswm.h"
#endif
#endif
// Linux support needs added.

#include "FreeImage.h"

#include "EngineTypes.h"
#include "Utility.h"
#include "CGSVectors.h"

// Forward declarations of graphics classes
class CGSShader;
class CGSMesh;
class CGSTexture;
class CGSStandardTexture;
class CGSRectangleTexture;
class CGSMeshToTextureAdapter;
enum class TextureType : uint8_t;
enum class TextureDimensionality : GLenum;
enum class TextureFormat : GLenum;

#ifndef NDEBUG
# define CGS_NDEBUG
#endif

// This is typed and assigned to be compatible with OpenGL's GLSL functions.
enum class ShaderType : GLenum
{
	VERTEX = GL_VERTEX_SHADER,
	TESSELLATION_CONTROL = GL_TESS_CONTROL_SHADER,
	TESSELLATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER
};

// This can be changed to other types (like int, unsigned int...) if your specific
// use needs/benefits from a different type for ordering render operations.
// Float, however, has the nice effect of allowing huge ranges as well as being
// directly relatable to depth.
typedef float CGSRenderOrderingType;

struct CGSRenderOperation
{
	CGSMesh* mesh;
	bool pullBackbuffer;
	
	CGSRenderOperation(
			CGSMesh* const& _mesh,
			const bool& _pullBackbuffer = false )
			: mesh( _mesh ), pullBackbuffer( _pullBackbuffer ) {};
};

class CGSRenderStage
{
public:
	const static uint8_t DEFAULT_BACK_BUFFER_TEXTURE_UNIT; // = 7
	
	// backbufferTextureUnit is which unit the backbuffer texture will be bound
	// to. CGS assumes you do not bind any textures in this render stage to that
	// unit. This is not checked against currently; if you do bind to the unit,
	// you will cause undefined behavior.
	CGSRenderStage( const uint8_t& _backbufferTextureUnit = DEFAULT_BACK_BUFFER_TEXTURE_UNIT )
			: backbufferTextureUnit( _backbufferTextureUnit ) {};
	
	// Add a render operation with the given ordering to the render stage.
	// 
	// If _pullBackbuffer is true, rendering will be suspended and the current
	// render target is copied to the backbuffer. This is very performance intense
	// and so should be limited; it is, however, necessary for non-blending
	// transparency (such as refraction). The back-buffer is automatically copied
	// at the transition between each render stage. It is worth noting that the
	// backbuffer is available regardless of if _pullBackbuffer is set; what
	// the flag does is ensure the backbuffer is up-to-date. As such, some users
	// might be able to exploit knowledge of scene layering to reduce pulls, if it
	// is known that objects utilizing the backbuffer do not overlap meaningfully.
	//
	// According to my tests, _pullBackbuffer is not actually that costly on
	// modern hardware, but be aware it COULD be on a given system.
	void insertOperation(
			const CGSRenderOrderingType& _position,
			CGSMesh* const& _mesh,
			const bool& _pullBackbuffer = false );
	
	// Render cycle internal functions.
	inline uint8_t _getBackbufferTextureUnit( ) { return backbufferTextureUnit; }
	
	inline AssocArrayMulti< CGSRenderOrderingType, CGSRenderOperation >& _getOperations( )
	{
		return operations;
	}
	
protected:
	AssocArrayMulti< CGSRenderOrderingType, CGSRenderOperation > operations;
	uint8_t backbufferTextureUnit;
};

class GraphicsSystem
{
public:
	// GLSL files will be accepted in the format:
	// [name].GLSL_*_EXTENSION.GLSL_FINAL_EXTENSION
	// For example, a vertex shader called "grass" will be searched for as:
	// grass.vert.glsl
	// You should ensure that only one file with such a name is within the shader
	// search paths; duplicate file names will cause undefined behavior, as any of
	// them could be loaded.
	const static char* GLSL_FINAL_EXTENSION; // = "glsl"
	const static char* GLSL_VERTEX_EXTENSION; // = "vert"
	const static char* GLSL_FRAGMENT_EXTENSION; // = "frag"
	const static char* GLSL_GEOMETRY_EXTENSION; // = "geom"
	const static char* GLSL_TESSELLATION_CONTROL_EXTENSION; // = "tctl"
	const static char* GLSL_TESSELLATION_EVALUATION_EXTENSION; // = "tevl"
	const static char* GLSL_INCLUDE_EXTENSION; // = "inc"
	const static char* DEFAULT_SHADER_NAME; // = "default"
	const static char* DEFAULT_SHADER_PATH; // = "glsl/"
	
	GraphicsSystem( const bool& _disableDebug = false );
	~GraphicsSystem( );

	bool init(
		const uint16_t& x,
		const uint16_t& y,
		const String& windowName,
		const String& _shaderPath = DEFAULT_SHADER_PATH );
	
	inline static GraphicsSystem* const& getGlobalInstance( )
	{
		return globalInstance;
	}
	
	inline const uint16_t& getScreenX( ) const { return screenX; }
	inline const uint16_t& getScreenY( ) const { return screenY; }
	
	// If set to true, will print extra debug information to the console.
	void setDebugMode( const bool& _mode );
	inline const bool& getDebugMode( ) { return debugMode; }
	
	// RENDER SYSTEM FUNCTIONS ===================================================
	// Functions in this section are defined in CGSRenderQueue.cpp, not CGS.cpp.
	
	// Generates a mesh set up specifically for compositing; effectively just a
	// mesh which occupies the entire screen.
	CGSMesh* createCompositorMesh( const String& fragmentShader = "" );
	
	// Render queue stages. They are rendered in the order of the index they are
	// given, unless that ordering is explicitly overridden.
	CGSRenderStage* getRenderStage( const uint8_t& index );
	bool removeRenderStage( const uint8_t& index );
	
	// Renders all render stages together, from the lowest index to the highest.
	// Each subsequent stage will have access to the cumulative backbuffer of the
	// previous stages. When complete, the render target is written to the default
	// framebuffer (ie, it is shown).
	void render( );
	
	// Behaves like render( ), except that stages are ordered according to the
	// indexes in the array passed. Passing { 0, 1, 0, 2 } will render stage 0,
	// then 1, then 0 again, then 2. As with the raw render( ) function, each 
	// stage has access to the cumulative backbuffer of previous stages.
//	void render( const Array< uint8_t >& stagesToRender );
	
	// SHADER FUNCTIONS ==========================================================
	
	// Add or remove a search path to use when looking up shader file names.
	// Relative to the executable, unless specified absolute (not suggested).
	// Removing a search path will not in any way invalidate shaders in use from
	// that path.
	void addShaderPath( const String& _path );
	void removeShaderPath( const String& _path );
	
	// Loads the shader in the appropriate file located using _name. The actual
	// file name will be this combined with the extension for both the shader type
	// given by _type and the general glsl extension used by the engine.
	// See GLSL_*_EXTENSION comments above for details.
	// 
	// Returns a valid CGSShader pointer on success, and NULL on failure.
	// 
	// Note: shaders returned are not likely to be unique. The system will only
	// create 1 object per file, as there is no need to have multiple. Do not
	// delete them. Currently, shaders will only be deleted if the GraphicsSystem
	// is deleted.
	CGSShader* getShader( const ShaderType& _type, const String& _name );
	
	// MESH FUNCTIONS ============================================================
	CGSMesh* createMesh( const GLenum& _renderOperation = GL_POINTS );
	void _notifyMeshDeleted( CGSMesh* const& mesh );
	
	// TEXTURE FUNCTIONS =========================================================
	
	// Generates a blank (zeroed) texture with the requested parameters.
	//
	// * _numMipMaps: The number of mipmaps IN ADDITION to the parent image. This
	// meaning differs slightly from OpenGL itself, which calls the parent image
	// mipmap level 0. If you do not want mipmaps, pass 0, and only the parent
	// image will be used.
	//
	// * _allowOverAlloc: Enable over-allocation behavior. See the documentation
	// for more information. Suggested to remain false unless you know what you
	// are doing.
	//
	// * _dTexelsAlloc: Number of texels to ALLOCATE in each dimension. 0
	// allocates the requested size (_dTexels) in that dimension. Ignored if
	// _allowOverAlloc = false.
	CGSTexture* createTexture(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint8_t& _numMipMaps,
		const uint16_t& _xTexels,
		const uint16_t& _yTexels = 1,
		const uint16_t& _zTexels = 1,
		const bool& _allowOverAlloc = false,
		const uint16_t& _xTexelsAlloc = 0,
		const uint16_t& _yTexelsAlloc = 0,
		const uint16_t& _zTexelsAlloc = 0 );
	
	// Use FreeImage to load an image from the file specified in _path, which is
	// relative to execution directory (unless absolute).
	//
	// Currently, this automatically picks a format based on the format of the
	// image file. Usually, this is R8_UNORM, RGB8_UNORM, or RGBA8_UNORM; you
	// should query the image after creation for the format chosen.
	CGSTexture* loadTextureFromFile(
		const String& _path,
		const uint8_t& _numMipMaps = 0 );
	
	void _textureRequestDeletion( CGSTexture* const& texture );
	
	// INFORMATION FUNCTIONS =====================================================
	
	// Returns the size in bytes of an OpenGL type identified by the passed enum;
	// based on https://www.opengl.org/wiki/OpenGL_Type . If an invalid type/enum
	// is passed, returns 0.
	static GLint oglSizeOf( const GLenum& _type );
	
	// Some platforms want texture data as RGBA, some want it as BGRA, and some
	// vary depending on the format. This function returns the (as far as CGS
	// knows) most efficient channel ordering for the given format on the current
	// platform.
	//
	// If this function returns true, BGRA ordering is preferred. If it returns
	// false, RGBA ordering is preferred.
	//
	// Because of limited data and the potential for individual drivers to vary,
	// this is not currently guaranteed to be accurate. OpenGL 4.3 has a
	// function to get the preferred format, but 4.3 support is not good enough
	// (it is essentially unavailable on Apple). If the function is available,
	// this function MIGHT use it to enhance the accuracy.
	static bool _useModifiedChannelOrdering( const TextureFormat& _format );
	
	// The function behind the above. Returns one of the type identifiers accepted
	// by the glTexImageND functions in the format position (not internalFormat).
	// This effectively identifies the number of channels, the ordering of those
	// channels, and their interpretation (float, integer, or special).
	static GLenum _getGLPreferredFormat( const TextureFormat& _format );
	
	static void getOpenGLErrors( );
	
protected:
	static GraphicsSystem* globalInstance;
	
	SDL_Window* sdlWindow;
	SDL_GLContext glContext;
	
	uint16_t screenX;
	uint16_t screenY;
	
	// Render system
	GLuint framebufferHandle; // FBO
	GLuint framebufferInternalTextureHandle; // Texture
	GLuint backbufferTextureHandle; // Texture
	AssocArray< uint8_t, CGSRenderStage > renderStages;
	
	OrderedSet< String > shaderSearchPaths;
	AssocArray< Pair< ShaderType, String >, CGSShader* > loadedShaders;
	AssocArray< uint32_t, CGSMesh* > meshes;
	UnorderedSet< CGSTexture* > textures;
	
	// This is a secondary indexing of textures, for textures which represent
	// image files. The existence of a texture pointer in this structure does NOT
	// mean the pointer is valid. Entries are not removed from textureFilesByPath
	// on texture deletion. Any texture here should be checked for validity by
	// finding it in the textures container. If it is there, the pointer is valid;
	// if it is not, it is invalid.
	AssocArray< String, CGSTexture* > textureFilesByPath;
	
	bool debugMode;
	
	bool inititalized;
	
	// Shader loading functions
	String _generateShaderFileName( const ShaderType& _type, const String& _name );
	CGSShader* _loadShader( const ShaderType& _type, const String& _path );
	
	// Variable for storing texture format conversions to base GL formats + order.
	// Not constant and protected, in order to allow it to be updated with
	// values returned by OpenGL, if the functionality exists.
	static AssocArray< TextureFormat, GLenum > TEXTURE_FORMAT_GL_FORMAT;
};

#endif	/* CGS_H */

