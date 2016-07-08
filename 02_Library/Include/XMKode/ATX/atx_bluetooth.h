/* --------------------------------------------------------------------------
 *
 *      File            atx_bluetooth.h
 *      Description     New socket type and address family for Bluetooth
 *                      communications, together with functions and events to
 *                      perform device and service discovery.
 *      Version         6, 2008-09-12
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

#ifndef __kd_ATX_bluetooth_h_
#define __kd_ATX_bluetooth_h_

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KD_SOCK_RFCOMM_ATX                          169

#define KD_AF_BLUETOOTH_ATX                         170

#define KD_BT_GIAC_ATX                              0x9e8b33

#define KD_BTFLAG_AUTHENTICATE                      1
#define KD_BTFLAG_ENCRYPT                           2
#define KD_BTFLAG_AUTHORIZE                         4

#define KD_BT_NOT_DISCOVERABLE_ATX                  0

// KD_EVENT_BT_DEVICE_DISCOVERED_ATX : kdBtInquireDevicesATX has found a device or has completed event.
#define KD_EVENT_BT_DEVICE_DISCOVERED_ATX           171

// KD_EVENT_BT_NAME_ATX : kdBtGetFriendlyNameATX complete event.
#define KD_EVENT_BT_NAME_ATX                        172

// KD_EVENT_BT_SERVICE_DISCOVERED_ATX : kdBtSearchServicesATX has found a service or has completed event.
#define KD_EVENT_BT_SERVICE_DISCOVERED_ATX          173

typedef struct KDBtServiceRecordATX                 KDBtServiceRecordATX;
typedef struct KDBtLocalDeviceATX                   KDBtLocalDeviceATX;

typedef struct KDUuidATX
{
    KDuint32                              i1, i2, i3, i4;
} KDUuidATX;

typedef struct KDBdAddrATX
{
    KDuint8                               b[6];
} KDBdAddrATX;

typedef struct KDSockaddrAfBtrcATX
{
    KDBdAddrATX                           bdaddr;
    KDuint8                               channel;
} KDSockaddrAfBtrcATX;

typedef struct KDBtRemoteDeviceInfoATX 
{
    KDBdAddrATX                           bdaddr;
    KDuint8                               deviceclass[3];
} KDBtRemoteDeviceInfoATX;

typedef struct KDEventBtDeviceATX 
{
    KDint32                               error;
    const struct KDBtRemoteDeviceInfoATX* result;
} KDEventBtDeviceATX;

typedef struct KDEventBtNameATX
{
    KDint32                               error;
    const KDchar*                         result;
} KDEventBtNameATX;

typedef struct KDEventBtServiceATX 
{
    KDint32                               error;
    struct KDBtServiceRecordATX*          result;
} KDEventBtServiceATX;

// kdBtInquireDevicesATX : Start an inquiry for remote Bluetooth devices.
KD_API KDint KD_APIENTRY kdBtInquireDevicesATX ( KDBtLocalDeviceATX* localdevice, KDint access, KDvoid* eventuserptr );

// kdBtCancelInquireDevicesATX : Selectively cancel ongoing kdBtInquireDevicesATX operations.
KD_API KDvoid KD_APIENTRY kdBtCancelInquireDevicesATX ( KDvoid *eventuserptr );

// kdBtGetFriendlyNameATX : Get the user-friendly name of a Bluetooth device.
KD_API KDint KD_APIENTRY kdBtGetFriendlyNameATX ( KDBtLocalDeviceATX* localdevice, const KDBdAddrATX* bdaddr, KDvoid* eventuserptr );

// kdBtCancelGetFriendlyNameATX : Selectively cancel ongoing kdBtGetFriendlyNameATX operations.
KD_API KDvoid KD_APIENTRY kdBtCancelGetFriendlyNameATX ( KDvoid* eventuserptr );

// kdBtSearchServicesATX : Search services on a remote Bluetooth device.
KD_API KDint KD_APIENTRY kdBtSearchServicesATX ( KDBtLocalDeviceATX* localdevice, const KDBdAddrATX* bdaddr, const KDint32* attrset, const KDUuidATX* uuidset, KDvoid* eventuserptr );

// kdBtCancelSearchServicesATX : Cancel an outstanding Bluetooth service search.
KD_API KDvoid KD_APIENTRY kdBtCancelSearchServicesATX ( KDvoid* eventuserptr );

// kdBtServiceRecordGetRfcommChannelATX : Get the RFCOMM channel from a Bluetooth service record.
KD_API KDint KD_APIENTRY kdBtServiceRecordGetRfcommChannelATX ( KDBtServiceRecordATX* servicerecord );

// kdBtServiceRecordFreeATX : Free a Bluetooth service record.
KD_API KDvoid KD_APIENTRY kdBtServiceRecordFreeATX ( KDBtServiceRecordATX* servicerecord );

// kdBtServiceRecordCreateRfcommATX : Create a Bluetooth service record for an RFCOMM service.
KD_API KDBtServiceRecordATX* KD_APIENTRY kdBtServiceRecordCreateRfcommATX ( const KDUuidATX* uuid, KDint channel );

// kdBtRegisterServiceATX : Register a Bluetooth service.
KD_API KDint KD_APIENTRY kdBtRegisterServiceATX ( KDBtLocalDeviceATX* localdevice, KDBtServiceRecordATX* servicerecord );

// kdBtSocketSetFlagsATX : Set Bluetooth socket flags for authentication, encryption and/or authorization.
KD_API KDint KD_APIENTRY kdBtSocketSetFlagsATX ( KDSocket* socket, KDuint flags );

// kdBtSetDiscoverableATX : Set the discoverable mode of the device. 
KD_API KDint KD_APIENTRY kdBtSetDiscoverableATX ( KDBtLocalDeviceATX* localdevice, KDuint mode );


#ifdef __cplusplus
}
#endif

#endif 


