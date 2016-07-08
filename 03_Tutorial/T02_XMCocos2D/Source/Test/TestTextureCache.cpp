/* --------------------------------------------------------------------------
 *
 *      File            TestTextureCache.cpp
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
#include "TestTextureCache.h"

KDbool TestTextureCache::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	m_nNumberOfSprites		 = 20;
	m_nNumberOfLoadedSprites = 0;

	CCDirector::sharedDirector ( )->purgeCachedData ( );

    const CCSize&  tLyrSize = this->getContentSize ( );

    m_pLabelLoading = CCLabelTTF::create ( "loading...", "fonts/Marker Felt.ttf", 30 );
    m_pLabelPercent = CCLabelTTF::create ( "%0"		   , "fonts/Marker Felt.ttf", 30 );

    m_pLabelLoading->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 - 45 ) );
    m_pLabelPercent->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 + 45 ) );

    this->addChild ( m_pLabelLoading );
    this->addChild ( m_pLabelPercent );

    // load textrues
	CCTextureCache*  pTextureCache = CCTextureCache::sharedTextureCache ( );
	SEL_CallFuncO    pSelector     = callfuncO_selector ( TestTextureCache::loadingCallBack );

    pTextureCache->addImageAsync ( "Images/HelloWorld.png"		 , this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini.png"		 , this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_01.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_02.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_03.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_04.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_05.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_06.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_07.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_08.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_09.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_10.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_11.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_12.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_13.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/grossini_dance_14.png", this, pSelector );
    pTextureCache->addImageAsync ( "Images/background1.png"		 , this, pSelector );
    pTextureCache->addImageAsync ( "Images/background2.png"		 , this, pSelector );
    pTextureCache->addImageAsync ( "Images/background3.png"		 , this, pSelector );
    pTextureCache->addImageAsync ( "Images/blocks.png"			 , this, pSelector );

	return KD_TRUE;
}

KDvoid TestTextureCache::loadingCallBack ( CCObject* pObject )
{
    ++m_nNumberOfLoadedSprites;

    m_pLabelPercent->setString ( ccszf ( "%%%d", (KDint) ( ( (KDfloat) m_nNumberOfLoadedSprites / m_nNumberOfSprites ) * 100 ) ) );

    if ( m_nNumberOfLoadedSprites == m_nNumberOfSprites )
    {
        this->removeChild ( m_pLabelLoading, KD_TRUE );
        this->removeChild ( m_pLabelPercent, KD_TRUE );
        this->addSprite ( );
    }
}

KDvoid TestTextureCache::addSprite ( KDvoid )
{
    // create sprites
    CCSprite*  pBG   = CCSprite::create ( "Images/HelloWorld.png" );
    pBG->setPosition ( ccpMid ( this->getContentSize ( ) ) );

    CCSprite*  pSP1  = CCSprite::create ( "Images/grossini.png" );
    CCSprite*  pSP2  = CCSprite::create ( "Images/grossini_dance_01.png" );
    CCSprite*  pSP3  = CCSprite::create ( "Images/grossini_dance_02.png" );
    CCSprite*  pSP4  = CCSprite::create ( "Images/grossini_dance_03.png" );
    CCSprite*  pSP5  = CCSprite::create ( "Images/grossini_dance_04.png" );
    CCSprite*  pSP6  = CCSprite::create ( "Images/grossini_dance_05.png" );
    CCSprite*  pSP7  = CCSprite::create ( "Images/grossini_dance_06.png" );
    CCSprite*  pSP8  = CCSprite::create ( "Images/grossini_dance_07.png" );
    CCSprite*  pSP9  = CCSprite::create ( "Images/grossini_dance_08.png" );
    CCSprite*  pSP10 = CCSprite::create ( "Images/grossini_dance_09.png" );
    CCSprite*  pSP11 = CCSprite::create ( "Images/grossini_dance_10.png" );
    CCSprite*  pSP12 = CCSprite::create ( "Images/grossini_dance_11.png" );
    CCSprite*  pSP13 = CCSprite::create ( "Images/grossini_dance_12.png" );
    CCSprite*  pSP14 = CCSprite::create ( "Images/grossini_dance_13.png" );
    CCSprite*  pSP15 = CCSprite::create ( "Images/grossini_dance_14.png" );

    // just loading textures to slow down
    CCSprite::create ( "Images/background1.png" );
    CCSprite::create ( "Images/background2.png" );
    CCSprite::create ( "Images/background3.png" );
    CCSprite::create ( "Images/blocks.png" );

    pSP1 ->setPosition ( ccp ( 150, 100 ) );
    pSP2 ->setPosition ( ccp ( 160, 100 ) );
    pSP3 ->setPosition ( ccp ( 170, 100 ) );
    pSP4 ->setPosition ( ccp ( 180, 100 ) );
    pSP5 ->setPosition ( ccp ( 190, 100 ) );
    pSP6 ->setPosition ( ccp ( 200, 100 ) );
    pSP7 ->setPosition ( ccp ( 150, 230 ) );
    pSP8 ->setPosition ( ccp ( 160, 230 ) );
    pSP9 ->setPosition ( ccp ( 170, 230 ) );
    pSP10->setPosition ( ccp ( 180, 230 ) );
    pSP11->setPosition ( ccp ( 190, 230 ) );
    pSP12->setPosition ( ccp ( 200, 230 ) );
    pSP13->setPosition ( ccp ( 150, 320 ) );
    pSP14->setPosition ( ccp ( 160, 320 ) );
    pSP15->setPosition ( ccp ( 170, 320 ) );

    this->addChild ( pBG );
   
    this->addChild ( pSP1 );
    this->addChild ( pSP2 );
    this->addChild ( pSP3 );
    this->addChild ( pSP4 );
    this->addChild ( pSP5 );
    this->addChild ( pSP6 );
    this->addChild ( pSP7 );
    this->addChild ( pSP8 );
    this->addChild ( pSP9 );
    this->addChild ( pSP10 );
    this->addChild ( pSP11 );
    this->addChild ( pSP12 );
    this->addChild ( pSP13 );
    this->addChild ( pSP14 );
    this->addChild ( pSP15 );
}

