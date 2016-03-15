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

#ifndef CGSSHADER_H
#define CGSSHADER_H

#include "CGS.h"

class CGSShader
{
public:
	CGSShader( const ShaderType& _type );
	virtual ~CGSShader( );
	
	inline const ShaderType& getType( ) const { return type; }
	inline const GLuint& _getShaderHandle( ) const { return shaderHandle; }
	inline const bool& isCompiled( ) const { return compiled; }
	
	// Returns true on successful compilation, false on failure.
	//
	// Source must be a valid GLSL program of the type specified when constructing this object.
	// 
	// Calling this on an already-built CGSShader is an error. However, this may be called again on
	// the same CGSShader if compilation fails.
	bool build( const String& source );
	
protected:
	ShaderType type;
	GLuint shaderHandle;
	bool compiled;
};

#endif	/* CGSSHADER_H */

