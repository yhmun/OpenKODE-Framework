/* --------------------------------------------------------------------------
 *
 *      File            CCEditBox.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      James Chen
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCEditBox/CCEditBox.h"

NS_CC_BEGIN

CCEditBox::CCEditBox ( KDvoid )
{
	m_pDelegate				= KD_NULL;
	m_eEditBoxInputMode		= kCCEditBoxInputModeSingleLine;
	m_eEditBoxInputFlag		= kCCEditBoxInputFlagInitialCapsAllCharacters;
	m_eKeyboardReturnType	= kCCKeyboardReturnTypeDefault;
	m_tColText				= ccWHITE;
	m_tColPlaceHolder		= ccGRAY;
	m_nMaxLength			= 0;
	m_fAdjustHeight			= 0.0f;
}

CCEditBox::~CCEditBox ( KDvoid )
{

}

KDvoid CCEditBox::touchDownAction ( CCObject* pSender, CCControlEvent uControlEvent )
{    
//    m_pEditBoxImpl->openKeyboard ( );
}

CCEditBox* CCEditBox::create ( const CCSize& tSize, CCScale9Sprite* pNormal9SpriteBg, CCScale9Sprite* pPressed9SpriteBg, CCScale9Sprite* pDisabled9SpriteBg )
{
    CCEditBox*  pRet = new CCEditBox ( );
    
    if ( pRet != KD_NULL && pRet->initWithSizeAndBackgroundSprite ( tSize, pNormal9SpriteBg ) )
    {
        if ( pPressed9SpriteBg != KD_NULL )
        {
            pRet->setBackgroundSpriteForState ( pPressed9SpriteBg, kCCControlStateHighlighted );
        }
        
        if ( pDisabled9SpriteBg != KD_NULL )
        {
            pRet->setBackgroundSpriteForState ( pDisabled9SpriteBg, kCCControlStateDisabled );
        }
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

KDbool CCEditBox::initWithSizeAndBackgroundSprite ( const CCSize& tSize, CCScale9Sprite* pPressed9SpriteBg )
{
    if ( CCControlButton::initWithBackgroundSprite ( pPressed9SpriteBg ) )
    {
//        m_pEditBoxImpl = new CCEditBoxImpl ( this );
//        m_pEditBoxImpl->initWithSize ( tSize );
        
        this->setPreferredSize ( tSize );
        this->setPosition ( ccp ( 0, 0 ) );
        this->addTargetWithActionForControlEvent ( this, cccontrol_selector ( CCEditBox::touchDownAction ), kCCControlEventTouchUpInside );
        
        return KD_TRUE;
    }

    return KD_FALSE;
}

KDvoid CCEditBox::setDelegate ( CCEditBoxDelegate* pDelegate )
{
    m_pDelegate = pDelegate;

//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->setDelegate ( pDelegate );
    }
}

KDvoid CCEditBox::setText ( const KDchar* szText )
{
    if ( szText != KD_NULL )
    {
        m_sText = szText;

//        if ( m_pEditBoxImpl != KD_NULL )
        {
//            m_pEditBoxImpl->setText ( szText );
        }
    }
}

const KDchar* CCEditBox::getText ( KDvoid )
{
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        return m_pEditBoxImpl->getText ( );
    }
    
    return KD_NULL;
}

KDvoid CCEditBox::setFontColor ( const ccColor3B& tColor )
{
    m_tColText = tColor;
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->setFontColor ( tColor );
    }
}

KDvoid CCEditBox::setPlaceholderFontColor ( const ccColor3B& tColor )
{
    m_tColText = tColor;
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->setPlaceholderFontColor ( tColor );
    }
}

KDvoid CCEditBox::setPlaceHolder ( const KDchar* szText )
{
    if ( szText != KD_NULL )
    {
        m_sPlaceHolder = szText;

//        if ( m_pEditBoxImpl != KD_NULL )
        {
//            m_pEditBoxImpl->setPlaceHolder ( szText );
        }
    }
}

const KDchar* CCEditBox::getPlaceHolder ( KDvoid )
{
    return m_sPlaceHolder.c_str ( );
}

KDvoid CCEditBox::setInputMode ( CCEditBoxInputMode eInputMode )
{
    m_eEditBoxInputMode = eInputMode;
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->setInputMode ( eInputMode );
    }
}

KDvoid CCEditBox::setMaxLength ( KDint nMaxLength )
{
    m_nMaxLength = nMaxLength;
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->setMaxLength ( nMaxLength );
    }
}

KDint CCEditBox::getMaxLength ( KDvoid )
{
    return m_nMaxLength;
}

KDvoid CCEditBox::setInputFlag ( CCEditBoxInputFlag eInputFlag )
{
    m_eEditBoxInputFlag = eInputFlag;
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->setInputFlag ( eInputFlag );
    }
}

KDvoid CCEditBox::setReturnType ( CCKeyboardReturnType eReturnType )
{
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->setReturnType ( eReturnType );
    }
}

// override function 
KDvoid CCEditBox::setPosition ( const CCPoint& tPosition )
{
    CCControlButton::setPosition ( tPosition );
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->setPosition ( tPosition );
    }
}

KDvoid CCEditBox::setContentSize ( const CCSize& tSize )
{
    CCControlButton::setContentSize ( tSize );
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->setContentSize ( tSize );
    }
}

KDvoid CCEditBox::visit ( KDvoid )
{
    CCControlButton::visit ( );
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->visit ( );
    }
}

KDvoid CCEditBox::onExit ( KDvoid )
{
    CCControlButton::onExit ( );
//    if ( m_pEditBoxImpl != KD_NULL )
    {
        // remove system edit control
//        m_pEditBoxImpl->closeKeyboard ( );
    }
}

static CCRect  getRect ( CCNode* pNode )
{
    CCRect  tRect;
    
	tRect.origin = pNode->getPosition ( );
    tRect.size   = pNode->getContentSize ( );

    tRect.origin.x -= tRect.size.cx / 2;
    tRect.origin.y -= tRect.size.cy / 2;

    return tRect;
}

KDvoid CCEditBox::keyboardWillShow ( CCIMEKeyboardNotificationInfo& tInfo )
{
    CCRect  tRectTracked = getRect ( this );
    
    // if the keyboard area doesn't intersect with the tracking node area, nothing needs to be done.
    if (!tRectTracked.intersectsRect ( tInfo.end ) )
    {
        CCLOG ( "needn't to adjust view layout." );
        return;
    }
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    m_fAdjustHeight = tInfo.end.getMaxY ( ) - tRectTracked.getMinY ( );

//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->doAnimationWhenKeyboardMove ( tInfo.duration, m_fAdjustHeight );
    }
}

KDvoid CCEditBox::keyboardDidShow ( CCIMEKeyboardNotificationInfo& tInfo )
{

}

KDvoid CCEditBox::keyboardWillHide ( CCIMEKeyboardNotificationInfo& tInfo )
{
//    if ( m_pEditBoxImpl != KD_NULL )
    {
//        m_pEditBoxImpl->doAnimationWhenKeyboardMove ( tInfo.duration, -m_fAdjustHeight );
    }
}

KDvoid CCEditBox::keyboardDidHide ( CCIMEKeyboardNotificationInfo& tInfo )
{

}

NS_CC_END
