/* --------------------------------------------------------------------------
 *
 *      File            bada_locale.c
 *      Description     Locale specific functions
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#include "../kd_library.h"

struct XMLocale
{
	KDint           country_code;
	KDint           language_code;
	const KDchar*   info;
};

static struct XMLocale    l_tables[] =
{
	{ COUNTRY_GB, LANGUAGE_ENG, "en_GB" },
	{ COUNTRY_US, LANGUAGE_ENG, "en_US" },					//	English		United Kingdom
	{ COUNTRY_KR, LANGUAGE_KOR, "ko_KR" },
	{ -1		, 0			  , 0		},
};

// kdGetLocale : Determine the current language and locale in platform specific.
const KDchar* xmPlatformGetLocale ( KDvoid )
{
	KDint           i;
	KDint           country_code;
	KDint           language_code;
	LocaleManager   localeManager;

	localeManager.Construct ( );
	Locale systemLocale = localeManager.GetSystemLocale ( );

	country_code  = systemLocale.GetCountryCode ( );
	language_code = systemLocale.GetLanguageCode ( );

	for ( i = 0; l_tables[ i ].country_code != -1; i++ )
	{
		if ( l_tables[ i ].country_code == country_code && l_tables[ i ].language_code == language_code )
		{
			return l_tables[i].info;
		}
	}

	return "unknown";
}
