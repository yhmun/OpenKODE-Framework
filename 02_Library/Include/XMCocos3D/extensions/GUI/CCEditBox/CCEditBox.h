/* -----------------------------------------------------------------------------------
 *
 *      File            CCEditBox.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2010-2012 cocos2d-x.org.
 *      Copyright (c) 2012      James Chen
 *
 *          http://www.cocos2d-x.org
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCEditBox_h__
#define __CCEditBox_h__

#include "extensions/ExtensionMacros.h"
#include "../CCControlExtension/CCControlExtensions.h"
#include "../../../2d/text_input_node/CCIMEDelegate.h"

NS_CC_EXT_BEGIN

class EditBox;
class EditBoxImpl;

class EditBoxDelegate 
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~EditBoxDelegate ( KDvoid ) { }
    
		/**
		 *	This method is called when an edit box gains focus after keyboard is shown.
		 *	@param editBox The edit box object that generated the event.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				editBoxEditingDidBegin ( EditBox* pEditBox ) { }
    
    
		/**
		 *	This method is called when an edit box loses focus after keyboard is hidden.
		 *	@param editBox The edit box object that generated the event.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				editBoxEditingDidEnd ( EditBox* pEditBox) { }
    
		/**
		 *	This method is called when the edit box text was changed.
		 *	@param editBox The edit box object that generated the event.
		 *	@param text The new text.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				editBoxTextChanged ( EditBox* pEditBox, const std::string& sText ) { }
    
		/**
		 *	This method is called when the return button was pressed or the outside area of keyboard was touched.
		 *	@param editBox The edit box object that generated the event.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				editBoxReturn ( EditBox* pEditBox ) = 0;
    
};

/**
 *	@brief Class for edit box.
 *
 *	You can use this widget to gather small amounts of text from the user.
 */
 
class EditBox : public ControlButton , public IMEDelegate
{
	public :

		enum class KeyboardReturnType
		{
			DEFAULT,
			DONE,
			SEND,
			SEARCH,
			GO
		};
    
		/**
		 *	@brief The EditBox::InputMode defines the type of text that the user is allowed
		 *	to enter.
		 */
		enum class InputMode
		{
			/**
			 *	The user is allowed to enter any text, including line breaks.
			 */
			ANY,
        
			/**
			 *	The user is allowed to enter an e-mail address.
			 */
			EMAIL_ADDRESS,
        
			/**
			 *	The user is allowed to enter an integer value.
			 */
			NUMERIC,
        
			/**
			 *	The user is allowed to enter a phone number.
			 */
			PHONE_NUMBER,
        
			/**
			 *	The user is allowed to enter a URL.
			 */
			URL,
        
			/**
			 *	The user is allowed to enter a real number value.
			 *	This extends kEditBoxInputModeNumeric by allowing a decimal point.
			 */
			DECIMAL,
        
			/**
			 *	The user is allowed to enter any text, except for line breaks.
			 */
			SINGLE_LINE,
		};
    
		/**
		 *	@brief The EditBox::InputFlag defines how the input text is displayed/formatted.
		 */
		enum class InputFlag
		{
			/**
			 *	Indicates that the text entered is confidential data that should be
			 *	obscured whenever possible. This implies EDIT_BOX_INPUT_FLAG_SENSITIVE.
			 */
			PASSWORD,
        
			/**
			 *	Indicates that the text entered is sensitive data that the
			 *	implementation must never store into a dictionary or table for use
			 *	in predictive, auto-completing, or other accelerated input schemes.
			 *	A credit card number is an example of sensitive data.
			 */
			SENSITIVE,
        
			/**
			 *	This flag is a hint to the implementation that during text editing,
			 *	the initial letter of each word should be capitalized.
			 */
			INITIAL_CAPS_WORD,
        
			/**
			 *	This flag is a hint to the implementation that during text editing,
			 *	the initial letter of each sentence should be capitalized.
			 */
			INITIAL_CAPS_SENTENCE,
        
			/**
			 *	Capitalize all characters automatically.
			 */
			INTIAL_CAPS_ALL_CHARACTERS,
		};
    
		/**
		 *	create a edit box with size.
		 *	@return An autorelease pointer of EditBox, you don't need to release it only if you retain it again.
		 */
		static EditBox*					create ( const Size& tSize, Scale9Sprite* pNormal9SpriteBg, Scale9Sprite* pPressed9SpriteBg = KD_NULL, Scale9Sprite* pDisabled9SpriteBg = KD_NULL );

		/**
		 *	Constructor.
		 *	@js ctor
		 */
		EditBox ( KDvoid );
    
		/**
		 *	Destructor.
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~EditBox ( KDvoid );

		/**
		 *	Init edit box with specified size. This method should be invoked right after constructor.
		 *	@param size The size of edit box.
		 */
		KDbool							initWithSizeAndBackgroundSprite ( const Size& tSize, Scale9Sprite* pNormal9SpriteBg );
    
		/**
		 *	Gets/Sets the delegate for edit box.
		 *	@lua NA
		 */
		KDvoid							setDelegate ( EditBoxDelegate* pDelegate );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		EditBoxDelegate*				getDelegate ( KDvoid );

