/* --------------------------------------------------------------------------
 *
 *      File            xm_nmea.h
 *      Description     NMEA
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
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

#ifndef __xm_nmea_h__
#define __xm_nmea_h__

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack ( push, 1 )

#define NP_MAX_SAT						12
#define NP_MAX_CHAN						36				// maximum number of channels

////////////////////////////////////////////////////////////////////////////////////////////
//
// Global Positioning System Fix Data 
//
// eg1. $GPGGA,170834,4124.8963,N,08151.6838,W,1,05,1.5,280.2,M,-34.0,M,,,*59
//
typedef struct
{	
	KDuint8			hour;								//  170834			: 17:08:34 UTC
	KDuint8			minute;							
	KDuint8			second;							
	KDfloat64		latitude;							// 4124.8963, N		: 41d 24.8963' or 41d 24' 54" ( < 0 = South, > 0 = North )							
	KDfloat64		longitude;							// 08151.6838, W	: 81d 51.6838' or 81d 51' 41" ( < 0 = West, > 0 = East )						
	KDuint8			fix_quality;						// 1				: Data is from a GPS fix ( 0 = Invalid, 1 = GPS fix, 2 = DGPS fix )
	KDuint16		num_of_satellites;					// 05				: 5 Satellites are in view
	KDfloat64		hdop;								// 1.5				: Relative accuracy of horizontal position 
	KDfloat64		altitude;							// 280.2, M			: meters above mean sea level ( unit 'M' skipped )  
	KDfloat64		height_of_geoid;					// -34.0, M			: -34.0 meters ( Height of geoid above WGS84 ellipsoid, unit 'M' skipped )
	KDfloat64		dgps_time;							// blank			: No last update ( Time since last DGPS update )
	KDuint32		dgps_sid;							// blank			: No station id ( DGPS reference station id )
	KDuint32        count;
} GPGGA;

////////////////////////////////////////////////////////////////////////////////////////////
//
// GPS DOP and active satellites 
//
// eg1. $GPGSA,A,3,,,,,,16,18,,22,24,,,3.6,2.1,2.2*3C
// eg2. $GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*34
//
typedef struct 
{
	KDchar			mode;								// M = manual ( forced to operate in 2D or 3D ) , A = automatic 3D/2D
	KDuint8			fix_mode;							// 1 = fix not available, 2 = 2D, 3 = 3D
	KDuint16		sat_prn[NP_MAX_SAT];				// ID of sats in solution
	KDfloat64		pdop;								// Position Dilution of Precision (PDOP)
	KDfloat64		hdop;								// Horizontal Dilution of Precision (HDOP)
	KDfloat64		vdop;								// Vertical Dilution of Precision (VDOP)
	KDuint32        count;
} GPGSA;

////////////////////////////////////////////////////////////////////////////////////////////
//
// GPS Satellites in view
//
// eg. $GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
//     $GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74
//     $GPGSV,3,3,11,22,42,067,42,24,14,311,43,27,05,244,00,,,,*4D
//
//     $GPGSV,1,1,13,02,02,213,,03,-3,000,,11,00,121,,14,13,172,05*62
//
typedef struct 
{
    KDuint16		prn;								// SV PRN number
    KDuint16		elevation;							// Elevation in degrees, 90 maximum
    KDuint16		azimuth;							// Azimuth, degrees from true north, 000 to 359
    KDuint16		signal;								// SNR, 00-99 dB (null when not tracking)
	KDbool          used_in_solution;					// Update "used in solution" flag. This is base on the GSA message and is an added convenience for post processing
} SatInfo;

typedef struct 
{
	KDint16			total_num_of_msg;					// Total number of messages of this type in this cycle
	KDint16			total_num_sats_in_view;				// Total number of SVs in view
	SatInfo			sat_infos[NP_MAX_CHAN];				// Four SV infomations
	KDuint32        count;
} GPGSV;

////////////////////////////////////////////////////////////////////////////////////////////
//
// Recommended minimum specific GPS/TRANSIT data 
//
// eg1. $GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62
// eg2. $GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68
//
typedef struct 
{
	KDuint8			hour;								// 225446			: Time of fix 22:54:46 UTC
	KDuint8			minute;								
	KDuint8			second;								
	KDchar			valid;								// A				: Navigation receiver warning A = Valid position, V = Warning
	KDfloat64		latitude;							// 4916.45, N		: Latitude 49 deg. 16.45 min. North ( < 0 = South, > 0 = North )
	KDfloat64		longitude;							// 12311.12, W		: Longitude 123 deg. 11.12 min. West ( < 0 = West, > 0 = East )
	KDfloat64		ground_speed;						// 000.5			: Speed over ground, Knots
	KDfloat64		course;								// 054.7			: Course Made Good, degrees true
	KDuint8			day;								// 191194			: UTC Date of fix, 19 November 1994
	KDuint8			month;								
	KDuint8		    year;									
	KDfloat64		magnetic;							// 020.3, E			: Magnetic variation, 20.3 deg. East ( < 0 = West, > 0 = East )
	KDuint32        count;
} GPRMC;

typedef struct 
{
	GPGGA			gga;
	GPGSA			gsa;
	GPGSV			gsv;
	GPRMC			rmc;
} XMNMEA;

#pragma pack ( pop )

// xmResetNMEA :Reset NMEA information.
KD_API KDvoid KD_APIENTRY xmResetNMEA ( KDboolean fromGPS );

// xmGetNMEA : Get NMEA information. If call this function, nmea's update flag will be set KD_FALSE. 
KD_API const XMNMEA* KD_APIENTRY xmGetNMEA ( KDboolean fromGPS );

// xmParseNMEA : Parse NMEA information from stream.
KD_API KDvoid KD_APIENTRY xmParseNMEA ( KDboolean fromGPS, const KDchar* stream, KDsize size );

// xmStartGPSLog : Save GPS logging.
KD_API KDvoid KD_APIENTRY xmStartGPSLog ( const KDchar* path );

// xmStopGPSLog : Stop GPS logging.
KD_API KDvoid KD_APIENTRY xmStopGPSLog ( KDvoid );

#define XM_PROVIDER_GPS          1
#define XM_PROVIDER_NETWORK      2
#define XM_PROVIDER_PASSIVE      3

// For : Android
KD_API KDint KD_APIENTRY xmGetProvider ( KDvoid );

#define XM_GPS_RESET_HOT         1
#define XM_GPS_RESET_WARM        2
#define XM_GPS_RESET_COLD        3

// xmGPSReset : Reset GPS ( only for Win32 platform )
KD_API KDvoid KD_APIENTRY xmGPSReset ( KDint mode );

#ifdef __cplusplus
}
#endif

#endif 



