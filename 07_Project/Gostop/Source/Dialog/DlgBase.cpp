/*
 *
 *      File            DlgBase.cpp
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

#include "Precompiled.h"
#include "DlgBase.h"
#include "Game/ScrGame.h"

#define INTERVAL_EFFECT     0.3f
#define INTERVAL_TALK		1.0f

//
// Dialog class
//
KDbool CDialog::init ( KDvoid )
{
    m_pBG = KD_NULL;
	m_pTitle = KD_NULL;
    
	if ( !CCLayer::init ( ) )
	{
		return KD_TRUE;
	}

	if ( this->getPathBG ( ) )
	{
		CCSprite*  pBG = CCSprite::create ( this->getPathBG ( ) );
		{
			CC_ASSERT ( pBG );

			this->addChild ( pBG, 0, (KDuint) pBG );
			this->setContentSize ( pBG->getContentSize ( ) );
			this->ignoreAnchorPointForPosition ( KD_FALSE );
			this->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );

			pBG->ignoreAnchorPointForPosition ( KD_TRUE );

			m_pBG = (CCSprite *) this->getChildByTag ( (KDuint) pBG );
		}
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	if ( this->getTitle ( ) )
	{
		CCLabelTTF*  pTitle = CCLabelTTF::create 
		(
			this->getTitle ( ),
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			18
		);
		{
			CC_ASSERT ( pTitle );

			this->addChild ( pTitle, 0, (KDuint) pTitle );

			const CCSize&  tSize = pTitle->getContentSize ( );

			pTitle->ignoreAnchorPointForPosition ( KD_TRUE );
			pTitle->setPosition ( ccpAdd ( ccp ( 10.f, tLyrSize.cy - tSize.cy - 8.f ), getPosTitle ( ) ) );

			m_pTitle = (CCLabelTTF *) this->getChildByTag ( (KDuint) pTitle );
		}
	}

	if ( hasHide ( ) )
	{
		CCMenuItemImage*  pHide = CCMenuItemImage::create 
		(
			g_pResManager->getPath ( eIMG_BTN_Close_Nor ), 
			g_pResManager->getPath ( eIMG_BTN_Close_Sel ),
			this,
			menu_selector ( CDialog::onHide ) 
		);
		{
			CC_ASSERT ( pHide );

			const CCSize&  tSize = pHide->getContentSize ( );

			pHide->setPosition ( ccp ( tLyrSize.cx - tSize.cx - 5.f, tLyrSize.cy - tSize.cy / 2.f - 8.f ) );

			CCMenu*  pMenu = CCMenu::createWithItem ( pHide );
			
			this->addChild ( pMenu ); 
				   
			pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		} 
	}

	return KD_TRUE;
}

KDvoid CDialog::onEnter ( KDvoid )
{
	CCLayer::onEnter ( );

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	const CCSize&  tLyrSize = this->getContentSize ( );

	CCActionInterval*  pAction = KD_NULL;

	if ( this->getShowAction ( ) != eActionNull && this->getDuration ( ) > 0 )
	{
		switch ( this->getShowAction ( ) )
		{
			case eActionMoveInT :

				this->setPosition ( ccp ( tWinSize.cx / 2.f, tWinSize.cy + tLyrSize.cy / 2.f ) );
				pAction = (CCActionInterval *) CCMoveTo::create ( this->getDuration ( ), ccpMid ( tWinSize ) );

				break;

			case eActionAutoHide :

				pAction = (CCActionInterval *) CCSequence::create
				(
					CCDelayTime::create ( this->getDuration ( ) ),
					CCCallFunc ::create ( this, callfunc_selector ( CDialog::onRemove ) ),
					KD_NULL
				);

				break;

			default :
				;
		}
	}

	if ( pAction )
	{
		this->runAction ( pAction );
	}
}

KDvoid CDialog::hide ( KDvoid )
{
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	const CCSize&  tSize = this->getContentSize ( );

	CCActionInterval*  pAction = KD_NULL;

	if ( this->getHideAction ( ) != eActionNull && this->getDuration ( ) > 0 )
	{
		switch ( this->getHideAction ( ) )
		{
			case eActionMoveOutT :

				pAction = CCMoveTo::create ( this->getDuration ( ), ccp ( tWinSize.cx / 2.f, tWinSize.cy + tSize.cy ) );

				break;

			default :
				;
		}
	}

	if ( pAction )
	{
		this->runAction
		(
			(CCActionInterval *) CCSequence::create 
			(
				pAction,
				CCCallFunc::create ( this, callfunc_selector ( CDialog::onRemove ) ),
				KD_NULL
			) 
		);
	}
	else
	{
		this->onRemove ( );
	}	
}

KDvoid CDialog::onHide ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	this->hide ( );
}

KDvoid CDialog::onRemove ( KDvoid )
{
	g_pScrManager->popDialog ( );
}

KDfloat CDialog::getDuration ( KDvoid )
{
	return 0.1f;
}

EAction CDialog::getShowAction ( KDvoid )
{
	return eActionNull;
}

EAction CDialog::getHideAction ( KDvoid )
{
	return eActionNull;
}

const KDchar* CDialog::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( this->getTitle ( ) ? eIMG_BG_Request1 : eIMG_BG_Request2 );
}

const KDchar* CDialog::getTitle ( KDvoid )
{
	return KD_NULL;
}

CCPoint CDialog::getPosTitle ( KDvoid )
{
    return ccp ( 0, 0 );
}

KDbool CDialog::hasHide ( KDvoid )
{
	return KD_TRUE;
}

KDbool CDialog::hasKeyHide ( KDvoid )
{
	return KD_TRUE;
}

//
// Keypad handling layer
//
KDbool CKeypad::init ( KDvoid )
{
	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_TRUE;
	}

	this->setKeypadEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CKeypad::keyBackClicked ( KDvoid )
{
	CDialog*  pDialog = (CDialog *) g_pScrManager->getTopDialog ( );
	KDbool    bClick  = KD_FALSE;

	if ( pDialog )
	{
		if ( pDialog->hasKeyHide ( ) )
		{
			bClick = KD_TRUE;
			pDialog->hide ( );
		}
	}
	else if ( g_pScrManager->getScene ( )->getTag ( ) == eScrMain )
	{
		bClick = KD_TRUE;
		g_pScrManager->setDialog ( eDlgQuit );
	}

	if ( bClick )
	{
		g_pResManager->playSound ( eSND_Etc_Click );
	}
}

//
// Dialog transparent layer
//
CTransparent* CTransparent::create ( KDvoid )
{
	CTransparent*  pRet = new CTransparent ( );

	if ( pRet && pRet->initWithColor ( TRANSPARENT_COLOR ) )
	{
		pRet->autorelease ( );
		pRet->setTouchEnabled ( KD_TRUE );
	}

	return pRet;
}

KDvoid CTransparent::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, kCCMenuHandlerPriority - 1, KD_TRUE );
}

KDbool CTransparent::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	return KD_TRUE;
}

//
// Effect sprite
//
CEffect* CEffect::create ( EPath eID, CCNode* pParent )
{
	CEffect*  pRet = new CEffect ( );

	if ( pRet && pRet->init ( eID, pParent ) )
	{
		pRet->autorelease ( );
	}

	return pRet;
}

KDbool CEffect::init ( EPath eID, CCNode* pParent )
{
	const CCSize&  tParentSize = pParent->getContentSize ( );
	CCPoint        tPosition   = ccpMid ( tParentSize );

	EPath   eSound    = ePath_Null;
	KDuint  uCount    = 6;
	KDfloat  fDuration = INTERVAL_EFFECT;
	KDfloat  fDelay    = INTERVAL_EFFECT * 1.5f;

	if ( this->initWithFile ( g_pResManager->getPath ( eID ) ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	switch ( eID )
	{
		case eIMG_Effect_Gage		:	eSound = eSND_Effect_Gage;		break;
		case eIMG_Effect_Gage2		:									break;
		case eIMG_Effect_Buck		:	
		case eIMG_Effect_Buck2		:	eSound = eSND_Effect_Buck;		break;
		case eIMG_Effect_Heading	:	eSound = eSND_Effect_Heading;	fDelay /= 2; break;
		case eIMG_Effect_Zack		:	eSound = eSND_Effect_Attatch1;  fDelay /= 2; break;
		case eIMG_Effect_Zock		:	
		case eIMG_Effect_Zock2		:	eSound = eSND_Effect_Zock;		break;
		case eIMG_Effect_DaDack		:	eSound = eSND_Effect_Attatch2;  fDelay /= 2; break;
		case eIMG_Effect_SackSul	:	eSound = eSND_Effect_SackSul;	break;
		case eIMG_Effect_Bomb		:	eSound = eSND_Effect_Bomb;		break;
		case eIMG_Effect_Bell		:	eSound = eSND_Effect_Warning;	break;
		case eIMG_Effect_Stop		:									break;
		case eIMG_Effect_1Go		:	
		case eIMG_Effect_2Go		:	eSound = eSND_Effect_1Go;		break;
		case eIMG_Effect_3Go		:	
		case eIMG_Effect_4Go		:	
		case eIMG_Effect_5Go		:	eSound = eSND_Effect_3Go;		break;
		case eIMG_Effect_6Go		:	
		case eIMG_Effect_7Go		:	
		case eIMG_Effect_8Go		:	eSound = eSND_Effect_6Go;		break;
		case eIMG_Effect_3Gwang		:	
		case eIMG_Effect_4Gwang		:	eSound = eSND_Effect_3Gwang;	break;
		case eIMG_Effect_5Gwang		:	eSound = eSND_Effect_5Gwang;	break;
		case eIMG_Effect_BGwang		:	eSound = eSND_Effect_BGwang;	break;
		case eIMG_Effect_ChoDan		:	
		case eIMG_Effect_HongDan	:	
		case eIMG_Effect_ChungDan	:	
		case eIMG_Effect_Godori		:	eSound = eSND_Effect_Dan;		break;
		case eIMG_Effect_MissionS	:	break;
		case eIMG_Effect_MissionF	:	break;

		default : ;
	}

	if ( eSound != ePath_Null )
	{
		g_pResManager->playSound ( eSound );
	}

	pParent->addChild ( this );

	if ( CCDirector::sharedDirector ( )->getWinSize ( ).equals ( tParentSize ) )
	{
		tPosition.y += 50.f;
	}
	this->setPosition ( tPosition );

	CCAnimation*  pAnimation = CCAnimation::create ( );
	for ( KDuint  i = 0; i < uCount; i++ )
	{
		pAnimation->addSpriteFrameWithFileName ( g_pResManager->getPathSprite ( eID, i ) );
	}
	pAnimation->setDelayPerUnit ( fDuration / uCount );
    pAnimation->setRestoreOriginalFrame ( KD_TRUE );

	this->runAction 
	(
		(CCActionInterval *) CCSequence::create
		(
			CCAnimate  ::create ( pAnimation ),
			CCDelayTime::create ( fDelay ),
			CCCallFunc ::create ( this, callfunc_selector ( CEffect::onRemove ) ),
			KD_NULL
		)
	);

	return KD_TRUE;
}

KDvoid CEffect::onRemove ( KDvoid )
{
	this->removeFromParentAndCleanup ( KD_TRUE );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	pScrGame->waitEvent ( );
}

//
// Effect sprite
//
CTalk* CTalk::create ( ETalk eID )
{
	CTalk*  pRet = new CTalk ( );
    
	if ( pRet && pRet->init ( eID ) )
	{
		pRet->autorelease ( );
	}
    
	return pRet;
}

KDbool CTalk::init ( ETalk eID )
{
	CScrGame*	pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	KDuint  uTurn  = pScrGame->getTurn ( );
    KDuint  uIndex = kdRand ( ) % 2 ? g_pEnvManager->getStage ( ) + 1 : 0;
	KDuint  uState = uIndex == 0 ? ( uTurn == 0 ? 0 : 1 ) : ( uTurn == 1 ? 0 : 1 );

    switch ( eID )
    {
        case eTALK_Stop :               
        case eTALK_Heading : 
			uIndex = uTurn == 0 ? 0 : g_pEnvManager->getStage ( ) + 1;
            uState = 0;  
            break;
            
        case eTALK_Gage1 :
        case eTALK_Gage2 :
        case eTALK_Gage3 :
            uIndex = 0;
            uState = 0;
            break;
            
        case eTALK_Slow  : 
        case eTALK_Limit :
            uIndex = 0;
            uState = 0;  
            this->addChild ( CTransparent::create ( ) );
            break;
            
        default : ;
    }
    
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	
	CCSprite*  pLabel = CCSprite::create
	(
        g_pResManager->getPath ( uIndex == 0 ? eIMG_ETC_Talk1 : eIMG_ETC_Talk2 )
    );
	{
		CC_ASSERT ( pLabel );
        
		this->addChild ( pLabel );
        
		const CCSize&  tSize = pLabel->getContentSize ( );
        
		pLabel->ignoreAnchorPointForPosition ( KD_TRUE );
		pLabel->setPosition 
        (
            uIndex == 0 ?
                ccp ( tSize.cx * 0.5f, 125.f ) :
                ccp ( tWinSize.cx - tSize.cx * 1.5f, tWinSize.cy - tSize.cy - 50.f )
        );
        
		CCLabelTTF*  pTalk = CCLabelTTF::create 
		(
            g_pResManager->getTextTalk ( eID, uIndex, uState ),
            g_pResManager->getPath ( eFNT_Gothic_Bold ),
            16
        );
		{
			CC_ASSERT ( pTalk );
            
			pLabel->addChild ( pTalk );
            
			pTalk->setColor ( ccBLACK );
			pTalk->setPosition 
            (
                ccp ( tSize.cx / 2.f + ( uIndex == 0 ? 10.f : -5.f ), tSize.cy / 2.f + 6.f )
            );
		}
	}
    
	CCSprite*  pTalker = CCSprite::create
	(
        g_pResManager->getPathTalk ( eID, uIndex, uState )
    );
	{
		CC_ASSERT ( pTalker );
        
		this->addChild ( pTalker );
        
		const CCSize&  tSize = pTalker->getContentSize ( );
        
		pTalker->ignoreAnchorPointForPosition ( KD_TRUE );
		pTalker->setPosition 
        (
            uIndex == 0 ?
                ccp ( -tSize.cx / 4.f, 0 ) :
                ccp ( tWinSize.cx - tSize.cx * 0.75f, tWinSize.cy - tSize.cy + 37.f )
        );
	}
    
    this->runAction
    (
        (CCActionInterval *) CCSequence::create
        (
            CCDelayTime::create ( INTERVAL_TALK ),
            CCCallFunc::create ( this, callfunc_selector ( CTalk::onRemove ) ),
            KD_NULL
        )
    );
	
    CCDirector::sharedDirector ( )->getRunningScene ( )->addChild ( this, 1 );
    
    return KD_TRUE;
}

KDvoid CTalk::onRemove ( KDvoid )
{
    this->removeFromParentAndCleanup ( KD_TRUE );

	if ( g_pScrManager->getSceneID ( ) == eScrGame )
	{
		CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

		pScrGame->waitEvent ( );
	}
}

//
// Text layer
//
CText* CText::create ( const KDchar* szText, EPath ePath, KDbool bRight )
{
	CText*  pRet = new CText ( );

	if ( pRet && pRet->init ( szText, ePath, bRight ) )
	{
		pRet->autorelease ( );
	}

	return pRet;
}

KDbool CText::init ( const KDchar* szText, EPath ePath, KDbool bRight )
{
	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	m_ePath  = ePath;
	m_bRight = bRight;
	m_sText.clear ( );

	setText ( szText );

	return KD_TRUE;
}

KDvoid CText::setText ( const KDchar* szText )
{
	if ( kdStrcmp ( m_sText.c_str ( ), szText ) )
	{
		m_sText = szText;

		this->removeAllChildrenWithCleanup ( KD_TRUE );

		KDfloat  fPos = 0;
		KDsize  uSize = kdStrlen ( szText );
		for ( KDuint  i = 0; i < uSize; i++ )
		{
			KDuint  uIndex = szText [ uSize - i - 1 ];

			if      ( uIndex == '+' || uIndex == '-' || uIndex == 'x' )
			{
				uIndex = '0' + 11;
			}
			else if ( uIndex == '.' || uIndex == ',' )
			{
				uIndex = '0' + 10;
			}

			CCSprite*  pChar = CCSprite::create 
			(
				g_pResManager->getPathSprite ( m_ePath, uIndex - '0' )
			);
			{
				CC_ASSERT ( pChar );

				this->addChild ( pChar );

				const CCSize&  tSize = pChar->getContentSize ( );

				if ( m_bRight )
				{
					fPos -= tSize.cx;
				}

				pChar->ignoreAnchorPointForPosition ( KD_TRUE );
				pChar->setPosition ( ccp ( fPos, 0 ) );

				if ( !m_bRight )
				{
					fPos += tSize.cx;
				}
			}
		}
	}
}

//
//  "CDlgConfirm" layer.
//
KDbool CDlgConfirm::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCMenuItemImage*  pConfirm = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Confirm_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Confirm_Sel ),
		this,
		menu_selector ( CDlgConfirm::onConfirm ) 
	);
	CC_ASSERT ( pConfirm );

	CCMenu*  pMenu = CCMenu::createWithItem ( pConfirm );
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pConfirm->getContentSize ( );

		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f - 6.f, tSize.cy - 3.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
	}

	return KD_TRUE;
}

const KDchar* CDlgConfirm::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Notice );
}

CCPoint CDlgConfirm::getPosTitle ( KDvoid )
{
    return ccp ( 5.f, -3.f );
}

KDvoid CDlgConfirm::onConfirm ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	this->hide ( );
}

//
//  "CDlgRequest" layer.
//
KDbool CDlgRequest::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCMenuItemImage*  pYes = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Yes_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Yes_Sel ),
		this,
		menu_selector ( CDlgRequest::onYes ) 
	);
	CC_ASSERT ( pYes );

	CCMenuItemImage*  pNo = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_No_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_No_Sel ),
		this,
		menu_selector ( CDlgRequest::onNo ) 
	);
	CC_ASSERT ( pNo );

	CCMenu*  pMenu = CCMenu::create ( pYes, pNo, KD_NULL );
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pYes->getContentSize ( );

		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f - 6.f, tSize.cy - 3.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsHorizontallyWithPadding ( 10.f );
	}
    
	return KD_TRUE;
}

CCPoint CDlgRequest::getPosTitle ( KDvoid )
{
    return ccp ( 5.f, -3.f );
}

KDvoid CDlgRequest::onYes ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	this->hide ( );
}

KDvoid CDlgRequest::onNo ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	this->hide ( );
}

//
//  "CDlgMulti" layer.
//
KDbool CDlgMulti::init ( KDvoid )
{
	m_uSelected = 0;
	m_pContent  = KD_NULL;

	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	KDuint  uCount = this->getCount ( );

	for ( KDuint  i = 0; i < uCount; i++ )
	{
		CCSprite*  pMark = CCSprite::create ( g_pResManager->getPath ( eIMG_ETC_Page_Mark_Nor ) );
		{
			CC_ASSERT ( pMark );

			this->addChild ( pMark, 1, (KDuint) pMark );

			const CCSize&  tSize = pMark->getContentSize ( );

			pMark->ignoreAnchorPointForPosition ( KD_TRUE );
			pMark->setPosition ( ccp ( ( tLyrSize.cx - tSize.cx * 2.5f * uCount ) / 2.f + tSize.cx * 2.5f * i - 5.f, 40.f ) );

			m_vMark.push_back ( (CCSprite*) this->getChildByTag ( (KDuint) pMark ) );
		}
	}

	CCMenuItemImage*  pPrev = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Prev_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Prev_Sel ),
		this,
		menu_selector ( CDlgMulti::onPrev ) 
	);
	CC_ASSERT ( pPrev );

	CCMenuItemImage*  pNext = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Next_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Next_Sel ),
		this,
		menu_selector ( CDlgMulti::onNext ) 
	);
	CC_ASSERT ( pNext );

	CCMenu*  pMenu = CCMenu::create ( pPrev, pNext, KD_NULL );
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pPrev->getContentSize ( );

		pMenu->setPosition ( ccp ( ( tLyrSize.cx - 20.f ) / 2.f, tSize.cy / 2.f + 28.f ) );

		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsHorizontallyWithPadding ( tLyrSize.cx - tSize.cx * 2 - 28.f );
	}

	addContent ( );

	return KD_TRUE;
}

const KDchar* CDlgMulti::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( eIMG_BG_Dlg_Multi1 );
}

KDuint CDlgMulti::getCount ( KDvoid )
{
	return 1;
}

CCNode* CDlgMulti::getContent ( KDvoid )
{
	return KD_NULL;
}

KDvoid CDlgMulti::onPrev ( CCObject* pSender )
{
	KDuint  uCount = this->getCount ( );	
	KDuint  uSelected = m_uSelected;

	m_uSelected = ( m_uSelected == 0 ? uCount : m_uSelected ) - 1;

	m_vMark [ uSelected ]->initWithFile ( g_pResManager->getPath ( eIMG_ETC_Page_Mark_Nor ) );

	addContent ( );

	g_pResManager->playSound ( eSND_Etc_Click );
}

KDvoid CDlgMulti::onNext ( CCObject* pSender )
{
	KDuint  uCount = this->getCount ( );	
	KDuint  uSelected = m_uSelected;

	m_uSelected = ( m_uSelected == ( uCount - 1 ) ) ? 0 : m_uSelected + 1;

	m_vMark [ uSelected ]->initWithFile ( g_pResManager->getPath ( eIMG_ETC_Page_Mark_Nor ) );

	addContent ( );

	g_pResManager->playSound ( eSND_Etc_Click );
}

KDvoid CDlgMulti::addContent ( KDvoid )
{
	CCNode*  pContent = this->getContent ( );

	if ( m_pContent )
	{
		this->removeChild ( m_pContent, KD_TRUE );
		m_pContent = KD_NULL;
	}

	if ( pContent )
	{
		this->addChild ( pContent, 0, (KDuint) pContent );

		m_pContent = this->getChildByTag ( (KDuint) pContent );
	}

	if ( m_pBG && this->getPathBG ( ) )
	{
		m_pBG->initWithFile ( this->getPathBG ( ) );
	}

    if ( m_pTitle && this->getTitle ( ) )
	{
		m_pTitle->setString ( this->getTitle ( ) );
	}

	m_vMark [ m_uSelected ]->initWithFile ( g_pResManager->getPath ( eIMG_ETC_Page_Mark_Sel ) );
}

//
//  "CDlgGame" layer.
//
KDbool CDlgGame::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	g_pResManager->playSound ( eSND_Etc_Notice1 );

	return KD_TRUE;
}

const KDchar* CDlgGame::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( eIMG_BG_Dlg_Game );
}

KDfloat CDlgGame::getDuration ( KDvoid )
{
	return 2.0f;
}

EAction CDlgGame::getShowAction ( KDvoid )
{
	return eActionAutoHide;
}

EAction CDlgGame::getHideAction ( KDvoid )
{
	return eActionNull;
}

KDbool CDlgGame::hasHide ( KDvoid )
{
    return KD_FALSE;
}

KDbool CDlgGame::hasKeyHide ( KDvoid )
{
	return KD_FALSE;
}

//
//  "CDlgGame2" layer.
//
KDbool CDlgGame2::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	g_pResManager->playSound ( eSND_Etc_Notice1 );

	return KD_TRUE;
}

const KDchar* CDlgGame2::getPathBG ( KDvoid )
{
	return KD_NULL;
}

KDfloat CDlgGame2::getDuration ( KDvoid )
{
	return 0;
}

EAction CDlgGame2::getShowAction ( KDvoid )
{
	return eActionNull;
}

EAction CDlgGame2::getHideAction ( KDvoid )
{
	return eActionNull;
}

KDbool CDlgGame2::hasHide ( KDvoid )
{
    return KD_FALSE;
}