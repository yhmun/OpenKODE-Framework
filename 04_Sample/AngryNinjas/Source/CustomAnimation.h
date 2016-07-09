/* --------------------------------------------------------------------------
 *
 *      File            CustomAnimation.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __CustomAnimation_h__
#define __CustomAnimation_h__

class CustomAnimation : public CCNode
{
	public :

		static CustomAnimation*		create				( const KDchar*	szTheFileNameToAnimate,
														  KDint			nTheFrameToStartWith,
														  KDint			nTheNumberOfFramesToAnimate,
														  KDint			nTheX,
														  KDint			nTheY,
														  KDbool		bFlipOnX,
														  KDbool		bFlipOnY,
														  KDbool		bDoesItLoop,
														  KDbool		bDoesItUseRandomFrameToLoop );
	protected :

		virtual KDbool		InitWithOurOwnProperties	( const KDchar*	szTheFileNameToAnimate,
														  KDint			nTheFrameToStartWith,
														  KDint			nTheNumberOfFramesToAnimate,
														  KDint			nTheX,
														  KDint			nTheY,
														  KDbool		bFlipOnX,
														  KDbool		bFlipOnY,
														  KDbool		bDoesItLoop,
														  KDbool		bDoesItUseRandomFrameToLoop );

		KDvoid				runAnimation ( KDfloat fDelta );

		KDvoid				changeOpacityTo ( KDubyte cOpacity );

		KDvoid				tintMe ( const ccColor3B& tColor );

	private :

		std::string			m_sFileNameToAnimate;
		KDint				m_nCurrentFrame;
		KDint				m_nFramesToAnimate;
		KDint				m_nFrameToStartWith;
		
		CCSprite*			m_pSomeSprite;
		
		KDbool				m_bAnimationFlippedX;
		KDbool				m_bAnimationFlippedY;
		KDbool				m_bDoesTheAnimationLoop;
		KDbool				m_bUseRandomFrameToLoop;
};

#endif	// __CustomAnimation_h__
