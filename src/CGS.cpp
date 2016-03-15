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

#include "CGS.h"
#include "CGSMesh.h"
#include "CGSShader.h"
#include "CGSTexture.h"

GraphicsSystem* GraphicsSystem::globalInstance = NULL;

const char* GraphicsSystem::GLSL_FINAL_EXTENSION = "glsl";
const char* GraphicsSystem::GLSL_VERTEX_EXTENSION = "vert";
const char* GraphicsSystem::GLSL_FRAGMENT_EXTENSION = "frag";
const char* GraphicsSystem::GLSL_GEOMETRY_EXTENSION = "geom";
const char* GraphicsSystem::GLSL_TESSELLATION_CONTROL_EXTENSION = "tctl";
const char* GraphicsSystem::GLSL_TESSELLATION_EVALUATION_EXTENSION = "tevl";
const char* GraphicsSystem::GLSL_INCLUDE_EXTENSION = "inc";
const char* GraphicsSystem::DEFAULT_SHADER_NAME = "default";
const char* GraphicsSystem::DEFAULT_SHADER_PATH = "glsl/";

AssocArray< TextureFormat, GLenum > _init_TEXTURE_FORMAT_GL_FORMAT( )
{
	AssocArray< TextureFormat, GLenum > r;
	
	// The modified channel ordering is based on information calculated by
	// freeimage. It may not be entirely accurate, but it is better than guessing.
	// If possible, the system will (IN THE FUTURE) request clarification from
	// OpenGL on the preferred internal format.
	
	// If red is the first channel, then normal ordering is in use.
	#if FI_RGBA_RED == 0
	#define CGS_THREE_COMP_ORDERING GL_RGB
	#define CGS_FOUR_COMP_ORDERING GL_RGBA
	#else
	#define CGS_THREE_COMP_ORDERING GL_BGR
	#define CGS_FOUR_COMP_ORDERING GL_BGRA
	#endif
	
	// 1 component float
	r[ TextureFormat::R8_UNORM ] = GL_RED;
	r[ TextureFormat::R16_UNORM ] = GL_RED;
	r[ TextureFormat::R8_SNORM ] = GL_RED;
	r[ TextureFormat::R16_SNORM ] = GL_RED;
	r[ TextureFormat::R16F ] = GL_RED;
	r[ TextureFormat::R32F ] = GL_RED;
	
	// 1 component int
	r[ TextureFormat::R8UI ] = GL_RED_INTEGER;
	r[ TextureFormat::R16UI ] = GL_RED_INTEGER;
	r[ TextureFormat::R32UI ] = GL_RED_INTEGER;
	r[ TextureFormat::R8I ] = GL_RED_INTEGER;
	r[ TextureFormat::R16I ] = GL_RED_INTEGER;
	r[ TextureFormat::R32I ] = GL_RED_INTEGER;
	
	// 2 component float
	r[ TextureFormat::RG8_UNORM ] = GL_RG;
	r[ TextureFormat::RG16_UNORM ] = GL_RG;
	r[ TextureFormat::RG8_SNORM ] = GL_RG;
	r[ TextureFormat::RG16_SNORM ] = GL_RG;
	r[ TextureFormat::RG16F ] = GL_RG;
	r[ TextureFormat::RG32F ] = GL_RG;
	
	// 2 component int
	r[ TextureFormat::RG8UI ] = GL_RG_INTEGER;
	r[ TextureFormat::RG16UI ] = GL_RG_INTEGER;
	r[ TextureFormat::RG32UI ] = GL_RG_INTEGER;
	r[ TextureFormat::RG8I ] = GL_RG_INTEGER;
	r[ TextureFormat::RG16I ] = GL_RG_INTEGER;
	r[ TextureFormat::RG32I ] = GL_RG_INTEGER;
	
	// 3 component float
	r[ TextureFormat::RGB8_UNORM ] = CGS_THREE_COMP_ORDERING; // Mod
	r[ TextureFormat::RGB16_UNORM ] = GL_RGB;
	r[ TextureFormat::RGB8_SNORM ] = CGS_THREE_COMP_ORDERING; // Mod
	r[ TextureFormat::RGB16_SNORM ] = GL_RGB;
	r[ TextureFormat::RGB16F ] = GL_RGB;
	r[ TextureFormat::RGB32F ] = GL_RGB;
	
	// 3 component int
	r[ TextureFormat::RGB8UI ] = GL_RGB_INTEGER;
	r[ TextureFormat::RGB16UI ] = GL_RGB_INTEGER;
	r[ TextureFormat::RGB32UI ] = GL_RGB_INTEGER;
	r[ TextureFormat::RGB8I ] = GL_RGB_INTEGER;
	r[ TextureFormat::RGB16I ] = GL_RGB_INTEGER;
	r[ TextureFormat::RGB32I ] = GL_RGB_INTEGER;
	
	// 4 component float
	r[ TextureFormat::RGBA8_UNORM ] = CGS_FOUR_COMP_ORDERING; // Mod
	r[ TextureFormat::RGBA16_UNORM ] = GL_RGBA;
	r[ TextureFormat::RGBA8_SNORM ] = CGS_FOUR_COMP_ORDERING; // Mod
	r[ TextureFormat::RGBA16_SNORM ] = GL_RGBA;
	r[ TextureFormat::RGBA16F ] = GL_RGBA;
	r[ TextureFormat::RGBA32F ] = GL_RGBA;
	
	// 4 component int
	r[ TextureFormat::RGBA8UI ] = GL_RGBA_INTEGER;
	r[ TextureFormat::RGBA16UI ] = GL_RGBA_INTEGER;
	r[ TextureFormat::RGBA32UI ] = GL_RGBA_INTEGER;
	r[ TextureFormat::RGBA8I ] = GL_RGBA_INTEGER;
	r[ TextureFormat::RGBA16I ] = GL_RGBA_INTEGER;
	r[ TextureFormat::RGBA32I ] = GL_RGBA_INTEGER;
	
	return r;
}

