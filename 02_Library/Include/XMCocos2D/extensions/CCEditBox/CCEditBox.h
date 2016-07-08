/* --------------------------------------------------------------------------
 *
 *      File            CCEditBox.h
 *      Author          Young-Hwan Mun
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

#ifndef __CCEditBox_h__
#define __CCEditBox_h__

#include "../../text_input_node/CCIMEDelegate.h"
#include "../CCControl/CCControlButton.h"

NS_CC_BEGIN

enum CCKeyboardReturnType 
{
    kCCKeyboardReturnTypeDefault	= 0 ,
    kCCKeyboardReturnTypeDone			,
    kCCKeyboardReturnTypeSend			,
    kCCKeyboardReturnTypeSearch		,
    kCCKeyboardReturnTypeGo			,
};

/**
 *	@brief The EditBoxInputMode defines the type of text that the user is allowed
 *	to enter.
 */
enum CCEditBoxInputMode
{
    /**	The user is allowed to enter any text, including line breaks. */
    kCCEditBoxInputModeAny = 0,
    
    /**	The user is allowed to enter an e-mail address. */
    kCCEditBoxInputModeEmailAddr,

    /**	The user is allowed to enter an integer value. */
    kCCEditBoxInputModeNumeric,

    /**	The user is allowed to enter a phone number. */
    kCCEditBoxInputModePhoneNumber,

    /**	The user is allowed to enter a URL. */
    kCCEditBoxInputModeUrl,

    /**
     * The user is allowed to enter a real number value.
     * This extends kEditBoxInputModeNumeric by allowing a decimal point.
     */
    kCCEditBoxInputModeDecimal,

    /**	The user is allowed to enter any text, except for line breaks. */
    kCCEditBoxInputModeSingleLine
};

/**
 *	@brief The EditBoxInputFlag defines how the input text is displayed/formatted.
 */
enum CCEditBoxInputFlag
{
    /**
     *	Indicates that the text entered is confidential data that should be
     *	obscured whenever possible. This implies EDIT_BOX_INPUT_FLAG_SENSITIVE.
     */
    kCCEditBoxInputFlagPassword = 0,

    /**
     *	Indicates that the text entered is sensitive data that the
     *	implementation must never store into a dictionary or table for use
     *	in predictive, auto-completing, or other accelerated input schemes.
     *	A credit card number is an example of sensitive data.
     */
    kCCEditBoxInputFlagSensitive,

    /**
     *	This flag is a hint to the implementation that during text editing,
     *	the initial letter of each word should be capitalized.
     */
    kCCEditBoxInputFlagInitialCapsWord,

    /**
     *	This flag is a hint to the implementation that during text editing,
     *	the initial letter of each sentence should be capitalized.
     */
    kCCEditBoxInputFlagInitialCapsSentence,

    /**
     *	Capitalize all characters automatically.
     */
    kCCEditBoxInputFlagInitialCapsAllCharacters

};

class CCEditBox;

class CCEditBoxDelegate 
{
	public :

		virtual ~CCEditBoxDelegate ( KDvoid ) { };
	    
	public :

		/**
		 *	This method is called when an edit box gains focus after keyboard is shown.
		 *	@param editBox The edit box object that generated the event.
		 */
		virtual KDvoid  editBoxEditingBegan ( CCEditBox* pEditBox ) { };
	    
	    
		/**
		 *	This method is called when an edit box loses focus after keyboard is hidden.
		 *	@param editBox The edit box object that generated the event.
		 */
		virtual KDvoid  editBoxEditingEnded ( CCEditBox* pEditBox ) { };
	    
		/**
		 *	This method is called when the edit box text was changed.
		 *	@param editBox The edit box object that generated the event.
		 *	@param text The new text.
		 */
		virtual KDvoid  editBoxTextChanged ( CCEditBox* pEditBox, const KDchar* szText ) { };
	    
		/**
		 *	This method is called when the return button was pressed or the outside area of keyboard was touched.
		 *	@param editBox The edit box object that generated the event.
		 */
		virtual KDvoid  editBoxReturn ( CCEditBox* pEditBox ) = 0;
    
};

/**
 *	@brief Class for edit box.
 *
 *	You can use this widget to gather small amounts of text from the user.
 */
 
class CCEditBox : public CCControlButton, public CCIMEDelegate
{
	public :

		/**	Constructor. */
		CCEditBox ( KDvoid );
	    
		/**	Destructor. */
		virtual ~CCEditBox ( KDvoid );

