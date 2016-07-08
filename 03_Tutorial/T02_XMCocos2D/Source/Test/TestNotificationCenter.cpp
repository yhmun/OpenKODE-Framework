/* --------------------------------------------------------------------------
 *
 *      File            TestNotificationCenter.cpp
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
#include "TestNotificationCenter.h"

#define kTagLight			100
#define kTagConnect			200

#define MSG_SWITCH_STATE	"SwitchState"

KDbool TestNotificationCenter::init ( KDvoid )
{	
	m_bShowImage = KD_FALSE;

	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

    const CCSize&   s = this->getContentSize ( );

    CCLabelTTF*      label1 = CCLabelTTF::create ( "switch off", "fonts/Marker Felt.ttf", 26 );
    CCLabelTTF*      label2 = CCLabelTTF::create ( "switch on" , "fonts/Marker Felt.ttf", 26 );
    CCMenuItemLabel*  item1 = CCMenuItemLabel::create ( label1 );
    CCMenuItemLabel*  item2 = CCMenuItemLabel::create ( label2 );
    CCMenuItemToggle*  item = CCMenuItemToggle::createWithTarget ( this, menu_selector ( TestNotificationCenter::toggleSwitch ), item1, item2, KD_NULL );
    
	// turn on
    item->setSelectedIndex ( 1 );
    CCMenu*  menu = CCMenu::create ( item, KD_NULL );
    menu->setPosition ( ccp ( s.cx / 2 + 100, s.cy / 2 ) );
    addChild ( menu );

    CCMenu*   menuConnect = CCMenu::create ( );
    addChild ( menuConnect );

    for ( KDint i = 1; i <= 3; i++ )
    {
        Light*  light = Light::create ( "Images/Pea.png" );
        light->setTag ( kTagLight + i );
        light->setPosition ( ccp ( s.cx / 2 - 100, s.cy / 4 * i - 50 ) );
        addChild ( light );

        CCLabelTTF*      label1 = CCLabelTTF::create ( "not connected", "fonts/Marker Felt.ttf", 26 );
        CCLabelTTF*      label2 = CCLabelTTF::create ( "connected"    , "fonts/Marker Felt.ttf", 26 );
        CCMenuItemLabel*  item1 = CCMenuItemLabel::create ( label1 );
        CCMenuItemLabel*  item2 = CCMenuItemLabel::create ( label2 );
        CCMenuItemToggle*  item = CCMenuItemToggle::createWithTarget ( this, menu_selector ( TestNotificationCenter::connectToSwitch ), item1, item2, KD_NULL );
        item->setTag ( kTagConnect + i );
        item->setPosition ( ccp ( light->getPosition ( ).x, light->getPosition ( ).y + 50 ) );
        menuConnect->addChild ( item, 0 );
        if ( i == 2 )
        {
            item->setSelectedIndex ( 1 );
        }
        KDbool  bConnected = item->getSelectedIndex ( ) == 1 ? KD_TRUE : KD_FALSE;
        light->setIsConnectToSwitch ( bConnected );
    }

    CCNotificationCenter::sharedNotificationCenter ( )->postNotification ( MSG_SWITCH_STATE, (CCObject*) item->getSelectedIndex ( ) );

    // for testing removeAllObservers 
    CCNotificationCenter::sharedNotificationCenter ( )->addObserver ( this, callfuncO_selector ( TestNotificationCenter::doNothing ), "random-observer1", KD_NULL );
    CCNotificationCenter::sharedNotificationCenter ( )->addObserver ( this, callfuncO_selector ( TestNotificationCenter::doNothing ), "random-observer2", KD_NULL );
    CCNotificationCenter::sharedNotificationCenter ( )->addObserver ( this, callfuncO_selector ( TestNotificationCenter::doNothing ), "random-observer3", KD_NULL );

	return KD_TRUE;
}

KDvoid TestNotificationCenter::toggleSwitch ( CCObject* pSender )
{
    CCMenuItemToggle*  pItem  = (CCMenuItemToggle*) pSender;
    KDint              nIndex = pItem->getSelectedIndex ( );

    CCNotificationCenter::sharedNotificationCenter ( )->postNotification ( MSG_SWITCH_STATE, (CCObject*) nIndex );
}

KDvoid TestNotificationCenter::connectToSwitch ( CCObject* pSender )
{
    CCMenuItemToggle*  pItem = (CCMenuItemToggle*) pSender;
    KDbool        bConnected = pItem->getSelectedIndex ( ) == 0 ? KD_FALSE : KD_TRUE;
    Light*            pLight = (Light*) this->getChildByTag ( pItem->getTag ( ) - kTagConnect + kTagLight );
    pLight->setIsConnectToSwitch ( bConnected );
}

KDvoid TestNotificationCenter::doNothing ( CCObject* pSender )
{

}

KDbool Light::s_bSwitchOn = KD_FALSE;

Light::Light ( KDvoid )
{
	m_bConnected = KD_FALSE;
}

Light::~Light ( KDvoid )
{
    CCNotificationCenter::sharedNotificationCenter ( )->removeObserver ( this, MSG_SWITCH_STATE );
}

Light* Light::create ( const KDchar* szName )
{
    Light*  pRet = new Light ( );
	
	if ( pRet && pRet->initWithFile ( szName ) )
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE ( szName );
	}
    
    return pRet;
}

KDvoid Light::setIsConnectToSwitch ( KDbool bConnectToSwitch )
{
    m_bConnected = bConnectToSwitch;

    if ( m_bConnected )
    {
        CCNotificationCenter::sharedNotificationCenter ( )->addObserver ( this, callfuncO_selector ( Light::switchStateChanged ), MSG_SWITCH_STATE, KD_NULL );
    }
    else
    {
        CCNotificationCenter::sharedNotificationCenter ( )->removeObserver ( this, MSG_SWITCH_STATE );
    }

    updateLightState ( );
}

KDvoid Light::switchStateChanged ( CCObject* pObject )
{
    KDint  nIndex = (KDint) pObject;

    s_bSwitchOn = nIndex == 0 ? KD_FALSE : KD_TRUE;
    
	updateLightState ( );
}

KDvoid Light::updateLightState ( KDvoid )
{
    if ( s_bSwitchOn && m_bConnected )
    {
        this->setOpacity ( 255 );
    }
    else
    {
        this->setOpacity ( 50 );
    }
}
