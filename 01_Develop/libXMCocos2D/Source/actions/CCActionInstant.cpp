/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionInstant.cpp  
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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
#include "actions/CCActionInstant.h"
#include "sprite_nodes/CCSprite.h"
#include "script_support/CCScriptSupport.h"
#include "cocoa/CCZone.h"

NS_CC_BEGIN
    
//
// InstantAction
//
CCObject* CCActionInstant::copyWithZone ( CCZone* pZone )
{
	CCZone*       pNewZone = NULL;
	CCActionInstant*  pRet = NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		pRet = (CCActionInstant*) ( pZone->m_pCopyObject );
	}
	else
	{
		pRet  = new CCActionInstant ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCFiniteTimeAction::copyWithZone ( pZone );
	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}

KDbool CCActionInstant::isDone ( KDvoid )
{
	return KD_TRUE;
}

KDvoid CCActionInstant::step ( KDfloat fElapsed )
{
	CC_UNUSED_PARAM ( fElapsed );
    
	update ( 1 );
}

KDvoid CCActionInstant::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );
}

CCFiniteTimeAction* CCActionInstant::reverse ( KDvoid )
{
	return (CCFiniteTimeAction*) copy ( )->autorelease ( );
}

//
// Show
//

CCShow* CCShow::create ( KDvoid )
{
	CCShow*  pRet = new CCShow ( );

	if ( pRet )
	{
		pRet->autorelease ( );
	}

	return pRet;
}

KDvoid CCShow::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );

	m_pTarget->setVisible ( KD_TRUE );
}

CCFiniteTimeAction* CCShow::reverse ( KDvoid )
{
	return (CCFiniteTimeAction*) ( CCHide::create ( ) );
}

CCObject* CCShow::copyWithZone ( CCZone* pZone ) 
{
	CCZone*  pNewZone = KD_NULL;
	CCShow*      pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pRet = (CCShow *) pZone->m_pCopyObject;
	} 
	else
	{
		pRet = new CCShow ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCActionInstant::copyWithZone ( pZone );
	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}

//
// Hide
//

CCHide* CCHide::create ( KDvoid )
{
	CCHide*  pRet = new CCHide ( );

	if ( pRet )
	{
		pRet->autorelease ( );
	}

	return pRet;
}

KDvoid CCHide::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );

	m_pTarget->setVisible ( KD_FALSE );
}

CCFiniteTimeAction* CCHide::reverse ( KDvoid )
{
	return (CCFiniteTimeAction*) ( CCShow::create ( ) );
}

CCObject* CCHide::copyWithZone ( CCZone* pZone ) 
{
	CCZone*  pNewZone = KD_NULL;
	CCHide*      pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pRet = (CCHide*) ( pZone->m_pCopyObject );
	} 
	else
	{
		pRet = new CCHide ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCActionInstant::copyWithZone ( pZone );
	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}

//
// ToggleVisibility
//

CCToggleVisibility* CCToggleVisibility::create ( KDvoid )
{
	CCToggleVisibility*  pRet = new CCToggleVisibility ( );

	if ( pRet )
	{
		pRet->autorelease ( );
	}

	return pRet;
}

KDvoid CCToggleVisibility::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );

	m_pTarget->setVisible ( !m_pTarget->isVisible ( ) );
}

CCObject* CCToggleVisibility::copyWithZone ( CCZone* pZone ) 
{
	CCZone*			 pNewZone = KD_NULL;
	CCToggleVisibility*  pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pRet = (CCToggleVisibility*) pZone->m_pCopyObject;
	} 
	else
	{
		pRet = new CCToggleVisibility ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCActionInstant::copyWithZone ( pZone );
	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}


//
// Remove Self
//

CCRemoveSelf::CCRemoveSelf ( KDvoid )
{

}

CCRemoveSelf::~CCRemoveSelf ( KDvoid )
{

}

