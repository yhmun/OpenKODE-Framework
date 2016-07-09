/* --------------------------------------------------------------------------
 *
 *      File            Enemy.h
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

#ifndef __Enemy_h__
#define __Enemy_h__

#include "BodyNode.h"

class Enemy : public BodyNode
{
	public :

		static Enemy*			create		(	b2World*		pWorld, 
												const CCPoint&	tLocation, 
												const KDchar*	szSpriteFileName,
												KDbool			bIsTheRotationFixed,
												KDbool			bGetsDamageFromGround,
												KDbool			bDoesGetDamageFromDamageEnabledStackObjects,
												KDint			nBreaksFromHowMuchContact,
												KDbool			bHasDifferentSpritesForDamage,
												KDint			nNumberOfFramesToAnimateOnBreak,
												KDfloat			fDensity,
												CreationMethod	eCreateHow,
												KDint			nPoints,
												BreakEffect		eSimpleScoreVisualFXType );
	public :

		virtual KDbool			InitWithWorld ( b2World*		pWorld, 
												const CCPoint&	tLocation, 
												const KDchar*	szSpriteFileName,
												KDbool			bIsTheRotationFixed,
												KDbool			bGetsDamageFromGround,
												KDbool			bDoesGetDamageFromDamageEnabledStackObjects,
												KDint			nBreaksFromHowMuchContact,
												KDbool			bHasDifferentSpritesForDamage,
												KDint			nNumberOfFramesToAnimateOnBreak,
												KDfloat			fDensity,
												CreationMethod	eCreateHow,
												KDint			nPoints,
												BreakEffect		eSimpleScoreVisualFXType );

		virtual KDvoid			damageEnemy ( KDvoid );
		
		virtual KDvoid			enemyCanBeDamagedAgain ( KDfloat fDelta );
		
		virtual KDvoid			breakEnemy ( KDvoid );
		
		virtual KDvoid			startBreakAnimation ( KDfloat fDelta );

		virtual KDvoid			blink ( KDvoid );
		
		virtual KDvoid			openEyes ( KDvoid );

		virtual KDvoid			makeUnScoreable ( KDvoid );

		virtual KDbool			 isBreaksOnNextDamage ( KDvoid );
		virtual KDvoid			setBreaksOnNextDamage ( KDbool bValue );

		virtual KDbool			 isDamagesFromGroundContact ( KDvoid );
		virtual KDvoid			setDamagesFromGroundContact ( KDbool bValue );

		virtual KDbool			 isDamagesFromDamageEnabledStackObjects ( KDvoid );
		virtual KDvoid			setDamagesFromDamageEnabledStackObjects ( KDbool bValue );

		virtual KDint			getPointValue ( KDvoid );
		virtual KDvoid			setPointValue ( KDint nValue );

		virtual BreakEffect		getSimpleScoreVisualFX ( KDvoid );
		virtual KDvoid			setSimpleScoreVisualFX ( BreakEffect eValue );

	private :

		KDvoid					createEnemy ( KDvoid );

	protected :

		b2World*				m_pTheWorld;
		std::string				m_sBaseImageName;
		std::string				m_sSpriteImageName;
		CCPoint					m_tInitialLocation;
		KDint					m_nBreaksAfterHowMuchContact;				// 0 will break the enemy after first contact
		KDint					m_nDamageLevel;								// tracks how much damage has been done
		KDbool					m_bBreaksOnNextDamage;			
		KDbool					m_bIsRotationFixed;
		KDfloat					m_fTheDensity;
		CreationMethod			m_eShapeCreationMethod;						// same as all stack objects, check shape definitions in Constants.h
		KDbool					m_bDamagesFromGroundContact;
		KDbool					m_bDamagesFromDamageEnabledStackObjects;	// stack objects must be enabled to damageEnemy 
		KDbool					m_bDifferentSpritesForDamage;				// whether or not you've included different images for damage progression (recommended)
		KDint					m_nPointValue;
		BreakEffect				m_eSimpleScoreVisualFX;						// defined in constants, which visual effect occurs when the enemy breaks
		KDint					m_nCurrentFrame;
		KDint					m_nFramesToAnimateOnBreak;					// if 0 won't show any break frames		
		KDbool					m_bEnemyCantBeDamagedForShortInterval;		// after damage occurs the enemy gets a moment of un-damage-abilty, which should play better ( I think)
};

#endif	// __GroundPlane_h__
