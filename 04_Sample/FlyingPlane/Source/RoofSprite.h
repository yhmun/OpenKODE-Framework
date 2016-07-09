/* --------------------------------------------------------------------------
 *
 *      File            RoofSprite.h
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __RoofSprite_h__
#define __RoofSprite_h__

#define ROOF_TAG_MOVE 123

class GameLayer;

class RoofSprite : public CCSprite
{
	public :

		static RoofSprite* create ( GameLayer* pGameLayer );

	public :

		virtual KDbool   init   ( GameLayer* pGameLayer );
		KDvoid           showUp ( KDvoid );

	private :

		GameLayer*       m_pGameLayer;
};

#endif // __RoofSprite_h__
