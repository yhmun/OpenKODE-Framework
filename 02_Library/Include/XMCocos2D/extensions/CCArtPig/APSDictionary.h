/* --------------------------------------------------------------------------
 *
 *      File            APSDictionary.h
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

#ifndef __APSDictionary_h__
#define __APSDictionary_h__

#include "APSObject.h"

NS_APS_BEGIN

typedef std::map<const std::string, APSObject*>  APSDictionaryStorage;

class APSDictionary : public APSObject 
{
	public :

		virtual ~APSDictionary ( KDvoid );
	    
		APS_CLASS_NAME ( APSDictionary );

		/** Creates autoreleased APSDictionary object */
		static APSDictionary*  dictionary ( KDvoid );
	    
	public :

		KDvoid      setObjectForKey ( APSObject* pObject, const std::string& sKey );
		APSObject*  getObjectForKey ( const std::string& sKey );
	    
		// Adds the entries from another dictionary.
		KDvoid  addEntriesFromDictionary ( const APSDictionary* pOtherDictionary );
	    
		KDvoid  removeForKey ( const std::string& sKey );
	    
		KDvoid  clear ( KDvoid );
	    
		APSDictionaryStorage::iterator  begin ( KDvoid ) { return this->m_aTable.begin ( ); }
		APSDictionaryStorage::iterator  end   ( KDvoid ) { return this->m_aTable.end   ( ); }
	    
	protected :

		APSDictionaryStorage    m_aTable;
};

NS_APS_END

#endif	// __APSDictionary_h__
