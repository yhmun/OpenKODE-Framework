/* --------------------------------------------------------------------------
 *
 *      File            APSXMLParser.h
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

#ifndef __APSXMLParser_h__
#define __APSXMLParser_h__

#include "APSElements.h"

NS_APS_BEGIN

extern const KDchar*  APXML_STRING_KEY;
extern const KDchar*  APXML_BOOLEAN_KEY;
extern const KDchar*  APXML_INTEGER_KEY;
extern const KDchar*  APXML_REAL_KEY;
extern const KDchar*  APXML_ARRAY_KEY;
extern const KDchar*  APXML_DICTIONARY_KEY;
extern const KDchar*  APXML_DICTIONARY_ITEM_NAME_KEY;

/**
 *	@brief	APSXMLParser is a utility class that loads XML file in dictionary property. 
 */
class APSXMLParser
{
	public :

		         APSXMLParser ( APSDictionary* pDictionary = KD_NULL, KDfloat fVersion = 1.0f );
		virtual ~APSXMLParser ( KDvoid );
	    
		APS_CLASS_NAME ( APSXMLParser );

	public :

		/**
		 *	Loads XML with a given file and saves in dictionary property.
		 */
		virtual KDbool  loadFile ( const KDchar* szPath );  

		/** Loaded objects are stored in dictionary property. */
		APS_PROPERTY ( APSDictionary*, m_pDictionary, Dictionary );
	    
		/** XML file version defined by ArtPig. */
		APS_SYNTHESIZE ( KDfloat, m_fVersion, Version );
};

NS_APS_END

#endif	// __APSXMLParser_h__
