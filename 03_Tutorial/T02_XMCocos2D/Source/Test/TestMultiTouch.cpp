/* --------------------------------------------------------------------------
 *
 *      File            TestMultiTouch.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "TestMultiTouch.h"

static ccColor3B	l_aTouchColors [ CC_MAX_TOUCHES ] = 
{
    ccYELLOW	,
    ccBLUE		,
    ccGREEN		,
    ccRED		,
    ccMAGENTA	,
	ccORANGE	,
	ccGRAY		,
	ccWHITE		,
	ccYELLOW	,
	ccBLUE
};

class TouchPoint : public CCNode
{
	public :

		TouchPoint ( KDvoid )
		{
			setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );
		}

		static TouchPoint*	create ( CCNode* pParent )
		{
			TouchPoint*  pRet = new TouchPoint ( );
			pRet->setContentSize ( pParent->getContentSize ( ) );
			pRet->setAnchorPoint ( ccp ( 0.0f, 0.0f ) );
			pRet->autorelease ( );
			return pRet;
		}

	public :

		virtual KDvoid	draw ( KDvoid )
		{
			ccDrawColor4B ( m_tTouchColor.r, m_tTouchColor.g, m_tTouchColor.b, 200 );
			ccLineWidth ( 10 );
			ccDrawLine	( ccp ( 0, m_tTouchPoint.y ), ccp ( getContentSize ( ).cx, m_tTouchPoint.y ) );
			ccDrawLine	( ccp ( m_tTouchPoint.x, 0 ), ccp ( m_tTouchPoint.x, getContentSize ( ).cy ) );
			glLineWidth	( 1 );
			ccPointSize ( 30 );
			ccDrawPoint ( m_tTouchPoint );
		}

		KDvoid	setTouchPos ( const CCPoint& tPoint )
		{
			m_tTouchPoint = tPoint;
		}

		KDvoid	setTouchColor ( ccColor3B tColor )
		{
			m_tTouchColor = tColor;
		}

	private :

		CCPoint			m_tTouchPoint;
		ccColor3B		m_tTouchColor;
};


static CCDictionary		s_tDictionary;

KDbool TestMultiTouch::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

const KDchar* TestMultiTouch::subtitle ( KDvoid )
{
    return "( tap screen )";
}

KDvoid TestMultiTouch::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
    for ( CCSetIterator  iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ )
    {
        CCTouch*	 pTouch = (CCTouch*) ( *iter );
        TouchPoint*  pTouchPoint = TouchPoint::create ( this );

        pTouchPoint->setTouchPos   ( this->convertTouchToNodeSpace ( pTouch ) );
        pTouchPoint->setTouchColor ( l_aTouchColors [ pTouch->getID ( ) ]);

        this->addChild ( pTouchPoint );
        m_tDictionary.setObject ( pTouchPoint, pTouch->getID ( ) );		
    }
}

KDvoid TestMultiTouch::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator  iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ )
	{
        CCTouch*     pTouch = (CCTouch*) ( *iter );
        TouchPoint*  pTouchPoint = (TouchPoint*) m_tDictionary.objectForKey ( pTouch->getID ( ) );

        pTouchPoint->setTouchPos ( this->convertTouchToNodeSpace ( pTouch ) );
	}
}

KDvoid TestMultiTouch::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    for ( CCSetIterator  iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ )
    {
        CCTouch*	 pTouch = (CCTouch*) ( *iter );
        TouchPoint*  pTouchPoint = (TouchPoint*) m_tDictionary.objectForKey ( pTouch->getID ( ) );
        
		this->removeChild ( pTouchPoint );
        m_tDictionary.removeObjectForKey ( pTouch->getID ( ) );
    }
}