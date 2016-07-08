/* --------------------------------------------------------------------------
 *
 *      File            APSNumber.h
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

#ifndef __APSNumber_h__
#define __APSNumber_h__

#include "APSObject.h"

NS_APS_BEGIN

typedef union 
{
    KDint		integer;
    KDbool		boolean;
    KDuint		uInteger;
    KDint32		lInteger;
    KDuint32	ulInteger;
    KDfloat		real;
    KDdouble	lReal;
} APSNumberData;

enum 
{
    kAPSNumberType_boolean	,
    kAPSNumberType_integer	,
    kAPSNumberType_uInteger	,
    kAPSNumberType_lInteger	,
    kAPSNumberType_ulInteger,
    kAPSNumberType_real		,
    kAPSNumberType_lReal	,
};

class APSString;

extern const APSNumberData  APSZeroNumberData;

class APSNumber : public APSObject 
{
	public :

		APSNumber ( KDint nType = kAPSNumberType_integer, APSNumberData tData = APSZeroNumberData );
	    
		APS_CLASS_NAME ( APSNumber );

		static APSNumber*  numberWithBoolean		 ( KDbool			  boolean	);
		static APSNumber*  numberWithInteger		 ( KDint			  integer	);
		static APSNumber*  numberWithUnsignedInteger ( KDuint			  uInteger	);
		static APSNumber*  numberWithLong			 ( KDint32			  lInteger	);
		static APSNumber*  numberWithUnsignedLong    ( KDuint32			  ulInteger );
		static APSNumber*  numberWithFloat		     ( KDfloat			  fReal		);
		static APSNumber*  numberWithDouble		     ( KDdouble			  lReal		);
		static APSNumber*  numberWithBooleanString   ( const std::string& sStr		);
	    
		static APSNumber*  numberWithIndex  ( APSIndex   uIndex  );
		static APSIndex    indexWithNumber  ( APSNumber* pNumber );
		static KDuint      integerFromIndex ( APSIndex   uIndex  );
		static APSIndex    indexFromInteger ( KDuint     uVal	 );

	public :

		KDbool  isKindOfBoolean ( KDvoid ) const;
		KDbool  isKindOfInteger ( KDvoid ) const;
		KDbool  isKindOfReal    ( KDvoid ) const;
	    
		KDbool      getBoolean		   ( KDvoid ) const;
		KDint       getInteger		   ( KDvoid ) const;
		KDuint      getUnsignedInteger ( KDvoid ) const;
		KDint32     getLong			   ( KDvoid ) const;
		KDuint32    getUnsignedLong	   ( KDvoid ) const;
		KDfloat     getFloat		   ( KDvoid ) const;
		KDdouble    getDouble		   ( KDvoid ) const;	    
		std::string getString		   ( KDvoid ) const;
	    
	protected :

		APS_SYNTHESIZE ( KDint, m_nType, Type );

	protected :

		APSNumberData		m_tData;
};

NS_APS_END

#endif	// __APSNumber_h__
