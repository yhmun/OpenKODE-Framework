/*
 *
 *      File            Card.cpp
 *      Description     Card class
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
#include "Card.h"

#define  DURATION_SHINE    0.3f

typedef struct
{
	ECardType	m_eType;
} SCardInfo;

static const ECardType  l_pTypeTables [ 52 ] =
{
	eCard_Type_Gwang , eCard_Type_5Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,	
	eCard_Type_10Pi  , eCard_Type_5Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,	
	eCard_Type_Gwang , eCard_Type_5Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,	
	eCard_Type_10Pi  , eCard_Type_5Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,	
                    
	eCard_Type_10Pi  , eCard_Type_5Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,
	eCard_Type_10Pi  , eCard_Type_5Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,
	eCard_Type_10Pi  , eCard_Type_5Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,
	eCard_Type_Gwang , eCard_Type_10Pi , eCard_Type_1Pi , eCard_Type_1Pi ,
                                       
	eCard_Type_10Pi  , eCard_Type_5Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,
    eCard_Type_10Pi  , eCard_Type_5Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,
	eCard_Type_Gwang , eCard_Type_2Pi  , eCard_Type_1Pi , eCard_Type_1Pi ,
	eCard_Type_Gwang , eCard_Type_10Pi , eCard_Type_5Pi , eCard_Type_2Pi ,
                                      
	eCard_Type_3Pi   , eCard_Type_2Pi  , eCard_Type_2Pi , eCard_Type_1Pi ,                                                                                                             
};

CCard* CCard::create ( KDuint uNumuer, ECardWhere eWhere )
{
	CCard*  pRet = new CCard ( );

	if ( pRet && pRet->init ( uNumuer, eWhere ) )
	{
		pRet->autorelease ( );
	}
	
	return pRet;
}

KDbool CCard::init ( KDuint uNumber, ECardWhere eWhere )
{
	if ( CCSprite::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}
 
	m_uNumber  = uNumber;
	m_uMonth   = uNumber / 4;
	m_eType    = l_pTypeTables [ uNumber ];
	m_bFront   = KD_FALSE;
	m_pMission = KD_NULL;
	m_pSelect  = KD_NULL;
	m_pDark    = KD_NULL;
	m_pArrow   = KD_NULL;
	m_pShine   = KD_NULL;
	m_uFlag    = 0;

	setWhere ( eWhere );

	return KD_TRUE;
}

KDbool CCard::isBonus ( KDvoid )
{
	return m_uNumber > 47 && m_uNumber < 51 ? KD_TRUE : KD_FALSE;
}

KDbool CCard::isBomb ( KDvoid )
{
	return m_uNumber == 51 ? KD_TRUE : KD_FALSE;
}

KDbool CCard::isNormal ( KDvoid )
{
	return m_uMonth < 12 ? KD_TRUE : KD_FALSE;
}

KDuint CCard::getNumber ( KDvoid )
{
	return m_uNumber;
}

KDuint CCard::getMonth ( KDvoid )
{
	return m_uMonth;
}

ECardType CCard::getType ( KDvoid )
{
	return m_eType;
}

ECardSize CCard::getSize ( KDvoid )
{
	return m_eSize;
}

ECardWhere CCard::getWhere ( KDvoid )
{
	return m_eWhere;
}

KDbool CCard::getFront ( KDvoid )
{
	return m_bFront;
}

KDbool CCard::getSelect ( KDvoid )
{
	return m_pSelect ? KD_TRUE : KD_FALSE;
}

KDuint CCard::getFlag ( KDvoid )
{
	return m_uFlag;
}

KDvoid CCard::setWhere ( ECardWhere eValue )
{
	m_eWhere = eValue;

	switch ( eValue )
	{
		case eCard_Where_Mission  : m_bFront = KD_TRUE;  m_eSize = eCard_Size_Small;  break;
		case eCard_Where_Shuffled : m_bFront = KD_FALSE; m_eSize = eCard_Size_Medium; break;
		case eCard_Where_Ground   : m_bFront = KD_TRUE;  m_eSize = eCard_Size_Medium; break;
		case eCard_Where_Player1  : m_bFront = KD_TRUE;  m_eSize = eCard_Size_Normal; break;
		case eCard_Where_Player2  : m_bFront = KD_FALSE; m_eSize = eCard_Size_Mini;   break;
		case eCard_Where_Obtain1  : m_bFront = KD_TRUE;  m_eSize = eCard_Size_Medium; break;
		case eCard_Where_Obtain2  : m_bFront = KD_TRUE;  m_eSize = eCard_Size_Medium; break;
	}

	setSize ( m_eSize );
}

KDvoid CCard::setType ( ECardType eValue )
{
	m_eType = eValue;
}

KDvoid CCard::setSize ( ECardSize eValue )
{
	switch ( eValue )
	{
		case eCard_Size_Normal : 
			
			this->initWithFile ( g_pResManager->getPathSprite ( eIMG_Sprite_Card, m_uNumber == 51 ? 52 : m_uNumber ) );

			if ( m_pDark )
			{
				m_pDark->setPosition ( ccpMid ( this->getContentSize ( ) ) );
			}

			if ( m_pMission )
			{
				m_pMission->setVisible ( KD_TRUE );
				m_pMission->initWithFile ( g_pResManager->getPath ( eIMG_ETC_Mission_Mark ) );
			}

			break;

		case eCard_Size_Medium :

			this->initWithFile ( g_pResManager->getPathSprite ( eIMG_Sprite_Card_Medium, m_bFront ? m_uNumber : 51 ) );

			if ( m_pMission )
			{
				m_pMission->setVisible ( m_bFront );
				m_pMission->initWithFile ( g_pResManager->getPath ( eIMG_ETC_Mission_Mark_Medium ) );
			}
			
			break;

		case eCard_Size_Small  :
			
			this->initWithFile ( g_pResManager->getPathSprite ( eIMG_Sprite_Card_Small, m_uNumber ) );
			
			break;

		case eCard_Size_Mini   : 

			this->initWithFile ( g_pResManager->getPath ( eIMG_Sprite_Card_Mini ) );
			
			if ( m_pMission )
			{
				m_pMission->setVisible ( KD_FALSE );
			}

			break;
	}

	m_eSize = eValue;

	if ( m_pSelect )
	{
		setSelect ( KD_FALSE );
		setSelect ( KD_TRUE  );
	}

	if ( m_pArrow )
	{
		setArrow ( KD_FALSE );
		setArrow ( KD_TRUE  );
	}

	if ( m_pShine )
	{
		this->removeChild ( m_pShine, KD_TRUE );
		m_pShine = KD_NULL;
	}
}

KDvoid CCard::setMission ( KDbool bValue )
{
	if ( bValue )
	{
		if ( !m_pMission )
		{
			CCSprite*  pMission = CCSprite::create ( );
			{
				CC_ASSERT ( pMission );

				this->addChild ( pMission, 1, (KDuint) pMission );
				
				pMission->ignoreAnchorPointForPosition ( KD_TRUE );

				m_pMission = (CCSprite *) this->getChildByTag ( (KDuint) pMission );
			}	
		}

		setSize ( m_eSize );
	}
	else
	{
		if ( m_pMission )
		{
			this->removeChild ( m_pMission, KD_TRUE );
			m_pMission = KD_NULL;
		}
	}
}

KDvoid CCard::setSelect ( KDbool bValue )
{
	if ( bValue )
	{
		if ( !m_pSelect )
		{
			EPath  ePath = m_eSize == eCard_Size_Normal ? eIMG_Sprite_Select : eIMG_Sprite_Select_Medium;

			CCSprite*  pSelect = CCSprite::create
			(
				g_pResManager->getPath ( ePath )
			);
			{
				CC_ASSERT ( pSelect );

				this->addChild ( pSelect, 4, (KDuint) pSelect );
				
				pSelect->setPosition ( ccpMid ( this->getContentSize ( ) ) );

				CCAnimation*  pAnimation = CCAnimation::create ( );
				for ( KDuint  i = 0; i < 2; i++ )
				{
					pAnimation->addSpriteFrameWithFileName ( g_pResManager->getPathSprite ( ePath, i ) );
				}
				pAnimation->setDelayPerUnit ( 0.2f / 2 );
				pAnimation->setRestoreOriginalFrame ( KD_TRUE );

				pSelect->runAction 
				(
					CCRepeatForever::create
					(
						CCAnimate::create ( pAnimation )
					)
				);

				m_pSelect = (CCSprite *) this->getChildByTag ( (KDuint) pSelect );
			}	
		}
	}
	else
	{
		if ( m_pSelect )
		{
			this->removeChild ( m_pSelect, KD_TRUE );
			m_pSelect = KD_NULL;
		}
	}
}

KDvoid CCard::setFront ( KDbool bValue )
{
	if ( m_bFront != bValue )
	{
		m_bFront = bValue;
	}
}

KDvoid CCard::setFlag ( KDuint uValue )
{
	m_uFlag = uValue;
}

KDvoid CCard::setDark ( KDbool bValue )
{
	if ( bValue )
	{
		if ( !m_pDark )
		{
			CCSprite*  pDark = CCSprite::create 
			(
				g_pResManager->getPathSprite ( eIMG_Sprite_Card, 53 )
			);
			{
				CC_ASSERT ( pDark );

				this->addChild ( pDark, 2, (KDuint) pDark );
				pDark->setPosition ( ccpMid ( this->getContentSize ( ) ) );

				m_pDark = (CCSprite *) this->getChildByTag ( (KDuint) pDark );
			}
		}
	}
	else
	{
		if ( m_pDark )
		{
			this->removeChild ( m_pDark, KD_TRUE );
			m_pDark = KD_NULL;
		}
	}
}

KDvoid CCard::setArrow ( KDbool bValue )
{    
	if ( m_pArrow )
	{
		this->removeChild ( m_pArrow, KD_TRUE );
		m_pArrow = KD_NULL;
	}

	if ( bValue )
	{
		CCSprite*  pArrow = CCSprite::create 
		(
			g_pResManager->getPath ( m_pDark ? eIMG_ETC_Player_Arrow2 : eIMG_ETC_Player_Arrow1 )
		);
		{
			CC_ASSERT ( pArrow );

			this->addChild ( pArrow, 5, (KDuint) pArrow );

			const CCSize&  tLyrSize = this->getContentSize ( );
			const CCSize&  tSize = pArrow->getContentSize ( );

			pArrow->setPosition
			( 
				m_eWhere == eCard_Where_Player1 ? 
					ccp ( tLyrSize.cx / 2.f, tLyrSize.cy - tSize.cy / 2.f + 2.f ) : ccpMid ( tLyrSize )
			);

			m_pArrow = (CCSprite *) this->getChildByTag ( (KDuint) pArrow );
		}
	}
}

KDvoid CCard::reorder ( KDuint uValue )
{
	m_uOrder = uValue;
	this->getParent ( )->reorderChild ( this, uValue );
}

KDfloat CCard::moveTo ( const CCPoint& tPosition, ECardWhere eWhere, KDuint uOrder, KDfloat fDelay, CCAction* pAction )
{
	KDfloat  fAnimate = g_pEnvManager->getAnimate ( );
	
	if ( m_eWhere == eCard_Where_Player1 )
	{
		this->setDark  ( KD_FALSE );
		this->setArrow ( KD_FALSE );
	}

	if ( !( m_eWhere == eCard_Where_Ground && eWhere == eCard_Where_Ground ) )
	{
		this->reorder  ( 100 );
	}

	this->setFront ( eWhere != eCard_Where_Player2 );
	this->setSize  ( eCard_Size_Medium );
	
	this->runAction 
	(
		(CCActionInterval *) CCSequence::create
		(
			CCDelayTime::create ( fDelay ),
			CCEaseIn::create 
			(
				(CCActionInterval *) CCSpawn::create	
				(
					CCScaleTo::create ( fAnimate, 1.f ),
					CCMoveTo ::create ( fAnimate, tPosition ),
					KD_NULL
				),
				5.f
			),
			CCCallFuncND::create ( this, callfuncND_selector ( CCard::onRewhere ), (KDvoid *) ( eWhere ) ),
			CCCallFuncND::create ( this, callfuncND_selector ( CCard::onReorder ), (KDvoid *) ( uOrder ) ),
			pAction,
			KD_NULL
		)
	);

	m_eWhere = eWhere;

	return fDelay + fAnimate;
}

KDvoid CCard::shine ( KDvoid )
{
	if ( !m_bFront || m_pShine )
	{
		return;
	}

	EPath  ePath = m_eSize == eCard_Size_Normal ? eIMG_Effect_Shine1 : eIMG_Effect_Shine2;

	CCSprite*  pShine = CCSprite::create 
	(
		g_pResManager->getPath ( ePath )
	);
	{
		CC_ASSERT ( pShine );

		this->addChild ( pShine, 3, (KDuint) pShine );

		pShine->setPosition ( ccpMid ( this->getContentSize ( ) ) );

		CCAnimation*  pAnimation = CCAnimation::create ( );

		for ( KDuint  i = 0; i < 4; i++ )
		{
			pAnimation->addSpriteFrameWithFileName ( g_pResManager->getPathSprite ( ePath, i ) );
		}
		pAnimation->setDelayPerUnit ( DURATION_SHINE / 4 );
		pAnimation->setRestoreOriginalFrame ( KD_TRUE );

		pShine->runAction 
		(
			(CCActionInterval *) CCSequence::create
			(
				CCAnimate::create ( pAnimation ),
				CCCallFunc::create ( this, callfunc_selector ( CCard::onEndShine ) ),
				KD_NULL
			)
		);

		m_pShine = (CCSprite *) this->getChildByTag ( (KDuint) pShine );
	}
}

KDvoid CCard::onEndShine ( KDvoid )
{
	if ( m_pShine )
	{
		this->removeChild ( m_pShine, KD_TRUE );
		m_pShine = KD_NULL;
	}
}

KDvoid CCard::onFront ( KDvoid )
{
	this->setFront ( KD_TRUE ); 
}

KDvoid CCard::onResize ( CCNode* pNode, KDvoid* pValue )
{
	this->setSize ( (ECardSize) ( (KDuint) pValue ) );
}

KDvoid CCard::onRewhere ( CCNode* pNode, KDvoid* pValue )
{
	setWhere ( (ECardWhere) ( (KDuint) pValue ) );
}

KDvoid CCard::onReorder ( CCNode* pNode, KDvoid* pValue )
{
	this->reorder ( (KDuint) pValue );
}