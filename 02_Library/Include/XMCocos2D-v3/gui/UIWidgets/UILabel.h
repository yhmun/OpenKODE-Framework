/* -----------------------------------------------------------------------------------
 *
 *      File            UILabel.h
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

#ifndef __UILabel_h__
#define __UILabel_h__

#include "../BaseClasses/UIWidget.h"
#include "../../2d/label_nodes/CCLabelTTF.h"

namespace gui {

/**
 *   @js NA
 *   @lua NA
 */
class UILabel : public UIWidget
{
	public:
		/**
		 *	Default constructor
		 */
		UILabel ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UILabel ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UILabel*					create ( KDvoid );
    
		/**
		 *	Changes the string value of label.
		 *
		 *	@param text  string value.
		 */
		KDvoid							setText ( const std::string& sText );
    
		/**
		 *	Gets the string value of label.
		 *
		 *	@return text  string value.
		 */
		const std::string&				getStringValue ( KDvoid );
    
		/**
		 *	Gets the string length of label.
		 *
		 *	@return  string length.
		 */
		KDint							getStringLength ( KDvoid );
    
		/**
		 *	Sets the font size of label.
		 *
		 *	@param  font size.
		 */
		KDvoid							setFontSize ( KDint nSize );
    
		/**
		 *	Sets the font name of label.
		 *
		 *	@param  font name.
		 */
		KDvoid							setFontName ( const std::string& sName );
    
		/**
		 *	Sets the touch scale enabled of label.
		 *
		 *	@param  touch scale enabled of label.
		 */
		KDvoid							setTouchScaleChangeEnabled ( KDbool bEnabled );
    
		/**
		 *	Gets the touch scale enabled of label.
		 *
		 *	@return  touch scale enabled of label.
		 */
		KDbool							isTouchScaleChangeEnabled ( KDvoid );
    
		/**
		 *	Changes both X and Y scale factor of the widget.
		 *
		 *	1.0 is the default scale factor. It modifies the X and Y scale at the same time.
		 *
		 *	@param scale     The scale factor for both X and Y axis.
		 */
		virtual KDvoid					setScale ( KDfloat fScale ) override;
    
		/**
		 *	Changes the scale factor on X axis of this widget
		 *
		 *	The deafult value is 1.0 if you haven't changed it before
		 *
		 *	@param fScaleX   The scale factor on X axis.
		 */
		virtual KDvoid					setScaleX ( KDfloat fScaleX ) override;
    
		/**
		 *	Changes the scale factor on Y axis of this widget
		 *
		 *	The Default value is 1.0 if you haven't changed it before.
		 *
		 *	@param fScaleY   The scale factor on Y axis.
		 */
		virtual KDvoid					setScaleY ( KDfloat fScaleY ) override;
    
		// override "setFlipX" method of widget.
		virtual KDvoid					setFlipX ( KDbool bFlipX ) override;
    
		// override "setFlipY" method of widget.
		virtual KDvoid					setFlipY ( KDbool bFlipY ) override;
    
		// override "isFlipX" method of widget.
		virtual KDbool					isFlipX ( KDvoid ) override;
    
		// override "isFlipY" method of widget.
		virtual KDbool					isFlipY ( KDvoid ) override;
    
		// override "setAnchorPoint" method of widget.
		virtual KDvoid					setAnchorPoint ( const cocos2d::Point& pt ) override;
    
		// override "getContentSize" method of widget.
		virtual const cocos2d::Size&	getContentSize ( KDvoid ) const override;
    
		// override "getVirtualRenderer" method of widget.
		virtual cocos2d::Node*			getVirtualRenderer ( KDvoid ) override;
    
		/**
		 *	Returns the "class name" of widget.
		 */
		virtual const KDchar*			getDescription ( KDvoid ) const override;
    
		KDvoid							setTextAreaSize ( const cocos2d::Size& tSize );
		KDvoid							setTextHorizontalAlignment ( cocos2d::TextHAlignment eAlignment );
		KDvoid							setTextVerticalAlignment ( cocos2d::TextVAlignment eAlignment );    

	protected :

		virtual KDbool					init ( KDvoid ) override;
		virtual KDvoid					initRenderer ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToNormal ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToPressed ( KDvoid ) override;
		virtual KDvoid					onPressStateChangedToDisabled ( KDvoid ) override;
		virtual KDvoid					onSizeChanged ( KDvoid ) override;
		KDvoid							clickScale ( KDfloat fScaleX, KDfloat fScaleY );
		KDvoid							labelScaleChangedWithSize ( KDvoid );
		virtual UIWidget*				createCloneInstance ( KDvoid ) override;
		virtual KDvoid					copySpecialProperties ( UIWidget* pModel ) override;

	protected :

		KDbool							m_bTouchScaleChangeEnabled;
		KDfloat							m_fNormalScaleValueX;
		KDfloat							m_fNormalScaleValueY;
		std::string						m_sFontName;
		KDint							m_nFontSize;
		KDfloat							m_fOnSelectedScaleOffset;
		cocos2d::LabelTTF*				m_pLabelRenderer;
};

}

#endif	// __UILabel_h__
