/*
 *
 *      File            NetManager_Linux.cpp
 *      Description     Network Manager for Linux
 *      Version         0.90.1227
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
#include "NetManager.h" 

CNetManager::CNetManager ( KDvoid )
{    
	CC_ASSERT ( !g_pNetManager );
}

CNetManager::~CNetManager ( KDvoid )
{

}

ENetError CNetManager::activate ( KDvoid )
{
	ENetError  eErr;

	//
	//	TODO : Program Activate Code...
	//

	eErr = eNet_True;

	return eErr;
}

ENetError CNetManager::agreeSMS ( KDvoid )
{
	ENetError  eErr;

	//
	//	TODO : SMS Agree Code
	//

	eErr = eNet_True;

	return eErr;
}

ENetError CNetManager::isUsedNickName ( const KDchar* szName )
{
	ENetError  eErr;

	//
	//	TODO : NickName Compare Code
	//

	eErr = eNet_False;

	return eErr;
}

ENetError CNetManager::registerRank ( const KDchar* szName, KDuint64 uCash, KDuint64 uCashMax, KDuint64 uScoreMax )
{
	ENetError  eErr;

	//
	//	TODO : Register Score
	//

	eErr = eNet_True;

	return eErr;
}

ENetError CNetManager::puchase ( KDuint64 uPrice )
{
	ENetError  eErr;

	//
	//	TODO : Program Purchase Code...
	//

	eErr = eNet_True;

	return eErr;
}

ENetError CNetManager::getRankList ( SRankList* pList )
{
	ENetError  eErr;

	//
	//	TODO : Rank List Refresh Code
	//

	eErr = eNet_Fail;

	return eErr;
}