/* --------------------------------------------------------------------------
 *
 *      File            bada_scanf.c
 *      Description     Read formatted input from a buffer or file.
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

// xmBadaVsscanf : Read formatted input from a buffer or file.
KDint xmBadaVsscanf ( const KDchar* str, const KDchar* format, va_list ap )
{
	KDchar*  s = (KDchar *) str;
	KDchar*  s0;

	KDint    d;
	KDuint   u;
	KDfloat  f;
	KDint    i;

	KDint*   ip;
	KDuint*  up;
	KDfloat* fp;
	KDchar*  cp;

	KDint    rc = 0;

	for ( ; ; )
	{
		for ( ; ; )
		{
			switch ( i = *(KDuchar *) format++ )
			{
				case 0 :

					goto done;

				case '%' :

					break;

				default :

					if ( i <= ' ' )
					{
						while ( *s <= ' ' )
						{
							if ( !*s++ )
								return rc;
						}
					}
					else if ( *s++ != i )
					{
						return rc;
					}

					continue;
			}

			switch ( *format++ )
			{
				case 'd' :	case 'D' :
				case 'i' :	case 'I' :

					ip = va_arg ( ap, KDint* );

					d = kdStrtol ( s0 = s, &s, 10 );

					if ( s > s0 )
					{
						rc++;
						*ip = d;
						continue;
					}

					return rc;

				case 'u' :	case 'U' :

					up = va_arg ( ap, KDuint* );

					u = kdStrtoul ( s0 = s, &s, 10 );

					if ( s > s0 )
					{
						rc++;
						*up = u;
						continue;
					}

					return rc;

				case 'e' :	case 'E' :
				case 'g' :  case 'G' :
				case 'f' :	case 'F' :

					fp = va_arg ( ap, KDfloat* );

					f = kdStrtof ( s0 = s, &s );

					if ( s > s0 )
					{
						rc++;
						*fp = f;
						continue;
					}

					return rc;

				case 'o' :	case 'O' :

					ip = va_arg ( ap, KDint* );

					d = kdStrtol ( s0 = s, &s, 8 );

					if ( s > s0 )
					{
						rc++;
						*ip = d;
						continue;
					}

					return rc;

				case 'x' :	case 'X' :

					ip = va_arg ( ap, KDint* );

					d = kdStrtol ( s0 = s, &s, 16 );

					if ( s > s0 )
					{
						rc++;
						*ip = d;
						continue;
					}

					return rc;


				case 'c' :

					cp = va_arg ( ap, KDchar* );

					if ( *s == '\0' )
					{
						return rc;
					}

					rc++;
					*cp = *s;
					*s++;

					continue;

				case 's' :

					cp = va_arg ( ap, KDchar* );

					if ( *s == '\0' )
					{
						return rc;
					}

					for ( ; ; )
					{
						if ( *s == ' ' || *s == '\0' )
						{
							rc++;
							*cp = '\0';
							break;
						}

						*cp = *s;
						*cp++;
						*s++;
					}

					continue;

				default :
					continue;
			}
		}

done :
		return rc;
	}
}