CCRemoveSelf* CCRemoveSelf::create ( KDbool bIsNeedCleanUp ) 
{
	CCRemoveSelf*	pRet = new CCRemoveSelf ( );

	if ( pRet && pRet->init ( bIsNeedCleanUp ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCRemoveSelf::init ( KDbool bIsNeedCleanUp )
{
	m_bIsNeedCleanUp = bIsNeedCleanUp;
	return KD_TRUE;
}

KDvoid CCRemoveSelf::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );

	m_pTarget->removeFromParentAndCleanup ( m_bIsNeedCleanUp );
}

CCFiniteTimeAction* CCRemoveSelf::reverse ( KDvoid )
{
	return (CCFiniteTimeAction*) CCRemoveSelf::create ( m_bIsNeedCleanUp );
}

CCObject* CCRemoveSelf::copyWithZone ( CCZone* pZone )
{
	CCZone*		  pNewZone = KD_NULL;
	CCRemoveSelf*	  pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		pRet = (CCRemoveSelf*) pZone->m_pCopyObject;
	}
	else 
	{
		pRet = new CCRemoveSelf ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCActionInstant::copyWithZone ( pZone );
	pRet->init ( m_bIsNeedCleanUp );
	CC_SAFE_DELETE ( pNewZone );
	return pRet;
}


//
// FlipX
//
CCFlipX::CCFlipX ( KDvoid )
{
    m_bFlipX = KD_FALSE;
}

CCFlipX* CCFlipX::create ( KDbool bFlipX )
{
	CCFlipX*  pRet = new CCFlipX ( );

	if ( pRet && pRet->initWithFlipX ( bFlipX ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCFlipX::initWithFlipX ( KDbool bFlipX )
{
	m_bFlipX = bFlipX;

	return KD_TRUE;
}

KDvoid CCFlipX::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );

	( (CCSprite*) m_pTarget )->setFlipX ( m_bFlipX );
}

CCFiniteTimeAction* CCFlipX::reverse ( KDvoid )
{
	return CCFlipX::create ( !m_bFlipX );
}

CCObject* CCFlipX::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCFlipX*     pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		pRet = (CCFlipX *) pZone->m_pCopyObject;
	}
	else
	{
		pRet  = new CCFlipX ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCActionInstant::copyWithZone ( pZone );
	
	pRet->initWithFlipX ( m_bFlipX );

	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}

//
// FlipY
//
CCFlipY::CCFlipY ( KDvoid )
{
    m_bFlipY = KD_FALSE;
}

CCFlipY* CCFlipY::create ( KDbool bFlipY )
{
	CCFlipY*  pRet = new CCFlipY ( );

	if ( pRet && pRet->initWithFlipY ( bFlipY ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCFlipY::initWithFlipY ( KDbool bFlipY )
{
	m_bFlipY = bFlipY;

	return KD_TRUE;
}

KDvoid CCFlipY::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );

	( (CCSprite*) m_pTarget )->setFlipY ( m_bFlipY );
}

CCFiniteTimeAction* CCFlipY::reverse ( KDvoid )
{
	return CCFlipY::create ( !m_bFlipY );
}

CCObject* CCFlipY::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCFlipY*     pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		pRet = (CCFlipY*) pZone->m_pCopyObject;
	}
	else
	{
		pRet  = new CCFlipY ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCActionInstant::copyWithZone ( pZone );
	
	pRet->initWithFlipY ( m_bFlipY );
	
	CC_SAFE_DELETE ( pNewZone );
	
	return pRet;
}

//
// Place
//

CCPlace* CCPlace::create ( const CCPoint& tPosition )
{
	CCPlace*  pRet = new CCPlace ( );
	
	if ( pRet && pRet->initWithPosition ( tPosition ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

KDbool CCPlace::initWithPosition ( const CCPoint& tPosition )
{
	m_tPosition = tPosition;

	return KD_TRUE;
}

CCObject* CCPlace::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCPlace*     pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		pRet = (CCPlace *) pZone->m_pCopyObject;
	}
	else
	{
		pRet  = new CCPlace ( );
		pZone = pNewZone = new CCZone ( pRet );
	}
	
	CCActionInstant::copyWithZone ( pZone );
	
	pRet->initWithPosition ( m_tPosition );
	
	CC_SAFE_DELETE ( pNewZone );
	
	return pRet;
}

KDvoid CCPlace::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );
	
	m_pTarget->setPosition ( m_tPosition );
}

//
// CallFunc
//
CCCallFunc::CCCallFunc ( KDvoid )
{
    m_pSelectorTarget = KD_NULL;
    m_pCallFunc       = KD_NULL;
	m_nScriptHandler  = 0;
}

