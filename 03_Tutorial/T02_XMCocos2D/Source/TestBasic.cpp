/* --------------------------------------------------------------------------
 *
 *      File            TestBasic.cpp
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
#include "TestBasic.h"
#include "TestList.h"

KDuint TestBasic::s_nSubTestIndex = 0;

KDbool TestBasic::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCLayerGradient*  pBG = CCLayerGradient::create ( ccc4 ( 52, 84, 236, 200 ), ccc4 ( 149, 0, 202, 200 ) );	
	pBG->setContentSize ( tLyrSize );
	this->addChild ( pBG, -10 );

	CCSprite*  pBottomLeft = CCSprite::create ( "ccb/jungle-left.png" );
	this->addChild ( pBottomLeft, 10 );
	pBottomLeft->setScale ( 1.5f );
	pBottomLeft->setOpacity ( 210 );
	pBottomLeft->setPositionWithParent ( kCCAlignmentBottomLeft );

	CCSprite*  pBottomRight = CCSprite::create ( "ccb/jungle-right.png" );
	this->addChild ( pBottomRight, 10 );
	pBottomRight->setScale ( 1.5f );
	pBottomRight->setOpacity ( 210 );
	pBottomRight->setPositionWithParent ( kCCAlignmentBottomRight );

	CCLayerColor*  pTitleBG = CCLayerColor::create ( ccc4 ( 0, 0, 0, 127 ), ccs ( tLyrSize.cx, 50 ) );
	this->addChild ( pTitleBG, 10 );
	pTitleBG->setPositionWithParent ( kCCAlignmentTop );

	CCMenuItemImage*  pPrev = CCMenuItemImage::create 
	(
		"ccb/btn-back-0.png", "ccb/btn-back-1.png", this, menu_selector ( TestBasic::onTestList ) 
	);
	pPrev->setScale ( 1.2f );
	pPrev->setPosition ( pTitleBG, kCCAlignmentLeft, ccp ( 10, 0 ) );
	pTitleBG->addChild ( CCMenu::createWithItem ( pPrev ), 10 );

	CCLabelTTF*  pTitle = CCLabelTTF::create
	(
		this->title ( ), "fonts/arial.ttf", 26
	);
	pTitleBG->addChild ( pTitle, 10 );
	pTitle->setPositionWithParent ( kCCAlignmentCenter );
	pTitle->setColor ( ccGREEN );

	CCLabelTTF*  pSubTitle = CCLabelTTF::create
	(
		this->subtitle ( ), "fonts/tahoma.ttf", 22
	);
	this->addChild ( pSubTitle, 10 );
	pSubTitle->setPositionWithParent ( kCCAlignmentTop, ccp ( 0, 55 ) );	
	pSubTitle->setColor ( ccYELLOW );	

	if ( count ( ) > 0 )
	{
		CCMenu*  pMenu = KD_NULL;

		if ( count ( ) == 1 )
		{
			pMenu = CCMenu::createWithItem
			(
				CCMenuItemImage::create ( "Images/r1.png", "Images/r2.png", this, menu_selector ( TestBasic::onRestart ) )			
			);
		}
		else
		{
			pMenu = CCMenu::create
			(
				CCMenuItemImage::create ( "Images/b1.png", "Images/b2.png", this, menu_selector ( TestBasic::onPrev    ) ),
				CCMenuItemImage::create ( "Images/r1.png", "Images/r2.png", this, menu_selector ( TestBasic::onRestart ) ),
				CCMenuItemImage::create ( "Images/f1.png", "Images/f2.png", this, menu_selector ( TestBasic::onNext    ) ),				
				KD_NULL
			);
		}
		this->addChild ( pMenu, 10 );	
		pMenu->setContentSize ( tLyrSize );
		pMenu->setPosition ( ccs ( tLyrSize.cx / 2, 35 ) );
		pMenu->alignItemsHorizontally ( 30 );	

        CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( pMenu->getChildren ( ), pObject )
		{
			CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( pObject );
			if ( pRGBAProtocol )
			{
				pRGBAProtocol->setOpacity ( 210 );
			}
		}
	}

	m_pBG = pBG;
	m_pTitle = pTitle;
	m_pSubTitle = pSubTitle;

	return KD_TRUE;
}

const KDchar* TestBasic::title ( KDvoid )
{
	return TestList::getName ( TestList::s_nTestIndex );
}

const KDchar* TestBasic::subtitle ( KDvoid )
{
	return "";
}

KDuint TestBasic::count ( KDvoid )
{
	return 0;
}

KDvoid TestBasic::onTestList ( CCObject* pSender )
{
	s_nSubTestIndex = 0;

	TestList::setScene ( -1, eDirClose );
}

KDvoid TestBasic::onPrev ( CCObject* pSender )
{	
	s_nSubTestIndex = s_nSubTestIndex == 0 ? count ( ) - 1 : s_nSubTestIndex - 1;
	
	TestList::setScene ( TestList::s_nTestIndex, eDirPrev );
}	

KDvoid TestBasic::onNext ( CCObject* pSender )
{
	s_nSubTestIndex = s_nSubTestIndex == ( count ( ) - 1 ) ? 0 : s_nSubTestIndex + 1;
	
	TestList::setScene ( TestList::s_nTestIndex, eDirNext );
}

KDvoid TestBasic::onRestart ( CCObject* pSender )
{	
	TestList::setScene ( TestList::s_nTestIndex, eDirRestart );
}
