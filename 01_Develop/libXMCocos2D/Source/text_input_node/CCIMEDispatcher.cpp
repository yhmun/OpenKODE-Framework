/* -----------------------------------------------------------------------------------
 *
 *      File            CCIMEDispatcher.cpp
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
#include "text_input_node/CCIMEDispatcher.h"
#include <list>

NS_CC_BEGIN
    
//////////////////////////////////////////////////////////////////////////
// add/remove delegate in CCIMEDelegate Cons/Destructor
//////////////////////////////////////////////////////////////////////////

CCIMEDelegate::CCIMEDelegate ( KDvoid )
{
    CCIMEDispatcher::sharedDispatcher ( )->addDelegate ( this );
}

CCIMEDelegate::~CCIMEDelegate ( KDvoid )
{
    CCIMEDispatcher::sharedDispatcher ( )->removeDelegate ( this );
}

KDbool CCIMEDelegate::attachWithIME ( KDvoid )
{
    return CCIMEDispatcher::sharedDispatcher ( )->attachDelegateWithIME ( this );
}

KDbool CCIMEDelegate::detachWithIME ( KDvoid )
{
    return CCIMEDispatcher::sharedDispatcher ( )->detachDelegateWithIME ( this );
}

//////////////////////////////////////////////////////////////////////////

typedef std::list< CCIMEDelegate * > DelegateList;
typedef std::list< CCIMEDelegate * >::iterator  DelegateIter;

//////////////////////////////////////////////////////////////////////////
// Delegate List manage class
//////////////////////////////////////////////////////////////////////////

class CCIMEDispatcher::Impl
{
	public :

		Impl ( KDvoid )
		{

		}

		~Impl ( KDvoid )
		{

		}

		KDvoid init ( KDvoid )
		{
			m_DelegateWithIme = 0;
		}

		DelegateIter findDelegate ( CCIMEDelegate* pDelegate )
		{
            DelegateIter  pIter = m_DelegateList.begin ( );
			DelegateIter  pLast = m_DelegateList.end ( );
			
            for ( ; pIter != pLast; ++pIter )
			{
				if ( pDelegate == *pIter )
				{
					return pIter;
				}
			}
            
			return pLast;
		}

		DelegateList        m_DelegateList;
		CCIMEDelegate*      m_DelegateWithIme;
};

//////////////////////////////////////////////////////////////////////////
// Cons/Destructor
//////////////////////////////////////////////////////////////////////////

CCIMEDispatcher::CCIMEDispatcher ( KDvoid )
: m_pImpl ( new CCIMEDispatcher::Impl )
{
    m_pImpl->init ( );
}

CCIMEDispatcher::~CCIMEDispatcher ( KDvoid )
{
    CC_SAFE_DELETE ( m_pImpl );
}

//////////////////////////////////////////////////////////////////////////
// Add/Attach/Remove CCIMEDelegate
//////////////////////////////////////////////////////////////////////////

KDvoid CCIMEDispatcher::addDelegate ( CCIMEDelegate* pDelegate )
{
    if ( !pDelegate || !m_pImpl )
    {
        return;
    }
    
    if ( m_pImpl->m_DelegateList.end ( ) != m_pImpl->findDelegate ( pDelegate ) )
    {
        // pDelegate already in list
        return;
    }
    
    m_pImpl->m_DelegateList.push_front ( pDelegate );
}

KDbool CCIMEDispatcher::attachDelegateWithIME ( CCIMEDelegate* pDelegate )
{
    KDbool  bRet = KD_FALSE;
    do
    {
        CC_BREAK_IF ( !m_pImpl || !pDelegate );

        DelegateIter end  = m_pImpl->m_DelegateList.end ( );
        DelegateIter iter = m_pImpl->findDelegate ( pDelegate );

        // if pDelegate is not in delegate list, return
        CC_BREAK_IF ( end == iter );

        if ( m_pImpl->m_DelegateWithIme )
        {
            // if old delegate canDetachWithIME return false 
            // or pDelegate canAttachWithIME return false,
            // do nothing.
            CC_BREAK_IF ( !m_pImpl->m_DelegateWithIme->canDetachWithIME ( ) || !pDelegate->canAttachWithIME ( ) );

            // detach first
            CCIMEDelegate* pOldDelegate = m_pImpl->m_DelegateWithIme;
            m_pImpl->m_DelegateWithIme = 0;
            pOldDelegate->didDetachWithIME ( );

            m_pImpl->m_DelegateWithIme = *iter;
            pDelegate->didAttachWithIME ( );
            bRet = KD_TRUE;
            break;
        }

        // delegate hasn't attached to IME yet
        CC_BREAK_IF ( !pDelegate->canAttachWithIME ( ) );

        m_pImpl->m_DelegateWithIme = *iter;
        pDelegate->didAttachWithIME ( );
        bRet = KD_TRUE;
        
    } while ( 0 );
    
    return bRet;
}

KDbool CCIMEDispatcher::detachDelegateWithIME ( CCIMEDelegate* pDelegate )
{
    KDbool bRet = KD_FALSE;
    do
    {
        CC_BREAK_IF ( !m_pImpl || !pDelegate );

        // if pDelegate is not the current delegate attached to IME, return
        CC_BREAK_IF ( m_pImpl->m_DelegateWithIme != pDelegate );

        CC_BREAK_IF ( !pDelegate->canDetachWithIME ( ) );

        m_pImpl->m_DelegateWithIme = 0;
        pDelegate->didDetachWithIME ( );
        bRet = KD_TRUE;
        
    } while ( 0 );
    
    return bRet;
}

KDvoid CCIMEDispatcher::removeDelegate ( CCIMEDelegate* pDelegate )
{
    do 
    {
        CC_BREAK_IF ( !pDelegate || !m_pImpl );

        DelegateIter  pIter = m_pImpl->findDelegate ( pDelegate );
        DelegateIter  pLast = m_pImpl->m_DelegateList.end ( );
        
        CC_BREAK_IF ( pIter == pLast );
            
        if ( *pIter == m_pImpl->m_DelegateWithIme )
        {
            m_pImpl->m_DelegateWithIme = 0;
        }
        
        m_pImpl->m_DelegateList.erase ( pIter );
        
    } while ( 0 );
}

//////////////////////////////////////////////////////////////////////////
// dispatch text message
//////////////////////////////////////////////////////////////////////////

KDvoid CCIMEDispatcher::dispatchInsertText ( const KDchar* szText, KDsize uLen )
{
    do 
    {
        CC_BREAK_IF ( !m_pImpl || !szText );

        // there is no delegate attached to IME
        CC_BREAK_IF ( !m_pImpl->m_DelegateWithIme );

        m_pImpl->m_DelegateWithIme->insertText ( szText, uLen );
        
    } while ( 0 );
}

KDvoid CCIMEDispatcher::dispatchDeleteBackward ( KDvoid )
{
    do 
    {
        CC_BREAK_IF ( !m_pImpl );

        // there is no delegate attached to IME
        CC_BREAK_IF ( !m_pImpl->m_DelegateWithIme );

        m_pImpl->m_DelegateWithIme->deleteBackward ( );
        
    } while ( 0 );
}

const KDchar* CCIMEDispatcher::getContentText ( KDvoid )
{
    const KDchar*  szContentText = 0;
    
    if ( m_pImpl && m_pImpl->m_DelegateWithIme )
    {
        szContentText = m_pImpl->m_DelegateWithIme->getContentText ( );
    }
    
    return ( szContentText ) ? szContentText : "";
}

//////////////////////////////////////////////////////////////////////////
// dispatch keyboard message
//////////////////////////////////////////////////////////////////////////

KDvoid CCIMEDispatcher::dispatchKeyboardWillShow ( CCIMEKeyboardNotificationInfo& tInfo )
{
    if ( m_pImpl )
    {
        CCIMEDelegate*  pDelegate = KD_NULL;
        DelegateIter  pIter = m_pImpl->m_DelegateList.begin ( );
        DelegateIter  pLast = m_pImpl->m_DelegateList.end ( );
        for ( ; pIter != pLast; ++pIter )
        {
            pDelegate = *( pIter );
            
            if ( pDelegate )
            {
                pDelegate->keyboardWillShow ( tInfo );
            }
        }
    }
}

KDvoid CCIMEDispatcher::dispatchKeyboardDidShow ( CCIMEKeyboardNotificationInfo& tInfo )
{
    if ( m_pImpl )
    {
        CCIMEDelegate*  pDelegate = KD_NULL;
        DelegateIter  pIter = m_pImpl->m_DelegateList.begin ( );
        DelegateIter  pLast = m_pImpl->m_DelegateList.end ( );
        for ( ; pIter != pLast; ++pIter )
        {
            pDelegate = *( pIter );
            
            if ( pDelegate )
            {
                pDelegate->keyboardDidShow ( tInfo );
            }
        }
    }
}

KDvoid CCIMEDispatcher::dispatchKeyboardWillHide ( CCIMEKeyboardNotificationInfo& tInfo )
{
    if ( m_pImpl )
    {
        CCIMEDelegate*  pDelegate = KD_NULL;
        DelegateIter  pIter = m_pImpl->m_DelegateList.begin ( );
        DelegateIter  pLast = m_pImpl->m_DelegateList.end ( );
        for ( ; pIter != pLast; ++pIter )
        {
            pDelegate = *( pIter );
            
            if ( pDelegate )
            {
                pDelegate->keyboardWillHide ( tInfo );
            }
        }
    }
}

KDvoid CCIMEDispatcher::dispatchKeyboardDidHide ( CCIMEKeyboardNotificationInfo& tInfo )
{
    if ( m_pImpl )
    {
        CCIMEDelegate*  pDelegate = KD_NULL;
        DelegateIter  pIter = m_pImpl->m_DelegateList.begin ( );
        DelegateIter  pLast = m_pImpl->m_DelegateList.end ( );
        for ( ; pIter != pLast; ++pIter )
        {
            pDelegate = *( pIter );
            
            if ( pDelegate )
            {
                pDelegate->keyboardDidHide ( tInfo );
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// protected member function
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
static CCIMEDispatcher*  l_pDispatcher = KD_NULL;
CCIMEDispatcher* CCIMEDispatcher::sharedDispatcher ( KDvoid )
{
	if ( !l_pDispatcher )
	{
		l_pDispatcher = new CCIMEDispatcher ( );
	}

    return l_pDispatcher;
}

KDvoid CCIMEDispatcher::purgeDispatcher ( KDvoid )
{
	CC_SAFE_DELETE ( l_pDispatcher );
}


NS_CC_END