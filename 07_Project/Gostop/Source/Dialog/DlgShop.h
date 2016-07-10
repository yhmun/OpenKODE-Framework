/*
 *
 *      File            DlgShop.h
 *      Description     Shop dialog.
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

#ifndef __DlgShop_h__
#define __DlgShop_h__

//
//  "CDlgShop" dialog layer
//
class CDlgShop : public CDialog
{
	public :

		CREATE_FUNC ( CDlgShop );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );
		virtual const KDchar*  getTitle ( KDvoid );
        virtual CCPoint  getPosTitle ( KDvoid );

		KDvoid  onMenu ( CCObject* pSender );

	public :

		static EDialog		s_eDialog;
		static KDuint		s_uIndex;
};

//
//  "CLyrCoin" sub layer
//
class CLyrCoin : public CCLayer
{
	public :

		CREATE_FUNC ( CLyrCoin );

	protected :

		KDbool  init ( KDvoid );

	public :

		KDvoid	refresh ( KDvoid );

		static  CLyrCoin*	s_pLyrCoin;

	protected :

		CCLabelTTF*			m_pLabel [ 3 ];
};

//
//  "CDlgShopCoin" dialog layer
//
class CDlgShopCoin : public CDialog
{
	public :

		CREATE_FUNC ( CDlgShopCoin );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );
		virtual const KDchar*  getTitle ( KDvoid );
        virtual CCPoint  getPosTitle ( KDvoid );

		KDvoid  onMenu ( CCObject* pSender );
};

//
//  "CDlgShopCash" dialog layer
//
class CDlgShopCash : public CDialog
{
	public :

		CREATE_FUNC ( CDlgShopCash );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );
		virtual const KDchar*  getTitle ( KDvoid );
        virtual CCPoint  getPosTitle ( KDvoid );

		KDvoid  onMenu ( CCObject* pSender );
};

//
//  "CDlgShopSkill" dialog layer
//
class CDlgShopSkill : public CDialog
{
	public :

		CREATE_FUNC ( CDlgShopSkill );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );
		virtual const KDchar*  getTitle ( KDvoid );
        virtual CCPoint  getPosTitle ( KDvoid );

		KDvoid  onSelect ( CCObject* pSender );
		KDvoid  onBuy ( CCObject* pSender );

	public :

		static  KDuint		m_uSelect;

	protected :

		CCSprite*			m_pSelect;
		CCLabelTTF*			m_pDesc;
		CCLabelTTF*			m_pCost;
};

//
//  "CDlgBuyRequest" dialog layer
//		
class CDlgBuyRequest : public CDlgRequest
{
	public :

		CREATE_FUNC ( CDlgBuyRequest );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

		virtual KDvoid  onYes ( CCObject* pSender );
};

//
//  "CDlgBuyDone" dialog layer
//
class CDlgBuyDone : public CDlgConfirm
{
	public :

		CREATE_FUNC ( CDlgBuyDone );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );
};
			
//
//  "CDlgBuyFail" dialog layer
//
class CDlgBuyFail : public CDlgConfirm
{
	public :

		CREATE_FUNC ( CDlgBuyFail );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );
};
				
//
//  "CDlgNotEnough" dialog layer
//
class CDlgNotEnough : public CDlgConfirm
{
	public :

		CREATE_FUNC ( CDlgNotEnough );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );
};	

#endif
