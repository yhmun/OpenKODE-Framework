/* --------------------------------------------------------------------------
 *
 *      File            CCBAnimationManager.cpp
 *      Author          Y.H Mun
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
 *      version 2 of the License, or (at your otPositionion) any later version.
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

#include "Precompiled.h"
#include "extensions/CCBReader/CCBAnimationManager.h"
#include "extensions/CCBReader/CCBReader.h"
#include "extensions/CCBReader/CCBRelativePositioning.h"
#include "extensions/CCBReader/CCBSelectorResolver.h"
#include "actions/CCActionEase.h"
#include "cocoa/CCZone.h"
#include "sprite_nodes/CCSprite.h"
#include "denshion/SimpleAudioEngine.h"
#include <sstream>

NS_CC_BEGIN

//
// Implementation of CCBAinmationManager
//

CCBAnimationManager::CCBAnimationManager ( KDvoid )
{
	m_pSequences			= KD_NULL;
	m_pNodeSequences		= KD_NULL;
	m_pBaseValues			= KD_NULL;
	m_nAutoPlaySequenceId	= 0;
	m_pRootNode				= KD_NULL;
	m_tRootContainerSize	= CCSizeZero;
	m_pDelegate				= KD_NULL;
	m_pRunningSequence		= KD_NULL;
	m_bJSControlled			= KD_FALSE;
	m_pOwner				= KD_NULL;

    init ( );
}

KDbool CCBAnimationManager::init ( KDvoid )
{
    m_pSequences			 = new CCArray ( );
    m_pNodeSequences		 = new CCDictionary ( );
    m_pBaseValues			 = new CCDictionary ( );
    
	m_pDocumentOutletNames   = new CCArray ( );
    m_pDocumentOutletNodes   = new CCArray ( );
    m_pDocumentCallbackNames = new CCArray ( );
    m_pDocumentCallbackNodes = new CCArray ( );
    m_pKeyframeCallbacks	 = new CCArray ( );
    m_pKeyframeCallFuncs	 = new CCDictionary ( );
    
    m_pTarget = KD_NULL;
    m_pAnimationCompleteCallbackFunc = KD_NULL;

    return KD_TRUE;
}

CCBAnimationManager::~CCBAnimationManager ( KDvoid )
{
//    CCDictElement*  pElement = KD_NULL;
//    CCDICT_FOREACH ( m_pNodeSequences, pElement )
//    {
//        CCNode*  pNode = (CCNode*) pElement->getIntKey ( );
//        pNode->release ( );
//    }
    
//    CCDICT_FOREACH ( m_pBaseValues, pElement )
//    {
//        CCNode*  pNode = (CCNode*)pElement->getIntKey ( );
//        pNode->release ( );
//    }
    
    m_pNodeSequences->release ( );
    m_pBaseValues->release ( );
    m_pSequences->release ( );
    setRootNode ( KD_NULL );
    setDelegate ( KD_NULL );

	CC_SAFE_RELEASE ( m_pDocumentOutletNames );
    CC_SAFE_RELEASE ( m_pDocumentOutletNodes );
    CC_SAFE_RELEASE ( m_pDocumentCallbackNames );
    CC_SAFE_RELEASE ( m_pDocumentCallbackNodes );

    CC_SAFE_RELEASE ( m_pKeyframeCallFuncs );
    CC_SAFE_RELEASE ( m_pKeyframeCallbacks );
    CC_SAFE_RELEASE ( m_pTarget );
}

CCArray* CCBAnimationManager::getSequences ( KDvoid )
{
    return m_pSequences;
}

KDvoid CCBAnimationManager::setSequences ( CCArray* pSeq )
{
    m_pSequences = pSeq;
}

KDint CCBAnimationManager::getAutoPlaySequenceId ( KDvoid )
{
    return m_nAutoPlaySequenceId;
}

KDvoid CCBAnimationManager::setAutoPlaySequenceId ( KDint nAutoPlaySequenceId )
{
    m_nAutoPlaySequenceId = nAutoPlaySequenceId;
}

CCNode* CCBAnimationManager::getRootNode ( KDvoid )
{
    return m_pRootNode;
}

KDvoid CCBAnimationManager::setRootNode ( CCNode* pRootNode )
{
    m_pRootNode = pRootNode;
}

KDvoid CCBAnimationManager::setDocumentControllerName ( const std::string& sName )
{
    m_sDocumentControllerName = sName;
}

std::string CCBAnimationManager::getDocumentControllerName ( KDvoid )
{
    return m_sDocumentControllerName;
}

KDvoid CCBAnimationManager::addDocumentCallbackNode ( CCNode* pNode )
{
    m_pDocumentCallbackNodes->addObject ( pNode );
}

KDvoid CCBAnimationManager::addDocumentCallbackName ( std::string sName )
{
    CCString*  pTempName = CCString::create ( sName );
    m_pDocumentCallbackNames->addObject ( pTempName );
}

CCArray* CCBAnimationManager::getDocumentCallbackNames ( KDvoid )
{
    return m_pDocumentCallbackNames;
}

CCArray* CCBAnimationManager::getDocumentCallbackNodes ( KDvoid )
{
    return m_pDocumentCallbackNodes;
}

KDvoid CCBAnimationManager::addDocumentOutletNode ( CCNode* pNode )
{
    m_pDocumentOutletNodes->addObject ( pNode );
}

KDvoid CCBAnimationManager::addDocumentOutletName ( std::string sName )
{
    m_pDocumentOutletNames->addObject ( CCString::create ( sName ) );
}

CCArray* CCBAnimationManager::getDocumentOutletNames ( KDvoid )
{
    return m_pDocumentOutletNames;
}

CCArray* CCBAnimationManager::getDocumentOutletNodes ( KDvoid )
{
    return m_pDocumentOutletNodes;
}

std::string CCBAnimationManager::getLastCompletedSequenceName ( KDvoid )
{
    return m_sLastCompletedSequenceName;
}

CCArray* CCBAnimationManager::getKeyframeCallbacks ( KDvoid )
{
    return m_pKeyframeCallbacks;
}

const CCSize& CCBAnimationManager::getRootContainerSize ( KDvoid )
{
    return m_tRootContainerSize;
}

KDvoid CCBAnimationManager::setRootContainerSize ( const CCSize& tRootContainerSize )
{
    m_tRootContainerSize.setSize ( tRootContainerSize.cx, tRootContainerSize.cy );
}

CCBAnimationManagerDelegate* CCBAnimationManager::getDelegate ( KDvoid )
{
    return m_pDelegate;
}

KDvoid CCBAnimationManager::setDelegate ( CCBAnimationManagerDelegate* pDelegate )
{
	CCObject*  p = dynamic_cast<CCObject*> ( m_pDelegate );
    CC_SAFE_RELEASE ( p );
    m_pDelegate = pDelegate;
    CC_SAFE_RETAIN ( dynamic_cast<CCObject*> ( m_pDelegate ) );
}

const KDchar* CCBAnimationManager::getRunningSequenceName ( KDvoid )
{
    return m_pRunningSequence->getName ( );
}

const CCSize& CCBAnimationManager::getContainerSize ( CCNode* pNode )
{
	return pNode ? pNode->getContentSize ( ) : m_tRootContainerSize;
}

// refer to CCBReader::readNodeGraph() for data structure of pSeq
KDvoid CCBAnimationManager::addNode ( CCNode* pNode, CCDictionary* pSeq )
{
//    pNode->retain ( );
    
    m_pNodeSequences->setObject ( pSeq, (KDintptr) pNode );
}

KDvoid CCBAnimationManager::setBaseValue ( CCObject* pValue, CCNode* pNode, const KDchar *szPropName )
{
    CCDictionary*  pProps = (CCDictionary*) m_pBaseValues->objectForKey ( (KDintptr) pNode );
    if ( !pProps )
    {
        pProps = CCDictionary::create ( );
        m_pBaseValues->setObject ( pProps, (KDintptr) pNode );
//        pNode->retain ( );
    }
    
    pProps->setObject ( pValue, szPropName );
}

CCObject* CCBAnimationManager::getBaseValue ( CCNode* pNode, const KDchar* szPropName )
{
    CCDictionary*  pProps = (CCDictionary*) m_pBaseValues->objectForKey ( (KDintptr) pNode );
    
    return pProps->objectForKey ( szPropName );
}

KDint CCBAnimationManager::getSequenceId ( const KDchar* szSequenceName )
{
    CCObject*    pElement = KD_NULL;
	std::string  sSeqName ( szSequenceName );
    CCARRAY_FOREACH ( m_pSequences, pElement )
    {
        CCBSequence*  pSeq = (CCBSequence*) pElement;
        if ( sSeqName.compare ( pSeq->getName ( ) ) == 0 )
        {
            return pSeq->getSequenceId ( );
        }
    }

    return -1;
}

CCBSequence* CCBAnimationManager::getSequence ( KDint nSequenceId )
{
    CCObject*  pElement = KD_NULL;
    CCARRAY_FOREACH ( m_pSequences, pElement )
    {
        CCBSequence*  pSeq = (CCBSequence*) pElement;
        if ( pSeq->getSequenceId ( ) == nSequenceId )
        {
            return pSeq;
        }
    }
    return KD_NULL;
}

KDvoid CCBAnimationManager::moveAnimationsFromNode ( CCNode* pFromNode, CCNode* pToNode )
{
    // Move base values
    CCObject*  pBaseValue = m_pBaseValues->objectForKey ( (KDintptr) pFromNode );
    if ( pBaseValue )
	{
        m_pBaseValues->setObject ( pBaseValue, (KDintptr) pToNode );
        m_pBaseValues->removeObjectForKey ( (KDintptr) pFromNode );

//        pFromNode->release ( );
//        pToNode->retain ( );
    }
    
    // Move seqs
    CCObject*  pSeqs = m_pNodeSequences->objectForKey ( (KDintptr) pFromNode );
    if ( pSeqs )
	{
        m_pNodeSequences->setObject ( pSeqs, (KDintptr) pToNode );
        m_pNodeSequences->removeObjectForKey ( (KDintptr) pFromNode );

//        pFromNode->release ( );
//        pToNode->retain ( );
    }
}

// Refer to CCBReader::readKeyframe() for the real type of value
CCActionInterval* CCBAnimationManager::getAction ( CCBKeyframe* pKeyframe0, CCBKeyframe* pKeyframe1, const KDchar* szPropName, CCNode* pNode )
{
    KDfloat  fDuration = pKeyframe1->getTime ( ) - ( pKeyframe0 ? pKeyframe0->getTime ( ) : 0 );
    
    if ( kdStrcmp ( szPropName, "rotationX" ) == 0 )
    {
        CCBValue*  pValue = (CCBValue*) pKeyframe1->getValue ( );
        return CCBRotateXTo::create ( fDuration, pValue->getFloatValue ( ) );
    }
    else if ( kdStrcmp ( szPropName, "rotationY" ) == 0 )
    {
        CCBValue*  pValue = (CCBValue*) pKeyframe1->getValue ( );
        return CCBRotateYTo::create ( fDuration, pValue->getFloatValue ( ) );
    }
    else if ( kdStrcmp ( szPropName, "rotation" ) == 0 )
    {
        CCBValue*  pValue = (CCBValue*) pKeyframe1->getValue ( );
        return CCBRotateTo::create ( fDuration, pValue->getFloatValue ( ) );
    }
    else if ( kdStrcmp ( szPropName, "opacity" ) == 0 )
    {
        CCBValue*  pValue = (CCBValue*) pKeyframe1->getValue ( );
        return CCFadeTo::create ( fDuration , pValue->getByteValue ( ) );
    }
    else if ( kdStrcmp ( szPropName, "color" ) == 0 )
    {
        ccColor3BWapper*  pColor = (ccColor3BWapper*) pKeyframe1->getValue ( );
        ccColor3B         tColor = pColor->getColor();
        
        return CCTintTo::create ( fDuration, tColor.r, tColor.g, tColor.b );
    }
    else if ( kdStrcmp ( szPropName, "visible" ) == 0 )
    {
        CCBValue*  pValue = (CCBValue*) pKeyframe1->getValue ( );

		if ( pValue->getBoolValue ( ) )
		{
			return CCSequence::createWithTwoActions ( CCDelayTime::create ( fDuration ), CCShow::create ( ) );
		}
		else
		{
			return CCSequence::createWithTwoActions ( CCDelayTime::create ( fDuration ), CCHide::create ( ) );
		}	
    }
    else if ( kdStrcmp ( szPropName, "displayFrame" ) == 0 )
    {
        return CCSequence::createWithTwoActions ( CCDelayTime::create ( fDuration ), CCBSetSpriteFrame::create ( (CCSpriteFrame*) pKeyframe1->getValue ( ) ) );
    }
    else if ( kdStrcmp ( szPropName, "position" ) == 0 )
    {
        // Get position type
        CCArray*  pArray = (CCArray*) getBaseValue ( pNode, szPropName );
        KDint     nType  = ( (CCBValue*) pArray->objectAtIndex ( 2 ) )->getIntValue ( );
        
        // Get relative position
        CCArray*  pValue = (CCArray*) pKeyframe1->getValue ( );
        KDfloat   x = ( (CCBValue*) pValue->objectAtIndex ( 0 ) )->getFloatValue ( );
        KDfloat   y = ( (CCBValue*) pValue->objectAtIndex ( 1 ) )->getFloatValue ( );
        
        CCSize    tContainerSize = getContainerSize ( pNode->getParent ( ) );        
        CCPoint   tAbsPos = getAbsolutePosition ( ccp ( x, y ), nType, tContainerSize, szPropName );
        
        return CCMoveTo::create ( fDuration, tAbsPos );
    }
    else if ( kdStrcmp ( szPropName, "scale" ) == 0 )
    {
        // Get position type
        CCArray*  pArray = (CCArray*) getBaseValue ( pNode, szPropName );
        KDint     nType  = ( (CCBValue*) pArray->objectAtIndex ( 2 ) )->getIntValue ( );
        
        // Get relative scale
        CCArray*  pValue = (CCArray*) pKeyframe1->getValue ( );
        KDfloat   x = ( (CCBValue*) pValue->objectAtIndex ( 0 ) )->getFloatValue ( );
        KDfloat   y = ( (CCBValue*) pValue->objectAtIndex ( 1 ) )->getFloatValue ( );
        
        if ( nType == kCCBScaleTypeMultiplyResolution )
        {
            KDfloat  fResolutionScale = CCBReader::getResolutionScale ( );
            x *= fResolutionScale;
            y *= fResolutionScale;
        }
        
        return CCScaleTo::create ( fDuration, x, y );
    }
    else if ( kdStrcmp ( szPropName, "skew" ) == 0 ) 
    {
        // Get relative skew
        CCArray*  pValue = (CCArray*) pKeyframe1->getValue ( );
        KDfloat   x = ( (CCBValue*) pValue->objectAtIndex ( 0 ) )->getFloatValue ( );
        KDfloat   y = ( (CCBValue*) pValue->objectAtIndex ( 1 ) )->getFloatValue ( );
        
        return CCSkewTo::create ( fDuration, x, y );
    }
    else 
    {
        CCLOG ( "CCBReader: Failed to create animation for property: %s", szPropName );
    }
    
    return KD_NULL;
}

KDvoid CCBAnimationManager::setAnimatedProperty ( const KDchar* szPropName, CCNode* pNode, CCObject* pValue, KDfloat fTweenDuration )
{
    if ( fTweenDuration > 0 )
    {
        // Create a fake keyframe to generate the action from
        CCBKeyframe*  pKeyframe1 = new CCBKeyframe ( );
        pKeyframe1->autorelease ( );
        pKeyframe1->setValue ( pValue );
        pKeyframe1->setTime  ( fTweenDuration );
        pKeyframe1->setEasingType ( kCCBKeyframeEasingLinear );
        
        // Animate
        CCActionInterval*  pTweenAction = getAction ( KD_NULL, pKeyframe1, szPropName, pNode );
        pNode->runAction ( pTweenAction );
    }
    else 
    {
        // Just set the value
        
        if ( kdStrcmp ( szPropName, "position" ) == 0 )
        {
            // Get position type
            CCArray*  pArray = (CCArray*) getBaseValue ( pNode, szPropName );
            KDint     nType  = ( (CCBValue*) pArray->objectAtIndex ( 2 ) )->getIntValue ( );
            
            // Get relative position
            CCArray*  pValue2 = (CCArray*) pValue;
            KDfloat   x = ( (CCBValue*) pValue2->objectAtIndex ( 0 ) )->getFloatValue ( );
            KDfloat   y = ( (CCBValue*) pValue2->objectAtIndex ( 1 ) )->getFloatValue ( );
            
            pNode->setPosition ( getAbsolutePosition ( ccp ( x, y ), nType, getContainerSize ( pNode->getParent ( ) ), szPropName ) );
        }
        else if ( kdStrcmp ( szPropName, "scale" ) == 0 )
        {
            // Get scale type
            CCArray*  pArray = (CCArray*) getBaseValue ( pNode, szPropName );
            KDint     nType  = ( (CCBValue*) pArray->objectAtIndex ( 2 ) )->getIntValue ( );
            
            // Get relative scale
            CCArray*  pValue2 = (CCArray*) pValue;
            KDfloat   x = ( (CCBValue*) pValue2->objectAtIndex ( 0 ) )->getFloatValue ( );
            KDfloat   y = ( (CCBValue*) pValue2->objectAtIndex ( 1 ) )->getFloatValue ( );
            
            setRelativeScale ( pNode, x, y, nType, szPropName );
        }
        else if ( kdStrcmp ( szPropName, "skew" ) == 0 )
        {
            // Get relative scale
            CCArray*  pValue2 = (CCArray*) pValue;
            KDfloat   x = ( (CCBValue*) pValue2->objectAtIndex ( 0 ) )->getFloatValue ( );
            KDfloat   y = ( (CCBValue*) pValue2->objectAtIndex ( 1 ) )->getFloatValue ( );

            pNode->setSkewX ( x );
            pNode->setSkewY ( y );
        }
        else 
        {
            // [node setValue:value forKey:name];

            // TODO only handle rotation, opacity, displayFrame, color
            if ( kdStrcmp ( szPropName, "rotation" ) == 0 )
            {
                KDfloat  fRotate = ( (CCBValue*) pValue )->getFloatValue ( );
                pNode->setRotation ( fRotate );
            }
			else if ( kdStrcmp ( szPropName, "rotationX" ) == 0)
            {
                KDfloat  fRotate = ((CCBValue*) pValue )->getFloatValue();
                pNode->setRotationX ( fRotate );
            }
			else if ( kdStrcmp ( szPropName, "rotationY" ) == 0)
            {
                KDfloat  fRotate = ((CCBValue*) pValue )->getFloatValue();
                pNode->setRotationY ( fRotate );
            }
            else if ( kdStrcmp ( szPropName, "opacity" ) == 0 )
            {
                KDint    nOpacity = ((CCBValue*)pValue)->getByteValue ( );
                ( dynamic_cast<CCRGBAProtocol*> ( pNode ) )->setOpacity ( nOpacity );
            }
            else if ( kdStrcmp ( szPropName, "displayFrame" ) == 0 )
            {
                ( (CCSprite*) pNode )->setDisplayFrame ( (CCSpriteFrame*) pValue );
            }
            else if ( kdStrcmp ( szPropName, "color" ) == 0 )
            {
                ccColor3BWapper*  pColor = (ccColor3BWapper*) pValue;
				( dynamic_cast<CCRGBAProtocol*> ( pNode ) )->setColor ( pColor->getColor ( ) );   
            }
            else if ( kdStrcmp ( szPropName, "visible" ) == 0 )
            {
				KDbool bVisible = ( (CCBValue*) pValue )->getBoolValue ( );
                pNode->setVisible ( bVisible );
            }            
            else 
            {
                CCLOG ( "unsupported property name is %s", szPropName );
                CCAssert ( KD_FALSE, "unsupported property now" );
            }
        }
    }
}

KDvoid CCBAnimationManager::setFirstFrame ( CCNode* pNode, CCBSequenceProperty* pSeqProp, KDfloat fTweenDuration )
{
    CCArray*  pKeyframes = pSeqProp->getKeyframes ( );
    
    if ( pKeyframes->count ( ) == 0 )
    {
        // Use base value (no animation)
        CCObject*  pBaseValue = getBaseValue(pNode, pSeqProp->getName ( ) );
        CCAssert ( pBaseValue, "No baseValue found for property" );
        setAnimatedProperty ( pSeqProp->getName ( ), pNode, pBaseValue, fTweenDuration );
    }
    else 
    {
        // Use first keyframe
        CCBKeyframe*  pKeyframe = (CCBKeyframe*) pKeyframes->objectAtIndex ( 0 );
        setAnimatedProperty ( pSeqProp->getName ( ), pNode, pKeyframe->getValue ( ), fTweenDuration );
    }
}

CCActionInterval* CCBAnimationManager::getEaseAction ( CCActionInterval* pAction, KDint nEasingType, KDfloat fEasingOpt )
{
    if ( dynamic_cast<CCSequence*> ( pAction ) )
    {
        return pAction;
    }

	switch ( nEasingType )
	{
		case kCCBKeyframeEasingLinear		:	return pAction;
		case kCCBKeyframeEasingInstant		:	return CCBEaseInstant	 ::create ( pAction );
		case kCCBKeyframeEasingCubicIn		:	return CCEaseIn			 ::create ( pAction, fEasingOpt );    
		case kCCBKeyframeEasingCubicOut		:	return CCEaseOut		 ::create ( pAction, fEasingOpt );    
		case kCCBKeyframeEasingCubicInOut	:	return CCEaseInOut		 ::create ( pAction, fEasingOpt );    
		case kCCBKeyframeEasingBackIn		:	return CCEaseBackIn		 ::create ( pAction );    
		case kCCBKeyframeEasingBackOut		:	return CCEaseBackOut	 ::create ( pAction );    
		case kCCBKeyframeEasingBackInOut	:	return CCEaseBackInOut	 ::create ( pAction );    
		case kCCBKeyframeEasingBounceIn		:	return CCEaseBounceIn	 ::create ( pAction );    
		case kCCBKeyframeEasingBounceOut	:	return CCEaseBounceOut	 ::create ( pAction );    
		case kCCBKeyframeEasingBounceInOut	:	return CCEaseBounceInOut ::create ( pAction );    
		case kCCBKeyframeEasingElasticIn	:	return CCEaseElasticIn	 ::create ( pAction, fEasingOpt );    
		case kCCBKeyframeEasingElasticOut	:	return CCEaseElasticOut	 ::create ( pAction, fEasingOpt );    
		case kCCBKeyframeEasingElasticInOut	:	return CCEaseElasticInOut::create ( pAction, fEasingOpt );
    
		default :    

			CCLOG ("CCBReader: Unkown easing type %d", nEasingType );			
	}

	return pAction;    
}

CCObject* CCBAnimationManager::actionForCallbackChannel ( CCBSequenceProperty* pChannel )
{  
    KDfloat   fLastKeyframeTime = 0;
    
    CCArray*  pActions = CCArray::create ( );
    CCArray*  pKeyframes = pChannel->getKeyframes ( );
    KDint     nNumKeyframes = pKeyframes->count ( );

    for ( KDint i = 0; i < nNumKeyframes; ++i )
	{

        CCBKeyframe*  pKeyframe = (CCBKeyframe*) pKeyframes->objectAtIndex ( i );
        KDfloat  fTimeSinceLastKeyframe = pKeyframe->getTime ( ) - fLastKeyframeTime;
        fLastKeyframeTime = pKeyframe->getTime ( );
        if ( fTimeSinceLastKeyframe > 0 ) 
		{
            pActions->addObject ( CCDelayTime::create ( fTimeSinceLastKeyframe ) );
        }
	
        CCArray*     pKeyVal = (CCArray*) pKeyframe->getValue ( );
        std::string  sSelectorName = ( (CCString*) pKeyVal->objectAtIndex ( 0 ) )->getCString ( );
        KDint        nSelectorTarget = kdAtoi ( ( (CCString*) pKeyVal->objectAtIndex ( 1 ) )->getCString ( ) );
	
        if ( m_bJSControlled ) 
		{
            CCString*    pCallbackName = CCString::createWithFormat ( "%d:%s", nSelectorTarget, sSelectorName.c_str ( ) );
            CCCallFunc*  pCallback = (CCCallFunc*) ( m_pKeyframeCallFuncs->objectForKey ( pCallbackName->getCString ( ) ) )->copy ( )->autorelease ( );

            if ( pCallback != KD_NULL )
			{
                pActions->addObject ( pCallback );
            }
        } 
		else 
		{
            CCObject*  pTarget = KD_NULL;
            if ( nSelectorTarget == kCCBTargetTypeDocumentRoot )
			{
				pTarget = m_pRootNode;
			}
            else if ( nSelectorTarget == kCCBTargetTypeOwner )
			{
				pTarget = m_pOwner;
			}

            if ( pTarget != KD_NULL )
			{
                if ( sSelectorName.length ( ) > 0 ) 
				{
                    SEL_CallFuncN  pSelCallFunc = 0;
                    
                    CCBSelectorResolver*  pTargetAsCCBSelectorResolver = dynamic_cast<CCBSelectorResolver*> ( pTarget );

                    if ( pTargetAsCCBSelectorResolver != KD_NULL )
					{
                        pSelCallFunc = pTargetAsCCBSelectorResolver->onResolveCCBCCCallFuncSelector ( pTarget, sSelectorName.c_str ( ) );
                    }
                    if ( pSelCallFunc == 0 ) 
					{
                        CCLOG ( "Skipping selector '%s' since no CCBSelectorResolver is present.", sSelectorName.c_str ( ) );
                    }
					else 
					{
                        CCCallFuncN*  pCallback = CCCallFuncN::create ( pTarget, pSelCallFunc );
                        pActions->addObject ( pCallback );
                    }
                } 
				else 
				{
                    CCLOG ( "Unexpected empty selector." );
                }
            }
        }
    }

    if ( pActions->count ( ) < 1 )
	{
		return KD_NULL;
	}
    
    return (CCObject*) CCSequence::create ( pActions );
}

CCObject* CCBAnimationManager::actionForSoundChannel ( CCBSequenceProperty* pChannel )
{   
    KDfloat   fLastKeyframeTime = 0;
    
    CCArray*  pActions = CCArray::create ( );
    CCArray*  pKeyframes = pChannel->getKeyframes ( );
    KDint     nNumKeyframes = pKeyframes->count ( );

    for ( KDint i = 0; i < nNumKeyframes; ++i )
	{
        CCBKeyframe*  pKeyframe = (CCBKeyframe*) pKeyframes->objectAtIndex ( i );
        KDfloat  fTimeSinceLastKeyframe = pKeyframe->getTime ( ) - fLastKeyframeTime;
        fLastKeyframeTime = pKeyframe->getTime ( );
        if ( fTimeSinceLastKeyframe > 0 )
		{
            pActions->addObject ( CCDelayTime::create ( fTimeSinceLastKeyframe ) );
        }
	
		std::stringstream    ss ( std::stringstream::in | std::stringstream::out );
        CCArray*        pKeyVal = (CCArray*) pKeyframe->getValue ( );
        std::string  sSoundFile = ((CCString*) pKeyVal->objectAtIndex ( 0 ) )->getCString ( );
    
        KDfloat  fPitch, fPan, fGain;
        ss << ( (CCString*) pKeyVal->objectAtIndex ( 1 ) )->getCString ( );
        ss >> fPitch;
        ss.flush ( );
    
        ss << ( (CCString*) pKeyVal->objectAtIndex ( 2 ) )->getCString ( );
        ss >> fPan;
        ss.flush ( );
        
        ss << ( (CCString*) pKeyVal->objectAtIndex ( 3 ) )->getCString ( );
        ss >> fGain;
        ss.flush ( );
        
        pActions->addObject(CCBSoundEffect::actionWithSoundFile ( sSoundFile, fPitch, fPan, fGain ) );
    }

    if ( pActions->count ( ) < 1 )
	{
		return KD_NULL;
	}
    
    return (CCObject*) CCSequence::create ( pActions );    
}

KDvoid CCBAnimationManager::runAction ( CCNode* pNode, CCBSequenceProperty* pSeqProp, KDfloat fTweenDuration )
{
    CCArray*  pKeyframes = pSeqProp->getKeyframes ( );
    KDint     nNumKeyframes = pKeyframes->count ( );
    
    if ( nNumKeyframes > 1 )
    {
        // Make an animation!
        CCArray*  pActions = CCArray::create ( );
        
        CCBKeyframe*  pKeyframeFirst = (CCBKeyframe*) pKeyframes->objectAtIndex ( 0 );
        KDfloat  fTimeFirst = pKeyframeFirst->getTime ( ) + fTweenDuration;
        
        if ( fTimeFirst > 0 )
        {
            pActions->addObject ( CCDelayTime::create ( fTimeFirst ) );
        }
        
        for ( KDint i = 0; i < nNumKeyframes - 1; ++i )
        {
            CCBKeyframe*  pKeyframe0 = (CCBKeyframe*) pKeyframes->objectAtIndex ( i );
            CCBKeyframe*  pKeyframe1 = (CCBKeyframe*) pKeyframes->objectAtIndex ( i + 1 );
            
            CCActionInterval*  pAction = getAction ( pKeyframe0, pKeyframe1, pSeqProp->getName ( ), pNode );
            if ( pAction )
            {
                // Apply easing
                pAction = getEaseAction ( pAction, pKeyframe0->getEasingType ( ), pKeyframe1->getEasingOpt ( ) );
                
                pActions->addObject ( pAction );
            }
        }
        
        CCFiniteTimeAction*  pSeq = CCSequence::create ( pActions );
        pNode->runAction ( pSeq );
    }
}

KDvoid CCBAnimationManager::runAnimationsForSequenceIdTweenDuration ( KDint nSeqId, KDfloat fTweenDuration )
{
    CCAssert ( nSeqId != -1, "Sequence id couldn't be found" );
    
    m_pRootNode->stopAllActions ( );
    
    CCDictElement*  pElement = KD_NULL;
    CCDICT_FOREACH ( m_pNodeSequences, pElement )
    {
        CCNode*  pNode = (CCNode*) pElement->getIntKey ( );
        pNode->stopAllActions ( );
        
        // Refer to CCBReader::readKeyframe() for the real type of value
        CCDictionary*  pSeqs = (CCDictionary*) pElement->getObject ( );
        CCDictionary*  pSeqNodeProps = (CCDictionary*) pSeqs->objectForKey ( nSeqId );
        
		std::set<std::string>  aSeqNodePropNames;
        
        if ( pSeqNodeProps )
        {
            // Reset nodes that have sequence node properties, and run pActions on them
            CCDictElement*  pElement1 = KD_NULL;
            CCDICT_FOREACH ( pSeqNodeProps, pElement1 )
            {
                const KDchar*  szPropName = pElement1->getStrKey ( );
                CCBSequenceProperty*  pSeqProp = (CCBSequenceProperty*) pSeqNodeProps->objectForKey ( szPropName );
                aSeqNodePropNames.insert ( szPropName );
                
                setFirstFrame ( pNode, pSeqProp, fTweenDuration );
                runAction ( pNode, pSeqProp, fTweenDuration );
            }
        }
        
        // Reset the nodes that may have been changed by other timelines
        CCDictionary*  pNodeBaseValues = (CCDictionary*) m_pBaseValues->objectForKey ( pElement->getIntKey ( ) );
        if ( pNodeBaseValues )
        {
            CCDictElement*  pElement2 = KD_NULL;
            CCDICT_FOREACH ( pNodeBaseValues, pElement2 )
            {
                if ( aSeqNodePropNames.find ( pElement2->getStrKey ( ) ) == aSeqNodePropNames.end ( ) )
                {
                    CCObject*  pValue = pElement2->getObject ( );
                    
                    if ( pValue )
                    {
                       setAnimatedProperty ( pElement2->getStrKey(), pNode, pValue, fTweenDuration );
                    }
                }
            }
        }
    }
    
    // Make callback at end of sequence
    CCBSequence*  pSeq = getSequence ( nSeqId );
    CCAction*  pCompleteAction = CCSequence::createWithTwoActions
	(
		CCDelayTime::create ( pSeq->getDuration ( ) + fTweenDuration ),
		CCCallFunc ::create ( this, callfunc_selector ( CCBAnimationManager::sequenceCompleted ) )
	);
    m_pRootNode->runAction ( pCompleteAction );
    
    // Set the running scene
    if ( pSeq->getCallbackChannel ( ) != KD_NULL )
	{
        CCAction*  pAction = (CCAction*) actionForCallbackChannel ( pSeq->getCallbackChannel ( ) );
        if ( pAction != NULL )
		{
            m_pRootNode->runAction ( pAction );
        }
    } 

    if ( pSeq->getSoundChannel ( ) != KD_NULL )
	{
        CCAction*  pAction = (CCAction*) actionForSoundChannel ( pSeq->getSoundChannel ( ) );
        if ( pAction != NULL )
		{
            m_pRootNode->runAction ( pAction );
        }
    }

    m_pRunningSequence = getSequence ( nSeqId );
}

KDvoid CCBAnimationManager::runAnimationsForSequenceNamedTweenDuration ( const KDchar* szName, KDfloat fTweenDuration )
{
    KDint  nSeqId = getSequenceId ( szName );
    runAnimationsForSequenceIdTweenDuration ( nSeqId, fTweenDuration );
}

KDvoid CCBAnimationManager::runAnimationsForSequenceNamed ( const KDchar* szName )
{
    runAnimationsForSequenceNamedTweenDuration ( szName, 0 );
}

KDvoid CCBAnimationManager::debug ( KDvoid )
{
    
}

KDvoid CCBAnimationManager::setAnimationCompletedCallback ( CCObject* pTarget, SEL_CallFunc pCallbackFunc )
{
    if ( pTarget )
    {
        pTarget->retain ( );
    }
    
    if ( m_pTarget )
    {
        m_pTarget->release ( );
    }
    
    m_pTarget = pTarget;
    m_pAnimationCompleteCallbackFunc = pCallbackFunc;
}

KDvoid CCBAnimationManager::setCallFunc ( CCCallFunc* pCallFunc, const std::string& sCallbackNamed )
{
    m_pKeyframeCallFuncs->setObject ( (CCObject*) pCallFunc, sCallbackNamed );
}

KDvoid CCBAnimationManager::sequenceCompleted ( KDvoid )
{
    const KDchar*  szRunningSequenceName = m_pRunningSequence->getName ( );
    KDint   nNextSeqId = m_pRunningSequence->getChainedSequenceId ( );
    m_pRunningSequence = KD_NULL;

    if ( m_sLastCompletedSequenceName != szRunningSequenceName )
	{
        m_sLastCompletedSequenceName = szRunningSequenceName;
    }
    
    if ( m_pDelegate )
    {
		// There may be another runAnimation() call in this delegate method
        // which will assign mRunningSequence
        m_pDelegate->completedAnimationSequenceNamed ( szRunningSequenceName );
    }
    
    if ( m_pTarget && m_pAnimationCompleteCallbackFunc )
	{
        ( m_pTarget->*m_pAnimationCompleteCallbackFunc ) ( );
    }
    
    if ( nNextSeqId != -1 )
    {
        runAnimationsForSequenceIdTweenDuration ( nNextSeqId, 0 );
    }
}


// Custom pActions


//
// CCBSetSpriteFrame
// 

CCBSetSpriteFrame* CCBSetSpriteFrame::create ( CCSpriteFrame* pSpriteFrame )
{
    CCBSetSpriteFrame*  pRet = new CCBSetSpriteFrame ( );

    if ( pRet && pRet->initWithSpriteFrame ( pSpriteFrame ) )
	{
		pRet->autorelease ( );
	}
	else 
	{
		CC_SAFE_DELETE ( pRet );
	}
    
    return pRet;
}

KDbool CCBSetSpriteFrame::initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
    m_pSpriteFrame = pSpriteFrame;
    CC_SAFE_RETAIN ( m_pSpriteFrame );
    
    return KD_TRUE;
}

CCBSetSpriteFrame::~CCBSetSpriteFrame ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pSpriteFrame );
}

CCObject* CCBSetSpriteFrame::copyWithZone(CCZone *pZone)
{
    CCZone*         pNewZone = KD_NULL;
    CCBSetSpriteFrame*  pRet = KD_NULL;
    
    if ( pZone && pZone->m_pCopyObject )
	{
        pRet = (CCBSetSpriteFrame*) pZone->m_pCopyObject;
    }
	else 
	{
        pRet = new CCBSetSpriteFrame ( );
        pZone = pNewZone = new CCZone ( pRet );
    }
    
    pRet->initWithSpriteFrame ( m_pSpriteFrame );
    CCActionInstant::copyWithZone ( pZone );
    CC_SAFE_DELETE ( pNewZone );

    return pRet;
}

KDvoid CCBSetSpriteFrame::update ( KDfloat fTime )
{
    ( (CCSprite*) m_pTarget )->setDisplayFrame ( m_pSpriteFrame );
}

//
// CCBSoundEffect
//

CCBSoundEffect* CCBSoundEffect::actionWithSoundFile ( const std::string& sFilename, KDfloat fPitch, KDfloat fPan, KDfloat fGain )
{
	CCBSoundEffect*  pRet = new CCBSoundEffect ( );
	if ( pRet != KD_NULL && pRet->initWithSoundFile ( sFilename, fPitch, fPan, fGain ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	return pRet;
}

CCBSoundEffect::~CCBSoundEffect ( KDvoid )
{

}

KDbool CCBSoundEffect::initWithSoundFile ( const std::string& sFilename, KDfloat fPitch, KDfloat fPan, KDfloat fGain )
{
    m_sSoundFile = sFilename;
    m_fPitch	 = fPitch;
    m_fPan		 = fPan;
    m_fGain		 = fGain;
    return KD_TRUE;
}

CCObject* CCBSoundEffect::copyWithZone ( CCZone* pZone )
{
    CCZone*      pNewZone = KD_NULL;
    CCBSoundEffect*  pRet = KD_NULL;
    
    if ( pZone && pZone->m_pCopyObject )
	{
        pRet = (CCBSoundEffect*) ( pZone->m_pCopyObject );
    } 
	else 
	{
        pRet  = new CCBSoundEffect ( );
        pZone = pNewZone = new CCZone ( pRet );
    }
    
    pRet->initWithSoundFile ( m_sSoundFile, m_fPitch, m_fPan, m_fGain );
    CCActionInstant::copyWithZone ( pZone );
    CC_SAFE_DELETE ( pNewZone );
    return pRet;
}


KDvoid CCBSoundEffect::update ( KDfloat fTime )
{
    SimpleAudioEngine::sharedEngine ( )->playEffect ( m_sSoundFile.c_str ( ) );
}


//
// CCBRotateTo
//

CCBRotateTo* CCBRotateTo::create ( KDfloat fDuration, KDfloat fAngle )
{
    CCBRotateTo*  pRet = new CCBRotateTo ( );

    if ( pRet && pRet->initWithDuration ( fDuration, fAngle ) )
	{
		pRet->autorelease ( );
	}
	else 
	{
		CC_SAFE_DELETE ( pRet );
	}    
    
    return pRet;
}

KDbool CCBRotateTo::initWithDuration ( KDfloat fDuration, KDfloat fAngle )
{
    if ( CCActionInterval::initWithDuration ( fDuration ) )
    {
        m_fDstAngle = fAngle;
        
        return KD_TRUE;
    }

	return KD_FALSE;
}

CCObject* CCBRotateTo::copyWithZone(CCZone *pZone)
{
    CCZone*	  pNewZone = KD_NULL;
    CCBRotateTo*  pRet = KD_NULL;
    
    if ( pZone && pZone->m_pCopyObject )
	{
        pRet = (CCBRotateTo*) pZone->m_pCopyObject;
    }
	else 
	{
        pRet = new CCBRotateTo ( );
        pZone = pNewZone = new CCZone ( pRet );
    }
    
    pRet->initWithDuration ( m_fDuration, m_fDstAngle );
    CCActionInterval::copyWithZone ( pZone );
    CC_SAFE_DELETE ( pNewZone );

    return pRet;
}

KDvoid CCBRotateTo::startWithTarget ( CCNode* pNode )
{
    CCActionInterval::startWithTarget ( pNode );

    m_fStartAngle = m_pTarget->getRotation ( );
    m_fDiffAngle  = m_fDstAngle - m_fStartAngle;
}

KDvoid CCBRotateTo::update ( KDfloat fTime )
{
    m_pTarget->setRotation ( m_fStartAngle + m_fDiffAngle * fTime );
}


//
// CCBRotateXTO
// 

CCBRotateXTo* CCBRotateXTo::create ( KDfloat fDuration, KDfloat fAngle )
{
    CCBRotateXTo*  pRet = new CCBRotateXTo ( );
    if ( pRet )
    {
        if ( pRet->initWithDuration ( fDuration, fAngle ) )
        {
            pRet->autorelease ( );
        }
        else
        {
            CC_SAFE_DELETE ( pRet );
        }
    }
    
    return pRet;
}

KDbool CCBRotateXTo::initWithDuration ( KDfloat fDuration, KDfloat fAngle )
{
    if ( CCActionInterval::initWithDuration ( fDuration ) )
    {
        m_fDstAngle = fAngle;
        
        return KD_TRUE;
    }
    else
    {
        return KD_FALSE;
    }
}


KDvoid CCBRotateXTo::startWithTarget ( CCNode* pNode )
{
//    CCActionInterval::startWithTarget ( pNode );
    m_pOriginalTarget = pNode;
    m_pTarget	  = pNode;
    m_fElapsed	  = 0.0f;
    m_bFirstTick  = KD_TRUE;
    m_fStartAngle = m_pTarget->getRotationX ( );
    m_fDiffAngle  = m_fDstAngle - m_fStartAngle;
}

CCObject* CCBRotateXTo::copyWithZone ( CCZone* pZone )
{
    CCZone*    pNewZone = KD_NULL;
    CCBRotateXTo*  pRet = KD_NULL;
    
    if ( pZone && pZone->m_pCopyObject )
	{
        pRet = (CCBRotateXTo*) ( pZone->m_pCopyObject );
    }
	else 
	{
        pRet  = new CCBRotateXTo ( );
        pZone = pNewZone = new CCZone ( pRet );
    }
    
    pRet->initWithDuration ( m_fDuration, m_fDstAngle );
    CCActionInterval::copyWithZone ( pZone );
    CC_SAFE_DELETE ( pNewZone );
    return pRet;
}

KDvoid CCBRotateXTo::update ( KDfloat fTime )
{
    m_pTarget->setRotationX ( m_fStartAngle + ( m_fDiffAngle * fTime ) );
}


//
// CCBRotateYTO
// 

CCBRotateYTo* CCBRotateYTo::create ( KDfloat fDuration, KDfloat fAngle )
{
    CCBRotateYTo*  pRet = new CCBRotateYTo();
    if ( pRet )
    {
        if ( pRet->initWithDuration ( fDuration, fAngle ) )
        {
            pRet->autorelease ( );
        }
        else
        {
            CC_SAFE_DELETE ( pRet );
        }
    }
    
    return pRet;
}

KDbool CCBRotateYTo::initWithDuration ( KDfloat fDuration, KDfloat fAngle )
{
    if ( CCActionInterval::initWithDuration ( fDuration ) )
    {
        m_fDstAngle = fAngle;        
        return KD_TRUE;
    }
    else
    {
        return KD_FALSE;
    }
}

KDvoid CCBRotateYTo::startWithTarget ( CCNode* pNode )
{
 //   CCActionInterval::startWithTarget ( pNode );
    m_pOriginalTarget	= pNode;
    m_pTarget			= pNode;
    m_fElapsed			= 0.0f;
    m_bFirstTick		= KD_TRUE;
    m_fStartAngle = m_pTarget->getRotationY ( );
    m_fDiffAngle  = m_fDstAngle - m_fStartAngle;
}


CCObject* CCBRotateYTo::copyWithZone ( CCZone* pZone )
{
    CCZone*    pNewZone = KD_NULL;
    CCBRotateYTo*  pRet = KD_NULL;
    
    if ( pZone && pZone->m_pCopyObject )
	{
        pRet = (CCBRotateYTo*) ( pZone->m_pCopyObject );
    } 
	else 
	{
        pRet = new CCBRotateYTo ( );
        pZone = pNewZone = new CCZone ( pRet );
    }
    
    pRet->initWithDuration ( m_fDuration, m_fDstAngle );
    CCActionInterval::copyWithZone ( pZone );
    CC_SAFE_DELETE ( pNewZone );
    return pRet;
}

KDvoid CCBRotateYTo::update ( KDfloat fTime )
{
    m_pTarget->setRotationY ( m_fStartAngle + ( m_fDiffAngle * fTime ) );
}


//
// CCBEaseInstant
// 

CCBEaseInstant* CCBEaseInstant::create ( CCActionInterval* pAction )
{
    CCBEaseInstant*  pRet = new CCBEaseInstant ( );
    if ( pRet && pRet->initWithAction ( pAction ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

KDvoid CCBEaseInstant::update ( KDfloat fDelta )
{
    if ( fDelta < 0 )
    {
        m_pInner->update ( 0 );
    }
    else
    {
        m_pInner->update ( 1 );
    }
}

NS_CC_END
