/* --------------------------------------------------------------------------
 *
 *      File            atx_bluetooth.c
 *      Description     New socket type and address family for Bluetooth
 *                      communications, together with functions and events to
 *                      perform device and service discovery.
 *      Author          Y.H Mun
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

#include "atx_library.h"

// kdBtInquireDevicesATX : Start an inquiry for remote Bluetooth devices.
KD_API KDint KD_APIENTRY kdBtInquireDevicesATX ( KDBtLocalDeviceATX* localdevice, KDint access, KDvoid* eventuserptr )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdBtCancelInquireDevicesATX : Selectively cancel ongoing kdBtInquireDevicesATX operations.
KD_API KDvoid KD_APIENTRY kdBtCancelInquireDevicesATX ( KDvoid *eventuserptr )
{
	kdSetError ( KD_ENOSYS );
}

// kdBtGetFriendlyNameATX : Get the user-friendly name of a Bluetooth device.
KD_API KDint KD_APIENTRY kdBtGetFriendlyNameATX ( KDBtLocalDeviceATX* localdevice, const KDBdAddrATX* bdaddr, KDvoid* eventuserptr )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdBtCancelGetFriendlyNameATX : Selectively cancel ongoing kdBtGetFriendlyNameATX operations.
KD_API KDvoid KD_APIENTRY kdBtCancelGetFriendlyNameATX ( KDvoid* eventuserptr )
{
	kdSetError ( KD_ENOSYS );
}

// kdBtSearchServicesATX : Search services on a remote Bluetooth device.
KD_API KDint KD_APIENTRY kdBtSearchServicesATX ( KDBtLocalDeviceATX* localdevice, const KDBdAddrATX* bdaddr, const KDint32* attrset, const KDUuidATX* uuidset, KDvoid* eventuserptr )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdBtCancelSearchServicesATX : Cancel an outstanding Bluetooth service search.
KD_API KDvoid KD_APIENTRY kdBtCancelSearchServicesATX ( KDvoid* eventuserptr )
{
	kdSetError ( KD_ENOSYS );
}

// kdBtServiceRecordGetRfcommChannelATX : Get the RFCOMM channel from a Bluetooth service record.
KD_API KDint KD_APIENTRY kdBtServiceRecordGetRfcommChannelATX ( KDBtServiceRecordATX* servicerecord )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdBtServiceRecordFreeATX : Free a Bluetooth service record.
KD_API KDvoid KD_APIENTRY kdBtServiceRecordFreeATX ( KDBtServiceRecordATX* servicerecord )
{
	kdSetError ( KD_ENOSYS );
}

// kdBtServiceRecordCreateRfcommATX : Create a Bluetooth service record for an RFCOMM service.
KD_API KDBtServiceRecordATX* KD_APIENTRY kdBtServiceRecordCreateRfcommATX ( const KDUuidATX* uuid, KDint channel )
{
	kdSetError ( KD_ENOSYS );

	return 0;
}

// kdBtRegisterServiceATX : Register a Bluetooth service.
KD_API KDint KD_APIENTRY kdBtRegisterServiceATX ( KDBtLocalDeviceATX* localdevice, KDBtServiceRecordATX* servicerecord )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdBtSocketSetFlagsATX : Set Bluetooth socket flags for authentication, encryption and/or authorization.
KD_API KDint KD_APIENTRY kdBtSocketSetFlagsATX ( KDSocket* socket, KDuint flags )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdBtSetDiscoverableATX : Set the discoverable mode of the device. 
KD_API KDint KD_APIENTRY kdBtSetDiscoverableATX ( KDBtLocalDeviceATX* localdevice, KDuint mode )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}