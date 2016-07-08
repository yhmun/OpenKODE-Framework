/* -----------------------------------------------------------------------------------
 *
 *      File            GB2ShapeCache.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2013      Created by Thomas Broquist. All rights reserved.
 *
 *         http://www.PhysicsEditor.de  
 *         http://texturepacker.com
 *         http://www.code-and-web.de
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCGB2Shape/GB2ShapeCache.h"
#include "XMBox2d/Box2D.h"
#include "cocoa/CCNS.h"
#include "cocoa/CCString.h"
#include "cocoa/CCDictionary.h"

NS_CC_BEGIN

/**
 * Internal class to hold the fixtures
 */
class FixtureDef 
{
	public :

		FixtureDef ( KDvoid )
		{
			next = KD_NULL;
		}
	    
		virtual ~FixtureDef ( KDvoid )
		{
			delete next;
			delete fixture.shape;
		}
	    
		FixtureDef*		next;
		b2FixtureDef	fixture;
		KDint			callbackData;
};

class BodyDef 
{
	public :

		BodyDef ( KDvoid )		
		{
			fixtures = KD_NULL;
		}
		
		virtual ~BodyDef ( KDvoid ) 
		{
			CC_SAFE_DELETE ( fixtures )
		}
		
		FixtureDef*		fixtures;
		CCPoint			anchorPoint;
};

static GB2ShapeCache*	l_pSharedGB2ShapeCache = KD_NULL;

GB2ShapeCache* GB2ShapeCache::sharedGB2ShapeCache ( KDvoid )
{
	if ( !l_pSharedGB2ShapeCache )
	{
		l_pSharedGB2ShapeCache = new GB2ShapeCache ( );
        l_pSharedGB2ShapeCache->init ( );
	}
	
	return l_pSharedGB2ShapeCache;
}

KDbool GB2ShapeCache::init ( KDvoid )
{
	return KD_TRUE;
}

KDvoid GB2ShapeCache::reset ( KDvoid )
{
	std::map<std::string, BodyDef*>::iterator  iter;
	for ( iter = m_aShapeObjects.begin ( ); iter != m_aShapeObjects.end ( ); ++iter )
	{
		delete iter->second;
	}
	m_aShapeObjects.clear ( );
}

KDfloat GB2ShapeCache::getPtmRatio ( KDvoid )
{
	return m_fPtmRatio;
}

KDvoid GB2ShapeCache::addFixturesToBody ( b2Body* pBody, const std::string& sShape )
{
	std::map<std::string, BodyDef*>::iterator  iter = m_aShapeObjects.find ( sShape );
	CCAssert ( iter != m_aShapeObjects.end ( ), "" );
	
	BodyDef*	 pSo  = ( *iter ).second;
	FixtureDef*  pFix = pSo->fixtures;
    while ( pFix )
	{
        pBody->CreateFixture ( &pFix->fixture );
        pFix = pFix->next;
    }
}

CCPoint GB2ShapeCache::anchorPointForShape ( const std::string& sShape )
{
	std::map<std::string, BodyDef*>::iterator  iter = m_aShapeObjects.find ( sShape );
	CCAssert ( iter != m_aShapeObjects.end ( ), "" );
	
	BodyDef*  pBodyDef = ( *iter ).second;
	return pBodyDef->anchorPoint;
}


