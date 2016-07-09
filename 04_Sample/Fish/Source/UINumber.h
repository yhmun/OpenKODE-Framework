/* --------------------------------------------------------------------------
 *
 *      File            UINumber.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      周海锋 on 12-04-08
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      CJLU. All rights reserved.
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

#ifndef __UINumber_h__
#define __UINumber_h__

#define NUM_HEIGHT		20
#define NUM_WIDTH		20

enum NumStyle
{
    NumStyleNormal		,
    NumStyleSameTime	,
};

class UINumber : public CCSprite 
{
	public :

		static UINumber*	create ( NumStyle eStyle );

	public :

		virtual KDbool		initWithStyle ( NumStyle eStyle );

		virtual KDvoid		setNumber ( KDint nNumer );
		
		virtual KDvoid		onRollDown ( KDfloat fDelta );
		
		virtual KDvoid		onRollUP ( KDfloat fDelta );

		virtual KDvoid		setup ( KDvoid );

	protected :

		NumStyle			m_eStyle;     
		KDint				m_nNum;           
		KDint				m_nPosCur;       
		KDint				m_nPosEnd;       
		KDint				m_nMoveLen;        
		CCTexture2D*		m_pTexture; 
};

#endif	// __UINumber_h__