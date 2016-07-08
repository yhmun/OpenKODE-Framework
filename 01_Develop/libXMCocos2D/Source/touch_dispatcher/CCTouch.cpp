/* -----------------------------------------------------------------------------------
 *
 *      File            CCTouch.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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
#include "touch_dispatcher/CCTouch.h"

NS_CC_BEGIN
  
CCTouch::CCTouch ( KDvoid ) 
{
	clear ( );
}

KDvoid CCTouch::clear ( KDvoid )
{
	m_nID				  = -1;
	m_tPoint			  = ccpz;
	m_tPrevPoint		  = ccpz;
	m_tStartPoint		  = ccpz;
	m_bStartPointCaptured = KD_FALSE;
}

const CCPoint& CCTouch::getLocation ( KDvoid ) const 
{
	return m_tPoint;
}

const CCPoint& CCTouch::getPreviousLocation ( KDvoid ) const
{
	return m_tPrevPoint; 
}

const CCPoint& CCTouch::getStartLocation ( KDvoid ) const
{
	return m_tStartPoint;
}

CCPoint CCTouch::getDelta ( KDvoid ) const 
{
	return ccpSub ( getLocation ( ), getPreviousLocation ( ) );
}

KDvoid CCTouch::setTouchInfo ( KDint nID, KDfloat fX, KDfloat fY )
{
	m_nID		 = nID;
	m_tPrevPoint = m_tPoint;
	m_tPoint.x   = fX;
	m_tPoint.y   = fY;

    if ( !m_bStartPointCaptured )
    {
        m_tStartPoint = m_tPoint;
        m_bStartPointCaptured = KD_TRUE;
    }
}

KDvoid CCTouch::setTouchInfo ( KDint nID, const CCPoint& tPoint )
{
	this->setTouchInfo ( nID, tPoint.x, tPoint.y );
}

KDvoid CCTouch::setTouchInfo ( const CCPoint& tPoint )
{
	this->setTouchInfo ( m_nID, tPoint.x, tPoint.y );
}

KDvoid CCTouch::setTouchInfo ( KDfloat fX, KDfloat fY )
{
	this->setTouchInfo ( m_nID, fX, fY );
}

KDint CCTouch::getID ( KDvoid ) const
{
	return m_nID;
}

NS_CC_END