KDvoid GB2ShapeCache::addShapesWithFile ( const std::string& sPlist ) 
{
    CCDictionary*  pDict = CCDictionary::createWithContentsOfFile ( sPlist.c_str ( ) );
	CCAssert ( pDict != KD_NULL, "Shape-file not found" );    
    CCAssert ( pDict->count ( ) != 0, "plist file empty or not existing" );
	
	CCDictionary*  pMetadataDict = (CCDictionary*) pDict->objectForKey ( "metadata" );
    
    KDint  nFormat = static_cast<CCString*> ( pMetadataDict->objectForKey ( "format" ) )->intValue ( );
    m_fPtmRatio    = static_cast<CCString*> ( pMetadataDict->objectForKey ( "ptm_ratio" ) )->floatValue ( );
//	CCLOG ( "m_fPtmRatio = %f", m_fPtmRatio );
	CCAssert ( nFormat == 1, "Format not supported" );

	CCDictionary*	pBodyDict = (CCDictionary*) pDict->objectForKey ( "bodies" );

    b2Vec2			aVertices [ b2_maxPolygonVertices ];    
    CCDictElement*	pDictElem;
    std::string		sBodyName;
	CCDictionary*	pBodyData;

    // iterate body list
    CCDICT_FOREACH ( pBodyDict, pDictElem )
    {
        pBodyData = (CCDictionary*) pDictElem->getObject ( );
        sBodyName = pDictElem->getStrKey ( );
        
        
        BodyDef*	pBodyDef  = new BodyDef ( );
        pBodyDef->anchorPoint = CCPointFromString ( static_cast<CCString*> ( pBodyData->objectForKey ( "anchorpoint" ) )->getCString ( ) );
        CCArray*	pFixtureList = (CCArray*) ( pBodyData->objectForKey ( "fixtures" ) );
        FixtureDef**  pNextFixtureDef = &( pBodyDef->fixtures );
        
        //iterate fixture list
        CCObject*	pArrayElem;
        CCARRAY_FOREACH ( pFixtureList, pArrayElem )
        {
            b2FixtureDef	tBasicData;
            CCDictionary*	pFixtureData = (CCDictionary*) pArrayElem;
            
            tBasicData.filter.categoryBits	= static_cast<CCString*> ( pFixtureData->objectForKey ( "filter_categoryBits"	) )->intValue ( );            
            tBasicData.filter.maskBits		= static_cast<CCString*> ( pFixtureData->objectForKey ( "filter_maskBits"		) )->intValue ( );
            tBasicData.filter.groupIndex	= static_cast<CCString*> ( pFixtureData->objectForKey ( "filter_groupIndex"		) )->intValue ( );
            tBasicData.friction				= static_cast<CCString*> ( pFixtureData->objectForKey ( "friction"				) )->floatValue ( );            
            tBasicData.density				= static_cast<CCString*> ( pFixtureData->objectForKey ( "density"				) )->floatValue ( );            
            tBasicData.restitution			= static_cast<CCString*> ( pFixtureData->objectForKey ( "restitution"			) )->floatValue ( );            
			tBasicData.isSensor				= static_cast<CCString*> ( pFixtureData->objectForKey ( "isSensor"				) )->intValue ( ) ? KD_TRUE : KD_FALSE;
           
            CCString*		pCb				= static_cast<CCString*> ( pFixtureData->objectForKey ( "userdataCbValue" ) );            
            KDint			nCallbackData	= 0;

			if ( pCb )
			{
				nCallbackData = pCb->intValue ( );
			}

			std::string		sFixtureType	= static_cast<CCString*> ( pFixtureData->objectForKey ( "fixture_type" ) )->m_sString;

			if ( sFixtureType == "POLYGON" ) 
			{
				CCArray*	pPolygonsArray = (CCArray*) ( pFixtureData->objectForKey ( "polygons" ) );			
                CCObject*	pDicArrayElem;
                CCARRAY_FOREACH ( pPolygonsArray, pDicArrayElem )
                {
                    FixtureDef*		pFix = new FixtureDef ( );
                    pFix->fixture		 = tBasicData; // copy basic data
                    pFix->callbackData	 = nCallbackData;
                    
                    b2PolygonShape*	pPolyshape = new b2PolygonShape ( );
                    KDint			nVindex = 0;
                    
                    CCArray*		pPolygonArray = (CCArray*) pDicArrayElem;
                    
                    CCAssert ( pPolygonArray->count ( ) <= b2_maxPolygonVertices, "" );
                    
                    CCObject*		pIter;
                    CCARRAY_FOREACH ( pPolygonArray, pIter )
                    {
                        CCString*	pVerStr = (CCString*) pIter;
                        CCPoint		tOffset = CCPointFromString ( pVerStr->getCString ( ) );
                        aVertices [ nVindex ].x = ( tOffset.x / m_fPtmRatio );
                        aVertices [ nVindex ].y = ( tOffset.y / m_fPtmRatio );
                        nVindex++;
                    }
                    
                    pPolyshape->Set ( aVertices, nVindex );
                    pFix->fixture.shape = pPolyshape;
                    
                    // create a list
                    *pNextFixtureDef = pFix;
                     pNextFixtureDef = &( pFix->next );
                }
			}
            else if ( sFixtureType == "CIRCLE" )
			{
				FixtureDef*		pFix = new FixtureDef ( );
                pFix->fixture		 = tBasicData; // copy basic data
                pFix->callbackData	 = nCallbackData;

                CCDictionary*	pCircleData = (CCDictionary*) pFixtureData->objectForKey ( "circle" );
                b2CircleShape*	pCircleShape = new b2CircleShape ( );
				
                pCircleShape->m_radius = static_cast<CCString*> ( pCircleData->objectForKey ( "radius" ) )->floatValue ( ) / m_fPtmRatio;
				CCPoint			tPos = CCPointFromString ( static_cast<CCString*> ( pCircleData->objectForKey ( "position" ) )->getCString ( ) );
                pCircleShape->m_p = b2Vec2 ( tPos.x / m_fPtmRatio, tPos.y / m_fPtmRatio );
                pFix->fixture.shape = pCircleShape;
				
                // create a list
                *pNextFixtureDef = pFix;
                 pNextFixtureDef = &( pFix->next );

			}
            else 
			{
				CCAssert ( 0, "Unknown fixtureType" );
			}
		}
        // add the body element to the hash
        m_aShapeObjects [ sBodyName ] = pBodyDef;

    }
}

NS_CC_END