/* --------------------------------------------------------------------------
 *
 *      File            kd_io.c
 *      Description     Input / output
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

// kdStateGeti, kdStateGetl, kdStateGetf : get state value(s)
KD_API KDint KD_APIENTRY kdStateGeti ( KDint startidx, KDuint numidxs, KDint32* buffer )
{
	KDint32        value = 0;

	switch ( startidx )
	{
		#ifdef XM_HAS_PLATFORM_KEYPAD

		case KD_STATE_PHONEKEYPAD_AVAILABILITY	:	value = 0x7fff;		break;
		case KD_STATE_GAMEKEYS_AVAILABILITY		:	value = 511;		break;
		case KD_STATE_GAMEKEYSNC_AVAILABILITY	:	value = 511;		break;

		#endif

		case KD_STATE_POINTER_AVAILABILITY		:	value = 7;			break;
	}

	kdMemcpy ( buffer, &value, sizeof ( KDint32 ) * numidxs );

	return numidxs;
}

KD_API KDint KD_APIENTRY kdStateGetl(KDint startidx, KDuint numidxs, KDint64 *buffer)
{
	KDint64        value = 0;

	switch ( startidx )
	{
		#ifdef XM_HAS_PLATFORM_KEYPAD

		case KD_STATE_PHONEKEYPAD_AVAILABILITY	:	value = 0x7fff;		break;
		case KD_STATE_GAMEKEYS_AVAILABILITY		:	value = 511;		break;
		case KD_STATE_GAMEKEYSNC_AVAILABILITY	:	value = 511;		break;

		#endif

		case KD_STATE_POINTER_AVAILABILITY		:	value = 7;			break;
	}

	kdMemcpy ( buffer, &value, sizeof ( KDint64 ) * numidxs );

	return numidxs;
}

KD_API KDint KD_APIENTRY kdStateGetf(KDint startidx, KDuint numidxs, KDfloat32 *buffer)
{
	KDfloat32        value = 0;

	switch ( startidx )
	{
		#ifdef XM_HAS_PLATFORM_KEYPAD

		case KD_STATE_PHONEKEYPAD_AVAILABILITY	:	value = 0x7fff;		break;
		case KD_STATE_GAMEKEYS_AVAILABILITY		:	value = 511;		break;
		case KD_STATE_GAMEKEYSNC_AVAILABILITY	:	value = 511;		break;

		#endif

		case KD_STATE_POINTER_AVAILABILITY		:	value = 7;			break;
	}

	kdMemcpy ( buffer, &value, sizeof ( KDfloat32 ) * numidxs );

	return numidxs;
}

// kdOutputSeti, kdOutputSetf : set outputs
KD_API KDint KD_APIENTRY kdOutputSeti ( KDint startidx, KDuint numidxs, const KDint32* buffer )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

KD_API KDint KD_APIENTRY kdOutputSetf ( KDint startidx, KDuint numidxs, const KDfloat32 *buffer )
{
	kdSetError( KD_ENOSYS );

	return -1;
}
