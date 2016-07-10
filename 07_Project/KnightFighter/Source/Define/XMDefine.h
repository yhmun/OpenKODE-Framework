/* --------------------------------------------------------------------------
 *
 *      File            XMDefine.h
 *      Description     XMDefine
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
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

#ifndef __XMDefine_h__
#define __XMDefine_h__

#define CL(__className__) [ ] ( b2World* pWorld, String* sType ) { return __className__::create ( pWorld, sType ); }

#define BOSS_STATE_RATE					3.5f

enum XMCharacterState
{
	CHARACTERSTATE_IDLE				,
	CHARACTERSTATE_SHOOT			,
	CHARACTERSTATE_SLASH			,
	CHARACTERSTATE_RECOVERY			,
	
};

enum XMSlashSate
{
	SLASHSTATE_NONE   =  0,
	SLASHSTATE_SLASH1 =  1,
	SLASHSTATE_SLASH2,
	SLASHSTATE_SLASH3,
};


enum XMStageZorder
{
	ZORDER_BACKGROUND		 = 0			,
	ZORDER_STAGEUI							,
	ZORDER_ENEMY							,
	ZORDER_PLAYER							,
	ZORDER_BULLET							,
	ZORDER_EFFECT							,
	ZORDER_DPAD				 = 10000		,
	ZORDER_BUTTON			 = ZORDER_DPAD  ,
	ZORDER_STAGEUI_END						,
};

enum XMCategoryBit
{
	CATEGORY_PLAYER			 = 1,
	CATEGORY_PWEAPON			,
	CATEGORY_PBULLET			,
	CATEGORY_ENEMY				,
	CATEGORY_EWEAPON			,
	CATEGORY_EBULLET			,
};

enum XMGroupIndex
{
	GROUPINDEX_PLAYER		 = 1,
	GROUPINDEX_ENEMY			,
};

enum XMShootDirection
{
	SHOOTDIRECTION_LEFT			,
	SHOOTDIRECTION_RIGHT		,
};

enum XMUiCompnentTag
{
	UI_BUTTON_SCENARIO			=	0,
	UI_BUTTON_BOSS					 ,
	UI_BUTTON_START					 ,
	UI_BUTTON_RESTART                ,
	UI_BUTTON_PREVIOUS				 ,
	UI_BUTTON_NEXT					 ,
	UI_BUTTON_CONFIRM				 ,
};

#endif  // __XMDefine_h__