/*
 *
 *      File            NetManager.cpp
 *      Description     Network Manager
 *      Version         0.95.1007
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2011 XMSoft.
 *      Copyright (C) 2011 Blue River Ltd.
 *      Copyright (C) 2011 Blueplay Ltd. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 *                     chris@blueplay.co.kr
 *                     	
 */ 

#include "Precompiled.h"

CNetManager*  g_pNetManager  = KD_NULL;

KDvoid CNetManager::create ( KDvoid )
{
    g_pNetManager = new CNetManager ( );

    CC_ASSERT ( g_pNetManager );
}

KDvoid CNetManager::release ( KDvoid )
{
	CC_SAFE_DELETE ( g_pNetManager );
}
