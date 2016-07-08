/* --------------------------------------------------------------------------
 *
 *      File            TestJson.cpp
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
#include "TestJson.h"
#include "XMJson/json.h"

KDbool TestJson::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	m_pLabel = CCLabelTTF::create 
	(
		"Touch the screen to test", "fonts/Marker Felt.ttf", 32, CCSizeZero, kCCTextAlignmentLeft 
	);	
	this->addChild ( m_pLabel );
	m_pLabel->setPositionWithParent ( kCCAlignmentCenter );

    this->setTouchEnabled ( KD_TRUE );  

	return KD_TRUE;
}

KDvoid TestJson::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	Encoding ( );

	Decoding ( );
}

KDvoid TestJson::Encoding ( KDvoid )
{
	Json::Value			tRoot; 
	Json::Value			tEncoding;  
	Json::Value			tPlugins;
	Json::Value			tIndent; 
	Json::StyledWriter  tWriter;  
	std::string			sOutput;
	KDFile*				pFile;
  
	tPlugins.append ( "python" ); 
	tPlugins.append ( "c++"    ); 
	tPlugins.append ( "ruby"   ); 

	tIndent["length"   ] = 3;  
	tIndent["use_space"] = true; 
	
	tRoot["encoding"] = "UTF-8"; 
	tRoot["plug-ins"] = tPlugins; 
	tRoot["indent"  ] = tIndent; 

	sOutput = tWriter.write ( tRoot );
	kdLogMessage ( sOutput.c_str ( ) );

	pFile = kdFopen ( "/data/Test.json", "wb" );
	kdFwrite ( sOutput.c_str ( ), sOutput.size ( ), 1, pFile );
	kdFclose ( pFile );
}

KDvoid TestJson::Decoding ( KDvoid )
{
	KDFile*			pFile;
	KDsize			uSize;
	KDchar*			pBuffer;
	KDbool			bRet;
	Json::Value		tRoot;
	Json::Reader	tReader;
	std::string		sOutput;
	KDchar          szTemp [256];
	
	pFile = kdFopen ( "/data/Test.json", "rb" );

	kdFseek ( pFile, 0, KD_SEEK_END );
	uSize = kdFtell ( pFile );
	kdFseek ( pFile, 0, KD_SEEK_SET );

	pBuffer = new KDchar [ uSize ];
	kdFread ( pBuffer, uSize, 1, pFile );

	kdFclose ( pFile );

	sOutput = "";
	bRet = tReader.parse ( std::string ( pBuffer ), tRoot );
	if ( bRet )
	{
		const Json::Value  tPlugins = tRoot["plug-ins"];

		sOutput = "plug-ins = ";
		for ( KDuint  i = 0; i < tPlugins.size ( ); ++i )
		{
			sOutput += tPlugins [ i ].asString ( );
			sOutput += " ";
		}
		sOutput += "\n";

		kdSprintfKHR ( szTemp, "indent : length = %d\n", tRoot["indent"].get ( "length", 0 ).asInt ( ) );
		sOutput += szTemp;
		
		kdSprintfKHR ( szTemp, "indent : use_space = %s", tRoot["indent"]["use_space"].asBool ( ) ? "true" : "false" );
		sOutput += szTemp;
	}
	else
	{
		sOutput = "Failed to parse json";
	}

	m_pLabel->setString ( sOutput.c_str ( ) );

	delete [] pBuffer;
}
