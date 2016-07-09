/* --------------------------------------------------------------------------
 *
 *      File            EnergyBar.h
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

#ifndef __EnergyBar_h__
#define __EnergyBar_h__


class EnergyBar : public CCNode 
{
	public :

		static	EnergyBar*	create ( const CCSize& tSize, KDfloat fMaxVal );

	public :

		virtual KDbool		initWithMaxSize ( const CCSize& tSize, KDfloat fMaxVal );

		virtual KDvoid		draw ( KDvoid );

		virtual KDvoid		updateBar ( KDfloat fCurVal );

	protected :

		KDfloat				m_fMaxValue;
		KDfloat				m_fCurValue;
		CCSize				m_tMaxSize;		// 에너지 바의 픽셀 크기
};


#endif	// __EnergyBar_h__