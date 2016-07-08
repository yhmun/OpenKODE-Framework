/* --------------------------------------------------------------------------
 *
 *      File            TestCurrentLanguage.cpp
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
#include "TestCurrentLanguage.h"

KDbool TestCurrentLanguage::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

    const KDchar*  szLanguage;

    switch ( CCApplication::sharedApplication ( )->getCurrentLanguage ( ) )
    {
		case kCCLanguageEnglish		:	szLanguage = "English";		break;
		case kCCLanguageChinese		:	szLanguage = "Chinese";		break;
		case kCCLanguageFrench		:	szLanguage = "French";		break;
		case kCCLanguageGerman		:	szLanguage = "German";		break;
		case kCCLanguageItalian		:	szLanguage = "Italian";		break;
		case kCCLanguageRussian		:	szLanguage = "Russian";		break;
		case kCCLanguageSpanish		:	szLanguage = "Spanish";		break;
		case kCCLanguageKorean		:	szLanguage = "Korean";		break;
		default						:	szLanguage = "Unknown";		break;
    }

	CCLabelTTF*  pLabel = CCLabelTTF::create
	(
		ccszf ( "Current language is %s. ( %s )", szLanguage, kdGetLocale ( ) ), "fonts/tahoma.ttf", 32 
	);
	pLabel->setPosition ( ccpMid ( this->getContentSize ( ) ) );
    this->addChild ( pLabel );

	return KD_TRUE;
}
