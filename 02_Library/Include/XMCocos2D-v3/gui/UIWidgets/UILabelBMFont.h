/* -----------------------------------------------------------------------------------
 *
 *      File            UILabelBMFont.h
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

#ifndef __UILabelBMFont_h__
#define __UILabelBMFont_h__

#include "../BaseClasses/UIWidget.h"
#include "../../2d/label_nodes/CCLabelBMFont.h"

namespace gui {
    
/**
 *   @js NA
 *   @lua NA
 */    
class UILabelBMFont : public UIWidget
{
	public :

		/**
		 *	Default constructor
		 */
		UILabelBMFont ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UILabelBMFont ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UILabelBMFont*			create ( KDvoid );
    
		/** init a bitmap font atlas with an initial string and the FNT file */
		KDvoid							setFntFile ( const KDchar* pFileName );
    
		/** set string value for labelbmfont*/
		KDvoid							setText ( const KDchar* pValue );
    
		/** get string value for labelbmfont*/
		const KDchar*					getStringValue ( KDvoid );
		virtual KDvoid					setAnchorPoint ( const cocos2d::Point& pt ) override;
		virtual const cocos2d::Size&	getContentSize ( KDvoid ) const override;
		virtual cocos2d::Node*			getVirtualRenderer ( KDvoid ) override;
		/**
		 * Returns the "class name" of widget.
		 */
		virtual const char* getDescription ( KDvoid ) const override;
	protected:
		virtual KDvoid					initRenderer ( KDvoid ) override;
		virtual KDvoid					onSizeChanged ( KDvoid ) override;
		KDvoid							labelBMFontScaleChangedWithSize ( KDvoid );
		virtual UIWidget*				createCloneInstance ( KDvoid ) override;
		virtual KDvoid					copySpecialProperties ( UIWidget* pModel ) override;

	protected :

		cocos2d::LabelBMFont*			m_pLabelBMFontRenderer;
		KDbool							m_bFntFileHasInit;
		std::string						m_sFntFileName;
		std::string						m_sStringValue;
};
    
}

#endif	// __UILabelBMFont_h__
