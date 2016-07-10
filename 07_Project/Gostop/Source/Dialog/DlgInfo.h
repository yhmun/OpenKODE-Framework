/*
 *
 *      File            DlgInfo.h
 *      Description     My information dialog.
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

#ifndef __DlgInfo_h__
#define __DlgInfo_h__

//
//  "CDlgInfo" dialog layer
//
class CDlgInfo : public CDlgMulti
{
	public :

		CREATE_FUNC ( CDlgInfo );

	protected :

		virtual KDuint  getCount ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

		virtual CCNode*  getContent ( KDvoid );

		virtual KDvoid   addContent ( KDvoid );
};

//
//  "CLyrInfo" sub layer of "CDlgInfo" layer.
//
class CLyrInfo : public CCLayer
{
	public :

		CREATE_FUNC ( CLyrInfo );

	protected :

		virtual KDvoid  onEnter ( KDvoid );
};

//
//  "CLyrTitle" sub layer of "CDlgInfo" layer.
//
class CLyrTitle : public CCLayer
{
	public :

		CREATE_FUNC ( CLyrTitle );

	protected :

		virtual KDvoid  onEnter ( KDvoid );
		virtual KDvoid  onExit  ( KDvoid );

		KDvoid  onSelect ( CCObject* pSender );

	protected :

		CCNode*			m_pSelect;
		CCLabelTTF*		m_pDesc;
};

//
//  "CLyrSkill" sub layer of "CDlgInfo" layer.
//
class CLyrSkill : public CCLayer
{
	public :

		CREATE_FUNC ( CLyrSkill );

	protected :

		virtual KDvoid  onEnter ( KDvoid );

		KDvoid  onSelect ( CCObject* pSender );

	protected :

		CCSprite*		m_pSelect;
		CCLabelTTF*		m_pDesc;
};

#endif
