/* -----------------------------------------------------------------------------------
 *
 *      File            CCEventKeyboard.h
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

#ifndef __CCEventKeyboard_h__
#define __CCEventKeyboard_h__

#include "CCEvent.h"

NS_CC_BEGIN

class EventKeyboard : public Event
{
	public :

		/**
		 *	The key (code).
		 */
		enum class KeyCode
		{
			KEY_NONE				= KD_INPUT_KEY_NONE					,		
			KEY_PAUSE				= KD_INPUT_KEY_PAUSE				,				
			KEY_SCROLL_LOCK			= KD_INPUT_KEY_SCROLL_LOCK			,
			KEY_PRINT				= KD_INPUT_KEY_PRINT				,		
			KEY_SYSREQ				= KD_INPUT_KEY_SYSREQ				,            	
			KEY_BREAK				= KD_INPUT_KEY_BREAK				,           	
			KEY_ESCAPE				= KD_INPUT_KEY_ESCAPE				,            	
			KEY_BACKSPACE			= KD_INPUT_KEY_BACKSPACE			,
			KEY_TAB					= KD_INPUT_KEY_TAB					,
			KEY_BACK_TAB			= KD_INPUT_KEY_BACK_TAB				,
			KEY_RETURN				= KD_INPUT_KEY_RETURN				,
			KEY_CAPS_LOCK			= KD_INPUT_KEY_CAPS_LOCK			,
			KEY_SHIFT				= KD_INPUT_KEY_SHIFT				,
			KEY_CTRL				= KD_INPUT_KEY_CTRL					,
			KEY_ALT					= KD_INPUT_KEY_ALT					,	
			KEY_MENU				= KD_INPUT_KEY_MENU					,	
			KEY_HYPER				= KD_INPUT_KEY_HYPER				,	
			KEY_INSERT				= KD_INPUT_KEY_INSERT				,	
			KEY_HOME				= KD_INPUT_KEY_HOME					,	
			KEY_PG_UP				= KD_INPUT_KEY_PG_UP				,	
			KEY_DELETE				= KD_INPUT_KEY_DELETE				,		
			KEY_END					= KD_INPUT_KEY_END					,	
			KEY_PG_DOWN				= KD_INPUT_KEY_PG_DOWN				,	
			KEY_LEFT_ARROW			= KD_INPUT_KEY_LEFT_ARROW			,	
			KEY_RIGHT_ARROW			= KD_INPUT_KEY_RIGHT_ARROW			,	
			KEY_UP_ARROW			= KD_INPUT_KEY_UP_ARROW				,	
			KEY_DOWN_ARROW			= KD_INPUT_KEY_DOWN_ARROW			,	
			KEY_NUM_LOCK			= KD_INPUT_KEY_NUM_LOCK				,	
			KEY_KP_PLUS				= KD_INPUT_KEY_KP_PLUS				,	
			KEY_KP_MINUS			= KD_INPUT_KEY_KP_MINUS				,	
			KEY_KP_MULTIPLY			= KD_INPUT_KEY_KP_MULTIPLY			,	
			KEY_KP_DIVIDE			= KD_INPUT_KEY_KP_DIVIDE			,	
			KEY_KP_ENTER			= KD_INPUT_KEY_KP_ENTER				,	
			KEY_KP_HOME				= KD_INPUT_KEY_KP_HOME				,	
			KEY_KP_UP				= KD_INPUT_KEY_KP_UP				,	
			KEY_KP_PG_UP			= KD_INPUT_KEY_KP_PG_UP				,	
			KEY_KP_LEFT				= KD_INPUT_KEY_KP_LEFT				,	
			KEY_KP_FIVE				= KD_INPUT_KEY_KP_FIVE				,	
			KEY_KP_RIGHT			= KD_INPUT_KEY_KP_RIGHT				,	
			KEY_KP_END				= KD_INPUT_KEY_KP_END				,	
			KEY_KP_DOWN				= KD_INPUT_KEY_KP_DOWN				,	
			KEY_KP_PG_DOWN			= KD_INPUT_KEY_KP_PG_DOWN			,	
			KEY_KP_INSERT			= KD_INPUT_KEY_KP_INSERT			,	
			KEY_KP_DELETE			= KD_INPUT_KEY_KP_DELETE			,	
			KEY_F1					= KD_INPUT_KEY_F1					,	
			KEY_F2					= KD_INPUT_KEY_F2					,	
			KEY_F3					= KD_INPUT_KEY_F3					,	
			KEY_F4					= KD_INPUT_KEY_F4					,	
			KEY_F5					= KD_INPUT_KEY_F5					,	
			KEY_F6					= KD_INPUT_KEY_F6					,	
			KEY_F7					= KD_INPUT_KEY_F7					,		
			KEY_F8					= KD_INPUT_KEY_F8					,	
			KEY_F9					= KD_INPUT_KEY_F9					,	
			KEY_F10					= KD_INPUT_KEY_F10					,	
			KEY_F11					= KD_INPUT_KEY_F11					,	
			KEY_F12					= KD_INPUT_KEY_F12					,	
			KEY_SPACE				= KD_INPUT_KEY_SPACE				,	
			KEY_EXCLAM				= KD_INPUT_KEY_EXCLAM				,	
			KEY_QUOTE				= KD_INPUT_KEY_QUOTE				,	
			KEY_NUMBER				= KD_INPUT_KEY_NUMBER				,	
			KEY_DOLLAR				= KD_INPUT_KEY_DOLLAR				,	
			KEY_PERCENT				= KD_INPUT_KEY_PERCENT				,	
			KEY_CIRCUMFLEX			= KD_INPUT_KEY_CIRCUMFLEX			,	
			KEY_AMPERSAND			= KD_INPUT_KEY_AMPERSAND			,	
			KEY_APOSTROPHE			= KD_INPUT_KEY_APOSTROPHE			,	
			KEY_LEFT_PARENTHESIS	= KD_INPUT_KEY_LEFT_PARENTHESIS		,	
			KEY_RIGHT_PARENTHESIS	= KD_INPUT_KEY_RIGHT_PARENTHESIS	,	
			KEY_ASTERISK			= KD_INPUT_KEY_ASTERISK				,
			KEY_PLUS				= KD_INPUT_KEY_PLUS					,
			KEY_COMMA				= KD_INPUT_KEY_COMMA				,
			KEY_MINUS				= KD_INPUT_KEY_MINUS				,
			KEY_PERIOD				= KD_INPUT_KEY_PERIOD				,
			KEY_SLASH				= KD_INPUT_KEY_SLASH				,
			KEY_0					= KD_INPUT_KEY_0					,
			KEY_1					= KD_INPUT_KEY_1					,
			KEY_2					= KD_INPUT_KEY_2					,
			KEY_3					= KD_INPUT_KEY_3					,
			KEY_4					= KD_INPUT_KEY_4					,
			KEY_5					= KD_INPUT_KEY_5					,
			KEY_6					= KD_INPUT_KEY_6					,
			KEY_7					= KD_INPUT_KEY_7					,
			KEY_8					= KD_INPUT_KEY_8					,
			KEY_9					= KD_INPUT_KEY_9					,
			KEY_COLON				= KD_INPUT_KEY_COLON				,
			KEY_SEMICOLON			= KD_INPUT_KEY_SEMICOLON			,
			KEY_LESS_THAN			= KD_INPUT_KEY_LESS_THAN			,
			KEY_EQUAL				= KD_INPUT_KEY_EQUAL				,
			KEY_GREATER_THAN		= KD_INPUT_KEY_GREATER_THAN			,
			KEY_QUESTION			= KD_INPUT_KEY_QUESTION				,
			KEY_AT					= KD_INPUT_KEY_AT					,
			KEY_CAPITAL_A			= KD_INPUT_KEY_CAPITAL_A			,
			KEY_CAPITAL_B			= KD_INPUT_KEY_CAPITAL_B			,
			KEY_CAPITAL_C			= KD_INPUT_KEY_CAPITAL_C			,
			KEY_CAPITAL_D			= KD_INPUT_KEY_CAPITAL_D			,
			KEY_CAPITAL_E			= KD_INPUT_KEY_CAPITAL_E			,
			KEY_CAPITAL_F			= KD_INPUT_KEY_CAPITAL_F			,
			KEY_CAPITAL_G			= KD_INPUT_KEY_CAPITAL_G			,
			KEY_CAPITAL_H			= KD_INPUT_KEY_CAPITAL_H			,
			KEY_CAPITAL_I			= KD_INPUT_KEY_CAPITAL_I			,
			KEY_CAPITAL_J			= KD_INPUT_KEY_CAPITAL_J			,
			KEY_CAPITAL_K			= KD_INPUT_KEY_CAPITAL_K			,
			KEY_CAPITAL_L			= KD_INPUT_KEY_CAPITAL_L			,
			KEY_CAPITAL_M			= KD_INPUT_KEY_CAPITAL_M			,
			KEY_CAPITAL_N			= KD_INPUT_KEY_CAPITAL_N			,
			KEY_CAPITAL_O			= KD_INPUT_KEY_CAPITAL_O			,
			KEY_CAPITAL_P			= KD_INPUT_KEY_CAPITAL_P			,
			KEY_CAPITAL_Q			= KD_INPUT_KEY_CAPITAL_Q			,
			KEY_CAPITAL_R			= KD_INPUT_KEY_CAPITAL_R			,
			KEY_CAPITAL_S			= KD_INPUT_KEY_CAPITAL_S			,
			KEY_CAPITAL_T			= KD_INPUT_KEY_CAPITAL_T			,
			KEY_CAPITAL_U			= KD_INPUT_KEY_CAPITAL_U			,
			KEY_CAPITAL_V			= KD_INPUT_KEY_CAPITAL_V			,
			KEY_CAPITAL_W			= KD_INPUT_KEY_CAPITAL_W			,
			KEY_CAPITAL_X			= KD_INPUT_KEY_CAPITAL_X			,
			KEY_CAPITAL_Y			= KD_INPUT_KEY_CAPITAL_Y			,
			KEY_CAPITAL_Z			= KD_INPUT_KEY_CAPITAL_Z			,
			KEY_LEFT_BRACKET		= KD_INPUT_KEY_LEFT_BRACKET			,
			KEY_BACK_SLASH			= KD_INPUT_KEY_BACK_SLASH			,
			KEY_RIGHT_BRACKET		= KD_INPUT_KEY_RIGHT_BRACKET		,
			KEY_UNDERSCORE			= KD_INPUT_KEY_UNDERSCORE			,
			KEY_GRAVE				= KD_INPUT_KEY_GRAVE				,
			KEY_A					= KD_INPUT_KEY_A					,
			KEY_B					= KD_INPUT_KEY_B					,
			KEY_C					= KD_INPUT_KEY_C					,
			KEY_D					= KD_INPUT_KEY_D					,
			KEY_E					= KD_INPUT_KEY_E					,
			KEY_F					= KD_INPUT_KEY_F					,
			KEY_G					= KD_INPUT_KEY_G					,
			KEY_H					= KD_INPUT_KEY_H					,
			KEY_I					= KD_INPUT_KEY_I					,
			KEY_J					= KD_INPUT_KEY_J					,
			KEY_K					= KD_INPUT_KEY_K					,
			KEY_L					= KD_INPUT_KEY_L					,
			KEY_M					= KD_INPUT_KEY_M					,
			KEY_N					= KD_INPUT_KEY_N					,
			KEY_O					= KD_INPUT_KEY_O					,
			KEY_P					= KD_INPUT_KEY_P					,
			KEY_Q					= KD_INPUT_KEY_Q					,
			KEY_R					= KD_INPUT_KEY_R					,
			KEY_S					= KD_INPUT_KEY_S					,
			KEY_T					= KD_INPUT_KEY_T					,
			KEY_U					= KD_INPUT_KEY_U					,
			KEY_V					= KD_INPUT_KEY_V					,
			KEY_W					= KD_INPUT_KEY_W					,
			KEY_X					= KD_INPUT_KEY_X					,
			KEY_Y					= KD_INPUT_KEY_Y					,
			KEY_Z					= KD_INPUT_KEY_Z					,
			KEY_LEFT_BRACE			= KD_INPUT_KEY_LEFT_BRACE			,
			KEY_BAR					= KD_INPUT_KEY_BAR					,
			KEY_RIGHT_BRACE			= KD_INPUT_KEY_RIGHT_BRACE			,
			KEY_TILDE				= KD_INPUT_KEY_TILDE				,
			KEY_EURO				= KD_INPUT_KEY_EURO					,
			KEY_POUND				= KD_INPUT_KEY_POUND				,
			KEY_YEN					= KD_INPUT_KEY_YEN					,
			KEY_MIDDLE_DOT			= KD_INPUT_KEY_MIDDLE_DOT			,
			KEY_SEARCH				= KD_INPUT_KEY_SEARCH				,
		};
    
		EventKeyboard ( KeyCode eKeyCode, KDbool bIsPressed );
    
	private :

		KeyCode						m_eKeyCode;
		KDbool						m_bIsPressed;
    
		friend class EventListenerKeyboard;
};

NS_CC_END

#endif	// __CCEventKeyboard_h__
