/* --------------------------------------------------------------------------
 *
 *      File            kd_time.c
 *      Description     Time functions
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

// kdGetTimeUST : Get the current unadjusted system time.
KD_API KDust KD_APIENTRY kdGetTimeUST ( KDvoid )
{
	return xmPlatformGetTimeUST ( );
}

// kdTime : Get the current wall clock time.
KD_API KDtime KD_APIENTRY kdTime ( KDtime* timep )
{	
	return time ( (time_t *) timep );
}

// kdGmtime_r, kdLocaltime_r : Convert a seconds-since-epoch time into broken-down time.
KD_API KDTm* KD_APIENTRY kdGmtime_r ( const KDtime* timep, KDTm* result )
{		
	struct tm*      ret = gmtime ( (time_t *) timep );

	result->tm_sec   = ret->tm_sec;
	result->tm_min   = ret->tm_min;
	result->tm_hour  = ret->tm_hour;
	result->tm_mday  = ret->tm_mday;
	result->tm_mon   = ret->tm_mon;
	result->tm_year  = ret->tm_year;
	result->tm_wday  = ret->tm_wday;
	result->tm_yday  = ret->tm_yday;
	result->tm_isdst = ret->tm_isdst;

	return result;
}

KD_API KDTm* KD_APIENTRY kdLocaltime_r ( const KDtime* timep, KDTm* result )
{	
	struct tm*      ret = localtime ( (time_t *) timep );

	result->tm_sec   = ret->tm_sec;
	result->tm_min   = ret->tm_min;
	result->tm_hour  = ret->tm_hour;
	result->tm_mday  = ret->tm_mday;
	result->tm_mon   = ret->tm_mon;
	result->tm_year  = ret->tm_year;
	result->tm_wday  = ret->tm_wday;
	result->tm_yday  = ret->tm_yday;
	result->tm_isdst = ret->tm_isdst;

	return result;	
}

// kdUSTAtEpoch : Get the UST corresponding to KDtime 0.
// XMSoft's revision : Not supported API.
KD_API KDust KD_APIENTRY kdUSTAtEpoch ( KDvoid )
{
	return xmPlatformUSTAtEpoch ( );
}

#if 0
struct timeval 
{
     time_t			tv_sec;				/* seconds since Jan. 1, 1970 */
     suseconds_t	tv_usec;			/* and microseconds */
};
#endif 



// kdGettimeofday : Get date and time
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdGettimeofday ( struct KDTimeval* tv, struct KDTimezone* tz )
{
	struct timeval		_tv;
	struct timezone		_tz;

	KDint  ret = gettimeofday ( &_tv, &_tz );

	if ( tv )
	{
		tv->tv_sec	= _tv.tv_sec;
		tv->tv_usec	= _tv.tv_usec;
	}

	if ( tz )
	{
		tz->tz_minuteswest	= _tz.tz_minuteswest; 
		tz->tz_dsttime		= _tz.tz_dsttime;     
	}

//    KDust  ust  = xmPlatformGetTimeUST ( );
//    KDust  sec  = ust / 1000000000UL;
//    tv->tv_sec  = (KDint32) sec;
//    tv->tv_usec = (KDint32) ( ( ust - sec ) / 1000000UL );
    return ret;
}

// kdSettimeofday : Set date and time.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdSettimeofday ( const struct KDTimeval* tv, const struct KDTimezone* tz )
{
	struct timeval		_tv;
	struct timezone		_tz;

	_tv.tv_sec			= tv->tv_sec;
	_tv.tv_usec			= tv->tv_usec;

    _tz.tz_minuteswest	= tz->tz_minuteswest; 
    _tz.tz_dsttime		= tz->tz_dsttime;      

	return settimeofday ( &_tv, &_tz );
}

// kdDifftime : Return difference between two times.
// XMSoft's revision : Added API.
KD_API KDdouble KD_APIENTRY kdDifftime ( KDtime end, KDtime beginning )
{
	return difftime ( (time_t) end, (time_t) beginning );
}

// Convert KDTm structure to KDtime
// XMSoft's revision : Added API.
KD_API KDtime KD_APIENTRY kdMktime ( struct KDTm* timeptr )
{
	struct tm  t;

	t.tm_sec   = timeptr->tm_sec;
	t.tm_min   = timeptr->tm_min;
	t.tm_hour  = timeptr->tm_hour;
	t.tm_mday  = timeptr->tm_mday;
	t.tm_mon   = timeptr->tm_mon;
	t.tm_year  = timeptr->tm_year;
	t.tm_wday  = timeptr->tm_wday;
	t.tm_yday  = timeptr->tm_yday;
	t.tm_isdst = timeptr->tm_isdst;

	return (KDtime) mktime ( &t );
}

