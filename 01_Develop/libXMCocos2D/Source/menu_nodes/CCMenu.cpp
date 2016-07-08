/* -----------------------------------------------------------------------------------
 *
 *      File            CCMenu.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "menu_nodes/CCMenu.h"
#include "cocoa/CCSet.h"
#include "cocoa/CCString.h"
#include "cocoa/CCInteger.h"
#include "support/CCDirector.h"
#include "support/CCPointExtension.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouch.h"
#include <vector>

NS_CC_BEGIN
    
static std::vector<KDuint> ccarray_to_std_vector ( CCArray* pArray )
{
    std::vector<KDuint>  aRet;
    
	CCObject*  pObject;
    CCARRAY_FOREACH ( pArray, pObject )
    {
        CCInteger*  pInteger = (CCInteger*) pObject;
        aRet.push_back ( (KDuint) pInteger->getValue ( ) );
    }

    return aRet;
}

//
// CCMenu
//
CCMenu::CCMenu ( KDvoid )
{
	m_pSelectedItem = KD_NULL;	
	m_bSwallow      = KD_TRUE;
	m_bAutoSwallow  = KD_FALSE;
}

CCMenu::~CCMenu ( KDvoid )
{

}

CCMenu* CCMenu::create ( KDvoid )
{
    return CCMenu::create ( KD_NULL, KD_NULL );
}

CCMenu* CCMenu::create ( CCMenuItem* pItem, ... )
{
	KDVaListKHR       pArgs;
	KD_VA_START_KHR ( pArgs, pItem );
    
    CCMenu*  pRet = CCMenu::createWithItems ( pItem, pArgs );
    
    KD_VA_END_KHR ( pArgs );

    return pRet;
}

CCMenu* CCMenu::createWithItems ( CCMenuItem* pItem, KDVaListKHR pArgs )
{
	CCArray*  pArray = NULL;
    if ( pItem )
    {
        pArray = CCArray::create ( pItem, KD_NULL );
        CCMenuItem*  i = KD_VA_ARG ( pArgs, CCMenuItem* );
        while ( i )
        {
            pArray->addObject ( i );
            i = KD_VA_ARG ( pArgs, CCMenuItem* );
        }
    }
    
    return CCMenu::createWithArray ( pArray );
}

CCMenu* CCMenu::createWithArray ( CCArray* pArrayOfItems )
{
    CCMenu*  pRet = new CCMenu ( );

    if ( pRet && pRet->initWithArray ( pArrayOfItems ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

CCMenu* CCMenu::createWithItem ( CCMenuItem* pItem )
{
	return CCMenu::create ( pItem, KD_NULL );
}

KDbool CCMenu::init ( KDvoid )
{
	return initWithArray ( KD_NULL );
}

KDbool CCMenu::initWithArray ( CCArray* pArrayOfItems )
{
    if ( CCLayer::init ( ) )
    {
		setTouchPriority ( kCCMenuHandlerPriority );
        setTouchMode     ( kCCTouchesOneByOne );
        setTouchEnabled  ( KD_TRUE );

        m_bEnabled = KD_TRUE;  

        if ( pArrayOfItems != KD_NULL )
        {
            KDint      nZOrder = 0;
            CCObject*  pObject = KD_NULL;
            CCARRAY_FOREACH ( pArrayOfItems, pObject )
            {
                CCMenuItem*  pItem = (CCMenuItem*) pObject;
                this->addChild ( pItem, nZOrder );
                nZOrder++;
            }
        }
    
        m_pSelectedItem = KD_NULL;
        m_eState = kCCMenuStateWaiting;

		// enable cascade color and opacity on menus
        setCascadeColorEnabled ( KD_TRUE );
        setCascadeOpacityEnabled ( KD_TRUE );

        return KD_TRUE;
    }

    return KD_FALSE;
}

//
// override add:
//

KDvoid CCMenu::addChild ( CCNode* pChild )
{
    CCLayer::addChild ( pChild );
}

KDvoid CCMenu::addChild ( CCNode* pChild, KDint nZOrder )
{
	CCLayer::addChild ( pChild, nZOrder );
}

KDvoid CCMenu::addChild ( CCNode* pChild, KDint nZOrder, KDint nTag )
{
    CCAssert ( dynamic_cast<CCMenuItem*> ( pChild ) != KD_NULL, "Menu only supports MenuItem objects as children" );
	CCLayer::addChild ( pChild, nZOrder, nTag );
}

KDvoid CCMenu::onExit ( KDvoid )
{
    if ( m_eState == kCCMenuStateTrackingTouch )
    {
		if ( m_pSelectedItem )
		{
			m_pSelectedItem->unselected ( );
			m_pSelectedItem = KD_NULL;
		}

        m_eState = kCCMenuStateWaiting;     
    }

    CCLayer::onExit ( );
}

KDvoid CCMenu::removeChild ( CCNode* pChild, KDbool bCleanup )
{
    CCMenuItem*  pMenuItem = dynamic_cast<CCMenuItem*> ( pChild );
    CCAssert ( pMenuItem != KD_NULL, "Menu only supports MenuItem objects as children" );
    
    if ( m_pSelectedItem == pMenuItem )
    {
        m_pSelectedItem = KD_NULL;
    }
    
    CCNode::removeChild ( pChild, bCleanup );
}

KDvoid CCMenu::setHandlerPriority ( KDint nPriority )
{
    CCTouchDispatcher*  pDispatcher = CCDirector::sharedDirector ( )->getTouchDispatcher ( );
    pDispatcher->setPriority ( nPriority, this );
	m_nTouchPriority = nPriority;
}

KDvoid CCMenu::registerWithTouchDispatcher ( KDvoid )
{
    CCDirector*  pDirector = CCDirector::sharedDirector ( );
    pDirector->getTouchDispatcher ( )->addTargetedDelegate ( this, this->getTouchPriority ( ), KD_TRUE );
}

KDvoid CCMenu::setHandlerSwallow ( KDbool bOn )
{
    CCTouchDispatcher*  pDispatcher = CCDirector::sharedDirector ( )->getTouchDispatcher ( );
    pDispatcher->setTargetedSwallow ( bOn, this );
	m_bSwallow = bOn;
}

KDvoid CCMenu::setHandlerAutoSwallow ( KDbool bOn, KDfloat fGap )
{
	m_bAutoSwallow = bOn;
    m_fAutoSwallowGap = fGap;
}

/// Menu - Events

KDbool CCMenu::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CC_UNUSED_PARAM ( pEvent );

	if ( m_eState != kCCMenuStateWaiting || !m_bVisible || !m_bEnabled )
	{
		return KD_FALSE;
	}

	for ( CCNode*  pParent = this->m_pParent; pParent != KD_NULL; pParent = pParent->getParent ( ) )
	{
		if ( pParent->isVisible ( ) == KD_FALSE )
		{
			return KD_FALSE;
		}
	}

	m_pSelectedItem = this->itemForTouch ( pTouch );
	if ( m_pSelectedItem )
	{
		m_eState = kCCMenuStateTrackingTouch;
		m_pSelectedItem->selected ( );

		if ( m_bSwallow && m_bAutoSwallow )
		{
			CCTouchDispatcher*  pDispatcher = CCDirector::sharedDirector ( )->getTouchDispatcher ( );

			pDispatcher->setTargetedSwallow ( KD_TRUE, this );
            
            m_tPrevLocation = pTouch->getLocation ( );
		}

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CCMenu::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	CC_UNUSED_PARAM ( pTouch );
	CC_UNUSED_PARAM ( pEvent );

	CCAssert ( m_eState != kCCMenuStateWaiting, "CCMenu::ccTouchEnded -- invalid state" );

	if ( m_pSelectedItem && m_eState == kCCMenuStateTrackingTouch )
	{
		m_pSelectedItem->unselected ( );
		m_pSelectedItem->activate ( );
	}

	m_eState = kCCMenuStateWaiting;
}

KDvoid CCMenu::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	CC_UNUSED_PARAM ( pEvent );
    
	CCAssert ( m_eState != kCCMenuStateWaiting, "CCMenu::ccTouchMoved -- invalid state" );

	if ( m_eState == kCCMenuStateAutoSwallow )
	{
		return;
	}

	if ( m_bSwallow && m_bAutoSwallow )
	{
		CCPoint   tLocation = pTouch->getLocation ( );			
		CCPoint   tDiff = ccpSub ( tLocation, m_tPrevLocation );
		
		if ( kdFabsf ( tDiff.x ) > m_fAutoSwallowGap || kdFabsf ( tDiff.y ) > m_fAutoSwallowGap )
		{
			m_eState = kCCMenuStateAutoSwallow;

			if ( m_pSelectedItem )
			{
				m_pSelectedItem->unselected ( );
			}

			CCSet  tSet;
			tSet.addObject ( pTouch );
            
			CCTouchDispatcher*  pDispatcher = CCDirector::sharedDirector ( )->getTouchDispatcher ( );

			pDispatcher->setTargetedSwallow ( KD_FALSE, this );
			pDispatcher->touchesBegan ( &tSet, KD_NULL );
			
			return;
		}
	}

	CCMenuItem*  pSelectedItem = this->itemForTouch ( pTouch );

	if ( pSelectedItem != m_pSelectedItem ) 
	{
		if ( m_pSelectedItem )
		{
			m_pSelectedItem->unselected ( );
		}

		m_pSelectedItem = pSelectedItem;

		if ( m_pSelectedItem )
		{
			m_pSelectedItem->selected ( );
		}
	}
}

// Menu - Alignment
KDvoid CCMenu::alignItemsVertically ( KDfloat fPadding )
{
	this->alignItemsVerticallyWithPadding ( fPadding );
}

KDvoid CCMenu::alignItemsVerticallyWithPadding ( KDfloat fPadding )
{
	KDfloat  fHeight = -fPadding;
	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pChild = dynamic_cast<CCNode*> ( pObject );
			if ( pChild )
			{
				fHeight += pChild->getContentSize ( ).height * pChild->getScaleY ( ) + fPadding;
			}
		}		
	}

	KDfloat  fY = fHeight / 2.0f;
	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pChild = dynamic_cast<CCNode*> ( pObject );
			if ( pChild )
			{
				pChild->setPosition ( ccp ( 0, fY - pChild->getContentSize ( ).height * pChild->getScaleY ( ) / 2.0f ) );
				fY -= pChild->getContentSize ( ).height * pChild->getScaleY ( ) + fPadding;
			}
		}
	}
}

KDvoid CCMenu::alignItemsHorizontally ( KDfloat fPadding )
{
	this->alignItemsHorizontallyWithPadding ( fPadding );
}

KDvoid CCMenu::alignItemsHorizontallyWithPadding ( KDfloat fPadding )
{
	KDfloat  fWidth = -fPadding;
	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pChild = dynamic_cast<CCNode*> ( pObject );
			if ( pChild )
			{
				fWidth += pChild->getContentSize ( ).width * pChild->getScaleX ( ) + fPadding;
			}
		}
	}

	KDfloat  fX = -fWidth / 2.0f;
	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
        CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pChild = dynamic_cast<CCNode*> ( pObject );
			if ( pChild )
			{
				pChild->setPosition ( ccp ( fX + pChild->getContentSize ( ).cx * pChild->getScaleX ( ) / 2.0f, 0 ) );
				fX += pChild->getContentSize ( ).width * pChild->getScaleX ( ) + fPadding;
			}
		}
	}
}

KDvoid CCMenu::alignItemsInColumns ( KDuint uColumns, ... )
{
	KDVaListKHR       pArgs;
	KD_VA_START_KHR ( pArgs, uColumns );

	this->alignItemsInColumns ( uColumns, pArgs );

	KD_VA_END_KHR ( pArgs );
}

KDvoid CCMenu::alignItemsInColumns ( KDuint uColumns, KDVaListKHR pArgs )
{
    CCArray*  pRows = CCArray::create ( );
    while ( uColumns )
    {
        pRows->addObject ( CCInteger::create ( uColumns ) );
        uColumns = KD_VA_ARG ( pArgs, KDuint );
    }

    alignItemsInColumnsWithArray ( pRows );
}

KDvoid CCMenu::alignItemsInColumnsWithArray ( CCArray* pRowsArray )
{
	std::vector<KDuint> aRows = ccarray_to_std_vector ( pRowsArray );

	KDint   nHeight          = -5;
	KDuint  uRow             = 0;
	KDuint  uRowHeight       = 0;
	KDuint  uColumnsOccupied = 0;
	KDuint  uRowColumns      = 0;

	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pChild = dynamic_cast<CCNode*> ( pObject );
			
			if ( pChild )
			{
				CCAssert ( uRow < aRows.size ( ), "" );
				
				uRowColumns = aRows [ uRow ];
				
				// can not have zero columns on a row
				CCAssert ( uRowColumns, "" );
				
				KDfloat  fTemp = pChild->getContentSize ( ).height;
				uRowHeight = (KDuint) ( ( uRowHeight >= fTemp || kdIsNan ( fTemp ) ) ? uRowHeight : fTemp );
				
				++uColumnsOccupied;
				if ( uColumnsOccupied >= uRowColumns )
				{
					nHeight += uRowHeight + 5;
					
					uColumnsOccupied = 0;
					uRowHeight = 0;
					++uRow;
				}
			}
		}
	}

	// check if too many rows/columns for available menu items
	CCAssert ( !uColumnsOccupied, "" );

	uRow = 0;
	uRowHeight  = 0;
	uRowColumns = 0;

	KDfloat  fW = 0.0f;
	KDfloat  fX = 0.0f;
	KDfloat  fY = nHeight / 2.0f;

	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pChild = dynamic_cast<CCNode*> ( pObject );
			if ( pChild )
			{
				if ( uRowColumns == 0 )
				{
					uRowColumns = aRows [ uRow ];
					fW = m_tContentSize.cx / ( 1 + uRowColumns );
					fX = fW;
				}
				
				KDfloat  fTemp = pChild->getContentSize ( ).height;
				uRowHeight = (KDuint) ( ( uRowHeight >= fTemp || kdIsNan ( fTemp ) ) ? uRowHeight : fTemp );
				
				pChild->setPosition ( ccp ( fX - m_tContentSize.cx / 2, fY - pChild->getContentSize ( ).height / 2 ) );
				
				fX += fW;
				++uColumnsOccupied;
				
				if ( uColumnsOccupied >= uRowColumns )
				{
					fY -= uRowHeight + 5;
					
					uColumnsOccupied = 0;
					uRowColumns = 0;
					uRowHeight  = 0;
					++uRow;
				}
			}
		}
	}	
}

KDvoid CCMenu::alignItemsInRows ( KDuint uRows, ... )
{
	KDVaListKHR   pArgs;
	KD_VA_START_KHR ( pArgs, uRows );

	this->alignItemsInRows ( uRows, pArgs );

	KD_VA_END_KHR ( pArgs );
}

KDvoid CCMenu::alignItemsInRows ( KDuint uRows, KDVaListKHR pArgs )
{
	CCArray*  pArray = CCArray::create ( );
    while ( uRows )
    {
        pArray->addObject ( CCInteger::create ( uRows ) );
        uRows = KD_VA_ARG ( pArgs, KDuint );
    }
    alignItemsInRowsWithArray ( pArray );
}

KDvoid CCMenu::alignItemsInRowsWithArray ( CCArray* pColumnArray )
{
	std::vector<KDuint> columns = ccarray_to_std_vector ( pColumnArray );

	std::vector<KDuint>  aColumnWidths;
	std::vector<KDuint>  aColumnHeights;

	KDint   nWidth        = -10;
	KDint   nColumnHeight = -5;
	KDuint  uColumn       = 0;
	KDuint  uColumnWidth  = 0;
	KDuint  uRowsOccupied = 0;
	KDuint  uColumnRows   = 0;

	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
		CCObject* pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pChild = dynamic_cast<CCNode*> ( pObject );
			if ( pChild )
			{
				// check if too many menu items for the amount of rows/columns
				CCAssert ( uColumn < columns.size ( ), "" );
				
				uColumnRows = columns [ uColumn ];
				
				// can't have zero rows on a column
				CCAssert ( uColumnRows, "" );
				
				// columnWidth = fmaxf(columnWidth, [item contentSize].width);
				KDfloat  fTemp = pChild->getContentSize ( ).cx;
				uColumnWidth   = (KDuint) ( ( uColumnWidth >= fTemp || kdIsNan ( fTemp ) ) ? uColumnWidth : fTemp );
				nColumnHeight += (KDint) ( pChild->getContentSize ( ).height + 5 );
				++uRowsOccupied;
				
				if ( uRowsOccupied >= uColumnRows )
				{
					aColumnWidths.push_back ( uColumnWidth );
					aColumnHeights.push_back ( nColumnHeight );
					
					nWidth += uColumnWidth + 10;
					uRowsOccupied = 0;
					uColumnWidth  = 0;
					nColumnHeight = -5;
					++uColumn;
				}
			}
		}
	}

	// check if too many rows/columns for available menu items.
	CCAssert ( !uRowsOccupied, "" );

	uColumn      = 0;
	uColumnWidth = 0;
	uColumnRows  = 0;

    CCSize   tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDfloat  fX = -nWidth / 2.0f;
	KDfloat  fY = 0.0f;

    if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
		CCObject* pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pChild = dynamic_cast<CCNode*> ( pObject );
			if ( pChild )
			{
				if ( uColumnRows == 0 )
				{
					uColumnRows = columns [ uColumn ];
					fY = (KDfloat) aColumnHeights [ uColumn ];
				}
				
				// columnWidth = fmaxf(columnWidth, [item contentSize].width);
				KDfloat  fTemp = pChild->getContentSize ( ).width;
				uColumnWidth = (KDuint) ( ( uColumnWidth >= fTemp || kdIsNan ( fTemp ) ) ? uColumnWidth : fTemp );

				pChild->setPosition ( ccp ( fX + aColumnWidths [ uColumn ] / 2, fY - tWinSize.height / 2 ) );

				fY -= pChild->getContentSize ( ).cy + 10;
				++uRowsOccupied;

				if ( uRowsOccupied >= uColumnRows )
				{
					fX += uColumnWidth + 5;
					uRowsOccupied = 0;
					uColumnRows   = 0;
					uColumnWidth  = 0;
					++uColumn;
				}
			}
		}
	}
}

CCMenuItem* CCMenu::itemForTouch ( CCTouch* pTouch )
{
	CCPoint  tLocation = pTouch->getLocation ( );
	
    CCNode*  pParent = this->getParent ( );
	while ( pParent != KD_NULL )
	{
		if ( pParent->isClipping ( ) == KD_TRUE )
		{
			CCPoint  tLocal = pParent->convertTouchToNodeSpace ( pTouch );
			CCRect   tRect  = CCRect ( CCPointZero, pParent->getContentSize ( ) );

			if ( !tRect.containsPoint ( tLocal ) )
			{
				return KD_NULL;
			}
		}

		pParent = pParent->getParent ( );
	}

    if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCMenuItem*  pChild = dynamic_cast<CCMenuItem*> ( pObject );
			if ( pChild && pChild->isVisible ( ) && pChild->isEnabled ( ) )
			{
				CCPoint tLocal = pChild->convertToNodeSpace ( tLocation );
				CCRect   tRect = pChild->rect ( );
                
				tRect.origin = CCPointZero;
				if ( tRect.containsPoint ( tLocal ) )
				{
					return pChild;
				}
			}
		}		
	}

	return KD_NULL;
}


//
// CCMenuRadio
//
CCMenuRadio::CCMenuRadio ( KDvoid )
{
	m_uSelectedIndex = 0;
	m_pListener = KD_NULL;
	m_pSelector = KD_NULL;
}

CCMenuRadio::~CCMenuRadio ( KDvoid )
{

}

CCMenuRadio* CCMenuRadio::create ( KDvoid )
{
    CCMenuRadio*  pRet = new CCMenuRadio ( );
    
	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
    
    return pRet;
}

CCMenuRadio* CCMenuRadio::create ( CCObject* pTarget, SEL_MenuHandler pSelector )
{
    CCMenuRadio*  pRet = new CCMenuRadio ( );
    
	if ( pRet && pRet->initWithTarget ( pTarget, pSelector ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
    
    return pRet;
}

CCMenuRadio* CCMenuRadio::create ( CCObject* pTarget, SEL_MenuHandler pSelector, CCMenuItem* pItem, ... )
{
	KDVaListKHR       pArgs;
	KD_VA_START_KHR ( pArgs, pItem );

	CCMenuRadio*  pRet = new CCMenuRadio ( );

	if ( pRet && pRet->initWithItems ( pTarget, pSelector, pItem, pArgs ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	KD_VA_END_KHR ( pArgs );

	return pRet;
}

KDbool CCMenuRadio::init ( KDvoid )
{
    return initWithTarget ( KD_NULL, KD_NULL );
}

KDbool CCMenuRadio::initWithTarget ( CCObject* pTarget, SEL_MenuHandler pSelector )
{
    if ( !CCMenu::init ( ) )
    {
        return KD_FALSE;
    }
    
    m_eState		= kCCMenuStateWaiting;
    m_pSelectedItem = KD_NULL;
    m_bTouchEnabled = KD_TRUE;
    
	m_pListener = pTarget;
	m_pSelector = pSelector;
    
    return KD_TRUE;
}

KDbool CCMenuRadio::initWithItems ( CCObject* pTarget, SEL_MenuHandler pSelector, CCMenuItem* pItem, KDVaListKHR pArgs )
{
	if ( !CCMenu::init ( ) )
	{
        return KD_FALSE;
    }
    
    KDint        nZOrder = 0;
    CCMenuItem*  pNewItem = pItem;

    while ( pNewItem )
    {
        pNewItem->initWithTarget ( this, menu_selector ( CCMenuRadio::onSelected ) );
			
        addChild ( pNewItem, nZOrder );

        pNewItem = KD_VA_ARG ( pArgs, CCMenuItem* );
        nZOrder++;
    }

//		if ( pItem )
//		{
//			pItem->setEnabled ( KD_FALSE );
//		}

    m_eState        = kCCMenuStateWaiting;
    m_pSelectedItem = KD_NULL;
    m_bTouchEnabled = KD_TRUE;
    
	m_pListener = pTarget;
	m_pSelector = pSelector;
    
	return KD_TRUE;
}

KDvoid CCMenuRadio::addChild ( CCNode* pChild )
{
    this->addChild ( pChild, 0, 0 );
}

KDvoid CCMenuRadio::addChild ( CCNode* pChild, KDint nZOrder )
{
    this->addChild ( pChild, nZOrder, 0 );
}

KDvoid CCMenuRadio::addChild ( CCNode* pChild, KDint nZOrder, KDint nTag )
{
    CCMenuItem*  pItem = (CCMenuItem*) pChild;    
    pItem->initWithTarget ( this, menu_selector ( CCMenuRadio::onSelected ) );
    CCMenu::addChild ( pItem, nZOrder, nTag );
}

KDvoid CCMenuRadio::setSelectedIndex ( KDuint uIndex )
{	
	onSelected ( m_pChildren->objectAtIndex ( uIndex ) );	
}

KDuint CCMenuRadio::getSelectedIndex ( KDvoid )
{
	return m_uSelectedIndex;
}

KDvoid CCMenuRadio::onSelected ( CCObject* pSender )
{
	CCMenuItem*  pOld = (CCMenuItem *) m_pChildren->objectAtIndex ( m_uSelectedIndex );	
    CCMenuItem*  pNew = (CCMenuItem *) pSender;
    
	pOld->setEnabled ( KD_TRUE );	

	m_uSelectedIndex = m_pChildren->indexOfObject ( pNew ); 
	pNew->setEnabled ( KD_FALSE );

	if ( m_pSelector && m_pListener )
	{
		( m_pListener->*m_pSelector ) ( this );
	}
}

NS_CC_END