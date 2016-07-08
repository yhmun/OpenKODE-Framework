/* -----------------------------------------------------------------------------------
 *
 *      File            UICheckBox.h
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

#ifndef __UICheckBox_h__
#define __UICheckBox_h__

#include "../BaseClasses/UIWidget.h"
#include "../../2d/sprite_nodes/CCSprite.h"

namespace gui {

typedef enum
{
    CHECKBOX_STATE_EVENT_SELECTED,
    CHECKBOX_STATE_EVENT_UNSELECTED
}CheckBoxEventType;

typedef KDvoid ( cocos2d::Object::*SEL_SelectedStateEvent ) ( cocos2d::Object*, CheckBoxEventType );
#define checkboxselectedeventselector(_SELECTOR)			(SEL_SelectedStateEvent) ( &_SELECTOR )

/**
 *   @js NA
 *   @lua NA
 */
class UICheckBox : public UIWidget
{
	public :

		/**
		 *	Default constructor
		 */
		UICheckBox ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UICheckBox ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UICheckBox*				create ( KDvoid );
    
		/**
		 *	Load textures for checkbox.
		 *
		 *	@param backGround    backGround texture.
		 *
		 *	@param backGroundSelected    backGround selected state texture.
		 *
		 *	@param cross    cross texture.
		 *
		 *	@param frontCrossDisabled    cross dark state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTextures ( const KDchar* pBackGround, const KDchar* pBackGroundSelected,const char* cross, const KDchar* pBackGroundDisabled, const KDchar* pFrontCrossDisabled, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load backGround texture for checkbox.
		 *
		 *	@param backGround    backGround texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTextureBackGround ( const KDchar* pBackGround, TextureResType eType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load backGroundSelected texture for checkbox.
		 *
		 *	@param backGroundSelected     backGround selected state texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTextureBackGroundSelected ( const KDchar* pBackGroundSelected, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load cross texture for checkbox.
		 *
		 *	@param cross    cross texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTextureFrontCross ( const KDchar* pCross, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load backGroundDisabled texture for checkbox.
		 *
		 *	@param backGroundDisabled    backGroundDisabled texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTextureBackGroundDisabled ( const KDchar* pBackGroundDisabled, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Load frontCrossDisabled texture for checkbox.
		 *
		 *	@param frontCrossDisabled    frontCrossDisabled texture.
		 *
		 *	@param texType    @see UI_TEX_TYPE_LOCAL
		 */
		KDvoid							loadTextureFrontCrossDisabled ( const KDchar* pFrontCrossDisabled, TextureResType eTexType = UI_TEX_TYPE_LOCAL );
    
		/**
		 *	Sets selcted state for checkbox.
		 *
		 *	@param selected    true that checkbox is selected, false otherwise.
		 */
		KDvoid							setSelectedState ( KDbool bSelected );
    
		/**
		 *	Gets selcted state of checkbox.
		 *
		 *	@return selected    true that checkbox is selected, false otherwise.
		 */
		KDbool							getSelectedState ( KDvoid );
    
		// override "setAnchorPoint" method of widget.
		virtual KDvoid					setAnchorPoint ( const cocos2d::Point& pt ) override;
    
		// add a call back function would called when checkbox is selected or unselected.
		KDvoid							addEventListenerCheckBox ( cocos2d::Object* pTarget, SEL_SelectedStateEvent pSelector );
    
		// override "setFlipX" method of widget.
		virtual KDvoid					setFlipX ( KDbool bFlipX ) override;
    
		// override "setFlipY" method of widget.
		virtual KDvoid					setFlipY ( KDbool bFlipY ) override;
    
		// override "isFlipX" method of widget.
		virtual KDbool					isFlipX ( KDvoid ) override;
    
		// override "isFlipY" method of widget.
		virtual KDbool					isFlipY ( KDvoid ) override;
    
		// override "onTouchEnded" method of widget.
		virtual KDvoid					onTouchEnded ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "getContentSize" method of widget.
		virtual const cocos2d::Size&	getContentSize ( KDvoid ) const override;
    
		// override "getVirtualRenderer" method of widget.
		virtual cocos2d::Node*			getVirtualRenderer ( KDvoid ) override;
    
		/**
		 *	Returns the "class name" of widget.
		 */
		virtual const KDchar*			getDescription ( KDvoid ) const override;

	protected :

		virtual KDbool					init ( KDvoid ) override;
		virtual KDvoid					initRenderer ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToNormal ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToPressed ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToDisabled ( KDvoid ) override;
		KDvoid							selectedEvent ( KDvoid );
		KDvoid							unSelectedEvent ( KDvoid );
		virtual KDvoid					onSizeChanged ( KDvoid ) override;
		KDvoid							backGroundTextureScaleChangedWithSize ( KDvoid );
		KDvoid							backGroundSelectedTextureScaleChangedWithSize ( KDvoid );
		KDvoid							frontCrossTextureScaleChangedWithSize ( KDvoid );
		KDvoid							backGroundDisabledTextureScaleChangedWithSize ( KDvoid );
		KDvoid							frontCrossDisabledTextureScaleChangedWithSize ( KDvoid );
		virtual UIWidget*				createCloneInstance ( KDvoid ) override;
		virtual KDvoid					copySpecialProperties ( UIWidget* pModel ) override;

	protected :

		cocos2d::Sprite*				m_pBackGroundBoxRenderer;
		cocos2d::Sprite*				m_pBackGroundSelectedBoxRenderer;
		cocos2d::Sprite*				m_pFrontCrossRenderer;
		cocos2d::Sprite*				m_pBackGroundBoxDisabledRenderer;
		cocos2d::Sprite*				m_pFrontCrossDisabledRenderer;
		KDbool							m_bIsSelected;

		cocos2d::Object*				m_pCheckBoxEventListener;
		SEL_SelectedStateEvent			m_pCheckBoxEventSelector;
    
		TextureResType					m_eBackGroundTexType;
		TextureResType					m_eBackGroundSelectedTexType;
		TextureResType					m_eFrontCrossTexType;
		TextureResType					m_eBackGroundDisabledTexType;
		TextureResType					m_eFrontCrossDisabledTexType;
    
		std::string						m_sBackGroundFileName;
		std::string						m_sBackGroundSelectedFileName;
		std::string						m_sFrontCrossFileName;
		std::string						m_sBackGroundDisabledFileName;
		std::string						m_sFrontCrossDisabledFileName;
};

}

#endif	// __UICheckBox_h__
