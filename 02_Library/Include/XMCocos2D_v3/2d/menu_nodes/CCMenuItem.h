/* -----------------------------------------------------------------------------------
 *
 *      File            CCMenuItem.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCMenuItem_h__
#define __CCMenuItem_h__

// C++ includes
#include <functional>

// cocos2d includes
#include "../base_nodes/CCNode.h"
#include "../../base/CCArray.h"

NS_CC_BEGIN

typedef std::function<void(Object*)> ccMenuCallback;

class LabelTTF;
class LabelAtlas;
class Sprite;
class SpriteFrame;
#define kItemSize 32
    
/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup menu
 *	@{
 */

/** 
 *	@brief MenuItem base class
 *
 *  Subclass MenuItem (or any subclass) to create your custom MenuItem objects.
 */
class CC_DLL MenuItem : public NodeRGBA
{
	public :

		/** Creates a MenuItem with no target/selector */
		static MenuItem*			create ( KDvoid );

		/** Creates a MenuItem with a target/selector */
		static MenuItem*			create ( const ccMenuCallback& pCallback );
		
	public :

		/**
		 *	@js ctor
		 */
		MenuItem ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~MenuItem ( KDvoid );

		/**
		 *	Initializes a MenuItem with a target/selector 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool						initWithCallback ( const ccMenuCallback& pCallback );

		/** Returns the outside box */
		Rect						rect ( KDvoid ) const;

		/** Activate the item */
		virtual KDvoid				activate ( KDvoid );

		/** The item was selected (not activated), similar to "mouse-over" */
		virtual KDvoid				selected ( KDvoid );

		/** The item was unselected */
		virtual KDvoid				unselected ( KDvoid );
		
		/** returns whether or not the item is enabled */
		virtual KDbool				isEnabled ( KDvoid ) const;
		
		/** enables or disables the item */
		virtual KDvoid				setEnabled ( KDbool bValue );
		
		/** returns whether or not the item is selected */
		virtual KDbool				isSelected ( KDvoid ) const;

		/**
		 *	set the callback to the menu item
		 *	@code
		 *	In js,can contain two params,the second param is jsptr
		 *	@endcode
		 *	@lua NA
		 */
		KDvoid						setCallback ( const ccMenuCallback& pCallback );

		// Added By XMSoft
		KDvoid						setSoundActivate   ( const KDchar* szPath );
		KDvoid						setSoundSelected   ( const KDchar* szPath );
		KDvoid						setSoundUnselected ( const KDchar* szPath );

		static KDvoid				setDefaultSoundActivate   ( const KDchar* szPath );
		static KDvoid				setDefaultSoundSelected   ( const KDchar* szPath );
		static KDvoid				setDefaultSoundUnselected ( const KDchar* szPath );

	protected :

		KDbool						m_bSelected;
		KDbool						m_bEnabled;

		// callback
		ccMenuCallback				m_pCallback;

		// Added By XMSoft
		std::string					m_sSoundActivate;
		std::string					m_sSoundSelected;
		std::string					m_sSoundUnselected;
};

/** 
 *	@brief An abstract class for "label" MenuItemLabel items 
 *	Any Node that supports the LabelProtocol protocol can be added.
 *	Supported nodes:
 *		- BitmapFontAtlas
 *		- LabelAtlas
 *		- LabelTTF
 */
class CC_DLL MenuItemLabel : public MenuItem
{
	public :

		/** creates a MenuItemLabel with a Label and a callback */
		static MenuItemLabel*		create ( Node* pLabel, const ccMenuCallback& pCallback );

		/** creates a MenuItemLabel with a Label. Target and selector will be nil */
		static MenuItemLabel*		create ( Node* pLabel );

		/**
		 *	@js ctor
		 */
		MenuItemLabel ( KDvoid )
		: m_fOriginalScale ( 0.0 )
		, m_pLabel(NULL)
		{ }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~MenuItemLabel ( KDvoid );

		/** initializes a MenuItemLabel with a Label, target and selector */
		KDbool						initWithLabel ( Node* pLabel, const ccMenuCallback& pCallback );

