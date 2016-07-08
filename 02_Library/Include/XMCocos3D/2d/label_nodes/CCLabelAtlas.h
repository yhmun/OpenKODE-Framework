/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelAtlas.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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

#ifndef __CCLabelAtlas_h__
#define __CCLabelAtlas_h__

#include "../base_nodes/CCAtlasNode.h"

NS_CC_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup label
 *	@{
 */

/** 
 *	@brief LabelAtlas is a subclass of AtlasNode.
 *
 *	It can be as a replacement of Label since it is MUCH faster.
 *
 *	LabelAtlas versus Label:
 *		- LabelAtlas is MUCH faster than Label
 *		- LabelAtlas "characters" have a fixed height and width
 *		- LabelAtlas "characters" can be anything you want since they are taken from an image file
 *
 *	A more flexible class is LabelBMFont. It supports variable width characters and it also has a nice editor.
 */
class CC_DLL LabelAtlas : public AtlasNode, public LabelProtocol
{
	public :

		/**
		 *	@js ctor
		 */
		LabelAtlas ( KDvoid ) : m_sString ( "" ) { }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~LabelAtlas ( KDvoid )
		{ 
			m_sString.clear ( ); 
		}

		/** creates the LabelAtlas with a string, a char map file(the atlas), the width and height of each element and the starting char of the atlas */
		static LabelAtlas*			create ( const std::string& sString, const std::string& sCharMapFile, KDint32 nItemWidth, KDint32 nItemHeight, KDint32 nStartCharMap );
    
		/** 
		 *	creates the LabelAtlas with a string and a configuration file
		 *	@since v2.0
		 */
		static LabelAtlas*			create ( const std::string& sString, const std::string& sFntFile );

		/** initializes the LabelAtlas with a string, a char map file(the atlas), the width and height of each element and the starting char of the atlas */
		KDbool						initWithString ( const std::string& sString, const std::string& sCharMapFile,  KDint32 nItemWidth, KDint32 nItemHeight, KDint32 nStartCharMap );
    
		/** 
		 *	initializes the LabelAtlas with a string and a configuration file
		 *	@since v2.0
		 */
		KDbool						initWithString ( const std::string& sString, const std::string& sFntFile );
    
		/** initializes the LabelAtlas with a string, a texture, the width and height in points of each element and the starting char of the atlas */
		KDbool						initWithString ( const std::string& sString, Texture2D* pTexture, KDint32 nItemWidth, KDint32 nItemHeight, KDint32 nStartCharMap );
    
		// super methods
		virtual KDvoid				updateAtlasValues ( KDvoid );

		virtual KDvoid				setString ( const std::string& sLabel ) override;
		virtual const std::string&	getString ( KDvoid ) const override;
    
	#if CC_LABELATLAS_DEBUG_DRAW
		virtual KDvoid				draw ( KDvoid ) override;
	#endif

	protected :

		// string to render
		std::string					m_sString;
		// the first char in the charmap
		KDint32						m_nMapStartChar;
	};

	//	end of GUI group
/// @}
/// @}


NS_CC_END

#endif	// __CCLabelAtlas_h__