AssocArray< TextureFormat, GLenum > GraphicsSystem::TEXTURE_FORMAT_GL_FORMAT
		= _init_TEXTURE_FORMAT_GL_FORMAT( );

GraphicsSystem::GraphicsSystem( const bool& _disableDebug )
{
	sdlWindow = NULL;
	screenX = 0;
	screenY = 0;
	debugMode = !_disableDebug;
	
	globalInstance = this;
	inititalized = false;
	
	framebufferHandle = 0;
	framebufferInternalTextureHandle = 0;
	backbufferTextureHandle = 0;
}

GraphicsSystem::~GraphicsSystem( )
{
	globalInstance = NULL;
	
	if( !inititalized )
	{
		return;
	}
	
	for( auto i = meshes.begin( ); i != meshes.end( ); ++i )
	{
		delete i->second;
	}
	
	for( auto i = textures.begin( ); i != textures.end( ); ++i )
	{
		delete (*i);
	}
	
	glDeleteFramebuffers( 1, &framebufferHandle );
	glDeleteTextures( 1, &framebufferInternalTextureHandle );
	glDeleteTextures( 1, &backbufferTextureHandle );
	
	SDL_GL_DeleteContext( glContext );
	SDL_DestroyWindow( sdlWindow );
	SDL_Quit( );
}

