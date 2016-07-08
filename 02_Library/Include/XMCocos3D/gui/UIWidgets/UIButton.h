/* -----------------------------------------------------------------------------------
 *
 *      File            UIButton.h
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

#ifndef __UIButton_h__
#define __UIButton_h__

#include "../BaseClasses/UIWidget.h"
#include "../../2d/label_nodes/CCLabelTTF.h"
#include "../../2d/draw_nodes/CCDrawNode.h"

namespace gui {

/**
 *   @js NA
 *   @lua NA
 */
class UIButton : public UIWidget
{
	public :

		/**
		 *	Default constructor
		 */
		UIButton ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UIButton ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UIButton*				create ( KDvoid );
    
		/**
		 *	Load textures for button.
		 *
		 *	@param normal    normal state texture.
		 *
		 *	@param selected    selected state texture.
		 *
		 *	@param disabled    dark state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTextures ( const KDchar* pNormal, const KDchar* pSelected, const KDchar* pDisabled, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load normal state texture for button.
		 *
		 *	@param normal    normal state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTextureNormal ( const KDchar* normal, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load selected state texture for button.
		 *
		 *	@param selected    selected state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTexturePressed ( const KDchar* pSelected, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load dark state texture for button.
		 *
		 *	@param disabled    dark state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTextureDisabled ( const KDchar* pDisabled, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Sets capinsets for button, if button is using scale9 renderer.
		 *
		 *	@param capInsets    capinsets for button
		 */
		KDvoid							setCapInsets ( const cocos2d::Rect& tCapInsets );

		/**
		 *	Sets capinsets for button, if button is using scale9 renderer.
		 *
		 *	@param capInsets    capinsets for button
		 */
		KDvoid							setCapInsetsNormalRenderer ( const cocos2d::Rect& tCapInsets );
    
		/**
		 *	Sets capinsets for button, if button is using scale9 renderer.
		 *
		 *	@param capInsets    capinsets for button
		 */
		KDvoid							setCapInsetsPressedRenderer ( const cocos2d::Rect& tCapInsets );
    
		/**
		 *	Sets capinsets for button, if button is using scale9 renderer.
		 *
		 *	@param capInsets    capinsets for button
		 */
		KDvoid							setCapInsetsDisabledRenderer ( const cocos2d::Rect& tCapInsets );
    
		// override "setAnchorPoint" of widget.
		virtual KDvoid					setAnchorPoint ( const cocos2d::Point& pt ) override;
    
		/**
		 *	Sets if button is using scale9 renderer.
		 *
		 *	@param true that using scale9 renderer, false otherwise.
		 */
		virtual KDvoid					setScale9Enabled ( KDbool bAble );
    
		// override "setFlipX" of widget.
		virtual KDvoid					setFlipX ( KDbool bFlipX ) override;
    
		// override "setFlipY" of widget.
		virtual KDvoid					setFlipY ( KDbool bFlipY ) override;
    
		// override "isFlipX" of widget.
		virtual KDbool					isFlipX ( KDvoid ) override;
    
		// override "isFlipY" of widget.
		virtual KDbool					isFlipY ( KDvoid ) override;
    
		/**
		 *	Changes if button can be clicked zoom effect.
		 *
		 *	@param true that can be clicked zoom effect, false otherwise.
		 */
		KDvoid							setPressedActionEnabled ( KDbool bEnabled );
    
		// override "ignoreContentAdaptWithSize" method of widget.
		virtual KDvoid					ignoreContentAdaptWithSize ( KDbool bIgnore ) override;
    
		// override "getContentSize" method of widget.
		virtual const cocos2d::Size&	getContentSize ( KDvoid ) const override;
    
		// override "getVirtualRenderer" method of widget.
		virtual cocos2d::Node*			getVirtualRenderer ( KDvoid ) override;
    
		/**
		 *	Sets color to widget
		 *
		 *	It default change the color of widget's children.
		 *
		 *	@param color
		 */
		virtual KDvoid					setColor ( const cocos2d::Color3B& tColor ) override;
    
		/**
		 *	Returns the "class name" of widget.
		 */
		virtual const KDchar*			getDescription ( KDvoid ) const override;
    
		KDvoid							setTitleText ( const std::string& sText );
		const std::string&				getTitleText ( KDvoid ) const;
		KDvoid							setTitleColor ( const cocos2d::Color3B& tColor );
		const cocos2d::Color3B&			getTitleColor ( KDvoid ) const;
		KDvoid							setTitleFontSize ( KDfloat fSize );
		KDfloat							getTitleFontSize ( KDvoid ) const;
		KDvoid							setTitleFontName ( const KDchar* pFontName );
		const KDchar*					getTitleFontName ( KDvoid ) const;

	protected :

		virtual KDbool					init ( KDvoid ) override;
		virtual KDvoid					initRenderer ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToNormal ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToPressed ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToDisabled ( KDvoid ) override;
		virtual KDvoid					onSizeChanged ( KDvoid ) override;
    
		KDvoid							normalTextureScaleChangedWithSize ( KDvoid );
		KDvoid							pressedTextureScaleChangedWithSize ( KDvoid );
		KDvoid							disabledTextureScaleChangedWithSize ( KDvoid );
		virtual UIWidget*				createCloneInstance ( KDvoid ) override;
		virtual KDvoid					copySpecialProperties ( UIWidget* pModel ) override;

	protected :

		cocos2d::Node*					m_pButtonNormalRenderer;
		cocos2d::Node*					m_pButtonClickedRenderer;
		cocos2d::Node*					m_pButtonDisableRenderer;
		cocos2d::LabelTTF*				m_pTitleRenderer;
		std::string						m_sNormalFileName;
		std::string						m_sClickedFileName;
		std::string						m_sDisabledFileName;
		KDbool							m_bPrevIgnoreSize;
		KDbool							m_bScale9Enabled;
		cocos2d::Rect					m_tCapInsetsNormal;
		cocos2d::Rect					m_tCapInsetsPressed;
		cocos2d::Rect					m_tCapInsetsDisabled;
		TextureResType					m_eNormalTexType;
		TextureResType					m_ePressedTexType;
		TextureResType					m_eDisabledTexType;
		cocos2d::Size					m_tNormalTextureSize;
		cocos2d::Size					m_tPressedTextureSize;
		cocos2d::Size					m_tDisabledTextureSize;
		KDbool							m_bPressedActionEnabled;
		cocos2d::Color3B				m_tTitleColor;
};

}

#endif	// __UIButton_h__
