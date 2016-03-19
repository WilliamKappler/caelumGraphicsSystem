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

// TODO:
// * Add the ability to write to only specific locations in a stream (ie, don't
// require writing the whole stream to beginning to end; allow the internal 
// pointer to be overridden, like with indexes).
// * Only re-upload data streams that have been changed?
// * In the future, there will only be one stream, one stream to rule them all,
// which is broken up into virtual streams. Each mesh will just get a block of 
// that stream to work with. Ogre2 does this. However, that may or may not take 
// forever to implement, especially since performance with this system is likely 
// to already be substantially better than Ogre1, enough that the fragment shaders
// are likely going to be the bottleneck well beyond everything else involved.
// * Ability to non-destructively resize the stream. Perhaps silently.

// Uncomment this to prevent errors from being reported by calls to the
// programUniform*( ) functions which attempt to set locations that do not exist.
// Unlike most CGS directives, this can actually decrease performance. However,
// leaving it enabled is probably preferable, as CGS considers attempting to set
// non-existent uniforms to be acceptable behavior. As such, commenting this out
// might cause acceptable behavior to issue error messages.
#define SUPPRESS_MISSING_UNIFORM_ERRORS

#ifndef MESH_H
#define MESH_H

#include "CGS.h"

class CGSMesh
{
public:
	// You don't have to use these bindings unless you're using a default shader
	// provided by this rendering system. However, if you choose to use different
	// bindings, know that bad things could happen if a default shader is tossed
	// into the render pipeline somehow. For the most part, there's no reason
	// to not use these, unless you run out and need to use one that was unused.
	//
	// If you use default shaders, note that all of the default binding points are
	// intended for some form of floating point type. That does not include double
	// precision floats (GLSL requires those to be named as such). Float, half
	// float, and normalized integers are valid. Any other type could result in
	// unexpected behavior or crashes.
	static const GLuint VERTEX_BINDING_POINT_POSITION; // = 0
	static const GLuint VERTEX_BINDING_POINT_UVW; // = 1
	static const GLuint VERTEX_BINDING_POINT_COLOR; // = 2
	static const GLuint VERTEX_BINDING_POINT_NORMAL; // = 3
	static const GLuint VERTEX_BINDING_POINT_FIRST_USER; // = 4
	// Further user-defined should be ( VERTEX_BINDING_POINT_FIRST_USER + n )

	CGSMesh( const GLenum& _renderOperation = GL_POINTS );
	~CGSMesh( );

	// Get the graphics system ID - use this to refer to the object, not the GLID.
	inline uint32_t getID( ) const { return id; }

	// Sets the render operation to use when calling glDrawArrays( ). See the docs
	// of that for details. This does NOT modify the stream, so changing it has no
	// overhead. Default is GL_POINTS.
	// https://www.opengl.org/sdk/docs/man/html/glDrawArrays.xhtml
	void setRenderOperation( const GLenum& mode );

	inline void setVisibility( const bool& _visibility ) { visible = _visibility; }
	inline bool getVisibility( ) { return visible; }
	
	// VERTEX BUFFER OBJECT + ATTRIBUTES FUNCTIONS ===============================

	// Declare a vertex attribute to attach to the mesh - including position 0, 
	// the vertexes themselves. This invalidates the stream and 
	// generateDataStream( ) must be called after this.
	// 
	// The used attributeIndex's do not have to be sequential, but they do have to 
	// line up with your shader input declarations. Shader input without an  
	// associated vertex attribute defined in the mesh  will be populated with
	// uninitialized data.
	// 
	// The type given must be a valid OpenGL type enum value. You should then use
	// an EXACT equivalent to that value to pass your vertex data. Failing to do
	// so is an error. Here is a list of equivalent types (as well as supported
	// OpenGL type enums):
	// * GL_FLOAT = float
	// * GL_HALF_FLOAT = float (only type implicitly cast)
	// * GL_DOUBLE = double
	// * GL_BYTE = int8_t
	// * GL_UNSIGNED_BYTE = uint8_t
	// * GL_SHORT = int16_t
	// * GL_UNSIGNED_SHORT = uint16_t
	// * GL_INT = int32_t
	// * GL_UNSIGNED_INT = uint32_t
	// 
	// Comments on other arguments:
	// * integerType indicates the value is to be passed to OpenGL as an integer,
	// not a float.
	// * normalize can be used to convert an integer to a float on a range of
	// -1/0 to +1. The minimum being -1 or 0 depends on signing.
	void createVertexAttribute(
			const GLuint& attributeIndex, // VERTEX_BINDING_POINT_*
			const GLenum& type, // Attribute type (GL_FLOAT, GL_HALF_FLOAT, etc)
			const GLint& numberOfElements, // How many of TYPE per entry; 1-4
			const bool& integerType = false, // Call glVertexAttrib(I)Pointer
			const GLboolean& normalize = false // Normalize integer values
			);

