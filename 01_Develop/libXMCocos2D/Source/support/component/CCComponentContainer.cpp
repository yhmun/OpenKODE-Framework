/* -----------------------------------------------------------------------------------
 *
 *      File            CCComponentContainer.cpp
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
#include "support/component/CCComponentContainer.h"
#include "support/component/CCComponent.h"
#include "support/CCDirector.h"
#include "base_nodes/CCNode.h"

NS_CC_BEGIN

CCComponentContainer::CCComponentContainer ( CCNode* pNode )
{
	m_pComponents = KD_NULL;
	m_pOwner = pNode;
}

CCComponentContainer::~CCComponentContainer ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pComponents );
}

CCComponent* CCComponentContainer::get ( const KDchar* szName ) const
{
    CCComponent*	pRet = KD_NULL;
    CCAssert ( szName != KD_NULL, "Argument must be non-nil" );
    
	do 
	{
        CC_BREAK_IF ( KD_NULL == szName );
        CC_BREAK_IF ( KD_NULL == m_pComponents );
        pRet = dynamic_cast<CCComponent*> ( m_pComponents->objectForKey ( szName ) );
        
    } while ( 0 );

    return pRet;
}

KDbool CCComponentContainer::add ( CCComponent* pCom )
{
    KDbool		bRet = KD_FALSE;
    CCAssert ( pCom != KD_NULL, "Argument must be non-nil");
    CCAssert ( pCom->getOwner ( ) == KD_NULL, "Component already added. It can't be added again" );
    do
    {
        if ( m_pComponents == KD_NULL )
        {
            m_pComponents = CCDictionary::create ( );
            m_pComponents->retain ( );
            m_pOwner->scheduleUpdate ( );
        }
        CCComponent*	pComponent = dynamic_cast<CCComponent*> ( m_pComponents->objectForKey ( pCom->getName ( ) ) );
        
        CCAssert ( pComponent == KD_NULL, "Component already added. It can't be added again" );
        CC_BREAK_IF ( pComponent );
        pCom->setOwner ( m_pOwner );
        m_pComponents->setObject ( pCom, pCom->getName ( ) );
        pCom->onEnter ( );
        bRet = KD_TRUE;
    } while ( 0 );
    return bRet;
}

KDbool CCComponentContainer::remove ( const KDchar* szName )
{
    KDbool	bRet = KD_FALSE;
    CCAssert ( szName != KD_NULL, "Argument must be non-nil" );
    do 
    {        
        CC_BREAK_IF ( !m_pComponents );
        CCObject*		pRetObject = KD_NULL;
        CCDictElement*	pElement = KD_NULL;
        HASH_FIND_PTR ( m_pComponents->m_pElements, szName, pElement );
        if ( pElement != KD_NULL )
        {
           pRetObject = pElement->getObject ( );
        }
        CCComponent*	pCom = dynamic_cast<CCComponent*> ( pRetObject );
        CC_BREAK_IF ( !pCom );
        pCom->onExit ( );
        pCom->setOwner ( KD_NULL );
        HASH_DEL ( m_pComponents->m_pElements, pElement );
        pElement->getObject ( )->release ( );
        CC_SAFE_DELETE ( pElement );
        bRet = KD_FALSE;
    } while ( 0 );
    return bRet;
}

KDvoid CCComponentContainer::removeAll ( KDvoid )
{
    if ( m_pComponents != KD_NULL )
    {
        CCDictElement*	pElement;
		CCDictElement*	pTemp;
        HASH_ITER ( hh, m_pComponents->m_pElements, pElement, pTemp )
        {
            HASH_DEL ( m_pComponents->m_pElements, pElement );
            ( (CCComponent*) pElement->getObject ( ) )->onExit ( );
            ( (CCComponent*) pElement->getObject ( ) )->setOwner ( KD_NULL );
            pElement->getObject ( )->release ( );
            CC_SAFE_DELETE ( pElement );
        }
        m_pOwner->unscheduleUpdate ( );
    }
}

KDvoid CCComponentContainer::alloc ( KDvoid )
{
    m_pComponents = CCDictionary::create ( );
    m_pComponents->retain ( );
}

KDvoid CCComponentContainer::visit ( KDfloat fDelta )
{
    if ( m_pComponents != KD_NULL )
    {
        CCDictElement*	pElement;
		CCDictElement*	pTemp;
        HASH_ITER ( hh, m_pComponents->m_pElements, pElement, pTemp )
        {
            ( (CCComponent*) pElement->getObject ( ) )->update ( fDelta );
        }
    }
}

KDbool CCComponentContainer::isEmpty ( KDvoid ) const
{
    return (KDbool) ( !( m_pComponents && m_pComponents->count ( ) ) );
}

NS_CC_END
