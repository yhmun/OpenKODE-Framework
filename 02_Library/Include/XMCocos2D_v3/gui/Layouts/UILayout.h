/* -----------------------------------------------------------------------------------
 *
 *      File            UILayout.h
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

#ifndef __UILayout_h__
#define __UILayout_h__

#include "../BaseClasses/UIWidget.h"
#include "../../2d/misc_nodes/CCClippingNode.h"
#include "../../2d/draw_nodes/CCDrawNode.h"

namespace gui {

typedef enum
{
    LAYOUT_COLOR_NONE,
    LAYOUT_COLOR_SOLID,
    LAYOUT_COLOR_GRADIENT
} LayoutBackGroundColorType;

typedef enum
{
    LAYOUT_ABSOLUTE,
    LAYOUT_LINEAR_VERTICAL,
    LAYOUT_LINEAR_HORIZONTAL,
    LAYOUT_RELATIVE
} LayoutType;


/**
 *  @js NA
 *  @lua NA
 */
class UILayout : public UIWidget
{
	public :

		/**
		 *	Default constructor
		 */
		UILayout ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UILayout ( KDvoid );
    
		/**
		 *	Allocates and initializes a layout.
		 */
		static UILayout*				create ( KDvoid );
    
		//override "hitTest" method of widget.
		virtual KDbool					hitTest ( const cocos2d::Point& pt ) override;
    
		// background