		/** sets a new string to the inner label */
		KDvoid						setString ( const std::string& sLabel );

		/** Gets the color that will be used to disable the item */
		inline const Color3B&		getDisabledColor ( KDvoid ) const { return m_tDisabledColor; }

		/** Sets the color that will be used to disable the item */
		inline KDvoid				setDisabledColor ( const Color3B& tColor ) { m_tDisabledColor = tColor; }
    
		/** Gets the label that is rendered. */
		inline Node*				getLabel ( KDvoid ) const { return m_pLabel; }
    
		/** Sets the label that is rendered. */
		KDvoid						setLabel ( Node* pNode );
    
		// Overrides
		virtual KDvoid				activate ( KDvoid ) override;
		virtual KDvoid				selected ( KDvoid ) override;
		virtual KDvoid				unselected ( KDvoid ) override;
		virtual KDvoid				setEnabled ( KDbool bEnabled ) override;
    
	protected :

		Color3B						m_tColorBackup;
		KDfloat						m_fOriginalScale;

		/** the color that will be used to disable the item */
		Color3B						m_tDisabledColor;
		/** Label that is rendered. It can be any Node that implements the LabelProtocol */
		Node*						m_pLabel;
};


/** 
 *	@brief A MenuItemAtlasFont
 *	Helper class that creates a MenuItemLabel class with a LabelAtlas
 */
class CC_DLL MenuItemAtlasFont : public MenuItemLabel
{
	public :

		/** creates a menu item from a string and atlas with a target/selector */
		static MenuItemAtlasFont*	create ( const std::string& sValue, const std::string& sCharMapFile, KDint32 nItemWidth, KDint32 nItemHeight, KDchar cStartCharMap );

		/** creates a menu item from a string and atlas. Use it with MenuItemToggle */
		static MenuItemAtlasFont*	create ( const std::string& sValue, const std::string& sCharMapFile, KDint32 nItemWidth, KDint32 nItemHeight, KDchar cStartCharMap, const ccMenuCallback& pCallback );

		/**
		 *	@js ctor
		 */
		MenuItemAtlasFont ( KDvoid ) { }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~MenuItemAtlasFont ( KDvoid ) { }

		/** initializes a menu item from a string and atlas with a target/selector */
		KDbool						initWithString ( const std::string& sValue, const std::string& sCharMapFile, KDint32 nItemWidth, KDint32 nItemHeight, KDchar cStartCharMap, const ccMenuCallback& pCallback );
};


/** 
 *	@brief A MenuItemFont
 *	Helper class that creates a MenuItemLabel class with a Label
 */
class CC_DLL MenuItemFont : public MenuItemLabel
{
	public :

		/** creates a menu item from a string without target/selector. To be used with MenuItemToggle */
		static MenuItemFont*		create ( const std::string& sValue );

		/** creates a menu item from a string with a target/selector */
		static MenuItemFont*		create ( const std::string& sValue, const ccMenuCallback& pCallback );

		/**
		 *	@js ctor
		 */
		MenuItemFont ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~MenuItemFont ( KDvoid );

		/** initializes a menu item from a string with a target/selector */
		KDbool						initWithString ( const std::string& sValue, const ccMenuCallback& pCallback );

		/** set default font size */
		static KDvoid				setFontSize ( KDint32 nSize );

		/** get default font size */
		static KDint32				getFontSize ( KDvoid );

		/** set the default font name */
		static KDvoid				setFontName ( const std::string& sName );

		/** get the default font name */
		static const std::string&	getFontName ( KDvoid );

		/**
		 *	set font size
		 *	c++ can not overload static and non-static member functions with the same parameter types
		 *	so change the name to setFontSizeObj
		 *	@js setFontSize
		 */
		KDvoid						setFontSizeObj ( KDint32 nSize );
    
		/** 
		 *	get font size 
		 *	@js getFontSize
		 */
		KDint32						getFontSizeObj ( KDvoid ) const;

		/** 
		 *	set the font name 
		 *	c++ can not overload static and non-static member functions with the same parameter types
		 *	so change the name to setFontNameObj
		 *	@js setFontName
		 */
		KDvoid						setFontNameObj ( const std::string& sName );

