/* --------------------------------------------------------------------------
 *
 *      File            CCJson.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#include "Precompiled.h"
#include "CCJson.h"
#include "XMJson/json.h"

using namespace Json;

static CCObject* valueToObject ( const Value& tRoot )
{
	CCObject*	pObject = KD_NULL;

	switch ( tRoot.type ( ) )
	{
		case objectValue :
		
			pObject = CCDictionary::create ( );

			for ( ValueIterator iter = tRoot.begin ( ); iter != tRoot.end ( ); iter++ )
			{
				const Value&	tKey  = iter.key ( );
				const KDchar*	szKey = tKey.asCString ( );

				( (CCDictionary*) pObject )->setObject ( valueToObject ( tRoot [ szKey ] ), szKey ); 
			}
			break;

		case arrayValue :

			pObject = CCArray::create ( );

			for ( ValueIterator iter = tRoot.begin ( ); iter != tRoot.end ( ); iter++ )
			{
				( (CCArray*) pObject )->addObject ( valueToObject ( *iter ) );
			}
			break;

		case nullValue		:		///< 'null' value
			break;

		case intValue		:		///< signed integer value
			pObject = CCString::create ( ccszf ( "%d", tRoot.asInt ( ) ) );
			break;

		case uintValue		:		///< unsigned integer value
			pObject = CCString::create ( ccszf ( "%d", tRoot.asUInt ( ) ) );
			break;

		case realValue		:		 ///< double value
			pObject = CCString::create ( ccszf ( "%f", tRoot.asDouble ( ) ) );
			break;

		case stringValue	:		///< UTF-8 string value
			pObject = CCString::create ( tRoot.asString ( ) );
			break;

		case booleanValue	:		///< bool value
			pObject = CCString::create ( tRoot.asBool ( ) ? "1" : "0" );
			break;
	}

	return pObject;
}

CCDictionary* CCJson::create ( const KDchar* szFilename )
{
	KDsize			uSize   = 0;
	KDubyte*		pBuffer = CCFileUtils::sharedFileUtils ( )->getFileData ( szFilename, "rb", &uSize );
	std::string		sBuffer = (KDchar*) pBuffer;
	CC_SAFE_DELETE_ARRAY ( pBuffer );

	Json::Value		tRoot;
	KDbool			bRet = Json::Reader ( ).parse ( sBuffer, tRoot );
	if ( !bRet )
	{
		return KD_NULL;
	}

	return (CCDictionary*) valueToObject ( tRoot );
}