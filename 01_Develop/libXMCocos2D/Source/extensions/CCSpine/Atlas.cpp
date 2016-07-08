/* -----------------------------------------------------------------------------------
 *
 *      File            Atlas.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2013      Esoteric Software. All rights reserved.
 *
 *         http://esotericsoftware.com/      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions are met:
 * 
 *     1. Redistributions of source code must retain the above copyright notice, this
 *        list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 * 
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *     ( INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND
 *     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *     ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCSpine/Atlas.h"
#include "extensions/CCSpine/Extension.h"

NS_CC_BEGIN


AtlasPage* AtlasPage_create ( const KDchar* name )
{
	AtlasPage*	 self = NEW ( AtlasPage );
	MALLOC_STR ( self->name, name );
	return self;
}

KDvoid AtlasPage_dispose ( AtlasPage* self ) 
{
	FREE ( self->name );
	_AtlasPage_disposeTexture ( self );
}



AtlasRegion* AtlasRegion_create ( KDvoid )
{
	return NEW ( AtlasRegion );
}

KDvoid AtlasRegion_dispose ( AtlasRegion* self ) 
{
	FREE ( self->name );
	FREE ( self->splits );
	FREE ( self->pads );
	FREE ( self );
}



typedef struct
{
	const KDchar*	begin;
	const KDchar*	end;
} Str;

static KDvoid trim ( Str* str ) 
{
	while ( kdIsspace ( *str->begin ) && str->begin < str->end )
	{
		 ( str->begin )++;
	}

	if ( str->begin == str->end ) 
	{
		return;
	}
	str->end--;
	while ( kdIsspace ( *str->end ) && str->end >= str->begin )
	{
		str->end--;
	}
	str->end++;
}

// Tokenize string without modification. Returns 0 on failure. 
static KDint readLine ( const KDchar* begin, const KDchar* end, Str* str ) 
{
	static const KDchar*	nextStart;
	if ( begin ) 
	{
		nextStart = begin;
		return 1;
	}
	
	if ( nextStart == end )
	{
		return 0;
	}
	str->begin = nextStart;

	// Find next delimiter. 
	while ( nextStart != end && *nextStart != '\n' )
	{
		nextStart++;
	}

	str->end = nextStart;
	trim ( str );

	if ( nextStart != end )
	{
		nextStart++;
	}
	return 1;
}

// Moves str->begin past the first occurence of c. Returns 0 on failure. 
static KDint beginPast ( Str* str, KDchar c ) 
{
	const KDchar*	begin = str->begin;
	while ( 1 ) 
	{
		KDchar	lastSkippedChar = *begin;
		if ( begin == str->end ) 
		{
			return 0;
		}
		begin++;

		if ( lastSkippedChar == c ) 
		{
			break;
		}
	}
	str->begin = begin;
	return 1;
}

// Returns 0 on failure.
static KDint readValue ( const KDchar* end, Str* str ) 
{
	readLine ( 0, end, str );
	if ( !beginPast ( str, ':' ) ) 
	{
		return 0;
	}
	trim ( str );
	return 1;
}

// Returns the number of tuple values read ( 2, 4, or 0 for failure ). 
static KDint readTuple ( const KDchar* end, Str tuple[] ) 
{
	Str		str;
	readLine ( 0, end, &str );
	if ( !beginPast ( &str, ':' ) ) 
	{
		return 0;
	}

	KDint i = 0;
	for ( i = 0; i < 3; ++i ) 
	{
		tuple[i].begin = str.begin;
		if ( !beginPast ( &str, ',' ) )
		{
			if ( i == 0 ) 
			{
				return 0;
			}
			break;
		}
		tuple[i].end = str.begin - 2;
		trim ( &tuple[i] );
	}
	tuple[i].begin = str.begin;
	tuple[i].end = str.end;
	trim ( &tuple[i] );
	return i + 1;
}

static KDchar* mallocString ( Str* str ) 
{
	KDint	length = str->end - str->begin;
	KDchar* string = MALLOC ( KDchar, length + 1 );
	kdMemcpy ( string, str->begin, length );
	string[length] = '\0';
	return string;
}

static KDint indexOf ( const KDchar** array_, KDint count, Str* str ) 
{
	KDint length = str->end - str->begin;
	KDint i;
	for ( i = count - 1; i >= 0; i-- )
	{
		if ( kdStrncmp ( array_[i], str->begin, length ) == 0 ) 
		{
			return i;
		}
	}
	return -1;
}

static KDint equals ( Str* str, const KDchar* other ) 
{
	return kdStrncmp ( other, str->begin, str->end - str->begin ) == 0;
}

static KDint toInt ( Str* str )
{
	return kdStrtol ( str->begin, (KDchar**) &str->end, 10 );
}

static Atlas* abortAtlas ( Atlas* self ) 
{
	Atlas_dispose ( self );
	return 0;
}

static const KDchar*	formatNames[] = { "Alpha", "Intensity", "LuminanceAlpha", "RGB565", "RGBA4444", "RGB888", "RGBA8888" };
static const KDchar*	textureFilterNames[] = { "Nearest", "Linear", "MipMap", "MipMapNearestNearest", "MipMapLinearNearest", "MipMapNearestLinear", "MipMapLinearLinear" };

Atlas* Atlas_readAtlas ( const KDchar* begin, KDint length, const KDchar* dir ) 
{
	const KDchar*	end = begin + length;
	KDint			dirLength  = kdStrlen ( dir );
	KDint			needsSlash = dirLength > 0 && dir[dirLength - 1] != '/' && dir[dirLength - 1] != '\\';

	Atlas*			self = NEW ( Atlas );

	AtlasPage*		page = 0;
	AtlasPage*		lastPage = 0;
	AtlasRegion*	lastRegion = 0;
	Str				str;
	Str				tuple[4];
	readLine ( begin, 0, 0 );
	while ( readLine ( 0, end, &str ) ) 
	{
		if ( str.end - str.begin == 0 ) 
		{
			page = 0;
		}
		else if ( !page ) 
		{
			KDchar*		name = mallocString ( &str );
			KDchar*		path = MALLOC ( KDchar, dirLength + needsSlash + kdStrlen ( name ) + 1 );
			kdMemcpy ( path, dir, dirLength );
			if ( needsSlash )
			{
				path[dirLength] = '/';
			}
			kdStrcpy ( path + dirLength + needsSlash, name );

			page = AtlasPage_create ( name );
			FREE ( name );
			if ( lastPage )
			{
				lastPage->next = page;
			}
			else
			{
				self->pages = page;
			}
			lastPage = page;

			if ( !readValue ( end, &str ) )
			{
				return abortAtlas ( self );
			}
			page->format = (AtlasFormat) indexOf ( formatNames, 7, &str );

			if ( !readTuple ( end, tuple ) )
			{
				return abortAtlas ( self );
			}
			page->minFilter = (AtlasFilter) indexOf ( textureFilterNames, 7, tuple );
			page->magFilter = (AtlasFilter) indexOf ( textureFilterNames, 7, tuple + 1 );

			if ( !readValue ( end, &str ) )
			{
				return abortAtlas ( self );
			}

			if ( !equals ( &str, "none" ) ) 
			{
				page->uWrap = *str.begin == 'x' ? ATLAS_REPEAT : ( *str.begin == 'y' ? ATLAS_CLAMPTOEDGE : ATLAS_REPEAT );
				page->vWrap = *str.begin == 'x' ? ATLAS_CLAMPTOEDGE : ( *str.begin == 'y' ? ATLAS_REPEAT : ATLAS_REPEAT );
			}

			_AtlasPage_createTexture ( page, path );
			FREE ( path );
		}
		else 
		{
			AtlasRegion*	region = AtlasRegion_create ( );
			if ( lastRegion )
			{
				lastRegion->next = region;
			}
			else
			{
				self->regions = region;
			}
			lastRegion = region;

			region->page = page;
			region->name = mallocString ( &str );

			if ( !readValue ( end, &str ) )
			{
				return abortAtlas ( self );
			}
			region->rotate = equals ( &str, "true" );

			if ( readTuple ( end, tuple ) != 2 )
			{
				return abortAtlas ( self );
			}
			region->x = toInt ( tuple );
			region->y = toInt ( tuple + 1 );

			if ( readTuple ( end, tuple ) != 2 ) 
			{
				return abortAtlas ( self );
			}
			region->width  = toInt ( tuple );
			region->height = toInt ( tuple + 1 );

			region->u = region->x / (KDfloat) page->width;
			region->v = region->y / (KDfloat) page->height;
			region->u2 = ( region->x + region->width  ) / (KDfloat) page->width;
			region->v2 = ( region->y + region->height ) / (KDfloat) page->height;

			KDint	 count;
			if ( ! ( count = readTuple ( end, tuple ) ) ) 
			{
				return abortAtlas ( self );
			}
			if ( count == 4 ) 
			{
				// split is optional 
				region->splits = MALLOC ( int, 4 );
				region->splits[0] = toInt ( tuple );
				region->splits[1] = toInt ( tuple + 1 );
				region->splits[2] = toInt ( tuple + 2 );
				region->splits[3] = toInt ( tuple + 3 );

				if ( ! ( count = readTuple ( end, tuple ) ) )
				{
					return abortAtlas ( self );
				}

				if ( count == 4 ) 
				{
					// pad is optional, but only present with splits 
					region->pads = MALLOC ( int, 4 );
					region->pads[0] = toInt ( tuple );
					region->pads[1] = toInt ( tuple + 1 );
					region->pads[2] = toInt ( tuple + 2 );
					region->pads[3] = toInt ( tuple + 3 );

					if ( !readTuple ( end, tuple ) )
					{
						return abortAtlas ( self );
					}
				}
			}

			region->originalWidth  = toInt ( tuple );
			region->originalHeight = toInt ( tuple + 1 );

			readTuple ( end, tuple );
			region->offsetX = (KDfloat) toInt ( tuple );
			region->offsetY = (KDfloat) toInt ( tuple + 1 );

			if ( !readValue ( end, &str ) ) 
			{
				return abortAtlas ( self );
			}
			region->index = toInt ( &str );
		}
	}

	return self;
}

Atlas* Atlas_readAtlasFile ( const KDchar* path ) 
{
	Atlas*	atlas = 0;

	// Get directory from atlas path. 
	const KDchar*	lastForwardSlash  = kdStrrchr ( path, '/' );
	const KDchar*	lastBackwardSlash = kdStrrchr ( path, '\\' );
	const KDchar*	lastSlash = lastForwardSlash > lastBackwardSlash ? lastForwardSlash : lastBackwardSlash;
	if ( lastSlash == path ) 
	{
		lastSlash++;	// Never drop starting slash. 
	}
	KDint	dirLength = lastSlash ? lastSlash - path : 0;
	KDchar* dir = MALLOC ( KDchar, dirLength + 1 );
	kdMemcpy ( dir, path, dirLength );
	dir[dirLength] = '\0';

	KDint			length;
	const KDchar*	data = _Util_readFile ( path, &length );
	if ( data ) 
	{
		atlas = Atlas_readAtlas ( data, length, dir );
	}

	FREE ( data );
	FREE ( dir );
	return atlas;
}

KDvoid Atlas_dispose ( Atlas* self ) 
{
	AtlasPage*	page = self->pages;
	while ( page )
	{
		AtlasPage*	nextPage = page->next;
		AtlasPage_dispose ( page );
		page = nextPage;
	}

	AtlasRegion*	region = self->regions;
	while ( region ) 
	{
		AtlasRegion* nextRegion = region->next;
		AtlasRegion_dispose ( region );
		region = nextRegion;
	}

	FREE ( self );
}

AtlasRegion* Atlas_findRegion ( const Atlas* self, const KDchar* name ) 
{
	AtlasRegion*	region = self->regions;
	while ( region ) 
	{
		if ( kdStrcmp ( region->name, name ) == 0 )
		{
			return region;
		}
		region = region->next;
	}
	return 0;
}


NS_CC_END