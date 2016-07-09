/* --------------------------------------------------------------------------
 *
 *      File            ScrMain.cpp
 *      Description     Main Scene
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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
#include "../Manager/EnvManager.h"
#include "../Manager/ResManager.h"
#include "../Manager/ScrManager.h"

#include "ScrMain.h"

KDbool CScrMain::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pBG = g_pResMgr->getSprite ( this, eIMG_BG_Main );

    pBG->setPosition ( ccpMid ( tWinSize ) );
		
	return KD_TRUE;
}

KDvoid CScrMain::onEnterTransitionDidFinish ( KDvoid )
{
	CCLayer::onEnterTransitionDidFinish ( );

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDint  i;

	CCMenu*  pMenu = CCMenu::create ( );
	do
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		for ( i = 0; i < 5; i++ )
		{
			CCMenuItemImage*  pBtn = CCMenuItemImage::create
			(
				g_pResMgr->getPath ( eIMG_BTN_Main_Nor, i ),
				g_pResMgr->getPath ( eIMG_BTN_Main_Sel, i ),
				this, menu_selector ( CScrMain::onButton )
			);

			pMenu->addChild ( pBtn, i, i );
		}

		pMenu->setPosition ( ccpMid ( tWinSize ) );
		pMenu->alignItemsVertically ( );

		CCArray*    pArray = pMenu->getChildren ( );
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( pArray, pObject )
		{
			if ( pObject == KD_NULL )
			{
				break;
			}

			CCNode*  pChild = (CCNode*) pObject;
			CCPoint  tPoint = pChild->getPosition( );
			KDint   nOffset = (KDint) ( tWinSize.cx / 2 + 200 );

			if ( i % 2 == 0 )
			{
				nOffset = -nOffset;
			}
			
			pChild->setPosition ( ccp ( tPoint.x + nOffset, tPoint.y ) );
			pChild->runAction
			( 
				CCEaseElasticOut::create 
				(
					CCMoveBy::create ( 2, ccp ( tPoint.x - nOffset, 0 ) ), 0.4f
				) 
			);

			i++;
		}

	} while ( 0 );
}

KDvoid CScrMain::onButton ( CCObject* pSender )
{
	CCNode*  pNode = (CCNode*) pSender;

    g_pEnvMgr->setStage ( pNode->getTag ( ) );
    g_pScrMgr->setScreen ( eScrLoading, eTranProgressInOut );
}
