/* --------------------------------------------------------------------------
 *
 *      File            KWSprite.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/05/30 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
 *      
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __KWSprite_h__
#define __KWSprite_h__

class KWSprite : public CCSprite 
{
	public :

		static  KWSprite*		create ( const KDchar* szPath );

		static  KWSprite*		createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect );

		static  KWSprite*		createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame );

	public :

		virtual KDbool			initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect );

		virtual KDbool			collideWithPoint  ( const CCPoint& tPoint );
		
		virtual KDbool			collideWithSprite ( KWSprite* pSprite );

		virtual KDbool			collideWithCircle ( const CCPoint& tCenter, KDfloat fRadius );

		virtual KDfloat			distance ( KWSprite* pSprite );

		virtual CCRect			absoluteHitArea ( KDvoid );

		virtual CCPoint			center ( KDvoid );

		virtual KDdouble		x ( KDvoid );
		virtual KDdouble		y ( KDvoid );

		virtual KDvoid			setX ( KDdouble x );
		virtual KDvoid			setY ( KDdouble y );

	protected :
  
		// relative hitarea from 'anchor point'.
		CCRect					m_tHitArea;
};

#endif	// __KWSprite_h__