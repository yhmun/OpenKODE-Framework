/* --------------------------------------------------------------------------
 *
 *      File            TestAssetsManager.cpp
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
#include "TestAssetsManager.h"

KDbool TestAssetsManager::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 32 );

    CCMenuItemFont*  pItemReset  = CCMenuItemFont::create ( "Reset" , this, menu_selector ( TestAssetsManager::onReset  ) );
    CCMenuItemFont*  pItemEnter  = CCMenuItemFont::create ( "Enter" , this, menu_selector ( TestAssetsManager::onEnter  ) );
    CCMenuItemFont*  pItemUpdate = CCMenuItemFont::create ( "Update", this, menu_selector ( TestAssetsManager::onUpdate ) );
    
    pItemEnter ->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 + 50 ) );
    pItemReset ->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2      ) );
    pItemUpdate->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 - 50 ) );
    
    CCMenu*  pMenu = CCMenu::create ( pItemUpdate, pItemEnter, pItemReset, KD_NULL );
    this->addChild ( pMenu );

	// Create Download Directory
	std::string  sPath = CCFileUtils::sharedFileUtils ( )->getWritablePath ( );
    sPath += "TempDir";
	kdMkdir ( sPath.c_str ( ) );

	m_pAssetsManager = new AssetsManager
	(
		"https://raw.github.com/minggo/AssetsManagerTest/master/package.zip",
		"https://raw.github.com/minggo/AssetsManagerTest/master/version",
		sPath.c_str ( )
	);

	return KD_TRUE;
}

KDvoid TestAssetsManager::onEnter ( CCObject* pSender )
{
//    CCScriptEngineProtocol*  pEngine = ScriptingCore::getInstance ( );
//    CCScriptEngineManager::sharedManager ( )->setScriptEngine ( pEngine );
//    ScriptingCore::getInstance ( )->runScript ( "main.js" );
}

KDvoid TestAssetsManager::onReset ( CCObject* pSender )
{
//	m_pAssetsManager->deleteVersion ( );
}

KDvoid TestAssetsManager::onUpdate ( CCObject* pSender )
{
//	m_pAssetsManager->update ( );
}