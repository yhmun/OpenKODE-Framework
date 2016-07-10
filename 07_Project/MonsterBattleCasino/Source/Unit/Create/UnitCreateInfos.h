/* -----------------------------------------------------------------------------------
 *
 *      File            UnitCreateInfos.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __UnitCreateInfos_h__
#define __UnitCreateInfos_h__

// monster action------------------------------------------------------------------
class cUnitScript : public Object
{
	public :

		cUnitScript ( KDvoid )
		: eType ( eScriptType_End	)
		, fSub  ( 0					)
		{

		}

		virtual ~cUnitScript ( KDvoid )
		{

		}

		KDvoid		copy ( const cUnitScript& tOther )
		{
			this->eType = tOther.eType;
			this->fSub  = tOther.fSub;
			this->sStr  = tOther.sStr;
		}

		eScriptType						eType;
		KDfloat							fSub;
		std::string						sStr;
};


// monster action------------------------------------------------------------------
class cUnitAction : public Object
{
	public :

		cUnitAction ( KDvoid )
		{

		}

		virtual ~cUnitAction ( KDvoid )
		{
			for ( auto pScript : aScripts )
			{
				CC_SAFE_RELEASE ( pScript );
			}
			aScripts.clear ( );
		}

		std::vector<cUnitScript*>		aScripts;
};


// monster create info-------------------------------------------------------------
class cUnitCreateInfo : public Object
{
	public :

		cUnitCreateInfo ( KDint nCreateIndex_ )
		: nCreateIndex					( nCreateIndex_ )
		, nChance						( 10			)
		, nSkillIcon1					( -1			)
		, nSkillIcon2					( -1			)
		, nSkillIcon3					( -1			)
		, nSkillIcon4					( -1			)
		, tPointSpecialX				( 0				)
		, tPointSpecialY				( 0				)
		, nCasinoAdvantageORuniqueType	( 0				)
		{
			aHp  [ 0 ] = 0;
			aHp  [ 1 ] = 0;
			aAtk [ 0 ] = 0;
			aAtk [ 1 ] = 0;
			aDef [ 0 ] = 0;
			aDef [ 1 ] = 0;
			aAgl [ 0 ] = 0;
			aAgl [ 1 ] = 0;
			aCrt [ 0 ] = 0;
			aCrt [ 1 ] = 0;

			aLvUpHp  [ 0 ] = 0;
			aLvUpHp  [ 1 ] = 0;
			aLvUpAtk [ 0 ] = 0;
			aLvUpAtk [ 1 ] = 0;
			aLvUpDef [ 0 ] = 0;
			aLvUpDef [ 1 ] = 0;
			aLvUpAgl [ 0 ] = 0;
			aLvUpAgl [ 1 ] = 0;
			aLvUpCrt [ 0 ] = 0;
			aLvUpCrt [ 1 ] = 0;
		}

		virtual ~cUnitCreateInfo ( KDvoid )
		{
			for ( auto pAction : aActions )
			{
				CC_SAFE_RELEASE ( pAction );
			}
			aActions.clear ( );
		}

		KDint							nCreateIndex;

		std::string						sName;
		KDint							nChance;
		std::string						sSpriteFileDir;
		std::string						sSpriteFileName;
		std::string						sStrDetail;

		KDint							nSkillIcon1;
		KDint							nSkillIcon2;
		KDint							nSkillIcon3;
		KDint							nSkillIcon4;
		std::string						sSkillStr1;
		std::string						sSkillStr2;
		std::string						sSkillStr3;
		std::string						sSkillStr4;

		KDint							aHp  [ 2 ];				// [0] == min, [1] == max
		KDint							aAtk [ 2 ];
		KDint							aDef [ 2 ];
		KDint							aAgl [ 2 ];
		KDint							aCrt [ 2 ];

		KDint							aLvUpHp  [ 2 ];			// [0] == min, [1] == max
		KDint							aLvUpAtk [ 2 ];
		KDint							aLvUpDef [ 2 ];
		KDint							aLvUpAgl [ 2 ];
		KDint							aLvUpCrt [ 2 ];

		KDint							tPointSpecialX;
		KDint							tPointSpecialY;

		KDint							nCasinoAdvantageORuniqueType;

		std::vector<cUnitAction*>		aActions;
};

// monster create infos------------------------------------------------------------
class cUnitCreateInfos : public Object
{
	public :
				 cUnitCreateInfos ( std::string sFileName );
		virtual ~cUnitCreateInfos ( KDvoid );

		// get
		const cUnitCreateInfo*			getUnitInfo ( KDint nIndex );
		const cUnitCreateInfo*			getUnitInfo ( std::string sName );
		const cUnitCreateInfo*			getUnitInfoRand ( KDvoid );				// 나올 확률에 맞춰서 랜덤하게 몬스터 생성 정보를 가져온다.

		inline KDint					getUnitInfoCount ( KDvoid )	{ return (KDint) m_aUnits.size ( ); }

	private :

		std::vector<cUnitCreateInfo*>	m_aUnits;
		KDint							m_nAllChance;
};



#endif	// __UnitCreateInfos_h__
