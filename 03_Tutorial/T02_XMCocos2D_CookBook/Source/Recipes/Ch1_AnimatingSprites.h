/* --------------------------------------------------------------------------
 *
 *      File            Ch1_AnimatingSprites.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#ifndef __Ch1_AnimatingSprites_h__
#define __Ch1_AnimatingSprites_h__

class SimpleAnimObject;

class Ch1_AnimatingSprites : public Recipe
{
	public :

		SCENE_FUNC ( Ch1_AnimatingSprites );

	protected :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onExit ( KDvoid );

		KDvoid				step ( KDfloat fDelta );

		KDfloat				makeBatFlyUp ( SimpleAnimObject* pBat );

		KDvoid				makeBatGlideDown ( SimpleAnimObject* pBat );

	private :

		CCArray*			m_pBats;
		CCSprite*			m_pLightningBolt;
		CCSprite*			m_pLightningGlow;
		KDint				m_nLightningRemoveCount;
};

#endif	// __Ch1_AnimatingSprites_h__
