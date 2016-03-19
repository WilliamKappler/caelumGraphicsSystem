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
#include "CGSMesh.h"
#include "CGSShader.h"
#include "CGSTexture.h"
#include "CGSMeshToTextureAdapter.h"

const GLuint CGSMesh::VERTEX_BINDING_POINT_POSITION = 0;
const GLuint CGSMesh::VERTEX_BINDING_POINT_UVW = 1;
const GLuint CGSMesh::VERTEX_BINDING_POINT_COLOR = 2;
const GLuint CGSMesh::VERTEX_BINDING_POINT_NORMAL = 3;
const GLuint CGSMesh::VERTEX_BINDING_POINT_FIRST_USER = 4;

uint32_t CGSMesh::nextMeshObjectID = 1;

// External ======================================================================

CGSMesh::CGSMesh( const GLenum& _renderOperation )
{
	id = nextMeshObjectID++;
	glGenVertexArrays( 1, &vaoHandle );
	glGenBuffers( 1, &vertexDataBufferHandle );
	// The buffer for indexes is only created if such is called for
	indexBufferHandle = 0;
	renderOperation = _renderOperation;
	
	visible = true;
	steamIsValid = false;
	steamUpdated = true;
	streamLength = -1;
	stream = NULL;
	
	calculatedStreamStride = -1;
	
	openAttributeIndex = -1;
	
	useIndexes = false;
	
	// Program
	programHandle = glCreateProgram( );
	vertexShader
		= GraphicsSystem::getGlobalInstance( )->getShader( ShaderType::VERTEX, GraphicsSystem::DEFAULT_SHADER_NAME );
	fragmentShader
		= GraphicsSystem::getGlobalInstance( )->getShader( ShaderType::FRAGMENT, GraphicsSystem::DEFAULT_SHADER_NAME );
	geometryShader = NULL;
	linked = false;
}

CGSMesh::~CGSMesh( )
{
	GraphicsSystem::getGlobalInstance( )->_notifyMeshDeleted( this );
	
	glDeleteProgram( programHandle );
	
	if( stream )
	{
		delete[] stream;
	}
	
	// TODO: Is this enough to clean up OpenGL?
	
	if( useIndexes )
	{
		deleteIndexBuffer( );
	}
	
	// Delete the vertex array created in the constructor
	glDeleteVertexArrays( 1, &vaoHandle );
	glDeleteBuffers( 1, &vertexDataBufferHandle );
	// If this tries to delete zero, it's ok, OpenGL ignores it
	glDeleteBuffers( 1, &indexBufferHandle );
}

void CGSMesh::setRenderOperation( const GLenum& mode )
{
	renderOperation = mode;
}

void CGSMesh::createVertexAttribute(
			const GLuint& attributeIndex,
			const GLenum& type,
			const GLint& numberOfElements,
			const bool& integerType,
			const GLboolean& normalize )
{
	AssocArray< GLuint, VertexAttributeData >::iterator i = attributeDefinitions.find( attributeIndex );
	
	// If a definition does not exist, create one
	if( i == attributeDefinitions.end() )
	{
		i = attributeDefinitions.insert( U::p( attributeIndex, VertexAttributeData() ) ).first;
	}
	else
	{
		U::log( "Warning: Duplicate vertex attribute created for index ", attributeIndex,
				" in MeshObject with ID ", getID( ) );
	}
	
	i->second.type = type;
	i->second.numberOfElements = numberOfElements;
	i->second.useInterger = integerType;
	i->second.normalize = normalize;
	
	i->second.length = numberOfElements * GraphicsSystem::oglSizeOf( type );
	
	steamIsValid = false;
}

bool CGSMesh::deleteVertexAttribute( const GLuint& attributeIndex )
{
	AssocArray< GLuint, VertexAttributeData >::iterator i = attributeDefinitions.find( attributeIndex );
	
	if( i != attributeDefinitions.end( ) )
	{
		attributeDefinitions.erase( i );
		steamIsValid = false;
		return true;
	}
	
	return false;
}

