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

#ifndef CGSMESHTOTEXTUREADAPTER_H
#define	CGSMESHTOTEXTUREADAPTER_H

#include "CGS.h"
#include "CGSMesh.h"

class CGSMeshToTextureAdapter
{
public:
	CGSMeshToTextureAdapter( );
	~CGSMeshToTextureAdapter( );
	
	inline uint8_t getTextureUnit( ) { return unit; }
	inline GLuint getProgramHandle( ) { return mesh->_getProgramHandle( true ); }
	
	inline bool isValid( ) { return texture; }
	inline CGSTexture* getTexture( ) { return texture; }
	
	// Create a link with the texture that _texture points to, bound to texture
	// unit _unit. If a link already exists, the current link is broken.
	void link(
		CGSTexture* const& _texture,
		const uint8_t& _unit,
		CGSMesh* const& _mesh );
	
	// Calls _detachFromMesh( ) on the attached texture (if one is attached), 
	// which triggers the texture removing this adapter from its adapter list and
	// then calls invalidate( ).
	void breakLink( );
	
	// Zeros the texture pointer. Should be called from CGSTexture only. Meshes
	// call breakLink( ) instead.
	inline void _invalidate( ) { texture = NULL; }
	
	// Binds the texture for rendering
	void bind( );
	
private:
	CGSTexture* texture;
	CGSMesh* mesh;
	uint8_t unit;
};

#endif	/* CGSMESHTOTEXTUREADAPTER_H */

