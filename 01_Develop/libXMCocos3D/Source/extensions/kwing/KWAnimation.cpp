/* --------------------------------------------------------------------------
 *
 *      File            KWAnimation.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/05/30 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
 *      
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "KWAnimation.h"
#include "KWSprite.h"

KWSprite* KWAnimation::spriteWithArray ( CCArray* pTextures, KDfloat fAps )
{
	CCArray*	pFrames = CCArray::createWithCapacity ( pTextures->count ( ) ); 
	CCObject*	pObject = KD_NULL;
	CCARRAY_FOREACH ( pTextures, pObject )
	{
		CCTexture2D*	pTexture = (CCTexture2D*) pObject;
		CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( pTexture, CCRect ( ccpz, pTexture->getContentSize ( ) ) );
		pFrames->addObject ( pFrame );
	}
	return KWAnimation::spriteWithSpriteFrames ( pFrames, fAps );
}

KWSprite* KWAnimation::spriteWithFiles ( CCArray* pFiles, KDfloat fAps )
{
	CCArray*	pSprites = CCArray::createWithCapacity ( pFiles->count ( ) ); 
	CCObject*	pObject  = KD_NULL;
	CCARRAY_FOREACH ( pFiles, pObject )
	{
		CCString*		pFile = (CCString*) pObject;
		CCTexture2D*	pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( pFile->getCString ( ) );
		pSprites->addObject ( pTexture );

	}
	return KWAnimation::spriteWithArray ( pSprites, fAps );
}

KWSprite* KWAnimation::spriteWithFile ( const KDchar* szFilename, CCSize tSize, KDfloat fAps )
{
	CCTexture2D*	pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szFilename );
	return KWAnimation::spriteWithTextureAtlas ( pTexture, tSize, fAps );
}

KWSprite* KWAnimation::spriteWithTextureAtlas ( CCTexture2D* pTexture, CCSize tSize, KDfloat fAps )
{
	CCSize		tTexSize = pTexture->getContentSize ( );
	KDint		nCol = (KDint) ( tTexSize.cx / tSize.cx );
	KDint		nRow = (KDint) ( tTexSize.cy / tSize.cy );
	KDint		nFrameCount = nCol * nRow;
	CCArray*	pFrames = CCArray::createWithCapacity ( nFrameCount );
	for ( KDint i = 0; i < nFrameCount; ++i )
	{
		CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( pTexture, CCRectMake ( i % nCol, i / nCol, tTexSize.cx, tTexSize.cy ) );
		pFrames->addObject ( pFrame );
	}
	return KWAnimation::spriteWithSpriteFrames ( pFrames, fAps );
}

KWSprite* KWAnimation::spriteWithSpriteFrames ( CCArray* pFrames, KDfloat fAps )
{
	KWSprite*		pSprite = KWSprite::createWithSpriteFrame ( (CCSpriteFrame*) pFrames->objectAtIndex ( 0 ) );
	CCAnimation*	pAnimation = CCAnimation::createWithSpriteFrames ( pFrames, fAps );
	pAnimation->setRestoreOriginalFrame ( KD_FALSE );

	CCAnimate*		pAnimate = CCAnimate::create ( pAnimation ); 
	CCSequence*		pSeq = CCSequence::create ( pAnimate, KD_NULL );
	pSprite->runAction ( CCRepeatForever::create ( pSeq ) );
	return pSprite;
}
