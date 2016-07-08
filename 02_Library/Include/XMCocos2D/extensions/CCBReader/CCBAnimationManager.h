/* --------------------------------------------------------------------------
 *
 *      File            CCBAnimationManager.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org   
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

#ifndef __CCBAnimationManager_h__
#define __CCBAnimationManager_h__

#include "CCBSequence.h"
#include "CCBValue.h"
#include "CCBSequenceProperty.h"
#include "../../actions/CCActionInstant.h"
#include "../../actions/CCActionInterval.h"
#include "../../actions/CCActionEase.h"

NS_CC_BEGIN

class CCBAnimationManagerDelegate
{
	public :

		virtual KDvoid		completedAnimationSequenceNamed ( const KDchar* szName ) = 0;
};

class CCBAnimationManager : public CCObject
{	    
	public :
				 CCBAnimationManager ( KDvoid );
		virtual ~CCBAnimationManager ( KDvoid );
	    
	public :

		virtual KDbool		init ( KDvoid );
	    
		CCArray*			getSequences ( KDvoid );
		KDvoid				setSequences ( CCArray* pSeq );
	    
		KDint				getAutoPlaySequenceId ( KDvoid );
		KDvoid				setAutoPlaySequenceId ( KDint nAutoPlaySequenceId );
	    
		CCNode*				getRootNode ( KDvoid );
		KDvoid				setRootNode ( CCNode* pRootNode );										// weak reference   
	    
		KDvoid				addDocumentCallbackNode	( CCNode* pNode );
		KDvoid				addDocumentCallbackName	( std::string sName );
		KDvoid				addDocumentOutletNode	( CCNode* pNode );
		KDvoid				addDocumentOutletName	( std::string sName );

		KDvoid				setDocumentControllerName ( const std::string& sName );
	    
		std::string			getDocumentControllerName    ( KDvoid );
		CCArray*			getDocumentCallbackNames     ( KDvoid );
		CCArray*			getDocumentCallbackNodes     ( KDvoid );
		CCArray*			getDocumentOutletNames	     ( KDvoid );
		CCArray*			getDocumentOutletNodes		 ( KDvoid );
		std::string			getLastCompletedSequenceName ( KDvoid );

		CCArray*			getKeyframeCallbacks ( KDvoid );

		const CCSize&		getRootContainerSize ( KDvoid );
		KDvoid				setRootContainerSize ( const CCSize& tRootContainerSize );
	    
		CCBAnimationManagerDelegate*	getDelegate ( KDvoid );
		KDvoid							setDelegate ( CCBAnimationManagerDelegate* pDelegate );		// retain
	    
		const KDchar*		getRunningSequenceName ( KDvoid );
	    
		const CCSize&		getContainerSize ( CCNode* pNode );
	    
		KDvoid				addNode ( CCNode* pNode, CCDictionary* pSeq );
		KDvoid				setBaseValue ( CCObject* pValue, CCNode* pNode, const KDchar* szPropName );
	    
		KDvoid				moveAnimationsFromNode(CCNode* fromNode, CCNode* toNode);

		KDvoid				runAnimationsForSequenceNamedTweenDuration ( const KDchar* szName, KDfloat fTweenDuration );
		KDvoid				runAnimationsForSequenceNamed ( const KDchar* szName );
		KDvoid				runAnimationsForSequenceIdTweenDuration ( KDint nSeqId, KDfloat fTweenDuraiton );

		KDvoid				setAnimationCompletedCallback ( CCObject* pTarget, SEL_CallFunc pCallbackFunc );
	    
		KDvoid				debug ( KDvoid );
	    
		KDvoid				setCallFunc ( CCCallFunc* pCallFunc, const std::string& sCallbackNamed );

		CCObject*			actionForCallbackChannel ( CCBSequenceProperty* pChannel );
		CCObject*			actionForSoundChannel	 ( CCBSequenceProperty* pChannel );

	private :

		CCObject*			getBaseValue ( CCNode* pNode, const KDchar* szPropName );
		KDint				getSequenceId ( const KDchar* pSequenceName );
		CCBSequence*		getSequence ( KDint nSequenceId );
		CCActionInterval*	getAction ( CCBKeyframe* pKeyframe0, CCBKeyframe* pKeyframe1, const KDchar* szPropName, CCNode* pNode );

		KDvoid				setAnimatedProperty ( const KDchar* szPropName, CCNode* pNode, CCObject* pValue, KDfloat fTweenDuration );
		KDvoid				setFirstFrame ( CCNode* pNode, CCBSequenceProperty* pSeqProp, KDfloat fTweenDuration );
		
		CCActionInterval*	getEaseAction ( CCActionInterval* pAction, KDint nEasingType, KDfloat fEasingOpt );
		
		KDvoid				runAction ( CCNode* pNode, CCBSequenceProperty* pSeqProp, KDfloat fTweenDuration );

		KDvoid				sequenceCompleted ( KDvoid );

	public :
		
		KDbool							m_bJSControlled;
		CCObject*						m_pOwner;

	private :

		CCArray*						m_pSequences;
		CCDictionary*					m_pNodeSequences;
		CCDictionary*					m_pBaseValues;
		KDint							m_nAutoPlaySequenceId;
	    
		CCNode*							m_pRootNode;
		CCSize							m_tRootContainerSize;
	    
		CCBAnimationManagerDelegate*	m_pDelegate;
		CCBSequence*					m_pRunningSequence;

		CCArray*						m_pDocumentOutletNames;
		CCArray*						m_pDocumentOutletNodes;
		CCArray*						m_pDocumentCallbackNames;
		CCArray*						m_pDocumentCallbackNodes;
		CCArray*						m_pKeyframeCallbacks;
		CCDictionary*					m_pKeyframeCallFuncs;
		std::string						m_sDocumentControllerName;
		std::string						m_sLastCompletedSequenceName;

		SEL_CallFunc					m_pAnimationCompleteCallbackFunc;
		CCObject*						m_pTarget;
};

class CCBSetSpriteFrame : public CCActionInstant
{    
	public :

		virtual ~CCBSetSpriteFrame ( KDvoid );
	    
		/** creates a Place action with a position */
		static   CCBSetSpriteFrame*		create ( CCSpriteFrame* pSpriteFrame );

	public :

		virtual KDbool			initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame );
		
		virtual KDvoid			update ( KDfloat fTime );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

	private :

		CCSpriteFrame*			m_pSpriteFrame;
};

