/* --------------------------------------------------------------------------
 *
 *      File            CustomAnimation.cpp    
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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
#include "CustomAnimation.h"

CustomAnimation* CustomAnimation::create		( const KDchar*	szTheFileNameToAnimate,
												  KDint			nTheFrameToStartWith,
												  KDint			nTheNumberOfFramesToAnimate,
												  KDint			nTheX,
												  KDint			nTheY,
												  KDbool		bFlipOnX,
												  KDbool		bFlipOnY,
												  KDbool		bDoesItLoop,
												  KDbool		bDoesItUseRandomFrameToLoop )
{
	CustomAnimation*	pRet = new CustomAnimation ( );

	if ( pRet && pRet->InitWithOurOwnProperties (	szTheFileNameToAnimate,
													nTheFrameToStartWith,
													nTheNumberOfFramesToAnimate,
													nTheX,
													nTheY,
													bFlipOnX,
													bFlipOnY,
													bDoesItLoop,
													bDoesItUseRandomFrameToLoop ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CustomAnimation::InitWithOurOwnProperties	( const KDchar*	szTheFileNameToAnimate,
													  KDint			nTheFrameToStartWith,
													  KDint			nTheNumberOfFramesToAnimate,
													  KDint			nTheX,
													  KDint			nTheY,
													  KDbool		bFlipOnX,
													  KDbool		bFlipOnY,
													  KDbool		bDoesItLoop,
													  KDbool		bDoesItUseRandomFrameToLoop )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_sFileNameToAnimate	= szTheFileNameToAnimate;
	
	m_nFrameToStartWith		= nTheFrameToStartWith;
	m_nCurrentFrame			= nTheFrameToStartWith;
	
	m_nFramesToAnimate		= nTheNumberOfFramesToAnimate;
	
	m_bAnimationFlippedX	= bFlipOnX;
	m_bAnimationFlippedY	= bFlipOnY;
	
	m_bDoesTheAnimationLoop = bDoesItLoop;
	m_bUseRandomFrameToLoop = bDoesItUseRandomFrameToLoop;
	
	m_pSomeSprite = CCSprite::create ( ccszf ( "%s_000%d.png", szTheFileNameToAnimate, nTheFrameToStartWith ) );
	this->addChild ( m_pSomeSprite );

	m_pSomeSprite->setPosition ( ccp ( nTheX, nTheY ) );
	
	m_pSomeSprite->setFlipX ( bFlipOnX );
	m_pSomeSprite->setFlipY ( bFlipOnY );		
	
	this->schedule ( schedule_selector ( CustomAnimation::runAnimation ), 1.0f / 60.0f );

	return KD_TRUE;
}

KDvoid CustomAnimation::runAnimation ( KDfloat fDelta )
{
	m_nCurrentFrame++;		// adds 1 to currentFrame

	if ( m_nCurrentFrame <= m_nFramesToAnimate )
	{		
		const KDchar*		szFile;

		// if you don't want leading zeros		
		if ( m_nCurrentFrame < 10 )
		{			
			szFile = ccszf ( "%s_000%d.png", m_sFileNameToAnimate.c_str ( ), m_nCurrentFrame );
		} 
		else if ( m_nCurrentFrame < 100 )
		{		
			szFile = ccszf ( "%s_00%d.png", m_sFileNameToAnimate.c_str ( ), m_nCurrentFrame );
		}
		else 
		{	
			szFile = ccszf ( "%s_0%d.png", m_sFileNameToAnimate.c_str ( ), m_nCurrentFrame );	
		}	

		m_pSomeSprite->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( szFile ) );
	}
	else 
	{		
		if ( m_bDoesTheAnimationLoop && !m_bUseRandomFrameToLoop )
		{			
			m_nCurrentFrame = m_nFrameToStartWith;			
		}
		else if ( m_bDoesTheAnimationLoop && m_bUseRandomFrameToLoop )
		{			
			m_nCurrentFrame = kdRand ( ) % m_nFramesToAnimate;			// you'd get a range of 0 to whatever framesToAnimate is			
		}
		else 
		{
			this->removeChild ( m_pSomeSprite, KD_FALSE );
			this->removeFromParentAndCleanup ( KD_FALSE );
			this->unschedule ( schedule_selector ( CustomAnimation::runAnimation ) );
		}	
	}
}

KDvoid CustomAnimation::changeOpacityTo ( KDubyte cOpacity )
{
	m_pSomeSprite->setOpacity ( cOpacity );		// range of 0 to 255
}

KDvoid CustomAnimation::tintMe ( const ccColor3B& tColor )
{
	m_pSomeSprite->setColor ( tColor );
}