	// Deletes the vertex attribute associated with attributeIndex. Returns true  
	// if an attribute with that index existed and was deleted; returns false if 
	// the index is not found. If true is returned, the stream is invalidated and 
	// generateDataStream( ) must be called.
	bool deleteVertexAttribute( const GLuint& attributeIndex );
	
	// Construct a new data stream for upload to the video card. Must be called
	// after any call to createVertexAttribute( ). Deletes current data stored in 
	// the mesh. You probably only want to call this when creating the mesh.
	//
	// Length determines the number of points which will be created in the data
	// stream. Each vertex attribute will have (length*numberOfElements) entries;
	// for example, if length is 10 and you are writing to the position attribute
	// (which has 3 parameters most of the time), you will have 30 entries, in
	// the pattern of [x1 y1 z1 x2 y2 z2...x9 y9 z9 x10 y10 z10].
	void generateDataStream( const uint16_t& length );

	// Opens a vertex attribute for writing via writeToA( ). Returns true if the 
	// attribute with attributeIndex is found and opened, false if it is not 
	// found. Default position is the beginning of the data stream, vertex 0.
	bool openAttribute( const GLuint& attributeIndex );

	// Writes to the currently open vertex attribute in the current location, and 
	// automatically increments the internal pointer. Different declarations are 
	// for different attribute data types. You should pass the matching type to
	// ensure there are no errors.
	//
	// Note that you must pass single-precision "float" for half-float, as C/C++
	// does not support this datatype natively. If the attribute is half-float,
	// the value will be down-converted to the right size automatically. This is
	// NOT true for doubles to floats, doubles to halves, or any other type, such
	// as down-conversion between ints.

	// GL_FLOAT + GL_HALF_FLOAT
	void writeToA( const float& d );
	// GL_DOUBLE
	void writeToA( const double& d );
	// GL_BYTE
	void writeToA( const int8_t& d );
	// GL_UNSIGNED_BYTE
	void writeToA( const uint8_t& d );
	// GL_SHORT
	void writeToA( const int16_t& d );
	// GL_UNSIGNED_SHORT
	void writeToA( const uint16_t& d );
	// GL_INT
	void writeToA( const int32_t& d );
	// GL_UNSIGNED_INT
	void writeToA( const uint32_t& d );
	// Fixed?

	// Closes the currently open vertex attribute. Implicitly called by 
	// openAttribute( ) if a vertex attribute is already open for writing.
	void closeAttribute( );

	// INDEX BUFFER FUNCTION =====================================================
	
	// Creates a buffer to store indexes. This does not invalidate or modify the
	// steam, as the indexes are stored separately.
	// 
	// If this is not called, each vertex will be rendered individually according
	// to the rendering operation. If this is specified, you can re-order vertexes
	// or call them repeatedly. This is suggested for more complex meshes or for
	// more complex render operations, but can be omitted for simple meshes at no
	// cost. In fact, if each vertex is only rendered once and in the order given,
	// using indexes would likely add overhead.
	//
	// Setting preallocate will reserve that number of indexes. If you know before
	// hand how many indexes you need, this will improve performance, but is not
	// required, and you may exceed it without issue.
	void createIndexBuffer( const uint16_t& preallocate = 0 );
	
