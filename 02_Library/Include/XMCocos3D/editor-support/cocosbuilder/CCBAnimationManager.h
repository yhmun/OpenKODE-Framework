/* -----------------------------------------------------------------------------------
 *
 *      File            CCBAnimationManager.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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
 * --------------------------------------------------------------------------------- */

#ifndef __CCBAnimationManager_h__
#define __CCBAnimationManager_h__

#include "../../extensions/ExtensionMacros.h"
#include "CCBSequence.h"
#include "CCBValue.h"
#include "CCBSequenceProperty.h"
#include "../../2d/actions/CCActionInterval.h"
#include "../../2d/actions/CCActionInstant.h"
#include "../../2d/actions/CCActionEase.h"
#include "../../extensions/GUI/CCControlExtension/CCControl.h"

namespace cocosbuilder {

class CCBAnimationManagerDelegate
{
	public :

		virtual ~CCBAnimationManagerDelegate ( KDvoid ) { }

		virtual KDvoid					completedAnimationSequenceNamed ( const KDchar* pName ) = 0;
};

class CCBAnimationManager : public cocos2d::Object
{
	public :

		KDbool							m_bJsControlled;
		
		/**
		 *	@js ctor
		 */
		CCBAnimationManager ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~CCBAnimationManager ( KDvoid );

		cocos2d::Object*				m_pOwner;
    
		virtual KDbool					init ( KDvoid );
    
		cocos2d::Array*					getSequences ( KDvoid );
		KDvoid							setSequences ( cocos2d::Array* pSeq );
    
		KDint							getAutoPlaySequenceId ( KDvoid );
		KDvoid							setAutoPlaySequenceId ( KDint nAutoPlaySequenceId );
    
		cocos2d::Node*					getRootNode ( KDvoid );
		KDvoid							setRootNode ( cocos2d::Node* pRootNode );		// weak reference    
    
		KDvoid							addDocumentCallbackNode ( cocos2d::Node* pNode );
		KDvoid							addDocumentCallbackName ( std::string sName );
		KDvoid							addDocumentCallbackControlEvents ( cocos2d::extension::Control::EventType eEventType );
    
		KDvoid							addDocumentOutletNode ( cocos2d::Node* pNode );
		KDvoid							addDocumentOutletName ( std::string sName );

		KDvoid							setDocumentControllerName ( const std::string& sName );
    
		std::string						getDocumentControllerName ( KDvoid );
		cocos2d::Array*					getDocumentCallbackNames ( KDvoid );
		cocos2d::Array*					getDocumentCallbackNodes ( KDvoid );
		cocos2d::Array*					getDocumentCallbackControlEvents ( KDvoid );
    
		cocos2d::Array*					getDocumentOutletNames ( KDvoid );
		cocos2d::Array*					getDocumentOutletNodes ( KDvoid );
		std::string						getLastCompletedSequenceName ( KDvoid );
    
		cocos2d::Array*					getKeyframeCallbacks ( KDvoid );
    
		const cocos2d::Size&			getRootContainerSize ( KDvoid );
		KDvoid							setRootContainerSize ( const cocos2d::Size& tRootContainerSize );
    
		CCBAnimationManagerDelegate*	getDelegate ( KDvoid );
		KDvoid							setDelegate ( CCBAnimationManagerDelegate* pDelegate );		// retain
    
		const KDchar*					getRunningSequenceName ( KDvoid );
    
		const cocos2d::Size&			getContainerSize ( cocos2d::Node* pNode );
    
		KDvoid							addNode ( cocos2d::Node* pNode, cocos2d::Dictionary* pSeq );
		KDvoid							setBaseValue ( cocos2d::Object* pValue, cocos2d::Node* pNode, const KDchar* pPropName );
		KDvoid							moveAnimationsFromNode ( cocos2d::Node* pFromNode, cocos2d::Node* pToNode );

