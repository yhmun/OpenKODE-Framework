/* --------------------------------------------------------------------------
 *
 *      File            APSCCActionInterval.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __APSCCActionInterval_h__
#define __APSCCActionInterval_h__

#include "APSGraphic.h"
#include "../../actions/CCActionInterval.h"

NS_APS_BEGIN

/** 
 *	@brief APSCCFadeChildrenTo 
 *	Cocos2D does not support reverse ( ) method for none relative actions.
 *	However, ArtPig supports it by storing graphic states when a none relative
 *	action starts. In ArtPig, none relative action can have only one reverse 
 *	instance, so calling reverse ( ) method multiple times returns only one instance.
 *
 *	Instead of changing CCRGBAProtocol's opacity directly, APSCCFadeChildrenTo 
 *	updates target APSGraphic object's runningOpacity property. Eventually 
 *	updateAccOpacity ( ) of target APSGraphic is called, and the graphic's opacity 
 *	propagates to all descendants by multiplying opacities. During the propagation, 
 *	if a child's CCNode implements CCRGBAProtocol, the CCNode object's opacity is 
 *	updated.
 */
class APSCCFadeChildrenTo : public CCActionInterval
{
	public :
				 APSCCFadeChildrenTo ( KDvoid );
		virtual ~APSCCFadeChildrenTo ( KDvoid );
	    
		/** creates an action with duration and opacity */
		static  APSCCFadeChildrenTo*	actionWithDuration ( KDfloat fDuration, GLubyte cOpacity, APSGraphic* pTargetGraphic );

	public :

		/** initializes the action with duration and opacity */
		virtual KDbool			initWithDuration ( KDfloat fDuration, GLubyte cOpacity, APSGraphic* pTargetGraphic );
	    
		virtual CCObject*		copyWithZone ( CCZone* pZone );
		virtual KDvoid			startWithTarget ( CCNode* pNode = KD_NULL );
		virtual KDvoid			update ( KDfloat fTime );

		virtual CCActionInterval*  reverse ( KDvoid );
	    
	protected :

		APS_SYNTHESIZE ( APSGraphic*, m_pTargetGraphic, TargetGraphic );

		APS_SYNTHESIZE ( GLubyte, m_cToOpacity  , ToOpacity   );
		APS_SYNTHESIZE ( GLubyte, m_cFromOpacity, FromOpacity );

	protected :

		APSCCFadeChildrenTo*	m_pReverseAction;
};

/**
 *	@brief APSCCFadeTo 
 *	Cocos2D does not support reverse ( ) method for none relative actions.
 *	However, ArtPig supports it by storing graphic states when a none relative
 *	action starts. In ArtPig, none relative action can have only one reverse 
 *	instance, so calling reverse ( ) method multiple times returns only one instance. 
 */
class APSCCFadeTo : public CCFadeTo 
{
	public :
				 APSCCFadeTo ( KDvoid );
		virtual ~APSCCFadeTo ( KDvoid );
	    
		static  APSCCFadeTo*		actionWithDuration ( KDfloat fDuration, GLubyte cOpacity, KDbool bFinishRecovery = KD_FALSE );
	    
	public :

		/** initializes the action with duration, opacity, and finishRecovery */
		virtual KDbool				initWithDuration ( KDfloat fDuration, GLubyte cOpacity, KDbool bFinishRecovery = KD_FALSE );
		
		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pNode = KD_NULL );
	    
		virtual CCActionInterval*	reverse ( KDvoid );
	    
		virtual KDvoid				stop ( KDvoid );
	    
		KDbool						getFinishRecovery ( KDvoid ) const { return m_bFinishRecovery; }
	    
	protected :

		APSCCFadeTo*				m_pReverseAction;
		KDbool						m_bFinishRecovery;
};

/** 
 *	@brief APSCCMoveTo 
 *	Cocos2D does not support reverse ( ) method for none relative actions.
 *	However, ArtPig supports it by storing graphic states when a none relative
 *	action starts. In ArtPig, none relative action can have only one reverse 
 *	instance, so calling reverse ( ) method multiple times returns only one instance. 
 */
class APSCCMoveTo : public CCMoveTo
{
	public :
				 APSCCMoveTo ( KDvoid );
		virtual ~APSCCMoveTo ( KDvoid );
	    
