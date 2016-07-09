/* --------------------------------------------------------------------------
 *
 *      File            GestureShapeLayer.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "GestureShapeLayer.h"
#include "GestureLine.h"

GestureShapeLayer* GestureShapeLayer::create ( KDvoid )
{
	GestureShapeLayer*	pRet = new GestureShapeLayer ( );

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

GestureShapeLayer::GestureShapeLayer ( KDvoid )
{
	m_pPoints = KD_NULL;
	m_pLines  = KD_NULL;
}

GestureShapeLayer::~GestureShapeLayer ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pPoints );
	CC_SAFE_RELEASE ( m_pLines );
}

KDbool GestureShapeLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	m_bDrawCircle = KD_FALSE;
	m_bDrawLines  = KD_FALSE;

	return KD_TRUE;
}

KDvoid GestureShapeLayer::draw ( KDvoid )
{	
	// Set line width.
	ccLineWidth ( 4.0f );
	
	// Set point size
	ccPointSize ( 16 );
	
	// Set color to blue
	ccDrawColor4B ( 255, 0, 0, 255 );

	// We either draw a circle or a series of lines
	if ( m_bDrawCircle )
	{
		// Draw a circle using circleRect information
		ccDrawCircle ( m_tCircleRect.origin, m_tCircleRect.size.cx, 0.0f, 100, KD_FALSE );
	}
	else if ( m_bDrawLines )
	{
		// Draw all lines
		for ( KDuint i = 0; i < m_pLines->count ( ); i++ )
		{
			GestureLine*	pLine = (GestureLine*) m_pLines->objectAtIndex ( i );
			ccDrawLine ( pLine->getPoint1 ( ), pLine->getPoint2 ( ) );
		}
	}
		
	// Restore original values
	ccLineWidth ( 1 );	
	ccDrawColor4B ( 255, 255, 255, 255 );
	ccPointSize ( 1 );		
}

KDvoid GestureShapeLayer::setCircleRectFromPoints ( KDvoid )
{
	// We approximate the circle dimensions based on the point array
	CCPoint		tP1 = ( (CCPointValue*) m_pPoints->objectAtIndex ( m_pPoints->count ( ) / 2 ) )->getValue ( );
	CCPoint		tP2 = ( (CCPointValue*) m_pPoints->objectAtIndex ( 0 ) )->getValue ( );
	CCPoint		tMidPoint = ccp ( ( tP1.x + tP2.x ) / 2, ( tP1.y + tP2.y ) / 2 );

	KDfloat		fDiameter = distanceBetweenPoints ( tP1, tP2 );
	KDfloat		fRadius = fDiameter / 2;
	
	m_tCircleRect = CCRect ( tMidPoint.x, tMidPoint.y, fRadius, fRadius );
}

CCArray* GestureShapeLayer::getPoints ( KDvoid )
{
	return m_pPoints;
}

KDvoid GestureShapeLayer::setPoints ( CCArray* pPoints )
{
	CC_SAFE_RELEASE ( m_pPoints );
	m_pPoints = pPoints;
	CC_SAFE_RETAIN ( m_pPoints );
}

CCArray* GestureShapeLayer::getLines ( KDvoid )
{
	return m_pLines;
}	

KDvoid GestureShapeLayer::setLines ( CCArray* pLines )
{
	CC_SAFE_RELEASE ( m_pLines );
	m_pLines = pLines;
	CC_SAFE_RETAIN ( m_pLines );
}

const CCRect& GestureShapeLayer::getCircleRect ( KDvoid )
{
	return m_tCircleRect;
}

KDvoid GestureShapeLayer::setCircleRect ( const CCRect& tCircleRect )
{
	m_tCircleRect = tCircleRect;
}

KDbool GestureShapeLayer::isDrawCircle ( KDvoid )
{
	return m_bDrawCircle;
}

KDvoid GestureShapeLayer::setDrawCircle ( KDbool bDraw )
{
	m_bDrawCircle = bDraw;
}

KDbool GestureShapeLayer::isDrawLines ( KDvoid )
{
	return m_bDrawLines;
}

KDvoid GestureShapeLayer::setDrawLines ( KDbool bDraw )
{
	m_bDrawLines = bDraw;
}