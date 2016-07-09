/* --------------------------------------------------------------------------
 *
 *      File            TheLevel.h
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

#ifndef __TheLevel_h__
#define __TheLevel_h__

class Ninja;
class GroundPlane;
class StartPlatform;
class TheStack;
class ContactListener;

class TheLevel : public CCLayer
{
	public :

		SCENE_FUNC ( TheLevel );

		static  TheLevel*	sharedLevel ( KDvoid );

	protected :

		virtual KDbool		init ( KDvoid );		

		virtual KDvoid		draw ( KDvoid );

		// TOUCHES
		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid				showMenuFromSelector ( KDfloat fDelta );

		KDvoid				showMenu ( CCObject* pObejct );

	public :

		KDvoid				switchAllStackObjectsToStatic ( KDfloat fDelta );

		KDvoid				switchAllStackObjectsToDynamic ( KDfloat fDelta );

		KDvoid				EnableDebugMode ( KDvoid );

		KDvoid				setUpParticleSystemSun ( KDvoid );

		KDvoid				tick ( KDfloat fDelta );

		KDvoid				proceedToNextTurn ( Ninja* pTheNinja );

		KDvoid				moveNextNinjaIntoSling ( KDfloat fDelta );

		KDvoid				stopDotting ( KDvoid );

		KDvoid				showNinjaImpactingStack ( Ninja* pTheNinja );

		KDvoid				showNinjaOnGround ( Ninja* pTheNinja );

		KDvoid				makeNinjaStaticOnGround ( KDfloat fDelta );

		KDvoid				moveScreen ( KDint nAmountToShiftScreen );

		KDvoid				putEverythingInStartingViewOfSlingShot ( KDvoid );

		KDvoid				putEverythingInViewOfTargets ( KDvoid );

		// Cock the slingshot to the strech poing given in the pt parameter.
		KDbool				cockTheSlingshot ( const CCPoint& tPoint );

		KDvoid				panTheScreen ( KDint nDiff );

		KDbool				checkCircleCollision ( const CCPoint& tCenter1, KDfloat fRadius1, const CCPoint& tCenter2, KDfloat fRadius2 );

		KDfloat				calculateAngle ( KDfloat x1, KDfloat y1, KDfloat x2, KDfloat y2 );

		KDint				returnAmountToShiftScreen ( KDint nDiff );

		KDvoid				fireSlignshot ( const b2Vec2& tDir );

		KDvoid				removePreviousDots ( KDvoid );

		KDvoid				removeAllDots ( KDvoid );

		KDvoid				placeWhiteDots ( KDfloat fDelta );

		KDvoid				timerAfterThrow ( KDfloat fDelta );

		// SCREEN PANNING
		KDvoid				startScreenPanToTargets ( KDvoid );

		KDvoid				startScreenPanToTargetsWithAutoReverseOn ( KDvoid );

		KDvoid				startScreenPanToSlingIfScoringIsNotOccuring ( KDfloat fDelta );

		KDvoid				startScreenPanToSling ( KDvoid );

		KDvoid				autoScreenPanToTargets ( KDfloat fDelta );

		KDvoid				autoScreenPanToSling ( KDfloat fDelta );

		KDvoid				cancelAutoPan ( KDvoid );

		KDvoid				moveScreenUp ( KDfloat fDelta );

		KDvoid				moveScreenDown ( KDfloat fDelta );		

		// SIMPLE BREAK FX
		KDvoid				showSimpleVisualFX ( const CCPoint& tPositionToShowScore, BreakEffect eTheSimpleScoreVisualFX );

		KDvoid				showPoints ( KDint nPointValue, const CCPoint& tPositionToShowScore, BreakEffect eTheSimpleScoreVisualFX );

		KDvoid				showPointsWithImagesForValue ( KDint nPointValue, const CCPoint& tPositionToShowScore );

		KDvoid				showPointsWithFontLabelForValue ( KDint nPointValue, const CCPoint& tPositionToShowScore );

		// POINTS
		KDvoid				updatePointsLabel ( KDvoid );

		KDvoid				somethingJustScoredVar ( KDvoid );

		KDvoid				resetSomethingJustScoredVar ( KDfloat fDelta );

		KDvoid				adjustBackStrap ( KDfloat fAngle );

		KDvoid				showBoardMessage ( const KDchar* szTheMessage );

		KDvoid				doPointBonusForExtraNinjas ( KDvoid );

		KDvoid				resetOrAdvanceLevel ( KDfloat fDelta );

		KDvoid				transitionAfterMenuPop ( KDvoid );

		KDvoid				transitionOut ( KDfloat fDelta );

		KDvoid				cleanupTheLevel ( KDvoid );
		
	public :

		KDbool				IS_IPAD;
		KDbool				IS_IPHONE;
		KDbool				IS_RETINA;

	private :

		KDint				m_nCurrentLevel;
		KDint				m_nPointTotalThisRound;
		KDint				m_nPointsToPassLevel;
		KDint				m_nBonusThisRound;
		KDint				m_nBonusPerExtraNinja;		
		KDfloat				m_fFontSizeForScore;
		
		KDfloat				m_fPanAmount;
		KDint				m_nInitialPanAmount;
		KDint				m_nExtraAmountOnPanBack;
		KDint				m_nMaxStretchOfSlingShot;
		
		KDbool				m_bOpenWithMenuInsteadOfGame;
		
		KDbool				m_bTopLeftTouchEnablesDebugMode;
		
		KDbool				m_bStackIsNowDynamic;
		
		KDbool				m_bAreWeOnTheIPad;
		
		KDbool				m_bUseImagesForPointScoreLabels;
		
		KDbool				m_bSomethingJustScored;
		KDbool				m_bDottingOn;
		KDbool				m_bAreWeInTheStartingPosition;
		KDbool				m_bSlingShotNinjaInHand;					// if the ninja is in the sling
		KDbool				m_bThrowInProgress;							// if the ninja is currently being thrown (in midair / midturn basically)
		KDbool				m_bAutoPanningInProgress;
		KDbool				m_bReverseHowFingerPansScreen;				// reverses which direction the screen moves based on the finger moving left or right
		KDbool				m_bPanningTowardSling;
		KDbool				m_bContinuePanningScreenOnFingerRelease;
		KDbool				m_bAutoReverseOn;
		
		KDfloat				m_fMultipyThrowPower; 

		KDfloat				m_fAxisGravityY; 
		KDbool				m_bGravityOn;

		
		b2World*			m_pWorld;
		ContactListener*	m_pContactListener;

		// the entire stack		
		TheStack*			m_pStack;

		// background art		
		CCSprite*			m_pBackgroundLayerClouds;
		CCSprite*			m_pBackgroundLayerHills;
		CCParticleSystem*	m_pSystem;

		CCPoint				m_tCloudLayerStartPosition;
		CCPoint				m_tHillsLayerStartPosition;
		CCPoint				m_tParticleSystemStartPosition;
		CCPoint				m_tLabelStartingPoint;

		// sling art		
		CCSprite*			m_pSlingShotFront;
		
		CCSprite*			m_pStrapFront;
		CCSprite*			m_pStrapBack;
		CCSprite*			m_pStrapEmpty;

		// BOX2D object start positions  (will probably vary between iPAd and iPhone		
		CCPoint				m_tGroundPlaneStartPosition;
		CCPoint				m_tPlatformStartPosition;
		CCPoint				m_tNinjaStartPosition1;
		CCPoint				m_tNinjaStartPosition2;
		CCPoint				m_tNinjaStartPosition3;
		CCPoint				m_tNinjaStartPosition4;
		CCPoint				m_tNinjaStartPosition5;

		// used for moving the world to see the targets..
		KDfloat				m_fWorldMaxHorizontalShift;
		KDfloat				m_fPreviousTouchLocationX;
		
		KDfloat				m_fAdjustY;
		KDfloat				m_fMaxScaleDownValue;
		KDfloat				m_fScaleAmount;
		KDfloat				m_fSpeed;
				
		CCLabelTTF*			m_pHighScoreLabel;
		CCLabelTTF*			m_pCurrentScoreLabel;
		CCPoint				m_tCurrentScoreLabelStartPosition;
		CCPoint				m_tHighScoreLabelStartPosition;

		CCPoint				m_tMenuStartPosition;
		
		CCPoint				m_tSlingShotCenterPosition;
		CCPoint				m_tPositionInSling;

		// white dots		
		KDint				m_nDotCount;
		KDint				m_nThrowCount;
		KDint				m_nDotTotalOnEvenNumberedTurn;
		KDint				m_nDotTotalOnOddNumberedTurn;

		// menu button		
		CCMenu*				m_pMenuButton;

		// ground and start platform
		GroundPlane*		m_pTheGroundPlane;
		StartPlatform*		m_pThePlatform;

		// current character being thrown		
		Ninja*				m_pCurrentBodyNode;
		Ninja*				m_pNinja1;

		// ninjas in line to be thrown		
		Ninja*				m_pNinja2;
		Ninja*				m_pNinja3;
		Ninja*				m_pNinja4;
		Ninja*				m_pNinja5;
		
		KDint				m_nNinjasToTossThisLevel;
		KDint				m_nNinjaBeingThrown;		
};

#endif	// __TheLevel_h__