bool GraphicsSystem::init( const uint16_t& _x, const uint16_t& _y, const String& _windowName, const String& _shaderPath )
{
	assert( !inititalized );
	
	addShaderPath( _shaderPath );
	
	screenX = _x;
	screenY = _y;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		U::log( "Unable to initialize SDL" );
		SDL_Quit( );
		return false;
	}
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 ); 
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 ); 
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );
	
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
	
	sdlWindow = SDL_CreateWindow(
			_windowName.c_str( ),
			SDL_WINDOWPOS_CENTERED, // x
			SDL_WINDOWPOS_CENTERED, // y
			_x,
			_y,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

	if( !sdlWindow )
	{
		U::log( "Unable to create window" );
		SDL_Quit( );
		return false;
	}

	glContext = SDL_GL_CreateContext( sdlWindow );
	SDL_GL_SetSwapInterval( 0 ); // 0 is no vsync, 1 is vsync, 2 is vsync-like

	U::log( SDL_GetError( ) );
	SDL_ClearError( );
	
#ifndef __APPLE__
	getOpenGLErrors( );
	
	GLenum glStatus = glewInit();
	
	// Ignore an invalid enum error which glew likes to generate.
	glGetError();
	
	if( glStatus != GLEW_OK )
	{
			U::log( "Error: GLEW / OpenGL failed to start with the following error:" );
			U::log( glewGetErrorString( glStatus ) );
			return false;
	}
	else
	{
		U::log( "Successfully initialized OpenGL instance. " );
		U::log( "Vendor:  ", String( (const char*)glGetString( GL_VENDOR ) ) );
		U::log( "Version: ", String( (const char*)glGetString( GL_VERSION ) ) );
		U::log( "Device:  ", String( (const char*)glGetString( GL_RENDERER ) ) );
		U::log( "Shading: ", String( (const char*)glGetString( GL_SHADING_LANGUAGE_VERSION ) ) );
	}
#else
	U::log( "Successfully initialized OpenGL instance. " );
	U::log( "Vendor:  ", String( (const char*)glGetString( GL_VENDOR ) ) );
	U::log( "Version: ", String( (const char*)glGetString( GL_VERSION ) ) );
	U::log( "Device:  ", String( (const char*)glGetString( GL_RENDERER ) ) );
	U::log( "Shading: ", String( (const char*)glGetString( GL_SHADING_LANGUAGE_VERSION ) ) );
#endif
	
	getOpenGLErrors( );
	
	// Check support of required features - move elsewhere later
	GLboolean value;
	glGetBooleanv( GL_SHADER_COMPILER, &value );
	if( value != GL_TRUE )
	{
		U::log( "Error: This system's OpenGL implementation does not support GLSL compilation, which is required by this engine." );
		U::log( "- This could be an issue with the video driver." );
		return false;
	}
	
	// Set this now so we can call some of the following functions to finish
	// the setup.
	inititalized = true;
	
	// Create and configure the buffer objects. Some of this will need moved and
	// modified when resizable windows are added [soon!].
	glGenFramebuffers( 1, &framebufferHandle );
	glGenTextures( 1, &framebufferInternalTextureHandle );
	glGenTextures( 1, &backbufferTextureHandle );
	
	glBindTexture( GL_TEXTURE_2D, backbufferTextureHandle );
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA32F, screenX, screenY );
	
	glBindTexture( GL_TEXTURE_2D, framebufferInternalTextureHandle );
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA32F, screenX, screenY );
	
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, framebufferHandle );
	glFramebufferTexture2D(
			GL_DRAW_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			framebufferInternalTextureHandle,
			0 );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	
	// Create the default vertex and fragment shaders
	if( !getShader( ShaderType::VERTEX, DEFAULT_SHADER_NAME ) )
	{
		U::log( "Error: Failed to load default vertex shader." );
		return false;
	}
	if( !getShader( ShaderType::FRAGMENT, DEFAULT_SHADER_NAME ) )
	{
		U::log( "Error: Failed to load default fragment shader." );
		return false;
	}
	
	return true;
}

void GraphicsSystem::setDebugMode( const bool& _mode )
{
	debugMode = _mode;
}

void GraphicsSystem::addShaderPath( const String& _path )
{
	shaderSearchPaths.insert( _path );
}

void GraphicsSystem::removeShaderPath( const String& _path )
{
	shaderSearchPaths.erase( _path );
}