		/**
		 *	create a edit box with size.
		 *	@return An autorelease pointer of CCEditBox, you don't need to release it only if you retain it again.
		 */
		static CCEditBox*  create ( const CCSize& tSize, CCScale9Sprite* pNormal9SpriteBg, CCScale9Sprite* pPressed9SpriteBg = KD_NULL, CCScale9Sprite* pDisabled9SpriteBg = KD_NULL );
	    
	public :

		/**
		 *	Init edit box with specified size. This method should be invoked right after constructor.
		 *	@param size The size of edit box.
		 */
		virtual KDbool  initWithSizeAndBackgroundSprite ( const CCSize& tSize, CCScale9Sprite* pNormal9SpriteBg );
	    
		/**
		 *	Set the delegate for edit box.
		 */
		KDvoid  setDelegate ( CCEditBoxDelegate* pDelegate );
	    
		/**
		 *	Set the text entered in the edit box.
		 *	@param pText The given text.
		 */
		KDvoid  setText ( const KDchar* szText );
	    
		/**
		 *	Get the text entered in the edit box.
		 *	@return The text entered in the edit box.
		 */
		const KDchar*  getText ( KDvoid );
	    
		/**
		 *	Set the font color of the widget's text.
		 */
		KDvoid  setFontColor ( const ccColor3B& tColor );
	    
		/**
		 *	Set the font color of the placeholder text when the edit box is empty.
		 *	Not supported on IOS.
		 */
		KDvoid  setPlaceholderFontColor ( const ccColor3B& tColor );
	    
		/**
		 *	Set a text in the edit box that acts as a placeholder when an
		 *	edit box is empty.
		 *	@param pText The given text.
		 */
		KDvoid  setPlaceHolder ( const KDchar* szText );
	    
		/**
		 *	Get a text in the edit box that acts as a placeholder when an
		 *	edit box is empty.
		 */
		const KDchar*  getPlaceHolder ( KDvoid );
	    
		/**
		 *	Set the input mode of the edit box.
		 *	@param inputMode One of the EditBoxInputMode constants.
		 */
		KDvoid  setInputMode ( CCEditBoxInputMode eInputMode );
	    
		/**
		 *	Sets the maximum input length of the edit box.
		 *	Setting this value enables multiline input mode by default.
		 *	Available on Android, iOS and Windows Phone.
		 *
		 *	@param maxLength The maximum length.
		 */
		KDvoid  setMaxLength ( KDint nMaxLength );
	    
		/**
		 *	Gets the maximum input length of the edit box.
		 *
		 *	@return Maximum input length.
		 */
		KDint  getMaxLength ( KDvoid );
	    
		/**
		 *	Set the input flags that are to be applied to the edit box.
		 *	@param inputFlag One of the EditBoxInputFlag constants.
		 */
		KDvoid  setInputFlag ( CCEditBoxInputFlag eInputFlag );
	    
		/**
		 *	Set the return type that are to be applied to the edit box.
		 *	@param returnType One of the CCKeyboardReturnType constants.
		 */
		KDvoid  setReturnType ( CCKeyboardReturnType eReturnType );
	    
		// override functions 
		virtual KDvoid  setPosition ( const CCPoint& tPosition );
		virtual KDvoid  setContentSize ( const CCSize& tSize );

		virtual KDvoid  visit  ( KDvoid );
		virtual KDvoid  onExit ( KDvoid );

		virtual KDvoid  keyboardWillShow ( CCIMEKeyboardNotificationInfo& tInfo );
		virtual KDvoid  keyboardDidShow  ( CCIMEKeyboardNotificationInfo& tInfo );
		virtual KDvoid  keyboardWillHide ( CCIMEKeyboardNotificationInfo& tInfo );
		virtual KDvoid  keyboardDidHide  ( CCIMEKeyboardNotificationInfo& tInfo );
	    
		// callback funtions 
		KDvoid  touchDownAction ( CCObject* pSender, CCControlEvent uControlEvent );
	    
	protected :

		CCEditBoxDelegate*		m_pDelegate;
	    
		CCEditBoxInputMode		m_eEditBoxInputMode;
		CCEditBoxInputFlag		m_eEditBoxInputFlag;
		CCKeyboardReturnType	m_eKeyboardReturnType;
	    
		std::string				m_sText;
		std::string				m_sPlaceHolder;
	    
		ccColor3B				m_tColText;
		ccColor3B				m_tColPlaceHolder;
	    
		KDint					m_nMaxLength;
		KDfloat					m_fAdjustHeight;
};

NS_CC_END

#endif	// __CCEditBox_h__

