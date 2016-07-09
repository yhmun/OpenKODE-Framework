/* --------------------------------------------------------------------------
 *
 *      File            HudLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/04
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
#include "HudLayer.h"

KDbool HudLayer::init ( KDvoid )
{
    if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}
    
    CCSprite*		pHudBackground = CCSprite::createWithSpriteFrameName ( "head_ui.png" );
    this->setContentSize ( pHudBackground->getContentSize ( ) );
    this->addChild ( pHudBackground );
    
	GameManager*	pGameManager = GameManager::sharedGameManager ( );
	DeviceType		eDeviceType  = pGameManager->getDeviceType ( );
	DeviceSize		eDeviceSize  = pGameManager->getDeviceSize ( );

    KDfloat			fLabelScaleText = 1.0f;
    if ( eDeviceType == kDeviceTypeAndroid854x480 ||
         eDeviceType == kDeviceTypeAndroid800x480 ||
         eDeviceType == kDeviceTypeAndroid800x400 ||
         eDeviceType == kDeviceTypeAndroid720x480 )
    {
        fLabelScaleText = 0.7f;
    }
    else if ( eDeviceSize == kDeviceSizeNormal )
    {
        fLabelScaleText = 0.8f;
    }
    else if ( eDeviceSize == kDeviceSizeSmall )
    {
        fLabelScaleText = 0.45f;
    }
    
    m_pLivesLabel = CCLabelBMFont::create ( ccszf ( "%d", pGameManager->getLives ( ) ), "myGlyphs.fnt" );
    m_pLivesLabel->retain ( );
    m_pLivesLabel->setScale ( fLabelScaleText );
    m_pLivesLabel->setTag ( kLabel_Lives );
    if ( eDeviceType == kDeviceTypeiPhone5 )
    {
        m_pLivesLabel->setPosition ( ccp ( - ( SCREEN_WIDTH / 4.4 ), m_pLivesLabel->getPosition ( ).y - 4 ) );
    }
    else if ( eDeviceType == kDeviceTypeAndroid1280x800 ||
              eDeviceType == kDeviceTypeAndroid1280x720 )
    {
        m_pLivesLabel->setPosition ( ccp ( - ( SCREEN_WIDTH / 4.9 ), m_pLivesLabel->getPosition ( ).y - 4 ) );
    }
    else
    {
        m_pLivesLabel->setPosition ( ccp ( - ( SCREEN_WIDTH / 3.7 ), m_pLivesLabel->getPosition ( ).y - 4 ) );
    }
    this->addChild ( m_pLivesLabel );
    
    m_pScoreLabel = CCLabelBMFont::create ( ccszf ( "%d", pGameManager->getScore ( ) ), "myGlyphs.fnt" );
    m_pScoreLabel->retain ( );
    m_pScoreLabel->setScale ( fLabelScaleText );
    m_pScoreLabel->setTag ( kLabel_Score );
    if ( eDeviceType == kDeviceTypeiPhone5 )
    {
        m_pScoreLabel->setPosition ( ccp ( ( SCREEN_WIDTH / 2 )- ( SCREEN_WIDTH / 2.1 ), m_pScoreLabel->getPosition ( ).y - 4 ) );
    }
    else if ( eDeviceType == kDeviceTypeAndroid1280x800 ||
              eDeviceType == kDeviceTypeAndroid1280x720 )
    {
        m_pScoreLabel->setPosition ( ccp ( ( SCREEN_WIDTH / 2 )- ( SCREEN_WIDTH / 2.09 ), m_pScoreLabel->getPosition ( ).y - 4 ) );
    }    
    else
    {
        m_pScoreLabel->setPosition ( ccp ( ( SCREEN_WIDTH / 2 )- ( SCREEN_WIDTH / 2.13 ), m_pScoreLabel->getPosition ( ).y - 4 ) );
    }
    this->addChild ( m_pScoreLabel );
    
    m_pDistanceLabel = CCLabelBMFont::create ( ccszf ( "%d", pGameManager->getDistance ( ) ), "myGlyphs.fnt" );
    m_pDistanceLabel->retain ( );
    m_pDistanceLabel->setScale ( fLabelScaleText );
    m_pDistanceLabel->setTag ( kLabel_Distance );
    if ( eDeviceType == kDeviceTypeiPhone5 )
    {
        m_pDistanceLabel->setPosition ( ccp ( ( SCREEN_WIDTH / 2 )- ( SCREEN_WIDTH / 4.4 ), m_pDistanceLabel->getPosition ( ).y - 4 ) );
    }
    else if ( eDeviceType == kDeviceTypeAndroid1280x800 ||
              eDeviceType == kDeviceTypeAndroid1280x720 )
    {
        m_pDistanceLabel->setPosition ( ccp ( ( SCREEN_WIDTH / 2 )- ( SCREEN_WIDTH / 3.85 ), m_pDistanceLabel->getPosition ( ).y - 4 ) );
    }
    else
    {
        m_pDistanceLabel->setPosition ( ccp ( ( SCREEN_WIDTH / 2 )- ( SCREEN_WIDTH / 5.65 ), m_pDistanceLabel->getPosition ( ).y - 4 ) );
    }
    this->addChild ( m_pDistanceLabel );
    
    return KD_TRUE;
}

