/* -----------------------------------------------------------------------------------
 *
 *      File            NodeChangeImgs.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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

#ifndef __NodeChangeImgs_h__
#define __NodeChangeImgs_h__

#include "Object/Sprite/Sprite.h"

class cNodeChangeImgs : public Object
{
	public :
				 cNodeChangeImgs ( Node* pNode, KDbool bIsEnlarge );
		virtual ~cNodeChangeImgs ( KDvoid );

		KDbool				update ( KDdouble dLeftTime );

	private :

		Node*				m_pNode;
		cSprite*			m_pSprite;

		KDbool				m_bIsEnlarge;
		KDfloat				m_fScale;

		KDbool				m_bIsSound;
		KDfloat				m_dSoundLeftTime;
};

#endif	// __NodeChangeImgs_h__