	// Writes to the index buffer at the next sequential position according to the
	// internal pointer and increments the internal pointer. If "at" is set, write
	// to that position of the index buffer (with "at", the internal pointer is
	// not incremented).
	//
	// WARNING: Using "at" can allow non-sequential writing. That is to say, you
	// could write to positions 0, 1, 2, 5. This would leave indexes 3 and 4
	// undefined, filled with junk data, which will cause OpenGL to flip out.
	// Use "at" with caution.
	void writeToI( const uint32_t& d, const uint16_t& at );
	void writeToI( const uint32_t& d );
	
	// Moves the internal pointer to newPos, so that the next write via writeToI 
	// will be at newPos. The same warning applies to this function as does using
	// "at" in writeToI( ). However, moveToI( 0 ) is a safe way to return to the
	// start of the index buffer.
	bool moveToI( const uint16_t& newPos );
	
	// This function will change the size of the index buffer.
	// * If newLength is greater than the current size, more memory will be
	// allocated to the AssocArray, but no changes will be made to the data.
	// * If newLength is less than the current index buffer size, elements beyond
	// [newLength - 1] will be deleted.
	// * If newLength is the same as the current size, nothing is done.
	void resizeIndexBuffer( const uint16_t& newLength );
	
	// Returns the current number of indexes in the index buffer.
	uint16_t getIndexBufferSize( );
	
	// Clears the index buffer, but leaves it attached to the vertex array.
	void clearIndexBuffer( );
	
	// Clears the index buffer and detaches it from the vertex array. After doing
	// this, vertexes will again be rendered individually, as if no index array
	// was ever defined.
	void deleteIndexBuffer( );
	
	// INTERNAL MESH FUNCTIONS ===================================================
	
	const GLuint& _getVAOHandle( ) { return vaoHandle; }
	
	// Implicitly called by render( ), unless called earlier. Should be called
	// before rendering on all meshes, as calling implicitly causes latency while
	// rendering.
	void _update( );
	
	// Binds/un and draws with attached assets (programs, etc.).
	void _render( );
	
	// PROGRAM FUNCTIONS =========================================================
	// If forceLink is true, the returned handle will be linked and up to date.
	// Otherwise, there is no guarantee that it is.
	const GLuint& _getProgramHandle( const bool& forceLink = false );
	
	// These will, with the help of GraphicsSystem, attempt to get a copy of the
	// shader specified in _fileName. Note that all shader paths are searched for
	// this name, and that the engine default extension will be added to each 
	// file. So, if you would like to, for example, load:
	// %PROGRAM%/shaders/glass.vert.glsl
	// You should pass "glass" to this function, and ensure "shaders/" is added to
	// your shader search paths. If you have duplicate file names, it is undefined
	// which you will be given.
	//
	// Returns true on successful attachment of the shader, false on failure.
	// 
	// A default vertex and fragment shader which does very little will be 
	// automatically attached to all programs. Attaching a proper vertex/fragment
	// shader will replace these.
	bool loadVertexShader( const String& _fileName );
	bool loadFragmentShader( const String& _fileName );
	bool loadGeometryShader( const String& _fileName );
	
	// Alternative to the above, if you have a pre-existing shader you'd like to
	// pass.
	bool attachVertexShader( CGSShader* const& _shader );
	bool attachFragmentShader( CGSShader* const& _shader );
	bool attachGeometryShader( CGSShader* const& _shader );
	
	// Returns a pointer to the shader of type _type used by this class, or NULL
	// if none is in use.
	// 
	// The return value of this function might not be what you expect for two
	// reasons:
	//   1. This will return default vertex and fragment shaders if they are in
	// use; as such, this should never return NULL for types ShaderType::VERTEX or
	// ShaderType::FRAGMENT. The defaults will be in use if you did not set a
	// vertex or fragment shader.
	//   2. This will return the last shader of type _type attached to this
	// mesh; this might not, however, be the shader currently LINKED into the 
	// OpenGL backend. Linking is usually done immediately before rendering, but
	// can be invoked by calling _update( ).
	CGSShader* getShaderAttached( const ShaderType& _type ) const;
	
