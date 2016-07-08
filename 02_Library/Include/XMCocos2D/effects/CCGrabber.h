/* -----------------------------------------------------------------------------------
 *
 *      File            CCGrabber.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
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

#ifndef __CCGrabber_h__
#define __CCGrabber_h__

#include "../cocoa/CCObject.h"

NS_CC_BEGIN
    
class CCTexture2D;

/**
 * @addtogroup effects
 * @{
 */

/** FBO class that grabs the the contents of the screen */
class CCGrabber : public CCObject
{
	public :
				 CCGrabber ( KDvoid );
		virtual ~CCGrabber ( KDvoid );

	public :

		KDvoid			grab ( CCTexture2D* pTexture );

		KDvoid			beforeRender ( CCTexture2D* pTexture );

		KDvoid			afterRender ( CCTexture2D* pTexture );

	protected :

		GLuint			m_uFBO;
		GLint			m_nOldFBO;
		GLfloat			m_aOldClearColor[4];
};

// end of effects group
/// @}

NS_CC_END

#endif // __CCGrabber_h__
