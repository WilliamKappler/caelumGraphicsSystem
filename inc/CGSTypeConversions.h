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

#ifndef CGSTYPECONVERSIONS_H
#define	CGSTYPECONVERSIONS_H

#include "EngineTypes.h"

// This is used to adjust the floating point -> integer conversion to be more in
// line with what sane people expect. Normally, positive always rounds down, and
// negative always rounds up. This is used to ensure float conversions round to
// the NEAREST integer, rather than the closest integer in the direction of 0.
#define FLOAT_ADJUSTMENT 0.5

uint8_t floatToUNORM8( const float& d );
uint16_t floatToUNORM16( const float& d );
uint32_t floatToUNORM32( const float& d );

float unorm8ToFloat( const uint8_t& d );
float unorm16ToFloat( const uint16_t& d );
float unorm32ToFloat( const uint32_t& d );

int8_t floatToSNORM8( const float& d );
int16_t floatToSNORM16( const float& d );
int32_t floatToSNORM32( const float& d );

float snorm8ToFloat( const int8_t& d );
float snorm16ToFloat( const int16_t& d );
float snorm32ToFloat( const int32_t& d );

uint16_t floatToHalf( const float& d );
float halfToFloat( const uint16_t& d );

#endif	/* CGSTYPECONVERSIONS_H */