		/**
		 *	Registers a script function that will be called for EditBox events.
		 *
		 *	This handler will be removed automatically after onExit() called.
		 *	@code
		 *			-- lua sample
		 *	local function editboxEventHandler(eventType)
		 *     if eventType == "began" then
		 *			-- triggered when an edit box gains focus after keyboard is shown
		 *     elseif eventType == "ended" then
		 *			-- triggered when an edit box loses focus after keyboard is hidden.
		 *     elseif eventType == "changed" then
		 *			-- triggered when the edit box text was changed.
		 *     elseif eventType == "return" then
		 *			-- triggered when the return button was pressed or the outside area of keyboard was touched.
		 *     end
		 *	end
		 *
		 *	local editbox = EditBox:create(Size(...), Scale9Sprite:create(...))
		 *	editbox = registerScriptEditBoxHandler(editboxEventHandler)
		 *	@endcode
		 *
		 *	@param handler A number that indicates a lua function.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid							registerScriptEditBoxHandler ( KDint nHandler );
    
		/**
		 *	Unregisters a script function that will be called for EditBox events.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid							unregisterScriptEditBoxHandler ( KDvoid );

		/**
		 *	get a script Handler
		 *	@js NA
		 *	@lua NA
		 */
		KDint							getScriptEditBoxHandler ( KDvoid ) { return m_nScriptEditBoxHandler; }

		/**
		 *	Set the text entered in the edit box.
		 *	@param pText The given text.
		 */
		KDvoid							setText ( const KDchar* pText );
    
		/**
		 *	Get the text entered in the edit box.
		 *	@return The text entered in the edit box.
		 */
		const KDchar*					getText ( KDvoid );
	
		/**
		 *	Set the font.
		 *	@param pFontName The font name.
		 *	@param fontSize The font size.
		 */
		KDvoid							setFont ( const KDchar* pFontName, KDint nFontSize );
    
		/**
		 *	Set the font name.
		 *	@param pFontName The font name.
		 */
		KDvoid							setFontName ( const KDchar* pFontName );
    
		/**
		 *	Set the font size.
		 *	@param fontSize The font size.
		 */
		KDvoid							setFontSize ( KDint nFontSize );
    
		/**
		 *	Set the font color of the widget's text.
		 */
		KDvoid							setFontColor ( const Color3B& tColor );
    
		/**
		 *	Set the placeholder's font.
		 *	@param pFontName The font name.
		 *	@param fontSize The font size.
		 */
		KDvoid							setPlaceholderFont ( const KDchar* pFontName, KDint nFontSize );
    
		/**
		 *	Set the placeholder's font name.
		 *	@param pFontName The font name.
		 */
		KDvoid							setPlaceholderFontName ( const KDchar* pFontName );
    
		/**
		 *	Set the placeholder's font size.
		 *	@param fontSize The font size.
		 */
		KDvoid							setPlaceholderFontSize ( KDint nFontSize );
    
		/**
		 *	Set the font color of the placeholder text when the edit box is empty.
		 *	Not supported on IOS.
		 */
		KDvoid							setPlaceholderFontColor ( const Color3B& tColor );
    
		/**
		 *	Set a text in the edit box that acts as a placeholder when an
		 *	edit box is empty.
		 *	@param pText The given text.
		 */
		KDvoid							setPlaceHolder ( const KDchar* pText );
    
		/**
		 *	Get a text in the edit box that acts as a placeholder when an
		 *	edit box is empty.
		 */
		const KDchar*					getPlaceHolder ( KDvoid );
    
		/**
		 *	Set the input mode of the edit box.
		 *	@param inputMode One of the EditBox::InputMode constants.
		 */
		KDvoid							setInputMode ( InputMode eInputMode );
    
		/**
		 *	Sets the maximum input length of the edit box.
		 *	Setting this value enables multiline input mode by default.
		 *	Available on Android, iOS and Windows Phone.
		 *
		 *	@param maxLength The maximum length.
		 */
		KDvoid							setMaxLength ( KDint nMaxLength );
    
		/**
		 *	Gets the maximum input length of the edit box.
		 *	
		 *	@return Maximum input length.
		 */
		KDint							getMaxLength ( KDvoid );
    
		/**
		 *	Set the input flags that are to be applied to the edit box.
		 *	@param inputFlag One of the EditBox::InputFlag constants.
		 */
		KDvoid							setInputFlag ( InputFlag eInputFlag );
    
		/**
		 *	Set the return type that are to be applied to the edit box.
		 *	@param returnType One of the EditBox::KeyboardReturnType constants.
		 */
		KDvoid							setReturnType ( EditBox::KeyboardReturnType eReturnType );
    
		/* override functions */
		virtual KDvoid					setPosition ( const Point& tPos );
		virtual KDvoid					setVisible ( KDbool bVisible );
		virtual KDvoid					setContentSize ( const Size& tSize );
		virtual KDvoid					setAnchorPoint ( const Point& tAnchorPoint );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					visit ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onEnter ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onExit ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					keyboardWillShow ( IMEKeyboardNotificationInfo& tInfo );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					keyboardDidShow ( IMEKeyboardNotificationInfo& tInfo );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					keyboardWillHide ( IMEKeyboardNotificationInfo& tInfo );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					keyboardDidHide ( IMEKeyboardNotificationInfo& tInfo );
    
		/**
		 *	callback funtions
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid							touchDownAction ( Object* pSender, Control::EventType eControlEvent );
    
	protected :

		EditBoxImpl*					m_pEditBoxImpl;
		EditBoxDelegate*				m_pDelegate;
    
		InputMode						m_eEditBoxInputMode;
		InputFlag						m_eEditBoxInputFlag;
		EditBox::KeyboardReturnType		m_eKeyboardReturnType;
    
		std::string						m_sText;
		std::string						m_sPlaceHolder;
    
		std::string						m_sFontName;
		std::string						m_sPlaceholderFontName;
    
		KDint							m_nFontSize;
		KDint							m_nPlaceholderFontSize;
    
		Color3B							m_tColText;
		Color3B							m_tColPlaceHolder;
    
		KDint							m_nMaxLength;
		KDfloat							m_fAdjustHeight;
		KDint							m_nScriptEditBoxHandler;
};

NS_CC_EXT_END

#endif	// __CCEditBox_h__

