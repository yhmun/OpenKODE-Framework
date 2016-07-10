/* -----------------------------------------------------------------------------------
 *
 *      File            MainScene.h
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

#ifndef __MainScene_h__
#define __MainScene_h__

#include "Scene/JBase/JBaseScene.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Button/Button.h"
#include "Object/ColorLayer/ColorLayer.h"

typedef struct 
{
	cDraw*			pDrawObject;
	KDint			nStep;
	KDdouble		dPassTime;
	KDdouble		dArrivedTime;
	Point			tStartPoint;
	Point			tEndPoint;
} SMainSceneMoveObject;

class MainScene : public JBaseScene
{
	public :

		CREATE_FUNC ( MainScene );

	public :

		KDbool			init ( KDvoid );

		KDvoid			update ( KDfloat fDelta );
		KDvoid			updateThrowDrawObject ( KDdouble dLeftTime );
		KDvoid			updateMoveObject ( KDdouble dLeftTime );

		KDvoid			onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent );

		KDvoid			onBtnArcadeMode ( Object* pSender );
		KDvoid			onBtnMoreFowcomGames ( Object* pSender );
		KDvoid			onBtnCredis ( Object* pSender );
		KDvoid			onBtnBackgroundMusicOn ( Object* pSender );
		KDvoid			onBtnBackgroundMusicOff ( Object* pSender );
		KDvoid			onBtnEffectMusicOn ( Object* pSender );
		KDvoid			onBtnEffectMusicOff ( Object* pSender );

	private :

		// sprite
		cSprite*							m_pSprBack;
		cSprite*							m_pSprNurungee;
		cSprite*							m_pSprCredis;

		// colorLayer
		cColorLayer*						m_pCredisBackLayer;

		// button
		cButton*							m_pBtnArcadeMode;
		cButton*							m_pBtnMoreFowcomGames;
		cButton*							m_pBtnCredis;
		cButton*							m_pBtnBackgroundMusicOn;
		cButton*							m_pBtnBackgroundMusicOff;
		cButton*							m_pBtnEffectMusicOn;
		cButton*							m_pBtnEffectMusicOff;	

		// switch
		KDbool								m_bIsDirection;
	
		// throw step
		KDdouble							m_dThrowCheckTime;
		KDbool								m_bIsThrowNurungee;
		KDbool								m_bIsThrowArcadeMode;
		KDbool								m_bIsThrowMoreFowcomGames;
		KDbool								m_bIsThrowCredis;

		// move draw object
		std::list<SMainSceneMoveObject>		m_aMoveObjects;
};

#endif	// __MainScene_h__
