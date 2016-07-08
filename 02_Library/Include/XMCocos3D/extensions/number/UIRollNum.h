/* --------------------------------------------------------------------------
 *
 *      File            UIRollNum.h
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

#ifndef __UIRollNum_h__
#define __UIRollNum_h__

#include "UINumber.h"

class UIRollNum : public CCSprite
{
	public :
				 UIRollNum ( KDvoid );
		virtual ~UIRollNum ( KDvoid );

		CREATE_FUNC ( UIRollNum );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		rebuildEffect ( KDvoid );
		
		virtual KDvoid		clearEffect ( KDvoid );

		virtual KDint		getNumber ( KDvoid );
		virtual KDvoid		setNumber ( KDint nNumber );

	private :

		KDint				m_nNumber;
		KDint				m_nMaxCol;
		CCArray*			m_pNumArray;
		CCPoint				m_tPoint;
		KDbool				m_bZeroFill;
		NumStyle			m_eStyle;
};

#endif	// __UIRollNum_h__