		/** 
		 *	returns the name of the Font 
		 *	@js getFontNameObj
		 */
		const std::string&			getFontNameObj ( KDvoid ) const;

	protected :

		KDvoid						recreateLabel ( KDvoid );
    
		KDint32						m_nFontSize;
		std::string					m_sFontName;
};


/**
 *	@brief MenuItemSprite accepts Node<RGBAProtocol> objects as items.
 *	The images has 3 different states:
 *		- unselected image
 *		- selected image
 *		- disabled image
 *
 *	@since v0.8.0
 */
class CC_DLL MenuItemSprite : public MenuItem
{
	public :

		/** creates a menu item with a normal, selected and disabled image*/
		static MenuItemSprite*		create ( Node* pNormalSprite, Node* pSelectedSprite, Node* pDisabledSprite = KD_NULL );
		
		/** creates a menu item with a normal and selected image with a callable object */
		static MenuItemSprite*		create ( Node* pNormalSprite, Node* pSelectedSprite, const ccMenuCallback& pCallback );

		/** creates a menu item with a normal,selected  and disabled image with target/selector */
		static MenuItemSprite*		create ( Node* pNormalSprite, Node* pSelectedSprite, Node* pDisabledSprite, const ccMenuCallback& pCallback );

		MenuItemSprite ( KDvoid )
		: m_pNormalImage   ( KD_NULL )
		, m_pSelectedImage ( KD_NULL )
		, m_pDisabledImage ( KD_NULL )
		{ }

		/** initializes a menu item with a normal, selected  and disabled image with a callable object */
		KDbool						initWithNormalSprite ( Node* pNormalSprite, Node* pSelectedSprite, Node* pDisabledSprite, const ccMenuCallback& pCallback );

		/** Gets the image used when the item is not selected */
		inline Node*				getNormalImage ( KDvoid ) const { return m_pNormalImage; }
    
		/** Sets the image used when the item is not selected */
		KDvoid						setNormalImage ( Node* pImage );
    
		/** Gets the image used when the item is selected */
		inline Node*				getSelectedImage ( KDvoid ) const { return m_pSelectedImage; }
    
		/** Sets the image used when the item is selected */
		KDvoid						setSelectedImage ( Node* pImage );
    
		/** Gets the image used when the item is disabled */
		inline Node*				getDisabledImage ( KDvoid ) const { return m_pDisabledImage; }
    
		/** Sets the image used when the item is disabled */
		KDvoid						setDisabledImage ( Node* pImage );
    
		/**
		 *	@since v0.99.5
		 */
		virtual KDvoid				selected ( KDvoid );
		virtual KDvoid				unselected ( KDvoid );
		virtual KDvoid				setEnabled ( KDbool bEnabled );
    
	protected :

		virtual KDvoid				updateImagesVisibility ( KDvoid );

		/** the image used when the item is not selected */
		Node*						m_pNormalImage;
		/** the image used when the item is selected */
		Node*						m_pSelectedImage;
		/** the image used when the item is disabled */
		Node*						m_pDisabledImage;
};


/** 
 *	@brief MenuItemImage accepts images as items.
 *	The images has 3 different states:
 *		- unselected image
 *		- selected image
 *		- disabled image
 *
 *	For best results try that all images are of the same size
 */
class CC_DLL MenuItemImage : public MenuItemSprite
{
	public :

		/** Creates an MenuItemImage. */
		static MenuItemImage*		create ( KDvoid );
		
		/** creates a menu item with a normal and selected image*/
		static MenuItemImage*		create ( const std::string& sNormalImage, const std::string& sSelectedImage );
		
		/** creates a menu item with a normal,selected  and disabled image*/
		static MenuItemImage*		create ( const std::string& sNormalImage, const std::string& sSelectedImage, const std::string& sDisabledImage );
		
		/** creates a menu item with a normal and selected image with a callable object */
		static MenuItemImage*		create ( const std::string& sNormalImage, const std::string& sSelectedImage, const ccMenuCallback& pCallback );

