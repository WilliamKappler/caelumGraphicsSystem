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

#include "CGSMeshToTextureAdapter.h"
#include "CGSTexture.h"
#include "CGSMesh.h"

CGSMeshToTextureAdapter::CGSMeshToTextureAdapter( )
{
	texture = NULL;
	mesh = NULL;
	unit = 0;
}

CGSMeshToTextureAdapter::~CGSMeshToTextureAdapter( )
{
	if( texture )
	{
		texture->_detachFromMesh( this );
	}
}

void CGSMeshToTextureAdapter::link(
	CGSTexture* const& _texture,
	const uint8_t& _unit,
	CGSMesh* const& _mesh )
{
	// If the link would not be modified, warn and return.
	if( texture == _texture && unit == _unit )
	{
		U::log( "Warning: Redundant call to CGSMeshToTextureAdapter::link() ignored." );
		return;
	}
	
	// If this object already describes a link, that link must be broken first.
	if( texture )
	{
		breakLink( );
	}
	
	// If a null was passed, just return.
	if( !_texture )
	{
		return;
	}
	
	// Set the link variables, which describe how the texture will attach to the
	// mesh.
	texture = _texture;
	mesh = _mesh;
	unit = _unit;
	
	// Must call after setting the above, because the texture will set parameters
	// on the mesh's program (range uniforms).
	_texture->_attachToMesh( this );
}

void CGSMeshToTextureAdapter::breakLink( )
{
	if( texture )
	{
		texture->_detachFromMesh( this );
		// texture is NULL'd by callback to _invalidate( ) in _detachFromMesh().
	}
}

void CGSMeshToTextureAdapter::bind( )
{
	texture->_bind( unit );
}