	// While this is provided, I strongly suggest using layout(location = N)
	// within GLSL instead. It's just much simpler and probably a lot more in line
	// with what you would want to do in real life with a shader/program.
	//
	// The program must be re-linked for this binding to take effect. It can be
	// called before attaching shaders.
	void bindAttributeLocation( const GLuint& _bindingIndex, const String& _variableName );
	void bindAttributeLocation( const GLuint& _bindingIndex, const GLchar* const& _variableName );
	
	// Links the specified shaders into a GLSL program. If successful, this object
	// will then be able to take uniform and attribute bindings. Does nothing and
	// returns true if no link is necessary. Returns true on program state being
	// successfully linked, false on the program failing to link. CGS has no
	// failure case for the failure to link shaders together - it is assumed that
	// all shader errors will be caught during compilation, so a failed link will
	// likely crash CGS.
	// 
	// You may change the shader used by a program, but the program must re-linked
	// for it to have any effect. Be warned: re-linking will DELETE all uniform
	// values. As such, you should likely not modify the shaders attached to a 
	// live mesh, which INCLUDES calling bindAttributeLocation( ), unless you know
	// exactly what you are doing. Note that bindAttributeLocation( ) is usually
	// not necessary to add attributes and it is never called implicitly by CGS,
	// so adding new vertex attributes will not itself trigger a re-link.
	//
	// This is implicitly called by numerous operations, generally those which
	// expect a valid and linked program to be attached to the mesh, but it may be 
	// called explicitly by the user if necessary for some reason.
	// 
	// If textures are attached to this mesh when a link is initiated, all 
	// attached textures will be re-attached.
	bool _linkProgram( );
	
	// PROGRAM UNIFORM FUNCTIONS =================================================
	
	// These should always be used to modify the uniforms of a mesh program. Using
	// these in place of direct calls to glProgramUniform*( ) or (worse)
	// glUseProgram( ) && glUniform*( ) ensures that uniforms are written to the
	// shaders you expect and that there are no unnecessary state changes (meaning
	// overhead). While CGS occasionally violates this convention, users of CGS
	// should not.
	//
	// These accept either an integer value (referring to a specified uniform
	// location) or a string (referring to a uniform name). Locations are
	// generally more efficient and should be assigned and used if you plan on
	// often writing to a given uniform. However, names are supported due to their
	// much greater flexibility.
	//
	// No checks are done on the validity of these calls (such as that the uniform
	// exists). However, OpenGL fails fairly gracefully on mismatches.
	//
	// Note: these are implemented in CGSProgramUniforms.cpp, not CGSMesh.cpp.
	
	void programUniform1f( const GLint& location,
			const GLfloat& v0 );
	void programUniform2f( const GLint& location,
			const GLfloat& v0, const GLfloat& v1 );
	void programUniform3f( const GLint& location,
			const GLfloat& v0, const GLfloat& v1, const GLfloat& v2 );
	void programUniform4f( const GLint& location,
			const GLfloat& v0, const GLfloat& v1, const GLfloat& v2, const GLfloat& v3 );
	
	void programUniform1f( const String& name,
			const GLfloat& v0 );
	void programUniform2f( const String& name,
			const GLfloat& v0, const GLfloat& v1 );
	void programUniform3f( const String& name,
			const GLfloat& v0, const GLfloat& v1, const GLfloat& v2 );
	void programUniform4f( const String& name,
			const GLfloat& v0, const GLfloat& v1, const GLfloat& v2, const GLfloat& v3 );
	
	void programUniform1i( const GLint& location,
			const GLint& v0 );
	void programUniform2i( const GLint& location,
			const GLint& v0, const GLint& v1 );
	void programUniform3i( const GLint& location,
			const GLint& v0, const GLint& v1, const GLint& v2 );
	void programUniform4i( const GLint& location,
			const GLint& v0, const GLint& v1, const GLint& v2, const GLint& v3 );
	
	void programUniform1i( const String& name,
			const GLint& v0 );
	void programUniform2i( const String& name,
			const GLint& v0, const GLint& v1 );
	void programUniform3i( const String& name,
			const GLint& v0, const GLint& v1, const GLint& v2 );
	void programUniform4i( const String& name,
			const GLint& v0, const GLint& v1, const GLint& v2, const GLint& v3 );
	
