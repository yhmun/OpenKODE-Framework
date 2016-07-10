/* -----------------------------------------------------------------------------------
 *
 *      File            Define.h
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

#ifndef __Define_h__
#define __Define_h__

#define _SCR_W											((KDint)480)
#define _SCR_H											((KDint)320)

#define _SCR_W_HARP										240.f
#define _SCR_H_HARP										160.f

#define _FILE_NAME_MY_DATA								"/data/Save.sav"
#define _FILE_NAME_CASINO_BATTLE_SAVE					"/data/CasinoBattleData.sav"
#define _FILE_NAME_CASINO_RESULT_SAVE					"/data/CasinoResultData.sav"
#define _FILE_NAME_MYROOM_BATTLE_SAVE					"/data/MyRoomBattleData.sav"
#define _FILE_NAME_MYROOM_RESULT_SAVE					"/data/MyRoomResultData.sav"

#define _OPENFEINT_ACHIEVEMENT_COIN_500_ID				"693282"
#define _OPENFEINT_ACHIEVEMENT_COIN_1000_ID				"693292"
#define _OPENFEINT_ACHIEVEMENT_COIN_10000_ID			"693302"
#define _OPENFEINT_ACHIEVEMENT_COIN_100000_ID			"693312"
#define _OPENFEINT_ACHIEVEMENT_COIN_1000000_ID			"693322"
#define _OPENFEINT_ACHIEVEMENT_COIN_10000000_ID			"693332"
#define _OPENFEINT_ACHIEVEMENT_COIN_100000000_ID		"693342"
#define _OPENFEINT_ACHIEVEMENT_COIN_1000000000_ID		"693352"
#define _OPENFEINT_ACHIEVEMENT_COIN_10000000000_ID		"693362"
#define _OPENFEINT_ACHIEVEMENT_COIN_100000000000_ID		"693372"

#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_2_ID		"693382"
#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_5_ID		"693392"
#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_10_ID		"693402"
#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_15_ID		"693412"
#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_20_ID		"693422"
#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_30_ID		"693432"
#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_40_ID		"693442"
#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_50_ID		"693452"
#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_100_ID		"693462"
#define _OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_1000_ID	"693472"

#define _OPENFEINT_ACHIEVEMENT_BET_COIN_100_ID			"693482"
#define _OPENFEINT_ACHIEVEMENT_BET_COIN_1000_ID			"693492"
#define _OPENFEINT_ACHIEVEMENT_BET_COIN_10000_ID		"693502"
#define _OPENFEINT_ACHIEVEMENT_BET_COIN_100000_ID		"693512"
#define _OPENFEINT_ACHIEVEMENT_BET_COIN_1000000_ID		"693522"

#define _OPENFEINT_ACHIEVEMENT_FIGHT_10_ID				"693582"
#define _OPENFEINT_ACHIEVEMENT_FIGHT_100_ID				"693592"
#define _OPENFEINT_ACHIEVEMENT_FIGHT_1000_ID			"693602"
#define _OPENFEINT_ACHIEVEMENT_FIGHT_10000_ID			"693612"
#define _OPENFEINT_ACHIEVEMENT_FIGHT_100000_ID			"693622"
#define _OPENFEINT_ACHIEVEMENT_FIGHT_1000000_ID			"693632"
#define _OPENFEINT_ACHIEVEMENT_FIGHT_10000000_ID		"693642"

#define _OPENFEINT_ACHIEVEMENT_LOSE_3_ID				"693532"
#define _OPENFEINT_ACHIEVEMENT_LOSE_10_ID				"693542" 
#define _OPENFEINT_ACHIEVEMENT_LOSE_30_ID				"693552"
#define _OPENFEINT_ACHIEVEMENT_LOSE_50_ID				"693562"
#define _OPENFEINT_ACHIEVEMENT_LOSE_100_ID				"693572"

#define _OPENFEINT_LEADERBOARD_RICH_ID					"551824"
#define _OPENFEINT_LEADERBOARD_SURVIVAL_ID				"607774"
#define _OPENFEINT_LEADERBOARD_KILL_ID					"607784"

#define _GAME_SAVE_FILE_SIZE_LATEST_VERSION_STR			"1.1_932jjsdcsmcsmcsdfdkhskfjahsf"

#define _EFFECT_STR_NAIL								"nail"
#define _EFFECT_STR_CUT									"cut"
#define _EFFECT_STR_BITE								"bite"
#define _EFFECT_STR_HIT									"hit"
#define _EFFECT_STR_NAIL_ALL							"nail_all"
#define _EFFECT_STR_CUT_ALL								"cut_all"
#define _EFFECT_STR_BITE_ALL							"bite_all"
#define _EFFECT_STR_HIT_ALL								"hit_all"
#define _EFFECT_STR_POISON								"poison"
#define _EFFECT_STR_REGEN								"regen"

#define _FRIENDSHIP_DOWN_OFFLINE_DOWN					( 1.0f / 3600.0f )

#define _EXP_UP_ONLINE_WORST							( 1.0f / 300.0f )
#define _EXP_UP_ONLINE_BAD								( 1.0f / 280.0f )
#define _EXP_UP_ONLINE_GOOD								( 1.0f / 260.0f )
#define _EXP_UP_ONLINE_BEST								( 1.0f / 230.0f )
#define _EXP_UP_ONLINE_LOVE								( 1.0f / 200.0f )
#define _EXP_UP_OFFLINE_WORST							( 1.0f / 864.0f )
#define _EXP_UP_OFFLINE_BAD								( 1.0f / 854.0f )
#define _EXP_UP_OFFLINE_GOOD							( 1.0f / 844.0f )
#define _EXP_UP_OFFLINE_BEST							( 1.0f / 834.0f )
#define _EXP_UP_OFFLINE_LOVE							( 1.0f / 824.0f )

#define _POSION_HP_UP									( 8 + kdRand ( ) % 2 )

#define _MACHINE_1_BUY_DELAY							( 60 * 60 * 12 )

#define	CLASS_SSS										250
#define	CLASS_SS										150
#define	CLASS_S											100
#define	CLASS_A											60
#define	CLASS_B											30
#define	CLASS_C											10
#define	CLASS_D											5

#define	FRIENDSHIP_LOVE									370
#define	FRIENDSHIP_BEST									170
#define	FRIENDSHIP_GOOD									70
#define	FRIENDSHIP_BAD									20

#define _CAPABLE_POINT_HP_MULT							2
#define _CAPABLE_POINT_ATK_MULT							2
#define _CAPABLE_POINT_DEF_MULT							3
#define _CAPABLE_POINT_AGL_MULT							1
#define _CAPABLE_POINT_CRT_MULT							1

#define _MYROOM_LEVELUP_POINT_HP_MIN					2
#define _MYROOM_LEVELUP_POINT_ATK_MIN					1
#define _MYROOM_LEVELUP_POINT_DEF_MIN					1
#define _MYROOM_LEVELUP_POINT_AGL_MIN					1
#define _MYROOM_LEVELUP_POINT_CRT_MIN					1
#define _MYROOM_LEVELUP_POINT_HP_MAX					6
#define _MYROOM_LEVELUP_POINT_ATK_MAX					4
#define _MYROOM_LEVELUP_POINT_DEF_MAX					3
#define _MYROOM_LEVELUP_POINT_AGL_MAX					5
#define _MYROOM_LEVELUP_POINT_CRT_MAX					4


typedef enum
{
	eDirectionType_Left						,
	eDirectionType_Right					,
} eDirectionType;


typedef enum
{
	eGameType_Tower							,
	eGameType_Casino						,
} eGameType;


typedef enum
{
	eHeroCreateType_Worrier					,
	eHeroCreateType_Magician				,
	eHeroCreateType_Soldier					,
	eHeroCreateType_Babarian				,
	eHeroCreateType_Priest					,
	eHeroCreateType_female_Gunner			,
	eHeroCreateType_Gun_Gril				,
	eHeroCreateType_Wizard					,
	eHeroCreateType_Space_Soldier			,
	eHeroCreateType_Mobile_Knight			,
	eHeroCreateType_End						,
} eHeroCreateType;


typedef enum
{
	eBatStateType_AllView					,				// 모든 몹을 보는 단계.
	eBatStateType_AllToDetailChanging		,				// 베팅을 하는 단계로 바뀌는 연출 중.
	eBatStateType_BetView					,				// 베팅을 하는 단계.
	eBatStateType_DetailToAllChanging		,				// 모든 몹을 보는 단계로 바뀌는 연출 중.
} eBatStateType;


typedef enum _eScriptType
{
	eScriptType_Chance						= 0,			// 해당 스크립트가 나올 확률.
	eScriptType_Skill_delay					,				// 해당 스킬이 발동되는 딜레이 시간.
	eScriptType_Skill_positonType			,				// 해당 스킬의 positon Type
	eScriptType_Skill_targetType			,				// 해당 스킬의 target Type
	eScriptType_Skill_rangeType				,				// 해당 스킬의 range Type
	
	eScriptType_Scparator_1					= 10,
	eScriptType_AttackMotion				,				// 공격 동작을 보여준다.
	eScriptType_SpecialMotion				,				// 특정 동작을 보여준다.

	eScriptType_Scparator_2					= 30,
	eScriptType_Damage						,				// 파라미터 값을 계산해서 데미지 계산을 해준다.

	eScriptType_Scparator_3					= 50,
	eScriptType_WaitEvent					,				// 현재 동작이 끝날 때까지 대기한다.
	eScriptType_WaitTime					,				// 값으로 지정된 시간이 지날 때 까지 대기한다.
	eScriptType_SuperDefenceTime			,				// 슈퍼 디펜스가 빌동되는 시간.

	eScriptType_Scparator_4					= 70,
	eScriptType_TargetClear					,				// 선택된 모든 목표를 취소한다.
	eScriptType_TargetMy					,				// 자신을 목표로 지정한다.
	eScriptType_TargetOne					,				// 자신을 제외한 하나를 랜덤하게 목표로 지정한다.
	eScriptType_TargetAll					,				// 자신을 제외한 전체를 목표로 지정한다.
	eScriptType_TargetRand					,				// 자신을 포함한 하나를 랜덤하게 목표로 지정한다.

	eScriptType_Scparator_5					= 100,
	eScriptType_HP							,				// 목표로 지정된 오브젝트의 HP에 지정된 값을 더한다.(-입력시 내려간다, Damage와 다른점은 파라미터 값을 계산하지 않는다.)
	eScriptType_ATK							,				// 목표로 지정된 오브젝트의 ATK에 지정된 값을 더한다.
	eScriptType_DEF							,				// 목표로 지정된 오브젝트의 DEF에 지정된 값을 더한다.
	eScriptType_AGL							,				// 목표로 지정된 오브젝트의 AGL에 지정된 값을 더한다.
	eScriptType_CRT							,				// 목표로 지정된 오브젝트의 CRT에 지정된 값을 더한다.

	eScriptType_Scparator_6					= 130,
	eScriptType_Effect						,				// 목표로 지정된 오브젝트에 지정된 액션 이름의 이펙트를 출력한다. sub값이 1이면 아이콘 위치에 0이면 센터에 출력한다.
	eScriptType_EffectFlicker				,				// 목표로 지정된 오브젝트 자체를 지정된 시간동안 깜빡인다.
	eScriptType_Time_Effect					,				// 목표로 지정된 오브젝트 자체에 지정된 시간마다 지정된 횟수동안 지정 된 이름의 이펙트를 센터에 출력한다.

	eScriptType_Scparator_7					= 140,
	eScriptType_SoundEffect					,				// 효과음을 출력한다.
		
	eScriptType_Scparator_8					= 150,
	eScriptType_Mob_Active_Gage_1_View		,				// 지정된 몬스터의 액티브 게이지를 모여준다.
	eScriptType_Mob_Active_Gage_All_View	,				// 모든 몬스터의 액티브 게이지를 모여준다.

	eScriptType_End = 250
} eScriptType;


typedef enum
{
	eUnitType_Hero							,
	eUnitType_Mob							,
	eUnitType_End
} eUnitType;


typedef enum
{
	eUniqueType_common						,
	eUniqueType_uncommon					,
	eUniqueType_rare						,
	eUniqueType_unique						,
	eUniqueType_not							,
} eUniqueType;


typedef enum
{
	eUnitAniType_Name_a						,
	eUnitAniType_Name_b						,
	eUnitAniType_Portrait					,
	eUnitAniType_Normal						,
	eUnitAniType_Attack						,
	eUnitAniType_Special					,
	eUnitAniType_Damage						,			
} eUnitAniType;


typedef enum
{
	eStatusIcon_Regene						,
	eStatusIcon_poison						,
	eStatusIcon_AtkUp						,
	eStatusIcon_AtkDown						,
	eStatusIcon_DefUp						,
	eStatusIcon_DefDown						,
	eStatusIcon_AglUp						,
	eStatusIcon_AglDown						,
	eStatusIcon_CrtUp						,
	eStatusIcon_CrtDown						,
	eStatusIcon_Max
} eStatusIcon;


typedef enum
{
	eStatusEffect_Regene					,
	eStatusEffect_poison					,
	eStatusEffect_AtkUp						,
	eStatusEffect_AtkDown					,
	eStatusEffect_DefUp						,
	eStatusEffect_DefDown					,
	eStatusEffect_AglUp						,
	eStatusEffect_AglDown					,
	eStatusEffect_CrtUp						,
	eStatusEffect_CrtDown					,
	eStatusEffect_Max
} eStatusEffect;


typedef enum
{
	ePositionType_Front						,
	ePositionType_Back						,
	ePositionType_End						,
} ePositionType;


typedef enum
{
	eTargetType_Ally_One					,
	eTargetType_Ally_All					,
	eTargetType_Enemy_One					,
	eTargetType_Enemy_All					,
	eTargetType_End							,
} eTargetType;


typedef enum
{
	eRangeType_Short						,
	eRangeType_Long							,
	eRangeType_Back							,
	eRangeType_Long_Long					,
	eRangeType_All							,
	eRangeType_End							,
} eRangeType;


typedef enum
{
	eCreateInfoFileType_Coin_1				= 0,
	eCreateInfoFileType_Coin_2				,
	eCreateInfoFileType_Coin_3				,
	eCreateInfoFileType_Coin_4				,
	eCreateInfoFileType_Coin_5				,
	eCreateInfoFileType_Coin_6				,
	eCreateInfoFileType_Coin_7				,
	eCreateInfoFileType_Coin_8				,
	eCreateInfoFileType_Coin_9				,
	eCreateInfoFileType_Coin_10				,
	eCreateInfoFileType_Coin_11				,
	eCreateInfoFileType_Coin_12				,
	eCreateInfoFileType_Coin_13				,
	eCreateInfoFileType_Premium_1			= 1000,
	eCreateInfoFileType_Premium_2			,
	eCreateInfoFileType_Premium_3			,
	eCreateInfoFileType_Premium_4			,
	eCreateInfoFileType_Casino_1			= 5000,
	eCreateInfoFileType_Casino_2			= 5001,
	eCreateInfoFileType_Casino_3			= 5002,
	eCreateInfoFileType_Casino_4			= 5003,
	eCreateInfoFileType_ArcadeEasy			= 10000,
	eCreateInfoFileType_ArcadeEasyMiddleBoss,
	eCreateInfoFileType_ArcadeEasyLastBoss	,
	eCreateInfoFileType_ArcadeNor			,
	eCreateInfoFileType_ArcadeNorMiddleBoss	,
	eCreateInfoFileType_ArcadeNorLastBoss	,
	eCreateInfoFileType_ArcadeHard			,
	eCreateInfoFileType_ArcadeHardMiddleBoss,
	eCreateInfoFileType_ArcadeHardLastBoss	,
	eCreateInfoFileType_Survival			= 15000,
	eCreateInfoFileType_Hero				= 30000,
	eCreateInfoFileType_Tower				= 31000,
} eCreateInfoFileType;


typedef enum
{
	eModeType_Casino						,
	eModeType_MyRoom_Casino100				,
	eModeType_MyRoom_Casino1000				,
	eModeType_MyRoom_Arcade_Easy			,
	eModeType_MyRoom_Arcade_Nor				,
	eModeType_MyRoom_Arcade_Hard			,
	eModeType_MyRoom_Survival				,
	eModeType_End							,
} eModeType;


typedef enum
{
	eIntoType_Casino						,
	eIntoType_MyRoom						,
} eIntoType;


typedef struct _sSkill
{
	KDint				nType;
	KDfloat				fWaitTime;
	ePositionType		ePosition;
	eTargetType			eTarget;
	eRangeType			eRange;
} sSkill;


typedef enum
{
	eDollarMachineType_Not					,
	eDollarMachineType_Unusual				,
	eDollarMachineType_Prettiness			,
	eDollarMachineType_RegendOfHeroRink		,
	eDollarMachineType_Warvel_vs_Hell		,
} eDollarMachineType;


// struct---------------------------------------------
typedef struct _sTouch
{
	KDint				nId;
	Point				tPoint;
} sTouch;

// function-------------------------------------------
KDbool		RectInPoint ( const Rect& tRect, const Point& tPoint );

KDint		collisionBox ( const Rect& tRect1, const Rect& tRect );

KDbool		readToData ( const KDubyte* pSrcData, const KDint nSrcDataSize, KDint& nSeek, KDvoid* pDestData, const KDint nDestDataSize );




KDvoid		buy_10000_coin					( KDvoid );
KDvoid		buy_33000_coin					( KDvoid );
KDvoid		buy_60000_coin					( KDvoid );
KDvoid		buy_130000_coin					( KDvoid );
KDvoid		buy_3_slot						( KDvoid );
KDvoid		buy_9_slot						( KDvoid );
KDvoid		buy_30_slot						( KDvoid );
KDvoid		buy_machine_unusual				( KDvoid );
KDvoid		buy_machine_prettiness			( KDvoid );
KDvoid		buy_machine_regendOfHeroRink	( KDvoid );
KDvoid		buy_machine_warvel_vs_hell		( KDvoid );

#endif	// __Define_h__
