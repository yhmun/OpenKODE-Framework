/* --------------------------------------------------------------------------
 *
 *      File            Ch2_Gestures.cpp
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
#include "Ch2_Gestures.h"
#include "Libraries/GestureShapeLayer.h"
#include "Libraries/GestureLine.h"

KDbool Ch2_Gestures::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Init message
	m_pMessage->setString ( "Draw a rectangle, triangle, circle or line" );
	m_pMessage->setColor ( ccc3 ( 255, 0, 0 ) );
	
	// Set font size
//	CCMenuItemFont setFontSize:20];

	// Add our breadcrumbs node
	this->addBreadcrumbs ( );
	
	// Init GestureShapeLayer
	m_pGestureShapeLayer = GestureShapeLayer::create ( );
	m_pGestureShapeLayer->setPosition ( ccp ( 0, 0 ) );
	this->addChild ( m_pGestureShapeLayer, 1 );

	return KD_TRUE;
}

KDvoid Ch2_Gestures::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
    CCPoint		tPoint = pTouch->getLocation ( );
	
	// Add a breadcrumb
	this->addCrumbWithPosition ( tPoint );
	
	// Start a new gesture
	this->newGestureWithPoint ( tPoint );
}

KDvoid Ch2_Gestures::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
    CCPoint		tPoint = pTouch->getLocation ( );
	
	// Add another breadcrumb
	this->addCrumbWithPosition ( tPoint );

	// Add a point to our current gesture
	this->addGesturePoint ( tPoint, KD_FALSE );
}

KDvoid Ch2_Gestures::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
    CCPoint		tPoint = pTouch->getLocation ( );

	// Reset breadcrumbs
	this->removeBreadcrumbs ( );
	this->addBreadcrumbs ( );
		
	// Finish our gesture
	this->finishGestureWithPoint ( tPoint );
}

KDvoid Ch2_Gestures::resetMessage ( KDvoid )
{
	m_pMessage->setString ( "Draw a rectangle, triangle, circle or line" );
}

KDvoid Ch2_Gestures::addBreadcrumbs ( KDvoid )
{
	m_pBreadcrumbs = CCNode::create ( );
	m_pBreadcrumbs->setPosition ( ccp ( 0, 0 ) );
	this->addChild ( m_pBreadcrumbs, 0 ); 
}

KDvoid Ch2_Gestures::removeBreadcrumbs ( KDvoid )
{
	this->removeChild ( m_pBreadcrumbs ); 
	m_pBreadcrumbs = KD_NULL;
}

KDvoid Ch2_Gestures::addCrumbWithPosition ( const CCPoint& tPoint )
{
	CCSprite*	pCrumb = CCSprite::create ( "blank.png" );
	pCrumb->setTextureRect ( CCRect ( 0, 0, 2, 2 ) );
	pCrumb->setColor ( ccc3 ( 255, 255, 0 ) );
	pCrumb->setPosition ( tPoint );
	m_pBreadcrumbs->addChild ( pCrumb );
}

KDvoid Ch2_Gestures::newGestureWithPoint ( const CCPoint& tPoint )
{
	this->resetMessage ( );

	// Init gesture variables
	m_pGestureShapeLayer->setPoints ( CCArray::create ( ) );
	m_pGestureShapeLayer->setLines  ( CCArray::create ( ) );

	m_tFirstPoint	= tPoint;
	m_tLastPoint	= tPoint;
	m_tVertex		= tPoint;

	m_pGestureShapeLayer->getPoints ( )->addObject ( CCPointValue::create ( tPoint ) );

	m_pGestureShapeLayer->setDrawCircle ( KD_FALSE );
	m_pGestureShapeLayer->setDrawLines ( KD_FALSE );
}

KDvoid Ch2_Gestures::addGesturePoint ( const CCPoint& tPoint, KDbool bOverride )
{
	// Set our angle change tolerance to 40 degrees. If it changes more than this we consider this a 'line'
	KDfloat		fAngleDiffTolerance = 40.0f;
	
	// Check the old angle versus the new one
	CCPoint		tVect = ccp ( tPoint.x - m_tLastPoint.x, tPoint.y - m_tLastPoint.y );
	KDfloat		fNewAngle = radiansToDegrees ( vectorToRadians ( tVect ) );

	// Add a line if the angle changed significantly
	if ( m_pGestureShapeLayer->getPoints ( )->count ( ) > 1 )
	{
		KDfloat		fAngleDiff = angleDifference ( fNewAngle, m_fAngle );
		if ( bOverride || ( fAngleDiff > fAngleDiffTolerance && distanceBetweenPoints ( m_tVertex, tPoint ) > 15.0f ) )
		{
			m_pGestureShapeLayer->getLines ( )->addObject ( GestureLine::create ( m_tVertex, tPoint ) );
			m_tVertex = tPoint;
		}
	}

	// Update values
	m_fAngle = fNewAngle;
	m_tLastPoint = tPoint;
	m_pGestureShapeLayer->getPoints ( )->addObject ( CCPointValue::create ( tPoint ) );
}

KDvoid Ch2_Gestures::finishGestureWithPoint ( const CCPoint& tPoint )
{
	this->addGesturePoint ( tPoint,KD_TRUE );

	m_pGestureShapeLayer->setDrawCircle ( KD_FALSE );
	m_pGestureShapeLayer->setDrawLines  ( KD_FALSE );

	// To finish gestures which require the end to be close to the beginning point we supply this distance tolerance
	KDfloat		fLastPointTolerance = 100.0f;
	KDuint		uLineCount = m_pGestureShapeLayer->getLines ( )->count ( );
	
	// Rectangles, triangles and circles
	if ( distanceBetweenPoints ( m_tFirstPoint, m_tLastPoint ) <= fLastPointTolerance )
	{
		if ( uLineCount == 4 )			// 4 lines
		{
			m_pMessage->setString ( "Rectangle" );
			m_pGestureShapeLayer->setDrawLines ( KD_TRUE ); 
		}
		else if ( uLineCount == 3 )		// 3 lines
		{
			m_pMessage->setString ( "Triangle" );
			m_pGestureShapeLayer->setDrawLines ( KD_TRUE ); 
		}
		else if ( uLineCount <= 1 )		// 0 or 1 lines
		{
			m_pMessage->setString ( "Circle" );
			m_pGestureShapeLayer->setCircleRectFromPoints ( );
			m_pGestureShapeLayer->setDrawCircle ( KD_TRUE ); 
		}
		else
		{
			this->resetMessage ( );
			m_pGestureShapeLayer->setLines ( CCArray::create ( ) );
		}
	}
	else	// Lines and angles
	{
		if ( uLineCount == 1 )		// 1 line
		{
			m_pMessage->setString ( "Line" );
			m_pGestureShapeLayer->setDrawLines ( KD_TRUE ); 
		}
		else if ( uLineCount == 2 )	// 2 lines
		{
			m_pMessage->setString ( "Angle" );
			m_pGestureShapeLayer->setDrawLines ( KD_TRUE ); 
		}
		else
		{
			this->resetMessage ( );
			m_pGestureShapeLayer->setLines ( CCArray::create ( ) );
		}
	}
}
