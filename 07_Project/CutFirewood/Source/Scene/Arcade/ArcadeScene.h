/* -----------------------------------------------------------------------------------
 *
 *      File            ArcadeScene.h
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

#ifndef __ArcadeScene_h__
#define __ArcadeScene_h__

#include "Scene/JBase/JBaseGameScene.h"

class cBackImg;
class cStartDirector;
class cColorLayer;
class cCommander;
class cSprite;
class cLabelAtlasCostom;
class cScore;
class cGameTime;
class cHouse;
class cCloud;
class cEndCommander;

class ArcadeScene : public JBaseGameScene
{
		enum eArcadeTypeState
		{
			eArcadeTypeState_Start,
			eArcadeTypeState_Game ,
			eArcadeTypeState_House,
			eArcadeTypeState_Next ,
			eArcadeTypeState_End
		};

	public :
				 ArcadeScene ( KDvoid );
		virtual ~ArcadeScene ( KDvoid );

		CREATE_FUNC ( ArcadeScene );

	public :

		KDbool				init ( KDvoid );

		// step
		KDvoid				update ( KDfloat fDelta );

		KDbool				stepPose  ( KDdouble dLeftTime );
		KDvoid				stepStart ( KDdouble dLeftTime );
		KDvoid				stepGame  ( KDdouble dLeftTime );
		KDvoid				stepHouse ( KDdouble dLeftTime );
		KDvoid				stepNext  ( KDdouble dLeftTime );
		KDvoid				stepEnd   ( KDdouble dLeftTime );

		// player
		KDvoid				addPlayer ( KDvoid );

		// msg
		KDvoid				msgSprBackGray ( KDbool bIsGray );
		KDvoid				msgCloudAdd ( KDint z, KDdouble dStartDelay, Point tPoint, Point tAdd, KDdouble dLeftTime, KDfloat fScale, KDfloat fScaleAdd, KDfloat fRotation, KDfloat fRotationAdd, KDbool bIsFlipX );
		KDvoid				msgClearPointAdd ( KDint nPoint );
		KDvoid				msgAddTime ( KDdouble dTime );
		KDvoid				msgAddScore ( KDint nScore );
		KDvoid				msgPlay ( KDvoid );
		KDvoid				msgRetry ( KDvoid );

		KDvoid				onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent );
		KDvoid				onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent );
		KDvoid				onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

	private :

		// background img
		cBackImg*			m_pBackImg;

		// effect director
		cStartDirector*		m_pStartDirector;
	
		// commander
		cCommander*			m_pCommander;

		// score
		cScore*				m_pScore;
							
		// time
		cGameTime*			m_pGameTime;

		// house
		cHouse*				m_pHouse;

		// cloud
		cCloud*				m_pCloud;

		// end commander
		cEndCommander*		m_pEndCommander;
			
		// state
		eArcadeTypeState	m_eState;

		// pose
		cColorLayer*		m_pPoseColorLayer;
		cSprite*			m_pPoseSprite;
		
		// stage
		KDint				m_nHouseLv;
	
		// clear point
		KDint				m_nClearLeftPoint;

		// background music
		KDbool				m_bIsBackgoundMusicFast;
	
		// temp
		cLabelAtlasCostom*	m_pLabelTemp;		
};

#endif	// __ArcadeScene_h__
