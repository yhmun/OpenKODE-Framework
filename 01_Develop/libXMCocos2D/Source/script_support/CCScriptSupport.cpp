/* -----------------------------------------------------------------------------------
 *
 *      File            CCScriptSupport.cpp
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
#include "script_support/CCScriptSupport.h"
#include "support/CCScheduler.h"

KDbool cc_assert_script_compatible ( const KDchar* szMsg )
{
    cocos2d::CCScriptEngineProtocol*  pEngine = cocos2d::CCScriptEngineManager::sharedManager ( )->getScriptEngine ( );
    if ( pEngine && pEngine->handleAssert ( szMsg ) )
    {
        return KD_TRUE;
    }

	return KD_FALSE;
}

NS_CC_BEGIN
    
// #pragma mark -
// #pragma mark CCScriptHandlerEntry

CCScriptHandlerEntry::CCScriptHandlerEntry ( KDint nHandler )
{
	m_nHandler = nHandler;

	static KDint  nNewEntryId = 0;
	nNewEntryId++;
	m_nEntryId = nNewEntryId;
}

CCScriptHandlerEntry* CCScriptHandlerEntry::create ( KDint nHandler )
{
    CCScriptHandlerEntry*  pEntry = new CCScriptHandlerEntry ( nHandler );
    pEntry->autorelease ( );
    return pEntry;
}

CCScriptHandlerEntry::~CCScriptHandlerEntry ( KDvoid )
{
	if ( m_nHandler != 0 )
	{
		CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->removeScriptHandler ( m_nHandler );
		m_nHandler = 0;
	}
}

// #pragma mark -
// #pragma mark CCSchedulerScriptHandlerEntry

CCSchedulerScriptHandlerEntry* CCSchedulerScriptHandlerEntry::create ( KDint nHandler, KDfloat fInterval, KDbool bPaused )
{
    CCSchedulerScriptHandlerEntry*  pEntry = new CCSchedulerScriptHandlerEntry ( nHandler );

	if ( pEntry && pEntry->init ( fInterval, bPaused ) )
	{
		pEntry->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pEntry );
	}
    
    return pEntry;
}

KDbool CCSchedulerScriptHandlerEntry::init ( KDfloat fInterval, KDbool bPaused )
{
    m_pTimer = new CCTimer ( );
    m_pTimer->initWithScriptHandler ( m_nHandler, fInterval );
    m_pTimer->autorelease ( );
    m_pTimer->retain ( );
    m_bPaused = bPaused;
    LUALOG ( "[LUA] ADD script schedule: %d, entryID: %d", m_nHandler, m_nEntryId );
    return KD_TRUE;
}

CCSchedulerScriptHandlerEntry::CCSchedulerScriptHandlerEntry ( KDint nHandler )
: CCScriptHandlerEntry ( nHandler )
{
	m_pTimer  = KD_NULL;
	m_bPaused = KD_TRUE;
	m_bMarkedForDeletion = KD_FALSE;
}

CCSchedulerScriptHandlerEntry::~CCSchedulerScriptHandlerEntry ( KDvoid )
{
    m_pTimer->release ( );
    LUALOG ( "[LUA] DEL script schedule %d, entryID: %d", m_nHandler, m_nEntryId );
}

// #pragma mark -
// #pragma mark CCTouchScriptHandlerEntry

CCTouchScriptHandlerEntry* CCTouchScriptHandlerEntry::create ( KDint nHandler, KDbool bIsMultiTouches, KDint nPriority, KDbool bSwallowsTouches )
{
    CCTouchScriptHandlerEntry*	pEntry = new CCTouchScriptHandlerEntry ( nHandler );

	if ( pEntry && pEntry->init ( bIsMultiTouches, nPriority, bSwallowsTouches ) )
	{
		pEntry->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pEntry );
	}

    return pEntry;
}

CCTouchScriptHandlerEntry::CCTouchScriptHandlerEntry ( KDint nHandler )
: CCScriptHandlerEntry ( nHandler )
{
	m_bIsMultiTouches	= KD_FALSE;
	m_nPriority			= 0;
	m_bSwallowsTouches	= KD_FALSE;
}

CCTouchScriptHandlerEntry::~CCTouchScriptHandlerEntry ( KDvoid )
{
	if ( m_nHandler != 0 )
	{
		CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->removeScriptHandler ( m_nHandler );
		LUALOG ( "[LUA] Remove touch event handler: %d", m_nHandler );
		m_nHandler = 0;
	}
}

KDbool CCTouchScriptHandlerEntry::init ( KDbool bIsMultiTouches, KDint nPriority, KDbool bSwallowsTouches )
{
    m_bIsMultiTouches	= bIsMultiTouches;
    m_nPriority			= nPriority;
    m_bSwallowsTouches	= bSwallowsTouches;
    
    return KD_TRUE;
}

// #pragma mark -
// #pragma mark CCScriptEngineManager

static CCScriptEngineManager*  l_pScriptEngineManager = KD_NULL;

CCScriptEngineManager::CCScriptEngineManager ( KDvoid ) 
{
	m_pScriptEngine = KD_NULL;
}

CCScriptEngineManager::~CCScriptEngineManager ( KDvoid ) 
{
	removeScriptEngine ( );
}

KDvoid CCScriptEngineManager::setScriptEngine ( CCScriptEngineProtocol* pScriptEngine )
{
	removeScriptEngine ( );

	m_pScriptEngine = pScriptEngine;
}

CCScriptEngineProtocol* CCScriptEngineManager::getScriptEngine ( KDvoid )
{
	return m_pScriptEngine;
}

KDvoid CCScriptEngineManager::removeScriptEngine ( KDvoid )
{
	CC_SAFE_DELETE ( m_pScriptEngine );
}

CCScriptEngineManager* CCScriptEngineManager::sharedManager ( KDvoid )
{
	if ( !l_pScriptEngineManager )
	{
		l_pScriptEngineManager = new CCScriptEngineManager ( );
	}

	return l_pScriptEngineManager;
}

KDvoid CCScriptEngineManager::purgeManager ( KDvoid )
{
	CC_SAFE_DELETE ( l_pScriptEngineManager );
}

    
NS_CC_END