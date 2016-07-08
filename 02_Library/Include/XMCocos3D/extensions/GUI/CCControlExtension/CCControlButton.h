/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlButton.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2011      Yannick Loriot.
 *
 *          http://www.cocos2d-x.org
 *          http://yannickloriot.com
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

#ifndef __CCControlButton_h__
#define __CCControlButton_h__

#include "CCControl.h"
#include "CCInvocation.h"
#include "CCScale9Sprite.h"

NS_CC_EXT_BEGIN

/* Define the button margin for Left/Right edge */
#define ControlButtonMarginLR			8 // px
/* Define the button margin for Top/Bottom edge */
#define ControlButtonMarginTB			2 // px


/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */

/** @class ControlButton Button control for Cocos2D. */
class ControlButton : public Control
{        
	public :

		static ControlButton*	create ( KDvoid );
		static ControlButton*	create ( Scale9Sprite* pSprite );
		static ControlButton*	create ( Node* pLabel, Scale9Sprite* pBackgroundSprite );
		static ControlButton*	create ( std::string sTitle, const KDchar* pFontName, KDfloat fFontSize );
		
		/**
		 *	@js ctor
		 */
		ControlButton ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ControlButton ( KDvoid );

		virtual KDbool			init ( KDvoid );
		virtual KDbool			initWithLabelAndBackgroundSprite ( Node* pLabel, Scale9Sprite* pBackgroundSprite );
		virtual KDbool			initWithBackgroundSprite ( Scale9Sprite* pSprite );
		virtual KDbool			initWithTitleAndFontNameAndFontSize ( std::string sTitle, const KDchar* pFontName, KDfloat fFontSize );

		virtual KDvoid			needsLayout ( KDvoid );

		virtual KDvoid			setEnabled ( KDbool bEnabled );
		virtual KDvoid			setSelected ( KDbool bEnabled );
		virtual KDvoid			setHighlighted ( KDbool bEnabled );

		KDbool					isPushed ( KDvoid ) const { return m_bIsPushed; }

		/**
		 *	Returns the title used for a state.
		 *
		 *	@param state The state that uses the title. Possible values are described in
		 *	"CCControlState".
		 *
		 *	@return The title for the specified state.
		 */
		virtual String*			getTitleForState ( State eState );

		/**
		 *	Sets the title string to use for the specified state.
		 *	If a property is not specified for a state, the default is to use
		 *	the ButtonStateNormal value.
		 *
		 *	@param title The title string to use for the specified state.
		 *	@param state The state that uses the specified title. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setTitleForState ( String* pTitle, State eState );

		/**
		 *	Returns the title color used for a state.
		 *
		 *	@param state The state that uses the specified color. The values are described
		 *	in "CCControlState".
		 *
		 *	@return The color of the title for the specified state.
		 */

		virtual Color3B			getTitleColorForState ( State eState ) const;

		/**
		 *	Sets the color of the title to use for the specified state.
		 *
		 *	@param color The color of the title to use for the specified state.
		 *	@param state The state that uses the specified color. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setTitleColorForState ( Color3B tColor, State eState );

		/**
		 *	Returns the title label used for a state.
		 *
		 *	@param state The state that uses the title label. Possible values are described
		 *	in "CCControlState".
		 */
		virtual Node*			getTitleLabelForState ( State eState );

		/**
		 *	Sets the title label to use for the specified state.
		 *	If a property is not specified for a state, the default is to use
		 *	the ButtonStateNormal value.
		 *
		 *	@param label The title label to use for the specified state.
		 *	@param state The state that uses the specified title. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setTitleLabelForState ( Node* pLabel, State eState );

		virtual KDvoid			setTitleTTFForState ( const KDchar* pFntFile, State eState );
		virtual const KDchar*	getTitleTTFForState ( State eState );

		virtual KDvoid			setTitleTTFSizeForState ( KDfloat fSize, State eState );
		virtual KDfloat			getTitleTTFSizeForState ( State eState );

		/**
		 *	Sets the font of the label, changes the label to a LabelBMFont if neccessary.
		 *	@param fntFile The name of the font to change to
		 *	@param state The state that uses the specified fntFile. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setTitleBMFontForState ( const KDchar* pFntFile, State eState );
		virtual const KDchar*	getTitleBMFontForState ( State state );

		/**
		 *	Returns the background sprite used for a state.
		 *
		 *	@param state The state that uses the background sprite. Possible values are
		 *	described in "CCControlState".
		 */
		virtual Scale9Sprite*	getBackgroundSpriteForState ( State eState );

