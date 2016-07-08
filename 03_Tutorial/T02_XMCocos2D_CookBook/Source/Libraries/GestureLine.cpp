/* --------------------------------------------------------------------------
 *
 *      File            GestureLine.cpp
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
#include "GestureLine.h"

GestureLine* GestureLine::create ( const CCPoint& tPoint1, const CCPoint& tPoint2 )
{
	GestureLine*	pRet = new GestureLine ( );

	if ( pRet && pRet->initWithPoints ( tPoint1, tPoint2 ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool GestureLine::initWithPoints ( const CCPoint& tPoint1, const CCPoint& tPoint2 )
{
	m_tPoint1 = tPoint1;
	m_tPoint2 = tPoint2;

	return KD_TRUE;
}

const CCPoint& GestureLine::getPoint1 ( KDvoid )
{
	return m_tPoint1;
}

KDvoid GestureLine::setPoint1 ( const CCPoint& tPoint )
{
	m_tPoint1 = tPoint;
}

const CCPoint& GestureLine::getPoint2 ( KDvoid )
{
	return m_tPoint2;
}

KDvoid GestureLine::setPoint2 ( const CCPoint& tPoint )
{
	m_tPoint2 = tPoint;
}