/* --------------------------------------------------------------------------
 *
 *      File            SimpleAnimObject.h
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

#ifndef __SimpleAnimObject_h__
#define __SimpleAnimObject_h__

class SimpleAnimObject : public CCSprite
{
	public :

		static SimpleAnimObject*	create ( KDvoid );

		static SimpleAnimObject*	create ( const KDchar* szFilePath );

		static SimpleAnimObject*	createWithBatchNode ( CCSpriteBatchNode* pBatchNode, const CCRect& tRect );

		static SimpleAnimObject*	createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame );

		static SimpleAnimObject*	createWithSpriteFrameName ( const KDchar* szSpriteFrameName );

	public :		

		virtual KDvoid			update ( KDfloat fDelta );

		virtual CCRect			rect ( KDvoid );

		virtual KDint			getAnimationType ( KDvoid );
		virtual KDvoid			setAnimationType ( KDint nAnimationType );

		virtual const CCPoint&	getVelocity ( KDvoid );
		virtual KDvoid			setVelocity ( const CCPoint& tVelocity );

	protected :

		KDint					m_nAnimationType;
		CCPoint					m_tVelocity;
};

#endif	// __SimpleAnimObject_h__