	void programUniform1ui( const GLuint& location,
			const GLuint& v0 );
	void programUniform2ui( const GLuint& location,
			const GLuint& v0, const GLuint& v1 );
	void programUniform3ui( const GLuint& location,
			const GLuint& v0, const GLuint& v1, const GLuint& v2 );
	void programUniform4ui( const GLuint& location,
			const GLuint& v0, const GLuint& v1, const GLuint& v2, const GLuint& v3 );
	
	void programUniform1ui( const String& name,
			const GLuint& v0 );
	void programUniform2ui( const String& name,
			const GLuint& v0, const GLuint& v1 );
	void programUniform3ui( const String& name,
			const GLuint& v0, const GLuint& v1, const GLuint& v2 );
	void programUniform4ui( const String& name,
			const GLuint& v0, const GLuint& v1, const GLuint& v2, const GLuint& v3 );
	
	// CGS Vector uniform setting functions.
	void programUniform2fv( const GLuint& location, const vec2& vector );
	void programUniform3fv( const GLuint& location, const vec3& vector );
	void programUniform4fv( const GLuint& location, const vec4& vector );
	
	void programUniform2fv( const String& name, const vec2& vector );
	void programUniform3fv( const String& name, const vec3& vector );
	void programUniform4fv( const String& name, const vec4& vector );
	
	void programUniform2iv( const GLuint& location, const ivec2& vector );
	void programUniform3iv( const GLuint& location, const ivec3& vector );
	void programUniform4iv( const GLuint& location, const ivec4& vector );
	
	void programUniform2iv( const String& name, const ivec2& vector );
	void programUniform3iv( const String& name, const ivec3& vector );
	void programUniform4iv( const String& name, const ivec4& vector );
	
	void programUniform2uiv( const GLuint& location, const uvec2& vector );
	void programUniform3uiv( const GLuint& location, const uvec3& vector );
	void programUniform4uiv( const GLuint& location, const uvec4& vector );
	
	void programUniform2uiv( const String& name, const uvec2& vector );
	void programUniform3uiv( const String& name, const uvec3& vector );
	void programUniform4uiv( const String& name, const uvec4& vector );
	
	// Raw vector uniform setting functions.
	//
	// These are presented primarily for completeness. numOfVectors is the count
	// parameter for vector uniform calls. This refers to the number of VECTORS
	// and NOT the number of parameters. It should be left set to 1 in almost all
	// cases.
	
