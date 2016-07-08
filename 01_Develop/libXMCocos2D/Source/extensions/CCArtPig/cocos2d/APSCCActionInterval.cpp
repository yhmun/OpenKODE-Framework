/* --------------------------------------------------------------------------
 *
 *      File            APSCCActionInterval.cpp
 *      Author          Y.H Mun
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

#include "Precompiled.h"
#include "extensions/CCArtPig/APSCCActionInterval.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "cocoa/CCZone.h"
#include "support/CCPointExtension.h"

NS_APS_BEGIN

//
// APSCCFadeChildrenTo
//
APSCCFadeChildrenTo::APSCCFadeChildrenTo ( KDvoid ) 
{
	m_pReverseAction = KD_NULL;
	m_pTargetGraphic = KD_NULL;
	m_cFromOpacity   = APS_MAX_OPACITY;
	m_cToOpacity	 = APS_MAX_OPACITY;
}

APSCCFadeChildrenTo::~APSCCFadeChildrenTo ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pReverseAction );
}

APSCCFadeChildrenTo* APSCCFadeChildrenTo::actionWithDuration ( KDfloat fDuration, GLubyte cOpacity, APSGraphic* pTargetGraphic )
{
	APSCCFadeChildrenTo*  pAction = new APSCCFadeChildrenTo ( );

	pAction->initWithDuration ( fDuration, cOpacity, pTargetGraphic );
	pAction->autorelease ( );
    
    return pAction;
}

KDbool APSCCFadeChildrenTo::initWithDuration ( KDfloat fDuration, GLubyte cOpacity, APSGraphic* pTargetGraphic )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_pTargetGraphic = pTargetGraphic;
        m_cToOpacity	 = cOpacity;
		return KD_TRUE;
	}
    
	return KD_FALSE;
}

CCObject* APSCCFadeChildrenTo::copyWithZone ( CCZone* pZone )
{
	CCZone*            pNewZone = KD_NULL;
	APSCCFadeChildrenTo*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCFadeChildrenTo*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCFadeChildrenTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCActionInterval::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( m_fDuration, m_cToOpacity, m_pTargetGraphic );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCFadeChildrenTo::startWithTarget ( CCNode* pNode )
{
    // node parameter is ignored.
    CCNode*  pTarget = m_pTargetGraphic->getNode ( );
	CCActionInterval::startWithTarget ( pTarget );
    
    m_cFromOpacity = m_pTargetGraphic->getRunningOpacity ( );
    
    // Before progress this action, fromOpacity is assigned to toOpacity of reverseAction
    if ( this->m_pReverseAction )
	{
        this->m_pReverseAction->setToOpacity ( m_cFromOpacity );
    }
}

KDvoid APSCCFadeChildrenTo::update ( KDfloat fTime )
{
    m_pTargetGraphic->setRunningOpacity ( (GLubyte) ( m_cFromOpacity + ( m_cToOpacity - m_cFromOpacity ) * fTime ) );
    m_pTargetGraphic->updateAccOpacity ( );
}

CCActionInterval* APSCCFadeChildrenTo::reverse ( KDvoid )
{
    if ( !m_pReverseAction ) 
	{
        this->m_pReverseAction = new APSCCFadeChildrenTo;
        this->m_pReverseAction->initWithDuration ( this->getDuration ( ), m_cFromOpacity, this->getTargetGraphic ( ) );
    }
    return this->m_pReverseAction;
}

//
//  APSCCFadeTo
//
APSCCFadeTo::APSCCFadeTo ( KDvoid ) 
{
	m_pReverseAction  = KD_NULL;
	m_bFinishRecovery = KD_FALSE;
}

APSCCFadeTo::~APSCCFadeTo ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pReverseAction );
}

APSCCFadeTo* APSCCFadeTo::actionWithDuration ( KDfloat fDuration, GLubyte cOpacity, KDbool bFinishRecovery ) 
{
    APSCCFadeTo*  pAction = new APSCCFadeTo ( );
    
    pAction->initWithDuration ( fDuration, cOpacity, bFinishRecovery );
    pAction->autorelease ( );
    
    return pAction;
}

// initializes the action with duration, cOpacity, and finishRecovery 
KDbool APSCCFadeTo::initWithDuration ( KDfloat fDuration, GLubyte cOpacity, KDbool bFinishRecovery ) 
{
    if ( CCFadeTo::initWithDuration ( fDuration, cOpacity ) ) 
	{
        m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    } 

	return KD_FALSE;    
}

CCObject* APSCCFadeTo::copyWithZone ( CCZone* pZone ) 
{
	CCZone*    pNewZone = KD_NULL;
	APSCCFadeTo*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCFadeTo*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCFadeTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCFadeTo::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( this->getDuration ( ), m_cToOpacity, this->getFinishRecovery ( ) );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCFadeTo::startWithTarget ( CCNode* pNode )
{
    CCFadeTo::startWithTarget ( pNode );
    
    CCRGBAProtocol*  pRgbaProtocol = dynamic_cast<CCRGBAProtocol*> ( pNode );
    
    // Before progress this action, set reverse action's toOpacity.
    if ( pRgbaProtocol && this->m_pReverseAction ) 
	{
        this->m_pReverseAction->m_cToOpacity = pRgbaProtocol->getOpacity ( );
    }    
}

KDvoid APSCCFadeTo::stop ( KDvoid )
{
    if  ( m_bFinishRecovery ) 
	{
        ( (CCRGBAProtocol*) this->getTarget ( ) )->setOpacity ( m_cFromOpacity );
    }
    CCFadeTo::stop ( );
}

CCActionInterval* APSCCFadeTo::reverse ( KDvoid ) 
{
    if ( !m_pReverseAction )
	{
        this->m_pReverseAction = new APSCCFadeTo;
        this->m_pReverseAction->initWithDuration ( this->getDuration ( ), 0, m_bFinishRecovery );
    }

    return this->m_pReverseAction;
}

//
//  APSCCMoveTo
//
APSCCMoveTo::APSCCMoveTo ( KDvoid ) 
{
	m_pReverseAction  = KD_NULL;
	m_bFinishRecovery = KD_FALSE;
}

APSCCMoveTo::~APSCCMoveTo ( KDvoid ) 
{
    CC_SAFE_RELEASE ( m_pReverseAction );
}

APSCCMoveTo*  APSCCMoveTo::actionWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDbool bFinishRecovery )
{
    APSCCMoveTo*  pAction = new APSCCMoveTo ( );
    
    pAction->initWithDuration ( fDuration, tPosition, bFinishRecovery );
    pAction->autorelease ( );
    
    return pAction;
}

// initializes the action with duration, tPosition, and finishRecovery 
KDbool APSCCMoveTo::initWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDbool bFinishRecovery ) 
{
    if (CCMoveTo::initWithDuration ( fDuration, tPosition ) ) 
	{
        m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    } 

	return KD_FALSE;   
}

CCObject* APSCCMoveTo::copyWithZone ( CCZone* pZone )
{
	CCZone*    pNewZone = KD_NULL;
	APSCCMoveTo*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCMoveTo*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCMoveTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCMoveTo::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( this->getDuration ( ), m_tEndPosition, this->getFinishRecovery ( ) );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCMoveTo::startWithTarget ( CCNode* pNode )
{
    CCMoveTo::startWithTarget ( pNode );
    
    // Before progress this action, set reverse action's endPosition.
    if ( this->m_pReverseAction ) 
	{
        ( (APSCCMoveTo*) this->m_pReverseAction )->m_tEndPosition = pNode->getPosition ( );
    }
}

KDvoid APSCCMoveTo::stop ( KDvoid )
{
    if  ( m_bFinishRecovery )
	{
        this->getTarget ( )->setPosition ( m_tStartPosition );
    }
    CCMoveTo::stop ( );
}

CCActionInterval* APSCCMoveTo::reverse ( KDvoid )
{
    if ( !m_pReverseAction )
	{
        this->m_pReverseAction = new APSCCMoveTo;
        ( (APSCCMoveTo*) this->m_pReverseAction )->initWithDuration ( this->getDuration ( ), CCPointZero, this->getFinishRecovery ( ) );
    }
    return this->m_pReverseAction;
}

APSCCMoveBy::APSCCMoveBy ( KDvoid ) 
{
	m_bFinishRecovery = KD_FALSE;    
}

APSCCMoveBy* APSCCMoveBy::actionWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDbool bFinishRecovery )
{
    APSCCMoveBy*  pAction = new APSCCMoveBy ( );
    
    pAction->initWithDuration ( fDuration, tPosition, bFinishRecovery );
    pAction->autorelease ( );
    
    return pAction;
}

// initializes the action 
KDbool APSCCMoveBy::initWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDbool bFinishRecovery ) 
{
    if (CCMoveBy::initWithDuration ( fDuration, tPosition ) ) 
	{
        m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    } 

	return KD_FALSE;    
}

CCObject* APSCCMoveBy::copyWithZone ( CCZone* pZone ) 
{
	CCZone*    pNewZone = KD_NULL;
	APSCCMoveBy*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCMoveBy*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCMoveBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCMoveBy::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( this->getDuration ( ), m_tPositionDelta, this->getFinishRecovery ( ) );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

CCActionInterval* APSCCMoveBy::reverse ( KDvoid )
{
    return APSCCMoveBy::actionWithDuration ( m_fDuration, ccp ( -m_tPositionDelta.x, -m_tPositionDelta.y ), this->getFinishRecovery ( ) );
}

KDvoid APSCCMoveBy::stop ( KDvoid )
{
    if  ( m_bFinishRecovery ) 
	{
        this->getTarget ( )->setPosition ( m_tStartPosition );
    }
    CCMoveBy::stop ( );
}

//
//  APSCCBezierTo
//
APSCCBezierTo::APSCCBezierTo ( KDvoid ) 
{
	m_pReverseAction  = KD_NULL;
	m_bFinishRecovery = KD_FALSE;
}

APSCCBezierTo::~APSCCBezierTo ( KDvoid ) 
{
    CC_SAFE_RELEASE ( m_pReverseAction );
}

APSCCBezierTo* APSCCBezierTo::actionWithDuration ( KDfloat fDuration, const ccBezierConfig& tConfig, KDbool bFinishRecovery ) 
{
    APSCCBezierTo*  pAction = new APSCCBezierTo ( );
    
    pAction->initWithDuration ( fDuration, tConfig, bFinishRecovery );
    pAction->autorelease ( );
    
    return pAction;
}

// initializes the action with duration, bezier config, and finishRecovery 
KDbool APSCCBezierTo::initWithDuration ( KDfloat fDuration, const ccBezierConfig& tConfig, KDbool bFinishRecovery )
{
    if ( CCBezierTo::initWithDuration ( fDuration, tConfig ) )
	{
        m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    }

	return KD_FALSE;        
}

CCObject* APSCCBezierTo::copyWithZone ( CCZone* pZone )
{
	CCZone*      pNewZone = KD_NULL;
	APSCCBezierTo*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCBezierTo*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCBezierTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCBezierTo::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( this->getDuration ( ), m_tBezierConfig, this->getFinishRecovery ( ) );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCBezierTo::startWithTarget ( CCNode* pNode ) 
{
    CCBezierTo::startWithTarget ( pNode );
    
    // Before progress this action, set reverse action's endPosition.
    if ( this->m_pReverseAction ) 
	{
        ( (APSCCBezierTo*) this->m_pReverseAction )->m_tBezierConfig.tEndPosition = pNode->getPosition ( );
    }    
}

KDvoid APSCCBezierTo::stop ( KDvoid )
{
    if  ( m_bFinishRecovery ) 
	{
        this->getTarget ( )->setPosition(this->m_tStartPosition);
    }
    CCBezierTo::stop ( );
}

CCActionInterval* APSCCBezierTo::reverse ( KDvoid )
{
    if ( !m_pReverseAction )
	{
        this->m_pReverseAction = new APSCCBezierTo;
        ccBezierConfig  tConfig = { this->m_tBezierConfig.tControlPoint2, this->m_tBezierConfig.tControlPoint1, ccp ( 0.f, 0.f) };
        this->m_pReverseAction->initWithDuration ( this->getDuration ( ), tConfig, this->getFinishRecovery ( ) );
    }
    return this->m_pReverseAction;
}


APSCCBezierBy::APSCCBezierBy ( KDvoid ) 
{
    m_bFinishRecovery = KD_FALSE;
}

APSCCBezierBy* APSCCBezierBy::actionWithDuration ( KDfloat fDuration, const ccBezierConfig& tConfig, KDbool bFinishRecovery ) 
{
    APSCCBezierBy*  pAction = new APSCCBezierBy ( );
    
    pAction->initWithDuration ( fDuration, tConfig, bFinishRecovery );
    pAction->autorelease ( );
    
    return pAction;
}

// initializes the action 
KDbool APSCCBezierBy::initWithDuration ( KDfloat fDuration, const ccBezierConfig& tConfig, KDbool bFinishRecovery ) 
{
    if ( CCBezierBy::initWithDuration ( fDuration, tConfig ) )
	{
        m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    }

	return KD_FALSE;    
}


CCObject* APSCCBezierBy::copyWithZone ( CCZone* pZone ) 
{
	CCZone*      pNewZone = KD_NULL;
	APSCCBezierBy*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCBezierBy*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCBezierBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCBezierBy::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( this->getDuration ( ), m_tBezierConfig, this->getFinishRecovery ( ) );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

CCActionInterval* APSCCBezierBy::reverse ( KDvoid ) 
{
	ccBezierConfig  tConfig;
    
	tConfig.tEndPosition   = ccpNeg ( m_tBezierConfig.tEndPosition );
	tConfig.tControlPoint1 = ccpAdd ( m_tBezierConfig.tControlPoint2, ccpNeg ( m_tBezierConfig.tEndPosition ) );
	tConfig.tControlPoint2 = ccpAdd ( m_tBezierConfig.tControlPoint1, ccpNeg ( m_tBezierConfig.tEndPosition ) );
    
	return APSCCBezierBy::actionWithDuration ( m_fDuration, tConfig, this->getFinishRecovery ( ) );    
}

KDvoid APSCCBezierBy::stop ( KDvoid )
{
    if  ( m_bFinishRecovery )
	{
        this->getTarget ( )->setPosition ( m_tStartPosition );
    }
    CCBezierBy::stop ( );
}

APSCCMoveSequence::APSCCMoveSequence ( KDvoid ) 
{
	m_tStartPosition  = CCPointZero;
	m_bFinishRecovery = KD_FALSE;
}

APSCCMoveSequence* APSCCMoveSequence::actionOneTwo ( CCFiniteTimeAction*  pActionOne, CCFiniteTimeAction*  pActionTwo, KDbool bFinishRecovery ) 
{
    APSCCMoveSequence*  pAction = new APSCCMoveSequence ( );
    
    pAction->initOneTwo ( pActionOne, pActionTwo, bFinishRecovery );
    pAction->autorelease ( );
    
    return pAction;
}

KDbool APSCCMoveSequence::initOneTwo ( CCFiniteTimeAction*  pActionOne, CCFiniteTimeAction*  pActionTwo, KDbool bFinishRecovery ) 
{
    if ( CCSequence::initWithTwoActions ( pActionOne, pActionTwo ) ) 
	{
        this->m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    } 
	
	return KD_FALSE;    
}

CCObject* APSCCMoveSequence::copyWithZone ( CCZone* pZone ) 
{
	CCZone*          pNewZone = KD_NULL;
	APSCCMoveSequence*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCMoveSequence*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCMoveSequence ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCSequence::copyWithZone ( pZone );
    
	pCopy->initOneTwo ( m_pActions[0], m_pActions[1] );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCMoveSequence::startWithTarget ( CCNode* pTarget )
{
    CCSequence::startWithTarget ( pTarget );
    m_tStartPosition = pTarget->getPosition ( );
}

CCActionInterval* APSCCMoveSequence::reverse ( KDvoid )
{
    return APSCCMoveSequence::actionOneTwo ( m_pActions[1]->reverse ( ), m_pActions[0]->reverse ( ), this->getFinishRecovery ( ) );
}

KDvoid APSCCMoveSequence::stop ( KDvoid )
{
    if (this->getFinishRecovery ( ) )
	{
        this->getTarget ( )->setPosition ( m_tStartPosition );
    }
    CCSequence::stop ( );
}
//
//  APSCCRotateTo
//

APSCCRotateTo::APSCCRotateTo ( KDvoid ) 
{
	m_pReverseAction  = KD_NULL;
	m_bFinishRecovery = KD_FALSE;
}

APSCCRotateTo::~APSCCRotateTo ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pReverseAction );
}

APSCCRotateTo* APSCCRotateTo::actionWithDuration ( KDfloat fDuration, KDfloat fAngle, KDbool bFinishRecovery )
{
    APSCCRotateTo*  pAction = new APSCCRotateTo;

    pAction->initWithDuration ( fDuration, fAngle, bFinishRecovery );
    pAction->autorelease ( );

    return pAction;
}

// initializes the action 
KDbool APSCCRotateTo::initWithDuration ( KDfloat fDuration, float fDeltaAngle, KDbool bFinishRecovery )
{
    if ( CCRotateTo::initWithDuration ( fDuration, fDeltaAngle ) )
	{
        m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    }

	return KD_FALSE;    
}

CCObject* APSCCRotateTo::copyWithZone ( CCZone* pZone )
{
	CCZone*      pNewZone = KD_NULL;
	APSCCRotateTo*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCRotateTo*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCRotateTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCRotateTo::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( this->getDuration ( ), this->m_tEndAngle.x, this->getFinishRecovery ( ) );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCRotateTo::startWithTarget ( CCNode* pNode )
{
    CCRotateTo::startWithTarget ( pNode );
    
    // Before progress this action, set reverse action's dstAngle.
    if ( this->m_pReverseAction ) 
	{
        this->m_pReverseAction->m_tEndAngle.x = pNode->getRotation ( );
    }    
}

KDvoid APSCCRotateTo::stop ( KDvoid )
{
    if  ( m_bFinishRecovery )
	{
        this->getTarget ( )->setRotation ( m_tStartAngle.x );
    }
    CCRotateTo::stop ( );
}

CCActionInterval* APSCCRotateTo::reverse ( KDvoid )
{
    if ( !m_pReverseAction )
	{
        this->m_pReverseAction = new APSCCRotateTo;
        ( (APSCCRotateTo*) this->m_pReverseAction )->initWithDuration ( this->getDuration ( ), 0.f, this->getFinishRecovery ( ) );
    }
    return this->m_pReverseAction;
}

APSCCRotateBy::APSCCRotateBy ( KDvoid ) 
{
	m_bFinishRecovery = KD_FALSE;
}

APSCCRotateBy* APSCCRotateBy::actionWithDuration ( KDfloat fDuration, KDfloat fAngle, KDbool bFinishRecovery ) 
{
    APSCCRotateBy*  pAction = new APSCCRotateBy;

    pAction->initWithDuration ( fDuration, fAngle, bFinishRecovery );
    pAction->autorelease ( );

    return pAction;
}

// initializes the action 
KDbool APSCCRotateBy::initWithDuration ( KDfloat fDuration, KDfloat fAngle, KDbool bFinishRecovery )
{
    if ( CCRotateBy::initWithDuration ( fDuration, fAngle ) ) 
	{
        m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    } 

	return KD_FALSE;    
}

CCObject* APSCCRotateBy::copyWithZone ( CCZone* pZone )
{
	CCZone*      pNewZone = KD_NULL;
	APSCCRotateBy*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCRotateBy*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCRotateBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCRotateBy::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( this->getDuration ( ), this->m_tAngle.x, this->getFinishRecovery ( ) );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCRotateBy::stop ( KDvoid ) 
{
    if  ( m_bFinishRecovery ) 
	{
        this->getTarget ( )->setRotation ( m_tStartAngle.x );
    }
    CCRotateBy::stop ( );
}

CCActionInterval* APSCCRotateBy::reverse ( KDvoid )
{
    return APSCCRotateBy::actionWithDuration ( this->getDuration ( ), -m_tAngle.x, this->getFinishRecovery ( ) );
}


//
//  APSCCScaleTo
//
APSCCScaleTo::APSCCScaleTo ( KDvoid ) 
{
	m_pReverseAction  = KD_NULL;
	m_bFinishRecovery = KD_FALSE;
}

APSCCScaleTo::~APSCCScaleTo ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pReverseAction );
}

APSCCScaleTo *APSCCScaleTo::actionWithDuration ( KDfloat fDuration, KDfloat sx, KDfloat sy, KDbool bFinishRecovery ) 
{
    APSCCScaleTo*  pAction = new APSCCScaleTo;

    pAction->initWithDuration ( fDuration, sx, sy, bFinishRecovery );
    pAction->autorelease ( );

    return pAction;
}

KDbool APSCCScaleTo::initWithDuration ( KDfloat fDuration, KDfloat sx, KDfloat sy, KDbool bFinishRecovery ) 
{
    if ( CCScaleTo::initWithDuration ( fDuration, sx, sy ) ) 
	{
        m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    }

	return KD_FALSE;        
}

CCObject* APSCCScaleTo::copyWithZone ( CCZone* pZone ) 
{
	CCZone*     pNewZone = KD_NULL;
	APSCCScaleTo*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCScaleTo*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCScaleTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCScaleTo::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( this->getDuration ( ), this->m_tEndScale.x, this->m_tEndScale.y, this->getFinishRecovery ( ) );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCScaleTo::startWithTarget ( CCNode* pNode )
{
    CCScaleTo::startWithTarget ( pNode );
    
    // Before progress this action, set reverse action's scale.
    if ( this->m_pReverseAction ) 
	{
        this->m_pReverseAction->m_tEndScale.x = pNode->getScaleX ( );
        this->m_pReverseAction->m_tEndScale.y = pNode->getScaleY ( );
    }    
}

KDvoid APSCCScaleTo::stop ( KDvoid )
{
    if  ( m_bFinishRecovery ) 
	{
        CCNode*  pNode = this->getTarget ( );
        pNode->setScaleX ( m_tStartScale.x );
        pNode->setScaleY ( m_tStartScale.y );
    }

    CCScaleTo::stop ( );
}

CCActionInterval* APSCCScaleTo::reverse ( KDvoid )
{
    if ( !m_pReverseAction )
	{
        this->m_pReverseAction = new APSCCScaleTo;
        this->m_pReverseAction->initWithDuration ( this->getDuration ( ), 1.f, 1.f, this->getFinishRecovery ( ) );
    }
    return this->m_pReverseAction;
}

APSCCScaleBy::APSCCScaleBy ( KDvoid ) 
{
	m_bFinishRecovery = KD_FALSE;
}

APSCCScaleBy* APSCCScaleBy::actionWithDuration ( KDfloat fDuration, KDfloat sx, KDfloat sy, KDbool bFinishRecovery )
{
    APSCCScaleBy*  pAction = new APSCCScaleBy;

    pAction->initWithDuration ( fDuration, sx, sy, bFinishRecovery );
    pAction->autorelease ( );

    return pAction;
}

KDbool APSCCScaleBy::initWithDuration ( KDfloat fDuration, KDfloat sx, KDfloat sy, KDbool bFinishRecovery ) 
{
    if ( CCScaleBy::initWithDuration ( fDuration, sx, sy ) ) 
	{
        m_bFinishRecovery = bFinishRecovery;
        return KD_TRUE;
    }
	else 
	{
        return KD_FALSE;
    }    
}

CCObject* APSCCScaleBy::copyWithZone ( CCZone* pZone ) 
{
	CCZone*     pNewZone = KD_NULL;
	APSCCScaleBy*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCScaleBy*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCScaleBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCScaleBy::copyWithZone ( pZone );
    
	pCopy->initWithDuration ( this->getDuration ( ), this->m_tEndScale.x, this->m_tEndScale.y, this->getFinishRecovery ( ) );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCScaleBy::stop ( KDvoid ) 
{
    if  ( m_bFinishRecovery )
	{
        CCNode*  pNode = this->getTarget ( );
        pNode->setScaleX ( m_tStartScale.x );
        pNode->setScaleY ( m_tStartScale.y );
    }
    
    CCScaleTo::stop ( );
}

CCActionInterval* APSCCScaleBy::reverse ( KDvoid ) 
{
    return APSCCScaleBy::actionWithDuration ( this->getDuration ( ), 1 / m_tEndScale.x, 1 / m_tEndScale.y, this->getFinishRecovery ( ) );
}

//
// APSCCAnimateChildren 
//
APSCCAnimateChildren::APSCCAnimateChildren ( KDvoid ) 
{
	m_pTargetGraphic	= KD_NULL;
	m_fInterval			= 0.1f;
	m_bWholeRange		= KD_FALSE;
	m_bReversed			= KD_FALSE;
	m_uOriginFrameIndex	= 0;
	m_uFrameCount		= 0;
}

APSCCAnimateChildren* APSCCAnimateChildren::actionWithDuration ( KDfloat fDuration, KDuint uFrameCount, APSGraphicGroup* pTargetGraphic ) 
{
    APSCCAnimateChildren*  pAction = new APSCCAnimateChildren;

    pAction->initWithDuration ( fDuration, uFrameCount, pTargetGraphic );
    pAction->autorelease ( );

    return pAction;
}

APSCCAnimateChildren* APSCCAnimateChildren::actionWithDuration ( KDfloat fDuration, KDuint uFrameCount, APSGraphicGroup* pTargetGraphic, KDfloat fInterval )
{
    APSCCAnimateChildren*  pAction = new APSCCAnimateChildren;

    pAction->initWithDuration ( fDuration, uFrameCount, pTargetGraphic, fInterval );
    pAction->autorelease ( );

    return pAction;
}

KDbool APSCCAnimateChildren::initWithDuration ( KDfloat fDuration, KDuint uFrameCount, APSGraphicGroup* pTargetGraphic ) 
{
    if ( CCActionInterval::initWithDuration ( fDuration ) ) 
	{
        m_uFrameCount	 = uFrameCount;
        m_pTargetGraphic = pTargetGraphic;
        m_bWholeRange	 = KD_TRUE;
        return KD_TRUE;
    }
    return KD_FALSE;
}

KDbool APSCCAnimateChildren::initWithDuration ( KDfloat fDuration, KDuint uFrameCount, APSGraphicGroup* pTargetGraphic, KDfloat fInterval ) 
{
    if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_uFrameCount	 = uFrameCount;
        m_fInterval		 = fInterval;
        m_pTargetGraphic = pTargetGraphic;
        m_bWholeRange	 = KD_FALSE;
        return KD_TRUE;
    }
    return KD_FALSE;
}

CCObject* APSCCAnimateChildren::copyWithZone ( CCZone* pZone ) 
{
	CCZone*				pNewZone = KD_NULL;
	APSCCAnimateChildren*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCAnimateChildren*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCAnimateChildren ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCActionInterval::copyWithZone ( pZone );
    
    if (this->getWholeRange ( ) )
	{
        pCopy->initWithDuration ( this->getDuration ( ), this->getFrameCount ( ), this->getTargetGraphic ( ) );
    } 
	else 
	{
        pCopy->initWithDuration ( this->getDuration ( ), this->getFrameCount ( ), this->getTargetGraphic ( ), this->getInterval ( ) );
    }
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

KDvoid APSCCAnimateChildren::update ( KDfloat fTime ) 
{
    KDuint  uIdx;
    
    if ( this->getReversed ( ) ) 
	{
        fTime = 1 - fTime;
        if ( fTime < 0.f ) 
		{
            fTime = 0.f;
        }
    }
    
    KDuint  uFrameCount = this->getFrameCount ( );
    if ( this->getWholeRange ( ) ) 
	{
        uIdx = (KDuint) ( fTime * uFrameCount );
        
        if ( uIdx >= uFrameCount )
		{
            uIdx = uFrameCount -1;
		}
    } 
	else 
	{
        // +0.005 fixes arithmetic round up
        KDfloat  fDuration = this->getDuration ( ) / this->getInterval ( ) + 0.005f;
        KDuint   uFrame = (KDuint) ( fTime * fDuration ) + this->getOriginFrameIndex ( );
        uIdx = uFrame % uFrameCount;
	}
    
    m_pTargetGraphic->setRunningFrameIndex ( uIdx );    
    m_pTargetGraphic->updateChildSpriteFrames ( );	
}

CCActionInterval* APSCCAnimateChildren::reverse ( KDvoid )
{
    APSCCAnimateChildren*  pReversedAction = APSCCAnimateChildren::actionWithDuration ( this->getDuration ( ), this->getFrameCount ( ), this->getTargetGraphic ( ) );
    
    pReversedAction->m_bReversed = !this->getReversed ( );
    
    return pReversedAction;
}

APSCCAnimate::APSCCAnimate ( KDvoid ) 
{
	m_fInterval			= 0.1f;
	m_uOriginFrameIndex = 0;    
}

APSCCAnimate *APSCCAnimate::actionWithDuration ( KDfloat fDuration, CCAnimation* pAnimation, KDbool bRestoreOriginalFrame ) 
{
    APSCCAnimate*  pAction = new APSCCAnimate;

    pAction->initWithDuration ( fDuration, pAnimation, bRestoreOriginalFrame );
    pAction->autorelease ( );

    return pAction;
}

APSCCAnimate* APSCCAnimate::actionWithDuration ( KDfloat fDuration, CCAnimation* pAnimation, KDbool bRestoreOriginalFrame, KDfloat fInterval ) 
{
    APSCCAnimate*  pAction = new APSCCAnimate;

    pAction->initWithDuration ( fDuration, pAnimation, bRestoreOriginalFrame, fInterval );
    pAction->autorelease ( );

    return pAction;
}

KDbool APSCCAnimate::initWithDuration ( KDfloat fDuration, CCAnimation* pAnimation, KDbool bRestoreOriginalFrame ) 
{
    pAnimation->setRestoreOriginalFrame ( bRestoreOriginalFrame );
    if ( CCAnimate::initWithAnimation ( pAnimation ) )
	{
        this->setDuration ( fDuration );
        m_fInterval = pAnimation->getDelayPerUnit ( );
        return KD_TRUE;
    }

	return KD_FALSE;    
}

KDbool APSCCAnimate::initWithDuration ( KDfloat fDuration, CCAnimation* pAnimation, KDbool bRestoreOriginalFrame, KDfloat fInterval )
{
    pAnimation->setRestoreOriginalFrame ( bRestoreOriginalFrame );
    pAnimation->setDelayPerUnit ( fInterval );
    if ( CCAnimate::initWithAnimation ( pAnimation ) ) 
	{
        this->setDuration ( fDuration );
        m_fInterval = pAnimation->getDelayPerUnit ( );

        return KD_TRUE;
    } 

	return KD_FALSE;    
}

CCObject* APSCCAnimate::copyWithZone ( CCZone* pZone )
{
	CCZone*		pNewZone = KD_NULL;
	APSCCAnimate*  pCopy = KD_NULL;
	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (APSCCAnimate*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new APSCCAnimate ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
    
	CCAnimate::copyWithZone ( pZone );
    
    this->getAnimation ( )->setRestoreOriginalFrame ( this->getAnimation ( )->getRestoreOriginalFrame ( ) );
    this->getAnimation ( )->setDelayPerUnit ( this->getInterval ( ) );
    pCopy->initWithAnimation ( this->getAnimation ( ) );
    pCopy->setDuration ( this->getDuration ( ) );
    pCopy->m_fInterval = this->getInterval ( );
	
	CC_SAFE_DELETE ( pNewZone );
	return pCopy;
}

CCActionInterval* APSCCAnimate::reverse ( KDvoid ) 
{
    CCArray*  pOldArray = this->getAnimation ( )->getFrames ( );
    CCArray*  pNewArray = CCArray::createWithCapacity ( pOldArray->count ( ) );
    
    CCARRAY_VERIFY_TYPE ( pOldArray, CCAnimationFrame* );
    
    if ( pOldArray->count ( ) > 0 )
    {
        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH_REVERSE ( pOldArray, pObject )
        {
            CCAnimationFrame*  pElement = (CCAnimationFrame*) pObject;
            if ( !pElement )
            {
                break;
            }
            
            pNewArray->addObject ( (CCAnimationFrame*) pElement->copy ( )->autorelease( ) );
        }
    }
    
    CCAnimation*  pNewAnim = CCAnimation::create ( pNewArray, this->getAnimation ( )->getDelayPerUnit ( ), this->getAnimation ( )->getLoops ( ) );
    pNewAnim->setRestoreOriginalFrame ( this->getAnimation ( )->getRestoreOriginalFrame( ) );
	return APSCCAnimate::actionWithDuration ( m_fDuration, pNewAnim, this->getAnimation ( )->getRestoreOriginalFrame ( ), this->getInterval ( ) );
}

NS_APS_END

