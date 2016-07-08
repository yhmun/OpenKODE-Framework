/* -----------------------------------------------------------------------------------
 *
 *      File            vpoint.cpp
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
#include "extensions/CCVRope/vpoint.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN


static KDfloat	l_fPointGravityX = 0.0f;
static KDfloat	l_fPointGravityY = 9.8f;

VPoint*	VPoint::create ( KDvoid )
{
	VPoint*		pRet = new VPoint ( );

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

VPoint*	VPoint::create ( KDfloat x, KDfloat y )
{
	VPoint*		pRet = new VPoint ( );

	if ( pRet && pRet->initWithPos ( x, y ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

VPoint*	VPoint::create ( const CCPoint& tPoint )
{
	VPoint*		pRet = new VPoint ( );

	if ( pRet && pRet->initWithPoint ( tPoint ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

VPoint::VPoint ( KDvoid )
{
	m_fX	= 0.f;
	m_fY	= 0.f;
	m_fOldX = 0.f;
	m_fOldY = 0.f;
}

VPoint::~VPoint ( KDvoid )
{

}

KDbool VPoint::init ( KDvoid )
{
	m_fX	= 0.f;
	m_fY	= 0.f;
	m_fOldX = 0.f;
	m_fOldY = 0.f;

	return KD_TRUE;
}

KDbool VPoint::initWithPos ( KDfloat x, KDfloat y )
{
	this->setPos ( x, y );

	return KD_TRUE;
}

KDbool VPoint::initWithPoint ( const CCPoint& tPoint )
{
	this->setPoint ( tPoint );

	return KD_TRUE;
}

KDvoid VPoint::setPos ( KDfloat x, KDfloat y )
{
	m_fX = m_fOldX = x;
	m_fY = m_fOldY = y;
}

KDvoid VPoint::update ( KDvoid )
{
	KDfloat		fTempX = m_fX;
	KDfloat		fTempY = m_fY;

	m_fX += m_fX - m_fOldX;
	m_fY += m_fY - m_fOldY;

	m_fOldX = fTempX;
	m_fOldY = fTempY;
}

KDvoid VPoint::applyGravity ( KDfloat fDelta )
{
	m_fX -= l_fPointGravityX * fDelta;
	m_fY -= l_fPointGravityY * fDelta;
}

KDvoid VPoint::applyGravity ( KDfloat fDelta, KDfloat gx, KDfloat gy )
{
	m_fX -= gx * fDelta;
	m_fY -= gy * fDelta;
}

KDvoid VPoint::applyGravityxdt ( KDfloat fDeltaX, KDfloat fDeltaY )
{
    m_fX -= fDeltaX;
    m_fY -= fDeltaY;
}

KDvoid VPoint::setPoint ( const CCPoint& tPoint )
{
	this->setPos ( tPoint.x, tPoint.y );
}

CCPoint VPoint::getPoint ( KDvoid )
{
	return ccp ( m_fX, m_fY );
}

KDvoid VPoint::setGravity ( KDfloat gx, KDfloat gy )
{
	l_fPointGravityX = gx;
	l_fPointGravityY = gy;        
}

CCPoint VPoint::getGravity ( KDvoid )
{
    return ccp ( l_fPointGravityX, l_fPointGravityY );
}


NS_CC_END