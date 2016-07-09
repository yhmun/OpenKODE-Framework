/* --------------------------------------------------------------------------
 *
 *      File            StackObject.h
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

#ifndef __StackObject_h__
#define __StackObject_h__

#include "BodyNode.h"

class StackObject : public BodyNode
{
	public :

		static StackObject*		create		(	b2World*		pWorld, 
												const CCPoint&	tLocation, 
												const KDchar*	szSpriteFileName,
												KDbool			bBreaksOnGround,
												KDbool			bBreaksFromNinja,
												KDbool			bHasAnimatedBreakFrames,
												KDbool			bDamagesEnemy,
												KDfloat			fDensity,
												CreationMethod	eCreateHow,
												KDint			nAngleChange,
												KDbool			bMakeImmovable,
												KDint			nPoints,
												BreakEffect		eSimpleScoreVisualFXType );
	public :

		virtual KDbool			InitWithWorld ( b2World*		pWorld, 
												const CCPoint&	tLocation, 
												const KDchar*	szSpriteFileName,
												KDbool			bBreaksOnGround,
												KDbool			bBreaksFromNinja,
												KDbool			bHasAnimatedBreakFrames,
												KDbool			bDamagesEnemy,
												KDfloat			fDensity,
												CreationMethod	eCreateHow,
												KDint			nAngleChange,
												KDbool			bMakeImmovable,
												KDint			nPoints,
												BreakEffect		eSimpleScoreVisualFXType );

		virtual KDvoid			playBreakAnimationFromNinjaContact ( KDvoid );

		virtual KDvoid			playBreakAnimationFromGroundContact ( KDvoid );

		virtual KDvoid			startBreakAnimation ( KDvoid );

		virtual KDvoid			makeUnScoreable ( KDvoid );

		virtual KDbool			 isBreaksOnGroundContact ( KDvoid );
		virtual KDvoid			setBreaksOnGroundContact ( KDbool bValue );

		virtual KDbool			 isBreaksOnNinjaContact ( KDvoid );
		virtual KDvoid			setBreaksOnNinjaContact ( KDbool bValue );

		virtual KDbool			 isCanDamageEnemy ( KDvoid );
		virtual KDvoid			setCanDamageEnemy ( KDbool bValue );

		virtual KDbool			 isStatic ( KDvoid );
		virtual KDvoid			setStatic ( KDbool bValue );

		virtual KDint			getPointValue ( KDvoid );
		virtual KDvoid			setPointValue ( KDint nValue );

		virtual BreakEffect		getSimpleScoreVisualFX ( KDvoid );
		virtual KDvoid			setSimpleScoreVisualFX ( BreakEffect eValue );

	private :

		KDvoid					createObject ( KDvoid );

	protected :

		b2World*				m_pTheWorld;
		std::string				m_sSpriteImageName;
		std::string				m_sBaseImageName;
		CCPoint					m_tInitialLocation;
		KDbool					m_bAddedAnimatedBreakFrames;
		KDbool					m_bIsBreaksOnGroundContact;
		KDbool					m_bIsBreaksOnNinjaContact;
		KDbool					m_bIsCanDamageEnemy;
		KDbool					m_bIsStatic;
		KDfloat					m_fTheDensity;
		CreationMethod			m_eShapeCreationMethod;
		KDint					m_nAngle;		
		KDint					m_nCurrentFrame;
		KDint					m_nFramesToAnimate;
		KDint					m_nPointValue;
		BreakEffect				m_eSimpleScoreVisualFX;
};

#endif	// __StackObject_h__
