/* --------------------------------------------------------------------------
 *
 *      File            win32_crypto.c
 *      Description     Crypto random function.
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

#ifdef XM_HAS_PLATFORM_CRYPTO

static HCRYPTPROV   l_crypt_prov = 0;			    // handle for a cryptographic provider context							  

KDint xmPlatformCryptoInit ( KDvoid )
{
	TCHAR*      username = _T( "XMKode Crypto" );	// name of the key container to be used
	KDint       ret = 0;

	//-------------------------------------------------------------------
	// Attempt to acquire a co7ntext and a key
	// container. The context will use the default CSP
	// for the RSA_FULL provider type. DwFlags is set to zero
	// to attempt to open an existing key container.
	if ( CryptAcquireContext(
		&l_crypt_prov,				// handle to the CSP
		username,					// container name 
		NULL,						// use the default provider
		PROV_RSA_FULL,				// provider type
		0 ) )						// flag values
	{
		ret = 1;
		kdLogMessage ( "A cryptographic context with the \"XMKode Crypto\" key container has been acquired." );
	}
	else
	{ 
		//-------------------------------------------------------------------
		// An error occurred in acquiring the context. This could mean
		// that the key container requested does not exist. In this case,
		// the function can be called again to attempt to create a new key 
		// container. Error codes are defined in Winerror.h.
		if ( GetLastError() == NTE_BAD_KEYSET )
		{
			if ( CryptAcquireContext(
				&l_crypt_prov, 
				username, 
				NULL, 
				PROV_RSA_FULL, 
				CRYPT_NEWKEYSET ) ) 
			{			
				ret = 1;
				kdLogMessage ( "A new key container has been created." );				
			}
			else
			{
				kdLogMessage ( "Could not create a new key container." );
			}
		}
		else
		{
			kdLogMessage ( "A cryptographic service handle could not be acquired." );
		}
	} 

	return ret;
}

KDvoid xmPlatformCryptoFree ( KDvoid )
{
	//-------------------------------------------------------------------
	// A cryptographic context and a key container are available. Perform
	// any functions that require a cryptographic provider handle.

	//-------------------------------------------------------------------
	// When the handle is no longer needed, it must be released.
	if ( l_crypt_prov )
	{
		if ( CryptReleaseContext ( l_crypt_prov, 0 ) )
		{
			kdLogMessage ( "The handle has been released." );
		}
		else
		{
			kdLogMessage ( "The handle could not be released." );
		}
	}
}

// xmPlatformCryptoRand : Return random data in in platform specific.
KDint xmPlatformCryptoRand ( KDuint8* buf, KDsize buflen )
{
	//--------------------------------------------------------------------
	//  This code assumes that a cryptographic context has been acquired 
	//  For code details, see "Example C Program: Duplicating a Session 
	//  Key."

	//--------------------------------------------------------------------
	// Generate a random initialization vector.
	KDint     ret = -1;

	if ( CryptGenRandom ( l_crypt_prov, buflen, buf ) ) 
	{
		ret = 0;
	}

	return ret;
}

#endif