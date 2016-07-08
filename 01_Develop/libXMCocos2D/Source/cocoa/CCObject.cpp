/* -----------------------------------------------------------------------------------
 *
 *      File            CCObject.cpp
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
#include "cocoa/CCObject.h"
#include "cocoa/CCString.h"
#include "cocoa/CCAutoreleasePool.h"
#include "script_support/CCScriptSupport.h"

NS_CC_BEGIN

CCObject* CCCopying::copyWithZone ( CCZone* pZone )
{
	CC_UNUSED_PARAM ( pZone );
	CCAssert ( 0, "not implement" );

	return KD_NULL;
}

CCObject::CCObject ( KDvoid )
{
	static KDuint  uObjectCount = 0;

	m_uID = ++uObjectCount;
	m_nLuaID = 0;
	m_uReference = 1;					// when the object is created, the reference count of it is 1
	m_uAutoReleaseCount = 0;
}

CCObject::~CCObject ( KDvoid )
{
	// if the object is managed, we should remove it
	// from pool manager
	if ( m_uAutoReleaseCount > 0 )  
	{
		CCPoolManager::sharedPoolManager ( )->removeObject ( this );
	}

	// if the object is referenced by Lua engine, remove it
    if ( m_nLuaID )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->removeScriptObjectByCCObject ( this );
    }
    else
    {
        CCScriptEngineProtocol*  pEngine = CCScriptEngineManager::sharedManager ( )->getScriptEngine ( );
        if ( pEngine != KD_NULL && pEngine->getScriptType ( ) == kScriptTypeJavascript )
        {
            pEngine->removeScriptObjectByCCObject ( this );
        }
    }
}

KDbool CCObject::init ( KDvoid )
{
	return KD_TRUE;
}

CCObject* CCObject::copy ( KDvoid )
{
	return copyWithZone ( KD_NULL );
}

KDvoid CCObject::release ( KDvoid )
{
	CCAssert ( m_uReference > 0, "reference count should greater than 0" );

	--m_uReference;

	if ( m_uReference == 0 )
	{
		delete this;
	}
}

KDvoid CCObject::retain ( KDvoid )
{
	CCAssert ( m_uReference > 0, "reference count should greater than 0" );

	++m_uReference;
}

CCObject* CCObject::autorelease ( KDvoid )
{
	CCPoolManager::sharedPoolManager ( )->addObject ( this );

	return this;
}

KDbool CCObject::isSingleReference ( KDvoid ) const
{
	return m_uReference == 1;
}

KDuint CCObject::retainCount ( KDvoid ) const
{
	return m_uReference;
}

KDbool CCObject::isEqual ( const CCObject* pObject )
{
	return this == pObject;
}

KDvoid CCObject::acceptVisitor ( CCDataVisitor& tVisitor )
{
	tVisitor.visitObject ( this );
}

NS_CC_END