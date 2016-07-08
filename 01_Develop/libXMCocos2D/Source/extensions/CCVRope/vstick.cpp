/* -----------------------------------------------------------------------------------
 *
 *      File            vstick.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Creeng Ltd. ( Ported to cocos2d-x 12/7/2012 )
 *      Copyright (c) 2012      Flightless Ltd. ( Modified 20/4/2012 )
 *      Copyright (c) 2010      Clever Hamster Games. ( Created by patrick 14/10/2010 )
 *
 *         http://www.www.flightless.co.nz 
 *         http://www.creeng.com      
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy 
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights 
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is furnished
 *      to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in all 
 *      copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *      INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *      A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 *      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCVRope/vstick.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

VStick*	VStick::create ( KDvoid )
{
	VStick*		pRet = new VStick ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

VStick*	VStick::create ( VPoint* pPointA, VPoint* pPointB )
{
	VStick*		pRet = new VStick ( );

	if ( pRet && pRet->initWithPoints ( pPointA, pPointB ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

VStick::VStick ( KDvoid ) 
{
	m_pPointA = 0;
	m_pPointB = 0;
	m_fHypotenuse = 0.f;
}

VStick::~VStick ( KDvoid )
{

}

KDbool VStick::init ( KDvoid )
{
	m_pPointA = 0;
	m_pPointB = 0;
	m_fHypotenuse = 0.f;

	return KD_TRUE;
}

KDbool VStick::initWithPoints ( VPoint* pPointA, VPoint* pPointB )
{
	m_pPointA = pPointA;
	m_pPointB = pPointB;
	m_fHypotenuse = ccpDistance ( ccp ( pPointA->x, pPointA->y ), ccp ( pPointB->x, pPointB->y ) );

	return KD_TRUE;
}

KDvoid VStick::contract ( KDvoid )
{
    KDfloat		dx = m_pPointB->x - m_pPointA->x;
	KDfloat		dy = m_pPointB->y - m_pPointA->y;

	KDfloat		h = ccpDistance ( ccp ( m_pPointA->x, m_pPointA->y ), ccp ( m_pPointB->x, m_pPointB->y ) );
	
	KDfloat		diff = m_fHypotenuse - h;
	
	KDfloat		offx = ( diff * dx / h ) * 0.5f;
	KDfloat		offy = ( diff * dy / h ) * 0.5f;

	m_pPointA->x -= offx;
	m_pPointA->y -= offy;
	m_pPointB->x += offx;
	m_pPointB->y += offy;
}

VPoint* VStick::getPointA ( KDvoid )
{
    return m_pPointA;
}

VPoint* VStick::getPointB ( KDvoid )
{
    return m_pPointB;
}

KDvoid VStick::setPointA ( VPoint* pPoint )
{
	m_pPointA = pPoint;
}

KDvoid VStick::setPointB ( VPoint* pPoint )
{
	m_pPointB = pPoint;
}

NS_CC_END