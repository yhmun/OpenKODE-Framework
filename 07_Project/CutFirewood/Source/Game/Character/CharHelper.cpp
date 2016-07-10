/* -----------------------------------------------------------------------------------
 *
 *      File            CharHelper.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "CharHelper.h"
#include "Object/Animation/Animations.h"

cCharHelper::cCharHelper ( JBaseScene* pScene, eLookAtType eLookAt, Point tPoint )
: m_pAni ( nullptr )
{
	// animations init
	m_pAni = new cAnimations ( "hussy_motion.spr", eLayerGame_CharacterHelper, tPoint );

	//animation start
	m_pAni->selectAnimation ( eAniType_nor );
	m_pAni->start ( );
	
	// add animation
	pScene->addDrawObject ( m_pAni );
}

cCharHelper::~cCharHelper ( KDvoid )
{
	m_pAni = nullptr;
}

KDvoid cCharHelper::setAniNor ( KDvoid )
{
	m_pAni->selectAnimation ( eAniType_nor );
	m_pAni->setIsLoop ( true );
	m_pAni->start ( );
}

KDvoid cCharHelper::setAniGet ( KDvoid )
{
	m_pAni->selectAnimation ( eAniType_get );
	m_pAni->setIsLoop ( false );
	m_pAni->start ( );
}

KDvoid cCharHelper::setAniThrow ( KDvoid )
{
	m_pAni->selectAnimation ( eAniType_throw );
	m_pAni->setIsLoop ( false );
	m_pAni->start ( );
	
	// sound
	if ( cGlobalData::getObj ( )->m_bIsEffectMusic == true )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "throw.wav" );
	}
}

KDvoid cCharHelper::update ( KDdouble dLeftTime )
{
	// ani
	m_pAni->update ( dLeftTime );
	
	if ( m_pAni->getIsPlay ( ) == false )
	{
		setAniNor ( );
	}
}
