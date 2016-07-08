/* -----------------------------------------------------------------------------------
 *
 *      File            CCProgress.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCProgress_h__
#define __CCProgress_h__

#include "CCProgressTimer.h"
#include <string>

#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN
    
/**
 * @addtogroup misc_nodes
 * @{
 */

typedef enum 
{
	kCCProgressTypeToRight       = 0x01,
	kCCProgressTypeToLeft        = 0x02,	
	kCCProgressTypeToTop         = 0x10,
	kCCProgressTypeToBottom      = 0x20,
	kCCProgressTypeToRightTop    = kCCProgressTypeToRight | kCCProgressTypeToTop	,
	kCCProgressTypeToRightBottom = kCCProgressTypeToRight | kCCProgressTypeToBottom	,
	kCCProgressTypeToLeftTop     = kCCProgressTypeToLeft  | kCCProgressTypeToTop	,
	kCCProgressTypeToLeftBottom  = kCCProgressTypeToLeft  | kCCProgressTypeToBottom	,
} CCProgressType;

class CCProgress : public CCProgressTimer
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
	public :
		         CCProgress ( KDvoid );
		virtual ~CCProgress ( KDvoid );

		static  CCProgress*			create ( const KDchar* szFilePath, const CCSize& tSize = CCSizeZero );		

	public :

		virtual KDbool				initWithFile ( const KDchar* szFilePath, const CCSize& tSize = CCSizeZero );	

		virtual KDvoid				draw ( KDvoid );

		KDvoid						setType ( CCProgressType eProgressType );
		CCProgressType				getType ( KDvoid );

		virtual KDvoid				setPercentage ( KDfloat fPercentage );
		virtual KDfloat				getPercentage ( KDvoid );

		virtual KDvoid				setContentSize ( const CCSize& tSize );

		virtual KDvoid				setColor ( const ccColor3B& tColor );
		virtual const ccColor3B&	getColor ( KDvoid ) const;

		virtual GLubyte				getOpacity ( KDvoid ) const;
		virtual KDvoid				setOpacity ( GLubyte cOpacity );

		virtual CCTexture2D*		getTexture ( KDvoid );

	private :

		KDvoid						purge ( KDvoid );

		KDvoid						updateVBO ( KDvoid );

	protected :
				
		CCProgressType				m_eProgressType;
		KDfloat						m_fPercentage;
		KDbool						m_bDirty;		
		CCSize						m_pSizes [ 9 ];
		GLuint						m_pGIDs [ 2 ];
		GLushort*					m_pIndices;
		ccV2F_C4B_T2F*				m_pVBOs;
		CCTexture2D*				m_pTexture;
        std::string					m_sFilePath;
};

// end of misc_nodes group
/// @}

NS_CC_END

#endif // __CCProgress_h__
