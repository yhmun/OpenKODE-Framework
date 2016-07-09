/* -----------------------------------------------------------------------------------
 *
 *      File            Grapes.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Allen Benson G Tan on 5/19/12 
 *      Copyright (c) 2012      WhiteWidget Inc. All rights reserved.        
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 * 
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 * 
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "Grapes.h"

Grapes* Grapes::createWithWorld ( b2World* pWorld )
{
    Grapes*		pRet = new Grapes ( );

    if ( pRet && pRet->initWithWorld ( pWorld ) )
    {
        pRet->autorelease ( );
    }
    else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDbool Grapes::initWithWorld ( b2World* pWorld )
{
	CCSize			tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );   
    const KDchar*	szFile = "grapes.png";
	KDint			nCount = 8;
    
	b2Vec2			aVertices [ ] = 
	{
        b2Vec2 ( 14.f / PTM_RATIO, 61.f / PTM_RATIO ),
        b2Vec2 ( 13.f / PTM_RATIO, 39.f / PTM_RATIO ),
        b2Vec2 ( 18.f / PTM_RATIO, 16.f / PTM_RATIO ),
        b2Vec2 ( 30.f / PTM_RATIO,  3.f / PTM_RATIO ),
        b2Vec2 ( 39.f / PTM_RATIO,  3.f / PTM_RATIO ),
        b2Vec2 ( 50.f / PTM_RATIO, 29.f / PTM_RATIO ),
        b2Vec2 ( 54.f / PTM_RATIO, 50.f / PTM_RATIO ),
        b2Vec2 ( 42.f / PTM_RATIO, 62.f / PTM_RATIO )
    };
       
    b2Body*			pBody = createBodyForWorld ( pWorld, b2Vec2 ( tWinSize.cx / 2 / PTM_RATIO, tWinSize.cy / 2 / PTM_RATIO ), 0, aVertices, nCount, 5.f, .2f, .2f );
        
    if ( !PolygonSpriteEx::initWithFile ( szFile, pBody, KD_TRUE, PTM_RATIO ) )
	{
		return KD_FALSE;
	}

	this->setType   ( kTypeGrapes );
	this->setSplurt ( CCParticleSystemQuad::create ( "grapes_splurt.plist" ) );
	this->getSplurt ( )->stopSystem ( );

	return KD_TRUE;
}

