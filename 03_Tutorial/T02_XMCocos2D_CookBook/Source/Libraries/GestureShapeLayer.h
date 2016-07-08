/* --------------------------------------------------------------------------
 *
 *      File            GestureShapeLayer.h
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

#ifndef __GestureShapeLayer_h__
#define __GestureShapeLayer_h__

class GestureShapeLayer : public CCLayer
{
	public :
				 GestureShapeLayer ( KDvoid );
		virtual ~GestureShapeLayer ( KDvoid );

		static GestureShapeLayer*	create ( KDvoid );

	public :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			draw ( KDvoid );

		virtual KDvoid			setCircleRectFromPoints ( KDvoid );

		virtual CCArray*		getPoints ( KDvoid );
		virtual KDvoid			setPoints ( CCArray* pPoints );

		virtual CCArray*		getLines ( KDvoid );
		virtual KDvoid			setLines ( CCArray* pLines );

		virtual const CCRect&	getCircleRect ( KDvoid );
		virtual KDvoid			setCircleRect ( const CCRect& tCircleRect );

		virtual KDbool			 isDrawCircle ( KDvoid );
		virtual KDvoid			setDrawCircle ( KDbool bDraw );

		virtual KDbool			 isDrawLines ( KDvoid );
		virtual KDvoid			setDrawLines ( KDbool bDraw );

	protected :

		CCArray*				m_pPoints;
		CCArray*				m_pLines;
		CCRect					m_tCircleRect;
		KDbool					m_bDrawCircle;
		KDbool					m_bDrawLines;
};

#endif	// __GestureShapeLayer_h__