String GraphicsSystem::_generateShaderFileName( const ShaderType& _type, const String& _name )
{
	if( _type == ShaderType::VERTEX )
	{
		return U::c( _name, ".", GLSL_VERTEX_EXTENSION, ".", GLSL_FINAL_EXTENSION );
	}
	else if( _type == ShaderType::FRAGMENT )
	{
		return U::c( _name, ".", GLSL_FRAGMENT_EXTENSION, ".", GLSL_FINAL_EXTENSION );
	}
	else if( _type == ShaderType::GEOMETRY )
	{
		return U::c( _name, ".", GLSL_GEOMETRY_EXTENSION, ".", GLSL_FINAL_EXTENSION );
	}
	else if( _type == ShaderType::TESSELLATION_EVALUATION )
	{
		return U::c( _name, ".", GLSL_TESSELLATION_EVALUATION_EXTENSION, ".", GLSL_FINAL_EXTENSION );
	}
	else // _type == ShaderType::TESSELLATION_CONTROL
	{
		return U::c( _name, ".", GLSL_TESSELLATION_CONTROL_EXTENSION, ".", GLSL_FINAL_EXTENSION );
	}
}

CGSShader* GraphicsSystem::_loadShader( const ShaderType& _type, const String& _name )
{
	assert( inititalized );
	
	for( OrderedSet< String >::iterator i = shaderSearchPaths.begin( );
		i != shaderSearchPaths.end( ); ++i )
	{
		String path = U::c( *i, _name );
		
		std::ifstream file( path );
		if( file.good( ) )
		{
			String shaderSource;
			file.seekg( 0, std::ifstream::end );
			shaderSource.reserve( file.tellg( ) );
			file.seekg( 0, std::ifstream::beg );
			shaderSource.assign( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>( ) );
			
			file.close( );
		
			CGSShader* shader = new CGSShader( _type );
			
			if( shader->build( shaderSource ) )
			{
				return shader;
			}
			else
			{
				// CGSShader will print an error on build failure.
				delete shader;
				return NULL;
			}
		}
		file.close( );
	}
	
	U::log( "Error: GLSL file ", _name, " not found in any GLSL search path." );
	return NULL;
}

CGSShader* GraphicsSystem::getShader( const ShaderType& _type, const String& _name )
{
	assert( inititalized );
	
	AssocArray< Pair< ShaderType, String >, CGSShader* >::iterator i
		= loadedShaders.find( U::p( _type, _name ) );
	
	if( i != loadedShaders.end( ) )
	{
		return i->second;
	}
	
	// No entry exists. Attempt to load shader.
	CGSShader* shader = _loadShader( _type, _generateShaderFileName( _type, _name ) );
	
	if( shader )
	{
		loadedShaders.insert( U::p( U::p( _type, _name ), shader ) );
	}
	
	return shader;
}

CGSMesh* GraphicsSystem::createMesh( const GLenum& _renderOperation )
{
	assert( inititalized );
	
	CGSMesh* r = new CGSMesh( _renderOperation );
	meshes.insert( U::p( r->getID( ), r ) );
	return r;
}

void GraphicsSystem::_notifyMeshDeleted( CGSMesh* const& mesh )
{
	meshes.erase( mesh->getID( ) );
}

