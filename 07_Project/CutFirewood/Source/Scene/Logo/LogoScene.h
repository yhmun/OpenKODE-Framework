/* -----------------------------------------------------------------------------------
 *
 *      File            LogoScene.h
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

#ifndef __LogoScene_h__
#define __LogoScene_h__

#include "Scene/JBase/JBaseScene.h"
#include "Object/Animation/Animations.h"

class LogoScene : public JBaseScene
{
	public :

		CREATE_FUNC ( LogoScene );

	public :

		KDbool			init ( KDvoid );

		// touches
		KDvoid			onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

		// update
		KDvoid			update ( KDfloat fDelta );		
		KDvoid			updateLogo ( KDdouble dLeftTime );
		KDvoid			updateNurungee ( KDdouble dLeftTime );
		KDvoid			updateNurungeePunch	( KDdouble dLeftTime );		

		KDvoid			timeout ( KDfloat fDelta );

	private :

		cAnimations*	m_pAniLogo;
		cAnimations*	m_pAniNurungee;
		cAnimations*	m_pAniNurungeePunch;

		// step
		KDint			m_nStepLogo;
		KDint			m_nStepNurungee;
		KDint			m_nStepNurungeePunch;
	
		// logo
		KDdouble		m_dLogoTime;
	
		// nurungee
		KDfloat			m_fNurungVibrate;
	
		// nurungee punch
		KDdouble		m_dNurungPunchTime;
		KDfloat			m_fNurungPunchVibrate;
	
		// next scene
		KDdouble		m_dNextScentDelay;
};

#endif	// __LogoScene_h__
