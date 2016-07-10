/*
 *
 *      File            NetManager.h
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

#ifndef __NetManager_h__
#define __NetManager_h__

#define RANK_LIST_COUNT			20

enum ENetError
{
	eNet_True		,
	eNet_False		,
	eNet_Fail		,
};

typedef struct 
{
	KDchar		szName  [ RANK_LIST_COUNT ][ 64 ];
	KDuint		uNumber [ RANK_LIST_COUNT ];
	KDuint		uMyOrder;
} SRankList;

class CNetManager
{
	protected :

		 CNetManager ( KDvoid );
		~CNetManager ( KDvoid );

	public :

		static KDvoid	create  ( KDvoid );
		static KDvoid	release ( KDvoid );

	public :

		ENetError  activate ( KDvoid );

		ENetError  agreeSMS ( KDvoid );

		ENetError  isUsedNickName ( const KDchar* szName );

		ENetError  registerRank ( const KDchar* szName, KDuint64 uCash, KDuint64 uCashMax, KDuint64 uScoreMax );

		ENetError  puchase ( KDuint64 uPrice );

		ENetError  getRankList ( SRankList* pList );

	protected :

};

extern CNetManager*  g_pNetManager;

#endif
