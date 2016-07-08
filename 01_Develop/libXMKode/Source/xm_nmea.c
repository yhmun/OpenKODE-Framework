/* --------------------------------------------------------------------------
 *
 *      File            xm_nmea.c
 *      Description     NMEA
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

#include "kd_library.h"

enum NP_STATE 
{
	NP_STATE_SOM			= 0,				// Search for start of message
	NP_STATE_CMD				,				// Get command
	NP_STATE_DATA				,				// Get data
	NP_STATE_CHECKSUM_1			,				// Get first checksum character
	NP_STATE_CHECKSUM_2			,				// Get second checksum character
};

#define NP_MAX_CMD_LEN			8	
#define NP_MAX_DATA_LEN			256
#define NP_MAX_FIELD	        25	

typedef struct 
{
	XMNMEA     nmea;
	KDbool     update;
	KDint      index;
	KDint      state;
	KDchar     checksum;
	KDchar     received_checksum;
	KDchar     command [ NP_MAX_CMD_LEN ];
	KDchar     data [ NP_MAX_DATA_LEN ];
} XMNMEAParser;

static XMNMEAParser      l_parser [ 2 ];
static KDFile*           l_log;
extern KDint             g_provider;

KDint16*				 g_total_num_sats_in_view = &l_parser [ 0 ].nmea.gsv.total_num_sats_in_view;

// Initialize NMEA
KDvoid xmInitNMEA ( KDvoid )
{
	l_log = KD_NULL;

	xmResetNMEA ( KD_TRUE  );
	xmResetNMEA ( KD_FALSE );
}

// Release NMEA
KDvoid xmFreeNMEA ( KDvoid )
{
	xmStopGPSLog ( );
}

// xmResetNMEA :Reset NMEA information.
KD_API KDvoid KD_APIENTRY xmResetNMEA ( KDbool fromGPS )
{
	KDint  id = fromGPS ? 0 : 1;

	kdMemset ( &l_parser [ id ], 0, sizeof ( XMNMEAParser ) );

	//
	// GPGSA
	//
	l_parser [ id ].nmea.gsa.mode		= 'M';

	//
	// GPRMC
	//
	l_parser [ id ].nmea.rmc.valid		= 'V';
	l_parser [ id ].nmea.rmc.day		= 1;
	l_parser [ id ].nmea.rmc.month		= 1;

	//
	// Parser
	//
	l_parser [ id ].update				= KD_FALSE;
	l_parser [ id ].state				= NP_STATE_SOM;
	l_parser [ id ].index				= 0;
	l_parser [ id ].checksum			= 0;
	l_parser [ id ].received_checksum	= 0;
}

KDbool xmIsNeededUpdateNMEA ( KDbool fromGPS )
{
	KDint  id = fromGPS ? 0 : 1;

	return l_parser [ id ].update;
}

// xmGetNMEA : Get NMEA information. If call this function, nmea's update flag will be set KD_FALSE. 
KD_API const XMNMEA* KD_APIENTRY xmGetNMEA ( KDboolean fromGPS )
{
	KDint  id = fromGPS ? 0 : 1;
	
	l_parser [ id ].update = KD_FALSE;

	return &l_parser [ id ].nmea;
}

static KDbool xmGetField ( const KDchar* data, KDchar* field, KDint field_num, KDint field_max )
{
	KDint  i = 0;
	KDint  i2 = 0;
	KDint  num = 0;
	//
	// Validate params
	//
	if ( data == NULL || field == NULL || field_max <= 0 )
	{
		return KD_FALSE;
	}

	//
	// Go to the beginning of the selected field
	//
	while ( num != field_num && data [ i ] )
	{
		if ( data [ i ] == ',' )
		{
			num++;
		}

		i++;

		if ( data [ i ] == 0 )
		{
			field [ 0 ] = '\0';
			return KD_FALSE;
		}
	}

	if ( data [ i ] == ',' || data [ i ] == '*' )
	{
		field [ 0 ] = '\0';
		return KD_FALSE;
	}

	//
	// copy field from pData to Field
	//
	while ( data [ i ] != ',' && data [ i ] != '*' && data [ i ] )
	{
		field [ i2 ] = data [ i ];
		i2++; 
		i++;

		//
		// check if field is too big to fit on passed parameter. If it is,
		// crop returned field to its max length.
		//
		if ( i2 >= field_max )
		{
			i2 = field_max - 1;
			break;
		}
	}
	field [ i2 ] = '\0';

	return KD_TRUE;
}

static KDvoid xmParseGPGGA ( const KDchar* data, GPGGA* out )
{
	KDchar    field [ NP_MAX_FIELD ];
	KDchar     buff [ 10 ];
	KDfloat64  temp;

	//
	// Time
	//
	if ( xmGetField ( data, field, 0, NP_MAX_FIELD ) )
	{
		// Hour
		buff [ 0 ] = field [ 0 ];
		buff [ 1 ] = field [ 1 ];
		buff [ 2 ] = '\0';

		kdSscanfKHR ( buff, "%d", &out->hour );

		// minute
		buff [ 0 ] = field [ 2 ];
		buff [ 1 ] = field [ 3 ];
		buff [ 2 ] = '\0';

		kdSscanfKHR ( buff, "%d", &out->minute );

		// Second
		buff [ 0 ]  = field [ 4 ];
		buff [ 1 ]  = field [ 5 ];
		buff [ 2 ]  = '\0';

		kdSscanfKHR ( buff, "%d", &out->second );
	}

	//
	// Latitude
	//
	if ( xmGetField ( data, field, 1, NP_MAX_FIELD ) )
	{
		temp  = kdStrtodKHR ( field, KD_NULL );

		out->latitude  = kdFmodKHR ( temp, 100.0 ) / 60.0;
		out->latitude += kdFloorKHR ( temp / 100.0 );
	}

	if ( xmGetField ( data, field, 2, NP_MAX_FIELD ) )
	{
		if ( field [ 0] == 'S' )
		{
			out->latitude = -out->latitude;
		}
	}

	//
	// Longitude
	//
	if ( xmGetField ( data, field, 3, NP_MAX_FIELD ) )
	{
		temp  = (KDfloat) kdStrtodKHR ( field, KD_NULL );

		out->longitude  = kdFmodKHR ( temp, 100.0 ) / 60.0;
		out->longitude += kdFloorKHR ( temp / 100.0 );
	}

	if ( xmGetField ( data, field, 4, NP_MAX_FIELD ) )
	{
		if ( field [ 0] == 'W' )
		{
			out->longitude = -out->longitude;
		}
	}

	//
	// Fix quality
	//
	if ( xmGetField ( data, field, 5, NP_MAX_FIELD ) )
	{
		out->fix_quality = field [ 0 ] - '0';
	}

	//
	// Number of Satellites
	//
	if ( xmGetField ( data, field, 6, NP_MAX_FIELD ) )
	{
		buff [ 0 ] = field [ 0 ];
		buff [ 1 ] = field [ 1 ];
		buff [ 2 ] = '\0';

		kdSscanfKHR ( buff, "%d", &out->num_of_satellites );
	}

	//
	// Horizontal Dilution of Precision (HDOP)
	//
	if ( xmGetField ( data, field, 7, NP_MAX_FIELD ) )
	{
		out->hdop = kdStrtodKHR ( field, KD_NULL );
	}
	
	//
	// Altitude
	//
	if ( xmGetField ( data, field, 8, NP_MAX_FIELD ) )
	{
		out->altitude = kdStrtodKHR ( field, KD_NULL );
	}

	// Height of geoid above WGS84 ellipsoid
	if ( xmGetField ( data, field, 10, NP_MAX_FIELD ) )
	{
		out->height_of_geoid = kdStrtodKHR ( field, KD_NULL );
	}
//
//	// Time since last DGPS update
//	if ( xmGetField ( data, field, 12, NP_MAX_FIELD ) )
//	{
//		out->dgps_time = kdStrtodKHR ( field, KD_NULL );
//	}
//
//	// DGPS reference station id
//	if ( xmGetField ( data, field, 13, NP_MAX_FIELD ) )
//	{
//		kdSscanfKHR ( field, "%d", &out->dgps_sid );
//	}
//
	out->count++;
}

static KDvoid xmParseGPGSA ( const KDchar* data, GPGSA* out )
{
	KDchar    field [ NP_MAX_FIELD ];
	KDchar     buff [ 10 ];
	KDint         i;

	//
	// Mode
	//
	if ( xmGetField ( data, field, 0, NP_MAX_FIELD ) )
	{
		out->mode = field [ 0 ];
	}

	//
	// Fix Mode 
	//
	if ( xmGetField ( data, field, 1, NP_MAX_FIELD ) )
	{
		out->fix_mode = field [ 0 ] - '0';
	}

	//
	// PRN's of Satellite Vechicles (SV's) used in position fix (null for unused fields)
	//
	for ( i = 0; i < NP_MAX_SAT; i++)
	{
		if ( xmGetField ( data, field, 2 + i, NP_MAX_FIELD ) )
		{
			buff [ 0 ] = field [ 0 ];
			buff [ 1 ] = field [ 1 ];
			buff [ 2 ] = '\0';

			kdSscanfKHR ( field, "%d", &out->sat_prn [ i ] );
		}
		else
		{
			out->sat_prn [ i ] = 0;
		}
	}

	//
	// Position Dilution of Precision (PDOP)
	//
	if ( xmGetField ( data, field, 14, NP_MAX_FIELD ) )
	{
		out->pdop = kdStrtodKHR ( field, KD_NULL );
	}
	else
	{
		out->pdop = 0.0;
	}

	//
	// Horizontal Dilution of Precision (HDOP)
	//
	if ( xmGetField ( data, field, 15, NP_MAX_FIELD ) )
	{
		out->hdop = kdStrtodKHR ( field, KD_NULL );
	}
	else
	{
		out->hdop = 0.0;
	}

	//
	// Vertical Dilution of Precision (VDOP)
	//
	if ( xmGetField ( data, field, 16, NP_MAX_FIELD ) )
	{
		out->vdop = kdStrtodKHR ( field, KD_NULL );
	}
	else
	{
		out->vdop = 0.0;
	}
	
	out->count++;
}

static KDbool xmIsSatUsedInSolution ( KDuint16 prn, GPGSA* out )
{
	KDint  i;

	if ( prn == 0 ) 
	{
		return KD_FALSE;
	}

	for ( i = 0; i < NP_MAX_SAT; i++ ) 
	{
		if ( prn == out->sat_prn [ i ] )
		{
			return KD_TRUE;
		}
	}

	return KD_FALSE;
}

static KDvoid xmParseGPGSV ( const KDchar* data, GPGSV* out, GPGSA* gsa )
{
	KDchar    field [ NP_MAX_FIELD ];
	KDint     total_num_of_msg = 0;
	KDint     msg_num;
	KDint     off;
	KDint     i;
	
	//
	// Total number of messages of this type in this cycle
	//
	if ( xmGetField ( data, field, 0, NP_MAX_FIELD ) )
	{
		kdSscanfKHR ( field, "%d", &total_num_of_msg );

		//
		// Make sure that the nTotalNumOfMsg is valid. This is used to
		// calculate indexes into an array. I've seen corrept NMEA strings
		// with no checksum set this to large values.
		//
		if ( total_num_of_msg > 9 || total_num_of_msg < 0 ) 
		{
			return; 
		}
	}

	if ( total_num_of_msg < 1 || total_num_of_msg * 4 >= NP_MAX_CHAN )
	{
		return;
	}

	//
	// message number
	//
	if ( xmGetField ( data, field, 1, NP_MAX_FIELD ) )
	{
		kdSscanfKHR ( field, "%d", &msg_num );

		//
		// Make sure that the message number is valid. This is used to
		// calculate indexes into an array
		//
		if ( msg_num > 9 || msg_num < 0 ) 
		{
			return; 
		}
	}

	//
	// Total number of SVs in view
	//
	if ( xmGetField ( data, field, 2, NP_MAX_FIELD ) )
	{
		kdSscanfKHR ( field, "%d", &out->total_num_sats_in_view );
	}

	//
	// Information about SV
	//
	for ( i = 0; i < 4; i++ )
	{
		off = i + ( msg_num - 1 ) * 4;

		// SV PRN number
		if ( xmGetField ( data, field, 3 + 4 * i, NP_MAX_FIELD ) )
		{
			kdSscanfKHR ( field, "%d", &out->sat_infos [ off ].prn );
		}
		else
		{
			out->sat_infos [ off ].prn = 0;
		}

		// Elevation in degrees, 90 maximum
		if ( xmGetField ( data, field, 4 + 4 * i, NP_MAX_FIELD ) )
		{
			kdSscanfKHR ( field, "%d", &out->sat_infos [ off ].elevation );
		}
		else
		{
			out->sat_infos [ off ].elevation = 0;
		}

		// Azimuth, degrees from true north, 000 to 359
		if ( xmGetField ( data, field, 5 + 4 * i, NP_MAX_FIELD ) )
		{
			kdSscanfKHR ( field, "%d", &out->sat_infos [ off ].azimuth );
		}
		else
		{
			out->sat_infos [ off ].azimuth = 0;
		}

		// SNR, 00-99 dB ( null when not tracking )
		if ( xmGetField ( data, field, 6 + 4 * i, NP_MAX_FIELD ) )
		{
			kdSscanfKHR ( field, "%d", &out->sat_infos [ off ].signal );
		}
		else
		{
			out->sat_infos [ off ].signal = 0;
		}

		//
		// Update "used in solution" (m_bUsedInSolution) flag. This is base
		// on the GSA message and is an added convenience for post processing
		//
		out->sat_infos [ off ].used_in_solution = xmIsSatUsedInSolution ( out->sat_infos [ off ].prn, gsa );
	}

	out->count++;
}

static KDvoid xmParseGPRMC ( const KDchar* data, GPRMC* out )
{
	KDchar    field [ NP_MAX_FIELD ];
	KDchar     buff [ 10 ];
	KDfloat64  temp;

	//
	// UTC time of fix
	//
	if ( xmGetField ( data, field, 0, NP_MAX_FIELD ) )
	{
		// Hour
		buff [ 0 ] = field [ 0 ];
		buff [ 1 ] = field [ 1 ];
		buff [ 2 ] = '\0';

		kdSscanfKHR ( buff, "%d", &out->hour );

		// minute
		buff [ 0 ] = field [ 2 ];
		buff [ 1 ] = field [ 3 ];
		buff [ 2 ] = '\0';

		kdSscanfKHR ( buff, "%d", &out->minute );

		// Second
		buff [ 0 ] = field [ 4 ];
		buff [ 1 ] = field [ 5 ];
		buff [ 2 ] = '\0';

		kdSscanfKHR ( buff, "%d", &out->second );
	}

	//
	// Data status (A=Valid position, V=navigation receiver warning)
	//
	if ( xmGetField ( data, field, 1, NP_MAX_FIELD ) )
	{
		out->valid = field [ 0 ];
	}
	else
	{
		out->valid = 'V';
	}

	//
	// Latitude of fix
	//
	if ( xmGetField ( data, field, 2, NP_MAX_FIELD ) )
	{
		temp  = kdStrtodKHR ( field, KD_NULL );

		out->latitude  = kdFmodKHR ( temp, 100.0 ) / 60.0;
		out->latitude += kdFloorKHR ( temp / 100.0 );
	}

	if ( xmGetField ( data, field, 3, NP_MAX_FIELD ) )
	{
		if ( field [ 0] == 'S' )
		{
			out->latitude = -out->latitude;
		}
	}

	//
	// Longitude of fix
	//
	if ( xmGetField ( data, field, 4, NP_MAX_FIELD ) )
	{
		temp  = (KDfloat) kdStrtodKHR ( field, KD_NULL );

		out->longitude  = kdFmodKHR ( temp, 100.0 ) / 60.0;
		out->longitude += kdFloorKHR ( temp / 100.0 );
	}

	if ( xmGetField ( data, field, 5, NP_MAX_FIELD ) )
	{
		if ( field [ 0] == 'W' )
		{
			out->longitude = -out->longitude;
		}
	}

	//
	// Speed over ground in knots
	//
	if ( xmGetField ( data, field, 6, NP_MAX_FIELD ) )
	{
		out->ground_speed = kdStrtodKHR ( field, KD_NULL );
	}
	else
	{
		out->ground_speed = 0.0;
	}

	//
	// Track made good in degrees True
	//
	if ( xmGetField ( data, field, 7, NP_MAX_FIELD ) )
	{
		out->course = kdStrtodKHR ( field, KD_NULL );
	}
	else
	{
		out->course = 0.0;
	}

	//
	// UTC Date of fix
	//
	if ( xmGetField ( data, field, 8, NP_MAX_FIELD ) )
	{
		// Day
		buff [ 0 ] = field [ 0 ];
		buff [ 1 ] = field [ 1 ];
		buff [ 2 ] = '\0';

		kdSscanfKHR ( buff, "%d", &out->day );

		// Month
		buff [ 0 ] = field [ 2 ];
		buff [ 1 ] = field [ 3 ];
		buff [ 2 ] = '\0';

		kdSscanfKHR ( buff, "%d", &out->month );

		// Year 
		buff [ 0 ] = field [ 4 ];
		buff [ 1 ] = field [ 5 ];
		buff [ 2 ] = '\0';

		kdSscanfKHR ( buff, "%d", &out->year );
	}

	//
	// Magnetic variation
	//
	if ( xmGetField ( data, field, 9, NP_MAX_FIELD ) )
	{
		out->magnetic = kdStrtodKHR ( field, KD_NULL );
	}
	else
	{
		out->magnetic = 0.0;
	}

	if ( xmGetField ( data, field, 10, NP_MAX_FIELD ) )
	{
		if ( field [ 0 ] == 'W' )
		{
			out->magnetic = -out->magnetic;
		}
	}

	out->count++;
}

static KDvoid xmParseCommand ( const KDchar* command, const KDchar* data, XMNMEAParser* out )
{
	KDbool  update = KD_TRUE;

	if      ( !kdStrcmp ( command, "GPGGA" ) )
	{
		xmParseGPGGA ( data, &out->nmea.gga );
	}
	else if ( !kdStrcmp ( command, "GPGSA" ) )
	{
		xmParseGPGSA ( data, &out->nmea.gsa );
	}
	else if ( !kdStrcmp ( command, "GPGSV" ) )
	{
		xmParseGPGSV ( data, &out->nmea.gsv, &out->nmea.gsa );
	}
	else if ( !kdStrcmp ( command, "GPRMC" ) )
	{
		xmParseGPRMC ( data, &out->nmea.rmc );
	}
	else
	{
		update = KD_FALSE;
	}

	if ( !out->update )
	{
		out->update = update;
	}
}

// xmParseNMEA : Parse NMEA information from stream.
KD_API KDvoid KD_APIENTRY xmParseNMEA ( KDboolean fromGPS, const KDchar* stream, KDsize size )
{
	XMNMEAParser*  parser = fromGPS ? &l_parser [ 0 ] : &l_parser [ 1 ];

	KDuint   i;
	KDchar   data;

	for ( i = 0; i < size; i++ )
	{
		data = (KDchar) stream [ i ];

		switch ( parser->state )
		{
			///////////////////////////////////////////////////////////////////////
			// Search for start of message '$'
			case NP_STATE_SOM :
	
				if ( data == '$' )
				{
					parser->checksum = 0;				// reset checksum
					parser->index = 0;				// reset index
					parser->state = NP_STATE_CMD;
				}
	
				break;

			///////////////////////////////////////////////////////////////////////
			// Retrieve command (NMEA Address)
			case NP_STATE_CMD :
	
				if ( data != ',' && data != '*' )
				{
					parser->command [ parser->index++ ] = data;
					parser->checksum ^= data;

					// Check for command overflow
					if ( parser->index >= NP_MAX_CMD_LEN )
					{
						parser->state = NP_STATE_SOM;
					}
				}
				else
				{
					parser->command [ parser->index ] = '\0';	// terminate command
					parser->checksum ^= data;
					parser->index = 0;
					parser->state = NP_STATE_DATA;		// goto get data state
				}
	
				break;

			///////////////////////////////////////////////////////////////////////
			// Store data and check for end of sentence or checksum flag
			case NP_STATE_DATA :
	
				if ( data == '*' )		// checksum flag?
				{
					parser->data [ parser->index ] = '\0';
					parser->state = NP_STATE_CHECKSUM_1;
				}
				else // no checksum flag, store data
				{
					//
					// Check for end of sentence with no checksum
					//
					if ( data == '\r' )
					{
						parser->data [ parser->index ] = '\0';
						xmParseCommand ( parser->command, parser->data, parser );
						parser->state = NP_STATE_SOM;
						continue;
					}

					//
					// Store data and calculate checksum
					//
					parser->checksum ^= data;
					parser->data [ parser->index ] = data;
					if ( ++parser->index >= NP_MAX_DATA_LEN )	// Check for buffer overflow
					{
						parser->state = NP_STATE_SOM;
					}
				}
	
				break;

			///////////////////////////////////////////////////////////////////////
			case NP_STATE_CHECKSUM_1 :
	
				if( ( data - '0' ) <= 9 )
				{
					parser->received_checksum = ( data - '0' ) << 4;
				}
				else
				{
					parser->received_checksum = ( data - 'A' + 10 ) << 4;
				}

				parser->state = NP_STATE_CHECKSUM_2;
	
				break;

			///////////////////////////////////////////////////////////////////////
			case NP_STATE_CHECKSUM_2 :
	
				if ( ( data - '0' ) <= 9 )
				{
					parser->received_checksum |= ( data - '0' );
				}
				else
				{
					parser->received_checksum |= ( data - 'A' + 10 );
				}

				if ( parser->checksum == parser->received_checksum )
				{
					xmParseCommand ( parser->command, parser->data, parser );
				}

				parser->state = NP_STATE_SOM;
	
				break;

			///////////////////////////////////////////////////////////////////////
			default :
				
				parser->state = NP_STATE_SOM;
		}
	}

	if ( fromGPS && l_log )
	{
		kdFwrite ( stream, size, 1, l_log ); 
	}
}

// xmStartGPSLog : Save GPS logging.
KD_API KDvoid KD_APIENTRY xmStartGPSLog ( const KDchar* path )
{
	l_log = kdFopen ( path, "wb" );
}

// xmStopGPSLog : Stop GPS logging.
KD_API KDvoid KD_APIENTRY xmStopGPSLog ( KDvoid )
{
	if ( l_log )
	{
		kdFclose ( l_log );
		l_log = KD_NULL;
	}
}

KD_API KDint KD_APIENTRY xmGetProvider ( KDvoid )
{
	return g_provider;
}

#if defined ( SHP ) || !defined ( _WIN32 )
// xmGPSReset : Reset GPS ( only for Win32 platform )
KD_API KDvoid KD_APIENTRY xmGPSReset ( KDint mode )
{

}
#endif