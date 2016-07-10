/* -----------------------------------------------------------------------------------
 *
 *      File            Sprite.h
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

#ifndef __Sprite_h__
#define __Sprite_h__

class cSprite : public Object
{
	public :

		cSprite ( Node* pNode, const KDchar* pPath, KDint z, Point tAnchorPoint, Point tPoint );
		cSprite ( Node* pNode );
		virtual ~cSprite ( KDvoid );

		KDvoid				changeNode ( Node* pNode, KDint z );

		KDvoid				initTexture ( const KDchar* pPath, KDint z, Point tPoint );
		KDvoid				releaseTexture ( KDvoid );

		KDvoid				addFrame ( Rect tRect );
		KDvoid				setFrame ( KDint nFrame );

		KDvoid				setPointTime ( Point tPoint, KDdouble dLeftTime );

		inline Sprite*		get ( KDvoid ) { return m_pSprite; }

		KDvoid				update ( KDdouble dLeftTime );

	private :

		Node*							m_pNode;
		Sprite*							m_pSprite;
		std::vector<SpriteFrame*>		m_aFrames;

		KDbool							m_bIsMove;
		Point							m_tMoveBefore;
		Point							m_tMove;
		KDdouble						m_dTimeMoveFull;
		KDdouble						m_dTimeMoveLeft;
};

#endif	// __Sprite_h__
