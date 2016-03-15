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

#include "CGSMesh.h"

#ifndef SUPPRESS_MISSING_UNIFORM_ERRORS
#define MISSING_UNIFORM_ERROR_SUPPRESSION( )
#else
#define MISSING_UNIFORM_ERROR_SUPPRESSION( ) glGetError( )
#endif

inline GLint _getUniformLocation( GLuint programHandle, const String& name )
{
	return glGetUniformLocation( programHandle, name.c_str( ) );
}

void CGSMesh::programUniform1f( const GLint& location,
			const GLfloat& v0 )
{
	glProgramUniform1f( _getProgramHandle( true ), location, v0 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform2f( const GLint& location,
			const GLfloat& v0, const GLfloat& v1 )
{
	glProgramUniform2f( _getProgramHandle( true ), location, v0, v1 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform3f( const GLint& location,
			const GLfloat& v0, const GLfloat& v1, const GLfloat& v2 )
{
	glProgramUniform3f( _getProgramHandle( true ), location, v0, v1, v2 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform4f( const GLint& location,
			const GLfloat& v0, const GLfloat& v1, const GLfloat& v2, const GLfloat& v3 )
{
	glProgramUniform4f( _getProgramHandle( true ), location, v0, v1, v2, v3 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

	
void CGSMesh::programUniform1f( const String& name,
			const GLfloat& v0 )
{
	// Only the call to _getUniformLocation( ) needs a fully linked program, so
	// that's why these arguments look a bit weird. Execution order in args is
	// a bit iffy.
	glProgramUniform1f( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0 );
}

void CGSMesh::programUniform2f( const String& name,
			const GLfloat& v0, const GLfloat& v1 )
{
	glProgramUniform2f( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0, v1 );
}

void CGSMesh::programUniform3f( const String& name,
			const GLfloat& v0, const GLfloat& v1, const GLfloat& v2 )
{
	glProgramUniform3f( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0, v1, v2 );
}

void CGSMesh::programUniform4f( const String& name,
			const GLfloat& v0, const GLfloat& v1, const GLfloat& v2, const GLfloat& v3 )
{
	glProgramUniform4f( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0, v1, v2, v3 );
}

	
void CGSMesh::programUniform1i( const GLint& location,
			const GLint& v0 )
{
	glProgramUniform1i( _getProgramHandle( true ), location, v0 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform2i( const GLint& location,
			const GLint& v0, const GLint& v1 )
{
	glProgramUniform2i( _getProgramHandle( true ), location, v0, v1 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform3i( const GLint& location,
			const GLint& v0, const GLint& v1, const GLint& v2 )
{
	glProgramUniform3i( _getProgramHandle( true ), location, v0, v1, v2 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform4i( const GLint& location,
			const GLint& v0, const GLint& v1, const GLint& v2, const GLint& v3 )
{
	glProgramUniform4i( _getProgramHandle( true ), location, v0, v1, v2, v3 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

	
void CGSMesh::programUniform1i( const String& name,
			const GLint& v0 )
{
	glProgramUniform1i( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0 );
}

void CGSMesh::programUniform2i( const String& name,
			const GLint& v0, const GLint& v1 )
{
	glProgramUniform2i( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0, v1 );
}

void CGSMesh::programUniform3i( const String& name,
			const GLint& v0, const GLint& v1, const GLint& v2 )
{
	glProgramUniform3i( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0, v1, v2 );
}

void CGSMesh::programUniform4i( const String& name,
			const GLint& v0, const GLint& v1, const GLint& v2, const GLint& v3 )
{
	glProgramUniform4i( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0, v1, v2, v3 );
}

	
void CGSMesh::programUniform1ui( const GLuint& location,
			const GLuint& v0 )
{
	glProgramUniform1ui( _getProgramHandle( true ), location, v0 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform2ui( const GLuint& location,
			const GLuint& v0, const GLuint& v1 )
{
	glProgramUniform2ui( _getProgramHandle( true ), location, v0, v1 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform3ui( const GLuint& location,
			const GLuint& v0, const GLuint& v1, const GLuint& v2 )
{
	glProgramUniform3ui( _getProgramHandle( true ), location, v0, v1, v2 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform4ui( const GLuint& location,
			const GLuint& v0, const GLuint& v1, const GLuint& v2, const GLuint& v3 )
{
	glProgramUniform4ui( _getProgramHandle( true ), location, v0, v1, v2, v3 );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

	
void CGSMesh::programUniform1ui( const String& name,
			const GLuint& v0 )
{
	glProgramUniform1ui( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0 );
}

void CGSMesh::programUniform2ui( const String& name,
			const GLuint& v0, const GLuint& v1 )
{
	glProgramUniform2ui( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0, v1 );
}

void CGSMesh::programUniform3ui( const String& name,
			const GLuint& v0, const GLuint& v1, const GLuint& v2 )
{
	glProgramUniform3ui( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0, v1, v2 );
}

void CGSMesh::programUniform4ui( const String& name,
			const GLuint& v0, const GLuint& v1, const GLuint& v2, const GLuint& v3 )
{
	glProgramUniform4ui( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			v0, v1, v2, v3 );
}


void CGSMesh::programUniform2fv( const GLuint& location, const vec2& vector )
{
	glProgramUniform2fv( _getProgramHandle( true ), location, 1, vector.d );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform3fv( const GLuint& location, const vec3& vector )
{
	glProgramUniform3fv( _getProgramHandle( true ), location, 1, vector.d );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform4fv( const GLuint& location, const vec4& vector )
{
	glProgramUniform4fv( _getProgramHandle( true ), location, 1, vector.d );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform2fv( const String& name, const vec2& vector )
{
	glProgramUniform2fv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			1, vector.d );
}

void CGSMesh::programUniform3fv( const String& name, const vec3& vector )
{
	glProgramUniform3fv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			1, vector.d );
}

void CGSMesh::programUniform4fv( const String& name, const vec4& vector )
{
	glProgramUniform4fv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			1, vector.d );
}
	
void CGSMesh::programUniform2iv( const GLuint& location, const ivec2& vector )
{
	glProgramUniform2iv( _getProgramHandle( true ), location, 1, vector.d );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform3iv( const GLuint& location, const ivec3& vector )
{
	glProgramUniform3iv( _getProgramHandle( true ), location, 1, vector.d );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform4iv( const GLuint& location, const ivec4& vector )
{
	glProgramUniform4iv( _getProgramHandle( true ), location, 1, vector.d );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform2iv( const String& name, const ivec2& vector )
{
	glProgramUniform2iv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			1, vector.d );
}

void CGSMesh::programUniform3iv( const String& name, const ivec3& vector )
{
	glProgramUniform3iv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			1, vector.d );
}

void CGSMesh::programUniform4iv( const String& name, const ivec4& vector )
{
	glProgramUniform4iv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			1, vector.d );
}
	
void CGSMesh::programUniform2uiv( const GLuint& location, const uvec2& vector )
{
	glProgramUniform2uiv( _getProgramHandle( true ), location, 1, vector.d );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform3uiv( const GLuint& location, const uvec3& vector )
{
	glProgramUniform3uiv( _getProgramHandle( true ), location, 1, vector.d );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform4uiv( const GLuint& location, const uvec4& vector )
{
	glProgramUniform4uiv( _getProgramHandle( true ), location, 1, vector.d );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform2uiv( const String& name, const uvec2& vector )
{
	glProgramUniform2uiv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			1, vector.d );
}

void CGSMesh::programUniform3uiv( const String& name, const uvec3& vector )
{
	glProgramUniform3uiv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			1, vector.d );
}

void CGSMesh::programUniform4uiv( const String& name, const uvec4& vector )
{
	glProgramUniform4uiv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			1, vector.d );
}


void CGSMesh::programUniform1fv( const GLuint& location,
			GLfloat* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform1fv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform2fv( const GLuint& location,
			GLfloat* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform2fv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform3fv( const GLuint& location,
			GLfloat* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform3fv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform4fv( const GLuint& location,
			GLfloat* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform4fv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

	
void CGSMesh::programUniform1fv( const String& name,
			GLfloat* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform1fv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

void CGSMesh::programUniform2fv( const String& name,
			GLfloat* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform2fv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

void CGSMesh::programUniform3fv( const String& name,
			GLfloat* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform3fv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

void CGSMesh::programUniform4fv( const String& name,
			GLfloat* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform4fv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

	
void CGSMesh::programUniform1iv( const GLuint& location,
			GLint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform1iv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform2iv( const GLuint& location,
			GLint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform2iv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform3iv( const GLuint& location,
			GLint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform3iv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform4iv( const GLuint& location,
			GLint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform4iv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

	
void CGSMesh::programUniform1iv( const String& name,
			GLint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform1iv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

void CGSMesh::programUniform2iv( const String& name,
			GLint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform2iv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

void CGSMesh::programUniform3iv( const String& name,
			GLint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform3iv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

void CGSMesh::programUniform4iv( const String& name,
			GLint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform4iv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

	
void CGSMesh::programUniform1uiv( const GLuint& location,
			GLuint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform1uiv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform2uiv( const GLuint& location,
			GLuint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform2uiv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform3uiv( const GLuint& location,
			GLuint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform3uiv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

void CGSMesh::programUniform4uiv( const GLuint& location,
			GLuint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform4uiv( _getProgramHandle( true ), location, numOfVectors, vector );
	
	MISSING_UNIFORM_ERROR_SUPPRESSION( );
}

	
void CGSMesh::programUniform1uiv( const String& name,
			GLuint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform1uiv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

void CGSMesh::programUniform2uiv( const String& name,
			GLuint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform2uiv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

void CGSMesh::programUniform3uiv( const String& name,
			GLuint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform3uiv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}

void CGSMesh::programUniform4uiv( const String& name,
			GLuint* const& vector,
			const GLsizei& numOfVectors )
{
	glProgramUniform4uiv( programHandle,
			_getUniformLocation( _getProgramHandle( true ), name ),
			numOfVectors, vector );
}