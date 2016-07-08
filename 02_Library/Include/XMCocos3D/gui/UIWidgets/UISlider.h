/* -----------------------------------------------------------------------------------
 *
 *      File            UISlider.h
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

#ifndef __UISlider_h__
#define __UISlider_h__

#include "../BaseClasses/UIWidget.h"
#include "../../2d/sprite_nodes/CCSprite.h"

namespace gui {

typedef enum
{
    SLIDER_PERCENTCHANGED
} SliderEventType;

typedef KDvoid ( cocos2d::Object::*SEL_SlidPercentChangedEvent ) ( cocos2d::Object*, SliderEventType );
#define sliderpercentchangedselector(_SELECTOR) (SEL_SlidPercentChangedEvent) ( &_SELECTOR )

/**
 *   @js NA
 *   @lua NA
 */
class UISlider : public UIWidget
{
	public :

		/**
		 *	Default constructor
		 */
		UISlider ( KDvoid );
    
		/**
		 *  Default destructor
		 */
		virtual ~UISlider ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UISlider*				create ( KDvoid );
    
		/**
		 *	Load texture for slider bar.
		 *
		 *	@param fileName   file name of texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadBarTexture ( const KDchar* pFileName, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Sets if slider is using scale9 renderer.
		 *
		 *	@param true that using scale9 renderer, false otherwise.
		 */
		KDvoid							setScale9Enabled ( KDbool bAble );
    
		/**
		 *	Sets capinsets for slider, if slider is using scale9 renderer.
		 *
		 *	@param capInsets    capinsets for slider
		 */
		KDvoid							setCapInsets ( const cocos2d::Rect& tCapInsets );
    
		/**
		 *	Sets capinsets for slider, if slider is using scale9 renderer.
		 *
		 *	@param capInsets    capinsets for slider
		 */
		KDvoid							setCapInsetsBarRenderer ( const cocos2d::Rect& tCapInsets );
		
		/**
		 *	Sets capinsets for slider, if slider is using scale9 renderer.
		 *
		 *	@param capInsets    capinsets for slider
		 */
		KDvoid							setCapInsetProgressBarRebderer ( const cocos2d::Rect& tCapInsets );
    
		/**
		 *	Load textures for slider ball.
		 *
		 *	@param slider ball normal    normal state texture.
		 *
		 *	@param slider ball selected    selected state texture.
		 *
		 *	@param slider ball disabled    dark state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadSlidBallTextures ( const KDchar* pNormal, const KDchar* pPressed, const KDchar* pDisabled, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load normal state texture for slider ball.
		 *
		 *	@param normal    normal state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadSlidBallTextureNormal ( const KDchar* pNormal, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load selected state texture for slider ball.
		 *
		 *	@param selected    selected state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadSlidBallTexturePressed ( const KDchar* pPressed, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load dark state texture for slider ball.
		 *
		 *	@param disabled    dark state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadSlidBallTextureDisabled ( const KDchar* pDisabled, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load dark state texture for slider progress bar.
		 *
		 *	@param fileName    file path of texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadProgressBarTexture ( const KDchar* pFileName, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Changes the progress direction of slider.
		 *
		 *	@param percent    percent value from 1 to 100.
		 */
		KDvoid							setPercent ( KDint nPercent );
    
		/**
		 *	Gets the progress direction of slider.
		 *
		 *	@return percent    percent value from 1 to 100.
		 */
		KDint							getPercent ( KDvoid );
    
		/**
		 *	Add call back function called when slider's percent has changed to slider.
		 */
		KDvoid							addEventListenerSlider ( cocos2d::Object* pTarget, SEL_SlidPercentChangedEvent pSelector );
    
		// override "onTouchBegan" method of widget.
		virtual KDbool					onTouchBegan ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchMoved" method of widget.
		virtual KDvoid					onTouchMoved ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchEnded" method of widget.
		virtual KDvoid					onTouchEnded ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchCancelled" method of widget.
		virtual KDvoid					onTouchCancelled ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "getContentSize" method of widget.
		virtual const cocos2d::Size&	getContentSize ( KDvoid ) const override;
    
		// override "getVirtualRenderer" method of widget.
		virtual cocos2d::Node*			getVirtualRenderer ( KDvoid ) override;
    
		// override "ignoreContentAdaptWithSize" method of widget.
		virtual KDvoid					ignoreContentAdaptWithSize ( KDbool bIgnore ) override;
    
		/**
		 *	Returns the "class name" of widget.
		 */
		virtual const KDchar*			getDescription ( KDvoid ) const override;

	protected :

		virtual KDvoid					initRenderer ( KDvoid ) override;
		KDfloat							getPercentWithBallPos ( KDfloat fLocation );
		KDvoid							percentChangedEvent ( KDvoid );
		virtual KDvoid					onPressStateChangedToNormal ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToPressed ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToDisabled ( KDvoid ) override;
		virtual KDvoid					onSizeChanged ( KDvoid ) override;
		KDvoid							barRendererScaleChangedWithSize ( KDvoid );
		KDvoid							progressBarRendererScaleChangedWithSize ( KDvoid );
		virtual UIWidget*				createCloneInstance ( KDvoid ) override;
		virtual KDvoid					copySpecialProperties ( UIWidget* pModel ) override;

	protected :

		cocos2d::Node*					m_pBarRenderer;
		cocos2d::Node*					m_pProgressBarRenderer;
		cocos2d::Size					m_tProgressBarTextureSize;
    
		cocos2d::Sprite*				m_pSlidBallNormalRenderer;
		cocos2d::Sprite*				m_pSlidBallPressedRenderer;
		cocos2d::Sprite*				m_pSlidBallDisabledRenderer;
		cocos2d::Node*					m_pSlidBallRenderer;
    
		KDfloat							m_fBarLength;
		KDint							m_nPercent;
    
		KDbool							m_bScale9Enabled;
		KDbool							m_bPrevIgnoreSize;
		std::string						m_sTextureFile;
		std::string						m_sProgressBarTextureFile;
		std::string						m_sSlidBallNormalTextureFile;
		std::string						m_sSlidBallPressedTextureFile;
		std::string						m_sSlidBallDisabledTextureFile;

		cocos2d::Rect					m_tCapInsetsBarRenderer;
		cocos2d::Rect					m_tCapInsetsProgressBarRenderer;

		cocos2d::Object*				m_pSliderEventListener;
		SEL_SlidPercentChangedEvent		m_pSliderEventSelector;
		TextureResType					m_eBarTexType;
		TextureResType					m_eProgressBarTexType;
		TextureResType					m_eBallNTexType;
		TextureResType					m_eBallPTexType;
		TextureResType					m_eBallDTexType;
};

}

#endif	// __UISlider_h__
