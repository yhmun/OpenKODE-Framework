/* --------------------------------------------------------------------------
 *
 *      File            RopeModel.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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

#ifndef __RopeModel_h__
#define __RopeModel_h__

#include "AbstractModel.h"

class RopeModel : public AbstractModel
{
	public :

		RopeModel ( KDvoid );
	    
	public :

		// The position of each of the rope ends.
		// If an end is connected to a pineapple, then this property is ignored
		// and the position of the pineapple is used instead.
		CCPoint			m_tAchorA;
		CCPoint			m_tAchorB;
	    
		// ID of the body the rope is connected to. -1 refers to the background.
		// all other IDs refer to pineapples distributed in the level
		KDint			m_nBodyAId;
		KDint			m_nBodyBId;
	    
		// The sagginess of the line
		KDfloat			m_fSagity;
};

#endif	// __RopeModel_h__
