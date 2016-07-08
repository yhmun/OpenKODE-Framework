/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextFieldTTF.h
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

#ifndef __CCTextFieldTTF_h__
#define __CCTextFieldTTF_h__

#include "CCIMEDelegate.h"
#include "../label_nodes/CCLabelTTF.h"
#include "../touch_dispatcher/CCTouchDelegateProtocol.h"

NS_CC_BEGIN
    
class CCTextFieldTTF;

/**
 * @addtogroup input
 * @{
 */

class CCTextFieldDelegate
{
	public :

		/**
		 *	@brief	If the sender doesn't want to attach to the IME, return true;
		 */
		virtual KDbool		onTextFieldAttachWithIME ( CCTextFieldTTF* pSender ) 
		{
			CC_UNUSED_PARAM ( pSender ); 
			return KD_FALSE; 
		}

		/**
		 *	@brief	If the sender doesn't want to detach from the IME, return true;
		 */
		virtual KDbool		onTextFieldDetachWithIME ( CCTextFieldTTF* pSender )
		{
			CC_UNUSED_PARAM ( pSender ); 
			return KD_FALSE;
		}

		/**
		 *	@brief	If the sender doesn't want to insert the text, return true;
		 */
		virtual KDbool		onTextFieldInsertText ( CCTextFieldTTF* pSender, const KDchar* szText, KDsize uLen )
		{
			CC_UNUSED_PARAM ( pSender ); 
			CC_UNUSED_PARAM ( szText ); 
			CC_UNUSED_PARAM ( uLen ); 
			return KD_FALSE;
		}

		/**
		 *	@brief	If the sender doesn't want to delete the delText, return true;
		 */
		virtual KDbool		onTextFieldDeleteBackward ( CCTextFieldTTF* pSender, const KDchar* szText, KDsize uLen ) 
		{
			CC_UNUSED_PARAM ( pSender ); 
			CC_UNUSED_PARAM ( szText ); 
			CC_UNUSED_PARAM ( uLen ); 
			return KD_FALSE;
		}

		/**
		 *	@brief	If the sender doesn't want to draw, return true.
		 */
		virtual KDbool		onDraw ( CCTextFieldTTF* pSender )
		{
			CC_UNUSED_PARAM ( pSender ); 
			return KD_FALSE;
		}
};

/**
 *	@brief	A simple text input field with TTF font.
 */
class CCTextFieldTTF : public CCLabelTTF, public CCIMEDelegate
{
	public :
		         CCTextFieldTTF ( KDvoid );
		virtual ~CCTextFieldTTF ( KDvoid );

		/** creates a CCTextFieldTTF from a fontname, alignment, dimension and font size */
		static   CCTextFieldTTF*	create ( const KDchar* szPlaceHolder, const CCSize& tDimension, CCTextAlignment eAlignment, const KDchar* szFontPath, KDfloat fFontSize );

		/** creates a CCLabelTTF from a fontname and font size */
		static   CCTextFieldTTF*	create ( const KDchar* szPlaceHolder, const KDchar* fontName, KDfloat fontSize );

	public :

		/** initializes the CCTextFieldTTF with a font name, alignment, dimension and font size */
		virtual KDbool		initWithPlaceHolder ( const KDchar* szPlaceHolder, const CCSize& tDimension, CCTextAlignment eAlignment, const KDchar* szFontPath, KDfloat fFontSize );

		/** initializes the CCTextFieldTTF with a font name and font size */
		virtual KDbool		initWithPlaceHolder ( const KDchar* szPlaceHolder, const KDchar* szFontPath, KDfloat fFontSize );

		/**
		 *	@brief	Open keyboard and receive input text.
		 */
		virtual KDbool		attachWithIME ( KDvoid );

		/**
		 *	@brief	End text input and close keyboard.
		 */
		virtual KDbool		detachWithIME ( KDvoid );

		//////////////////////////////////////////////////////////////////////////
		// properties
		//////////////////////////////////////////////////////////////////////////
	    
		virtual const ccColor3B&	getColorSpaceHolder ( KDvoid );
		virtual KDvoid				setColorSpaceHolder ( const ccColor3B& tColor );

		CC_SYNTHESIZE ( CCTextFieldDelegate*, m_pDelegate, Delegate );
		CC_SYNTHESIZE_READONLY ( KDsize, m_nCharCount, CharCount );

	public :

		// input text property
		virtual KDvoid			setString ( const KDchar* szText );
		virtual const KDchar*	getString ( KDvoid );

		// place holder text property
		// place holder text displayed when there is no text in the text field.
		virtual KDvoid			setPlaceHolder ( const KDchar* szText );
		virtual const KDchar*	getPlaceHolder ( KDvoid );

		KDvoid					setReplaceCharacter ( const KDchar* szChar );


		virtual KDvoid			setSecureTextEntry ( KDbool bValue );
		virtual KDbool			 isSecureTextEntry ( KDvoid );

	protected :

		virtual KDvoid			draw ( KDvoid );

		//////////////////////////////////////////////////////////////////////////
		// CCIMEDelegate interface
		//////////////////////////////////////////////////////////////////////////
		virtual KDbool			canAttachWithIME ( KDvoid );
		virtual KDbool			canDetachWithIME ( KDvoid );

		virtual KDvoid			insertText ( const KDchar* szText, KDsize uLen );
		virtual KDvoid			deleteBackward ( KDvoid );
		virtual const KDchar*	getContentText ( KDvoid );

	protected :

		std::string				m_sInputText;
		std::string				m_sPlaceHolder;
		std::string				m_sReplaceChar;
		ccColor3B				m_tColorSpaceHolder;
		KDbool					m_bSecureTextEntry;

	private :
		class LengthStack;
		LengthStack*			m_pLens;
};

// end of input group
/// @}

NS_CC_END

#endif // __CCTextFieldTTF_h__