/* --------------------------------------------------------------------------
 *
 *      File            APSDeviceInfo.cpp
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
#include "extensions/CCArtPig/APSDeviceInfo.h"
#include "support/CCDirector.h"

NS_APS_BEGIN

const std::string& APSDeviceInfo::getMainModelIndexKey ( KDvoid )
{
    static const std::string  sKey = "mainModelIndex";
    return sKey;
}

const std::string& APSDeviceInfo::getDevicesKey ( KDvoid )
{
    static const std::string  sKey = "devices";
    return sKey;
}

APSDeviceInfo::APSDeviceInfo ( APSDictionary* pProperties )
{
	m_pAppliedDevice	= KD_NULL;
	m_uMainModelIndex	= 0;
	m_pDevices			= new std::vector<APSDevice*>;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

APSDeviceInfo::APSDeviceInfo ( const APSDeviceInfo& tDeviceInfo ) 
{
	m_pAppliedDevice	= KD_NULL;
	m_uMainModelIndex	= 0;
	m_pDevices			= new std::vector<APSDevice*>;

    this->m_uMainModelIndex = tDeviceInfo.getMainModelIndex();
    
	APS_FOREACH ( std::vector<APSDevice*>, *tDeviceInfo.getDevices ( ), iter )
	{
        APSDevice*  pDev = new APSDevice ( *(APSDevice*)*iter );
        this->m_pDevices->push_back ( pDev );
    }
}

APSDeviceInfo::~APSDeviceInfo ( KDvoid ) 
{
	APS_FOREACH ( std::vector<APSDevice*>, *this->m_pDevices, iter )
	{
        APS_SAFE_DELETE ( *iter );
    }
    APS_SAFE_DELETE ( this->m_pDevices );
}

KDbool APSDeviceInfo::initWithProperties ( APSDictionary* pProperties )
{    
    // set mainModelIndex
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getMainModelIndexKey ( ) );
    if ( pNumber )
	{
        this->setMainModelIndex ( pNumber->getInteger ( ) );
    }
    
    // set devices
    APSArray*  pDevices = (APSArray*) pProperties->getObjectForKey ( this->getDevicesKey ( ) );
    if ( pDevices )
	{
        APS_FOREACH ( APSArrayStorage, *pDevices, iter )
		{
            APSDictionary*  pDict = (APSDictionary*) *iter;

            // Allocate a device object with the class name 
            APSDevice*  pDV = new APSDevice;
            
            // initialize with the properties in the dictionary.
            if ( !pDV->initWithProperties ( pDict ) )
			{
                return KD_FALSE;
			}
            this->m_pDevices->push_back ( pDV );
        }
        
        // main model device is the default for applied device.
        this->setAppliedDevice ( this->getMainModelDevice ( ) );
    }
    
    return KD_TRUE;
}

APSDevice* APSDeviceInfo::getMainModelDevice ( KDvoid )
{
    return ( *this->m_pDevices ) [ this->getMainModelIndex ( ) ];
}

APSDevice* APSDeviceInfo::getMostFittingDevice ( KDvoid )
{
    CCSize  tWinSize = CCDirector::sharedDirector ( ) ->getWinSize ( );
    
    APSDevice*  pSelectedDevice = KD_NULL;
    KDdouble    dSelectedDistance = KD_FLT_MAX;
    
	APS_FOREACH ( std::vector<APSDevice*>, *this->m_pDevices, iter )
	{
        APSDevice*  pDev = (APSDevice*) *iter;
        if ( pDev->getEnabled ( ) ) 
		{
            CCSize    tSize = pDev->getScreenSize ( );
            KDdouble  dDistance = kdPow ( 1 - tSize.cx / tWinSize.cx, 2 ) + kdPow ( 1 - tSize.cy / tWinSize.cy, 2 );
            if ( dSelectedDistance > dDistance )
			{
                pSelectedDevice = pDev;
                dSelectedDistance = dDistance;
            }
        }
    }
    
    return pSelectedDevice;
}

KDfloat APSDeviceInfo::getScreenScaleForDevice ( APSDevice* pDevice )
{
    CCSize   tDeviceSize = pDevice->getScreenSize();
    CCSize   tModelSize  = this->getMainModelDevice ( )->getScreenSize ( );
    CCPoint  tRate = CCPoint ( tDeviceSize.cx / tModelSize.cx, tDeviceSize.cy / tModelSize.cy );
    
    // Choose the minimum scale change from model device size
    return kdFabsf ( 1 - tRate.x ) < kdFabsf ( 1 - tRate.y ) ? tRate.x : tRate.y;
    
}

NS_APS_END
