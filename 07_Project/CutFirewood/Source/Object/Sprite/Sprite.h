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

#ifndef __cSprite_h__
#define __cSprite_h__

#include "../Draw/Draw.h"
#include "TextureManage.h"

class cCamera;

class cSprite : public cDraw, cTextureManage
{
	public :

		//init
		cSprite ( const KDchar* pFileName, KDint z, Point tPoint );
		virtual ~cSprite ( KDvoid );

		// visible
		inline KDvoid		setIsVisible ( KDbool bIsVisible ) { m_bIsVisible = bIsVisible; }
		inline KDbool		getIsVisible ( KDvoid ) { return m_bIsVisible; }

		// static
		inline KDvoid		setIsStatic ( KDbool bIsStatic ) { m_bIsStatic = bIsStatic; }
		inline KDbool		getIsStatic ( KDvoid ) { return m_bIsStatic; }

		// position
		inline KDvoid		setAnchorPoint ( Point tPoint )	{ m_tAnchorPoint = tPoint; }
		inline KDvoid		setPoint	   ( Point tPoint )	{ m_tPoint = tPoint; }
		inline Point		getAnchorPoint ( KDvoid ) {	return m_tAnchorPoint; }
		inline Point		getPoint	   ( KDvoid ) {	return m_tPoint; }
		Point				getScreenPoint ( cCamera* pCamera );

		// size
		KDfloat				getWidth ( KDvoid );
		KDfloat				getHeight ( KDvoid );

		// opacity
		KDvoid				setOpacity ( KDint nOpacity );
		inline KDint		getOpacity ( KDvoid ) { return m_nOpacity; }

		// color
		KDvoid				setColor ( KDint r, KDint g, KDint b );
		inline Color3B		getColor ( KDvoid )	{ return m_tColor; }

		// rotation
		inline KDvoid		setRotation ( KDfloat fRotation ) {	m_fRotation = fRotation; }
		inline KDfloat		getRotation ( KDvoid ) { return m_fRotation; }

		// scale
		inline KDvoid		setScale  ( KDfloat fScale ) { m_fScaleX = fScale; m_fScaleY = fScale; }
		inline KDvoid		setScaleX ( KDfloat fScale ) { m_fScaleX = fScale; }
		inline KDvoid		setScaleY ( KDfloat fScale ) { m_fScaleY = fScale; }
		inline KDfloat		getScaleX ( KDvoid ) { return m_fScaleX; }
		inline KDfloat		getScaleY ( KDvoid ) { return m_fScaleY; }
						
		// flip
		inline KDvoid		setIsFlipX ( KDbool bIsFlip ) {	m_bIsFlipX = bIsFlip; }
		inline KDvoid		setIsFlipY ( KDbool bIsFlip ) {	m_bIsFlipY = bIsFlip; }
		inline KDbool		getIsFlipX ( KDvoid ) {	return m_bIsFlipX; }
		inline KDbool		getIsFlipY ( KDvoid ) {	return m_bIsFlipY; }

		// set text rect
		KDvoid				setTextureRect ( Rect tRect );
		inline Rect			getTextureRect ( KDvoid ) {	return m_tTextureRect; }

		// update
		KDvoid				update ( KDdouble dLeftTime ) { }

		// draw
 		KDvoid				draw ( cCamera* pCamera );	

	private :

		// texture uid
		KDint				m_nTextureUid;

		// visible
		KDbool				m_bIsVisible;

		// static
		KDbool				m_bIsStatic;

		// pt
		Point				m_tAnchorPoint;
		Point				m_tPoint;

		// opacity
		KDint				m_nOpacity;

		// color
		Color3B				m_tColor;

		// rotation angle
		KDfloat				m_fRotation;

		// scaling factors
		KDfloat				m_fScaleX;
		KDfloat				m_fScaleY;

		// flip
		KDbool				m_bIsFlipX;
		KDbool				m_bIsFlipY;

		// text rect
		Rect				m_tTextureRect;		
};

#endif	// __cSprite_h__