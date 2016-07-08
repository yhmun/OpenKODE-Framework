/* --------------------------------------------------------------------------
 *
 *      File            APSDevice.h
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

#ifndef __APSDevice_h__
#define __APSDevice_h__

#include "APSResource.h"

NS_APS_BEGIN

class APSDevice 
{
	public :

		APSDevice ( const KDchar* szIdentifier = "", CCSize tScreenSize = CCSizeZero );
		APSDevice ( const APSDevice& tDevice );
		APSDevice ( APSDictionary* pProperties );

		APS_CLASS_NAME ( APSDevice );

	public :

		virtual KDbool initWithProperties ( APSDictionary* pProperties );

	protected :

		/**
		 *	Identifier is used to distinguish device. Also identifier is used
		 *	in setting image file names.
		 */
		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sIdentifier, Identifier );

		/** 
		 *	Screen size of device in pixel. In artpig, all points and sizes are
		 *	assumed to be pixel based. 
		 */
		APS_SYNTHESIZE_WITH_KEY ( CCSize, m_tScreenSize, ScreenSize );
	    
		/** If true, images are generated for this device. */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bEnabled, Enabled );
};

NS_APS_END

#endif	// __APSDevice_h__


