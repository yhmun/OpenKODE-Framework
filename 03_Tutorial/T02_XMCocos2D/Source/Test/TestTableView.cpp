/* --------------------------------------------------------------------------
 *
 *      File            TestTableView.cpp
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
#include "TestTableView.h"

KDvoid CustomTableViewCell::draw ( KDvoid )
{
	CCTableViewCell::draw ( );
}

KDbool TestTableView::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

    CCTableView*  pTableView = CCTableView::create ( this, CCSizeMake ( tLyrSize.cx / 3, 60 ) );
	this->addChild ( pTableView );
    pTableView->setDirection ( kCCScrollViewDirectionHorizontal );
    pTableView->setPosition ( ccp ( tLyrSize.cx / 6, tLyrSize.cy / 2 - 30 ) );
    pTableView->setDelegate ( this );   
	
	pTableView = CCTableView::create ( this, CCSizeMake ( 60, tLyrSize.cy / 2 ) );
	this->addChild ( pTableView );
	pTableView->setDirection ( kCCScrollViewDirectionVertical );
	pTableView->setPosition ( ccp ( tLyrSize.cx * 2 / 3, tLyrSize.cy / 4 ) );
	pTableView->setDelegate ( this );
	pTableView->setVerticalFillOrder ( kCCTableViewFillTopDown );
	pTableView->setTouchPriority ( -129 );

	return KD_TRUE;
}

KDvoid TestTableView::tableCellTouched ( CCTableView* pTable, CCTableViewCell* pCell )
{
	CCLOG ( "cell touched at index : %i", pCell->getIdx ( ) );
}

CCSize TestTableView::tableCellSizeForIndex ( CCTableView* pTable, KDuint uIdx )
{
	if ( uIdx == 2 ) 
	{
        return CCSizeMake ( 100, 100 );
    }
	return CCSizeMake ( 60, 60 );
}

CCTableViewCell* TestTableView::tableCellAtIndex ( CCTableView* pTable, KDuint uIdx )
{
    CCString*  pString = CCString::createWithFormat ( "%d", uIdx );
    CCTableViewCell*  pCell = pTable->dequeueCell ( );
    if ( !pCell ) 
	{
		CCLOG ( "New Cell %p, %d", pTable, uIdx );

        pCell = new CustomTableViewCell ( );
        pCell->autorelease ( );

        CCSprite*  pSprite = CCSprite::create ( "Images/Icon.png" );
        pSprite->setAnchorPoint ( CCPointZero );
        pSprite->setPosition ( CCPointZero );
        pCell->addChild ( pSprite );

        CCLabelTTF*  pLabel = CCLabelTTF::create ( pString->getCString ( ), "fonts/Marker Felt.ttf", 20.0f );
        pLabel->setPosition ( CCPointZero );
		pLabel->setAnchorPoint ( CCPointZero );
        pLabel->setTag ( 123 );
        pCell->addChild ( pLabel );
    }
    else
    {
		CCLOG ( "Old Cell %p, %d", pTable, uIdx );

        CCLabelTTF*  pLabel = (CCLabelTTF*) pCell->getChildByTag ( 123 );
        pLabel->setString ( pString->getCString ( ) );
    }

    return pCell;
}

KDuint TestTableView::numberOfCellsInTableView ( CCTableView* pTable )
{
	return 20;
}