/* --------------------------------------------------------------------------
 *
 *      File            CCFish.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      海锋 周 on 12-04-11
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      CJLU. All rights reserved.
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
#include "CCFish.h"

CCFish*	CCFish::createWithSpriteFrameName ( const KDchar* szSpriteFrameName )
{
	CCFish*	pRet = new CCFish ( );

	if ( pRet && pRet->initWithSpriteFrameName ( szSpriteFrameName ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCFish::isCatch ( KDvoid )
{
	return m_bIsCatch;
}

KDvoid CCFish::setCatch ( KDbool bCatch )
{
	m_bIsCatch = bCatch;
}

KDvoid CCFish::addPath ( KDvoid )
{
    switch ( kdRand ( ) % 7 )
    {
        case 0 :	this->moveWithParabola ( this, ccp ( 1200,   200 ), ccp ( -500,  800 ),   0,  20, kdRand ( ) % 10 + 15.f );	break;
		case 1 :	this->moveWithParabola ( this, ccp ( -200,   300 ), ccp ( 1300,  400 ), 180, 170, kdRand ( ) % 10 + 18.f );	break;
		case 2 :	this->moveWithParabola ( this, ccp ( -200,   300 ), ccp ( 1000, -200 ), 190, 200, kdRand ( ) % 10 + 18.f );	break;
		case 3 :	this->moveWithParabola ( this, ccp ( 1300,   400 ), ccp ( -200,  300 ),  10,   5, kdRand ( ) % 10 + 18.f );	break;
		case 4 :	this->moveWithParabola ( this, ccp (  400, -1200 ), ccp (  600, 1000 ),  90,  93, kdRand ( ) % 10 + 18.f );	break;
		case 5 :	this->moveWithParabola ( this, ccp (  600,  1000 ), ccp (  400, -200 ), -70, -80, kdRand ( ) % 10 + 18.f );	break;
		case 6 :	this->moveWithParabola ( this, ccp ( 1200,  2100 ), ccp ( -200,  300 ),  30, -30, kdRand ( ) % 10 + 18.f );	break;
    }
}

KDvoid CCFish::moveWithParabola ( CCSprite* pSprite, const CCPoint& tStartPoint, const CCPoint& tEndPoint, KDfloat fStartAngle, KDfloat fEndAngle, KDfloat fTime )
{
    KDfloat		sx = tStartPoint.x;
    KDfloat		sy = tStartPoint.y; 
    KDfloat		ex = tEndPoint.x + kdRand ( ) % 50;
    KDfloat		ey = tEndPoint.y + kdRand ( ) % 150; 
	KDint		h  = (KDint) ( pSprite->getContentSize ( ).cy * 0.5f );
   
    CCPoint		tPos = ccp ( sx + -200 + kdRand ( ) % 400, sy + -200 + kdRand ( ) % 400 );
    pSprite->setPosition ( tPos );
    pSprite->setRotation ( fStartAngle );

    ccBezierConfig		tBezier; 
    tBezier.tControlPoint1 = ccp ( sx, sy ); 
    tBezier.tControlPoint2 = ccp ( sx + ( ex - sx ) * 0.5f, sy + ( ey - sy ) * 0.5f + kdRand ( ) % 300 ); 
    tBezier.tEndPosition   = ccp ( tEndPoint.x - 30, tEndPoint.y + h );

	pSprite->runAction 
	(
		CCSequence::create 
		(
			CCSpawn::create 
			(
				CCBezierTo::create ( fTime, tBezier ),
				CCRotateTo::create ( fTime, fEndAngle ),
				KD_NULL
			),
			CCRemoveSelf::create ( ),
			KD_NULL
		)
	);
}

KDbool CCFish::randomCatch ( KDint nLevel )
{
    if ( kdRand ( ) % 10 >= nLevel )
	{
        m_bIsCatch = KD_TRUE;
    }
	else
	{
        m_bIsCatch = KD_FALSE;
    }

    return m_bIsCatch;
}
