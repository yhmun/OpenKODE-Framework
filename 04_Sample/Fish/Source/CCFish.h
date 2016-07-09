/* --------------------------------------------------------------------------
 *
 *      File            CCFish.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      海锋 周 on 12-04-12
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      CJLU. All rights reserved.
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

#ifndef __CCFish_h__
#define __CCFish_h__

enum FishType
{
    FishNoraml	= 0 ,
    FishGold		,
    FishShark		,
};

class CCFish : public CCSprite
{
	public :

		static  CCFish*		createWithSpriteFrameName ( const KDchar* szSpriteFrameName );

	public :

		virtual KDbool		 isCatch ( KDvoid );
		virtual KDvoid		setCatch ( KDbool bCatch );

		virtual KDvoid		addPath ( KDvoid );

		virtual KDvoid		moveWithParabola ( CCSprite* pSprite, const CCPoint& tStartPoint, const CCPoint& tEndPoint, KDfloat fStartAngle, KDfloat fEndAngle, KDfloat fTime );
		 
		virtual KDbool		randomCatch ( KDint nLevel );

	protected :

		KDbool				m_bIsCatch;
};

#endif	// __CCFish_h__