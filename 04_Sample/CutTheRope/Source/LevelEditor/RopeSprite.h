/* --------------------------------------------------------------------------
 *
 *      File            RopeSprite.h
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

#ifndef __RopeSprite_h__
#define __RopeSprite_h__

#include "../Constants.h"
#include "../Model/RopeModel.h"


class RopeSprite : public CCObject
{
	public :
				 RopeSprite ( CCNode* pParent, RopeModel* pRopeModel );
		virtual ~RopeSprite ( KDvoid );

	public :

		// logic
		KDint			getID ( KDvoid );

		CCPoint			getAnchorA ( KDvoid );
		CCPoint			getAnchorB ( KDvoid );

		KDint			getBodyAID ( KDvoid );
		KDint			getBodyBID ( KDvoid );

		KDvoid			setAnchorA ( CCPoint tPos );
		KDvoid			setAnchorB ( CCPoint tPos );

		KDvoid			updateRope ( KDvoid );

		KDfloat			distanceToPoint ( CCPoint tPos );

		CCRect			getBoundingBox ( KDvoid );

		KDvoid			cleanupSprite ( KDvoid );

		KDvoid			setSelectedAnchorType ( EAnchorType eType );
		EAnchorType		getSelectedAnchorType ( KDvoid );

		CCPoint			getSelectedAnchor ( KDvoid );

		KDvoid			moveSelectedAnchorTo ( CCPoint tVector );

		KDbool			isValideNewAnchorID ( KDint nNewAnchorID );

	private :

		CCSprite*		m_pRopeSprite;
		RopeModel*		m_pRopeModel;
		EAnchorType		m_eSelectedAnchor;
};

#endif	// __RopeSprite_h__
