/* --------------------------------------------------------------------------
 *
 *      File            win32_vscanf.c
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

// xmWinVsscanf, xmWinVscanf, xmWinVfscanf : Read formatted input from a buffer or file. 
static int xmWinVscanfGeneric ( const char* str, FILE* file, const char* p_format, va_list arg_list )
{
    char*   nformat = _strdup ( p_format );
    char*   format  = nformat;
    int     convtot = 0;
    int     conv    = EOF;
	int     n       = 0;
	int     i       = 0;    

    for ( i = 0; format[ i ]; i++ )
	{
        if ( format[ i ] == '%' )
		{
            if ( format[ i + 1 ] == '%' || format[ i + 1 ] == '*' || format[ i + 1 ] == '\0' )
			{
                i++; 
			}
            else
			{ 
                if ( n == 0 ) 
				{
                    n++; 
				}
                else 
				{                    
                    void*   p  = va_arg ( arg_list, void * );
                    int     lu = -1;
					int     conv;
					char    f[3];

                    f[ 1 ] = format[ i + 1 ];
					f[ 2 ] = format[ i + 2 ];

                    format[ i + 1 ] = 'n';
					format[ i + 2 ] ='\0';

                    conv = ( str ? sscanf ( str, format, p, &lu ) : file ? fscanf ( file, format, p, &lu ) : scanf ( format, p, &lu ) );
                    if ( lu == -1 )
					{ 
                        convtot += ( conv >= 0 ? conv : 0 ); 
                        break; 
                    }

                    convtot++; 

                    if ( str )
					{
                        str += lu; 
					}

                    format[ i + 1 ] = f[ 1 ];
					format[ i + 2 ] = f[ 2 ]; 
                    format += i;
					i = -1;
                    n = 0; 
                }
            }
        }
    }

    if ( format[ i ] == '\0' && n > 0 ) 
	{       
        void*   p = va_arg ( arg_list, void * );
        int     conv;

        conv = ( str ? sscanf ( str, format, p ) : file ? fscanf ( file, format, p ) : scanf ( format, p ) );
        convtot += ( conv >= 0 ? conv : 0 );
    }

    free ( nformat );

    return ( convtot > 0 ? convtot : conv );
}

int	xmWinVsscanf ( const char* str, const char* format, va_list arg_list )
{
	return xmWinVscanfGeneric ( str, 0, format, arg_list );
}

int xmWinVscanf ( const char* format, va_list arg_list )
{   
    return xmWinVscanfGeneric ( 0, 0, format, arg_list );
}

int	xmWinVfscanf ( FILE* file, const char* format, va_list arg_list )
{
	return xmWinVscanfGeneric ( 0, file, format, arg_list );
}

