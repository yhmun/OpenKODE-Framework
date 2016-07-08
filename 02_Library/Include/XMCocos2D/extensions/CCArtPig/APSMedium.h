/* --------------------------------------------------------------------------
 *
 *      File            APSMedium.h
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

#ifndef __APSMedium_h__
#define __APSMedium_h__

#include "APSElements.h"
#include "APSResource.h"

NS_APS_BEGIN

class APSSymbol;
class APSDevice;
class APSAction;
class APSResourceManager;

class APSMedium : public APSResource
{
	public :

		APSMedium ( const KDchar* szCode = "", APSResourceManager* pManager = KD_NULL );
		APSMedium ( APSDictionary* pProperties );
	    
		APS_CLASS_NAME ( APSMedium );

	public :

		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		virtual KDvoid  preloadData     ( KDvoid ) { };           // preloads data before it is used.
		virtual KDvoid  clearCachedData ( KDvoid ) { };           // Minimizes memory usage.
    
		KDvoid  setResourceManager ( APSResourceManager* pManager );

		APSResourceManager*  getResourceManager ( KDvoid ) const;
	    
	protected :

		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sCode, Code );
		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sFileName, Filename );

	protected :

		APSResourceManager*		m_pResourceManager;
};

NS_APS_END

#endif	// __APSMedium_h__
