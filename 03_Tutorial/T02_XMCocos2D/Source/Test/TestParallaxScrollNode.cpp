/* -----------------------------------------------------------------------------------
 *
 *      File            TestParallaxScrollNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Created by Thomas Broquist on 3/26/12. 
 *      Copyright (c) 2012      Little Wins LLC. All rights reserved.
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
 *       all copies or substantial portions of the Software.
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
#include "TestParallaxScrollNode.h"

KDbool TestParallaxScrollNode::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&	tLyrSIze = this->getContentSize ( );

	CCSprite*  pSky = CCSprite::create ( "xm_parallaxscrollnode/sky_evening.png" );
	pSky->setPosition ( ccpMid ( tLyrSIze ) );
	this->addChild ( pSky );

	const CCSize&	tSkySize = pSky->getContentSize ( );
    m_pParallax = CCParallaxScrollNode::create ( );
	m_pParallax->setContentSize ( tSkySize );
	pSky->addChild ( m_pParallax );

    CCSprite*	pLand1 = CCSprite::create ( "xm_parallaxscrollnode/land_green.png" );
    CCSprite*	pLand2 = CCSprite::create ( "xm_parallaxscrollnode/land_green.png" );
    m_pParallax->addInfiniteScrollXWithZ  ( 0, ccp ( 0.5f, 0.2f ), ccp ( 0, 0 ), pLand1, pLand2, KD_NULL );
    
    CCSprite*	pLand3 = CCSprite::create ( "xm_parallaxscrollnode/land_grey.png" );
    CCSprite*	pLand4 = CCSprite::create ( "xm_parallaxscrollnode/land_grey.png" );
    m_pParallax->addInfiniteScrollXWithZ  ( -2, ccp ( 0.05f, 0.1f ), ccp ( 0, 60 ), pLand3, pLand4, KD_NULL );
    
    CCSprite*	pClouds1 = CCSprite::create ( "xm_parallaxscrollnode/clouds.png" );
    CCSprite*	pClouds2 = CCSprite::create ( "xm_parallaxscrollnode/clouds.png" );
    m_pParallax->addInfiniteScrollXWithZ  ( 1, ccp ( 0.1f, 0.1f), ccp ( 0, tSkySize.cy - pClouds1->getContentSize ( ).cy ), pClouds1, pClouds2, KD_NULL );

    for ( KDint i = 0; i < 10; i++ )
	{
        CCSprite*	pMountain = CCSprite::create ( "xm_parallaxscrollnode/mountain_grey.png" );
        CCPoint		tPos = ccp ( this->randomValueBetween ( 0, pLand1->getContentSize ( ).cx ), this->randomValueBetween ( tSkySize.cy * 0.1f, tSkySize.cy * 0.25f ) );
        
        KDfloat		fSpeedMountainX = this->randomValueBetween ( 0.15f, 0.25f );
        m_pParallax->addChild ( pMountain, -1, ccp ( fSpeedMountainX, 0.015f ), tPos, ccp ( pLand1->getContentSize ( ).cx * 2, 0 ) );
        
        pMountain->setScale ( this->randomValueBetween ( 0.6f, 1 ) );
    }	
   
    this->scheduleUpdate ( );

	return KD_TRUE;
}

KDuint TestParallaxScrollNode::count ( KDvoid )
{
	return 1;
}

KDvoid TestParallaxScrollNode::update ( KDfloat fDelta )
{
	m_pParallax->updateWithVelocity ( ccp ( -5, 0 ), fDelta );
}

KDfloat TestParallaxScrollNode::randomValueBetween ( KDfloat fLow, KDfloat fHigh )
{
    return ( CCRANDOM_0_1 ( ) * ( fHigh - fLow ) ) + fLow;
}