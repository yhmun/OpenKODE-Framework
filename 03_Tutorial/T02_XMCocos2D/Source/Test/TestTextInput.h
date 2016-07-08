/* --------------------------------------------------------------------------
 *
 *      File            TestTextInput.h
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

#ifndef __TestTextInput__
#define __TestTextInput__

#include "../TestBasic.h"

class TestTextInput : public TestBasic, public CCIMEDelegate
{
	public :

		static  TestTextInput*	create ( KDvoid );

	protected :

		virtual KDbool			init   ( KDvoid );
		virtual KDuint			count  ( KDvoid );
        virtual KDvoid			onExit ( KDvoid );

		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

	protected :

		CCTextFieldTTF*			m_pTextField;
};

class TextFieldTTFDefaultTest : public TestTextInput
{
	public :

		virtual const KDchar*   subtitle ( KDvoid );
};

class TextFieldTTFActionTest : public TestTextInput, public CCTextFieldDelegate
{		
	public :

		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual	KDvoid			onExit   ( KDvoid );

		virtual KDbool			onTextFieldAttachWithIME ( CCTextFieldTTF* pSender );
		virtual KDbool			onTextFieldDetachWithIME ( CCTextFieldTTF* pSender );

		virtual KDbool			onTextFieldInsertText     ( CCTextFieldTTF* pSender, const KDchar* szText, KDsize uLen );
		virtual KDbool			onTextFieldDeleteBackward ( CCTextFieldTTF* pSender, const KDchar* szText, KDsize uLen );
		 
		KDvoid					callbackRemoveNodeWhenDidAction ( CCNode* pNode );

	protected :

		KDuint					m_uLimit;
		KDsize					m_uLength;
		CCAction*				m_pAction;     
};

#endif	// __TestTextInput__
