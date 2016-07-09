/* -----------------------------------------------------------------------------------
 *
 *      File            CCIMEDispatcher.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#include "2d/text_input_node/CCIMEDispatcher.h"

#include <list>

NS_CC_BEGIN

//////////////////////////////////////////////////////////////////////////
// add/remove delegate in IMEDelegate Cons/Destructor
//////////////////////////////////////////////////////////////////////////

IMEDelegate::IMEDelegate()
{
    IMEDispatcher::sharedDispatcher()->addDelegate(this);
}

IMEDelegate::~IMEDelegate()
{
    IMEDispatcher::sharedDispatcher()->removeDelegate(this);
}

bool IMEDelegate::attachWithIME()
{
    return IMEDispatcher::sharedDispatcher()->attachDelegateWithIME(this);
}

bool IMEDelegate::detachWithIME()
{
    return IMEDispatcher::sharedDispatcher()->detachDelegateWithIME(this);
}

//////////////////////////////////////////////////////////////////////////

typedef std::list< IMEDelegate * > DelegateList;
typedef std::list< IMEDelegate * >::iterator  DelegateIter;

//////////////////////////////////////////////////////////////////////////
// Delegate List manage class
//////////////////////////////////////////////////////////////////////////

class IMEDispatcher::Impl
{
public:
    Impl()
    {
    }

    ~Impl()
    {

    }

    void init()
    {
        _delegateWithIme = 0;
    }

    DelegateIter findDelegate(IMEDelegate* pDelegate)
    {
        DelegateIter end = _delegateList.end();
        for (DelegateIter iter = _delegateList.begin(); iter != end; ++iter)
        {
            if (pDelegate == *iter)
            {
                return iter;
            }
        }
        return end;
    }

    DelegateList    _delegateList;
    IMEDelegate*  _delegateWithIme;
};

//////////////////////////////////////////////////////////////////////////
// Cons/Destructor
//////////////////////////////////////////////////////////////////////////

IMEDispatcher::IMEDispatcher()
: m_pImpl(new IMEDispatcher::Impl)
{
    m_pImpl->init();
}

IMEDispatcher::~IMEDispatcher()
{
    CC_SAFE_DELETE(m_pImpl);
}

//////////////////////////////////////////////////////////////////////////
// Add/Attach/Remove IMEDelegate
//////////////////////////////////////////////////////////////////////////

void IMEDispatcher::addDelegate(IMEDelegate* pDelegate)
{
    if (! pDelegate || ! m_pImpl)
    {
        return;
    }
    if (m_pImpl->_delegateList.end() != m_pImpl->findDelegate(pDelegate))
    {
        // pDelegate already in list
        return;
    }
    m_pImpl->_delegateList.push_front(pDelegate);
}

bool IMEDispatcher::attachDelegateWithIME(IMEDelegate * pDelegate)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! m_pImpl || ! pDelegate);

        DelegateIter end  = m_pImpl->_delegateList.end();
        DelegateIter iter = m_pImpl->findDelegate(pDelegate);

        // if pDelegate is not in delegate list, return
        CC_BREAK_IF(end == iter);

        if (m_pImpl->_delegateWithIme)
        {
            // if old delegate canDetachWithIME return false 
            // or pDelegate canAttachWithIME return false,
            // do nothing.
            CC_BREAK_IF(! m_pImpl->_delegateWithIme->canDetachWithIME()
                || ! pDelegate->canAttachWithIME());

            // detach first
            IMEDelegate * pOldDelegate = m_pImpl->_delegateWithIme;
            m_pImpl->_delegateWithIme = 0;
            pOldDelegate->didDetachWithIME();

            m_pImpl->_delegateWithIme = *iter;
            pDelegate->didAttachWithIME();
            bRet = true;
            break;
        }

        // delegate hasn't attached to IME yet
        CC_BREAK_IF(! pDelegate->canAttachWithIME());

        m_pImpl->_delegateWithIme = *iter;
        pDelegate->didAttachWithIME();
        bRet = true;
    } while (0);
    return bRet;
}

bool IMEDispatcher::detachDelegateWithIME(IMEDelegate * pDelegate)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! m_pImpl || ! pDelegate);

        // if pDelegate is not the current delegate attached to IME, return
        CC_BREAK_IF(m_pImpl->_delegateWithIme != pDelegate);

        CC_BREAK_IF(! pDelegate->canDetachWithIME());

        m_pImpl->_delegateWithIme = 0;
        pDelegate->didDetachWithIME();
        bRet = true;
    } while (0);
    return bRet;
}

void IMEDispatcher::removeDelegate(IMEDelegate* pDelegate)
{
    do 
    {
        CC_BREAK_IF(! pDelegate || ! m_pImpl);

        DelegateIter iter = m_pImpl->findDelegate(pDelegate);
        DelegateIter end  = m_pImpl->_delegateList.end();
        CC_BREAK_IF(end == iter);

        if (m_pImpl->_delegateWithIme)

        if (*iter == m_pImpl->_delegateWithIme)
        {
            m_pImpl->_delegateWithIme = 0;
        }
        m_pImpl->_delegateList.erase(iter);
    } while (0);
}

//////////////////////////////////////////////////////////////////////////
// dispatch text message
//////////////////////////////////////////////////////////////////////////

void IMEDispatcher::dispatchInsertText(const char * pText, int nLen)
{
    do 
    {
        CC_BREAK_IF(! m_pImpl || ! pText || nLen <= 0);

        // there is no delegate attached to IME
        CC_BREAK_IF(! m_pImpl->_delegateWithIme);

        m_pImpl->_delegateWithIme->insertText(pText, nLen);
    } while (0);
}

void IMEDispatcher::dispatchDeleteBackward()
{
    do 
    {
        CC_BREAK_IF(! m_pImpl);

        // there is no delegate attached to IME
        CC_BREAK_IF(! m_pImpl->_delegateWithIme);

        m_pImpl->_delegateWithIme->deleteBackward();
    } while (0);
}

const char * IMEDispatcher::getContentText()
{
    const char * pszContentText = 0;
    if (m_pImpl && m_pImpl->_delegateWithIme)
    {
        pszContentText = m_pImpl->_delegateWithIme->getContentText();
    }
    return (pszContentText) ? pszContentText : "";
}

//////////////////////////////////////////////////////////////////////////
// dispatch keyboard message
//////////////////////////////////////////////////////////////////////////

void IMEDispatcher::dispatchKeyboardWillShow(IMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        IMEDelegate * pDelegate = 0;
        DelegateIter last = m_pImpl->_delegateList.end();
        for (DelegateIter first = m_pImpl->_delegateList.begin(); first != last; ++first)
        {
            pDelegate = *(first);
            if (pDelegate)
            {
                pDelegate->keyboardWillShow(info);
            }
        }
    }
}

void IMEDispatcher::dispatchKeyboardDidShow(IMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        IMEDelegate * pDelegate = 0;
        DelegateIter last = m_pImpl->_delegateList.end();
        for (DelegateIter first = m_pImpl->_delegateList.begin(); first != last; ++first)
        {
            pDelegate = *(first);
            if (pDelegate)
            {
                pDelegate->keyboardDidShow(info);
            }
        }
    }
}

void IMEDispatcher::dispatchKeyboardWillHide(IMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        IMEDelegate * pDelegate = 0;
        DelegateIter last = m_pImpl->_delegateList.end();
        for (DelegateIter first = m_pImpl->_delegateList.begin(); first != last; ++first)
        {
            pDelegate = *(first);
            if (pDelegate)
            {
                pDelegate->keyboardWillHide(info);
            }
        }
    }
}

void IMEDispatcher::dispatchKeyboardDidHide(IMEKeyboardNotificationInfo& info)
{
    if (m_pImpl)
    {
        IMEDelegate * pDelegate = 0;
        DelegateIter last = m_pImpl->_delegateList.end();
        for (DelegateIter first = m_pImpl->_delegateList.begin(); first != last; ++first)
        {
            pDelegate = *(first);
            if (pDelegate)
            {
                pDelegate->keyboardDidHide(info);
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

IMEDispatcher* IMEDispatcher::sharedDispatcher()
{
    static IMEDispatcher s_instance;
    return &s_instance;
}

NS_CC_END