void CGSMesh::generateDataStream( const uint16_t& length )
{
	streamLength = length;
	
	// Calculate the stride
	calculatedStreamStride = 0;
	
	AssocArray< GLuint, VertexAttributeData >::iterator aDefI = attributeDefinitions.begin( );
	while( aDefI != attributeDefinitions.end( ) )
	{
		// Set where this data will begin, which is whatever the currently
		// built up stride is. 0 for the first block of data
		aDefI->second.streamPointerOffset = calculatedStreamStride;
		
		// Get the amount of space the data will take
		calculatedStreamStride += GraphicsSystem::oglSizeOf( aDefI->second.type )
				* aDefI->second.numberOfElements;
		
		++aDefI;
	}
	
	if( stream )
	{
		// Delete the current stream data. No need to inform OGL, it doesn't
		// link itself to the pointer. We just override the data later.
		delete[] stream;
	}
	
	if( calculatedStreamStride )
	{
		stream = new uint8_t[ calculatedStreamStride * streamLength ];

		// Reset the open attribute to none; user shouldn't have an open attribute
		// when calling this, but if they do, it should be closed.
		openAttributeIndex = -1;

		// Stream is now valid for attributes declared
		steamIsValid = true;

		// Stream needs uploaded to video card next frame or sooner - Hopefully after
		// writing meaningful data.
		steamUpdated = true;
	}
	else
	{
		U::log( "Warning: generateDataStream() called on mesh with no vertex attributes (not even position), resulting in a zero-length (invalid) stream, in MeshObject with ID ", getID( ) );

		// Failsafe values
		stream = NULL;
		steamIsValid = false;
	}
}

bool CGSMesh::openAttribute( const GLuint& attributeIndex )
{
	if( openAttributeIndex != ( GLuint )( -1 ) )
	{
		// No warning necessary, this is acceptable, at least for now
		closeAttribute( );
		return false;
	}
	
	AssocArray< GLuint, VertexAttributeData >::iterator aDefI = attributeDefinitions.find( attributeIndex );
	if( aDefI == attributeDefinitions.end( ) )
	{
		U::log( "Warning: openAttribute() called with non-existent index '", attributeIndex, "', in MeshObject with ID ", getID( ) );

		return false;
	}
	
	openAttributeIndex = attributeIndex;
	openAttributePosition = 0;
	openAttributeType = aDefI->second.type;
	openAttributeNumberOfElements = aDefI->second.numberOfElements;
	openAttributeStride = calculatedStreamStride - aDefI->second.length;
	openAttributeOffset = aDefI->second.streamPointerOffset;
	return true;
}

void* CGSMesh::_getDataLocationPointer( )
{
	// stream is a 1-byte type, not void*, so this is valid
	return stream + (
			// The position in a given vertex where the current attribute resides
			openAttributeOffset
			// Jumps between the vertex data blocks
			+ ( openAttributePosition / openAttributeNumberOfElements ) * calculatedStreamStride
			// Increments within the attribute itself, if multi-element
			+ ( openAttributePosition % openAttributeNumberOfElements ) * GraphicsSystem::oglSizeOf( openAttributeType ) );
}

bool CGSMesh::_checkOpenAttribute( const GLenum& _functionType )
{
	// No (valid) vertex attribute index open
	if( openAttributeIndex == ( GLuint )( -1 ) )
	{
		return false;
	}
	
	// Current location at/past end of stream
	if( openAttributePosition >= streamLength * openAttributeNumberOfElements )
	{
		return false;
	}
	
	// Types match. Note the float function is used for half floats
	if( _functionType == openAttributeType
			|| ( _functionType == GL_FLOAT && openAttributeType == GL_HALF_FLOAT ) )
	{
		return true;
	}
	
	U::log( "Error: Open vertex attribute type does not match type passed to writeToA() in MeshObject with ID ", getID( ) );
	return false;
}

void CGSMesh::_writeToABackend( const void* const& dP, const GLenum& _type )
{
	if( !_checkOpenAttribute( _type ) )
	{
		U::log( "Error: Invalid vertex attribute write operation called in MeshObject with ID ", getID( ) );
		
		return;
	}
	
	memcpy( _getDataLocationPointer( ),
			dP,
			GraphicsSystem::oglSizeOf( _type ) );
	
	++openAttributePosition;
}

