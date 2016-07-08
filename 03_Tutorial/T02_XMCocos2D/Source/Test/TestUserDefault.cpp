/* --------------------------------------------------------------------------
 *
 *      File            TestUserDefault.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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
#include "TestUserDefault.h"

KDbool TestUserDefault::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&   tLyrSize = this->getContentSize ( );
	CCUserDefault*  pUserDefault = CCUserDefault::sharedUserDefault ( );

	CCLabelTTF*  pLabel = CCLabelTTF::create 
	(
		ccszf ( "\'Save\' key value = %s",  pUserDefault->getStringForKey ( "Save", "Empty" ).c_str ( ) ),
		"fonts/Marker Felt.ttf", 24 
	);
	this->addChild ( pLabel );
	pLabel->setColor ( ccRED );
	pLabel->setPosition ( ccp ( tLyrSize.cx / 2, 100 ) );

	for ( KDuint  i = 0; i < 2; i++ )
	{
		std::string   sOutput;

		if ( i == 0 )
		{
			pUserDefault->setStringForKey	( "string" , "value1" );
			pUserDefault->setIntegerForKey	( "integer", 10		  );
			pUserDefault->setFloatForKey	( "float"  , 2.3f	  );
			pUserDefault->setDoubleForKey	( "double" , 2.4	  );
			pUserDefault->setBoolForKey		( "bool"   , KD_TRUE  );

			sOutput = "* Init value\n\n";
		}
		else
		{
			pUserDefault->setStringForKey   ( "Save"    , "Ok" );
			pUserDefault->setStringForKey	( "string"	, "value2"	);
			pUserDefault->setIntegerForKey	( "integer"	, 11		);
			pUserDefault->setFloatForKey	( "float"	, 2.5f		);
			pUserDefault->setDoubleForKey	( "double"	, 2.6		);
			pUserDefault->setBoolForKey		( "bool"	, KD_FALSE	);

			pUserDefault->flush ( );

			sOutput = "* After change value\n\n";
		}

		sOutput += ccszf ( "string is %s\n" , pUserDefault->getStringForKey ( "string"  ).c_str ( ) );
		sOutput += ccszf ( "double is %f\n" , pUserDefault->getDoubleForKey ( "double"  ) );
		sOutput += ccszf ( "integer is %d\n", pUserDefault->getFloatForKey  ( "integer" ) );
		sOutput += ccszf ( "float is %f\n"  , pUserDefault->getDoubleForKey ( "float"   ) );
		sOutput += ccszf ( "bool is %s\n"   , pUserDefault->getBoolForKey   ( "bool"    ) ? "true" : "false" );

		CCLabelTTF*  pLabel = CCLabelTTF::create 
		(
			sOutput.c_str ( ), "fonts/Marker Felt.ttf", 24 
		);	
		this->addChild ( pLabel );
		pLabel->setPosition ( ccp ( tLyrSize.cx / 2 + ( i == 0 ? - 200 : + 200 ), tLyrSize.cy / 2 - 20 ) );
	}

	return KD_TRUE;
}

const KDchar* TestUserDefault::subtitle ( KDvoid )
{
	return ccszf ( "See \'UserDefault.xml\' file in Data Folder." );
}