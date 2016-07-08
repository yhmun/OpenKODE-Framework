/* -----------------------------------------------------------------------------------
 *
 *      File            CCScrollLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-06
 * 
 * -----------------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
 *
 *      Cocos2D-iPhone-Extensions v0.2.1
 *      https://github.com/cocos2d/cocos2d-iphone-extensions
 *
 *      Copyright (c) 2010      DK101
 *      http://dk101.net/2010/11/30/implementing-page-scrolling-in-cocos2d/
 *
 *      Copyright (c) 2010      Giv Parvaneh.
 *      http://www.givp.org/blog/2010/12/30/scrolling-menus-in-cocos2d/
 *
 *      Copyright (c) 2011-2012 Stepan Generalov
 *      Copyright (c) 2011      Brian Feller
 * 
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCScrollLayer/CCScrollLayer.h"
#include "cocoa/CCString.h"
#include "menu_nodes/CCMenu.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouchHandler.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"

NS_CC_BEGIN

enum
{
	kCCScrollLayerStateIdle		,
	kCCScrollLayerStateSliding	,
};

CCScrollLayer* CCScrollLayer::create ( CCArray* pLayers, KDfloat fWidth, KDint nWidthOffset )
{
	CCScrollLayer*	pRet = new CCScrollLayer ( );

	if ( pRet && pRet->initWithLayers ( pLayers, fWidth, nWidthOffset ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCScrollLayer::CCScrollLayer ( KDvoid )
{
	m_pLayers = KD_NULL;
}

CCScrollLayer::~CCScrollLayer ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pLayers );
}

KDbool CCScrollLayer::initWithLayers ( CCArray* pLayers, KDfloat fWidth, KDint nWidthOffset )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCAssert ( pLayers->count ( ), "CCScrollLayer#initWithLayers:widthOffset: you must provide at least one layer!" );

	this->setTouchEnabled ( KD_TRUE );
	this->setStealTouches ( KD_TRUE );

	m_pScrollTouch = KD_NULL;
	m_pDelegate = KD_NULL;

	m_fLayerWidth = fWidth;
	
	// Set default minimum touch length to scroll.
	m_fMinimumTouchLengthToSlide = 20.0f;
	m_fMinimumTouchLengthToChangePage = 50.0f;

	m_fMarginOffset = m_tContentSize.cx;

	// Show indicator by default.
	m_bShowPagesIndicator = KD_TRUE;
	m_tPagesIndicatorPosition = ccp ( 0.5f * m_tContentSize.cx, kdCeilf ( m_tContentSize.cy / 8.0f ) );
	m_tPagesIndicatorNormalColor = ccc4 ( 0x96, 0x96, 0x96, 0xFF );
	m_tPagesIndicatorSelectedColor = ccc4 ( 0xFF, 0xFF, 0xFF, 0xFF );

	// Set up the starting variables
	m_nCurrentScreen = 0;

	// Save offset.
	m_fPagesWidthOffset = (KDfloat) nWidthOffset;

	// Save array of layers.
	m_pLayers = pLayers;
	m_pLayers->retain ( );

	this->updatePages ( );

	return KD_TRUE;
}

KDvoid CCScrollLayer::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, kCCMenuHandlerPriority - 1, KD_FALSE );
}

KDbool CCScrollLayer::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_pScrollTouch == KD_NULL ) 
	{
		m_pScrollTouch = pTouch;
	}
	else 
	{
		return KD_FALSE;
	}

	CCPoint		tTouchPoint = pTouch->getLocation ( );

	m_nState = kCCScrollLayerStateIdle;
	m_fStartSwipe = tTouchPoint.x;
	m_tLastPosition = this->getPosition ( );

	return KD_TRUE;
}

KDvoid CCScrollLayer::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_pScrollTouch != pTouch ) 
	{
		return;
	}

	CCPoint		tTouchPoint = pTouch->getLocation ( );

	// If finger is dragged for more distance then minimum - start sliding and cancel pressed buttons.
	// Of course only if we not already in sliding mode
	if ( ( m_nState != kCCScrollLayerStateSliding ) && ( kdFabsf ( tTouchPoint.x - m_fStartSwipe ) >= m_fMinimumTouchLengthToSlide ) )
	{
		m_nState = kCCScrollLayerStateSliding;
	    
		// Avoid jerk after state change.
		m_fStartSwipe = tTouchPoint.x;
		m_tLastPosition = this->getPosition ( );
		if ( m_bStealTouches )
		{
			this->claimTouch ( pTouch );
		}

		if ( m_pDelegate )
		{
			m_pDelegate->scrollLayerScrollingStarted ( this );
		}
	}
	
	if ( m_nState == kCCScrollLayerStateSliding )
	{
		this->setPositionX (  m_tLastPosition.x + ( tTouchPoint.x - m_fStartSwipe ) );
	}	
}

KDvoid CCScrollLayer::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_pScrollTouch != pTouch ) 
	{
		return;
	}

	m_pScrollTouch = KD_NULL;

	KDint	nSelectedPage = this->pageNumberAtOrigin ( this->getPosition ( ) );

	this->moveToPage ( nSelectedPage );
}

KDvoid CCScrollLayer::claimTouch ( CCTouch* pTouch )
{
	CCTouchDispatcher*	pDispatcher = CCDirector::sharedDirector ( )->getTouchDispatcher ( );
	CCArray*			pTargetedHandlers = pDispatcher->getTargetedHandlers ( );

	// Enumerate through all targeted handlers.
	CCObject*	pObject;
	CCARRAY_FOREACH ( pTargetedHandlers, pObject )
	{
		CCTargetedTouchHandler*		pHandler = (CCTargetedTouchHandler*) pObject;
		
		// Only our handler should claim the touch.
		if ( pHandler->getDelegate ( ) == this )
		{
			if ( !pHandler->getClaimedTouches ( )->containsObject ( pTouch ) )
			{
				pHandler->getClaimedTouches ( )->addObject ( pTouch );
			}
		}
		else
		{
			// Steal touch from other targeted delegates, if they claimed it.
			if ( pHandler->getClaimedTouches ( )->containsObject ( pTouch ) )
			{
				if ( pHandler->getDelegate ( ) )
				{
					pHandler->getDelegate ( )->ccTouchEnded ( pTouch, KD_NULL );
				}

				pHandler->getClaimedTouches ( )->removeObject ( pTouch );
			}
		}
	}
}

KDvoid CCScrollLayer::updatePages ( KDvoid )
{
	// Loop through the array and add the screens if needed.
	KDint		i = 0;
    KDfloat		fWidth = 0.0f;
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pLayers, pObject )
	{
		CCLayer*	pLayer = (CCLayer*) pObject;
		pLayer->setAnchorPoint ( ccp ( 0, 0 ) );
		pLayer->setPosition ( ccp ( ( m_fLayerWidth + m_fPagesWidthOffset ) * i, 0 ) );

		if ( !pLayer->getParent ( ) )
		{
			this->addChild ( pLayer );
		}

		i++;
	}

    this->setContentSize ( ccs ( fWidth, m_tContentSize.cy ) );

	KDfloat		fMargin = ( CCDirector::sharedDirector ( )->getWinSize ( ).cx - m_fLayerWidth ) / 2;
    this->setPosition ( ccp ( fMargin, this->getPosition ( ).y ) );
}

KDvoid CCScrollLayer::moveToPage ( KDint nPage )
{
    if ( nPage < 0 || nPage >= (KDint) m_pLayers->count ( ) )
	{
        CCLOGERROR ( "CCScrollLayer#moveToPage: %d - wrong page number, out of bounds. ", nPage );
		return;
    }
    
    this->runAction 
	(
		CCSequence::create 
		( 
			CCMoveTo::create ( 0.3f, this->originFromPageNumber ( nPage ) ),
			CCCallFunc::create ( this, callfunc_selector ( CCScrollLayer::moveToPageEnded ) ),
			KD_NULL 
		)
	);
    m_nCurrentScreen = nPage;
}

KDvoid CCScrollLayer::selectPage ( KDint nPage )
{
    if ( nPage < 0 || nPage >= (KDint) m_pLayers->count ( ) )
	{
        CCLOGERROR ( "CCScrollLayer#selectPage: %d - wrong page number, out of bounds. ", nPage );
		return;
    }
    
    this->setPosition ( this->originFromPageNumber ( nPage ) );
    m_nPrevScreen = m_nCurrentScreen;
    m_nCurrentScreen = nPage;
}

CCScrollLayerDelegate* CCScrollLayer::getDelegate ( KDvoid )
{
	return m_pDelegate;
}

KDvoid CCScrollLayer::setDelegate ( CCScrollLayerDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}

KDfloat CCScrollLayer::getLayerWidth ( KDvoid )
{
	return m_fLayerWidth;
}

const CCPoint& CCScrollLayer::getLastPosition ( KDvoid )
{
	return m_tLastPosition;
}

KDfloat CCScrollLayer::getMinimumTouchLengthToSlide ( KDvoid )
{
	return m_fMinimumTouchLengthToSlide;
}

KDvoid CCScrollLayer::setMinimumTouchLengthToSlide ( KDfloat fValue )
{
	m_fMinimumTouchLengthToSlide = fValue;
}

KDfloat CCScrollLayer::getMinimumTouchLengthToChangePage ( KDvoid )
{	
	return m_fMinimumTouchLengthToChangePage;
}

KDvoid CCScrollLayer::setMinimumTouchLengthToChangePage ( KDfloat fValue )
{
	m_fMinimumTouchLengthToChangePage = fValue;
}

KDfloat CCScrollLayer::getMarginOffset ( KDvoid )
{
	return m_fMarginOffset;
}

KDvoid CCScrollLayer::setMarginOffset ( KDfloat fOffset )
{
	m_fMarginOffset = fOffset;
}

KDbool CCScrollLayer::isStealTouches ( KDvoid )
{
	return m_bStealTouches;
}

KDvoid CCScrollLayer::setStealTouches ( KDbool bIsSteal )
{
	m_bStealTouches = bIsSteal;
}

KDbool CCScrollLayer::isShowPagesIndicator ( KDvoid )
{
	return m_bShowPagesIndicator;
}

KDvoid CCScrollLayer::setShowPagesIndicator ( KDbool bShow )
{
	m_bShowPagesIndicator = bShow;
}

const CCPoint& CCScrollLayer::getPagesIndicatorPosition ( KDvoid )
{
	return m_tPagesIndicatorPosition;
}

KDvoid CCScrollLayer::setPagesIndicatorPosition ( const CCPoint& tPosition )
{
	m_tPagesIndicatorPosition = tPosition;
}

const ccColor4B& CCScrollLayer::getPagesIndicatorSelectedColor ( KDvoid )
{
	return m_tPagesIndicatorSelectedColor;
}

KDvoid CCScrollLayer::setPagesIndicatorSelectedColor ( const ccColor4B& tColor )
{
	m_tPagesIndicatorSelectedColor = tColor;
}

const ccColor4B& CCScrollLayer::getPagesIndicatorNormalColor ( KDvoid )
{
	return m_tPagesIndicatorNormalColor;
}

KDvoid CCScrollLayer::setPagesIndicatorNormalColor ( const ccColor4B& tColor )
{
	m_tPagesIndicatorNormalColor = tColor;
}

KDint CCScrollLayer::getTotalScreens ( KDvoid )
{
	return (KDint) m_pLayers->count ( );
}

KDint CCScrollLayer::getCurrentScreen ( KDvoid )
{
	return m_nCurrentScreen;
}

KDfloat CCScrollLayer::getPagesWidthOffset ( KDvoid )
{
	return m_fPagesWidthOffset;
}

KDvoid CCScrollLayer::setPagesWidthOffset ( KDfloat fOffset )
{
	m_fPagesWidthOffset = fOffset;
}

CCArray* CCScrollLayer::getPages ( KDvoid )
{
	return m_pLayers;
}

KDint CCScrollLayer::pageNumberAtOrigin ( const CCPoint& tOrigin )
{    
	KDfloat		fWinWidth   = CCDirector::sharedDirector ( )->getWinSize ( ).cx;
    KDfloat		fMargin     = ( fWinWidth - m_fLayerWidth ) / 2;
    
	KDfloat		fPageFloat  = -( tOrigin.x - fMargin ) / ( m_fLayerWidth + m_fPagesWidthOffset );
	KDint		nPageNumber = (KDint) kdCeilf ( fPageFloat );
	if ( (KDfloat) nPageNumber - fPageFloat  >= 0.5f )
	{
		nPageNumber--;
	}
    
	nPageNumber = KD_MAX ( 0, nPageNumber );
	nPageNumber = KD_MIN ( (KDint) m_pLayers->count ( ) - 1, nPageNumber );
    
	return nPageNumber;
}

CCPoint CCScrollLayer::originFromPageNumber ( KDint nPage )
{
	KDfloat		fWinWidth = CCDirector::sharedDirector ( )->getWinSize ( ).cx;
    KDfloat		fLeft = nPage * ( m_fLayerWidth + m_fPagesWidthOffset );
    KDfloat		fMargin = ( fWinWidth - m_fLayerWidth ) / 2;
    
	return ccp ( fMargin - fLeft, this->getPositionY ( ) );
}

KDvoid CCScrollLayer::moveToPageEnded ( KDvoid )
{
    if ( m_nPrevScreen != m_nCurrentScreen )
    {
		if ( m_pDelegate )
		{
			m_pDelegate->scrollLayer ( this, m_nCurrentScreen );
		}
    }
    
    m_nPrevScreen = m_nCurrentScreen = this->pageNumberAtOrigin ( this->getPosition ( ) );
}

NS_CC_END