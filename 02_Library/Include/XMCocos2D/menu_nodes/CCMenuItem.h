/* -----------------------------------------------------------------------------------
 *
 *      File            CCMenuItem.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

#ifndef __CCMenuItem_h__
#define __CCMenuItem_h__

#include "../CCProtocols.h"
#include "../base_nodes/CCNode.h"
#include "../cocoa/CCArray.h"

NS_CC_BEGIN
    
/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */

class CCLabelTTF;
class CCLabelAtlas;
class CCSprite;
class CCSpriteFrame;

#define kCCItemSize       32

/** 
 *	@brief	CCMenuItem base class
 *
 *  Subclass CCMenuItem (or any subclass) to create your custom CCMenuItem objects.
 */
class CCMenuItem : public CCNodeRGBA
{
	public :
			     CCMenuItem ( KDvoid );
		virtual ~CCMenuItem ( KDvoid );

		/** Creates a CCMenuItem with no target/selector */
		static   CCMenuItem*	create ( KDvoid );

		/** Creates a CCMenuItem with a target/selector */
		static   CCMenuItem*	create ( CCObject* pListener, SEL_MenuHandler pSelector );

	public :

		/** Initializes a CCMenuItem with a target/selector */
		virtual KDbool			initWithTarget ( CCObject* pListener, SEL_MenuHandler pSelector );

		/** Returns the outside box */
		CCRect					rect ( KDvoid );

		/** Activate the item */
		virtual KDvoid			activate ( KDvoid );

		/** The item was selected (not activated), similar to "mouse-over" */
		virtual KDvoid			selected ( KDvoid );

		/** The item was unselected */
		virtual KDvoid			unselected ( KDvoid );

		/** Register menu handler script function */
		virtual KDvoid			  registerScriptTapHandler ( KDint nHandler );
        virtual KDvoid			unregisterScriptTapHandler ( KDvoid );
		inline  KDint			getScriptTapHandler ( KDvoid ) { return m_nScriptTapHandler; };

		virtual KDbool			 isEnabled ( KDvoid );
		virtual KDvoid			setEnabled ( KDbool bEnable );
		virtual KDbool			isSelected ( KDvoid );

		/** set the target/selector of the menu item */
        KDvoid					setTarget ( CCObject* pListener, SEL_MenuHandler pSelector );

	protected :

		KDbool					m_bSelected;
		KDbool					m_bEnabled;
		CCObject*				m_pListener;
		SEL_MenuHandler			m_pSelector;
		KDint					m_nScriptTapHandler;
};

/** 
 *	@brief An abstract class for "label" CCMenuItemLabel items 
 *	Any CCNode that supports the CCLabelProtocol protocol can be added.
 *	Supported nodes:
 *		- CCBitmapFontAtlas
 *		- CCLabelAtlas
 *		- CCLabelTTF
 */
class CCMenuItemLabel : public CCMenuItem
{
	public :
				 CCMenuItemLabel ( KDvoid );
		virtual ~CCMenuItemLabel ( KDvoid );

		/** creates a CCMenuItemLabel with a Label. Target and selector will be null */
		static   CCMenuItemLabel*	create ( CCNode* pLabel );

		/** creates a CCMenuItemLabel with a Label, target and selector */
		static   CCMenuItemLabel*	create ( CCNode* pLabel, CCObject* pListener, SEL_MenuHandler pSelector );

	public :

		/** creates a CCMenuItemLabel with a Label, target and selector */
		virtual KDbool				initWithLabel ( CCNode* pLabel, CCObject* pListener, SEL_MenuHandler pSelector );

		/** sets a new string to the inner label */
		KDvoid						setString ( const KDchar* szText );

		// super methods
		virtual KDvoid				activate ( KDvoid );
		virtual KDvoid				selected ( KDvoid );
		virtual KDvoid				unselected ( KDvoid );

		/**
		 *	Enable or disabled the CCMenuItemFont
		 *	@warning setEnabled changes the RGB color of the font
		 */
		virtual KDvoid				setEnabled ( KDbool bEnabled );

		/** the color that will be used to disable the item */
		CC_PROPERTY_PASS_BY_REF ( ccColor3B, m_tDisabledColor, DisabledColor );

		/** Label that is rendered. It can be any CCNode that implements the CCLabelProtocol */
		CC_PROPERTY ( CCNode*, m_pLabel, Label );

	protected :

		ccColor3B					m_tColorBackup;
		KDfloat						m_fOriginalScale;
};