		static APSCCMoveTo*  actionWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDbool bFinishRecovery = KD_FALSE );
	    
	public :

		/** initializes the action with duration, position, and finishRecovery */
		KDbool  initWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDbool bFinishRecovery = KD_FALSE );

		virtual CCObject*  copyWithZone ( CCZone* pZone );

		virtual KDvoid  startWithTarget ( CCNode* pNode = KD_NULL );
	    
		virtual CCActionInterval*  reverse ( KDvoid );
	    
		virtual KDvoid  stop ( KDvoid );
	    
		KDbool  getFinishRecovery ( KDvoid ) const { return m_bFinishRecovery; }
	    
	protected :
		APSCCMoveTo *m_pReverseAction;
		KDbool m_bFinishRecovery;
};

/** 
 *	@brief APSCCMoveBy allows finishRecovery property
 */
class APSCCMoveBy : public CCMoveBy
{
	public :

		APSCCMoveBy ( KDvoid );
	    
		static APSCCMoveBy*  actionWithDuration ( KDfloat fDuration, const CCPoint& position, KDbool bFinishRecovery = KD_FALSE );

	public :

		/** initializes the action */
		KDbool  initWithDuration ( KDfloat fDuration, const CCPoint& position, KDbool bFinishRecovery = KD_FALSE );

		virtual CCObject*  copyWithZone ( CCZone* pZone );

		virtual CCActionInterval*  reverse ( KDvoid );
	    
		virtual KDvoid  stop ( KDvoid );
	    
		KDbool  getFinishRecovery ( KDvoid ) { return m_bFinishRecovery; }
	    
	protected :

		KDbool		m_bFinishRecovery;
};

/** 
 *	@brief APSCCBezierTo 
 *	Cocos2D does not support reverse ( ) method for none relative actions.
 *	However, ArtPig supports it by storing graphic states when a none relative
 *	action starts. In ArtPig, none relative action can have only one reverse 
 *	instance, so calling reverse ( ) method multiple times returns only one instance.  
 */
class APSCCBezierTo : public CCBezierTo 
{
	public :
				 APSCCBezierTo ( KDvoid );
		virtual ~APSCCBezierTo ( KDvoid );
	    
		static  APSCCBezierTo*  actionWithDuration ( KDfloat fDuration, const ccBezierConfig& tConfig, KDbool bFinishRecovery = KD_FALSE );
	    
	public :

		virtual KDbool  initWithDuration ( KDfloat fDuration, const ccBezierConfig& tConfig, KDbool bFinishRecovery = KD_FALSE );
		
		virtual CCObject*  copyWithZone ( CCZone* pZone );

		virtual KDvoid  startWithTarget ( CCNode* pNode = KD_NULL );
	    
		virtual CCActionInterval*  reverse ( KDvoid );
	    
		virtual KDvoid  stop ( KDvoid );
	    
		KDbool  getFinishRecovery ( KDvoid ) const { return m_bFinishRecovery; }
	    
	protected :

		APSCCBezierTo*		m_pReverseAction;
		KDbool				m_bFinishRecovery;
};

/** 
 *	@brief APSCCBezierBy allows finishRecovery property
 */
class APSCCBezierBy : public CCBezierBy
{
	public :

		APSCCBezierBy ( KDvoid );;
	    
		static  APSCCBezierBy*  actionWithDuration ( KDfloat fDuration, const ccBezierConfig& tConfig, KDbool bFinishRecovery = KD_FALSE );
	    
	public :

		/** initializes the action */
		KDbool  initWithDuration ( KDfloat fDuration, const ccBezierConfig& tConfig, KDbool bFinishRecovery = KD_FALSE );
	    
		virtual CCObject*  copyWithZone ( CCZone* pZone );

		virtual CCActionInterval*  reverse ( KDvoid );
	    
		virtual KDvoid  stop ( KDvoid );
	    
		KDbool  getFinishRecovery ( KDvoid ) { return m_bFinishRecovery; }
	    
	protected :

		KDbool		m_bFinishRecovery;
};

/**
 *	@brief APSCCMoveSequence inherits from CCSequence and allows finishRecovery property
 */
class APSCCMoveSequence : public CCSequence 
{
	public :

		APSCCMoveSequence ( KDvoid );
	    
		static  APSCCMoveSequence*  actionOneTwo ( CCFiniteTimeAction* pActionOne, CCFiniteTimeAction* pActionTwo, KDbool bFinishRecovery = KD_FALSE );
	    
