/* -----------------------------------------------------------------------------------
 *
 *      File            Cloud.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "Cloud.h"
#include "Object/Sprite/Sprite.h"

cCloud::cCloud ( JBaseScene* pScene )
: m_pScene			( pScene	)
, m_pSprCloudBackup	( nullptr	)
{
	// sprite
	m_pSprCloudBackup = new cSprite ( "cloud.png", 0, Point::ZERO );
	m_pSprCloudBackup->setIsVisible ( false );
}

cCloud::~cCloud ( KDvoid )
{
	// sprite
	m_pSprCloudBackup = nullptr;
}

KDvoid cCloud::add ( KDint z, KDdouble dStartDelay, Point tPoint, Point tPointAdd, KDdouble dLeftTime, KDfloat fScale, KDfloat fScaleAdd, KDfloat fRotation, KDfloat fRotationAdd, KDbool bIsFlipX )
{
	// init
	sCloudObj		tCloudObj;
	
	//set
	tCloudObj.pSpr			= new cSprite ( "cloud.png", z, tPoint );
	tCloudObj.dStartDelay	= dStartDelay;
	tCloudObj.tPoint		= tPoint;
	tCloudObj.tPointAdd		= tPointAdd;
	tCloudObj.dTime			= dLeftTime;
	tCloudObj.dLeftTime		= dLeftTime;
	tCloudObj.fScale		= fScale;
	tCloudObj.fScaleAdd		= fScaleAdd;
	tCloudObj.fRotation		= fRotation;
	tCloudObj.fRotationAdd	= fRotationAdd;

	//sprite set
	tCloudObj.pSpr->setAnchorPoint	( Point ( 0.5f, 0.5f )  );
	tCloudObj.pSpr->setIsVisible	( false					);
	tCloudObj.pSpr->setScale		( fScale				);
	tCloudObj.pSpr->setRotation		( fRotation				);
	tCloudObj.pSpr->setOpacity		( 255					);
	tCloudObj.pSpr->setIsFlipX		( bIsFlipX				);
	
	// draw obj add
	m_pScene->addDrawObject ( tCloudObj.pSpr );
	
	// add
	m_aClouds.push_back ( tCloudObj );
}

KDint cCloud::getCloudCount ( KDvoid )
{
	return (KDint) m_aClouds.size ( );
}

KDvoid cCloud::update ( KDdouble dLeftTime )
{
	// loop
	std::vector<sCloudObj>::iterator	pNode = m_aClouds.begin ( );
	while ( pNode != m_aClouds.end ( ) )
	{
		// init
		sCloudObj&		pObj = *pNode;
	
		// start delay check
		if ( pObj.dStartDelay > 0 )
		{
			// left time
			pObj.dStartDelay -= dLeftTime;

			// next
			++pNode;

			// continue;
			continue;
		}

		// visible
		pObj.pSpr->setIsVisible ( true );
		
		// time
		pObj.dLeftTime -= dLeftTime;
		
		// time check
		if ( pObj.dLeftTime < 0 )
		{
			pNode = m_aClouds.erase ( pNode );
			continue;
		}
		
		// percent
		KDfloat		fPercent = (pObj.dTime - pObj.dLeftTime) / pObj.dTime;

		// point
		pObj.pSpr->setPoint ( pObj.tPoint + pObj.tPointAdd * fPercent );
		
		// opacity
		if ( fPercent > 0.5f )	pObj.pSpr->setOpacity ( 255 - ( ( fPercent - 0.5f ) * 2 * 255 ) );
		else					pObj.pSpr->setOpacity ( 255 );
		
		// scale
		pObj.pSpr->setScale ( pObj.fScale + pObj.fScaleAdd*fPercent );
		
		// rotation
		pObj.pSpr->setRotation ( pObj.fRotation + pObj.fRotationAdd * fPercent );
		
		// next
		++pNode;
	}
}