		/**
		 *	Sets a background image for layout
		 *
		 *	@param fileName image file path.
		 *
		 *	@param texType @see TextureResType. UI_TEX_TYPE_LOCAL means local file, UI_TEX_TYPE_PLIST means sprite frame.
		 */
		KDvoid							setBackGroundImage ( const KDchar* pFileName,TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Sets a background image capinsets for layout, if the background image is a scale9 render.
		 *
		 *	@param capinsets of background image.
		 *
		 */
		KDvoid							setBackGroundImageCapInsets ( const cocos2d::Rect& tCapInsets );
    
		/**
		 *	Sets Color Type for layout.
		 *
		 *	@param type   @see LayoutBackGroundColorType.
		 */
		KDvoid							setBackGroundColorType ( LayoutBackGroundColorType eType );
    
		/**
		 *	Sets background iamge use scale9 renderer.
		 *
		 *	@param enabled   true that use scale9 renderer, false otherwise.
		 */
		KDvoid							setBackGroundImageScale9Enabled ( KDbool bEnabled );
    
		/**
		 *	Sets background color for layout, if color type is LAYOUT_COLOR_SOLID
		 *
		 *	@param color
		 */
		KDvoid							setBackGroundColor ( const cocos2d::Color3B& tColor );
    
		/**
		 *	Sets background color for layout, if color type is LAYOUT_COLOR_GRADIENT
		 *
		 *	@param start color
		 *
		 *	@param end color
		 */
		KDvoid							setBackGroundColor ( const cocos2d::Color3B& tStartColor, const cocos2d::Color3B& tEndColor );
    
		/**
		 * Sets background opacity layout.
		 *
		 * @param opacity
		 */
		KDvoid							setBackGroundColorOpacity ( KDint nOpacity );
    
		/**
		 *	Sets background color vector for layout, if color type is LAYOUT_COLOR_GRADIENT
		 *
		 *	@param vector
		 */
		KDvoid							setBackGroundColorVector ( const cocos2d::Point& tVector );
    
		// override "setColor" method of widget.
		virtual KDvoid					setColor ( const cocos2d::Color3B& tColor ) override;
    
		// override "setOpacity" method of widget.
		virtual KDvoid					setOpacity ( KDint nOpacity ) override;
    
		/**
		 *	Remove the background image of layout.
		 */
		KDvoid							removeBackGroundImage ( KDvoid );
    
		/**
		 *	Gets background image texture size.
		 *
		 *	@return background image texture size.
		 */
		const cocos2d::Size&			getBackGroundImageTextureSize ( KDvoid ) const;
    
		/**
		 *	Changes if layout can clip it's content and child.
		 *
		 *	If you really need this, please enable it. But it would reduce the rendering efficiency. 
		 *
		 *	@param clipping enabled.
		 */
		virtual KDvoid					setClippingEnabled ( KDbool bEnabled );
    
		/**
		 *	Gets if layout is clipping enabled.
		 *
		 *	@return if layout is clipping enabled.
		 */
		virtual KDbool					isClippingEnabled ( KDvoid );
    
		/**
		 *	Gets the content size of widget.
		 *
		 *	Content size is widget's texture size.
		 */
		virtual const cocos2d::Size&	getContentSize ( KDvoid ) const override;
    
		/**
		 *	Returns the "class name" of widget.
		 */
		virtual const KDchar*			getDescription ( KDvoid ) const override;
    
		/**
		 *	Sets LayoutType.
		 *
		 *	@see LayoutType
		 *
		 *	@param LayoutType
		 */
		virtual KDvoid					setLayoutType ( LayoutType eType );
    
		/**
		 *	Gets LayoutType.
		 *
		 *	@see LayoutType
		 *
		 *	@return LayoutType
		 */
		virtual LayoutType				getLayoutType ( KDvoid ) const;
    
		virtual KDvoid					doLayout ( KDvoid );
    
		/**
		 *	Adds a child to the container.
		 *
		 *	@param child A child widget	
		 */
		virtual KDbool					addChild ( UIWidget* pChild ) override;
    
	protected :

		// override "init" method of widget.
		virtual KDbool					init ( KDvoid ) override;
    
		// override "initRenderer" method of widget.
		virtual KDvoid					initRenderer ( KDvoid ) override;
    
		// override "onSizeChanged" method of widget.
		virtual KDvoid					onSizeChanged ( KDvoid ) override;
    
		// init background image renderer.
		KDvoid							addBackGroundImage ( KDvoid );
    
		KDvoid							supplyTheLayoutParameterLackToChild ( UIWidget* pChild );
		virtual UIWidget*				createCloneInstance ( KDvoid ) override;
		virtual KDvoid					copySpecialProperties ( UIWidget* pModel ) override;
		virtual KDvoid					copyClonedWidgetChildren ( UIWidget* pModel ) override;

	protected :

		KDbool							m_bClippingEnabled;
		
		// background
		KDbool							m_bBackGroundScale9Enabled;
		cocos2d::Node*					m_pBackGroundImage;
		std::string						m_sBackGroundImageFileName;
		cocos2d::Rect					m_tBackGroundImageCapInsets;
		LayoutBackGroundColorType		m_eColorType;
		TextureResType					m_eBgImageTexType;
		cocos2d::LayerColor*			m_pColorRender;
		cocos2d::LayerGradient*			m_pGradientRender;
		cocos2d::Color3B				m_cColor;
		cocos2d::Color3B				m_gStartColor;
		cocos2d::Color3B				m_gEndColor;
		cocos2d::Point					m_tAlongVector;
		KDint							m_cOpacity;
		cocos2d::Size					m_tBackGroundImageTextureSize;
		LayoutType						m_eLayoutType;
};

/**
 *  @js NA
 *  @lua NA
 */
class UIRectClippingNode : public cocos2d::ClippingNode
{
	public :

		virtual ~UIRectClippingNode ( KDvoid );

		virtual KDbool					init ( KDvoid );
		static UIRectClippingNode*		create ( KDvoid );
		KDvoid							setClippingSize ( const cocos2d::Size& tSize );
		KDvoid							setClippingEnabled ( KDbool bEnabled );
		virtual KDvoid					visit ( KDvoid );
		KDvoid							setEnabled ( KDbool bEnabled );
		KDbool							 isEnabled ( KDvoid ) const;

	protected :

		cocos2d::DrawNode*				m_pInnerStencil;
		KDbool							m_bEnabled;

	private :

		UIRectClippingNode ( KDvoid );

		cocos2d::Point					m_aRect [ 4 ];
		cocos2d::Size					m_tClippingSize;
		KDbool							m_bClippingEnabled;
};

}

#endif	// __UILayout_h__
