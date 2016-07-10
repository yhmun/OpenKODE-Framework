/*
 *
 *      File            Card.h
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

#ifndef __Card_h__
#define __Card_h__

enum ECardType
{
	eCard_Type_Gwang	,
	eCard_Type_10Pi		,
	eCard_Type_5Pi		,
	eCard_Type_3Pi		,
	eCard_Type_2Pi		,
	eCard_Type_1Pi		,
};

enum ECardWhere
{
	eCard_Where_Mission	    ,
	eCard_Where_Shuffled	,
	eCard_Where_Ground		,
	eCard_Where_Player1		,
	eCard_Where_Player2		,
	eCard_Where_Obtain1		,
	eCard_Where_Obtain2		,
};

enum ECardSize
{
	eCard_Size_Normal		,
	eCard_Size_Medium		,
	eCard_Size_Small		,
	eCard_Size_Mini			,
};

enum ECardFlag
{
	eCard_Flag_Null			= 0x0000,
	eCard_Flag_Deal			= 0x0001,
	eCard_Flag_ChongTong    = 0x0010,
	eCard_Flag_Bomb			= 0x0020,
	eCard_Flag_Combo		= 0x0040,
	eCard_Flag_JaBuck		= 0x0080,
	eCard_Flag_GukJin       = 0x0100,
	eCard_Flag_Gage 		= 0x1000,
};

class CCard : public CCSprite
{
	public :

		static CCard*  create ( KDuint uNumuer, ECardWhere eWhere );

	protected :
	
		KDbool		init ( KDuint uNumber, ECardWhere eWhere );

	public :

		KDbool      isBonus    ( KDvoid );
		KDbool      isBomb     ( KDvoid );
		KDbool      isNormal   ( KDvoid );
		
		KDuint		getNumber  ( KDvoid );
		KDuint		getMonth   ( KDvoid );
		ECardType	getType    ( KDvoid );
		ECardSize   getSize    ( KDvoid );
		ECardWhere  getWhere   ( KDvoid );
		KDbool		getFront   ( KDvoid );
		KDbool		getSelect  ( KDvoid );
		KDuint      getFlag    ( KDvoid );

		KDvoid		setWhere   ( ECardWhere eValue );
		KDvoid		setType    ( ECardType  eType  );
		KDvoid		setSize	   ( ECardSize  eValue );	
		KDvoid      setFront   ( KDbool     bValue );
		KDvoid		setMission ( KDbool	    bValue );
		KDvoid		setSelect  ( KDbool		bValue );
		KDvoid		setFlag    ( KDuint     uValue );
		KDvoid      setDark    ( KDbool     bValue );
		KDvoid      setArrow   ( KDbool     bValue );
		
		KDvoid      reorder    ( KDuint     uValue );

		KDfloat      moveTo     ( const CCPoint& tPosition, ECardWhere eWhere, KDuint uOrder, KDfloat fDelay = 0, CCAction* pAction = KD_NULL );

		KDvoid      shine      ( KDvoid );

		KDvoid		onEndShine ( KDvoid );
		KDvoid		onFront    ( KDvoid );
		KDvoid		onResize   ( CCNode* pNode, KDvoid* pValue );
		KDvoid      onRewhere  ( CCNode* pNode, KDvoid* pValue );
		KDvoid      onReorder  ( CCNode* pNode, KDvoid* pValue );

	protected :
	
		KDuint			m_uNumber;
		KDuint			m_uMonth;
		ECardType		m_eType;
		ECardWhere		m_eWhere;
		ECardSize		m_eSize;
		KDbool			m_bFront;
		CCSprite*		m_pCard;
		CCSprite*		m_pMission;
		CCSprite*		m_pSelect;
		CCSprite*		m_pDark;
		CCSprite*		m_pArrow;
		CCSprite*		m_pShine;
		KDuint			m_uFlag;
		KDuint          m_uOrder;
};

class CCardList : public std::deque<CCard*>
{
	public :

		CCard*  getLast ( KDvoid )
		{
			return size ( ) > 0 ? back ( ) : KD_NULL;
		}

		CCard*  addFromParent ( CCard* pCard )
		{
			pCard = (CCard *) pCard->getParent ( )->getChildByTag ( (KDuint) pCard );
			
			push_back ( pCard );
			
			return pCard;
		}

		KDvoid  remove ( CCard* pCard )
		{
			std::deque<CCard*>::iterator  iter;
			for ( iter = begin ( ); iter != end ( ) ; ++iter )
			{
				if ( pCard == *iter )
				{
					erase ( iter );
					break;
				}
			}
		}
};

#endif
