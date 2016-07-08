/* -----------------------------------------------------------------------------------
 *
 *      File            Atlas.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
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
 *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __Atlas_h__
#define __Atlas_h__

#include <kode.h>

namespace spine {

typedef enum 
{
	ATLAS_ALPHA, ATLAS_INTENSITY, ATLAS_LUMINANCE_ALPHA, ATLAS_RGB565, ATLAS_RGBA4444, ATLAS_RGB888, ATLAS_RGBA8888
} AtlasFormat;

typedef enum 
{
	ATLAS_NEAREST,
	ATLAS_LINEAR,
	ATLAS_MIPMAP,
	ATLAS_MIPMAP_NEAREST_NEAREST,
	ATLAS_MIPMAP_LINEAR_NEAREST,
	ATLAS_MIPMAP_NEAREST_LINEAR,
	ATLAS_MIPMAP_LINEAR_LINEAR
} AtlasFilter;

typedef enum 
{
	ATLAS_MIRROREDREPEAT, ATLAS_CLAMPTOEDGE, ATLAS_REPEAT
} AtlasWrap;

typedef struct AtlasPage AtlasPage;
struct AtlasPage 
{
	const KDchar*	name;
	AtlasFormat		format;
	AtlasFilter		minFilter, magFilter;
	AtlasWrap		uWrap, vWrap;

	KDvoid*			rendererObject;
	KDint			width, height;

	AtlasPage*		next;
};

AtlasPage*			AtlasPage_create  ( const KDchar* pName );
KDvoid				AtlasPage_dispose ( AtlasPage* pSelf );

/**/

typedef struct AtlasRegion AtlasRegion;
struct AtlasRegion 
{
	const KDchar*	name;
	KDint			x, y, width, height;
	KDfloat			u, v, u2, v2;
	KDint			offsetX, offsetY;
	KDint			originalWidth, originalHeight;
	KDint			index;
	KDint			rotate;
	KDint			flip;
	KDint*			splits;
	KDint*			pads;

	AtlasPage*		page;

	AtlasRegion*	next;
};

AtlasRegion*		AtlasRegion_create  ( KDvoid );
KDvoid				AtlasRegion_dispose ( AtlasRegion* pSelf );

/**/

typedef struct 
{
	AtlasPage*		pages;
	AtlasRegion*	regions;
} Atlas;

/* Image files referenced in the atlas file will be prefixed with dir. */
Atlas*				Atlas_readAtlas ( const KDchar* pData, KDint nLength, const KDchar* pDir );
/* Image files referenced in the atlas file will be prefixed with the directory containing the atlas file. */
Atlas*				Atlas_readAtlasFile ( const KDchar* pPath );
KDvoid				Atlas_dispose ( Atlas* pAtlas );

/* Returns 0 if the region was not found. */
AtlasRegion*		Atlas_findRegion ( const Atlas* pSelf, const KDchar* pName );

} // namespace spine {

#endif	// __Atlas_h__