CGSTexture* GraphicsSystem::createTexture(
		const TextureDimensionality& _dimensionality,
		const TextureFormat& _format,
		const uint8_t& _numMipMaps,
		const uint16_t& _xTexels,
		const uint16_t& _yTexels,
		const uint16_t& _zTexels,
		const bool& _allowOverAlloc,
		const uint16_t& _xTexelsAlloc,
		const uint16_t& _yTexelsAlloc,
		const uint16_t& _zTexelsAlloc )
{
	assert( inititalized );
	
	uint8_t dim = CGSTexture::TEXTURE_DIMENSIONALITY_DIMENSIONS.find( _dimensionality )->second;
	CGSTexture* tex;
	
	if( dim == 2 )
	{ // 2D
		tex = new CGSTexture2D( 
				_dimensionality,
				_format,
				_numMipMaps,
				_xTexels,
				_yTexels,
				_allowOverAlloc,
				_xTexelsAlloc,
				_yTexelsAlloc );
	}
	else if( dim == 1 )
	{ // 1D
		tex = new CGSTexture1D( 
				_dimensionality,
				_format,
				_numMipMaps,
				_xTexels,
				_allowOverAlloc,
				_xTexelsAlloc );
	}
	else
	{ // 3D
		tex = new CGSTexture3D( 
				_dimensionality,
				_format,
				_numMipMaps,
				_xTexels,
				_yTexels,
				_zTexels,
				_allowOverAlloc,
				_xTexelsAlloc,
				_yTexelsAlloc,
				_zTexelsAlloc );
	}
	
	textures.insert( tex );
	return tex;
}

CGSTexture* GraphicsSystem::loadTextureFromFile(
		const String& _path,
		const uint8_t& _numMipMaps )
{
	assert( inititalized );
	
	size_t lastPeriod = _path.rfind( '.' );
	if( String::npos == lastPeriod )
	{
		U::log( "Invalid texture file path given in call to loadTextureFromFile( )." );
		U::log( " - File lacks proper extension." );
		U::log( " - Path: ", _path );
		return NULL;
	}
	
	// Check if the file is already loaded.
	auto existingImage = textureFilesByPath.find( _path );
	if( existingImage != textureFilesByPath.end( ) )
	{
		// Just being in the textureFilesByPath list doesn't mean the image hasn't
		// been unloaded. It is only valid if it is also in the textures list.
		if( textures.find( existingImage->second ) != textures.end( ) )
		{
			return existingImage->second;
		}
		
		// If this is reached, the entry in textureFilesByPath is no longer in
		// memory, so loading should continue.
	}
	
	FIBITMAP* imageFile = FreeImage_Load( FreeImage_GetFileType( _path.c_str( ), 0 ), _path.c_str( ) );
	
	if( !imageFile )
	{
		U::log( "FreeImage unable to load image file in loadTextureFromFile( )." );
		U::log( " - Path: ", _path );
		return NULL;
	}
	
	TextureFormat cgsFormat;
	unsigned short numChannels = 0;
	bool unsupportedFormat = false;
	
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType( imageFile );
	if( imageType == FIT_BITMAP )
	{
		unsigned int bits = FreeImage_GetBPP( imageFile );
		
		if( bits == 8 )
		{ // Single component
			cgsFormat = TextureFormat::R8_UNORM;
			numChannels = 1;
		}
		else if( bits == 24 )
		{ // Three component
			cgsFormat = TextureFormat::RGB8_UNORM;
			numChannels = 3;
		}
		else if( bits == 32 )
		{ // Four component
			cgsFormat = TextureFormat::RGBA8_UNORM;
			numChannels = 4;
		}
		else
		{
			unsupportedFormat = true;
		}
	}
	else
	{
		unsupportedFormat = true;
	}
	
	if( unsupportedFormat )
	{
		U::log( "Unsupported image format in loadTextureFromFile( )." );
		U::log( " - Path: ", _path );
		FreeImage_Unload( imageFile );
		return NULL;
	}
	
	// If this got this far, we know the format CGS needs to use to match the
	// FreeImage internal type. Ordering is another matter which will be dealt
	// with below.
	
	unsigned int width = FreeImage_GetWidth( imageFile );
	unsigned int height = FreeImage_GetHeight( imageFile );
	unsigned int pitch = FreeImage_GetPitch( imageFile );
	
	CGSTexture2D* tex = new CGSTexture2D(
		TextureDimensionality::TEX_2D, cgsFormat, _numMipMaps,
		width, height,
		false, 0, 0,
		true );
	
	// Figure out channel ordering.
	uint8_t redPos;
	uint8_t bluePos;
	// Green is always 1, alpha is always 3, no matter the ordering.
	if( tex->usesModifiedChannelOrdering( ) )
	{
		redPos = CGSTexture::TEXTURE_MODIFIED_ORDERING_OFFSET_RED; // 2
		bluePos = CGSTexture::TEXTURE_MODIFIED_ORDERING_OFFSET_BLUE; // 0
	}
	else
	{ // Standard ordering
		redPos = CGSTexture::TEXTURE_STANDARD_ORDERING_OFFSET_RED; // 0
		bluePos = CGSTexture::TEXTURE_STANDARD_ORDERING_OFFSET_BLUE; // 2
	}
	
	BYTE* source = (BYTE*)FreeImage_GetBits( imageFile );
	for( unsigned int y = 0; y < height; ++y )
	{
		BYTE* sourcePixel = (BYTE*)source;
		
		// Channel 4 is start-of-memory no matter ordering.
		// (height - y - 1) is used for y because FreeImage is "upside-down" with
		// regard to CGS' conventions.
		uint8_t* destPixel = tex->access( 0, height - y - 1, 0, 4 );
		
		for( unsigned int x = 0; x < width; ++x )
		{
			destPixel[ redPos ] = sourcePixel[ FI_RGBA_RED ]; // R
			
			if( numChannels > 1 )
			{
				destPixel[ 1 ] = sourcePixel[ FI_RGBA_GREEN ]; // G
				destPixel[ bluePos ] = sourcePixel[ FI_RGBA_BLUE ]; // B
			}
			
			if( numChannels == 4 )
			{
				destPixel[ 4 ] = sourcePixel[ FI_RGBA_ALPHA ]; // A
			}
			
			sourcePixel += numChannels;
			destPixel += numChannels;
		}
		
		source += pitch;
	}
	
	// Clean up and store texture.
	FreeImage_Unload( imageFile );
	textures.insert( tex );
	textureFilesByPath.insert( U::p( _path, tex ) );
	
	return tex;
}

