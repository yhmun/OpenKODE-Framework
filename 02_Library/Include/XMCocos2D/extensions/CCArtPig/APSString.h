/* --------------------------------------------------------------------------
 *
 *      File            APSString.h
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

#ifndef __APSString_h__
#define __APSString_h__

#include "APSObject.h"

NS_APS_BEGIN

class APSNumber;
class APSArray;
class APSString;
class APSResourceArray;

struct APSPathFields 
{
    std::string  sLocation;
    std::string  sName;
    std::string  sExtension;
};

class APSString : public APSObject
{
	public :

		APSString ( KDvoid );

		APSString ( const APSString*   pStr    );
		APSString ( const std::string& sStr    );
		APSString ( const APSNumber*   pNumber );
		APSString ( const ccColor4B&   tColor  );
		APSString ( const CCPoint&     tPoint  );
		APSString ( const CCSize&      tSize   );
		APSString ( const CCRect&      tRect   );	 

		APS_CLASS_NAME ( APSString );

		static APSString*  stringWithString ( const APSString*   pStr    );
		static APSString*  stringWithString ( const std::string& sStr    );
		static APSString*  stringWithNumber ( const APSNumber*   pNumber );
		static APSString*  stringWithColor  ( const ccColor4B&   tColor  );
		static APSString*  stringWithPoint  ( const CCPoint&     tPoint  );
		static APSString*  stringWithSize   ( const CCSize&      tSize   );
		static APSString*  stringWithRect   ( const CCRect&      tRect   );    
	    
	public :

		// Sets APSPathFields of file's location ,filename, and extension.
		static KDvoid  getPathFields ( const std::string& sPath, APSPathFields* pPathFields );
	    
		static KDbool  hasSuffix ( const KDchar* szStr, const KDchar* szSuffix );

		static const KDchar*  getIntsFromString   ( const KDchar* szStr, KDchar cDel, KDint*   pValues, KDint nMaxNValues, KDint* nValues );
		static const KDchar*  getFloatsFromString ( const KDchar* szStr, KDchar cDel, KDfloat* pValues, KDint nMaxNValues, KDint* nValues );
	    
		KDbool   isEqualToString ( APSString* pStr ) const;
	    
		KDvoid   getInts ( std::vector<KDint>* pList ) const;
	    
		KDvoid   getFloats ( std::vector<KDfloat>* pList ) const;
	    
		KDvoid   getIndexes ( std::vector<APSIndex>* pList ) const;
	    
		KDvoid   getResourceArray ( APSResourceArray* pList ) const;
	    
		ccColor4B  getColorValue ( KDvoid ) const;
		CCPoint    getPointValue ( KDvoid ) const;
		CCSize     getSizeValue  ( KDvoid ) const;
		CCRect     getRectValue  ( KDvoid ) const;
	    
		const std::string&  getString ( KDvoid ) const;
	    
		const KDchar*  c_str ( KDvoid ) const;
	    
	protected :

		std::string		m_sString;
};

NS_APS_END

#endif	// __APSString_h__
