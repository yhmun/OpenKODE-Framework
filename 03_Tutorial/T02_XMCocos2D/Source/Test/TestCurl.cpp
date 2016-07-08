/* --------------------------------------------------------------------------
 *
 *      File            TestCurl.cpp
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
#include "XMCurl/curl.h"
#include "TestCurl.h"

KDbool TestCurl::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	m_pLabel = CCLabelTTF::create 
	(
		"Touch the screen to connect", "fonts/Marker Felt.ttf", 32, CCSizeZero, kCCTextAlignmentLeft 
	);	
	this->addChild ( m_pLabel );
	m_pLabel->setPositionWithParent ( kCCAlignmentCenter );

    this->setTouchEnabled ( KD_TRUE );  

	return KD_TRUE;
}

KDvoid TestCurl::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CURL*     pCurl;
	CURLcode  eResponse;
	KDchar    szStr[10];

	pCurl = curl_easy_init ( );
	if ( pCurl ) 
	{
		curl_easy_setopt ( pCurl, CURLOPT_URL, "google.com" );

		eResponse = curl_easy_perform ( pCurl );

		curl_easy_cleanup ( pCurl );

		if ( eResponse == CURLE_OK )
		{
			m_pLabel->setString ( "0 response" );
		}
		else
		{
			kdSprintfKHR ( szStr, "code : %i", eResponse );
			m_pLabel->setString ( szStr );
		}
	} 
	else 
	{
		m_pLabel->setString ( "no curl" );
	} 
}
