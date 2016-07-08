/* --------------------------------------------------------------------------
 *
 *      File            APSResourceManager.cpp
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
#include "extensions/CCArtPig/APSResourceManager.h"
#include "extensions/CCArtPig/APSMedium.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "extensions/CCArtPig/APSXMLParser.h"
#include "extensions/CCArtPig/APSSpriteSheetHolder.h"

#include "extensions/CCArtPig/APSImageHolder.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSSprite.h"
#include "extensions/CCArtPig/APSActionGroup.h"
#include "extensions/CCArtPig/APSAudioAction.h"
#include "extensions/CCArtPig/APSLinkerAction.h"
#include "extensions/CCArtPig/APSGraphicAction.h"
#include "extensions/CCArtPig/APSInstantFadeAction.h"
#include "extensions/CCArtPig/APSInstantMoveAction.h"
#include "extensions/CCArtPig/APSInstantRotationAction.h"
#include "extensions/CCArtPig/APSInstantScaleAction.h"
#include "extensions/CCArtPig/APSVisibilityAction.h"
#include "extensions/CCArtPig/APSFadeAction.h"
#include "extensions/CCArtPig/APSMoveAction.h"
#include "extensions/CCArtPig/APSRotationAction.h"
#include "extensions/CCArtPig/APSScaleAction.h"
#include "extensions/CCArtPig/APSAnchor.h"
#include "extensions/CCArtPig/APSBezierMoveAction.h"
#include "extensions/CCArtPig/APSAnimateAction.h"
#include "extensions/CCArtPig/APSAudioHolder.h"
#include "extensions/CCArtPig/APSSpriteSheetHolder.h"
#include "extensions/CCArtPig/APSSymbol.h"

#include "support/CCDirector.h"
#include "support/CCFileUtils.h"

NS_APS_BEGIN

typedef std::map<const std::string, APSResourceManager*>  APSResourceManagerMap;
static APSResourceManagerMap  l_aResourceManagersCache;

const std::string& APSResourceManager::getVersionKey ( KDvoid ) 
{
    static const std::string  sKey = "version";
    return sKey;
}

const std::string& APSResourceManager::getProjectIdKey ( KDvoid )
{
    static const std::string  sKey = "projectId";
    return sKey;
}

const std::string& APSResourceManager::getDeviceInfoKey ( KDvoid ) 
{
    static const std::string  sKey = "deviceInfo";
    return sKey;
}

const std::string& APSResourceManager::getMediumsKey ( KDvoid ) 
{
    static const std::string  sKey = "mediums";
    return sKey;
}

APSResourceManager::APSResourceManager ( const KDchar* szProjectID, const KDchar* szLocation )
{
	m_pDeviceInfo		= KD_NULL;
	m_pSymbolProperties = KD_NULL;
    
    this->setProjectId ( szProjectID );
    
    if ( szLocation )
	{
        this->setLocation ( szLocation );
    }
    
    m_pSymbolProperties = new APSDictionary ( );    
}

APSResourceManager::~APSResourceManager ( KDvoid )
{    
    APS_FOREACH ( APSMediumMap, this->m_aMediums, iter )
	{
        APS_SAFE_DELETE ( iter->second );
    }
	
	APS_SAFE_DELETE  ( m_pDeviceInfo );    
    APS_SAFE_RELEASE ( m_pSymbolProperties );
}

APSResourceManager* APSResourceManager::getResourceManagerForProjectId ( const KDchar* szProjectID, const KDchar* szLocation )
{
    APSResourceManager*  pResourceManager = l_aResourceManagersCache [ szProjectID ];
    
    if ( !pResourceManager ) 
	{                
        pResourceManager = new APSResourceManager ( szProjectID, szLocation );
        APSXMLParser*  pParser = new APSXMLParser ( );
        
		std::string  sFileName = APSResourceManager::getXMLFilenameForProjectId ( szProjectID );
        std::string  sPath = pResourceManager->getPathForFilename ( sFileName.c_str ( ) );
        
        if ( pParser->loadFile ( sPath.c_str ( ) ) && pResourceManager->initWithProperties ( pParser->getDictionary ( ) ) )
		{
            l_aResourceManagersCache [ szProjectID ] = pResourceManager;
            
            // Find and set current device in runtime.
            APSDeviceInfo*  pDevInfo = pResourceManager->getDeviceInfo ( );
            pDevInfo->setAppliedDevice ( pDevInfo->getMostFittingDevice ( ) );
        } 
		else 
		{
            APS_SAFE_DELETE ( pResourceManager );
            APSLOG ( "Failed to load project '%s'.", szProjectID );
        }
        APS_SAFE_DELETE ( pParser );
    }
    return pResourceManager;
}

KDvoid APSResourceManager::removeResourceManagerForProjectId ( const std::string& sProjectID )
{
    APSResourceManager*  pRM = l_aResourceManagersCache [ sProjectID ];
    if ( pRM ) 
	{
        l_aResourceManagersCache.erase ( sProjectID );
        delete pRM;
    }
}

KDvoid APSResourceManager::removeAllResourceManagers ( KDvoid )
{
	for ( std::map<const std::string, APSResourceManager*>::const_iterator  iter = l_aResourceManagersCache.begin ( ); iter != l_aResourceManagersCache.end ( ); ++iter )
	{
        if ( iter->second )
		{
            delete iter->second;
        }
    }
    l_aResourceManagersCache.clear ( );
}

std::string APSResourceManager::getXMLFilenameForProjectId ( const std::string& sProjectID ) 
{
    return sProjectID + "_RM.xml";
}

APSResource* APSResourceManager::newResourceWithProperties ( APSDictionary* pProperties, APSSymbol* pParentSymbol )
{
    // retrieve class name
    APSString*  pCN = (APSString*) pProperties->getObjectForKey ( APSObject::getClassNameKey ( ) );
    if ( pCN ) 
	{
        // Allocate a resource object with the class name 
        APSResource*  pRS = (APSResource*) APSResourceManager::allocForClassName ( pCN->getString ( ) );
        if ( pRS )
		{
            pRS->setParentSymbol ( pParentSymbol );
            
            // Initialize with the properties in the dictionary, and add into the resource list.
            pRS->initWithProperties ( pProperties );
        } 
		else 
		{
            APSLOG ( "Creating a resource failed for class %s.", pCN->getString ( ).c_str ( ) );
            return KD_NULL;
        }
        return pRS;
    }
    
    return NULL;
}

APSMedium* APSResourceManager::getMediumForCode ( const std::string& sCode ) 
{
    return m_aMediums [ sCode ];
}

CCPoint APSResourceManager::getScaleModelToDevice ( APSOrientation uOrientation, KDbool bKeepRatio ) const
{
	CCSize  tScreenSize = CCDirector::sharedDirector ( )->getWinSize ( );
    if ( uOrientation == kAPSOrientationPortrait || uOrientation == kAPSOrientationPortraitUpsideDown )
	{
        tScreenSize = CCSize ( tScreenSize.cy, tScreenSize.cx );
    }
    
    CCSize  tModelSize = this->getDeviceInfo ( )->getMainModelDevice ( )->getScreenSize ( );
    if ( bKeepRatio )
	{
        CCPoint  tRate = CCPoint ( tScreenSize.cx / tModelSize.cx, tScreenSize.cy / tModelSize.cy );
        
        // Choose the minimum scale change from model device size
        KDfloat  fScale = KD_MIN ( tRate.x, tRate.y );
        return CCPoint ( fScale, fScale );
    } 
	else 
	{
        return CCPoint ( tScreenSize.cx / tModelSize.cx, tScreenSize.cy / tModelSize.cy );
    }
}

KDbool APSResourceManager::initWithProperties ( APSDictionary* pProperties )
{
    APSString*  pVersion = (APSString*) pProperties->getObjectForKey ( this->getVersionKey ( ) );
    if ( pVersion ) 
	{
        this->setVersion ( kdAtof ( pVersion->c_str ( ) ) );
    }

    // set projectId
    APSString*  pProjectID = (APSString*) pProperties->getObjectForKey ( this->getProjectIdKey ( ) );
    if ( pProjectID )
	{
        this->setProjectId ( pProjectID->c_str ( ) );
    }
    
    // set deviceInfo. This part comes first so that newly created resources can use deviceInfo. 
    APSDictionary*  pDict = (APSDictionary*) pProperties->getObjectForKey ( this->getDeviceInfoKey ( ) );
    if ( pDict ) 
	{
        this->m_pDeviceInfo = new APSDeviceInfo;
        if ( !this->m_pDeviceInfo->initWithProperties ( pDict ) )
		{
            return KD_FALSE;
        }
    }
    
    // set m_aMediums
    APSArray*  pArray = (APSArray*) pProperties->getObjectForKey ( this->getMediumsKey ( ) );
    if ( pArray )
	{
        APS_FOREACH ( APSArrayStorage, *pArray, iter )
		{
            APSDictionary*  pDict2 = (APSDictionary*) *iter;
            // retrieve class name
            APSString*  pCN = (APSString*) pDict2->getObjectForKey ( APSObject::getClassNameKey ( ) );
            if ( pCN ) 
			{
                // Allocate a resource object with the class name 
                APSMedium*  pMedium = (APSMedium*) allocForClassName ( pCN->c_str ( ) );
                if ( pMedium )
				{                    
                    // initialize with the properties in the dictionary, and add into the resource list.
                    if ( !pMedium->initWithProperties ( pDict2 ) )
					{
                        return KD_FALSE;
                    }
                    
                    this->addMedium ( pMedium );
                }
				else 
				{
                    APSLOG ( "Failed to create class %s.", pCN->getString ( ).c_str ( ) );
                    return KD_FALSE;
                }
            }
        }
    }

    return KD_TRUE;
}

APSSymbol* APSResourceManager::newSymbol ( const KDchar* szTag, KDbool bApplyScale, KDbool bCacheProperties )
{
    // Check if symbol properties are cached.
    APSDictionary*  pSymProp = (APSDictionary*) m_pSymbolProperties->getObjectForKey ( szTag );
    
    if ( !pSymProp ) 
	{
		std::string  sFileName = APSSymbol::getXMLFilename ( this->getProjectId ( ), szTag );

		std::string  sPath = this->getPathForFilename ( sFileName.c_str ( ) );
        
        APSLOG ( "Loading APSSymbol with tag '%s'.", szTag );

        APSXMLParser*  pParser = new APSXMLParser ( );
        if ( pParser->loadFile ( sPath.c_str ( ) ) )
		{
            pSymProp = pParser->getDictionary ( );
            // Cache properties for later use.
            if ( bCacheProperties )
			{
                m_pSymbolProperties->setObjectForKey ( pParser->getDictionary ( ), szTag );
            }
        }
        APS_SAFE_DELETE ( pParser );
    }
    
    if ( pSymProp ) 
	{
        APSSymbol*  pSymbol = new APSSymbol ( pSymProp );
        
        if ( pSymbol->getIsRoot ( ) )
		{
            if ( bApplyScale )
			{
                // In case of single symbol mode(=symbol does not contain sub-symbol)
                pSymbol->setScale ( this->getScaleModelToDevice ( pSymbol->getOrientation ( ) ) );
            }
            
            // Position in the center of screen.
            CCSize  tScrSize = CCDirector::sharedDirector ( )->getWinSize ( );
            pSymbol->setPosition ( CCPoint ( tScrSize.cx * 0.5f, tScrSize.cy * 0.5f ) );
            
            switch ( pSymbol->getOrientation ( ) )
			{
                case kAPSOrientationPortrait			:	pSymbol->setRotation ( 90 );	break;
                case kAPSOrientationPortraitUpsideDown	:	pSymbol->setRotation ( -90 );	break;
                case kAPSOrientationLandscapeLeft		:	pSymbol->setRotation ( 180 );	break;
                default :	break;
            }
        }

        pSymbol->setResourceManager ( this );
        return pSymbol;
    }
	else 
	{
        APSLOG ( "Failed to load symbol with tag '%s'.", szTag );
    }
    
    return KD_NULL;
}

KDvoid APSResourceManager::addMedium ( APSMedium* pMedium ) 
{
    this->m_aMediums [ pMedium->getCode_string ( ) ] = pMedium;
    pMedium->setResourceManager ( this );
}

std::string APSResourceManager::getPathForFilename ( const KDchar* szFileName ) const 
{
    if ( *this->getLocation ( ) ) 
	{
		std::string  sStr = this->getLocation ( );
        sStr.append ( "/" );
        sStr.append ( szFileName );
		return CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( sStr.c_str ( ) );
    } 
	else 
	{
		return CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFileName );
    }
}

APSResource* APSResourceManager::allocForClassName ( const std::string& sName ) 
{
	static std::map<const std::string, APSResource *(*)()>*  pAllocators = KD_NULL;
    
    if ( !pAllocators) 
	{
		pAllocators = new std::map<const std::string, APSResource *(*)()>;
        
        ( *pAllocators ) [ APSResource				::className ( ) ] = ( APSResource *(*)() ) ( APSResource				::alloc );
        
        //
        //  Graphic
        //
        ( *pAllocators ) [ APSSymbol				::className ( ) ] = ( APSResource *(*)() ) ( APSSymbol					::alloc );
        ( *pAllocators ) [ APSGraphic				::className ( ) ] = ( APSResource *(*)() ) ( APSSprite					::alloc );
        ( *pAllocators ) [ APSGraphicGroup			::className ( ) ] = ( APSResource *(*)() ) ( APSGraphicGroup			::alloc );
        ( *pAllocators ) [ APSSprite				::className ( ) ] = ( APSResource *(*)() ) ( APSSprite					::alloc );        

		//
        //  Action
        //
        ( *pAllocators ) [ APSAction				::className ( ) ] = ( APSResource *(*)() ) ( APSAction					::alloc );
        ( *pAllocators ) [ APSActionGroup			::className ( ) ] = ( APSResource *(*)() ) ( APSActionGroup				::alloc );
        ( *pAllocators ) [ APSAudioAction			::className ( ) ] = ( APSResource *(*)() ) ( APSAudioAction				::alloc );
        ( *pAllocators ) [ APSLinkerAction			::className ( ) ] = ( APSResource *(*)() ) ( APSLinkerAction			::alloc );    
		( *pAllocators ) [ APSInstantFadeAction		::className ( ) ] = ( APSResource *(*)() ) ( APSInstantFadeAction		::alloc );
        ( *pAllocators ) [ APSInstantMoveAction		::className ( ) ] = ( APSResource *(*)() ) ( APSInstantMoveAction		::alloc );
        ( *pAllocators ) [ APSInstantRotationAction	::className ( ) ] = ( APSResource *(*)() ) ( APSInstantRotationAction	::alloc );
        ( *pAllocators ) [ APSInstantScaleAction	::className ( ) ] = ( APSResource *(*)() ) ( APSInstantScaleAction		::alloc );
        ( *pAllocators ) [ APSVisibilityAction		::className ( ) ] = ( APSResource *(*)() ) ( APSVisibilityAction		::alloc );
        ( *pAllocators ) [ APSFadeAction			::className ( ) ] = ( APSResource *(*)() ) ( APSFadeAction				::alloc );
        ( *pAllocators ) [ APSMoveAction			::className ( ) ] = ( APSResource *(*)() ) ( APSMoveAction				::alloc );
        ( *pAllocators ) [ APSRotationAction		::className ( ) ] = ( APSResource *(*)() ) ( APSRotationAction			::alloc );
        ( *pAllocators ) [ APSScaleAction			::className ( ) ] = ( APSResource *(*)() ) ( APSScaleAction				::alloc );
        ( *pAllocators ) [ APSAnchor				::className ( ) ] = ( APSResource *(*)() ) ( APSAnchor					::alloc );
        ( *pAllocators ) [ APSBezierMoveAction		::className ( ) ] = ( APSResource *(*)() ) ( APSBezierMoveAction		::alloc );
        ( *pAllocators ) [ APSAnimateAction			::className ( ) ] = ( APSResource *(*)() ) ( APSAnimateAction			::alloc );
        
        //
        //  Media
        //
        ( *pAllocators ) [ APSAudioHolder			::className ( ) ] = ( APSResource *(*)() ) ( APSAudioHolder				::alloc );
        ( *pAllocators ) [ APSImageHolder			::className ( ) ] = ( APSResource *(*)() ) ( APSImageHolder				::alloc );
        ( *pAllocators ) [ APSSpriteSheetHolder		::className ( ) ] = ( APSResource *(*)() ) ( APSSpriteSheetHolder		::alloc );
    }
    
    APSResource*  (*pAlloc)() = ( *pAllocators ) [ sName ];
    if ( pAlloc ) 
	{
        return (APSResource*) pAlloc ( );
    }

	return KD_NULL;
}

NS_APS_END
