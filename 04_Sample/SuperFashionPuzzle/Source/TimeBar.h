/* --------------------------------------------------------------------------
 *
 *      File            TimeBar.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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

#ifndef __TimeBar_h__
#define __TimeBar_h__

class PlayingScene;

// Timebar, es un nodo que contiene 3 sprites que simulan una barra de tiempo
// Tamaño: 35*320
// Posición con tablero a la derecha: 125+35*0.5
// Posición con el tablero a la izquierda: 320-125-35*0.5
class TimeBar : public CCNode 
{
	public :

		static TimeBar*		create ( PlayingScene* pPlayingScene );

	public :

		virtual KDbool		initWithPlayingScene ( PlayingScene* pPlayingScene );

		virtual KDvoid		update ( KDfloat fDelta );

		KDvoid				activate ( KDvoid );

		KDvoid				deactivate ( KDvoid );

		KDvoid				reset ( KDvoid );

		KDvoid				addSomeTime ( KDfloat fAmount );

		KDvoid				removeSomeTime ( KDfloat fAmount );

		KDvoid				setDuration ( KDfloat fDuration );

	private :

		KDfloat				m_fLeftTime;
		CCSprite*			m_pTimeBarSprite;
		KDbool				m_bIsActivated;
		PlayingScene*		m_pPlayingScene;
		KDfloat				m_fDuration;
};

#endif	// __TimeBar_h__
