/* --------------------------------------------------------------------------
 *
 *      File            KawazTan.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/06/14 
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

#ifndef __KawazTan_h__
#define __KawazTan_h__

typedef enum
{
  KawazTanStateNormal		, 
  KawazTanStateWaiting		,    
  KawazTanStateMoving		,     
  KawazTanStateDamaged		,    
  KawazTanStateInvinsible	, 
  KawazTanStateDisable		,   
} KawazTanState;

typedef enum
{
  KawazTanTypeNormal		, 
  KawazTanTypeSmile			,  
  KAwazTanTypeDeride		, 
  KawazTanTypeBomb			,    
} KawazTanType;

class KWSprite;

class KawazTan : public KWSprite
{
	public :

		static KawazTan*	create ( const CCPoint& tPosition );

	public :

		virtual KDbool		initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect );

		virtual KDbool		initWithPosition ( const CCPoint& tPosition );

		KDbool				start ( KDfloat fWaitTime );

		KDbool				isMoving ( KDvoid );

		KDbool				tab ( KDvoid );

		KDbool				canTouch ( KDvoid );

		KawazTanType		getType ( KDvoid );

		KDint				getScore ( KDvoid );

	private :

		KDvoid				onBacked ( KDvoid );

		KDvoid				onChangeStateNormal ( KDvoid );
		KDvoid				onChangeStateMoving ( KDvoid );

	private :

	  KawazTanType			m_eType;   
	  KawazTanState			m_eState; 
	  KDint					m_nScore;          
};

#endif	// __KawazTan_h__