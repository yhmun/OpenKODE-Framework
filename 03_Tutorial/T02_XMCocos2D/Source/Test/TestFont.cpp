/* --------------------------------------------------------------------------
 *
 *      File            TestFont.cpp
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

#include "Precompiled.h"
#include "TestFont.h"

const KDchar* l_szFontList [ ] =
{
	"xm_supports/msgothic.ttc",
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    "American Typewriter",
    "Marker Felt",
#endif
    "Default",
    "fonts/A Damn Mess.ttf",
    "fonts/Abberancy.ttf",
    "fonts/Abduction.ttf",
    "fonts/Paint Boy.ttf",
    "fonts/Schwarzwald Regular.ttf",
    "fonts/Scissor Cuts.ttf",
};

const KDuint  l_uCount = sizeof ( l_szFontList ) / sizeof ( *l_szFontList );

KDbool TestFont::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );

	KDint  nIdx = s_nSubTestIndex / l_uCount;
	CCVerticalTextAlignment eVertical;	
	switch ( nIdx )
	{
		case  0 : eVertical = kCCVerticalTextAlignmentTop;	  break;
		case  1 : eVertical = kCCVerticalTextAlignmentCenter; break;
		default : eVertical = kCCVerticalTextAlignmentBottom; break;
	}

	CCSize  tSize ( tLyrSize.cx / 3, tLyrSize.cy / 3 * 2 );

    CCLayerColor* pBGLeft   = CCLayerColor::create ( ccc4 ( 100, 100, 100, 255 ), tSize );
    CCLayerColor* pBGCenter = CCLayerColor::create ( ccc4 ( 200, 100, 100, 255 ), tSize );
    CCLayerColor* pBGRight  = CCLayerColor::create ( ccc4 ( 100, 100, 200, 255 ), tSize );

	pBGLeft  ->setPosition ( ccp ( 0           , tLyrSize.cy / 6 - 10 ) );
	pBGCenter->setPosition ( ccp ( tSize.cx    , tLyrSize.cy / 6 - 10 ) );
	pBGRight ->setPosition ( ccp ( tSize.cx * 2, tLyrSize.cy / 6 - 10 ) );

    addChild ( pBGLeft   );
    addChild ( pBGCenter );
    addChild ( pBGRight  );
	
	
	CCLabelTTF*  pLeft   = CCLabelTTF::create ( "alignment left"  , l_szFontList [ s_nSubTestIndex % l_uCount ], 24, tSize, kCCTextAlignmentLeft  , eVertical );
	CCLabelTTF*  pCenter = CCLabelTTF::create ( "alignment center", l_szFontList [ s_nSubTestIndex % l_uCount ], 24, tSize, kCCTextAlignmentCenter, eVertical );
	CCLabelTTF*  pRight  = CCLabelTTF::create ( "alignment right" , l_szFontList [ s_nSubTestIndex % l_uCount ], 24, tSize, kCCTextAlignmentRight , eVertical );

	pLeft  ->setPosition ( ccpMid ( tSize ) );
	pCenter->setPosition ( ccpMid ( tSize ) );
	pRight ->setPosition ( ccpMid ( tSize ) );

    pBGLeft  ->addChild ( pLeft   );
    pBGCenter->addChild ( pCenter );
    pBGRight ->addChild ( pRight  );

	if ( s_nSubTestIndex % l_uCount == 0 )
	{
		CCDictionary*	pDict = CCDictionary::createWithContentsOfFile ( "xm_supports/strings.plist" );
		const KDchar*	pStr  = pDict->valueForKey( "Japanease" )->getCString ( );

		pLeft  ->setString ( pStr );
		pCenter->setString ( pStr );
		pRight ->setString ( pStr );
	}

	return KD_TRUE;
}

KDuint TestFont::count ( KDvoid )
{
	return l_uCount * 3;
}

const KDchar* TestFont::subtitle ( KDvoid )
{
	return l_szFontList [ s_nSubTestIndex % l_uCount ];
}

