/* --------------------------------------------------------------------------
 *
 *      File            win32_time.c
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

#include "../kd_library.h"

// xmPlatformGetTimeUST : Get the current unadjusted system time in platform specific.
KDust xmPlatformGetTimeUST ( KDvoid )
{	
	LARGE_INTEGER       count; 
	LARGE_INTEGER	    frequency; 
	
	QueryPerformanceCounter ( &count );
	QueryPerformanceFrequency ( &frequency );    

	return (KDust) ( count.QuadPart * ( 1000000000.0 / frequency.QuadPart ) );
}

// xmPlatformUSTAtEpoch : Get the UST corresponding to KDtime 0 in platform specific.
KDust xmPlatformUSTAtEpoch ( KDvoid )
{
	LARGE_INTEGER       count; 
	LARGE_INTEGER	    frequency; 
	
	QueryPerformanceCounter ( &count );
	QueryPerformanceFrequency ( &frequency );   

	count.QuadPart -= 0x19db1ded53e8000;
	
	return (KDust) ( count.QuadPart * ( 1000000000.0 / frequency.QuadPart ) );
}

int xmWinGettimeofday ( struct timeval* tv, struct timezone* tz )
{
#if defined ( _WIN32_WCE )
    SYSTEMTIME  wtm;
	struct tm   tTm;

    GetLocalTime ( &wtm );

    tTm.tm_year	 = wtm.wYear - 1900;
    tTm.tm_mon	 = wtm.wMonth - 1;
    tTm.tm_mday	 = wtm.wDay;
    tTm.tm_hour	 = wtm.wHour;
    tTm.tm_min	 = wtm.wMinute;
    tTm.tm_sec	 = wtm.wSecond;
	tTm.tm_isdst = -1;
	tv->tv_sec	 = GetTickCount ( ) / 1000;
	tv->tv_usec	 = wtm.wMilliseconds * 1000;
#else
	struct _timeb		tb;
    _tzset ( );
    _ftime ( &tb );
    if ( tv )
	{
		tv->tv_sec  = (long) tb.time;
		tv->tv_usec = tb.millitm * 1000;
    }
    if ( tz )
	{
		tz->tz_minuteswest = tb.timezone;
		tz->tz_dsttime = tb.dstflag;
    }
#endif

    return 0;
}

int xmWinSettimeofday ( const struct timeval* tv, const struct timezone* tz )
{
	SYSTEMTIME		systime;
    struct tm*		newtime;

    newtime = gmtime ( (time_t*) &( tv->tv_sec ) );
    systime.wYear	= 1900 + newtime->tm_year;
    systime.wMonth	= 1 + newtime->tm_mon;
    systime.wDay	= newtime->tm_mday;
    systime.wHour	= newtime->tm_hour;
    systime.wMinute = newtime->tm_min;
    systime.wSecond = newtime->tm_sec;
    systime.wMilliseconds = 0;

    return SetSystemTime ( &systime );
}

// xmPlatformSleep : The function sleep gives a simple way to make the program wait for a short interval.
KDvoid xmPlatformSleep ( KDust ust )
{
	Sleep ( KD_N2M_SEC ( ust ) );
}

#ifdef _WIN32_WCE

/*
* $Id: wce_localtime.c,v 1.2 2006/04/09 16:48:18 mloskot Exp $
*
* Defines localtime() function.
*
* Created by Mateusz Loskot (mateusz@loskot.net)
*
* Copyright (c) 2006 Taxus SI Ltd.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom 
* the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
* THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* MIT License:
* http://opensource.org/licenses/mit-license.php
*
* Contact:
* Taxus SI Ltd.
* http://www.taxussi.com.pl
*
*/

/*******************************************************************************
	Constants and macros used internally
*******************************************************************************/

#define SECS_PER_MIN		60
#define MINS_PER_HOUR		60
#define HOURS_PER_DAY		24
#define DAYS_PER_WEEK		7
#define DAYS_PER_NYEAR		365
#define DAYS_PER_LYEAR		366
#define SECS_PER_HOUR		( SECS_PER_MIN * MINS_PER_HOUR )
#define SECS_PER_DAY		( (long) SECS_PER_HOUR * HOURS_PER_DAY )
#define MONS_PER_YEAR		12

