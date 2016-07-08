/* --------------------------------------------------------------------------
 *
 *      File            APSResource.h
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

#ifndef __APSResource_h__
#define __APSResource_h__

#include "APSElements.h"
#include "APSSharedType.h"
#include <vector>
#include <set>
#include <map>

NS_APS_BEGIN

class APSResource;
class APSMedium;
class APSResourceArray;
class APSSymbol;

class  APSResource : public CCObject
{
	public :

		/**
		 *	Constructor of APSResource. If 'properties' parameter is not NULL, the 
		 *	instance is initialized by the properties.
		 */
		APSResource ( APSDictionary* pProperties = KD_NULL );    
	    
		APS_CLASS_NAME ( APSResource );

	public :

		/** Initializes this APSResource object with given properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );

		/** Minimizes memory usage in this resource. */
		virtual KDvoid  clearCachedData ( KDvoid ) { };
	    
		/** Preloads data before it is used. */
		virtual KDvoid  preloadData ( KDvoid ) { };
	        	    
		/**
		 *	Finds and returns a resource from parent symbol with a given index.
		 *	If there is no resource with the index, NULL is returned.
		 *	With 0 index value, parent symbol is returned.
		 *	@warning parentSymbol property must be set.
		 */
		APSResource*  getResource ( KDuint uIndex ) const;
	    
	protected :

		/** 
		 *	Returns an APSMedium object with a given code.
		 *	@warning  To use this method, parentSymbol property must be set and the 
		 *	symbol must be created by
		 *	APSResourceManager.
		 */
		APSMedium*  getMediumForCode ( const std::string& sCode );
	        	    
		/** 
		 *	Tag is a unique and identifiable value that is specified in 
		 *	ArtPigEditor. With tag property,
		 *	programmers can access an APSResource object. 
		 */
		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sTag, Tag );
	    
		/**
		 *	An index of this resource in parent symbol's resources list
		 */
		APS_SYNTHESIZE_WITH_KEY ( APSIndex, m_uIndex, Index );
	    
		/** An APSSymbol object containing this resource. */
		APS_SYNTHESIZE_WITH_KEY ( APSSymbol*, m_pParentSymbol, ParentSymbol );

		friend class APSResourceArray;
};

NS_APS_END

#endif	// __APSResource_h__
