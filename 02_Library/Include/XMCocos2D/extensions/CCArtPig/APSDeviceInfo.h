/* --------------------------------------------------------------------------
 *
 *      File            APSDeviceInfo.h
 *      Author          Young-Hwan Mun
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

#ifndef __APSDeviceInfo_h__
#define __APSDeviceInfo_h__

#include "APSDevice.h"

NS_APS_BEGIN

class APSDeviceInfo
{
	public :
	    
		APSDeviceInfo ( APSDictionary* pProperties = KD_NULL );
	    
		APSDeviceInfo ( const APSDeviceInfo& tDeviceInfo );
	    
		virtual ~APSDeviceInfo ( KDvoid );

		APS_CLASS_NAME ( APSDeviceInfo );
	    
	public :

		KDbool   initWithProperties ( APSDictionary* pProperties );
	        
		/** Returns main model device in devices list. mainModelIndex property must be set before using this method. */
		APSDevice*  getMainModelDevice ( KDvoid );
	    
		/** Finds and returns APSDevice that the most matches to the runtime device. */
		APSDevice*  getMostFittingDevice ( KDvoid );
	    
		/** Returns an appropriate scale from model to a given device. */
		float getScreenScaleForDevice ( APSDevice *device );
    
	protected :
 
		/** Index of main model device in devices list. */
		APS_SYNTHESIZE_WITH_KEY ( KDuint, m_uMainModelIndex, MainModelIndex );
	    
		/** Applied device that is selected out of devices list in runtime. */
		APS_SYNTHESIZE_WITH_KEY ( APSDevice*, m_pAppliedDevice, AppliedDevice );
	    
		/** List of candiate devices. */
		APS_SYNTHESIZE_READONLY_WITH_KEY ( std::vector<APSDevice*>*, m_pDevices, Devices );
};
NS_APS_END

#endif	// __APSDeviceInfo_h__

