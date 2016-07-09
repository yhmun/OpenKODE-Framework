/* --------------------------------------------------------------------------
 *
 *      File            LevelFileHandler.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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

#include "Precompiled.h"
#include "LevelFileHandler.h"
#include "XMTinyXml/tinyxml2.h"
#include "Model/PineappleModel.h"
#include "Model/RopeModel.h"
#include "Constants.h"
#include "Utils/GameManager.h"
//#include <algorithm>

LevelFileHelper::LevelFileHelper ( const KDchar* szLevelFileName )
{
	m_szLevelFile	= szLevelFileName;
	m_pRopes		= KD_NULL;
	m_pPineapples	= KD_NULL;

    this->loadLevel ( );
}

LevelFileHelper::~LevelFileHelper ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pPineapples );
    CC_SAFE_RELEASE ( m_pRopes );
    
    CCLOG ( "~LevelFileHelper" );
}

KDvoid LevelFileHelper::loadLevel ( KDvoid )
{
	tinyxml2::XMLDocument	tDoc;
    
    m_pPineapples = CCArray::create ( );
    m_pPineapples->retain ( );
    
    m_pRopes = CCArray::create ( );
    m_pRopes->retain();
    
    KDubyte*	pBuffer = KD_NULL;
	KDsize		uBufferSize = 0;
    
    // for android
    pBuffer = CCFileUtils::sharedFileUtils ( )->getFileData ( m_szLevelFile, "r", &uBufferSize );
    
    if ( pBuffer )
    {
        tDoc.Parse ( (const KDchar*) pBuffer );
    }
    
//    doc.LoadFile(m_szLevelFile);
    
    tinyxml2::XMLElement*	pLevel = tDoc.RootElement ( );
    tinyxml2::XMLElement*	pPineaplle = pLevel->FirstChildElement ( "pineapple" );
    while ( pPineaplle )
	{
        KDint		nId = (KDint) pPineaplle->IntAttribute ( "id" );
        KDfloat		x   = pPineaplle->FloatAttribute ( "x" );
        KDfloat		y   = pPineaplle->FloatAttribute ( "y" );
        KDfloat		fDamping = pPineaplle->FloatAttribute ( "damping" );
        
        PineappleModel*		pModel = new PineappleModel ( );
        pModel->m_nId		= nId;
        pModel->m_tPosition = ccp ( x, y );
        pModel->m_fDamping	= fDamping > 0 ? fDamping : kDefaultDamping;
        
        m_pPineapples->addObject ( pModel );
        pModel->release ( );       
        
        pPineaplle = pPineaplle->NextSiblingElement ( "pineapple" );
    }
    
    tinyxml2::XMLElement*	pRope = pLevel->FirstChildElement ( "rope" );
    KDint					nRopeID = 1;
    
    while ( pRope ) 
	{
        RopeModel*			pModel = new RopeModel;
        
        pModel->m_nId = nRopeID;
        
        //achorA
        const tinyxml2::XMLElement*		pRopeChild = pRope->FirstChildElement ( );
        KDint		nBodyAId = pRopeChild->IntAttribute ( "body" );
        pModel->m_nBodyAId = nBodyAId;
        
        KDfloat		ax;
        KDfloat		ay;
        
        if ( pModel->m_nBodyAId == -1 )
		{
            ax = pRopeChild->FloatAttribute ( "x" );
            ay = pRopeChild->FloatAttribute ( "y" );
        }
		else
		{
            PineappleModel*		pModel2 = this->getPineappleWithID ( pModel->m_nBodyAId );
            ax = pModel2->m_tPosition.x;
            ay = pModel2->m_tPosition.y;
        }
        
        pModel->m_tAchorA = ccp ( ax, ay );
        
        //achorB
        pRopeChild = pRopeChild->NextSiblingElement ( );
        KDint	nBodyBId = pRopeChild->IntAttribute ( "body" );
        pModel->m_nBodyBId = nBodyBId;

        
        if ( pModel->m_nBodyBId == -1 ) 
		{
            ax = pRopeChild->FloatAttribute ( "x" );
            ay = pRopeChild->FloatAttribute ( "y" );
        }
		else
		{
            PineappleModel*		pModel2 = this->getPineappleWithID ( pModel->m_nBodyBId );
            ax = pModel2->m_tPosition.x;
            ay = pModel2->m_tPosition.y;
        }
        
        pModel->m_tAchorB = ccp ( ax, ay );
        
        KDfloat		fSagity = pRope->FloatAttribute ( "sagity" );
        if ( fSagity > 0 )
		{
            pModel->m_fSagity = fSagity;
        }
        
        m_pRopes->addObject ( pModel );
        pModel->release ( );
        
        nRopeID++;
        
        
        // next rope
        pRope = pRope->NextSiblingElement ( "rope" );
    }        
}

PineappleModel* LevelFileHelper::getPineappleWithID ( KDint nId )
{
    return (PineappleModel*) this->getModelWithIDArray ( nId, m_pPineapples );
}

AbstractModel* LevelFileHelper::getModelWithIDArray ( KDint nId, CCArray* pArray )
{
    CCObject*	pObject;
    CCARRAY_FOREACH ( pArray, pObject )
    {
        AbstractModel*	pModel = (AbstractModel*) pObject;
        if ( pModel->m_nId == nId )
		{
            return pModel;
        }
    }

    return KD_NULL;
}

static KDbool compareScores ( const KDvoid* p1, const KDvoid* p2 )
{
    AbstractModel*	pAm1 = (AbstractModel*) p1;
    AbstractModel*	pAm2 = (AbstractModel*) p2;
    KDbool			bResult = pAm1->m_nId < pAm2->m_nId;

    return bResult;
}

KDvoid LevelFileHelper::sortArrayById ( CCArray* pArray )
{
    std::sort ( pArray->m_pData->arr, pArray->m_pData->arr + pArray->m_pData->num, compareScores );
}

KDint LevelFileHelper::findUnusedIdInArray ( CCArray* pArray )
{
    KDint		nFirstUnusedID = -1;
    KDint		nLastID = 0;

    CCObject*	pObject;    
    CCARRAY_FOREACH ( pArray, pObject )
    {
        AbstractModel*	pAm = (AbstractModel*) pObject;
        if ( pAm->m_nId - nLastID > 1 ) 
		{
            nFirstUnusedID = nLastID + 1;
            break;
        }
        nLastID++;
    }
    
    if ( nFirstUnusedID == -1 )
	{
        nFirstUnusedID = nLastID + 1;
    }
    
    return nFirstUnusedID;
}

PineappleModel* LevelFileHelper::addPineappleAt ( CCPoint tPos )
{
    KDint				nFirstUnusedID = this->findUnusedIdInArray ( m_pPineapples );
    PineappleModel*		pNewPineapple  = new PineappleModel;

    pNewPineapple->m_nId = nFirstUnusedID;
    pNewPineapple->m_tPosition = tPos;
    m_pPineapples->addObject ( pNewPineapple );
    
    return pNewPineapple;
}


KDvoid LevelFileHelper::addRopeFromModel ( RopeModel* pModel )
{
    this->sortArrayById ( m_pRopes );
    
    if ( !pModel->m_nId ) 
	{
        KDint	nFirstUnusedID = this->findUnusedIdInArray ( m_pRopes );
        pModel->m_nId = nFirstUnusedID;
    }
    m_pRopes->addObject ( pModel );
}


KDvoid LevelFileHelper::removeRopeWithID ( KDint nId )
{
    RopeModel*	pRopeToBeRemoved;
    CCObject*	pObject;
    CCARRAY_FOREACH ( m_pRopes, pObject )
    {
        RopeModel*	pRope = (RopeModel*) pObject;
        if ( pRope->m_nId == nId )
		{
            pRopeToBeRemoved = pRope;
            break;
        }
    }
    m_pRopes->removeObject ( pRopeToBeRemoved );
}

KDvoid LevelFileHelper::removePineappleWithID ( KDint nId )
{
    PineappleModel*		pPineappleToBeRemoved;
    CCObject*			pObject;
    CCARRAY_FOREACH ( m_pPineapples, pObject )
    {
        PineappleModel*		pPineapple = (PineappleModel*) pObject;
        if ( pPineapple->m_nId == nId ) 
		{
            pPineappleToBeRemoved = pPineapple;
            break;
        }
    }
    m_pPineapples->removeObject ( pPineappleToBeRemoved );
}

KDvoid LevelFileHelper::moveRopeWithId ( KDint nRopeID, EAnchorType eType, KDint nAnchorID, CCPoint tPos )
{
    CCObject*	pObject;
    CCARRAY_FOREACH ( m_pRopes, pObject )
    {
        RopeModel*	pRopeModel = (RopeModel*) pObject;
        if ( pRopeModel->m_nId == nRopeID ) 
		{
            switch ( nAnchorID )
			{
                case kAnchorA :
                    pRopeModel->m_tAchorA  = tPos;
                    pRopeModel->m_nBodyAId = nAnchorID;
                    break;

                case kAnchorB :
                    pRopeModel->m_tAchorB  = tPos;
                    pRopeModel->m_nBodyBId = nAnchorID;
                    break;

                default :
                    break;

            }
        }
    }

}

KDvoid LevelFileHelper::movePineappleWithID ( KDint nPineappleID, CCPoint tPos )
{
    CCObject*	pObject;
    CCARRAY_FOREACH ( m_pPineapples, pObject )
    {
        PineappleModel*		pModel = (PineappleModel*) pObject;
        if ( pModel->m_nId == nPineappleID ) 
		{
            pModel->m_tPosition = tPos;
            break;
        }
    }
}

KDvoid LevelFileHelper::saveFile ( KDvoid )
{
//    // create empty xml structure

	tinyxml2::XMLDocument*	pDoc = new tinyxml2::XMLDocument ( );
	tinyxml2::XMLNode*		pLevelElement = pDoc->InsertEndChild ( pDoc->NewElement ( "level" ) );
    CCObject*				pObject;
    CCARRAY_FOREACH ( m_pPineapples, pObject )
    {
        PineappleModel*			pModel = (PineappleModel*) pObject;
		tinyxml2::XMLElement*	pPineappleElemnent = pDoc->NewElement ( "pineapple" );
        pPineappleElemnent->SetAttribute ( "id", ccszf ( "%d"  , pModel->m_nId ) );
        pPineappleElemnent->SetAttribute ( "x" , ccszf ( "%.3f", pModel->m_tPosition.x ) );
        pPineappleElemnent->SetAttribute ( "y" , ccszf ( "%.3f", pModel->m_tPosition.y ) );
        if ( kdFabsf ( pModel->m_fDamping - kDefaultDamping ) > 0.00001f )
		{
            pPineappleElemnent->SetAttribute ( "damping", ccszf ( "%.1f", pModel->m_fDamping ) );
        }
        
        pLevelElement->InsertEndChild ( pPineappleElemnent );
    }

    // Add all rope elements
    CCARRAY_FOREACH ( m_pRopes, pObject )
    {
        RopeModel*				pPope = (RopeModel*) pObject;                
		tinyxml2::XMLElement*	pRopeElemnent = pDoc->NewElement ( "rope" );
        
        //Anchor A
        tinyxml2::XMLElement*	pAnchorAElement = pDoc->NewElement ( "anchorA" );
        pAnchorAElement->SetAttribute ( "body", ccszf ( "%d", pPope->m_nBodyAId ) );
        
        if ( pPope->m_nBodyAId == -1 ) 
		{
            pAnchorAElement->SetAttribute ( "x", ccszf ( "%.3f", pPope->m_tAchorA.x ) );
            pAnchorAElement->SetAttribute ( "y", ccszf ( "%.3f", pPope->m_tAchorA.y ) );
        }
        
        // Anchor B
        tinyxml2::XMLElement*	pAnchorBElement = pDoc->NewElement ( "anchorB" );
        pAnchorBElement->SetAttribute("body", ccszf ("%d", pPope->m_nBodyBId ) );
        if ( pPope->m_nBodyBId == -1 ) 
		{
            pAnchorBElement->SetAttribute("x", ccszf ( "%.3f", pPope->m_tAchorB.x ) );
            pAnchorBElement->SetAttribute("y", ccszf ( "%.3f", pPope->m_tAchorB.y ) );
        }
        
        
        // add rope
        pRopeElemnent->InsertEndChild ( pAnchorAElement );
        pRopeElemnent->InsertEndChild ( pAnchorBElement );
        pRopeElemnent->InsertEndChild ( pRopeElemnent );
    }

    //doc->Print();
    CCString*		pLevelName = CCString::createWithFormat ( "level%d.xml",GameManager::getInstance ( )->m_nLevelNumber );
    std::string		sFileName  = CCFileUtils::sharedFileUtils ( )->getWritablePath ( ) + pLevelName->getCString ( );
    pDoc->SaveFile ( sFileName.c_str ( ) );
    m_szLevelFile = sFileName.c_str ( );
}

KDvoid LevelFileHelper::reset ( KDvoid )
{
    this->loadLevel ( );
}
