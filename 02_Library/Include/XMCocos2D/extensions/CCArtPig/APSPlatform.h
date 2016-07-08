/* --------------------------------------------------------------------------
 *
 *      File            APSPlatform.h
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

#ifndef __APSPlatform_h__
#define __APSPlatform_h__

#include "APSSharedType.h"

NS_APS_BEGIN

class APSPlatform
{
	public :
	    
		APSPlatform ( KDvoid );
	       
		/** Returns a shared instance of the APSPlatform object. */
		static APSPlatform*   sharedPlatform ( KDvoid );

		/** Returns the full pathname of the directory containing resources. */
		static const KDchar*  getResourcePathForFile ( const KDchar* szName );
	    
	public :

		CCPoint  convertPointFromPixels ( const CCPoint& tPoint );    
		CCPoint  convertPointToPixels   ( const CCPoint& tPoint );
	    
		CCSize   convertSizeFromPixels ( const CCSize& tSize );
		CCSize   convertSizeToPixels   ( const CCSize& tSize );
	    
		APS_SYNTHESIZE ( CCSize , m_tWinSize, WinSize )
		APS_SYNTHESIZE ( KDfloat, m_fScaleFactor, ScaleFactor )
    
};

NS_APS_END

#endif	// __APSPlatform_h__