class CCBSoundEffect : public CCActionInstant
{    
	public :

		~CCBSoundEffect ( KDvoid );
	    
		static CCBSoundEffect* actionWithSoundFile ( const std::string& sFile, KDfloat fPitch, KDfloat fPan, KDfloat fGain );

	public :

		virtual KDbool			initWithSoundFile ( const std::string& sFile, KDfloat fPitch, KDfloat fPan, KDfloat fGain );
		
		virtual KDvoid			update ( KDfloat fTime );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

	private :

		std::string				m_sSoundFile;
		KDfloat					m_fPitch;
		KDfloat					m_fPan;
		KDfloat					m_fGain;
};

class CCBRotateTo : public CCActionInterval
{
	public :

		static CCBRotateTo*		create ( KDfloat fDuration, KDfloat fAngle );

	public :

		virtual KDbool			initWithDuration ( KDfloat fDuration, KDfloat fAngle );

		virtual KDvoid			update ( KDfloat fTime );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			startWithTarget ( CCNode* pNode );

	private :

		KDfloat					m_fStartAngle;
		KDfloat					m_fDstAngle;
		KDfloat					m_fDiffAngle;    
};

class CCBRotateXTo: public CCActionInterval
{
	public :
		
		static CCBRotateXTo*	create ( KDfloat fDuration, KDfloat fAngle );

	public :

		virtual KDbool			initWithDuration ( KDfloat fDuration, KDfloat fAngle );

		virtual KDvoid			startWithTarget ( CCNode* pNode );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			update ( KDfloat fTime );

	private :

		KDfloat					m_fStartAngle;
		KDfloat					m_fDstAngle;
		KDfloat					m_fDiffAngle;
};


class CCBRotateYTo: public CCActionInterval 
{	    
	public :

		static CCBRotateYTo*	create ( KDfloat fDuration, KDfloat fAngle );
		
	public :

		virtual KDbool			initWithDuration ( KDfloat fDuration, KDfloat fAngle );
		
		virtual KDvoid			startWithTarget ( CCNode* pNode );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			update ( KDfloat fTime );

	private :

		KDfloat					m_fStartAngle;
		KDfloat					m_fDstAngle;
		KDfloat					m_fDiffAngle;
};


class CCBEaseInstant : public CCActionEase
{
	public :

		static CCBEaseInstant*	create ( CCActionInterval* pAction );
	    
	public :

		virtual KDvoid			update ( KDfloat fDelta );
};

NS_CC_END

#endif	// __CCBAnimationManager_h__
