/* --------------------------------------------------------------------------
 *
 *      File            Controller.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

#ifndef __Controller_h__
#define __Controller_h__

enum
{
    CMD_ROTATE_LEFT			= 0	,
    CMD_MOVE_FORWARD			,
    CMD_ROTATE_RIGHT			,
    CMD_MOVE_STRAFE_LEFT		,
    CMD_MOVE_STRAFE_RIGHT		,
    CMD_MOVE_BACKWARD			,
    CMD_JUMP					,
    CMD_FIRE					,    
    CMD_ESCAPE					,
};

class CDemo;

class Controller : public CCLayer
{
	public :		

		CREATE_FUNC ( Controller );

		static  KDvoid  EventProc ( const KDEvent* event );

		static  KDvoid	DispatchToDemo ( KDint nCommand, KDbool bPress );

	protected :
		
		virtual KDbool  init ( KDvoid ); 

		virtual KDvoid  draw ( KDvoid );

	private :

		KDvoid  onClose ( CCObject* pSender );
    
    public :
    
        CDemo*  s_pDemo;
};

class RollerOverCheck : public CCMenuItemImage
{
	public :

		static   RollerOverCheck*  create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, KDint nTag );

	protected :

		virtual KDvoid  selected   ( KDvoid );
		virtual KDvoid  unselected ( KDvoid );
};
#endif 
