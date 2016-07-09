/* --------------------------------------------------------------------------
 *
 *      File            SBoxMan.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-06
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "SBoxMan.h"

SBoxMan* SBoxMan::create ( const CCPoint& tPosition )
{
	SBoxMan*	pRet = new SBoxMan ( );

	if ( pRet && pRet->initWithPosition ( tPosition ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool SBoxMan::initWithPosition ( const CCPoint& tPosition )
{
	if ( !CCSprite::initWithFile ( "man_baby.png" ) )
	{
		return KD_FALSE;
	}

	m_eDirection = SDirectionUnknown;

	this->setPosition ( tPosition );

	this->setDirection ( SDirectionDown );

	return KD_TRUE;
}

KDvoid SBoxMan::moveWithDirection ( SDirection eDirection, const CCPoint& tDestination )
{

}

KDvoid SBoxMan::setDirection ( SDirection eDirection )
{
    if ( m_eDirection != eDirection )
	{
        const KDchar*	szFile;

        switch ( eDirection )
		{
            case SDirectionUp		:	szFile = "man_baby_up.png";		break;
			case SDirectionDown		:	szFile = "man_baby_down.png";	break;
			case SDirectionLeft		:	szFile = "man_baby_left.png";	break;
			case SDirectionRight	:	szFile = "man_baby_right.png";	break;
			default					:	szFile = "man_baby.png";		break;
        }

		this->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( szFile ) );

        m_eDirection = eDirection;
    }
}
