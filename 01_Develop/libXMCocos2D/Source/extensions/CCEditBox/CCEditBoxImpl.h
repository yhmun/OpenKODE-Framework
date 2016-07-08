/* --------------------------------------------------------------------------
 *
 *      File            CCEditBoxImpl.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      James Chen
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __CCEditBoxImpl_h__
#define __CCEditBoxImpl_h__

NS_CC_BEGIN

class CCEditBoxImpl
{
	public :

		CCEditBoxImpl ( CCEditBox* pEditBox ) 
		{
			m_pEditBox  = pEditBox;
			m_pDelegate = KD_NULL;
		}

		virtual ~CCEditBoxImpl ( KDvoid ) 
		{ 

		}
	    
	public :

		virtual KDbool  initWithSize ( const CCSize& tSize ) = 0;
		
		virtual KDvoid  setFontColor ( const ccColor3B& tColor ) = 0;
		virtual KDvoid  setPlaceholderFontColor ( const ccColor3B& tColor ) = 0;
		
		virtual KDvoid  setInputMode ( EditBoxInputMode eInputMode ) = 0;
		virtual KDvoid  setInputFlag ( EditBoxInputFlag eInputFlag ) = 0;
		
		virtual KDvoid  setMaxLength ( KDint nMaxLength ) = 0;
		virtual KDint   getMaxLength ( KDvoid ) = 0;

		virtual KDvoid  setReturnType ( KeyboardReturnType eReturnType ) = 0;

		virtual KDbool  isEditing ( KDvoid ) = 0;
	    
		virtual KDvoid         setText ( const KDchar* szText ) = 0;
		virtual const KDchar*  getText ( KDvoid ) = 0;

		virtual KDvoid  setPlaceHolder ( const KDchar* szText ) = 0;

		virtual KDvoid  doAnimationWhenKeyboardMove ( KDfloat fDuration, KDfloat fDistance ) = 0;
	    
		virtual KDvoid  openKeyboard  ( KDvoid ) = 0;
		virtual KDvoid  closeKeyboard ( KDvoid ) = 0;
	    
		virtual KDvoid  setPosition ( const CCPoint& tPosition ) = 0;
		virtual KDvoid  setContentSize (const CCSize& tSize ) = 0;

		virtual KDvoid  visit ( KDvoid ) = 0;
	   
		inline  KDvoid  setDelegate ( CCEditBoxDelegate* pDelegate )
		{
			m_pDelegate = pDelegate; 
		};

		inline  CCEditBoxDelegate*  getDelegate ( KDvoid ) 
		{
			return m_pDelegate; 
		};

		inline  CCEditBox*  getCCEditBox ( KDvoid )
		{
			return m_pEditBox; 
		};

	protected :

		CCEditBoxDelegate*		m_pDelegate;
		CCEditBox*				m_pEditBox;
};

NS_CC_END

#endif	// __CCEditBoxImpl_h__
