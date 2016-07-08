/* --------------------------------------------------------------------------
 *
 *      File            CCControlButton.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      Angus C        ( Converted to c++ )
 *
 *         http://www.cocos2d-x.org   
 *         http://creativewax.co.uk
 *         http://yannickloriot.com
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

#ifndef __CCControlButton_h__
#define __CCControlButton_h__

#include "CCControl.h"
#include "CCScale9Sprite.h"

NS_CC_BEGIN

/* Define the button margin for Left/Right edge */
#define CCControlButtonMarginLR		8 // px

/* Define the button margin for Top/Bottom edge */
#define CCControlButtonMarginTB		2 // px

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

/** @class CCControlButton Button control for Cocos2D. */
class CCControlButton : public CCControl
{		
	public :
				 CCControlButton ( KDvoid );
		virtual ~CCControlButton ( KDvoid );

		static  CCControlButton*	create ( CCNode* pLabel, CCScale9Sprite* pBackgroundSprite );	

		static  CCControlButton*	create ( std::string sTitle, const KDchar* szFontName, KDfloat fFontSize );

		static  CCControlButton*	create ( CCScale9Sprite* pSprite );

		static  CCControlButton*	create ( KDvoid );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDbool			initWithLabelAndBackgroundSprite ( CCNode* pLabel, CCScale9Sprite* pBackgroundSprite );
		
		virtual KDbool			initWithTitleAndFontNameAndFontSize ( std::string sTitle, const KDchar* szFontName, KDfloat fFontSize );
		
		virtual KDbool			initWithBackgroundSprite ( CCScale9Sprite* pSprite );

		virtual KDvoid			needsLayout ( KDvoid );
	
		virtual KDvoid			setEnabled ( KDbool bEnabled );
		virtual KDvoid			setSelected ( KDbool bEnabled );
		virtual KDvoid			setHighlighted ( KDbool bEnabled );

	public :
		
		// events
		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		/**
		 *	Returns the title used for a state.
		 *
		 *	@param state The state that uses the title. Possible values are described in
		 *	"CCControlState".
		 *
		 *	@return The title for the specified state.
		 */
		virtual CCString*		getTitleForState ( CCControlState uState );

		/**
		 *	Sets the title string to use for the specified state.
		 *	If a property is not specified for a state, the default is to use
		 *	the CCButtonStateNormal value.
		 *
		 *	@param title The title string to use for the specified state.
		 *	@param state The state that uses the specified title. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setTitleForState ( CCString* pTitle, CCControlState uState );

		/**
		 *	Returns the title color used for a state.
		 *
		 *	@param state The state that uses the specified color. The values are described
		 *	in "CCControlState".
		 *
		 *	@return The color of the title for the specified state.
		 */
		virtual const ccColor3B	getTitleColorForState ( CCControlState uState );

		/**
		 *	Sets the color of the title to use for the specified state.
		 *
		 *	@param color The color of the title to use for the specified state.
		 *	@param state The state that uses the specified color. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setTitleColorForState ( ccColor3B tColor, CCControlState uState );

		/**
		 *	Returns the title label used for a state.
		 *
		 *	@param state The state that uses the title label. Possible values are described
		 *	in "CCControlState".
		 */
		virtual CCNode*			getTitleLabelForState ( CCControlState uState );

		/**
		 *	Sets the title label to use for the specified state.
		 *	If a property is not specified for a state, the default is to use
		 *	the CCButtonStateNormal value.
		 *
		 *	@param title The title label to use for the specified state.
		 *	@param state The state that uses the specified title. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setTitleLabelForState ( CCNode* pLabel, CCControlState uState );

		virtual KDvoid			setTitleTTFForState ( const KDchar* szFntFile, CCControlState uState );
		virtual const KDchar*	getTitleTTFForState ( CCControlState uState );

		virtual KDvoid			setTitleTTFSizeForState ( KDfloat fSize, CCControlState uState );
		virtual KDfloat			getTitleTTFSizeForState ( CCControlState uState );

		/**
		 *	Sets the font of the label, changes the label to a CCLabelBMFont if neccessary.
		 *	@param fntFile The name of the font to change to
		 *	@param state The state that uses the specified fntFile. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setTitleBMFontForState ( const KDchar* szFntFile, CCControlState uState );
		virtual const KDchar*	getTitleBMFontForState ( CCControlState uState );

		/**
		 *	Returns the background sprite used for a state.
		 *
		 *	@param state The state that uses the background sprite. Possible values are
		 *	described in "CCControlState".
		 */
		virtual CCScale9Sprite*	getBackgroundSpriteForState ( CCControlState uState );