		/**
		 *	Sets the background sprite to use for the specified button state.
		 *
		 *	@param sprite The background sprite to use for the specified state.
		 *	@param state The state that uses the specified image. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setBackgroundSpriteForState ( Scale9Sprite* pSprite, State eState );

		/**
		 *	Sets the background spriteFrame to use for the specified button state.
		 *
		 *	@param spriteFrame The background spriteFrame to use for the specified state.
		 *	@param state The state that uses the specified image. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setBackgroundSpriteFrameForState ( SpriteFrame* pSpriteFrame, State eState );

		// set the margins at once (so we only have to do one call of needsLayout)
		virtual KDvoid			setMargins ( KDint nMarginH, KDint nMarginV );

		virtual KDbool			onTouchBegan ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchMoved ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchEnded ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchCancelled ( Touch* pTouch, Event* pEvent ) override;
    
		// Overrides
		virtual GLubyte			getOpacity ( KDvoid ) const override;
		virtual KDvoid			setOpacity ( GLubyte cVar ) override;
		virtual const Color3B&	getColor ( KDvoid ) const override;
		virtual KDvoid			setColor ( const Color3B& ) override;

		/** 
		 *	Adjust the background image. YES by default. If the property is set to NO, the 
		 *	background will use the prefered size of the background image. 
		 */
		KDbool					doesAdjustBackgroundImage ( KDvoid );
		KDvoid					setAdjustBackgroundImage ( KDbool bAdjustBackgroundImage );

	protected :

		KDbool					m_bIsPushed;
		KDbool					m_bParentInited;
		KDbool					m_bDoesAdjustBackgroundImage;

		/** The current title that is displayed on the button. */
		CC_SYNTHESIZE_READONLY ( String*, m_pCurrentTitle, CurrentTitle );

		/** The current color used to display the title. */
		CC_SYNTHESIZE_READONLY_PASS_BY_REF ( Color3B, m_tCurrentTitleColor, CurrentTitleColor );

		/** The current title label. */
		CC_SYNTHESIZE_RETAIN ( Node*, m_pTitleLabel, TitleLabel );

		/** The current background sprite. */
		CC_SYNTHESIZE_RETAIN ( Scale9Sprite*, m_pBackgroundSprite, BackgroundSprite );

		/** The prefered size of the button, if label is larger it will be expanded. */
		CC_PROPERTY ( Size, m_tPreferredSize, PreferredSize );

		/** Adjust the button zooming on touchdown. Default value is YES. */
		CC_PROPERTY ( KDbool, m_bZoomOnTouchDown, ZoomOnTouchDown );

		CC_PROPERTY ( Point, m_tLabelAnchorPoint, LabelAnchorPoint );

		// <ControlState, String*>
		CC_SYNTHESIZE_RETAIN ( Dictionary*, m_pTitleDispatchTable, TitleDispatchTable );
		// <ControlState, Color3bObject*>
		CC_SYNTHESIZE_RETAIN ( Dictionary*, m_pTitleColorDispatchTable, TitleColorDispatchTable );
		// <ControlState, Node*>
		CC_SYNTHESIZE_RETAIN ( Dictionary*, m_pTitleLabelDispatchTable, TitleLabelDispatchTable );
		// <ControlState, Scale9Sprite*>
		CC_SYNTHESIZE_RETAIN ( Dictionary*, m_pBackgroundSpriteDispatchTable, BackgroundSpriteDispatchTable );

		/* Define the button margin for Top/Bottom edge */
		CC_SYNTHESIZE_READONLY ( KDint, m_nMarginV, VerticalMargin );
		/* Define the button margin for Left/Right edge */
		CC_SYNTHESIZE_READONLY ( KDint, m_nMarginH, HorizontalOrigin );
};

//	end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif	// __CCControlButton_h__