CCCallFunc::~CCCallFunc ( KDvoid )
{
	if ( m_nScriptHandler )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->removeScriptHandler ( m_nScriptHandler );
    }

	CC_SAFE_RELEASE ( m_pSelectorTarget );
}

CCCallFunc* CCCallFunc::create ( CCObject* pSelectorTarget, SEL_CallFunc pSelector )
{
	CCCallFunc*  pRet = new CCCallFunc ( );
    
	if ( pRet && pRet->initWithTarget ( pSelectorTarget ) )
	{
		pRet->m_pCallFunc = pSelector;
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

CCCallFunc* CCCallFunc::create ( KDint nHandler )
{
	CCCallFunc*  pRet = new CCCallFunc ( );
    
	if ( pRet )
	{
		pRet->m_nScriptHandler = nHandler;
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

KDbool CCCallFunc::initWithTarget ( CCObject* pSelectorTarget )
{
	if ( pSelectorTarget )
	{
		pSelectorTarget->retain ( );
	}

	if ( m_pSelectorTarget )
	{
		m_pSelectorTarget->release ( );
	}

	m_pSelectorTarget = pSelectorTarget;
	
	return KD_TRUE;
}

CCObject* CCCallFunc::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCCallFunc*  pRet = KD_NULL;
	
	if ( pZone && pZone->m_pCopyObject ) 
	{
		pRet = (CCCallFunc *) pZone->m_pCopyObject;
	}
	else
	{
		pRet  = new CCCallFunc ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCActionInstant::copyWithZone ( pZone );
	
	pRet->initWithTarget ( m_pSelectorTarget );
	pRet->m_pCallFunc = m_pCallFunc;
	
	if ( m_nScriptHandler > 0 )
	{
        pRet->m_nScriptHandler = CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->reallocateScriptHandler ( m_nScriptHandler );
    }

	CC_SAFE_DELETE ( pNewZone );
	
	return pRet;
}

KDvoid CCCallFunc::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );
	
	this->execute ( );
}

KDvoid CCCallFunc::execute ( KDvoid )
{
	if ( m_pSelectorTarget && m_pCallFunc )
	{
		( m_pSelectorTarget->*m_pCallFunc ) ( );
	}

	if ( m_nScriptHandler )
	{
		CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeCallFuncActionEvent ( this );
	}
}

//
// CallFuncN
//
KDvoid CCCallFuncN::execute ( KDvoid )
{
	if ( m_pSelectorTarget && m_pCallFuncN )
	{
		( m_pSelectorTarget->*m_pCallFuncN ) ( m_pTarget );
	}

	if ( m_nScriptHandler )
	{
		CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeCallFuncActionEvent ( this, m_pTarget );
	}
}

CCCallFuncN* CCCallFuncN::create ( CCObject* pSelectorTarget, SEL_CallFuncN pSelector )
{
	CCCallFuncN*  pRet = new CCCallFuncN ( );
	
	if ( pRet && pRet->initWithTarget ( pSelectorTarget, pSelector ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

CCCallFuncN* CCCallFuncN::create ( KDint nHandler )
{
	CCCallFuncN*  pRet = new CCCallFuncN ( );

	if ( pRet ) 
	{
		pRet->m_nScriptHandler = nHandler;
		pRet->autorelease ( );
	}
	else 
	{
		CC_SAFE_DELETE ( pRet );
	}
	return pRet;
}

KDbool CCCallFuncN::initWithTarget ( CCObject* pSelectorTarget, SEL_CallFuncN pSelector )
{
	if ( CCCallFunc::initWithTarget ( pSelectorTarget ) ) 
	{
		m_pCallFuncN = pSelector;
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDint32 CCCallFuncN::getClassTypeInfo ( KDvoid )
{
	static const KDint32  nID = getHashCodeByString ( typeid ( CCCallFunc ).name ( ) );
	return nID;
}

CCObject* CCCallFuncN::copyWithZone ( CCZone* pZone )
{
	CCZone*   pNewZone = KD_NULL;
	CCCallFuncN*  pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pRet = (CCCallFuncN*) pZone->m_pCopyObject;
	}
	else
	{
		pRet = new CCCallFuncN ( );
		pZone = pNewZone = new CCZone ( pRet );
	}
	
	CCCallFunc::copyWithZone ( pZone );
	
	pRet->initWithTarget ( m_pSelectorTarget, m_pCallFuncN );
	
	CC_SAFE_DELETE ( pNewZone );
	
	return pRet;
}

//
// CallFuncND
//

CCCallFuncND* CCCallFuncND::create ( CCObject* pSelectorTarget, SEL_CallFuncND pSelector, KDvoid* pData )
{
	CCCallFuncND*  pRet = new CCCallFuncND ( );
	
	if ( pRet && pRet->initWithTarget ( pSelectorTarget, pSelector, pData ) )
	{
		pRet->autorelease ( );	
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

KDbool CCCallFuncND::initWithTarget ( CCObject* pSelectorTarget, SEL_CallFuncND pSelector, KDvoid* pData )
{
	if ( CCCallFunc::initWithTarget ( pSelectorTarget ) ) 
	{
		m_pData = pData;	
		m_pCallFuncND = pSelector;
		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDint32 CCCallFuncND::getClassTypeInfo ( KDvoid )
{
    static const KDint32  nID = getHashCodeByString ( typeid ( CCCallFunc ).name ( ) );
	return nID;
}

CCObject* CCCallFuncND::copyWithZone ( CCZone* pZone )
{
	CCZone*    pNewZone = KD_NULL;
	CCCallFuncND*  pRet = KD_NULL;
	
	if ( pZone && pZone->m_pCopyObject )
	{
		pRet = (CCCallFuncND*) pZone->m_pCopyObject;
	}
	else
	{
		pRet = new CCCallFuncND ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCCallFunc::copyWithZone ( pZone );
	
	pRet->initWithTarget ( m_pSelectorTarget, m_pCallFuncND, m_pData );
	
	CC_SAFE_DELETE ( pNewZone );
	
	return pRet;
}

KDvoid CCCallFuncND::execute ( KDvoid )
{
	if ( m_pSelectorTarget && m_pCallFuncND )
	{
		( m_pSelectorTarget->*m_pCallFuncND ) ( m_pTarget, m_pData );
	}
}

//
// CCCallFuncO
//
CCCallFuncO::CCCallFuncO ( KDvoid )
{
    m_pObject = KD_NULL;
}

CCCallFuncO::~CCCallFuncO ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pObject );
}

KDvoid CCCallFuncO::execute ( KDvoid )
{
    if ( m_pCallFuncO )
    {
        ( m_pSelectorTarget->*m_pCallFuncO ) ( m_pObject );
    }
}

CCCallFuncO* CCCallFuncO::create ( CCObject* pSelectorTarget, SEL_CallFuncO pSelector, CCObject* pObject )
{
    CCCallFuncO*  pRet = new CCCallFuncO ( );

    if ( pRet && pRet->initWithTarget ( pSelectorTarget, pSelector, pObject ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
    
	return pRet;
}

KDbool CCCallFuncO::initWithTarget ( CCObject* pSelectorTarget, SEL_CallFuncO pSelector, CCObject* pObject )
{
    if ( CCCallFunc::initWithTarget ( pSelectorTarget ) ) 
    {
        m_pObject = pObject;
        CC_SAFE_RETAIN ( m_pObject );

        m_pCallFuncO = pSelector;
        return KD_TRUE;
    }

    return KD_FALSE;
}

KDint32 CCCallFuncO::getClassTypeInfo ( KDvoid )
{
	static const KDint32  nID = getHashCodeByString ( typeid ( CCCallFunc ).name ( ) );
	return nID;
}

CCObject* CCCallFuncO::copyWithZone ( CCZone* pZone )
{
    CCZone*   pNewZone = KD_NULL;
    CCCallFuncO*  pRet = KD_NULL;

    if ( pZone && pZone->m_pCopyObject ) 
	{
        pRet = (CCCallFuncO*) pZone->m_pCopyObject;
	}
    else
    {
        pRet = new CCCallFuncO ( );
        pZone = pNewZone = new CCZone ( pRet );
    }

    CCCallFunc::copyWithZone ( pZone );
    
	pRet->initWithTarget ( m_pSelectorTarget, m_pCallFuncO, m_pObject );
    
	CC_SAFE_DELETE ( pNewZone );
    
	return pRet;
}
    
NS_CC_END