/** 
 *	@brief A CCMenuItemAtlasFont
 *	Helper class that creates a MenuItemLabel class with a LabelAtlas
 */
class CCMenuItemAtlasFont : public CCMenuItemLabel
{
	public :

		/** creates a menu item from a string and atlas with a target/selector */
		static   CCMenuItemAtlasFont*	create ( const KDchar* szText, const KDchar* szAtlasPath, KDint uTileWidth, KDint uTileHeight, KDubyte cStartChar );

		/** creates a menu item from a string and atlas. Use it with MenuItemToggle */
		static   CCMenuItemAtlasFont*	create ( const KDchar* szText, const KDchar* szAtlasPath, KDint uTileWidth, KDint uTileHeight, KDubyte cStartChar, CCObject* pListener, SEL_MenuHandler pSelector );

	public :

		/** initializes a menu item from a string and atlas with a target/selector */
		virtual  KDbool				initWithString ( const KDchar* szText, const KDchar* szAtlasPath, KDint uTileWidth, KDint uTileHeight, KDubyte cStartChar, CCObject* pListener, SEL_MenuHandler pSelector );
};

/**
 *	@brief	A CCMenuItemFont
 *	Helper class that creates a CCMenuItemLabel class with a Label
 */
class CCMenuItemFont : public CCMenuItemLabel
{
	public :

		/** creates a menu item from a string without target/selector. To be used with CCMenuItemToggle */		
		static   CCMenuItemFont*	create ( const KDchar* szText );

		/** creates a menu item from a string with a target/selector */
		static   CCMenuItemFont*	create ( const KDchar* szText, CCObject* pListener, SEL_MenuHandler pSelector );

	public :

		/** initializes a menu item from a string with a target/selector */
		virtual KDbool				initWithString ( const KDchar* szText, CCObject* pListener, SEL_MenuHandler pSelector );

		/** set default font size */
		static  KDvoid				setFontSize ( KDuint uFontSize );

		/** get default font size */
		static  KDuint				getFontSize ( KDvoid );

		/** set the default font name */
		static  KDvoid				setFontName ( const KDchar* szFontPath );

		/** get the default font name */
		static  const KDchar*		getFontName ( KDvoid );

		/** 
		 *	set font size
		 *	c++ can not overload static and non-static member functions with the same parameter types
		 *	so change the name to setFontSizeObj
		 */
		KDvoid						setFontSizeObj ( KDuint uFontSize );

		/** get font size */
		KDuint						fontSizeObj ( KDvoid );

		/**
		 *	set the font name 
		 *	c++ can not overload static and non-static member functions with the same parameter types
		 *	so change the name to setFontNameObj
		 */
		KDvoid						setFontNameObj ( const KDchar* szFontName );

		const KDchar*				fontNameObj ( KDvoid );

	protected :

		KDvoid						recreateLabel ( KDvoid );

	protected :

		KDuint						m_uFontSize;
		std::string					m_sFontName;
};

/** 
 *	@brief CCMenuItemSprite accepts CCNode<CCRGBAProtocol> objects as items.
 *	The images has 3 different states:
 *		- unselected image
 *		- selected image
 *		- disabled image
 *
 *	@since v0.8.0
 */
class CCMenuItemSprite : public CCMenuItem
{
	public :

		CCMenuItemSprite ( KDvoid );

		/** creates a menu item with a normal and selected image*/
		static   CCMenuItemSprite*	create ( CCNode* pNormalSprite, CCNode* pSelectedSprite );

		/** creates a menu item with a normal and selected image with target/selector */
		static   CCMenuItemSprite*  create ( CCNode* pNormalSprite, CCNode* pSelectedSprite, CCObject* pListener, SEL_MenuHandler pSelector );

		/** creates a menu item with a normal,selected  and disabled image with target/selector */
		static   CCMenuItemSprite*  create ( CCNode* pNormalSprite, CCNode* pSelectedSprite, CCNode* pDisabledSprite, CCObject* pListener, SEL_MenuHandler pSelector );

	public :

		/** initializes a menu item with a normal, selected  and disabled image with target/selector */
		virtual KDbool				initWithNormalSprite ( CCNode* pNormalSprite, CCNode* pSelectedSprite, CCNode* pDisabledSprite, CCObject* pListener, SEL_MenuHandler pSelector );

		/**
         *	@since	v0.99.5
         */
        virtual KDvoid				  selected ( KDvoid );
        virtual KDvoid				unselected ( KDvoid );
        virtual KDvoid				setEnabled ( KDbool bEnabled );