static KDint get_utc_offset ( KDvoid ) 
{
	KDtime	zero = 24 * 60 * 60L;
	KDTm	tm;
	KDint	gmtime_hours;

	// get the local time for Jan 2, 1900 00:00 UTC 
	kdLocaltime_r ( &zero, &tm );
	gmtime_hours = tm.tm_hour;

	// if the local time is the "day before" the UTC, subtract 24 hours from the hours to get the UTC offset 
	if ( tm.tm_mday < 2 )
	{
		gmtime_hours -= 24;
	}

	return gmtime_hours;
}

// Convert KDTm structure to KDtime
// XMSoft's revision : Added API.
KD_API KDtime KD_APIENTRY kdTimegm ( struct KDTm* timeptr )
{
	// gets the epoch time relative to the local time zone,
	// and then adds the appropriate number of seconds to make it UTC 
	return kdMktime ( timeptr ) + get_utc_offset ( ) * 3600;
}

KD_API KDsize KD_APIENTRY kdStrftime ( KDchar* ptr, KDsize maxsize, const KDchar* format, const struct KDTm* timeptr )
{
	struct tm  t;

	t.tm_sec   = timeptr->tm_sec;
	t.tm_min   = timeptr->tm_min;
	t.tm_hour  = timeptr->tm_hour;
	t.tm_mday  = timeptr->tm_mday;
	t.tm_mon   = timeptr->tm_mon;
	t.tm_year  = timeptr->tm_year;
	t.tm_wday  = timeptr->tm_wday;
	t.tm_yday  = timeptr->tm_yday;
	t.tm_isdst = timeptr->tm_isdst;

	return strftime ( ptr, maxsize, format, &t );
}

#if defined(_WIN32)

static KDfloat64	l_start = 0;
static KDfloat64	l_invFrequency = 0.0;

KDvoid xmInitTickCount ( KDvoid )
{
	LARGE_INTEGER	largeInteger;

	if ( l_invFrequency == 0.0 )
	{
		QueryPerformanceFrequency ( &largeInteger );
		l_invFrequency = (KDfloat64) ( largeInteger.QuadPart );
		if ( l_invFrequency > 0.0 )
		{
			l_invFrequency = 1000.0f / l_invFrequency;
		}
	}

	QueryPerformanceCounter ( &largeInteger );
	l_start = (KDfloat64) ( largeInteger.QuadPart );
}

KD_API KDvoid KD_APIENTRY kdResetTickCount ( KDvoid )
{
	LARGE_INTEGER  largeInteger;
	QueryPerformanceCounter ( &largeInteger );
	l_start = (KDfloat64) ( largeInteger.QuadPart );
}

KD_API KDuint KD_APIENTRY kdGetTickCount ( KDvoid )
{
	LARGE_INTEGER	largeInteger;
	KDfloat64		count;

	QueryPerformanceCounter ( &largeInteger );
	count = (KDfloat64) ( largeInteger.QuadPart );

	return (KDuint) ( l_invFrequency * ( count - l_start ) );
}

KD_API KDdouble KD_APIENTRY kdGetMilliseconds ( KDvoid )
{
	LARGE_INTEGER	largeInteger;
	KDfloat64		count;

	QueryPerformanceCounter ( &largeInteger );
	count = (KDfloat64) ( largeInteger.QuadPart );

	return l_invFrequency * ( count - l_start );
}

#else

static unsigned long	l_start_sec  = 0;
static unsigned long	l_start_msec = 0;

KDvoid xmInitTickCount ( KDvoid )
{
	kdResetTickCount ( );
}

KD_API KDvoid KD_APIENTRY kdResetTickCount ( KDvoid )
{
    struct timeval	 t;
    
    gettimeofday ( &t, 0 );

    l_start_sec  = t.tv_sec;
    l_start_msec = t.tv_usec * 0.001f;
}

KD_API KDuint KD_APIENTRY kdGetTickCount ( KDvoid )
{
    struct timeval  t;
    gettimeofday ( &t, 0 );
    return (KDuint) ( ( t.tv_sec - l_start_sec ) * 1000 + t.tv_usec * 0.001f - l_start_msec );
}

KD_API KDdouble KD_APIENTRY kdGetMilliseconds ( KDvoid )
{
    struct timeval  t;
    gettimeofday ( &t, 0 );
    return ( t.tv_sec - l_start_sec ) * 1000 + t.tv_usec * 0.001 - l_start_msec;
}

#endif

