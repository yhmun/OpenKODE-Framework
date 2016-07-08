/* -----------------------------------------------------------------------------------
 *
 *      File            CCKeypadDelegate.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "keypad_dispatcher/CCKeypadDelegate.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN

//------------------------------------------------------------------
//
// CCKeypadHandler
//
//------------------------------------------------------------------
CCKeypadDelegate* CCKeypadHandler::getDelegate ( KDvoid )
{
    return m_pDelegate;
}

CCKeypadHandler::~CCKeypadHandler ( KDvoid )
{
	if ( m_pDelegate )
	{
		dynamic_cast<CCObject*> ( m_pDelegate )->release ( );
	}
}

KDvoid CCKeypadHandler::setDelegate ( CCKeypadDelegate* pDelegate )
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

KDbool CCKeypadHandler::initWithDelegate ( CCKeypadDelegate* pDelegate )
{
    CCAssert ( pDelegate != KD_NULL, "It's a wrong delegate!" );

    m_pDelegate = pDelegate;
	dynamic_cast<CCObject*> ( pDelegate )->retain ( );

    return KD_TRUE;
}

CCKeypadHandler* CCKeypadHandler::create ( CCKeypadDelegate* pDelegate )
{
    CCKeypadHandler*   pHandler = new CCKeypadHandler;

    if ( pHandler )
    {
        if ( pHandler->initWithDelegate ( pDelegate ) )
        {
            pHandler->autorelease ( );
        }
        else
        {
            CC_SAFE_RELEASE ( pHandler );
        }
    }

    return pHandler;
}


NS_CC_END