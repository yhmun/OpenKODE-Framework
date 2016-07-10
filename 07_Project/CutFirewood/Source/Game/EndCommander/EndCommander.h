/* -----------------------------------------------------------------------------------
 *
 *      File            EndCommander.h
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

#ifndef __EndCommander_h__
#define __EndCommander_h__

#include "Scene/JBase/JBaseGameScene.h"

//#import "FBConnect.h"

enum eResultType
{
	eResultType_Next,
	eResultType_Exit
};

class cSprite;
class cButton;
class cLabelAtlasCostom;

//@class SHKItem;

class cEndCommander : public Object //<FBSessionDelegate, FBRequestDelegate>
{
	public :
				 cEndCommander ( JBaseGameScene* pScene );
		virtual ~cEndCommander ( KDvoid );

		// view
		KDvoid					viewNext ( KDbool bIsPerfectSuccess, KDbool bIsBerserkSuccess, KDbool bIsTimeStopSuccess, KDint nMaxPowerCount, KDint nTotalScore );
		KDvoid					viewExit ( KDint nTotalScore );

		// update
		KDvoid					update	   ( KDdouble dLeftTime );
		KDvoid					updateNext ( KDdouble dLeftTime );
		KDvoid					updateExit ( KDdouble dLeftTime );

		// btn delegate
		KDvoid					onBtnArcadeMode ( Object* pSender );
		KDvoid					onBtnTwitter	( Object* pSender );
		KDvoid					onBtnFacebook	( Object* pSender );
		KDvoid					onBtnRank		( Object* pSender );
		KDvoid					onBtnRetry		( Object* pSender );		
		KDvoid					onBtnNext		( Object* pSender );
		KDvoid					onBtnExit		( Object* pSender );

	/*
//init
- (id)init:(J_baseGameScene *)scene;







//facebook
- (void)session:(FBSession*)session didLogin:(FBUID)uid;
- (void)session:(FBSession*)session willLogout:(FBUID)uid;
- (void)getFacebookName;
- (void)request:(FBRequest*)request didLoad:(id)result;
- (void)postToWall;
- (void)viewDidUnload;
*/
	private :

		// scene
		JBaseGameScene*			m_pScene;
	
		// sprite
		cSprite*				m_pSprBackNext;
		cSprite*				m_pSprBackExit;
		cSprite*				m_pSprPerfectFailed;
		cSprite*				m_pSprBerserkFailed;
		cSprite*				m_pSprTimeStopFailed;
		cSprite*				m_pSprMaxPowerFailed;
	
		// button
		cButton*				m_pBtnTwitter;
		cButton*				m_pBtnFacebook;
		cButton*				m_pBtnRank;
		cButton*				m_pBtnRetry;
		cButton*				m_pBtnNext;
		cButton*				m_pBtnExit;
	
		// label
		cLabelAtlasCostom*		m_pLblPerfect;
		cLabelAtlasCostom*		m_pLblBerserk;
		cLabelAtlasCostom*		m_pLblTimeStop;
		cLabelAtlasCostom*		m_pLblMaxPower;
		cLabelAtlasCostom*		m_pLblTotal;
		cLabelAtlasCostom*		m_pLblEndScore;
		cLabelAtlasCostom*		m_pLblBestScore;

		// point
		Point					m_tPos;
		Point					m_tPosBack;
		Point					m_tPosPerfect;
		Point					m_tPosBerserk;
		Point					m_tPosTimeStop;
		Point					m_tPosMaxPower;
		Point					m_tPosTotal;
		Point					m_tPosEndScore;
		Point					m_tPosBestScore;
		Point					m_tPosTwitter;
		Point					m_tPosFacebook;
		Point					m_tPosRank;

		// result type
		eResultType				m_eResultType;

		// step
		KDint					m_nStep;
		KDdouble				m_dStepLeftTime;
	
		// time, score
		KDdouble				m_dAddTime;
		KDdouble				m_dAddScore;

		// switch
		KDbool					m_bIsRetry;
	
		// facebook
//		FBSession*				m_pSession;
//		FBLoginDialog*			m_pLoginDialog;
		std::string				m_sFacebookName;
		KDbool					m_bIsPosting;
};

#endif	// __EndCommander_h__
