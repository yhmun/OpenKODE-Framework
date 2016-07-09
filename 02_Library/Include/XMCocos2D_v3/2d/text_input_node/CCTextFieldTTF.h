/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextFieldTTF.h
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

#ifndef __CCTextFieldTTF_h__
#define __CCTextFieldTTF_h__

#include "../label_nodes/CCLabelTTF.h"
#include "../text_input_node/CCIMEDelegate.h"

NS_CC_BEGIN

class TextFieldTTF;

/**
 * @addtogroup input
 * @{
 */

class CC_DLL TextFieldDelegate
{
	public :

		virtual ~TextFieldDelegate ( KDvoid ) { }

		/**
		 *	@brief    If the sender doesn't want to attach to the IME, return true;
		 */
		virtual KDbool		onTextFieldAttachWithIME ( TextFieldTTF* pSender )
		{
			CC_UNUSED_PARAM ( pSender );
			return false;
		}

		/**
		 *	@brief    If the sender doesn't want to detach from the IME, return true;
		 */
		virtual KDbool		onTextFieldDetachWithIME ( TextFieldTTF* pSender )
		{
			CC_UNUSED_PARAM ( pSender );
			return false;
		}

		/**
		 *	@brief    If the sender doesn't want to insert the text, return true;
		 */
		virtual KDbool		onTextFieldInsertText ( TextFieldTTF* pSender, const KDchar* pText, KDint nLen )
		{
			CC_UNUSED_PARAM ( pSender );
			CC_UNUSED_PARAM ( pText );
			CC_UNUSED_PARAM ( nLen );
			return false;
		}

		/**
		 *	@brief    If the sender doesn't want to delete the delText, return true;
		 */
		virtual KDbool		onTextFieldDeleteBackward ( TextFieldTTF* pSender, const KDchar* pDelText, KDint nLen )
		{
			CC_UNUSED_PARAM ( pSender );
			CC_UNUSED_PARAM ( pDelText );
			CC_UNUSED_PARAM ( nLen );
			return false;
		}

		/**
		 *	@brief    If the sender doesn't want to draw, return true.
		 */
		virtual KDbool		onDraw ( TextFieldTTF* pSender )
		{
			CC_UNUSED_PARAM ( pSender );
			return false;
		}
};

/**
 *	@brief    A simple text input field with TTF font.
 */
class CC_DLL TextFieldTTF : public LabelTTF, public IMEDelegate
{
	public :

		/**
		 *	@js ctor
		 */
		TextFieldTTF ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TextFieldTTF ( KDvoid );

//		KDchar*						description ( KDvoid );

		/** creates a TextFieldTTF from a fontname, alignment, dimension and font size */
		static TextFieldTTF*		textFieldWithPlaceHolder ( const std::string& sPlaceholder, const Size& tDimensions, TextHAlignment eAlignment, const std::string& sFontName, KDfloat fFontSize );

		/** creates a LabelTTF from a fontname and font size */
		static TextFieldTTF*		textFieldWithPlaceHolder ( const std::string& sPlaceholder, const std::string& sFontName, KDfloat fFontSize );

		/** initializes the TextFieldTTF with a font name, alignment, dimension and font size */
		KDbool						initWithPlaceHolder ( const std::string& sPlaceholder, const Size& tDimensions, TextHAlignment eAlignment, const std::string& sFontName, KDfloat fFontSize );

		/** initializes the TextFieldTTF with a font name and font size */
		KDbool						initWithPlaceHolder ( const std::string& sPlaceholder, const std::string& sFontName, KDfloat fFontSize );

		/**
		 *	@brief    Open keyboard and receive input text.
		 */
		virtual KDbool				attachWithIME ( KDvoid );

		/**
		 *	@brief    End text input and close keyboard.
		 */
		virtual KDbool				detachWithIME ( KDvoid );

		//////////////////////////////////////////////////////////////////////////
		// properties
		//////////////////////////////////////////////////////////////////////////
		/**
		 *	@js NA
		 *	@lua NA
		 */
		inline TextFieldDelegate*	getDelegate ( KDvoid ) const { return m_pDelegate; }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		inline KDvoid				setDelegate ( TextFieldDelegate* pDelegate ) { m_pDelegate = pDelegate; }

		inline KDint				getCharCount ( KDvoid ) const { return m_nCharCount; };
		virtual const Color3B&		getColorSpaceHolder ( KDvoid );
		virtual KDvoid				setColorSpaceHolder ( const Color3B& tColor );

		// input text property

	public :

		virtual KDvoid				setString ( const std::string& sText ) override;
		virtual const std::string&	getString ( KDvoid ) const override;

	protected :

		TextFieldDelegate*			m_pDelegate;
		KDint						m_nCharCount;
    
		std::string					m_sInputText;

		// place holder text property
		// place holder text displayed when there is no text in the text field.
	public :

		virtual KDvoid				setPlaceHolder ( const std::string& sText );
		virtual const std::string&	getPlaceHolder ( KDvoid ) const;

	protected :

		std::string					m_sPlaceHolder;
		Color3B						m_tColorSpaceHolder;

	public :

		virtual KDvoid				setSecureTextEntry ( KDbool bValue );
		virtual KDbool				 isSecureTextEntry ( KDvoid );

	protected :

		KDbool						m_bSecureTextEntry;

	protected :

		virtual KDvoid				draw ( KDvoid );

		//////////////////////////////////////////////////////////////////////////
		// IMEDelegate interface
		//////////////////////////////////////////////////////////////////////////

		virtual KDbool				canAttachWithIME ( KDvoid ) override;
		virtual KDbool				canDetachWithIME ( KDvoid ) override;
		virtual KDvoid				insertText ( const KDchar* pText, KDint nLen ) override;
		virtual KDvoid				deleteBackward ( KDvoid ) override;
		virtual const KDchar*		getContentText ( KDvoid ) override;

	private :

		class LengthStack;
		LengthStack*				m_pLens;
};

//	end of input group
/// @}

NS_CC_END

#endif	// __CCTextFieldTTF_h__
