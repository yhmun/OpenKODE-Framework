/* -----------------------------------------------------------------------------------
 *
 *      File            UILabelAtlas.h
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

#ifndef __UILabelAtlas_h__
#define __UILabelAtlas_h__

#include "../BaseClasses/UIWidget.h"
#include "../../2d/label_nodes/CCLabelAtlas.h"

namespace gui {

/**
 *   @js NA
 *   @lua NA
 */
class UICCLabelAtlas : public cocos2d::LabelAtlas
{
	public :

		/**
		 *	Default constructor
		 */
		UICCLabelAtlas ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UICCLabelAtlas ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UICCLabelAtlas*			create ( KDvoid );

		KDvoid							setProperty ( const std::string& sString, const std::string& sCharMapFile, KDuint uItemWidth, KDuint uItemHeight, KDuint uStartCharMap );
		KDvoid							setProperty ( const std::string& sString, cocos2d::Texture2D* pTexture, KDuint uItemWidth, KDuint uItemHeight, KDuint uStartCharMap );

		virtual KDvoid					updateDisplayedOpacity ( GLubyte cOpacity ) override;
		virtual KDvoid					draw ( KDvoid ) override;
};
/**
 *   @js NA
 *   @lua NA
 */
class UILabelAtlas : public UIWidget
{
	public :

		/**
		 *	Default constructor
		 */
		UILabelAtlas ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UILabelAtlas ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UILabelAtlas*			create ( KDvoid );
    
		/** initializes the UILabelAtlas with a string, a char map file(the atlas), the width and height of each element and the starting char of the atlas */
		KDvoid							setProperty ( const std::string& sStringValue, const std::string& sCharMapFile, KDint nItemWidth, KDint nItemHeight, const std::string& sStartCharMap );
    
		// set string value for labelatlas.
		KDvoid							setStringValue ( const std::string& sValue );
    
		// get string value for labelatlas.
		const std::string&				getStringValue ( KDvoid ) const;
    
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
    
	protected :
		virtual KDvoid					initRenderer ( KDvoid ) override;
		virtual KDvoid					onSizeChanged ( KDvoid ) override;
		KDvoid							labelAtlasScaleChangedWithSize ( KDvoid );
		virtual UIWidget*				createCloneInstance ( KDvoid ) override;
		virtual KDvoid					copySpecialProperties ( UIWidget* pModel ) override;

	protected :

		UICCLabelAtlas*					m_pLabelAtlasRenderer;
		std::string						m_sStringValue;
		std::string						m_sCharMapFileName;
		KDint							m_nItemWidth;
		KDint							m_nItemHeight;
		std::string						m_sStartCharMap;
};

}

#endif	// __UILabelAtlas_h__
