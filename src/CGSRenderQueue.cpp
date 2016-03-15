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

const uint8_t CGSRenderStage::DEFAULT_BACK_BUFFER_TEXTURE_UNIT = 7;

void CGSRenderStage::insertOperation(
		const CGSRenderOrderingType& _position,
		CGSMesh* const& _mesh,
		const bool& _pullBackbuffer )
{
	operations.insert( U::p( _position, CGSRenderOperation( _mesh, _pullBackbuffer ) ) );
}


CGSMesh* GraphicsSystem::createCompositorMesh( const String& fragmentShader )
{
	U::log( "createCompositorMesh( ) not implemented." );
	return NULL;
}

CGSRenderStage* GraphicsSystem::getRenderStage( const uint8_t& index )
{
	return &renderStages[ index ];
}

bool GraphicsSystem::removeRenderStage( const uint8_t& index )
{
	return renderStages.erase( index );
}

void _copyToBackbuffer(
		const uint8_t& unit,
		const GLuint& framebufferHandle,
		const GLuint& backbufferTextureHandle,
		const uint16_t& screenX,
		const uint16_t& screenY )
{
	glActiveTexture( GL_TEXTURE0 + unit );
	glBindTexture( GL_TEXTURE_2D, backbufferTextureHandle );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, framebufferHandle );
	glCopyTexSubImage2D(
			GL_TEXTURE_2D,
			0, // level
			0, 0, // offset
			0, 0, // x, y
			screenX, screenY );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, framebufferHandle );
}

void GraphicsSystem::render( )
{
	assert( inititalized );
	
	// Perform CGS system updates, primarily to ensure the data uploaded to the
	// video card is up to date, and all linkages between objects are accurate.
	
	// Update of meshes must come first, in case uniforms need set from textures.
	for( AssocArray< uint32_t, CGSMesh* >::iterator i = meshes.begin( );
		i != meshes.end( ); ++i )
	{
		i->second->_update( );
	}
	
	for( auto i = textures.begin( ); i != textures.end( ); ++i )
	{
		(*i)->_update( );
	}
	
	// Prepare the render target framebuffer
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, framebufferHandle );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	// Now, perform the actual render operations.
	for( auto i = renderStages.begin( ); i != renderStages.end( ); ++i )
	{
		// On the initial stage, this will also clear the backbuffer.
		_copyToBackbuffer(
				i->second._getBackbufferTextureUnit( ),
				framebufferHandle,
				backbufferTextureHandle,
				screenX, screenY );
		
		AssocArrayMulti< CGSRenderOrderingType, CGSRenderOperation >& operations
				= i->second._getOperations( );
		for( auto j = operations.begin( ); j != operations.end( ); ++j )
		{
			if( j->second.pullBackbuffer )
			{
				_copyToBackbuffer(
						i->second._getBackbufferTextureUnit( ),
						framebufferHandle,
						backbufferTextureHandle,
						screenX, screenY );
			}
			
			j->second.mesh->_render( );
		}
		operations.clear( );
	}
	
	// Copy render target to the default framebuffer
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ); // Default framebuffer
	glBindFramebuffer( GL_READ_FRAMEBUFFER, framebufferHandle );
	glBlitFramebuffer(
			0, 0, screenX, screenY,
			0, 0, screenX, screenY,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
			GL_NEAREST );
	
	// Render the default framebuffer to the screen
	SDL_GL_SwapWindow( sdlWindow );
}
