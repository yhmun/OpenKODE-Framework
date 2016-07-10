/*
 *
 *      File            DlgBase.h
 *      Description     Base dialogs.
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

#ifndef __DlgBase_h__
#define __DlgBase_h__

//
// Dialog class
//
class CDialog : public CCLayer
{
	protected :

		virtual KDbool  init ( KDvoid );

		virtual KDvoid   onEnter ( KDvoid );

		virtual KDfloat   getDuration   ( KDvoid );
		virtual EAction  getShowAction ( KDvoid );
		virtual EAction  getHideAction ( KDvoid );

		virtual const KDchar*  getPathBG   ( KDvoid );
		virtual const KDchar*  getTitle    ( KDvoid );
        virtual CCPoint        getPosTitle ( KDvoid );
		virtual KDbool         hasHide     ( KDvoid );

		virtual KDvoid  onHide ( CCObject* pSender );
		virtual KDvoid  onRemove ( KDvoid );
		
	public :

		virtual KDbool  hasKeyHide ( KDvoid );

		KDvoid  hide ( KDvoid );

	protected :

		CCSprite*		m_pBG;
		CCLabelTTF*		m_pTitle;
};

//
// Keypad handling layer
//
class CKeypad : public CCLayer
{
	public :

		CREATE_FUNC ( CKeypad );

	protected :

		virtual KDbool  init ( KDvoid );

		virtual KDvoid  keyBackClicked ( KDvoid );
};

//
// Dialog transparent layer
//
class CTransparent : public CCLayerColor
{
	public :

		static CTransparent*  create ( KDvoid );

	protected :

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );	

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
};

//
// Effect sprite
//
class CEffect : public CCSprite
{
	public :

		static CEffect*  create ( EPath eID, CCNode* pParent ); 

	protected :

		virtual KDbool  init ( EPath eID, CCNode* pParent );

		virtual KDvoid  onRemove ( KDvoid );

	protected :

		CCNode*		m_pParent;
};

//
// Talk layer
//
class CTalk : public CCLayer
{
    public :
    
        static CTalk*   create ( ETalk eID );
    
    protected :
    
        virtual KDbool  init ( ETalk eID );
    
        virtual KDvoid  onRemove ( KDvoid );
};

//
// Text layer
//
class CText : public CCLayer
{
	public : 

		static CText*   create ( const KDchar* szText, EPath ePath, KDbool bRight = KD_TRUE );

	protected :

		virtual KDbool  init ( const KDchar* szText, EPath ePath, KDbool bRight );

	public :

		KDvoid  setText ( const KDchar* szText );

	protected :

		EPath				m_ePath;
		KDbool				m_bRight;
		std::string			m_sText;
};

//
//  "CDlgConfirm" layer.
//
class CDlgConfirm : public CDialog
{
	protected :

		virtual KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );
        virtual CCPoint  getPosTitle ( KDvoid );
    
		virtual KDvoid  onConfirm ( CCObject* pSender );
};

//
//  "CDlgRequest" layer.
//
class CDlgRequest : public CDialog
{
	protected :

		virtual KDbool  init ( KDvoid );

        virtual CCPoint  getPosTitle ( KDvoid );
    
		virtual KDvoid  onYes ( CCObject* pSender );
		virtual KDvoid  onNo  ( CCObject* pSender );
};

//
//  "CDlgMulti" layer.
//
class CDlgMulti : public CDialog 
{
	protected :

		virtual KDbool  init ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );

		virtual KDuint   getCount ( KDvoid );
		virtual CCNode*  getContent ( KDvoid );

		virtual KDvoid  onPrev ( CCObject* pSender );
		virtual KDvoid  onNext ( CCObject* pSender );

		virtual KDvoid  addContent ( KDvoid );

	protected :

		CCNode*					m_pContent;
		KDuint					m_uSelected;
		std::vector<CCSprite*>  m_vMark;
};

//
//  "CDlgGame" layer.
//
class CDlgGame : public CDialog 
{
	protected :

		virtual KDbool  init ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );

		virtual KDfloat  getDuration ( KDvoid );

		virtual EAction  getShowAction ( KDvoid );
		virtual EAction  getHideAction ( KDvoid );

        virtual KDbool   hasHide ( KDvoid );

	public :

		virtual KDbool  hasKeyHide ( KDvoid );
};

//
//  "CDlgGame2" layer.
//
class CDlgGame2 : public CDialog 
{
	protected :

		virtual KDbool  init ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );

		virtual KDfloat   getDuration   ( KDvoid );
		virtual EAction  getShowAction ( KDvoid );
		virtual EAction  getHideAction ( KDvoid );

        virtual KDbool   hasHide ( KDvoid );
};

#endif
