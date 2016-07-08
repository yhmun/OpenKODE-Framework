/* -----------------------------------------------------------------------------------
 *
 *      File            CCTouchHandler.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Valentin Milea
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
#include "touch_dispatcher/CCTouchHandler.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN
    
CCTouchDelegate* CCTouchHandler::getDelegate ( KDvoid )
{
	return m_pDelegate;
}

KDvoid CCTouchHandler::setDelegate ( CCTouchDelegate* pDelegate )
{
	if ( pDelegate )
	{
		dynamic_cast<CCObject*> ( pDelegate )->retain ( );
    }

    if ( m_pDelegate )
    {
		dynamic_cast<CCObject*> ( m_pDelegate )->release ( );
    }

	m_pDelegate = pDelegate;
}

KDint CCTouchHandler::getPriority ( KDvoid )
{
	return m_nPriority;
}

KDvoid CCTouchHandler::setPriority ( KDint nPriority )
{
	m_nPriority = nPriority;
}

KDint CCTouchHandler::getEnabledSelectors ( KDvoid )
{
	return m_nEnabledSelectors;
}

KDvoid CCTouchHandler::setEnalbedSelectors ( KDint nEnabledSelectors )
{
	m_nEnabledSelectors = nEnabledSelectors;
}

CCTouchHandler* CCTouchHandler::create ( CCTouchDelegate* pDelegate, KDint nPriority )
{
	CCTouchHandler*  pRet = new CCTouchHandler ( );

	if ( pRet && pRet->initWithDelegate ( pDelegate, nPriority ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
		
	return pRet;
}

KDbool CCTouchHandler::initWithDelegate ( CCTouchDelegate* pDelegate, KDint nPriority )
{
	CCAssert ( pDelegate != KD_NULL, "touch delegate should not be null" );

	m_pDelegate = pDelegate; 

	dynamic_cast<CCObject*> ( pDelegate )->retain ( );

	m_nPriority = nPriority;
	m_nEnabledSelectors = 0;

	return KD_TRUE;
}

CCTouchHandler::~CCTouchHandler ( KDvoid )
{
	if ( m_pDelegate )
	{
		dynamic_cast<CCObject*> ( m_pDelegate )->release ( );
	}
}

//
// implementation of CCStandardTouchHandler
//
KDbool CCStandardTouchHandler::initWithDelegate ( CCTouchDelegate* pDelegate, KDint nPriority )
{
	if ( CCTouchHandler::initWithDelegate ( pDelegate, nPriority ) )
	{
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCStandardTouchHandler* CCStandardTouchHandler::create ( CCTouchDelegate* pDelegate, KDint nPriority )
{
	CCStandardTouchHandler*  pRet = new CCStandardTouchHandler ( );

	if ( pRet && pRet->initWithDelegate ( pDelegate, nPriority ) )
	{		
		pRet->autorelease ( );
	}	
	else
	{
		CC_SAFE_DELETE ( pRet );		
	}

	return pRet;
}

//
// implementation of CCTargetedTouchHandler
//
KDbool CCTargetedTouchHandler::isSwallowsTouches ( KDvoid )
{
	return m_bSwallowsTouches;
}

KDvoid CCTargetedTouchHandler::setSwallowsTouches ( KDbool bSwallowsTouches )
{
	m_bSwallowsTouches = bSwallowsTouches;
}

CCSet* CCTargetedTouchHandler::getClaimedTouches ( KDvoid )
{
	return m_pClaimedTouches;
}

CCTargetedTouchHandler* CCTargetedTouchHandler::create ( CCTouchDelegate* pDelegate, KDint nPriority, KDbool bSwallow )
{
	CCTargetedTouchHandler*  pRet = new CCTargetedTouchHandler ( );

	if ( pRet && pRet->initWithDelegate ( pDelegate, nPriority, bSwallow ) )
	{		
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );	
	}

	return pRet;
}

KDbool CCTargetedTouchHandler::initWithDelegate ( CCTouchDelegate* pDelegate, KDint nPriority, KDbool bSwallow )
{
	if ( CCTouchHandler::initWithDelegate ( pDelegate, nPriority ) )
	{
		m_pClaimedTouches  = new CCSet ( );
		m_bSwallowsTouches = bSwallow;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCTargetedTouchHandler::~CCTargetedTouchHandler ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pClaimedTouches );
}

NS_CC_END