		KDvoid							runAnimationsForSequenceNamedTweenDuration ( const KDchar* pName, KDfloat fTweenDuration );
		KDvoid							runAnimationsForSequenceNamed ( const KDchar* pName );
		KDvoid							runAnimationsForSequenceIdTweenDuration ( KDint nSeqId, KDfloat fTweenDuraiton );
		
		/**
		 *	when this function bound to js ,the second param are callfunc_selector
		 *	@lua NA
		 */
		KDvoid							setAnimationCompletedCallback ( cocos2d::Object* pTarget, cocos2d::SEL_CallFunc pCallbackFunc );

		KDvoid							debug ( KDvoid );
		
		/**
		 *	@js setCallFuncForJSCallbackNamed
		 */
		KDvoid							setCallFunc ( cocos2d::CallFunc* pCallFunc, const std::string& sCallbackNamed );

		cocos2d::Object*				actionForCallbackChannel ( CCBSequenceProperty* pChannel );
		cocos2d::Object*				actionForSoundChannel ( CCBSequenceProperty* pChannel );

		// return -1 if timeline not exsit
		KDint							getSequenceId ( const KDchar* pSequenceName );
    
		// get timeline duration
		KDfloat							getSequenceDuration ( const KDchar* pSequenceName );
    
	private :

		cocos2d::Object*				getBaseValue ( cocos2d::Node* pNode, const KDchar* pPropName );
		CCBSequence*					getSequence ( KDint nSequenceId );
		cocos2d::ActionInterval*		getAction ( CCBKeyframe* pKeyframe0, CCBKeyframe* pKeyframe1, const KDchar* pPropName, cocos2d::Node* pNode );
		KDvoid							setAnimatedProperty ( const KDchar* pPropName, cocos2d::Node* pNode, Object* pValue, KDfloat fTweenDuraion );
		KDvoid							setFirstFrame ( cocos2d::Node* pNode, CCBSequenceProperty* pSeqProp, KDfloat fTweenDuration );
		cocos2d::ActionInterval*		getEaseAction ( cocos2d::ActionInterval* pAction, CCBKeyframe::EasingType eEasingType, KDfloat fEasingOpt );
		KDvoid							runAction ( cocos2d::Node* pNode, CCBSequenceProperty* pSeqProp, KDfloat fTweenDuration );
		KDvoid							sequenceCompleted ( KDvoid );
    
	private :

		cocos2d::Array*					m_pSequences;
		cocos2d::Dictionary*			m_pNodeSequences;
		cocos2d::Dictionary*			m_pBaseValues;
		KDint							m_nAutoPlaySequenceId;
    
		cocos2d::Node*					m_pRootNode;
    
		cocos2d::Size					m_tRootContainerSize;
    
		CCBAnimationManagerDelegate*	m_pDelegate;
		CCBSequence*					m_pRunningSequence;
    
		cocos2d::Array*					m_pDocumentOutletNames;
		cocos2d::Array*					m_pDocumentOutletNodes;
		cocos2d::Array*					m_pDocumentCallbackNames;
		cocos2d::Array*					m_pDocumentCallbackNodes;
		cocos2d::Array*					m_pDocumentCallbackControlEvents;
		cocos2d::Array*					m_pKeyframeCallbacks;
		cocos2d::Dictionary*			m_pKeyframeCallFuncs;
    
		std::string						m_sDocumentControllerName;
		std::string						m_sLastCompletedSequenceName;
    
		cocos2d::SEL_CallFunc			m_pAnimationCompleteCallbackFunc;
		cocos2d::Object*				m_pTarget;
};

class CCBSetSpriteFrame : public cocos2d::ActionInstant
{
	public :

		/** creates a Place action with a position */
		static CCBSetSpriteFrame*		create ( cocos2d::SpriteFrame* pSpriteFrame );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~CCBSetSpriteFrame ( KDvoid );
    
