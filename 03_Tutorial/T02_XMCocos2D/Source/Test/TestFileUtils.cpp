/* --------------------------------------------------------------------------
 *
 *      File            TestFileUtils.cpp
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
#include "TestFileUtils.h"

TestFileUtils* TestFileUtils::create ( KDvoid )
{
	TestFileUtils*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new TestResolutionDirectories	( ); break;
		case  1 : pLayer = new TestSearchPath				( ); break;
        case  2 : pLayer = new TestFilenameLookup           ( ); break;
		case  3 : pLayer = new TestIsFileExist				( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestFileUtils::count ( KDvoid )
{
	return 4;
}

//------------------------------------------------------------------
//
// TestResolutionDirectories
//
//------------------------------------------------------------------
KDvoid TestResolutionDirectories::onEnter ( KDvoid )
{
	TestFileUtils::onEnter ( );

    CCFileUtils*	pSharedFileUtils = CCFileUtils::sharedFileUtils ( );
	std::string		sRet;
    
    pSharedFileUtils->purgeCachedEntries ( );
    m_aDefaultSearchPathArray = pSharedFileUtils->getSearchPaths ( );
	std::vector<std::string>  aSearchPaths = m_aDefaultSearchPathArray;
    aSearchPaths.insert ( aSearchPaths.begin ( ), "Misc" );
    pSharedFileUtils->setSearchPaths ( aSearchPaths );
    
    m_aDefaultResolutionsOrderArray = pSharedFileUtils->getSearchResolutionsOrder ( );
	std::vector<std::string>  aResolutionsOrder = m_aDefaultResolutionsOrderArray;

    aResolutionsOrder.insert ( aResolutionsOrder.begin ( )    , "resources-ipadhd" );
    aResolutionsOrder.insert ( aResolutionsOrder.begin ( ) + 1, "resources-ipad"   );
    aResolutionsOrder.insert ( aResolutionsOrder.begin ( ) + 2, "resources-widehd" );
    aResolutionsOrder.insert ( aResolutionsOrder.begin ( ) + 3, "resources-wide"   );
    aResolutionsOrder.insert ( aResolutionsOrder.begin ( ) + 4, "resources-hd"     );
    aResolutionsOrder.insert ( aResolutionsOrder.begin ( ) + 5, "resources-iphone" );
    
    pSharedFileUtils->setSearchResolutionsOrder ( aResolutionsOrder );
    
    for ( KDint i = 1; i < 7; i++ )
	{
        CCString*  pFilename = CCString::createWithFormat ( "test%d.txt", i );
        sRet = pSharedFileUtils->fullPathForFilename ( pFilename->getCString ( ) );
        CCLOG ( "%s -> %s", pFilename->getCString ( ), sRet.c_str ( ) );
    }
}

KDvoid TestResolutionDirectories::onExit ( KDvoid )
{
    CCFileUtils*	pSharedFileUtils = CCFileUtils::sharedFileUtils ( );
    
	// reset search path
	pSharedFileUtils->setSearchPaths ( m_aDefaultSearchPathArray );
    pSharedFileUtils->setSearchResolutionsOrder ( m_aDefaultResolutionsOrderArray );

	TestFileUtils::onExit ( );
}

const KDchar* TestResolutionDirectories::title ( KDvoid )
{
	return "FileUtils: resolutions in directories";
}

const KDchar* TestResolutionDirectories::subtitle ( KDvoid )
{
	return "See the console";
}

//------------------------------------------------------------------
//
// TestSearchPath
//
//------------------------------------------------------------------
KDvoid TestSearchPath::onEnter ( KDvoid )
{
	TestFileUtils::onEnter ( );

	CCFileUtils*	pSharedFileUtils = CCFileUtils::sharedFileUtils ( );
    
	std::string		sRet;
    
    pSharedFileUtils->purgeCachedEntries ( );
    m_aDefaultSearchPathArray = pSharedFileUtils->getSearchPaths ( );
	std::vector<std::string>  aSearchPaths  = m_aDefaultSearchPathArray;
	std::string				  sWritablePath = pSharedFileUtils->getWritablePath ( );
	std::string				  sFileName     = sWritablePath + "external.txt";
    KDchar					  szBuf[100]    = "Hello Cocos2d-x!";
    KDFile*  pFile = kdFopen ( sFileName.c_str ( ), "wb" );
    if ( pFile )
    {
        kdFwrite ( szBuf, 1, kdStrlen ( szBuf ), pFile );
        kdFclose ( pFile );
        CCLOG ( "Writing file to writable path succeed." );
    }
    
    aSearchPaths.insert ( aSearchPaths.begin ( )    , sWritablePath );
    aSearchPaths.insert ( aSearchPaths.begin ( ) + 1, "Misc/searchpath1" );
    aSearchPaths.insert ( aSearchPaths.begin ( ) + 2, "Misc/searchpath2" );
    pSharedFileUtils->setSearchPaths ( aSearchPaths );
    
    m_aDefaultResolutionsOrderArray = pSharedFileUtils->getSearchResolutionsOrder ( );
	std::vector<std::string>  aResolutionsOrder = m_aDefaultResolutionsOrderArray;
    
    aResolutionsOrder.insert ( aResolutionsOrder.begin ( ), "resources-ipad" );
    pSharedFileUtils->setSearchResolutionsOrder ( aResolutionsOrder );
    
    for ( KDint i = 1; i < 3; i++ )
	{
        CCString*  pFilename = CCString::createWithFormat ( "file%d.txt", i );
        sRet = pSharedFileUtils->fullPathForFilename ( pFilename->getCString ( ) );
        CCLOG ( "%s -> %s", pFilename->getCString ( ), sRet.c_str ( ) );
    }
    
    // Gets external.txt from writable path
	std::string  sFullPath = pSharedFileUtils->fullPathForFilename ( "external.txt" );
    CCLOG ( "external file path = %s", sFullPath.c_str ( ) );
    if ( sFullPath.length ( ) > 0 )
    {
        pFile = kdFopen ( sFullPath.c_str ( ), "rb" );
        if ( pFile )
        {
            KDchar  szReadBuf[100] = {0};
            KDint   nRead = kdFread ( szReadBuf, 1, kdStrlen ( szBuf ), pFile );
            if ( nRead > 0 )
			{
                CCLOG ( "The content of file from writable path: %s", szReadBuf );
			}
            kdFclose ( pFile );
        }
    }
}

KDvoid TestSearchPath::onExit ( KDvoid )
{
	CCFileUtils*	pSharedFileUtils = CCFileUtils::sharedFileUtils ( );

	// reset search path
	pSharedFileUtils->setSearchPaths ( m_aDefaultSearchPathArray );
    pSharedFileUtils->setSearchResolutionsOrder ( m_aDefaultResolutionsOrderArray );

	TestFileUtils::onExit ( );
}

const KDchar* TestSearchPath::title ( KDvoid )
{
	return "FileUtils: search path";
}

const KDchar* TestSearchPath::subtitle ( KDvoid )
{
	return "See the console";
}


//------------------------------------------------------------------
//
// TestFilenameLookup
//
//------------------------------------------------------------------

KDvoid TestFilenameLookup::onEnter ( KDvoid )
{
	TestFileUtils::onEnter ( );

    CCFileUtils*	pSharedFileUtils = CCFileUtils::sharedFileUtils ( );

    CCDictionary*	pDict = CCDictionary::create ( );
    pDict->setObject ( CCString::create ( "Images/grossini.png" ), "grossini.bmp" );
    pDict->setObject ( CCString::create ( "Images/grossini.png" ), "grossini.xcf" );
    
    pSharedFileUtils->setFilenameLookupDictionary ( pDict );
    
    
    // Instead of loading carlitos.xcf, it will load grossini.png
    CCSprite*		pSprite = CCSprite::create ( "grossini.xcf" );
    this->addChild ( pSprite );
    
	CCSize			s = this->getContentSize ( );
    pSprite->setPosition ( ccpMid ( s ) );
}

KDvoid TestFilenameLookup::onExit ( KDvoid )
{
	CCFileUtils*  pSharedFileUtils = CCFileUtils::sharedFileUtils ( );
	
	// reset filename lookup
    pSharedFileUtils->setFilenameLookupDictionary ( CCDictionary::create ( ) );

	TestFileUtils::onExit ( );
}

const KDchar* TestFilenameLookup::title ( KDvoid )
{
	return "FileUtils: filename lookup";
}

const KDchar* TestFilenameLookup::subtitle ( KDvoid )
{
	return "See the console";
}


//------------------------------------------------------------------
//
// TestIsFileExist
//
//------------------------------------------------------------------

KDvoid TestIsFileExist::onEnter ( KDvoid )
{
	TestFileUtils::onEnter ( );

	CCSize			s = this->getContentSize ( );

    CCFileUtils*	pSharedFileUtils = CCFileUtils::sharedFileUtils ( );
    CCLabelTTF*		pTTF = KD_NULL;
    KDbool			bIsExist = KD_FALSE;
    
    bIsExist = pSharedFileUtils->isFileExist ( "Images/grossini.png" );
    
    pTTF = CCLabelTTF::create ( bIsExist ? "Images/grossini.png exists" : "Images/grossini.png doesn't exist", "fonts/Marker Felt.ttf", 20 );
    pTTF->setPosition ( ccp ( s.cx / 2, s.cy / 3 ) );
    this->addChild ( pTTF );
    
    bIsExist = pSharedFileUtils->isFileExist ( "Images/grossini.xcf" );
    pTTF = CCLabelTTF::create ( bIsExist ? "Images/grossini.xcf exists" : "Images/grossini.xcf doesn't exist", "fonts/Marker Felt.ttf", 20 );
    pTTF->setPosition ( ccp ( s.cx / 2, s.cy / 3 * 2 ) );
    this->addChild ( pTTF );
}

KDvoid TestIsFileExist::onExit ( KDvoid )
{
	CCFileUtils*  pSharedFileUtils = CCFileUtils::sharedFileUtils ( );
	
	// reset filename lookup
    pSharedFileUtils->setFilenameLookupDictionary ( CCDictionary::create ( ) );

	TestFileUtils::onExit ( );
}

const KDchar* TestIsFileExist::title ( KDvoid )
{
	return "FileUtils: check whether the file exists";
}

const KDchar* TestIsFileExist::subtitle ( KDvoid )
{
	return "";
}