void CGSMesh::writeToA( const float& d )
{
	
	// TEMPORARY: Half-float support needs implemented. Need way to cast float
	// to half-float in C++.
	if( openAttributeType == GL_HALF_FLOAT )
	{
		uint16_t half = floatToHalf( d );
		_writeToABackend( &half, GL_HALF_FLOAT );
		return;
	}
	
	_writeToABackend( &d, GL_FLOAT );
}

void CGSMesh::writeToA( const double& d )
{
	_writeToABackend( &d, GL_DOUBLE );
}

void CGSMesh::writeToA( const int8_t& d )
{
	_writeToABackend( &d, GL_BYTE );
}

void CGSMesh::writeToA( const uint8_t& d )
{
	_writeToABackend( &d, GL_UNSIGNED_BYTE );
}

void CGSMesh::writeToA( const int16_t& d )
{
	_writeToABackend( &d, GL_SHORT );
}

void CGSMesh::writeToA( const uint16_t& d )
{
	_writeToABackend( &d, GL_UNSIGNED_SHORT );
}

void CGSMesh::writeToA( const int32_t& d )
{
	_writeToABackend( &d, GL_INT );
}

void CGSMesh::writeToA( const uint32_t& d )
{
	_writeToABackend( &d, GL_UNSIGNED_INT );
}

void CGSMesh::closeAttribute( )
{
	if( openAttributePosition != streamLength * openAttributeNumberOfElements )
	{
		U::log( "Warning: Incomplete write to vertex attribute with attribute index '",
				openAttributeIndex, "' [", openAttributePosition, " out of ",
				streamLength * openAttributeNumberOfElements,
				" elements written] in MeshObject with ID ", getID( ) );
	}
	
	steamUpdated = true;
	openAttributeIndex = -1;
}

void CGSMesh::createIndexBuffer( const uint16_t& preallocate )
{
	if( useIndexes )
	{
		U::log( "Warning: CGSMesh::createIndexBuffer() called, but index buffer already exists in MeshObject with ID ", getID( ), " - clearing instead" );
		
		clearIndexBuffer( );
	}
	else
	{
		glGenBuffers( 1, &indexBufferHandle );
		useIndexes = true;
	}
	
	indexData.reserve( preallocate );
	indexesUpdated = true;
}

void CGSMesh::writeToI( const uint32_t& d, const uint16_t& at )
{
	indexData[ at ] = d;
	indexesUpdated = true;
}

void CGSMesh::writeToI( const uint32_t& d )
{
	indexData[ indexPosition++ ] = d;
	indexesUpdated = true;
}

bool CGSMesh::moveToI( const uint16_t& newPos )
{
	indexPosition = newPos;
	return true;
}

void CGSMesh::resizeIndexBuffer( const uint16_t& newLength )
{
	indexData.resize( newLength );
}

uint16_t CGSMesh::getIndexBufferSize( )
{
	return indexData.size( );
}

void CGSMesh::clearIndexBuffer( )
{
	indexData.clear( );
	indexPosition = 0;
	indexesUpdated = true;
}

void CGSMesh::deleteIndexBuffer( )
{
	// Is this sufficient to remove indexes?
	glDeleteBuffers( 1, &indexBufferHandle );
	indexData.resize( 0 );
	useIndexes = false;
}

// Internal ======================================================================

