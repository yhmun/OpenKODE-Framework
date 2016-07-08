/* --------------------------------------------------------------------------
 *
 *      File            APSBezierMoveAction.cpp
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
#include "extensions/CCArtPig/APSBezierMoveAction.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSCCActionInterval.h"
#include "support/CCPointExtension.h"

NS_APS_BEGIN

const std::string& APSBezierMoveAction::getAnchorsKey ( KDvoid ) 
{
    static const std::string  sKey = "anchors";
    return sKey;
}

APSBezierMoveAction::APSBezierMoveAction ( APSDictionary* pProperties )
{
    if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

APSBezierMoveAction::~APSBezierMoveAction ( KDvoid )
{
	APS_FOREACH ( std::vector<APSAnchor*>, this->m_aAnchors, iter )
	{
        APS_SAFE_DELETE ( *iter );
    }
}

KDbool APSBezierMoveAction::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSIntervalGraphicAction::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    // set anchors
    APSArray*  pAnchorsPl = (APSArray*) pProperties->getObjectForKey ( this->getAnchorsKey ( ) );

    APS_FOREACH ( APSArrayStorage, *pAnchorsPl, iter ) 
	{
        APSAnchor*  pAnchor = new APSAnchor;
        pAnchor->initWithProperties ( (APSDictionary*) *iter );
        this->m_aAnchors.push_back ( pAnchor );
    }
    return KD_TRUE;
}

KDvoid APSBezierMoveAction::saveNodeState ( KDvoid ) 
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic ) 
	{
        // Save the position of graphic into the first anchor's position.
        APSAnchor*  pAnc1 = this->m_aAnchors[0];
        pAnc1->setPosition ( pGraphic->getNode ( )->getPosition ( ) );
    }
}

KDvoid APSBezierMoveAction::restoreNodeState ( KDvoid )
{
    APSGraphic*  pGraphic = this->getTargetGraphic ( );
    if ( pGraphic )
	{
        APSAnchor*  pFAnchor = this->m_aAnchors[0];
        pGraphic->getNode ( )->setPosition ( pFAnchor->getPosition ( ) );
    }
}

std::vector<APSAnchor*>* APSBezierMoveAction::getAnchors ( KDvoid ) 
{
    return &this->m_aAnchors;
}

CCActionInterval* APSBezierMoveAction::makeGraphicCCAction ( KDbool bUniversal, KDfloat fScale )
{
    if ( bUniversal && this->isRestoringAction ( ) )
	{
        APSLOG ( "Making general purposed action of %s failed. a repeating action with restoring graphic's state cannot be made for general purpose action.", this->getClassName ( ).c_str ( ) );
        return KD_NULL;
    }

    KDint  nSize = this->m_aAnchors.size ( );
    
	std::vector<CCActionInterval*>  aActions;
    aActions.reserve ( nSize );
    
    ccBezierConfig     tConfig;
    CCActionInterval*  pAction = KD_NULL;
        
    APSAnchor*  pAnc2 = this->m_aAnchors[0];
    for ( KDint i = 0; i < nSize - 1; i++ )
	{
        APSAnchor*  pAnc1 = pAnc2;
        pAnc2 = this->m_aAnchors [ i + 1 ];
        
        tConfig.tControlPoint1 = pAnc1->getFrontHandle ( fScale );
        tConfig.tControlPoint2 = pAnc2->getBackHandle ( fScale );
        tConfig.tEndPosition   = pAnc2->getPosition ( fScale );
        if ( this->getRelative ( ) )
		{
            CCPoint  tPrevPoint = pAnc1->getPosition ( fScale );
            tConfig.tControlPoint1 = ccpSub ( tConfig.tControlPoint1, tPrevPoint );
            tConfig.tControlPoint2 = ccpSub ( tConfig.tControlPoint2, tPrevPoint );
            tConfig.tEndPosition   = ccpSub ( tConfig.tEndPosition  , tPrevPoint );

            pAction = CCBezierBy::create ( pAnc1->getProportion ( ) * this->getDuration ( ), tConfig );
        } 
		else
		{
            pAction = APSCCBezierTo::actionWithDuration ( pAnc1->getProportion ( ) * this->getDuration ( ), tConfig );                
        }
        
        aActions.push_back ( pAction );
    }
    
    if ( aActions.size ( ) == 1 )
	{
        // If bezier move is single move, append null move action.
        aActions.push_back ( CCMoveBy::create ( 0.f, CCPointZero ) );
    }
    
    CCActionInterval*  pSeq = KD_NULL;
	std::vector<CCActionInterval*>::reverse_iterator  rit;
	std::vector<CCActionInterval*>::reverse_iterator  rend = aActions.rend ( );
    for ( rit = aActions.rbegin ( ) ; rit < rend; ++rit ) 
	{
        pAction = *rit;
        if ( !pSeq )
		{
            pSeq = pAction;
        }
		else if ( rit + 1 == rend && bUniversal && this->isRestoringAction ( ) ) 
		{
            pSeq = APSCCMoveSequence::actionOneTwo ( pAction, pSeq, KD_TRUE );
        }
		else 
		{
            pSeq = CCSequence::createWithTwoActions ( pAction, pSeq );
        }
    }
    
    return pSeq;
}

NS_APS_END
