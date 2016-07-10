/*
 *
 *      File            DlgHelp.cpp
 *      Description     Help dialog.
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
#include "DlgBase.h"
#include "DlgHelp.h"

//
//  "CDlgHelp" dialog layer
//
KDuint CDlgHelp::getCount ( KDvoid )
{
	return 11;
}

const KDchar* CDlgHelp::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Help1, m_uSelected );
}

CCNode* CDlgHelp::getContent ( KDvoid )
{
	KDuint     uCount = this->getCount ( );
	KDuint	   uIndex = m_uSelected;
	CCNode*  pContent = KD_NULL;

	const CCSize&  tLyrSize = this->getContentSize ( );

	if ( m_uSelected == ( uCount - 1 ) )
	{
		pContent = CCSprite::create
		(
			g_pResManager->getPath ( eIMG_ETC_Game_Info )
		);
		{
			CC_ASSERT ( pContent );

			pContent->setPosition ( ccp ( tLyrSize.cx / 2.f - 10.f, tLyrSize.cy / 2.f + 10.f ) );
		}
	}
	else
	{
		pContent = CCLabelTTF::create 
		(
			g_pResManager->getTextHelp ( uIndex ),
			g_pResManager->getPath ( eFNT_Gothic_Bold ), 16,
			CCSize ( 400.f, 295.f ),
			uIndex == ( uCount - 2 ) ? kCCTextAlignmentCenter : kCCTextAlignmentLeft
		);
		{
			CC_ASSERT ( pContent );
	        
			pContent->ignoreAnchorPointForPosition ( KD_TRUE );
			pContent->setPosition ( ccp ( 15.f, 85.f ) );
		}   
	}

	return pContent;
}
