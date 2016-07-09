/* --------------------------------------------------------------------------
 *
 *      File            MainScene.h
 *      Ported By       Hyeon-Ho Jeong
 *      Contact         skdldnjs@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.       
 *
 *      http://www.usmanacademy.com/2012/07/rage-of-war-game-using-cocos2d-x-and.html
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

#ifndef __MainScene_h__
#define __MainScene_h__


class MainScene : public CCLayer
{
	public : 

		CREATE_FUNC ( MainScene );

	protected :

		virtual KDbool		init		( KDvoid );  
    
		KDvoid				onMenuClose ( CCObject* pSender );
		KDvoid				onLevel1	( CCObject* pSender );
		KDvoid				onLevel2	( CCObject* pSender );
};


#endif  // __MainScene_h__