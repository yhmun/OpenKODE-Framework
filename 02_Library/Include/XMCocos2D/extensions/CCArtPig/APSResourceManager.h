/* --------------------------------------------------------------------------
 *
 *      File            APSResourceManager.h
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

#ifndef __APSResourceManager_h__
#define __APSResourceManager_h__

#include "APSElements.h"
#include "APSDeviceInfo.h"
#include "APSSymbol.h"

NS_APS_BEGIN

class APSMedium;
class APSSPriteFrame;

typedef std::map<const std::string, APSMedium*>  APSMediumMap;
typedef std::map<const std::string, APSSymbol*>  APSSymbolMap;

/** 
 *	@brief APSResourceManager
 *	APSResourceManager helps to create APSResource and APSMedium objects. 
 *	Each project can have one resource manager.
 *	APSSymbol objects can be created by resource manager, and properties 
 *	information used to initialize symbol is cached to reuse it when a symbol with 
 *	the same tag is created again.
 *
 *	If you create a symbol without resource manager, the symbol does not have 
 *	access to images. Therefore, CCSprite objects in symbol cannot be created
 *	with image textures.
 */
class APSResourceManager
{	
	public :

		APSResourceManager ( const KDchar* szProjectID = "", const KDchar* szLocation = KD_NULL );

		virtual ~APSResourceManager ( KDvoid );

		APS_CLASS_NAME ( APSResourceManager );

		/** 
		 *	Returns an APSResourceManager object with a given project id. If there
		 *	is no resource manager created yet with the project id, APSResourceManager
		 *	object is allocated and cached.
		 *	If files are not located in default resource path, location parameter should
		 *	be set.
		 */
		static APSResourceManager*  getResourceManagerForProjectId ( const KDchar* szProjectID, const KDchar* szLocation = KD_NULL );
	    
		/** Deallocates an APSResourceManager object with a given project id. */
		static KDvoid  removeResourceManagerForProjectId ( const std::string& sProjectID );
	    
		/** Deallocates all APSResourceManager objects. */
		static KDvoid  removeAllResourceManagers ( KDvoid );
	    
		/** Returns resource manager's XML filename for a given project id. */
		static std::string  getXMLFilenameForProjectId ( const std::string& sProjectID );
	            
		/** 
		 *	Allocates and returns a new APSResource object initialized by given
		 *	properties. 
		 *	@warning To allocate class object, properties must include an APSString 
		 *	object specifying class name.
		 */
		static APSResource*  newResourceWithProperties ( APSDictionary* pProperties, APSSymbol* pParentSymbol = KD_NULL );
	    
		/** Allocates and returns a new resource with a given class name. */
		static APSResource*  allocForClassName ( const std::string& sName );

	public :
	    
		/** Initializes resource manager with given properties. */
		virtual  KDbool  initWithProperties ( APSDictionary *properties );

		/**
		 *	Returns an APSMedium object with a given code in resource manager. 
		 *	If there is no APSMedium object with the code, NULL is returned.
		 */
		APSMedium*  getMediumForCode ( const std::string& sCode );
	    
		/**
		 *	Returns a scale value ( x and y ) which can make model screen size to fit
		 *	with the runtime device's screen size.
		 */
		CCPoint  getScaleModelToDevice ( APSOrientation nOrientation = kAPSOrientationLandscapeRight, KDbool bKeepRatio = KD_FALSE ) const;
	    
		/** 
		 *	Allocates and returns symbol with a given tag. If cacheProperties is 
		 *	true, symbol properties are cached and reused later newSymbol ( ) call.
		 */
		APSSymbol*  newSymbol ( const KDchar* szTag = kAPSDefaultSymbolTag, KDbool bApplyScale = KD_TRUE, KDbool bCacheProperties = KD_TRUE );
	    	    
		/** Returns full path for a given file name. */
		std::string getPathForFilename ( const KDchar* szFileName ) const;
	    
		KDvoid  addMedium ( APSMedium* pMedium );
	        
	protected :

		APS_SYNTHESIZE_WITH_KEY ( KDfloat, m_fVersion, Version );
	    
		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sProjectID, ProjectId );
	    
		/** 
		 *	A directory where symbol files are stored. If location property is not
		 *	set, default resource path is used. In case of IOS, location should be set
		 *	only when xml and images files are stored other than path returned by
		 *	[[NSBundle mainBundle] resourceURL].
		 */
		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sLocation, Location ); 

		APS_PROPERTY_KEY ( DeviceInfo );
		APS_PROPERTY_KEY ( Mediums );
	    
		APS_SYNTHESIZE_READONLY ( APSDeviceInfo*, m_pDeviceInfo, DeviceInfo );

	protected :

		APSMediumMap		m_aMediums;
		APSDictionary*		m_pSymbolProperties;
};

NS_APS_END

#endif	// __APSResourceManager_h__