		/** the image used when the item is not selected */
		CC_PROPERTY ( CCNode*, m_pNormalImage  , NormalImage   );

		/** the image used when the item is selected */
		CC_PROPERTY ( CCNode*, m_pSelectedImage, SelectedImage );

		/** the image used when the item is disabled */
		CC_PROPERTY ( CCNode*, m_pDisabledImage, DisabledImage );

	protected :

		virtual KDvoid				updateImagesVisibility ( KDvoid );
};

/** 
 *	@brief	CCMenuItemImage accepts images as items.
 *	The images has 3 different states:
 *		- unselected image
 *		- selected image
 *		- disabled image
 *
 *	For best results try that all images are of the same size
 */
class CCMenuItemImage : public CCMenuItemSprite
{
	public :

		/** creates a menu item with a normal and selected image*/
		static   CCMenuItemImage*	create ( const KDchar* szNormalImage, const KDchar* szSelectedImage );

		/** creates a menu item with a normal,selected  and disabled image*/
		static   CCMenuItemImage*	create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, const KDchar* szDisabledImage );
		
		/** creates a menu item with a normal and selected image with target/selector */
		static   CCMenuItemImage*	create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, CCObject* pListener, SEL_MenuHandler pSelector );

		/** creates a menu item with a normal,selected  and disabled image with target/selector */
		static   CCMenuItemImage*	create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, const KDchar* szDisabledImage, CCObject* pListener, SEL_MenuHandler pSelector );

		/** Creates an CCMenuItemImage. */
		static   CCMenuItemImage*	create ( KDvoid );

	public :

		virtual KDbool				init ( KDvoid );

		/** initializes a menu item with a normal, selected  and disabled image with target/selector */
		virtual KDbool				initWithNormalImage ( const KDchar* szNormalImage, const KDchar* szSelectedImage, const KDchar* szDisabledImage, CCObject* pListener, SEL_MenuHandler pSelector );

		/** sets the sprite frame for the normal image */
		KDvoid						setNormalSpriteFrame   ( CCSpriteFrame* pFrame );

		/** sets the sprite frame for the selected image */
		KDvoid						setSelectedSpriteFrame ( CCSpriteFrame* pFrame );

		/** sets the sprite frame for the disabled image */
		KDvoid						setDisabledSpriteFrame ( CCSpriteFrame* pFrame );
};

/** 
 *	@brief	A CCMenuItemToggle
 *	A simple container class that "toggles" it's inner items
 *	The inner items can be any MenuItem
 */
class CCMenuItemToggle : public CCMenuItem
{
	public :
		         CCMenuItemToggle ( KDvoid );
		virtual ~CCMenuItemToggle ( KDvoid );

		/** creates a menu item from a CCArray with a target selector */
		static   CCMenuItemToggle*	createWithTarget ( CCObject* pListener, SEL_MenuHandler pSelector, CCArray* pMenuItems );

		/** creates a menu item from a list of items with a target/selector */
		static   CCMenuItemToggle*	createWithTarget ( CCObject* pListener, SEL_MenuHandler pSelector, CCMenuItem* pItem, ... );

		/** creates a menu item with no target/selector and no items */
		static   CCMenuItemToggle*  create ( KDvoid );

		// The follow methods offered to lua
		/** creates a menu item with a item */
		static   CCMenuItemToggle*  create ( CCMenuItem* pItem );

	public :

		/** initializes a menu item from a list of items with a target selector */
		virtual KDbool				initWithTarget ( CCObject* pListener, SEL_MenuHandler pSelector, CCMenuItem* pItem, KDVaListKHR pArgs );

		/** initializes a menu item with a item */
		virtual KDbool				initWithItem ( CCMenuItem* pItem );

		/** add more menu item */
		KDvoid						addSubItem ( CCMenuItem* pItem );

		/** return the selected item */
		CCMenuItem*					selectedItem ( KDvoid );

		// super methods
		virtual KDvoid				activate ( KDvoid );
		virtual KDvoid				selected ( KDvoid );
		virtual KDvoid				unselected ( KDvoid );
		virtual KDvoid				setEnabled ( KDbool bEnabled );

		/** returns the selected item */
		CC_PROPERTY ( KDuint, m_uSelectedIndex, SelectedIndex );

		/**
		 *	CCMutableArray that contains the subitems. You can add/remove items in runtime, and you can replace the array with a new one.
		 *	@since	v0.7.2
		 */
		CC_PROPERTY ( CCArray*, m_pSubItems, SubItems );
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCMenuItem_h__