#define TM_SUNDAY			0
#define TM_MONDAY			1
#define TM_TUESDAY			2
#define TM_WEDNESDAY		3
#define TM_THURSDAY			4
#define TM_FRIDAY			5
#define TM_SATURDAY			6

#define TM_JANUARY			0
#define TM_FEBRUARY			1
#define TM_MARCH			2
#define TM_APRIL			3
#define TM_MAY				4
#define TM_JUNE				5
#define TM_JULY				6
#define TM_AUGUST			7
#define TM_SEPTEMBER		8
#define TM_OCTOBER			9
#define TM_NOVEBER			10
#define TM_DECEMBER			11
#define TM_SUNDAY			0

#define TM_YEAR_BASE		1900

#define EPOCH_YEAR			1970
#define EPOCH_WDAY			TM_THURSDAY

#define isleap(y)			( ( (y) % 4 ) == 0 && ( (y) % 100) != 0 || ( (y) % 400 ) == 0 )

static int	mon_lengths[2][MONS_PER_YEAR] =
{
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
	31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int	year_lengths[2] = { DAYS_PER_NYEAR, DAYS_PER_LYEAR };

struct tm * __wceex_offtime ( const time_t *timer, long tzoffset )
{
	register struct tm* tmp;
	register long		days;
	register long		rem;
	register int		y;
	register int		yleap;
	register int*		ip;
	static struct tm    tm;

	tmp  = &tm;
	days = (long) ( *timer / SECS_PER_DAY );
	rem  = (long) ( *timer % SECS_PER_DAY );
	rem += tzoffset;
	while ( rem < 0 )
	{
		rem += SECS_PER_DAY;
		--days;
	}

	while ( rem >= SECS_PER_DAY )
	{
		rem -= SECS_PER_DAY;
		++days;
	}
	tmp->tm_hour = (int) ( rem / SECS_PER_HOUR );

	rem = rem % SECS_PER_HOUR;
	tmp->tm_min  = (int) ( rem / SECS_PER_MIN );
	tmp->tm_sec  = (int) ( rem % SECS_PER_MIN );
	tmp->tm_wday = (int) ( ( EPOCH_WDAY + days ) % DAYS_PER_WEEK );

	if ( tmp->tm_wday < 0 )
	{
		tmp->tm_wday += DAYS_PER_WEEK;
	}
	y = EPOCH_YEAR;

	if ( days >= 0 )
	{
		for ( ; ; )
		{
			yleap = isleap( y );
			if ( days < (long) year_lengths[yleap] )
			{
				break;
			}

			++y;
			days = days - (long) year_lengths[yleap];
		}
	}
	else
	{
		do
		{
			--y;
			yleap = isleap( y );
			days = days + (long) year_lengths[yleap];
		} while ( days < 0 );
	}

	tmp->tm_year = y - TM_YEAR_BASE;
	tmp->tm_yday = (int) days;
	ip = mon_lengths[yleap];

	for ( tmp->tm_mon = 0; days >= (long) ip[tmp->tm_mon]; ++( tmp->tm_mon ) )
	{
		days = days - (long) ip[tmp->tm_mon];
	}

	tmp->tm_mday = (int) (days + 1);
	tmp->tm_isdst = 0;

	return tmp;
}

time_t __wceex_tm_to_time_t ( const struct tm *tmbuff )
{
    time_t timer;

    // If the year is <1970 or the value is negative, the relationship is undefined  
    if (tmbuff->tm_year < 70)
    {
        return (time_t) -1;
    }
    
    // If the year is >=1970 
    // Each and every day shall be accounted for by exactly 86400 seconds 

    timer = tmbuff->tm_sec
        + tmbuff->tm_min * 60							// convert minutes to seconds 
        + tmbuff->tm_hour * 3600						// convert hours to seconds 
        + tmbuff->tm_yday * 86400						// convert day of year to seconds 
        + (tmbuff->tm_year - 70) * 31536000				// convert year to seconds 
        + ((tmbuff->tm_year - 69) / 4) * 86400			// add a day (seconds) every 4 years starting in 1973 
        - ((tmbuff->tm_year - 1) / 100) * 86400			// subtract a day back out every 100 years starting in 2001 
        + ((tmbuff->tm_year + 299) / 400) * 86400;		// add a day back in every 400 years starting in 2001 

    return timer;
}

struct tm* xmWinGmtime ( const time_t * _Time )
{
	register struct tm*		tmp;

	tmp = __wceex_offtime( _Time, 0L );

	return tmp;
}

struct tm* xmWinLocaltime ( const time_t * _Time )
{
	register struct tm*		tmp;
	long					tzoffset;
	TIME_ZONE_INFORMATION	tzi;

	// Retrive timezone offset in seconds
	tzoffset = 0;
	if ( GetTimeZoneInformation( &tzi ) != 0xFFFFFFFF )
	{
		tzoffset += ( tzi.Bias * 60 );
		if ( tzi.StandardDate.wMonth != 0 )
		{
			tzoffset += ( tzi.StandardBias * 60 );
		}
	}

	tzoffset *= -1;
	tmp = __wceex_offtime( _Time, tzoffset );

	return tmp;
}

time_t xmWinMktime ( struct tm* timeptr )
{
	return __wceex_tm_to_time_t ( timeptr );
}

time_t xmWinTime ( time_t* inTT )
{ 
	SYSTEMTIME		sysTimeStruct; 
	FILETIME		fTime; 
	ULARGE_INTEGER	int64time; 
	time_t			locTT = 0; 

	if( inTT == NULL )
	{ 
		inTT = &locTT; 
	} 

	GetSystemTime( &sysTimeStruct ); 
	if( SystemTimeToFileTime( &sysTimeStruct, &fTime ) )
	{ 
		memcpy( &int64time, &fTime, sizeof( FILETIME ) ); 
		// Subtract the value for 1970-01-01 00:00 (UTC) 
		int64time.QuadPart -= 0x19db1ded53e8000; 
		// Convert to seconds. 
		int64time.QuadPart /= 10000000; 
		*inTT = (time_t) int64time.QuadPart; 
	} 

	return *inTT; 
} 


/* strftime() - taken from OpenBSD.  */

#define IN_NONE	0
#define IN_SOME	1
#define IN_THIS	2
#define IN_ALL	3
#define CHAR_BIT      8

#define TYPE_BIT(type)	(sizeof (type) * CHAR_BIT)
#define TYPE_SIGNED(type) (((type) -1) < 0)

#define INT_STRLEN_MAXIMUM(type) \
    ((TYPE_BIT(type) - TYPE_SIGNED(type)) * 302 / 1000 + 1 + TYPE_SIGNED(type))

//#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

#define MONSPERYEAR	12
#define DAYSPERWEEK	7
#define TM_YEAR_BASE	1900
#define HOURSPERDAY	24
#define DAYSPERNYEAR	365
#define DAYSPERLYEAR	366

static char wildabbr[] = "WILDABBR";

static char *tzname[2] = 
{
  wildabbr,
  wildabbr
};


#define Locale	(&C_time_locale)

struct lc_time_T
{
  const char *mon[MONSPERYEAR];
  const char *month[MONSPERYEAR];
  const char *wday[DAYSPERWEEK];
  const char *weekday[DAYSPERWEEK];
  const char *X_fmt;
  const char *x_fmt;
  const char *c_fmt;
  const char *am;
  const char *pm;
  const char *date_fmt;
};

static const struct lc_time_T C_time_locale = {
  {
   "Jan", "Feb", "Mar", "Apr", "May", "Jun",
   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  }, {
   "January", "February", "March", "April", "May", "June",
   "July", "August", "September", "October", "November", "December"
  }, {
   "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  }, {
   "Sunday", "Monday", "Tuesday", "Wednesday", 
   "Thursday", "Friday", "Saturday"
  },

  /* X_fmt */
  "%H:%M:%S",

  /*
   ** x_fmt
   ** C99 requires this format.
   ** Using just numbers (as here) makes Quakers happier;
   ** it's also compatible with SVR4.
   */
  "%m/%d/%y",

  /*
   ** c_fmt
   ** C99 requires this format.
   ** Previously this code used "%D %X", but we now conform to C99.
   ** Note that
   **      "%a %b %d %H:%M:%S %Y"
   ** is used by Solaris 2.3.
   */
  "%a %b %e %T %Y",

  /* am */
  "AM",

  /* pm */
  "PM",

  /* date_fmt */
  "%a %b %e %H:%M:%S %Z %Y"
};


static char *
_add (const char *str, char *pt, const char *const ptlim)
{
  while (pt < ptlim && (*pt = *str++) != '\0')
    ++pt;
  return pt;
}


static char *
_conv (const int n, const char *const format, char *const pt,
       const char *const ptlim)
{
  char buf[INT_STRLEN_MAXIMUM (int) + 1];

  (void) _snprintf (buf, sizeof (buf), format, n);
  return _add (buf, pt, ptlim);
}


static char *
_fmt (const char *format, const struct tm *const t, char *pt,
      const char *const ptlim, int *warnp)
{
  for (; *format; ++format)
    {
      if (*format == '%')
        {
        label:
          switch (*++format)
            {
            case '\0':
              --format;
              break;
            case 'A':
              pt = _add ((t->tm_wday < 0 ||
                          t->tm_wday >= DAYSPERWEEK) ?
                         "?" : Locale->weekday[t->tm_wday], pt, ptlim);
              continue;
            case 'a':
              pt = _add ((t->tm_wday < 0 ||
                          t->tm_wday >= DAYSPERWEEK) ?
                         "?" : Locale->wday[t->tm_wday], pt, ptlim);
              continue;
            case 'B':
              pt = _add ((t->tm_mon < 0 ||
                          t->tm_mon >= MONSPERYEAR) ?
                         "?" : Locale->month[t->tm_mon], pt, ptlim);
              continue;
            case 'b':
            case 'h':
              pt = _add ((t->tm_mon < 0 ||
                          t->tm_mon >= MONSPERYEAR) ?
                         "?" : Locale->mon[t->tm_mon], pt, ptlim);
              continue;
            case 'C':
              /*
               ** %C used to do a...
               **     _fmt("%a %b %e %X %Y", t);
               ** ...whereas now POSIX 1003.2 calls for
               ** something completely different.
               ** (ado, 1993-05-24)
               */
              pt = _conv ((t->tm_year + TM_YEAR_BASE) / 100,
                          "%02d", pt, ptlim);
              continue;
            case 'c':
              {
                int warn2 = IN_SOME;

                pt = _fmt (Locale->c_fmt, t, pt, ptlim, warnp);
                if (warn2 == IN_ALL)
                  warn2 = IN_THIS;
                if (warn2 > *warnp)
                  *warnp = warn2;
              }
              continue;
            case 'D':
              pt = _fmt ("%m/%d/%y", t, pt, ptlim, warnp);
              continue;
            case 'd':
              pt = _conv (t->tm_mday, "%02d", pt, ptlim);
              continue;
            case 'E':
            case 'O':
              /*
               ** C99 locale modifiers.
               ** The sequences
               **     %Ec %EC %Ex %EX %Ey %EY
               **     %Od %oe %OH %OI %Om %OM
               **     %OS %Ou %OU %OV %Ow %OW %Oy
               ** are supposed to provide alternate
               ** representations.
               */
              goto label;
            case 'e':
              pt = _conv (t->tm_mday, "%2d", pt, ptlim);
              continue;
            case 'F':
              pt = _fmt ("%Y-%m-%d", t, pt, ptlim, warnp);
              continue;
            case 'H':
              pt = _conv (t->tm_hour, "%02d", pt, ptlim);
              continue;
            case 'I':
              pt = _conv ((t->tm_hour % 12) ?
                          (t->tm_hour % 12) : 12, "%02d", pt, ptlim);
              continue;
            case 'j':
              pt = _conv (t->tm_yday + 1, "%03d", pt, ptlim);
              continue;
            case 'k':
              /*
               ** This used to be...
               **     _conv(t->tm_hour % 12 ?
               **             t->tm_hour % 12 : 12, 2, ' ');
               ** ...and has been changed to the below to
               ** match SunOS 4.1.1 and Arnold Robbins'
               ** strftime version 3.0.  That is, "%k" and
               ** "%l" have been swapped.
               ** (ado, 1993-05-24)
               */
              pt = _conv (t->tm_hour, "%2d", pt, ptlim);
              continue;
#ifdef KITCHEN_SINK
            case 'K':
              /*
               ** After all this time, still unclaimed!
               */
              pt = _add ("kitchen sink", pt, ptlim);
              continue;
#endif /* defined KITCHEN_SINK */
            case 'l':
              /*
               ** This used to be...
               **     _conv(t->tm_hour, 2, ' ');
               ** ...and has been changed to the below to
               ** match SunOS 4.1.1 and Arnold Robbin's
               ** strftime version 3.0.  That is, "%k" and
               ** "%l" have been swapped.
               ** (ado, 1993-05-24)
               */
              pt = _conv ((t->tm_hour % 12) ?
                          (t->tm_hour % 12) : 12, "%2d", pt, ptlim);
              continue;
            case 'M':
              pt = _conv (t->tm_min, "%02d", pt, ptlim);
              continue;
            case 'm':
              pt = _conv (t->tm_mon + 1, "%02d", pt, ptlim);
              continue;
            case 'n':
              pt = _add ("\n", pt, ptlim);
              continue;
            case 'p':
              pt = _add ((t->tm_hour >= (HOURSPERDAY / 2)) ?
                         Locale->pm : Locale->am, pt, ptlim);
              continue;
            case 'R':
              pt = _fmt ("%H:%M", t, pt, ptlim, warnp);
              continue;
            case 'r':
              pt = _fmt ("%I:%M:%S %p", t, pt, ptlim, warnp);
              continue;
            case 'S':
              pt = _conv (t->tm_sec, "%02d", pt, ptlim);
              continue;
            case 's':
              {
                struct tm tm;
                char buf[INT_STRLEN_MAXIMUM (time_t) + 1];
                time_t mkt;

                tm = *t;
                mkt = mktime (&tm);
                if (TYPE_SIGNED (time_t))
                  (void) _snprintf (buf, sizeof buf, "%ld", (long) mkt);
                else
                  (void) _snprintf (buf, sizeof buf,
                                    "%lu", (unsigned long) mkt);
                pt = _add (buf, pt, ptlim);
              }
              continue;
            case 'T':
              pt = _fmt ("%H:%M:%S", t, pt, ptlim, warnp);
              continue;
            case 't':
              pt = _add ("\t", pt, ptlim);
              continue;
            case 'U':
              pt = _conv ((t->tm_yday + DAYSPERWEEK -
                           t->tm_wday) / DAYSPERWEEK, "%02d", pt, ptlim);
              continue;
            case 'u':
              /*
               ** From Arnold Robbins' strftime version 3.0:
               ** "ISO 8601: Weekday as a decimal number
               ** [1 (Monday) - 7]"
               ** (ado, 1993-05-24)
               */
              pt = _conv ((t->tm_wday == 0) ?
                          DAYSPERWEEK : t->tm_wday, "%d", pt, ptlim);
              continue;
            case 'V':          /* ISO 8601 week number */
            case 'G':          /* ISO 8601 year (four digits) */
            case 'g':          /* ISO 8601 year (two digits) */
              {
                int year;
                int yday;
                int wday;
                int w;

                year = t->tm_year + TM_YEAR_BASE;
                yday = t->tm_yday;
                wday = t->tm_wday;
                for (;;)
                  {
                    int len;
                    int bot;
                    int top;

                    len = isleap (year) ? DAYSPERLYEAR : DAYSPERNYEAR;
                    /*
                     ** What yday (-3 ... 3) does
                     ** the ISO year begin on?
                     */
                    bot = ((yday + 11 - wday) % DAYSPERWEEK) - 3;
                    /*
                     ** What yday does the NEXT
                     ** ISO year begin on?
                     */
                    top = bot - (len % DAYSPERWEEK);
                    if (top < -3)
                      top += DAYSPERWEEK;
                    top += len;
                    if (yday >= top)
                      {
                        ++year;
                        w = 1;
                        break;
                      }
                    if (yday >= bot)
                      {
                        w = 1 + ((yday - bot) / DAYSPERWEEK);
                        break;
                      }
                    --year;
                    yday += isleap (year) ? DAYSPERLYEAR : DAYSPERNYEAR;
                  }
                if (*format == 'V')
                  pt = _conv (w, "%02d", pt, ptlim);
                else if (*format == 'g')
                  {
                    *warnp = IN_ALL;
                    pt = _conv (year % 100, "%02d", pt, ptlim);
                  }
                else
                  pt = _conv (year, "%04d", pt, ptlim);
              }
              continue;
            case 'v':
              pt = _fmt ("%e-%b-%Y", t, pt, ptlim, warnp);
              continue;
            case 'W':
              pt = _conv ((t->tm_yday + DAYSPERWEEK -
                           (t->tm_wday ?
                            (t->tm_wday - 1) :
                            (DAYSPERWEEK - 1))) / DAYSPERWEEK,
                          "%02d", pt, ptlim);
              continue;
            case 'w':
              pt = _conv (t->tm_wday, "%d", pt, ptlim);
              continue;
            case 'X':
              pt = _fmt (Locale->X_fmt, t, pt, ptlim, warnp);
              continue;
            case 'x':
              {
                int warn2 = IN_SOME;

                pt = _fmt (Locale->x_fmt, t, pt, ptlim, &warn2);
                if (warn2 == IN_ALL)
                  warn2 = IN_THIS;
                if (warn2 > *warnp)
                  *warnp = warn2;
              }
              continue;
            case 'y':
              *warnp = IN_ALL;
              pt = _conv ((t->tm_year + TM_YEAR_BASE) % 100,
                          "%02d", pt, ptlim);
              continue;
            case 'Y':
              pt = _conv (t->tm_year + TM_YEAR_BASE, "%04d", pt, ptlim);
              continue;
            case 'Z':
              if (t->tm_isdst >= 0)
                pt = _add (tzname[t->tm_isdst != 0], pt, ptlim);
              /*
               ** C99 says that %Z must be replaced by the
               ** empty string if the time zone is not
               ** determinable.
               */
              continue;
            case 'z':
              {
                int diff;
                char const *sign;

                if (t->tm_isdst < 0)
                  continue;
                continue;
                if (diff < 0)
                  {
                    sign = "-";
                    diff = -diff;
                  }
                else
                  sign = "+";
                pt = _add (sign, pt, ptlim);
                diff /= 60;
                pt = _conv ((diff / 60) * 100 + diff % 60, "%04d", pt, ptlim);
              }
              continue;
            case '+':
              pt = _fmt (Locale->date_fmt, t, pt, ptlim, warnp);
              continue;
            case '%':
            default:
              break;
            }
        }
      if (pt == ptlim)
        break;
      *pt++ = *format;
    }
  return pt;
}

size_t xmWinStrftime ( char *const s, const size_t maxsize, const char *const format, const struct tm * const t )
{
	char *p;
	int warn;

	warn = IN_NONE;
	p = _fmt (((format == NULL) ? "%c" : format), t, s, s + maxsize, &warn);

	if (p == s + maxsize)
	{
		if (maxsize > 0)
		s[maxsize - 1] = '\0';
		return 0;
	}
	*p = '\0';
	return p - s;
}

#endif	// _WIN32_WCE