	void programUniform1fv( const GLuint& location,
			GLfloat* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform2fv( const GLuint& location,
			GLfloat* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform3fv( const GLuint& location,
			GLfloat* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform4fv( const GLuint& location,
			GLfloat* const& vector,
			const GLsizei& numOfVectors = 1 );
	
	void programUniform1fv( const String& name,
			GLfloat* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform2fv( const String& name,
			GLfloat* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform3fv( const String& name,
			GLfloat* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform4fv( const String& name,
			GLfloat* const& vector,
			const GLsizei& numOfVectors = 1 );
	
	void programUniform1iv( const GLuint& location,
			GLint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform2iv( const GLuint& location,
			GLint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform3iv( const GLuint& location,
			GLint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform4iv( const GLuint& location,
			GLint* const& vector,
			const GLsizei& numOfVectors = 1 );
	
	void programUniform1iv( const String& name,
			GLint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform2iv( const String& name,
			GLint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform3iv( const String& name,
			GLint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform4iv( const String& name,
			GLint* const& vector,
			const GLsizei& numOfVectors = 1 );
	
	void programUniform1uiv( const GLuint& location,
			GLuint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform2uiv( const GLuint& location,
			GLuint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform3uiv( const GLuint& location,
			GLuint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform4uiv( const GLuint& location,
			GLuint* const& vector,
			const GLsizei& numOfVectors = 1 );
	
	void programUniform1uiv( const String& name,
			GLuint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform2uiv( const String& name,
			GLuint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform3uiv( const String& name,
			GLuint* const& vector,
			const GLsizei& numOfVectors = 1 );
	void programUniform4uiv( const String& name,
			GLuint* const& vector,
			const GLsizei& numOfVectors = 1 );
	
	// The matrix types will be implemented later, when I create CGS matrix types.
	
	// TEXTURE FUNCTIONS =========================================================
	
	// Attach a texture specified in _tex to the texture unit _unit.
	// 
	// For most usages, _unit should be below 16 (the minimum max textures
	// required per-stage in OpenGL), and definitely should be below 80 (the
	// minimum max total number of textures active at once). Using >= 80
	// will trigger a warning. Using more than 16 is up to you to manage correctly
	// and could lead to platform-specific and/or undefined behavior.
	void attachTexture( const uint8_t& _unit, CGSTexture* const& _tex );
	
	// Remove a texture binding at the texture unit _unit. Returns true if a
	// texture was bound to _unit, false if one was not.
	bool detachTexture( const uint8_t& _unit );
	
	// Returns the texture bound to the texture unit _unit, or NULL if no texture
	// has been bound.
	CGSTexture* getAttachedTexture( const uint8_t& _unit );
	
protected:
	// GENERAL VARIABLES + PROTECTED TYPES =======================================
	struct VertexAttributeData
	{
		// the attr index is the AssocArray key, so not stored here.
		GLenum type;
		GLint numberOfElements;
		bool useInterger;
		GLboolean normalize;

		uint16_t length; // Size, in bytes, this attribute occupies
		
		uint16_t streamPointerOffset; // Where in the stride the attr data starts, in bytes
	};

	static uint32_t nextMeshObjectID; // = 1;
	uint32_t id;
	
	// MESH VARIABLES ============================================================
	GLuint vaoHandle; // Handle for the mesh's vao in OpenGL
	GLuint vertexDataBufferHandle;
	GLenum renderOperation; // Operation passed to glDrawArrays( )
	
	bool visible; // If true, draws when _render( ) is called
	bool steamIsValid; // If false, generateDataStream( ) is needed
	bool steamUpdated; // If true, the stream will be uploaded before binding
	
	GLuint calculatedStreamStride; // Length of vertex data blocks (vertexes)

	GLuint openAttributeIndex; // Index of the attr in use, -1 if none.
	uint16_t openAttributePosition; // The "Internal pointer" starting at 0.
	GLenum openAttributeType; // A copy of "type" for the open attr [optimization]
	GLint openAttributeNumberOfElements; // Copy of numberOfElements [optimization]
	uint8_t openAttributeStride; // The spacing BETWEEN the open elements [optimization]
	uint16_t openAttributeOffset; // Where in the stream the data begins [optimization]
	
	// Attribute definitions
	// < bound attr index, vertex attr data >
	AssocArray< GLuint, VertexAttributeData > attributeDefinitions;

	// Raw bytes of data to stream to the GPU - needs cast for any operations
	// NULL if no stream is allocated. Number of chars is equal to:
	// calculatedStreamStride * streamLength
	uint8_t* stream;
	uint16_t streamLength; // Number of elements available in the stream
	
	// Index data, if used
	bool useIndexes;
	bool indexesUpdated;
	GLuint indexBufferHandle;
	Array< uint32_t > indexData;
	uint16_t indexPosition;
	
	// TEXTURE VARIABLES =========================================================
	// Be careful with this container! Note that the class will automatically make
	// and delete CGSMeshToTextureAdapter objects, since they are not stored as
	// a pointer. This is to ease cleanup, but requires careful calling
	// conventions in order to not create or destroy objects incorrectly.
	AssocArray< uint8_t, CGSMeshToTextureAdapter > textures;
	
	// PROTECTED MESH FUNCTIONS ==================================================
	
	// Uses the variables for the currently open attribute
	void* _getDataLocationPointer( );
	
	// Checks that a valid attr (!= -1) is open and the types match
	bool _checkOpenAttribute( const GLenum& _functionType );
	
	// Back-end for writeToA( )
	void _writeToABackend( const void* const& dP, const GLenum& _type );
	
	// PROGRAM VARIABLES =========================================================
	
	GLuint programHandle;
	
	bool linked;
	
	CGSShader* vertexShader;
	CGSShader* fragmentShader;
	CGSShader* geometryShader;
};

#endif	/* MESH_H */

