/* -----------------------------------------------------------------------------------
 *
 *      File            SpineCocos2dx.cpp
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
 *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCSpine/SpineCocos2dx.h"
#include "extensions/CCSpine/extension.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "textures/CCTextureCache.h"
#include "textures/CCTextureAtlas.h"
#include "support/CCFileUtils.h"
#include "shaders/CCShaderCache.h"

NS_CC_BEGIN



KDvoid _AtlasPage_createTexture ( AtlasPage* self, const KDchar* path ) 
{
	CCTexture2D*	texture = CCTextureCache::sharedTextureCache ( )->addImage ( path );
	CCTextureAtlas* textureAtlas = CCTextureAtlas::createWithTexture ( texture, 4 );
	textureAtlas->retain ( );
	self->texture = textureAtlas;
    
    // Using getContentSize to make it supports the strategy of loading resources in cocos2d-x.
    
	self->width  = (KDint) texture->getContentSize ( ).cx;
	self->height = (KDint) texture->getContentSize ( ).cy;
}

KDvoid _AtlasPage_disposeTexture ( AtlasPage* self ) 
{
	 ( (CCTextureAtlas*) self->texture )->release ( );
}

KDchar* _Util_readFile ( const KDchar* path, KDint* length ) 
{
	KDsize   uSize;
    KDchar*  pData = reinterpret_cast<char*> ( CCFileUtils::sharedFileUtils ( )->getFileData ( path, "r", &uSize ) );
	*length = uSize;
	return pData;
}



KDvoid RegionAttachment_updateQuad ( RegionAttachment* self, Slot* slot, ccV3F_C4B_T2F_Quad* quad )
{
	RegionAttachment_updateVertices ( self, slot );

	GLubyte r = (GLubyte) ( slot->skeleton->r * slot->r * 255 );
	GLubyte g = (GLubyte) ( slot->skeleton->g * slot->g * 255 );
	GLubyte b = (GLubyte) ( slot->skeleton->b * slot->b * 255 );
	GLubyte a = (GLubyte) ( slot->skeleton->a * slot->a * 255 );
	quad->bl.colors.r = r;
	quad->bl.colors.g = g;
	quad->bl.colors.b = b;
	quad->bl.colors.a = a;
	quad->tl.colors.r = r;
	quad->tl.colors.g = g;
	quad->tl.colors.b = b;
	quad->tl.colors.a = a;
	quad->tr.colors.r = r;
	quad->tr.colors.g = g;
	quad->tr.colors.b = b;
	quad->tr.colors.a = a;
	quad->br.colors.r = r;
	quad->br.colors.g = g;
	quad->br.colors.b = b;
	quad->br.colors.a = a;

	quad->bl.vertices.x = self->vertices[VERTEX_X1];
	quad->bl.vertices.y = self->vertices[VERTEX_Y1];
	quad->tl.vertices.x = self->vertices[VERTEX_X2];
	quad->tl.vertices.y = self->vertices[VERTEX_Y2];
	quad->tr.vertices.x = self->vertices[VERTEX_X3];
	quad->tr.vertices.y = self->vertices[VERTEX_Y3];
	quad->br.vertices.x = self->vertices[VERTEX_X4];
	quad->br.vertices.y = self->vertices[VERTEX_Y4];

	if ( self->region->rotate )
	{
		quad->tl.texCoords.u = self->region->u;
		quad->tl.texCoords.v = self->region->v2;
		quad->tr.texCoords.u = self->region->u;
		quad->tr.texCoords.v = self->region->v;
		quad->br.texCoords.u = self->region->u2;
		quad->br.texCoords.v = self->region->v;
		quad->bl.texCoords.u = self->region->u2;
		quad->bl.texCoords.v = self->region->v2;
	} 
	else 
	{
		quad->bl.texCoords.u = self->region->u;
		quad->bl.texCoords.v = self->region->v2;
		quad->tl.texCoords.u = self->region->u;
		quad->tl.texCoords.v = self->region->v;
		quad->tr.texCoords.u = self->region->u2;
		quad->tr.texCoords.v = self->region->v;
		quad->br.texCoords.u = self->region->u2;
		quad->br.texCoords.v = self->region->v2;
	}
}



CCSkeleton* CCSkeleton::create ( const KDchar* skeletonDataFile, Atlas* atlas, KDfloat scale ) 
{
	SkeletonJson*	json = SkeletonJson_create ( atlas );
	json->scale = scale;

	SkeletonData*	skeletonData = SkeletonJson_readSkeletonDataFile ( json, skeletonDataFile );
	SkeletonJson_dispose ( json );

	CCSkeleton*		node = skeletonData ? create ( skeletonData ) : 0;
	node->ownsSkeleton = KD_TRUE;
	return node;
}

CCSkeleton* CCSkeleton::create ( const KDchar* skeletonDataFile, const KDchar* atlasFile, KDfloat scale )
{
	Atlas*	atlas = Atlas_readAtlasFile ( atlasFile );
	if ( !atlas ) 
	{
		return 0;
	}

	SkeletonJson*	json = SkeletonJson_create ( atlas );
	json->scale = scale;

	SkeletonData*	skeletonData = SkeletonJson_readSkeletonDataFile ( json, skeletonDataFile );
	SkeletonJson_dispose ( json );
	if ( !skeletonData ) 
	{
		Atlas_dispose ( atlas );
		return 0;
	}

	CCSkeleton*		node = create ( skeletonData );
	node->ownsSkeleton = KD_TRUE;
	node->atlas = atlas;
	return node;
}

CCSkeleton* CCSkeleton::create ( SkeletonData* skeletonData, AnimationStateData* stateData )
{
	CCSkeleton*		node = new CCSkeleton ( skeletonData, stateData );
	node->autorelease ( );
	return node;
}

CCSkeleton::CCSkeleton ( SkeletonData *skeletonData, AnimationStateData *stateData ) :
				ownsSkeleton ( false ), ownsStateData ( false ), atlas ( 0 ),
				skeleton ( 0 ), state ( 0 ), debugSlots ( false ), debugBones ( false ) {
	CONST_CAST ( Skeleton*, skeleton ) = Skeleton_create ( skeletonData );

	if ( !stateData ) {
		stateData = AnimationStateData_create ( skeletonData );
		ownsStateData = true;
	}
	CONST_CAST ( AnimationState*, state ) = AnimationState_create ( stateData );

	m_tBlendFunc.src = GL_ONE;
	m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;

	timeScale = 1;

	setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );
	scheduleUpdate ( );
}

CCSkeleton::~CCSkeleton ( KDvoid )
{
	if ( ownsSkeleton ) 
	{
		Skeleton_dispose ( skeleton );
	}

	if ( ownsStateData ) 
	{
		AnimationStateData_dispose ( state->data );
	}

	if ( atlas ) 
	{
		Atlas_dispose ( atlas );
	}

	AnimationState_dispose ( state );
}

KDvoid CCSkeleton::update ( KDfloat deltaTime )
{
	Skeleton_update ( skeleton, deltaTime );
	AnimationState_update ( state, deltaTime * timeScale );
	AnimationState_apply ( state, skeleton );
	Skeleton_updateWorldTransform ( skeleton );
}

KDvoid CCSkeleton::draw ( KDvoid ) 
{
	CC_NODE_DRAW_SETUP ( );

	ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

	ccColor3B	color = getColor ( );
	skeleton->r = color.r / (KDfloat) 255;
	skeleton->g = color.g / (KDfloat) 255;
	skeleton->b = color.b / (KDfloat) 255;
	skeleton->a = getOpacity ( ) / (KDfloat) 255;

	CCTextureAtlas*		textureAtlas = 0;
	ccV3F_C4B_T2F_Quad	quad;
	quad.tl.vertices.z = 0;
	quad.tr.vertices.z = 0;
	quad.bl.vertices.z = 0;
	quad.br.vertices.z = 0;

	for ( KDint i = 0, n = skeleton->slotCount; i < n; i++ ) 
	{
		Slot*	slot = skeleton->slots[i];
		if ( !slot->attachment || slot->attachment->type != ATTACHMENT_REGION )
		{
			continue;
		}

		RegionAttachment*	attachment = (RegionAttachment*) slot->attachment;
		CCTextureAtlas*		regionTextureAtlas = (CCTextureAtlas*) attachment->region->page->texture;
		if ( regionTextureAtlas != textureAtlas )
		{
			if ( textureAtlas ) 
			{
				textureAtlas->drawQuads ( );
				textureAtlas->removeAllQuads ( );
			}
		}
		textureAtlas = regionTextureAtlas;
		if ( textureAtlas->getCapacity ( ) == textureAtlas->getTotalQuads ( ) && !textureAtlas->resizeCapacity ( textureAtlas->getCapacity ( ) * 2 ) ) 
		{
			return;
		}
		RegionAttachment_updateQuad ( attachment, slot, &quad );
		textureAtlas->updateQuad ( &quad, textureAtlas->getTotalQuads ( ) );
	}

	if ( textureAtlas ) 
	{
		textureAtlas->drawQuads ( );
		textureAtlas->removeAllQuads ( );
	}

	if ( debugSlots )
	{
		// Slots.
		ccDrawColor4B ( 0, 0, 255, 255 );
		ccLineWidth ( 1 );

		CCPoint				points[4];
		ccV3F_C4B_T2F_Quad	quad;
		for ( KDint i = 0, n = skeleton->slotCount; i < n; i++ ) 
		{
			Slot*	slot = skeleton->slots[i];
			if ( !slot->attachment || slot->attachment->type != ATTACHMENT_REGION )
			{
				continue;
			}

			RegionAttachment*	attachment = (RegionAttachment*) slot->attachment;
			RegionAttachment_updateQuad ( attachment, slot, &quad );
			points[0] = ccp ( quad.bl.vertices.x, quad.bl.vertices.y );
			points[1] = ccp ( quad.br.vertices.x, quad.br.vertices.y );
			points[2] = ccp ( quad.tr.vertices.x, quad.tr.vertices.y );
			points[3] = ccp ( quad.tl.vertices.x, quad.tl.vertices.y );
			ccDrawPoly ( points, 4, KD_TRUE );
		}
	}

	if ( debugBones ) 
	{
		// Bone lengths.
		ccLineWidth ( 2 );
		ccDrawColor4B ( 255, 0, 0, 255 );
		for ( KDint i = 0, n = skeleton->boneCount; i < n; i++ ) 
		{
			Bone*		bone = skeleton->bones[i];
			KDfloat		x = bone->data->length * bone->m00 + bone->worldX;
			KDfloat		y = bone->data->length * bone->m10 + bone->worldY;
			ccDrawLine ( ccp ( bone->worldX, bone->worldY ), ccp ( x, y ) );
		}
		// Bone origins.
		ccPointSize ( 4 );
		ccDrawColor4B ( 0, 0, 255, 255 ); // Root bone is blue.
		for ( KDint i = 0, n = skeleton->boneCount; i < n; i++ ) 
		{
			Bone*	bone = skeleton->bones[i];
			ccDrawPoint ( ccp ( bone->worldX, bone->worldY ) );
			if ( i == 0 )
			{
				ccDrawColor4B ( 0, 255, 0, 255 );
			}
		}
	}
}

CCRect CCSkeleton::boundingBox ( KDvoid )
{
	KDfloat		fMinX = KD_FLT_MAX, fMinY = KD_FLT_MAX, fMaxX = KD_FLT_MIN, fMaxY = KD_FLT_MIN;
	KDfloat		fScaleX = getScaleX ( );
	KDfloat		fScaleY = getScaleY ( );

	ccV3F_C4B_T2F_Quad	tQuad;
	for ( KDint i = 0; i < skeleton->slotCount; ++i ) 
	{
		Slot*	pSlot = skeleton->slots[i];
		if ( !pSlot->attachment || pSlot->attachment->type != ATTACHMENT_REGION )
		{
			continue;
		}

		RegionAttachment*	pAttachment = (RegionAttachment*) pSlot->attachment;
		RegionAttachment_updateQuad ( pAttachment, pSlot, &tQuad );
		fMinX = KD_MIN ( fMinX, tQuad.bl.vertices.x * fScaleX );
		fMinY = KD_MIN ( fMinY, tQuad.bl.vertices.y * fScaleY );
		fMaxX = KD_MAX ( fMaxX, tQuad.bl.vertices.x * fScaleX );
		fMaxY = KD_MAX ( fMaxY, tQuad.bl.vertices.y * fScaleY );
		fMinX = KD_MIN ( fMinX, tQuad.br.vertices.x * fScaleX );
		fMinY = KD_MIN ( fMinY, tQuad.br.vertices.y * fScaleY );
		fMaxX = KD_MAX ( fMaxX, tQuad.br.vertices.x * fScaleX );
		fMaxY = KD_MAX ( fMaxY, tQuad.br.vertices.y * fScaleY );
		fMinX = KD_MIN ( fMinX, tQuad.tl.vertices.x * fScaleX );
		fMinY = KD_MIN ( fMinY, tQuad.tl.vertices.y * fScaleY );
		fMaxX = KD_MAX ( fMaxX, tQuad.tl.vertices.x * fScaleX );
		fMaxY = KD_MAX ( fMaxY, tQuad.tl.vertices.y * fScaleY );
		fMinX = KD_MIN ( fMinX, tQuad.tr.vertices.x * fScaleX );
		fMinY = KD_MIN ( fMinY, tQuad.tr.vertices.y * fScaleY );
		fMaxX = KD_MAX ( fMaxX, tQuad.tr.vertices.x * fScaleX );
		fMaxY = KD_MAX ( fMaxY, tQuad.tr.vertices.y * fScaleY );
	}

	CCPoint  tPosition = getPosition ( );
	fMinX = tPosition.x + fMinX;
	fMinY = tPosition.y + fMinY;
	fMaxX = tPosition.x + fMaxX;
	fMaxY = tPosition.y + fMaxY;
	return CCRectMake ( fMinX, fMinY, fMaxX - fMinX, fMaxY - fMinY );
}

// CCBlendProtocol

ccBlendFunc CCSkeleton::getBlendFunc ( KDvoid )
{
    return m_tBlendFunc;
}

KDvoid CCSkeleton::setBlendFunc ( ccBlendFunc tBlendFunc ) 
{
    this->m_tBlendFunc = tBlendFunc;
}

NS_CC_END
