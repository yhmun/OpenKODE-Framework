/* -----------------------------------------------------------------------------------
 *
 *      File            UITextField.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __UITextField_h__
#define __UITextField_h__

#include "../BaseClasses/UIWidget.h"
#include "../../2d/text_input_node/CCTextFieldTTF.h"

namespace gui {

/**
 *  @js NA
 *  @lua NA
 */
class UICCTextField : public cocos2d::TextFieldTTF, public cocos2d::TextFieldDelegate
{
	public :

		 UICCTextField ( KDvoid );
		~UICCTextField ( KDvoid );
    
		virtual KDvoid					onEnter ( KDvoid );
    
		// static
		static UICCTextField*			create ( const KDchar* pPlaceholder, const KDchar* pFontName, KDfloat fFontSize );
    
		// CCTextFieldDelegate
		virtual KDbool					onTextFieldAttachWithIME ( cocos2d::TextFieldTTF* pSender );
		virtual KDbool					onTextFieldDetachWithIME ( cocos2d::TextFieldTTF* pSender );
		virtual KDbool					onTextFieldInsertText ( cocos2d::TextFieldTTF* pSender, const KDchar* pText, KDint nLen );
		virtual KDbool					onTextFieldDeleteBackward ( cocos2d::TextFieldTTF* pSender, const KDchar* pDelText, KDint nLen );
    
		KDvoid							insertText ( const KDchar* pText, KDint nLen );
		KDvoid							deleteBackward ( KDvoid );
    
		KDvoid							openIME ( KDvoid );
		KDvoid							closeIME ( KDvoid );
    
		KDvoid							setMaxLengthEnabled ( KDbool bEnable );
		KDbool							 isMaxLengthEnabled ( KDvoid );

		KDvoid							setMaxLength ( KDint nLength );
		KDint							getMaxLength ( KDvoid );
		KDint							getCharCount ( KDvoid );

		KDvoid							setPasswordEnabled ( KDbool bEnable );
		KDbool							 isPasswordEnabled ( KDvoid );

		KDvoid							setPasswordStyleText ( const KDchar* pStyleText );
		KDvoid							setPasswordText ( const KDchar* pText );

		KDvoid							setAttachWithIME ( KDbool bAttach );
		KDbool							getAttachWithIME ( KDvoid );

		KDvoid							setDetachWithIME ( KDbool bDetach );
		KDbool							getDetachWithIME ( KDvoid );

		KDvoid							setInsertText ( KDbool bInsert );
		KDbool							getInsertText ( KDvoid );

		KDvoid							setDeleteBackward ( KDbool bDeleteBackward );
		KDbool							getDeleteBackward ( KDvoid );

	protected :

		KDbool							m_bMaxLengthEnabled;
		KDint							m_nMaxLength;
		KDbool							m_bPasswordEnabled;
		std::string						m_sPasswordStyleText;
		KDbool							m_bAttachWithIME;
		KDbool							m_bDetachWithIME;
		KDbool							m_bInsertText;
		KDbool							m_bDeleteBackward;
};

typedef enum
{
    TEXTFIELD_EVENT_ATTACH_WITH_IME,
    TEXTFIELD_EVENT_DETACH_WITH_IME,
    TEXTFIELD_EVENT_INSERT_TEXT,
    TEXTFIELD_EVENT_DELETE_BACKWARD,
} TextFiledEventType;

typedef KDvoid ( cocos2d::Object::*SEL_TextFieldEvent ) ( cocos2d::Object*, TextFiledEventType );
#define textfieldeventselector(_SELECTOR) (SEL_TextFieldEvent) ( &_SELECTOR )

/** 
 *	class UITextField : public UIWidget
 *  @js NA
 *  @lua NA
 */
class UITextField : public UIWidget
{
	public :

		UITextField ( KDvoid );
		virtual ~UITextField ( KDvoid );

		static UITextField*				create ( KDvoid );

		virtual KDbool					init ( KDvoid ) override;
		virtual KDvoid					initRenderer ( KDvoid ) override;

		KDvoid							setTouchSize ( const cocos2d::Size& tSize );
		KDvoid							setText ( const std::string& sText );
		KDvoid							setPlaceHolder ( const std::string& sValue );
		KDvoid							setFontSize ( KDint nSize );
		KDvoid							setFontName ( const std::string& sName );
		virtual KDvoid					didNotSelectSelf ( KDvoid );
		const std::string&				getStringValue ( KDvoid );
		virtual KDbool					onTouchBegan ( const cocos2d::Point& tTouchPoint ) override;

		KDvoid							setMaxLengthEnabled ( KDbool bEnable );
		KDbool							 isMaxLengthEnabled ( KDvoid );

		KDvoid							setMaxLength ( KDint nLength );
		KDint							getMaxLength ( KDvoid );

		KDvoid							setPasswordEnabled ( KDbool bEnable );
		KDbool							 isPasswordEnabled ( KDvoid );
		KDvoid							setPasswordStyleText ( const KDchar* pStyleText );

		virtual KDvoid					update ( KDfloat dt ) override;

		KDbool							getAttachWithIME ( KDvoid );
		KDvoid							setAttachWithIME ( KDbool bAttach );
		KDbool							getDetachWithIME ( KDvoid );
		KDvoid							setDetachWithIME ( KDbool bDetach );

		KDbool							getInsertText ( KDvoid );
		KDvoid							setInsertText ( KDbool bInsertText );

		KDbool							getDeleteBackward ( KDvoid );
		KDvoid							setDeleteBackward ( KDbool bDeleteBackward );

		KDvoid							addEventListenerTextField ( cocos2d::Object* pTarget, SEL_TextFieldEvent pSelecor );

		virtual KDvoid					setAnchorPoint ( const cocos2d::Point& pt ) override;
		virtual KDvoid					setColor ( const cocos2d::Color3B& tColor ) override;
		virtual KDvoid					setOpacity ( KDint nOpacity ) override;
    
		/**
		 *	Returns the "class name" of widget.
		 */
		virtual const KDchar*			getDescription ( KDvoid ) const override;

		virtual const cocos2d::Size&	getContentSize ( KDvoid ) const override;
		virtual cocos2d::Node*			getVirtualRenderer ( KDvoid ) override;
		KDvoid							attachWithIME ( KDvoid );

	protected :

		// event
		KDvoid							attachWithIMEEvent ( KDvoid );
		KDvoid							detachWithIMEEvent ( KDvoid );
		KDvoid							insertTextEvent ( KDvoid );
		KDvoid							deleteBackwardEvent ( KDvoid );
		virtual KDvoid					onSizeChanged ( KDvoid ) override;
		KDvoid							textfieldRendererScaleChangedWithSize ( KDvoid );
		virtual UIWidget*				createCloneInstance ( KDvoid ) override;
		virtual KDvoid					copySpecialProperties ( UIWidget* pModel ) override;

	protected :

		UICCTextField*					m_pTextFieldRenderer;

		KDfloat							m_fTouchWidth;
		KDfloat							m_fTouchHeight;
		KDbool							m_bUseTouchArea;
    
		cocos2d::Object*				m_pTextFieldEventListener;
		SEL_TextFieldEvent				m_pTextFieldEventSelector;
    
		std::string						m_sPasswordStyleText;
};

}

#endif	// __UITextField_h__
