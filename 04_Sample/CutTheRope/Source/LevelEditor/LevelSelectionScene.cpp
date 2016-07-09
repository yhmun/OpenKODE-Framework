/* --------------------------------------------------------------------------
 *
 *      File            LevelSelectionScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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
#include "LevelSelectionScene.h"
#include "LevelFileHandler.h"
#include "GameScene.h"
#include "Utils/GameManager.h"

LevelSelectionScene::LevelSelectionScene ( KDvoid )
{
    CCArray*	pItems = CCArray::create ( );
    
    KDint		nLevelNumber = 0;
    KDbool		bFileExists;
    
	CCMenuItemFont::setFontName ( "font/Marker Felt.ttf" );

	do 
	{
        CCFileUtils*	pFileUtils  = CCFileUtils::sharedFileUtils ( );
        const KDchar*	szLevelName = CCString::createWithFormat ( "level%d.xml", nLevelNumber )->getCString ( );
        bFileExists = pFileUtils->isFileExist ( pFileUtils->getWritablePath ( ) + szLevelName );
        CCMenuItem*		pItem;
        if ( bFileExists )
		{
            CCString*	pItemLabel = CCString::createWithFormat ( "Level %d", nLevelNumber );
            pItem = CCMenuItemFont::create ( pItemLabel->getCString ( ), this, menu_selector ( LevelSelectionScene::selectLevel ) );
        }
		else
		{
            pItem = CCMenuItemFont::create ( "New Level", this, menu_selector ( LevelSelectionScene::selectLevel ) );
        }
        pItem->setTag ( nLevelNumber );
        pItems->addObject ( pItem );
        
        nLevelNumber++;

    } while ( bFileExists );
    
    CCMenu*		pMenu = CCMenu::createWithArray ( pItems );
    pMenu->alignItemsVertically ( );
    pMenu->setPosition ( ccp ( 160, 240 ) );    
    this->addChild ( pMenu );
}

KDvoid LevelSelectionScene::selectLevel ( CCObject* pSender )
{
    CCMenuItem*		pItem = (CCMenuItem*) pSender;
    KDint			nLevelNumber = pItem->getTag ( );
//	CCLOG ( "n = %d", nLevelNumber );

    CCFileUtils*	pFileUtils  = CCFileUtils::sharedFileUtils ( );
    const KDchar*	szLevelName = CCString::createWithFormat ( "level%d.xml", nLevelNumber )->getCString ( );
    KDbool			bFileExists = pFileUtils->isFileExist ( pFileUtils->getWritablePath ( ) + szLevelName );    
    
    LevelFileHelper*	pFileHandler;
    if ( bFileExists ) 
	{
        pFileHandler = new LevelFileHelper ( ( pFileUtils->getWritablePath ( ) + szLevelName ).c_str ( ) );
    }
	else
	{
        pFileHandler = new LevelFileHelper ( pFileUtils->fullPathForFilename ( kLevelName ).c_str ( ) );
    }
    
    
    GameManager::getInstance ( )->m_nLevelNumber = nLevelNumber;
    
	CCDirector::sharedDirector ( )->replaceScene ( GameScene::GameSceneWithLevelHandler ( pFileHandler ) );
}

LevelSelectionScene::~LevelSelectionScene ( KDvoid )
{
    
}

CCScene* LevelSelectionScene::scene ( KDvoid )
{
    CCScene*				pScene = CCScene::create ( );    
    LevelSelectionScene*	pLayer = new LevelSelectionScene ( );
    
    pScene->addChild ( pLayer );
    
    return pScene;
}