	public :

		KDbool  initOneTwo ( CCFiniteTimeAction* pActionOne, CCFiniteTimeAction* pActionTwo, KDbool bFinishRecovery = KD_FALSE );
	    
		virtual CCObject*  copyWithZone ( CCZone* pZone );
	    
		virtual KDvoid startWithTarget ( CCNode *pTarget );
	    
		virtual CCActionInterval*  reverse ( KDvoid );
	    
		virtual KDvoid  stop ( KDvoid );
	    
		KDbool  getFinishRecovery ( KDvoid ) { return m_bFinishRecovery; }
	    
	protected :

		CCPoint		m_tStartPosition;
		KDbool		m_bFinishRecovery;
};

/** 
 *	@brief APSCCRotateTo 
 *	Cocos2D does not support reverse ( ) method for none relative actions.
 *	However, ArtPig supports it by storing graphic states when a none relative
 *	action starts. In ArtPig, none relative action can have only one reverse 
 *	instance, so calling reverse ( ) method multiple times returns only one instance. 
 */
class APSCCRotateTo : public CCRotateTo 
{
	public :
				 APSCCRotateTo ( KDvoid );
		virtual ~APSCCRotateTo ( KDvoid );
	    
		static  APSCCRotateTo*  actionWithDuration ( KDfloat fDuration, KDfloat fAngle, KDbool bFinishRecovery = KD_FALSE );
	        
		/** initializes the action */
		KDbool  initWithDuration ( KDfloat fDuration, KDfloat fAngle, KDbool bFinishRecovery = KD_FALSE );
	    
		virtual CCObject*  copyWithZone ( CCZone* pZone );

		virtual KDvoid  startWithTarget ( CCNode* pNode = KD_NULL );
	    
		virtual CCActionInterval*  reverse ( KDvoid );
	    
		virtual KDvoid  stop ( KDvoid );
	    
		KDbool  getFinishRecovery ( KDvoid ) const { return m_bFinishRecovery; }

	protected :

		APSCCRotateTo*	m_pReverseAction;
		KDbool			m_bFinishRecovery;
};

/** 
 *	@brief APSCCRotateBy allows finishRecovery property
 */
class APSCCRotateBy : public CCRotateBy
{
	public :

		APSCCRotateBy ( KDvoid );
	    
		static  APSCCRotateBy*  actionWithDuration ( KDfloat fDuration, KDfloat fAngle, KDbool bFinishRecovery = KD_FALSE );

	public :

		/** initializes the action */
		KDbool  initWithDuration ( KDfloat fDuration, KDfloat fAngle, KDbool bFinishRecovery = KD_FALSE );
	    
		virtual CCObject*  copyWithZone ( CCZone* pZone );
	    
		virtual CCActionInterval*  reverse ( KDvoid );
	    
		virtual KDvoid  stop ( KDvoid );
	    
		KDbool  getFinishRecovery ( KDvoid ) const { return m_bFinishRecovery; }
	    
	protected :

		KDbool		m_bFinishRecovery;
};

/** 
 *	@brief APSCCScaleTo 
 *	Cocos2D does not support reverse ( ) method for none relative actions.
 *	However, ArtPig supports it by storing graphic states when a none relative
 *	action starts. In ArtPig, none relative action can have only one reverse 
 *	instance, so calling reverse ( ) method multiple times returns only one instance. 
 */
class APSCCScaleTo : public CCScaleTo 
{
	public :
				 APSCCScaleTo ( KDvoid );
		virtual ~APSCCScaleTo ( KDvoid );
	    
		static  APSCCScaleTo*  actionWithDuration ( KDfloat fDuration, KDfloat sx, KDfloat sy, KDbool bFinishRecovery = KD_FALSE );

	public :

		/** initializes the action with and X factor and a Y factor */
		KDbool  initWithDuration ( KDfloat fDuration, KDfloat sx, KDfloat sy, KDbool bFinishRecovery = KD_FALSE );
	    
		virtual CCObject*  copyWithZone ( CCZone* pZone );

		virtual KDvoid  startWithTarget ( CCNode* pNode = KD_NULL );
	    
		virtual KDvoid  stop ( KDvoid );
	    
		virtual CCActionInterval*  reverse ( KDvoid );
	    
		KDbool  getFinishRecovery ( KDvoid ) const { return m_bFinishRecovery; }
	    
