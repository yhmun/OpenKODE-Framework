/* --------------------------------------------------------------------------
 *
 *      File            TestZwoptex.cpp
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestZwoptex.h"

KDbool TestZwoptex::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "zwoptex/grossini.plist" );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "zwoptex/grossini-generic.plist" );

	CCLayerColor*  pLayer1 = CCLayerColor::create ( ccc4 ( 255, 0, 0, 255 ), 85, 121 );
	pLayer1->setPosition ( ccp ( tLyrSize.cx / 2 - 80 - ( 85.0f * 0.5f ), tLyrSize.cy / 2 - ( 121.0f * 0.5f ) ) );
	addChild ( pLayer1 );

	m_pSprite1 = CCSprite::createWithSpriteFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "grossini_dance_01.png" ) );
	m_pSprite1->setPosition ( ccp ( tLyrSize.cx / 2 - 80, tLyrSize.cy / 2 ) );
	addChild ( m_pSprite1 );

	m_pSprite1->setFlipX ( KD_FALSE );
	m_pSprite1->setFlipY ( KD_FALSE );

	CCLayerColor*  pLayer2 = CCLayerColor::create ( ccc4 ( 255, 0, 0, 255 ), 85, 121 );
	pLayer2->setPosition ( ccp ( tLyrSize.cx / 2 + 80 - ( 85.0f * 0.5f ), tLyrSize.cy / 2 - ( 121.0f * 0.5f ) ) );
	addChild ( pLayer2 );

	m_pSprite2 = CCSprite::createWithSpriteFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "grossini_dance_generic_01.png" ) );
	m_pSprite2->setPosition ( ccp ( tLyrSize.cx / 2 + 80, tLyrSize.cy / 2 ) );
	addChild ( m_pSprite2 );

	m_pSprite2->setFlipX ( KD_FALSE );
	m_pSprite2->setFlipY ( KD_FALSE );

	schedule ( schedule_selector ( TestZwoptex::startIn05Secs ), 1.0f );

	m_nCounter = 0;

	return KD_TRUE;
}

TestZwoptex::~TestZwoptex ( KDvoid )
{
    CCSpriteFrameCache*  pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
    pCache->removeSpriteFramesFromFile ( "zwoptex/grossini.plist" );
    pCache->removeSpriteFramesFromFile ( "zwoptex/grossini-generic.plist" );
}

const KDchar* TestZwoptex::subtitle ( KDvoid )
{
	return "Coordinate Formats, Rotation, Trimming, flipX / Y";
}

static KDint l_nSpriteFrameIndex = 0;
KDvoid TestZwoptex::flipSprites ( KDfloat fDelta )
{
    m_nCounter++;

    KDbool  fx = KD_FALSE;
    KDbool  fy = KD_FALSE;

    KDint    i = m_nCounter % 4;

    switch ( i )
    {
        case 0 :
            fx = KD_FALSE;
            fy = KD_FALSE;
            break;

        case 1 :
            fx = KD_TRUE;
            fy = KD_FALSE;
            break;
        
		case 2 :
            fx = KD_FALSE;
            fy = KD_TRUE;
            break;
        
		case 3 :
            fx = KD_TRUE;
            fy = KD_TRUE;
            break;
    }

    m_pSprite1->setFlipX ( fx );
    m_pSprite2->setFlipX ( fx );
    m_pSprite1->setFlipY ( fy );
    m_pSprite2->setFlipY ( fy );

    if ( ++l_nSpriteFrameIndex > 14 )
    {
        l_nSpriteFrameIndex = 1;
    }

    KDchar  szStr1 [32] = { 0 };
    KDchar  szStr2 [32] = { 0 };
    kdSprintf ( szStr1, "grossini_dance_%02d.png"        , l_nSpriteFrameIndex );
    kdSprintf ( szStr2, "grossini_dance_generic_%02d.png", l_nSpriteFrameIndex );

    m_pSprite1->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( szStr1 ) );
    m_pSprite2->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( szStr2 ) );
}

KDvoid TestZwoptex::startIn05Secs ( KDfloat fDelta )
{
    unschedule ( schedule_selector ( TestZwoptex::startIn05Secs ) );
    schedule   ( schedule_selector ( TestZwoptex::flipSprites ), 0.5f );
}