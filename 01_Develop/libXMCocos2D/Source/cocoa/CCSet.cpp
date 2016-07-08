/* -----------------------------------------------------------------------------------
 *
 *      File            CCSet.cpp
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
#include "cocoa/CCSet.h"
#include "cocoa/CCArray.h"

NS_CC_BEGIN
    
CCSet::CCSet ( KDvoid )
{
	m_pSet = new std::set<CCObject*>;
}

CCSet::CCSet ( const CCSet& tSetObject )
{
	m_pSet = new std::set<CCObject*> ( *tSetObject.m_pSet );

	// call retain of members
	CCSetIterator  iter;
	for ( iter = m_pSet->begin ( ); iter != m_pSet->end ( ); ++iter )
	{
		if ( !( *iter ) )
		{
			break;
		}

		( *iter )->retain ( );
	}
}

CCSet::~CCSet ( KDvoid )
{
	removeAllObjects ( );

	CC_SAFE_DELETE ( m_pSet );
}

KDvoid CCSet::acceptVisitor ( CCDataVisitor& tVisitor )
{
	tVisitor.visitObject ( this );
}

CCSet* CCSet::create ( KDvoid )
{
    CCSet*  pRet = new CCSet ( );
    
    if ( pRet != KD_NULL )
    {
        pRet->autorelease ( );
    }
    
    return pRet;
}

CCSet* CCSet::copy ( KDvoid )
{
	CCSet*  pSet = new CCSet ( *this );

	return pSet;
}

CCSet* CCSet::mutableCopy ( KDvoid )
{
	return copy ( );
}

KDint CCSet::count ( KDvoid )
{
	return (KDint) m_pSet->size ( );
}

KDvoid CCSet::addObject ( CCObject* pObject )
{
	CC_SAFE_RETAIN ( pObject );
	m_pSet->insert ( pObject );
}

KDvoid CCSet::removeObject ( CCObject* pObject )
{
	m_pSet->erase ( pObject );
	CC_SAFE_RELEASE ( pObject );
}

KDvoid CCSet::removeAllObjects ( KDvoid )
{
	CCSetIterator  iter;
	for ( iter = m_pSet->begin ( ); iter != m_pSet->end ( ); ++iter )
	{
		if ( !( *iter ) )
		{
			break;
		}

		( *iter )->release ( );
	}
}

KDbool CCSet::containsObject ( CCObject* pObject )
{
	return ( m_pSet->find ( pObject ) != m_pSet->end ( ) ) ? KD_TRUE : KD_FALSE;
}

CCSetIterator CCSet::begin ( KDvoid ) 
{
	return m_pSet->begin ( );
}

CCSetIterator CCSet::end ( KDvoid )
{
	return m_pSet->end ( );
}

CCObject* CCSet::anyObject ( KDvoid )
{
    if ( !m_pSet || m_pSet->empty ( ) )
    {
        return KD_NULL;
    }
    
    CCSetIterator  iter;
    for ( iter = m_pSet->begin ( ); iter != m_pSet->end ( ); ++iter )
    {
        if ( *iter )
        {
            return ( *iter );
        }
    }

    return KD_NULL;
}


CCArray* CCSet::allObjects ( KDvoid )
{
	CCArray*	pArray = CCArray::create ( );

    CCSetIterator  iter;
    for ( iter = m_pSet->begin ( ); iter != m_pSet->end ( ); ++iter )
	{
        if ( *iter )
        {
            pArray->addObject ( *iter );
        }
	}

	return pArray;
}

NS_CC_END