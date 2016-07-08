/* --------------------------------------------------------------------------
 *
 *      File            APSAnchor.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __APSAnchor_h__
#define __APSAnchor_h__

#include "APSElements.h"

NS_APS_BEGIN

class APSAnchor 
{
	public :

		APSAnchor ( CCPoint tPosition = CCPointZero, CCPoint tBackHandle = CCPointZero, CCPoint tFrontHandle = CCPointZero, KDfloat fProportion = 1.f );
		APSAnchor ( APSDictionary* pProperties );
		APSAnchor ( const APSAnchor& tAnchor );	    	

		APS_CLASS_NAME ( APSAnchor );
	    
	public :
	    
		/** Initializes APSAnchor object with given properties. */
		virtual KDbool		initWithProperties ( APSDictionary* pProperties );
	    
		// Returns position of the anchor, scaled by a given scale value.
		CCPoint				getPosition ( KDfloat fScale );
	    
		// Returns back handle point of the anchor, scaled by a given scale value.
		CCPoint				getBackHandle ( KDfloat fScale );
	    
		// Returns front handle point of the anchor, scaled by a given scale value.
		CCPoint				getFrontHandle ( KDfloat fScale );

	protected :

		/** Position of anchor. */
		APS_SYNTHESIZE_WITH_KEY ( CCPoint, m_tPosition, Position );
	    
		/** Position of back handle. */
		APS_SYNTHESIZE_WITH_KEY ( CCPoint, m_tBackHandle, BackHandle );
	    
		/** Position of front handle. */
		APS_SYNTHESIZE_WITH_KEY ( CCPoint, m_tFrontHandle, FrontHandle );
	    
		/** 
		 *	Length proportion from anchor's position to front handle assuming that
		 *	whole length of bezier path is 1.
		 */
		APS_SYNTHESIZE_WITH_KEY ( KDfloat, m_fProportion, Proportion );
};

NS_APS_END

#endif	// __APSAnchor_h__
