/* -----------------------------------------------------------------------------------
 *
 *      File            SprAni.h
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

#ifndef __SprAni_h__
#define __SprAni_h__

#include "Sprite.h"

class cFrame
{
	public :

		Rect					tRectSrc;

		KDfloat					fDelay;

		KDbool					bAtt;
		KDbool					bDef;

		Point					tCenter;
		Rect					tRectAtt;
		Rect					tRectDef;

		eDirectionType			eDirection;
};

class cSprAni : public Object
{
	public :
				 cSprAni ( Node* pNode, const KDchar* pPath, KDint z, Point tPoint, KDbool bIsTexLoad = true );
		virtual ~cSprAni ( KDvoid );

		KDvoid					changeNode ( Node* pNode, KDint z );

		KDvoid					texLoad ( KDvoid );
		KDvoid					texLoad ( KDint z );
		KDvoid					texRelease ( KDvoid );

		KDvoid					addFrame ( Rect tRectSrc, KDfloat fDelay, KDbool bAtt, KDbool bDef, Point tCenter, Rect tRectAtt, Rect tRectDef );

		KDvoid					setVisible ( KDbool bVisible );
		KDvoid					setAlpha ( KDubyte cAlpha );
		KDubyte					getAlpha ( KDvoid );
		KDvoid					setColor ( KDubyte r, KDubyte g, KDubyte b );
		KDvoid					setPoint ( const Point& tDest );
		KDvoid					anchorPoint ( const Point& tAnchor );
		KDvoid					setPointTime ( Point tPoint, KDdouble dLeftTime, KDbool bIsForciblyChange = true );
		KDvoid					setLoop ( KDbool bLoop );
		KDvoid					setDirection ( eDirectionType eType );
		KDvoid					setFrameInit ( KDint nFrame );
		KDvoid					setScale ( KDfloat fScaleX, KDfloat fScaleY );

		KDvoid					start ( KDvoid );
		KDvoid					stop ( KDvoid );

		KDbool					update ( KDdouble dLeftTickCount, KDbool bIsMove = true );
		KDvoid					updateMove ( KDdouble dLeftTickCount );
	

		cSprite*				getSprite ( KDvoid );
		inline Point			getPoint ( KDvoid )	{ return m_tPoint; }
		inline KDint			getFrameSize ( KDvoid )	{ return m_aFrames.size ( ); }
		inline KDint			getNowFrame	( KDvoid ) { return m_nNowFrame; }
		KDvoid					getFrameData ( KDint nIndex, KDbool& bIsSucces, cFrame& tFrame );

		Rect					getNowFrameAttRect ( KDvoid );
		Rect					getNowFrameDefRect ( KDvoid );

		inline KDvoid			setID ( KDint nId )	{ m_nId = nId; }
		inline KDint			getID ( KDvoid ) { return m_nId; }

	private :

		KDvoid					setFrame ( KDint nFrame );

		Node*					m_pNode;
		cSprite*				m_pSprite;

		KDbool					m_bIsTexLoad;
		std::string				m_sTexPath;

		KDbool					m_bIsVisible;

		KDint					m_nZorder;

		KDubyte					m_cRed;
		KDubyte					m_cGreen;
		KDubyte					m_cBlue;
		KDubyte					m_cAlpha;

		Point					m_tPoint;
		eDirectionType			m_eDirectionType;

		KDbool					m_bLoop;
		KDbool					m_bPlay;
		KDint					m_nNowFrame;
		KDfloat					m_fLeftDelay;

		std::vector<cFrame>		m_aFrames;

		KDbool					m_bIsMove;
		Point					m_tMoveBefore;
		Point					m_tMove;
		KDdouble				m_dTimeMoveFull;
		KDdouble				m_dTimeMoveLeft;

		KDint					m_nId;
};

#endif	// __SprAni_h__
