/* --------------------------------------------------------------------------
 *
 *      File            APSAnchor.cpp
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
#include "extensions/CCArtPig/APSAnchor.h"

NS_APS_BEGIN

const std::string& APSAnchor::getPositionKey ( KDvoid ) 
{
    static const std::string  sKey = "position";
    return sKey;
}

const std::string& APSAnchor::getBackHandleKey ( KDvoid )
{
    static const std::string  sKey = "backHandle";
    return sKey;
}

const std::string& APSAnchor::getFrontHandleKey ( KDvoid )
{
    static const std::string  sKey = "frontHandle";
    return sKey;
}

const std::string& APSAnchor::getProportionKey ( KDvoid ) 
{
    static const std::string  sKey = "proportion";
    return sKey;
}

APSAnchor::APSAnchor ( CCPoint tPosition, CCPoint tBackHandle, CCPoint tFrontHandle, KDfloat fProportion ) 
{
	m_tPosition		= tPosition;
	m_tBackHandle	= tBackHandle;
	m_tFrontHandle	= tFrontHandle;
	m_fProportion	= fProportion;    
}

APSAnchor::APSAnchor ( APSDictionary *pProperties ) 
{
	m_tPosition		= CCPointZero;
	m_tBackHandle	= CCPointZero;
	m_tFrontHandle	= CCPointZero;
	m_fProportion	= 1.f;    

    if ( pProperties ) 
	{
        this->initWithProperties ( pProperties );
    }
}

APSAnchor::APSAnchor ( const APSAnchor& tAnchor )
{
    this->m_tPosition	 = tAnchor.getPosition    ( );
    this->m_tBackHandle	 = tAnchor.getBackHandle  ( );
    this->m_tFrontHandle = tAnchor.getFrontHandle ( );
    this->m_fProportion	 = tAnchor.getProportion  ( );
}

KDbool APSAnchor::initWithProperties ( APSDictionary* pProperties ) 
{
    // set position
    APSString*  pStr = (APSString*) pProperties->getObjectForKey ( this->getPositionKey ( ) );
    if ( pStr )
	{
        this->setPosition ( pStr->getPointValue ( ) );
    }
    
    // set backHandle
    pStr = (APSString*) pProperties->getObjectForKey ( this->getBackHandleKey ( ) );
    if ( pStr )
	{
        this->setBackHandle ( pStr->getPointValue ( ) );
    }
    
    // set frontHandle
    pStr = (APSString*) pProperties->getObjectForKey ( this->getFrontHandleKey ( ) );
    if ( pStr )
	{
        this->setFrontHandle ( pStr->getPointValue ( ) );
    }
    
    // set proportion
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getProportionKey ( ) );
    if ( pNumber ) 
	{
        this->setProportion ( pNumber->getFloat ( ) );
    }
    
    return true;
}

CCPoint APSAnchor::getPosition ( KDfloat fScale )
{
    CCPoint  tP = this->getPosition ( );
    return CCPoint ( tP.x * fScale, tP.y * fScale );
}

CCPoint APSAnchor::getBackHandle ( KDfloat fScale ) 
{
    CCPoint  tP = this->getBackHandle ( );
    return CCPoint ( tP.x * fScale, tP.y * fScale );
}

CCPoint APSAnchor::getFrontHandle ( KDfloat fScale )
{
    CCPoint  tP = this->getFrontHandle ( );
    return CCPoint ( tP.x * fScale, tP.y * fScale );
}

NS_APS_END