		/**
		 *	Sets the background sprite to use for the specified button state.
		 *
		 *	@param sprite The background sprite to use for the specified state.
		 *	@param state The state that uses the specified image. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setBackgroundSpriteForState ( CCScale9Sprite* pSprite, CCControlState uState );

		/**
		 *	Sets the background spriteFrame to use for the specified button state.
		 *
		 *	@param spriteFrame The background spriteFrame to use for the specified state.
		 *	@param state The state that uses the specified image. The values are described
		 *	in "CCControlState".
		 */
		virtual KDvoid			setBackgroundSpriteFrameForState ( CCSpriteFrame* pSpriteFrame, CCControlState uState );

		/** 
		 *	Adjust the background image. YES by default. If the property is set to NO, the 
		 *	background will use the prefered size of the background image.
		 */
		KDbool					doesAdjustBackgroundImage ( KDvoid );
		KDvoid					 setAdjustBackgroundImage ( KDbool bAdjustBackgroundImage );

		/* Override setter to affect a background sprite too */
		virtual GLubyte			getOpacity ( KDvoid );
		virtual KDvoid			setOpacity ( GLubyte cOpacity );

		/** Flag to know if the button is currently pushed.  */
		inline  KDbool			isPushed ( KDvoid ) { return m_bIsPushed; }

		// set the margins at once (so we only have to do one call of needsLayout)
		virtual KDvoid			setMargins ( KDint nMarginH, KDint nMarginV );

	protected :

		// CCRGBAProtocol

		/** The current title that is displayed on the button. */
		CC_SYNTHESIZE_READONLY ( CCString*, m_pCurrentTitle, CurrentTitle );

		/** The current color used to display the title. */
		CC_SYNTHESIZE_READONLY_PASS_BY_REF ( ccColor3B, m_tCurrentTitleColor, CurrentTitleColor );

		/** The current title label. */
		CC_SYNTHESIZE_RETAIN ( CCNode*, m_pTitleLabel, TitleLabel );

		/** The current background sprite. */
		CC_SYNTHESIZE_RETAIN ( CCScale9Sprite*, m_pBackgroundSprite, BackgroundSprite );
		
	    /** The prefered size of the button, if label is larger it will be expanded. */
		CC_PROPERTY ( CCSize, m_tPreferredSize, PreferredSize );

		/** Adjust the button zooming on touchdown. Default value is YES. */
		CC_PROPERTY ( KDbool, m_bZoomOnTouchDown, ZoomOnTouchDown );

		CC_PROPERTY ( CCPoint, m_tLabelAnchorPoint, LabelAnchorPoint );

		// <CCControlState, CCString*>
		CC_SYNTHESIZE_RETAIN ( CCDictionary*, m_pTitleDispatchTable, TitleDispatchTable );
		// <CCControlState, CCColor3bObject*>
		CC_SYNTHESIZE_RETAIN ( CCDictionary*, m_pTitleColorDispatchTable, TitleColorDispatchTable );
		// <CCControlState, CCNode*>
		CC_SYNTHESIZE_RETAIN ( CCDictionary*, m_pTitleLabelDispatchTable, TitleLabelDispatchTable );
		// <CCControlState, CCScale9Sprite*>
		CC_SYNTHESIZE_RETAIN ( CCDictionary*, m_pBackgroundSpriteDispatchTable, BackgroundSpriteDispatchTable );

		/** Define the button margin for Top/Bottom edge */
		CC_SYNTHESIZE_READONLY ( KDint, m_nMarginV, VerticalMargin );

		/** Define the button margin for Left/Right edge */
		CC_SYNTHESIZE_READONLY ( KDint, m_nMarginH, HorizontalOrigin );

	protected :

		KDbool				m_bDoesAdjustBackgroundImage;
		KDbool				m_bIsPushed;
		KDbool				m_bParentInited;
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCControlButton_h__