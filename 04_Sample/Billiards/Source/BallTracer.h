/* --------------------------------------------------------------------------
 *
 *      File            BallTracer.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Matsumae Kentaro on 12/03/24
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

#ifndef __BallTracer_h__
#define __BallTracer_h__

class BallTracer : public CCNode
{
	public :

		static BallTracer*		create ( const CCPoint& tTouchLoaction, const CCPoint& tBallLocation );

	public :

		virtual KDbool			initWithTouchLocation ( const CCPoint& tTouchLoaction, const CCPoint& tBallLocation );

		virtual KDvoid			update ( KDfloat fDelta );

		virtual KDvoid			setTouchLocation ( const CCPoint& tTouchLoaction );

	private :

		KDvoid					addBallShadow ( KDvoid );

	protected :

	    CCSpriteBatchNode*		m_pBatch;
	    KDfloat					m_fBallWidth;
		CCPoint					m_tBallLocation;
		CCPoint					m_tTouchLocation;
};

#endif	// __BallTracer_h__