void CGSMesh::_update( )
{
	if( !visible )
	{
		return;
	}
	
	_linkProgram( );
	
	if( !steamIsValid )
	{
		U::log( "Error: Cannot upload mesh data to OpenGL as vertex attributes are not valid. This is likely due to failing to call generateDataStream( ) and populate the stream with valid data." );
		return;
	}
	
	// Bind the VAO
	glBindVertexArray( vaoHandle );
	
	// Indexes
	if( useIndexes && indexesUpdated )
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle );
		
		// Dump the current data - orphan it
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW );
		
		// Upload the read data
		glBufferData( GL_ELEMENT_ARRAY_BUFFER,
				sizeof( uint32_t ) * indexData.size( ),
				indexData.data( ),
				GL_DYNAMIC_DRAW );
		
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		
		indexesUpdated = false;
	}
	
	// Vertex attribute data - much more complex
	if( steamUpdated )
	{
		glBindBuffer( GL_ARRAY_BUFFER, vertexDataBufferHandle );
		glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW );
		glBufferData( GL_ARRAY_BUFFER, streamLength * calculatedStreamStride, stream, GL_DYNAMIC_DRAW );
		
		// Define attribute locations
		for( AssocArray< GLuint, VertexAttributeData >::iterator i = attributeDefinitions.begin( );
			i != attributeDefinitions.end( ); ++i )
		{
			if( i->second.useInterger )
			{
				// glVertexAttrib_I_Pointer i->second.
				glVertexAttribIPointer( i->first, // Index
					i->second.numberOfElements, // 1-4
					i->second.type,
					calculatedStreamStride, // Spacing between element blocks
					(char*)(uintptr_t)i->second.streamPointerOffset );
			}
			else if( i->second.type == GL_DOUBLE )
			{
				// glVertexAttrib_L_Pointer
				glVertexAttribLPointer( i->first,
					i->second.numberOfElements,
					GL_DOUBLE,
					calculatedStreamStride,
					(char*)(uintptr_t)i->second.streamPointerOffset );
			}
			else
			{
				// Just normal glVertexAttribPointer
				glVertexAttribPointer( i->first,
					i->second.numberOfElements,
					i->second.type,
					i->second.normalize,
					calculatedStreamStride,
					(char*)(uintptr_t)i->second.streamPointerOffset );
			}
			
			// Enable the above-specified attribute
			glEnableVertexAttribArray( i->first );
		}
		
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		
		steamUpdated = false;
	}
	
	glBindVertexArray( 0 );
}

void CGSMesh::_render( )
{
	if( !visible )
	{
		return;
	}
	
	if( steamUpdated || !linked || ( useIndexes && indexesUpdated ) )
	{
		_update( );
	}
	
	// glBindVertexArray
	// glDrawArrays
	
	GLsizei numberOfVertexes = 0;
	// If using indexes, there might actually be more vertexes than the stream represents, due to
	// repeated vertexes. As such, first it must first be determined if indexes are used or not.
	if( useIndexes )
	{
		numberOfVertexes = indexData.size( );
	}
	else
	{
		numberOfVertexes = streamLength;
	}
	
	// Moved this from _update( ) - I seem to have misplaced it there when I wrote
	// this. Textures are bound to render, not update.
	
	// Bind textures
	for( auto i = textures.begin( ); i != textures.end( ); ++i )
	{
		i->second.bind( );
	}
	
	// Actually draw the mesh
	glUseProgram( programHandle );
	
	glBindVertexArray( vaoHandle );
	glDrawArrays( renderOperation, 0, numberOfVertexes );
	glBindVertexArray( 0 );
	
	glUseProgram( 0 );
}

// PROGRAM FUNCTIONS =============================================================

const GLuint& CGSMesh::_getProgramHandle( const bool& forceLink )
{
	if( forceLink )
	{
		_linkProgram( );
	}
	
	return programHandle;
}

