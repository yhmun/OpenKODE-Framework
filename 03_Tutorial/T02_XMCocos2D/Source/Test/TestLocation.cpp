/* --------------------------------------------------------------------------
 *
 *      File            TestLocation.cpp
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
#include "TestLocation.h"

KDvoid TestLocation::onEnter ( KDvoid )
{	
	TestBasic::onEnter ( );

	m_pLabel = CCLabelTTF::create 
	(
		"", "fonts/Thonburi.ttf", 20, CCPointZero, kCCAlignmentLeft
	);	
	this->addChild ( m_pLabel );
	m_pLabel->setPositionWithParent ( kCCAlignmentCenter );

	this->setEventEnabled ( KD_TRUE ); 

	KDbool  bFromGPS;
	if ( s_nSubTestIndex == 0 )
	{
		m_pFile = kdFopen ( "/res/xm_supports/GPS_LOG.txt", "rb" );
		this->schedule ( schedule_selector ( TestLocation::onReadFile ), 0.5f );

		bFromGPS = KD_FALSE;
        xmResetNMEA ( bFromGPS );
	}
	else
	{
		m_pFile  = KD_NULL;
		bFromGPS = KD_TRUE;
	}

	this->onRefresh ( bFromGPS );

	if ( CCApplication::sharedApplication ( )->getTargetPlatform ( ) == kCCTargetAndroid )
	{
		xmStartGPSLog ( "/storage/GPS_LOG.txt" );
	}
	else
	{
		xmStartGPSLog ( "/data/GPS_LOG.txt" );
	}		
}

KDvoid TestLocation::onExit ( KDvoid )
{
	if ( m_pFile )
	{
		kdFclose ( m_pFile );
		m_pFile = KD_NULL;
	}

	xmStopGPSLog ( );

	TestBasic::onExit ( );
}

KDuint TestLocation::count ( KDvoid )
{
	return 2;
}

const KDchar* TestLocation::subtitle ( KDvoid )
{
	if ( s_nSubTestIndex == 0 )
	{
		return "Received From GPS Log File";
	}
	else
	{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		const KDchar*  szProvider = "Received From Unknown";

		switch ( xmGetProvider ( ) )
		{
			case XM_PROVIDER_GPS	 :	szProvider = "Received From GPS";		break;
			case XM_PROVIDER_NETWORK :	szProvider = "Received From Network";	break;
			case XM_PROVIDER_PASSIVE :	szProvider = "Received From Passive";	break;
		} 
		return szProvider;
	#else
		return "Received From GPS or Cell";
	#endif		
	}
}

KDvoid TestLocation::onEvent ( const KDEvent* pEvent )
{
	if ( pEvent->type == KD_EVENT_LOCATION )
	{
		if		( s_nSubTestIndex == 0 && pEvent->data.value.i == KD_NMEA_UPDATED_USER )
		{
			onRefresh ( KD_FALSE );
		}
		else if ( s_nSubTestIndex == 1 && pEvent->data.value.i == KD_NMEA_UPDATED_GPS  ) 
		{
			onRefresh ( KD_TRUE );
		}
	}
	else if ( pEvent->type == KD_EVENT_PROVIDER )
	{
		const KDchar*  szProvider = "Received From Unknown";

		switch ( pEvent->data.value.i )
		{
			case XM_PROVIDER_GPS	 :	szProvider = "Received From GPS";		break;
			case XM_PROVIDER_NETWORK :	szProvider = "Received From Network";	break;
			case XM_PROVIDER_PASSIVE :	szProvider = "Received From Passive";	break;
		}

		m_pSubTitle->setString ( szProvider );
	}
}

KDvoid TestLocation::onReadFile ( KDfloat fDelta )
{
	static KDchar  szBuffer [ 1024 ];

	if ( m_pFile )
	{
		KDsize  uReadBytes = kdFread ( (KDvoid*) szBuffer, 1, 1024, m_pFile );
		if ( uReadBytes > 0 )
		{
			xmParseNMEA ( KD_FALSE, szBuffer, uReadBytes );
		}
		else
		{
			kdFclose ( m_pFile );
			m_pFile = KD_NULL;
		}
	}
}

KDvoid TestLocation::onRefresh ( KDbool bFromGPS )
{
	const XMNMEA*	pNMEA   = xmGetNMEA ( bFromGPS );
	std::string		sOutput = "";
	KDchar			szLine [ 256 ];

	kdSprintf ( szLine, "GPGGA ==> %02d:%02d:%02d, pos = %.6f, %.6f, fix = %d, sat =%d\n",
								pNMEA->gga.hour, pNMEA->gga.minute, pNMEA->gga.second,
								(KDfloat) pNMEA->gga.latitude,
                                (KDfloat) pNMEA->gga.longitude,
								pNMEA->gga.fix_quality,
								pNMEA->gga.num_of_satellites );
	sOutput += szLine;

	kdSprintf ( szLine, "hdop = %0.2f, altitude = %.2f, cnt = %d\n",
								(KDfloat) pNMEA->gga.hdop, 
								(KDfloat) pNMEA->gga.altitude,
								pNMEA->gga.count );
	sOutput += szLine;

	kdSprintf ( szLine, "GPGSA ==> prn = %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, %02d, cnt = %d\n",
								pNMEA->gsa.sat_prn [ 0 ], 
								pNMEA->gsa.sat_prn [ 1 ], 
								pNMEA->gsa.sat_prn [ 2 ], 
								pNMEA->gsa.sat_prn [ 3 ], 
								pNMEA->gsa.sat_prn [ 4 ], 
								pNMEA->gsa.sat_prn [ 5 ], 
								pNMEA->gsa.sat_prn [ 6 ], 
								pNMEA->gsa.sat_prn [ 7 ], 
								pNMEA->gsa.sat_prn [ 8 ], 
								pNMEA->gsa.sat_prn [ 9 ], 
								pNMEA->gsa.sat_prn [ 10 ], 
								pNMEA->gsa.sat_prn [ 11 ],
								pNMEA->gsa.count );
	sOutput += szLine;

	kdSprintf ( szLine, "mode = %c, fix = %d, pdop = %.2f, hdop = %.2f, vdop = %.2f\n",
								pNMEA->gsa.mode, 
								pNMEA->gsa.fix_mode,
								(KDfloat) pNMEA->gsa.pdop,
								(KDfloat) pNMEA->gsa.hdop,
								(KDfloat) pNMEA->gsa.vdop );
	sOutput += szLine;

	kdSprintf ( szLine, "GPGSV ==> total msg = %d, total sat num = %d, cnt = %d\n",
								pNMEA->gsv.total_num_of_msg, 
								pNMEA->gsv.total_num_sats_in_view,
								pNMEA->gsv.count );
	sOutput += szLine;

	kdSprintf ( szLine, "%02d %02d %02d %02d, %02d %02d %02d %02d, %02d %02d %02d %02d, %02d %02d %02d %02d\n",
		pNMEA->gsv.sat_infos [ 0 ].prn, pNMEA->gsv.sat_infos [ 0 ].elevation, pNMEA->gsv.sat_infos [ 0 ].elevation, pNMEA->gsv.sat_infos [ 0 ].signal,
		pNMEA->gsv.sat_infos [ 1 ].prn, pNMEA->gsv.sat_infos [ 1 ].elevation, pNMEA->gsv.sat_infos [ 1 ].elevation, pNMEA->gsv.sat_infos [ 1 ].signal,
		pNMEA->gsv.sat_infos [ 2 ].prn, pNMEA->gsv.sat_infos [ 2 ].elevation, pNMEA->gsv.sat_infos [ 2 ].elevation, pNMEA->gsv.sat_infos [ 2 ].signal,
		pNMEA->gsv.sat_infos [ 3 ].prn, pNMEA->gsv.sat_infos [ 3 ].elevation, pNMEA->gsv.sat_infos [ 3 ].elevation, pNMEA->gsv.sat_infos [ 3 ].signal );
	sOutput += szLine;

	kdSprintf ( szLine, "%02d %02d %02d %02d, %02d %02d %02d %02d, %02d %02d %02d %02d, %02d %02d %02d %02d\n",
		pNMEA->gsv.sat_infos [ 4 ].prn, pNMEA->gsv.sat_infos [ 4 ].elevation, pNMEA->gsv.sat_infos [ 4 ].elevation, pNMEA->gsv.sat_infos [ 4 ].signal,
		pNMEA->gsv.sat_infos [ 5 ].prn, pNMEA->gsv.sat_infos [ 5 ].elevation, pNMEA->gsv.sat_infos [ 5 ].elevation, pNMEA->gsv.sat_infos [ 5 ].signal,
		pNMEA->gsv.sat_infos [ 6 ].prn, pNMEA->gsv.sat_infos [ 6 ].elevation, pNMEA->gsv.sat_infos [ 6 ].elevation, pNMEA->gsv.sat_infos [ 6 ].signal,
		pNMEA->gsv.sat_infos [ 7 ].prn, pNMEA->gsv.sat_infos [ 7 ].elevation, pNMEA->gsv.sat_infos [ 7 ].elevation, pNMEA->gsv.sat_infos [ 7 ].signal );
	sOutput += szLine;

	kdSprintf ( szLine, "%02d %02d %02d %02d, %02d %02d %02d %02d, %02d %02d %02d %02d, %02d %02d %02d %02d\n",
		pNMEA->gsv.sat_infos [  8 ].prn, pNMEA->gsv.sat_infos [  8 ].elevation, pNMEA->gsv.sat_infos [  8 ].elevation, pNMEA->gsv.sat_infos [  8 ].signal,
		pNMEA->gsv.sat_infos [  9 ].prn, pNMEA->gsv.sat_infos [  9 ].elevation, pNMEA->gsv.sat_infos [  9 ].elevation, pNMEA->gsv.sat_infos [  9 ].signal,
		pNMEA->gsv.sat_infos [ 10 ].prn, pNMEA->gsv.sat_infos [ 10 ].elevation, pNMEA->gsv.sat_infos [ 10 ].elevation, pNMEA->gsv.sat_infos [ 10 ].signal,
		pNMEA->gsv.sat_infos [ 11 ].prn, pNMEA->gsv.sat_infos [ 11 ].elevation, pNMEA->gsv.sat_infos [ 11 ].elevation, pNMEA->gsv.sat_infos [ 11 ].signal );
	sOutput += szLine;

	kdSprintf ( szLine, "GPRMC ==> %02d:%02d:%02d, valid = %c, pos = %.6f, %.6f, %d.%02d.%02d cnt = %d\n",
								pNMEA->rmc.hour, pNMEA->rmc.minute, pNMEA->rmc.second, 
								pNMEA->rmc.valid,
								(KDfloat) pNMEA->rmc.latitude, 
                                (KDfloat) pNMEA->rmc.longitude,
                                pNMEA->rmc.year + ( pNMEA->rmc.year < 70 ? 2000 : 1900 ),
                                pNMEA->rmc.month,
                                pNMEA->rmc.day,
								pNMEA->rmc.count );
	sOutput += szLine;

	m_pLabel->setString ( sOutput.c_str ( ) );
}