		/** creates a menu item with a normal,selected  and disabled image with a callable object */
		static MenuItemImage*		create ( const std::string& sNormalImage, const std::string& sSelectedImage, const std::string& sDisabledImage, const ccMenuCallback& pCallback );
		
		/**
		 *	@js ctor
		 */
		MenuItemImage ( KDvoid ) { }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~MenuItemImage ( KDvoid ) { }

		KDbool						init ( KDvoid );

		/** initializes a menu item with a normal, selected  and disabled image with a callable object */
		KDbool						initWithNormalImage ( const std::string& sNormalImage, const std::string& sSelectedImage, const std::string& sDisabledImage, const ccMenuCallback& pCallback );

		/** sets the sprite frame for the normal image */
		KDvoid						setNormalSpriteFrame ( SpriteFrame* pFrame );
		
		/** sets the sprite frame for the selected image */
		KDvoid						setSelectedSpriteFrame ( SpriteFrame* pFrame );
		
		/** sets the sprite frame for the disabled image */
		KDvoid						setDisabledSpriteFrame ( SpriteFrame* pFrame );
};


/** 
 *	@brief A MenuItemToggle
 *	A simple container class that "toggles" it's inner items
 *	The inner items can be any MenuItem
 */
class CC_DLL MenuItemToggle : public MenuItem
{
	public :

		/** creates a menu item from a Array with a callable object */
		static MenuItemToggle*		createWithCallback ( const ccMenuCallback& pCallback, Array* pMenuItems );

		/** creates a menu item from a list of items with a callable object */
		static MenuItemToggle*		createWithCallback ( const ccMenuCallback& pCallback, MenuItem* pItem, ... ) CC_REQUIRES_NULL_TERMINATION;

		/** creates a menu item with no target/selector and no items */
		static MenuItemToggle*		create ( KDvoid );

		/** creates a menu item with a item */
		static MenuItemToggle*		create ( MenuItem* pItem );

		/**
		 *	@js ctor
		 */
		MenuItemToggle ( KDvoid )
		: m_uSelectedIndex ( 0 )
		, m_pSubItems ( KD_NULL )            
		{ }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~MenuItemToggle ( KDvoid );

		/** initializes a menu item from a list of items with a callable object */
		KDbool						initWithCallback ( const ccMenuCallback& pCallback, MenuItem* pItem, KDVaListKHR pArgs );

		/** initializes a menu item with a item */
		KDbool						initWithItem ( MenuItem* pItem );
		
		/** add more menu item */
		KDvoid						addSubItem ( MenuItem* pItem );
    
		/** return the selected item */
		MenuItem*					getSelectedItem ( KDvoid );

		/** Gets the index of the selected item */
		inline KDuint				getSelectedIndex ( KDvoid ) const { return m_uSelectedIndex; }
    
		/** Sets the index of the selected item */
		KDvoid						setSelectedIndex ( KDuint uIndex );
    
		/**
		 *	Gets the array that contains the subitems.
		 *	You can add/remove items in runtime, and you can replace the array with a new one.
		 *	@since v0.7.2
		 *	@js NA
		 *	@lua NA
		 */
		inline Array*				getSubItems ( KDvoid ) const { return m_pSubItems; }

		/** Sets the array that contains the subitems. */
		inline KDvoid				setSubItems ( Array* pItems )
		{
			CC_SAFE_RETAIN ( pItems );
			CC_SAFE_RELEASE ( m_pSubItems );
			m_pSubItems = pItems;
		}
    
		// Overrides
		virtual KDvoid				activate ( KDvoid ) override;
		virtual KDvoid				selected ( KDvoid ) override;
		virtual KDvoid				unselected ( KDvoid ) override;
		virtual KDvoid				setEnabled ( KDbool bVar ) override;

	protected :

		/** returns the selected item */
		KDuint						m_uSelectedIndex;
		/**
		 *	Array that contains the subitems. You can add/remove items in runtime, and you can replace the array with a new one.
		 *	@since v0.7.2
		 */
		Array*						m_pSubItems;
};

//	end of GUI group
/// @}
/// @}

NS_CC_END

#endif	// __CCMenuItem_h__
