/* -----------------------------------------------------------------------------------
 *
 *      File            CCThreadLayer.cpp
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
#include "layers_scenes_transitions_nodes/CCThreadLayer.h"
#include "cocoa/CCString.h"
#include "menu_nodes/CCMenu.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "support/CCDirector.h"

NS_CC_BEGIN
    
CCThreadLayer::CCThreadLayer ( KDvoid )
{
	m_pMain   = kdThreadSelf ( );	
	m_pThread = KD_NULL;
	m_pLock   = kdThreadSemCreate ( 1 );
    
	this->setTouchEnabled ( KD_FALSE );
}

CCThreadLayer::~CCThreadLayer ( KDvoid )
{
	if ( m_pThread )
	{
		kdThreadDetach ( m_pThread );
		m_pThread = KD_NULL;
	}
}

KDvoid CCThreadLayer::visit ( KDvoid )
{
	CCLayer::visit ( );
	
	if ( m_pThread )
	{
		const KDEvent*  pEvent = kdWaitEvent ( 0 );
	
		if ( pEvent )
		{
			if ( m_pLock )
			{
				kdThreadSemWait ( m_pLock );
			}

			this->receive ( pEvent->type, pEvent->userptr );	
		
			if ( m_pLock )
			{
				kdThreadSemPost ( m_pLock );
			}

			if ( pEvent->type == kCCThreadLayerDone )
			{ 
				kdThreadJoin ( m_pThread, KD_NULL );
				this->setTouchEnabled ( KD_FALSE );
				m_pThread = KD_NULL;
			}
					
			kdDefaultEvent ( pEvent );
		}
	}
}

KDvoid CCThreadLayer::execute ( KDvoid )
{

}

KDvoid CCThreadLayer::receive ( KDint nType, KDvoid* pData )
{
	
}

KDvoid CCThreadLayer::post ( KDint nType, KDvoid* pData )
{
	if ( m_pLock )
	{
		kdThreadSemWait ( m_pLock );
	}

	KDEvent*  event = kdCreateEvent ( );
	CCAssert ( event, "kdCreateEvent ( ) failed." );
	
	event->type = nType;
	event->userptr = pData;
	
	kdPostThreadEvent ( event, m_pMain );	

	if ( m_pLock )
	{
		kdThreadSemPost ( m_pLock );
	}
}

KDvoid CCThreadLayer::resume ( KDvoid )
{
	CCAssert ( !m_pThread, "Must be start first or finished thread" );
    
	this->setTouchEnabled ( KD_TRUE );
	m_pThread = kdThreadCreate ( KD_NULL, thread, this );
}

KDvoid CCThreadLayer::setIsLock ( KDbool bOn )
{
	if ( bOn )
	{
		if ( !m_pLock )
		{
			m_pLock = kdThreadSemCreate ( 1 );
		}
	}
	else
	{
		if ( m_pLock )
		{
			kdThreadSemFree ( m_pLock );
			m_pLock = KD_NULL;
		}
	}
}

KDvoid* CCThreadLayer::thread ( KDvoid* pArg )
{
	CCThreadLayer*  pLayer = (CCThreadLayer *) pArg;
	
	pLayer->execute ( );	
	pLayer->post ( kCCThreadLayerDone );
	
	return KD_NULL;
}

NS_CC_END
