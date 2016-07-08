/* --------------------------------------------------------------------------
 *
 *      File            TestEditBox.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "TestEditBox.h"

KDbool TestEditBox::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
    
    CCSprite*  pBg = CCSprite::create ( "Images/HelloWorld.png" );
    pBg->setPosition ( ccpMid ( tLyrSize ) );
    this->addChild ( pBg );
    
    
	CCSize  tEditBoxSize = CCSize ( CCEGLView::sharedOpenGLView ( )->getVisibleSize ( ).cx - 100, 60 );

    // top
    m_pEditName = CCEditBox::create ( tEditBoxSize, CCScale9Sprite::create ( "extensions/green_edit.png" ) );
	m_pEditName->setPosition    ( ccp ( tLyrSize.cx / 2, tLyrSize.cy * 4 / 5 ) );
    m_pEditName->setFontColor   ( ccRED );
    m_pEditName->setPlaceHolder ( "Name :" );
    m_pEditName->setMaxLength   ( 8 );
    m_pEditName->setReturnType  ( kCCKeyboardReturnTypeDone );
    m_pEditName->setDelegate    ( this );
    this->addChild ( m_pEditName );
    
    // middle
    m_pEditPassword = CCEditBox::create ( tEditBoxSize, CCScale9Sprite::create ( "extensions/orange_edit.png" ) );
	m_pEditPassword->setPosition    ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 ) );
    m_pEditPassword->setFontColor   ( ccGREEN );
    m_pEditPassword->setPlaceHolder ( "Password :" );
    m_pEditPassword->setMaxLength   ( 6 );
    m_pEditPassword->setInputFlag   ( kCCEditBoxInputFlagPassword );
	m_pEditPassword->setInputMode   ( kCCEditBoxInputModeSingleLine );
    m_pEditPassword->setDelegate    ( this );
    this->addChild ( m_pEditPassword );
    
    // bottom
    m_pEditEmail = CCEditBox::create ( tEditBoxSize, CCScale9Sprite::create ( "extensions/yellow_edit.png" ) );
	m_pEditEmail->setPosition    ( ccp ( tLyrSize.cx / 2, tLyrSize.cy * 1 / 5 ) );
    m_pEditEmail->setFontColor   ( ccRED );
    m_pEditEmail->setPlaceHolder ( "Email :" );
    m_pEditEmail->setInputMode   ( kCCEditBoxInputModeEmailAddr );
    m_pEditEmail->setDelegate    ( this );
    this->addChild ( m_pEditEmail );

	return KD_TRUE;
}

KDvoid TestEditBox::editBoxEditingBegan ( CCEditBox* pEditBox )
{
	CCLOG ( "editBox %p Editing Began !", pEditBox );
}

KDvoid TestEditBox::editBoxEditingEnded ( CCEditBox* pEditBox )
{
	CCLOG ( "editBox %p Editing Ended !", pEditBox );
}

KDvoid TestEditBox::editBoxTextChanged ( CCEditBox* pEditBox, const KDchar* szText )
{
	CCLOG ( "editBox %p TextChanged, text : %s ", pEditBox, szText );
}

KDvoid TestEditBox::editBoxReturn ( CCEditBox* pEditBox )
{
    CCLOG ( "editBox %p was returned !" );
    
    if      ( pEditBox == m_pEditName )
    {
        m_pSubTitle->setString ( "Name EditBox return !" );
    }
    else if ( pEditBox == m_pEditPassword )
    {
        m_pSubTitle->setString ( "Password EditBox return !" ); 
    }
    else if ( pEditBox == m_pEditEmail )
    {
        m_pSubTitle->setString ( "Email EditBox return !" );
    }
}