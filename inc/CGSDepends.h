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

#ifndef CGSDEPENDS_H
#define	CGSDEPENDS_H

// OpenGL

#ifdef _WIN32
#include <windef.h> // Must come before glew
#include "GL/glew.h" // Must come before OpenGL (SDL)
//#include "glxew.h"
//#include "wglew.h"
#else
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#endif
#endif
// Linux support needs added. Should be similar to Windows for the most part.

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "SDL2/SDL_syswm.h"

// Standard

#include "iostream"
#include <stdint.h>
#include <map>
#include <list>
#include <string>
#include <bitset>
#include <utility>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <unordered_set>
#include <set>
#include <fstream>
#include <unordered_map>

typedef std::string String;

template <typename T>
using Array = std::vector<T>;

template <typename A, typename B>
using Pair = std::pair<A, B>;

template <typename T>
using LinkedList = std::list<T>;

template <typename A, typename B>
using AssocArray = std::map<A, B>;

template <typename A, typename B>
using UnorderedAssocArray = std::unordered_map<A, B>;

template <typename A, typename B>
using AssocArrayMulti = std::multimap<A, B>;

template <typename T>
using OrderedSet = std::set<T>;

template <typename T>
using UnorderedSet = std::unordered_set<T>;

// Boost
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

// Dependent libraries

#include "FreeImage.h"

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

#endif	/* CGSDEPENDS_H */