	protected :

		APSCCScaleTo*	m_pReverseAction;
		KDbool			m_bFinishRecovery;
};

/**
 *	@brief APSCCScaleBy allows finishRecovery property
 */
class APSCCScaleBy : public CCScaleBy
{
	public :

		APSCCScaleBy ( KDvoid );
	    
		static  APSCCScaleBy*  actionWithDuration ( KDfloat fDuration, KDfloat sx, KDfloat sy, KDbool bFinishRecovery = KD_FALSE );
	    
		/** initializes the action with and X factor and a Y factor */
		KDbool  initWithDuration ( KDfloat fDuration, KDfloat sx, KDfloat sy, KDbool bFinishRecovery = KD_FALSE );
	    
		virtual CCObject*  copyWithZone ( CCZone* pZone );
	    
		virtual KDvoid  stop ( KDvoid );
	    
		virtual CCActionInterval*  reverse ( KDvoid );
	    
		KDbool  getFinishRecovery ( KDvoid ) const { return m_bFinishRecovery; }
	    
	protected :

		KDbool		m_bFinishRecovery;
};

/** 
 *	@brief APSCCAnimateChildren 
 */
class APSCCAnimateChildren : public CCActionInterval 
{	    
	public :

		APSCCAnimateChildren ( KDvoid );
	    
		static  APSCCAnimateChildren*  actionWithDuration ( KDfloat fDuration, KDuint frameCount, APSGraphicGroup* pTargetGraphic );
		static  APSCCAnimateChildren*  actionWithDuration ( KDfloat fDuration, KDuint frameCount, APSGraphicGroup* pTargetGraphic, KDfloat fInterval );
	    
	public :

		KDbool  initWithDuration ( KDfloat fDuration, KDuint frameCount, APSGraphicGroup* pTargetGraphic );
		KDbool  initWithDuration ( KDfloat fDuration, KDuint frameCount, APSGraphicGroup* pTargetGraphic, KDfloat fInterval );
	    
		virtual CCObject*  copyWithZone ( CCZone* pZone );
	    
		virtual CCActionInterval*  reverse ( KDvoid );

		virtual KDvoid  update ( KDfloat fTime );

	protected :

		APS_SYNTHESIZE ( APSGraphicGroup*, m_pTargetGraphic	  , TargetGraphic	 );
		APS_SYNTHESIZE_READONLY ( KDfloat, m_fInterval		  , Interval		 );
		APS_SYNTHESIZE_READONLY ( KDbool , m_bWholeRange	  , WholeRange		 );
		APS_SYNTHESIZE_READONLY ( KDbool , m_bReversed		  , Reversed		 );
		APS_SYNTHESIZE_READONLY ( KDuint , m_uOriginFrameIndex, OriginFrameIndex );
		APS_SYNTHESIZE_READONLY ( KDuint , m_uFrameCount	  , FrameCount		 );
};

/**
 *	@brief APSCCAnimate 
 *	APSCCAnimate animates sprite with fixed interval for a duration. When the animation
 *	reaches the last sprite frame, the first sprite frame is used for the next 
 *	iteration.
 */
class APSCCAnimate : public CCAnimate
{
	public :

		APSCCAnimate ( KDvoid );
		    
		static  APSCCAnimate*  actionWithDuration ( KDfloat fDuration, CCAnimation* pAnimation, KDbool bRestoreOriginalFrame );
		static  APSCCAnimate*  actionWithDuration ( KDfloat fDuration, CCAnimation* pAnimation, KDbool bRestoreOriginalFrame, KDfloat fInterval );
	    
	public :

		KDbool  initWithDuration ( KDfloat fDuration, CCAnimation* pAnimation, KDbool bRestoreOriginalFrame );
		KDbool  initWithDuration ( KDfloat fDuration, CCAnimation* pAnimation, KDbool bRestoreOriginalFrame, KDfloat fInterval );
	    
		virtual CCObject*  copyWithZone ( CCZone* pZone );

		virtual CCActionInterval*  reverse ( KDvoid );

	protected :

		APS_SYNTHESIZE_READONLY ( KDfloat, m_fInterval, Interval );
		APS_SYNTHESIZE_READONLY ( KDuint , m_uOriginFrameIndex, OriginFrameIndex );
};

NS_APS_END

#endif	// __APSCCActionInterval_h__