void GraphicsSystem::_textureRequestDeletion( CGSTexture* const& texture )
{
	textures.erase( texture );
	delete texture;
}

GLint GraphicsSystem::oglSizeOf( const GLenum& _type )
{
	// Values from https://www.opengl.org/wiki/OpenGL_Type
	if( _type == GL_BYTE )
	{
		return 1;
	}
	else if( _type == GL_UNSIGNED_BYTE )
	{
		return 1;
	}
	else if( _type == GL_SHORT )
	{
		return 2;
	}
	else if( _type == GL_UNSIGNED_SHORT )
	{
		return 2;
	}
	else if( _type == GL_INT )
	{
		return 4;
	}
	else if( _type == GL_UNSIGNED_INT )
	{
		return 4;
	}
	else if( _type == GL_FIXED )
	{
		return 4;
	}
	else if( _type == GL_HALF_FLOAT )
	{
		return 2;
	}
	else if( _type == GL_FLOAT )
	{
		return 4;
	}
	else // _type == GL_DOUBLE
	{
		return 8;
	}
}

bool GraphicsSystem::_useModifiedChannelOrdering( const TextureFormat& _format )
{
	GLenum prefGLFormat = _getGLPreferredFormat( _format );
	
	return ( ( prefGLFormat == GL_BGR )
			|| ( prefGLFormat == GL_BGRA )
			|| ( prefGLFormat == GL_BGR_INTEGER )
			|| ( prefGLFormat == GL_BGRA_INTEGER ) );
}

GLenum GraphicsSystem::_getGLPreferredFormat( const TextureFormat& _format )
{
	return TEXTURE_FORMAT_GL_FORMAT.find( _format )->second;
}

void GraphicsSystem::getOpenGLErrors( )
{
	GLenum err = GL_NO_ERROR;
	while( ( err = glGetError() ) != GL_NO_ERROR )
	{
		U::log( "OpenGL error: ", String( (char*)gluErrorString( err ) ), " - ", __FILE__, ":", __LINE__ );
	}
}