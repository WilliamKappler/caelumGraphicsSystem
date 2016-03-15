/*	Copyright (c) 2015 William Kappler

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE. */

#include "CGSShader.h"

CGSShader::CGSShader( const ShaderType& _type )
{
	type = _type;
	compiled = false;
	shaderHandle = glCreateShader( ( GLenum )( _type ) );
}

CGSShader::~CGSShader( ) 
{
	glDeleteShader( shaderHandle );
}

bool CGSShader::build( const String& source )
{
	if( compiled )
	{
		U::log( "Error: build( ) called on CGSShader object which has already been successfully built." );
		return false;
	}
	
	// This is required to take the address of the c_str, as required for glShaderSource( ).
	// It's not exactly safe to do this, but as long as the string is not modified, STD should not
	// be doing anything to the string.
	const char* sourceStringLocation = source.c_str( );
	glShaderSource( shaderHandle, 1, &sourceStringLocation, NULL );
	glCompileShader( shaderHandle );
	GLint status;
	glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &status );
	
	if( status == GL_TRUE )
	{
		compiled = true;
		return true;
	}
	else
	{
		GLint logLength;
		glGetShaderiv( shaderHandle, GL_INFO_LOG_LENGTH, &logLength );
		
		if( logLength )
		{
			// Failed with an error message
			U::log( "Error: CGSShader::build( ) failed. OpenGL failed to build the shader with the following error:" );
			
			// This might be a fragmentation issue at some point.
			char* message = new char[logLength];
			glGetShaderInfoLog( shaderHandle, logLength, NULL, message );
			U::log( message );
			delete[] message;
		}
		else
		{
			// Failed, with no error message
			U::log( "Error: CGSShader::build( ) failed. OpenGL failed to build the shader with no error message." );
			// They might have loaded an empty file.
			U::log( "- The length of the passed GLSL shader source is ", source.length( ), " characters." );
		}
		
		return false;
	}
}