bool CGSMesh::loadVertexShader( const String& _fileName )
{
	CGSShader* temp
		= GraphicsSystem::getGlobalInstance( )->getShader( ShaderType::VERTEX, _fileName );
		
	if( temp )
	{
		vertexShader = temp;
		linked = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool CGSMesh::loadFragmentShader( const String& _fileName )
{
	CGSShader* temp
		= GraphicsSystem::getGlobalInstance( )->getShader( ShaderType::FRAGMENT, _fileName );
		
	if( temp )
	{
		fragmentShader = temp;
		linked = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool CGSMesh::loadGeometryShader( const String& _fileName )
{
	CGSShader* temp
		= GraphicsSystem::getGlobalInstance( )->getShader( ShaderType::GEOMETRY, _fileName );
		
	if( temp )
	{
		geometryShader = temp;
		linked = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool CGSMesh::attachVertexShader( CGSShader* const& _shader )
{
	assert( _shader->getType( ) == ShaderType::VERTEX );
	
	if( !_shader )
	{
		return false;
	}
	
	vertexShader = _shader;
	linked = false;
	return true;
}

bool CGSMesh::attachFragmentShader( CGSShader* const& _shader )
{
	assert( _shader->getType( ) == ShaderType::FRAGMENT );
	
	if( !_shader )
	{
		return false;
	}
	
	fragmentShader = _shader;
	linked = false;
	return true;
}

bool CGSMesh::attachGeometryShader( CGSShader* const& _shader )
{
	assert( _shader->getType( ) == ShaderType::GEOMETRY );
	
	if( !_shader )
	{
		return false;
	}
	
	geometryShader = _shader;
	linked = false;
	return true;
}


CGSShader* CGSMesh::getShaderAttached( const ShaderType& _type ) const
{
	if( _type == ShaderType::VERTEX )
	{
		return vertexShader;
	}
	else if( _type == ShaderType::FRAGMENT )
	{
		return fragmentShader;
	}
	else if( _type == ShaderType::GEOMETRY )
	{
		return geometryShader;
	}
	else if( _type == ShaderType::TESSELLATION_EVALUATION )
	{
		return NULL;
	}
	else // _type == ShaderType::TESSELLATION_CONTROL
	{
		return NULL;
	}
}
void CGSMesh::bindAttributeLocation( const GLuint& _bindingIndex, const String& _variableName )
{
	bindAttributeLocation( _bindingIndex, _variableName.c_str( ) );
}

void CGSMesh::bindAttributeLocation( const GLuint& _bindingIndex, const GLchar* const& _variableName )
{
	glBindAttribLocation( programHandle, _bindingIndex, _variableName );
	linked = false;
}

bool CGSMesh::_linkProgram( )
{
	if( linked )
		return true;
	
	// Must have vertex + fragment, which will be default, at least, unless something went wrong.
	assert( vertexShader );
	assert( fragmentShader );
	assert( vertexShader->isCompiled( ) );
	assert( fragmentShader->isCompiled( ) );
	
	glAttachShader( programHandle, vertexShader->_getShaderHandle( ) );
	glAttachShader( programHandle, fragmentShader->_getShaderHandle( ) );
	
	if( geometryShader )
	{
		assert( geometryShader->isCompiled( ) );
		
		glAttachShader( programHandle, geometryShader->_getShaderHandle( ) );
	}
	
	glLinkProgram( programHandle );
	
	// This is mostly an unfortunate copy-paste of the same thing for building the shader in
	// CGSShader, since OpenGL isn't the most straight-forward to deal with.
	
	GLint status;
	glGetProgramiv( programHandle, GL_LINK_STATUS, &status );
	
	if( status == GL_TRUE )
	{
		linked = true;
		glUseProgram( 0 );
		return true;
	}
	else
	{
		linked = false; // Unlike shaders, I allow re-linking programs, so you might break them.
		
		GLint logLength;
		glGetProgramiv( programHandle, GL_INFO_LOG_LENGTH, &logLength );
		
		if( logLength )
		{
			// Failed with an error message
			U::log( "Error: CGSMesh::_linkProgram( ) failed. OpenGL failed to link the program with the following error:" );
			
			// This might be a fragmentation issue at some point.
			char* message = new char[logLength];
			glGetProgramInfoLog( programHandle, logLength, NULL, message );
			U::log( message );
			delete[] message;
		}
		else
		{
			// Failed, with no error message
			U::log( "Error: CGSMesh::_linkProgram( ) failed. OpenGL failed to build the program with no error message." );
		}
		
		return false;
	}
}

// TEXTURE FUNCTIONS =============================================================

void CGSMesh::attachTexture( const uint8_t& _unit, CGSTexture* const& _tex )
{
	if( _unit >= 80 )
	{
		U::log( "Warning: Texture unit ", _unit, " is above the OpenGL standard's minimum maximum texture units. This means such bindings are implementation-dependent, and strongly advised against." );
	}
	
	// If a link already exists at _unit, it is implicitly unlinked. So this is
	// safe even though it looks iffy.
	textures[ _unit ].link( _tex, _unit, this );
}

bool CGSMesh::detachTexture( const uint8_t& _unit )
{
	auto i = textures.find( _unit );
	
	if( i != textures.end() )
	{
		// According to my specification, this only returns true if a texture was
		// LINKED. A connector merely existing does not count, it must be valid.
		bool r = i->second.isValid( );
		textures.erase( i );
		return r;
	}
	
	return false;
}

CGSTexture* CGSMesh::getAttachedTexture( const uint8_t& _unit )
{
	auto i = textures.find( _unit );
	
	if( i != textures.end() )
	{
		// Will be NULL if not linked, so can return directly.
		return i->second.getTexture( );
	}
	
	return NULL;
}