		KDbool							initWithSpriteFrame ( cocos2d::SpriteFrame* pSpriteFrame );

		// Overrides
		virtual KDvoid					update ( KDfloat fTime ) override;
		virtual CCBSetSpriteFrame*		clone ( KDvoid ) const override;
		virtual CCBSetSpriteFrame*		reverse ( KDvoid ) const override;

	private :

		cocos2d::SpriteFrame*			m_pSpriteFrame;
};


class CCBSoundEffect : public cocos2d::ActionInstant
{
	public :

		static CCBSoundEffect*			actionWithSoundFile ( const std::string& sFile, KDfloat fPitch, KDfloat fPan, KDfloat fGain );

		/**
		 *	@js NA
		 *	 @lua NA
		 */
		~CCBSoundEffect ( KDvoid );

		KDbool							initWithSoundFile ( const std::string& sFile, KDfloat fPitch, KDfloat fPan, KDfloat fGain );

		// Overrides
		virtual KDvoid					update ( KDfloat fTime ) override;
		virtual CCBSoundEffect*			clone ( ) const override;
		virtual CCBSoundEffect*			reverse ( KDvoid ) const override;

	private :

		std::string						m_sSoundFile;
		KDfloat							m_fPitch;
		KDfloat							m_fPan;
		KDfloat							m_fGain;
};


class CCBRotateTo : public cocos2d::ActionInterval
{
	public :

		static CCBRotateTo*				create ( KDfloat fDuration, KDfloat fAngle );

		KDbool							initWithDuration ( KDfloat fDuration, KDfloat fAngle );

		// Override
		virtual KDvoid					update ( KDfloat fTime ) override;
		virtual CCBRotateTo*			clone ( KDvoid ) const override;
		virtual CCBRotateTo*			reverse ( KDvoid ) const override;
		virtual KDvoid					startWithTarget ( cocos2d::Node* pNode ) override;

	private :

		KDfloat							m_fStartAngle;
		KDfloat							m_fDstAngle;
		KDfloat							m_fDiffAngle;
};

class CCBRotateXTo: public cocos2d::ActionInterval
{
	public :

		static CCBRotateXTo*			create ( KDfloat fDuration, KDfloat fAngle );

		KDbool							initWithDuration ( KDfloat fDuration, KDfloat fAngle );

		// Overrides
		virtual KDvoid					startWithTarget ( cocos2d::Node* pNode ) override;
		virtual CCBRotateXTo*			clone ( KDvoid ) const override;
		virtual CCBRotateXTo*			reverse ( KDvoid ) const override;
		virtual KDvoid					update ( KDfloat fTime ) override;

	private :

		KDfloat							m_fStartAngle;
		KDfloat							m_fDstAngle;
		KDfloat							m_fDiffAngle;
};

class CCBRotateYTo : public cocos2d::ActionInterval
{
	public :

		static CCBRotateYTo*			create ( KDfloat fDuration, KDfloat fAngle );

		KDbool							initWithDuration ( KDfloat fDuration, KDfloat fAngle );

		// Override
		virtual KDvoid					startWithTarget ( cocos2d::Node* pNode ) override;
		virtual CCBRotateYTo*			clone ( KDvoid ) const override;
		virtual CCBRotateYTo*			reverse ( KDvoid ) const override;
		virtual KDvoid					update ( KDfloat fTime ) override;

	private :

		KDfloat							m_fStartAngle;
		KDfloat							m_fDstAngle;
		KDfloat							m_fDiffAngle;
};


class CCBEaseInstant : public cocos2d::ActionEase
{
	public :

		static CCBEaseInstant*			create ( cocos2d::ActionInterval* pAction );

		virtual CCBEaseInstant*			clone ( KDvoid ) const override;
		virtual CCBEaseInstant*			reverse ( KDvoid ) const override;
		virtual KDvoid					update ( KDfloat dt ) override;
};


}

#endif	// __CCBAnimationManager_h__
