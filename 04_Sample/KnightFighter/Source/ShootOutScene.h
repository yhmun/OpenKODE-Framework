/* --------------------------------------------------------------------------
 *
 *      File            ShootOutScene.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 17/05/2011
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

#ifndef __ShootOutScene_h__
#define __ShootOutScene_h__

class ShootOutScene : public CCLayer 
{
	public :

		SCENE_FUNC ( ShootOutScene );

	public :

		virtual KDbool		init ( KDvoid );
	/*
	Knight *knight;
	int playerShots;
	CCLabelTTF *playerShotsLabel;
	*/
};

/*
@property (nonatomic, retain) Knight *knight;
@property (nonatomic) int playerShots;
@property (nonatomic, retain) CCLabelTTF *playerShotsLabel;

-(void) shootOutMovement;
-(void)spriteMoveFinished:(id)sender;
-(void) finishedShootOut:(BOOL)playerDead;

@end
*/

#endif	// __ShootOutScene_h__
