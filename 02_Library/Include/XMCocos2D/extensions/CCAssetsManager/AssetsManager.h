/* --------------------------------------------------------------------------
 *
 *      File            AssetsManager.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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

#ifndef __AssetsManager_h__
#define __AssetsManager_h__

#include "../../ccMacros.h"
#include "XMCurl/curl.h"
#include <string>

NS_CC_BEGIN

/** 
 *  This class is used to auto update resources, such as pictures or scripts.
 *  The updated package should be a zip file. And there should be a file named
 *  version in the server, which contains version code.
 */
class AssetsManager
{
	public :

		enum ErrorCode
		{
			ServerNotAvailable,    /** server address error or timeout */
			TimeOut,
		};
	    
		//! Default constructor. You should set server address later.
		AssetsManager ( KDvoid );
	    
		/**
		 *	@brief Creates a AssetsManager with new package url and version code url.
		 *         AssetsManager will use the value returned by CCFileUtils::getWritablePath() as storage path.
		 *
		 *	@param packageUrl URL of new package, the package should be a zip file.
		 *	@param versionFileUrl URL of version file. It should contain version code of new package.
		 */
		AssetsManager ( const KDchar* szPackageUrl, const KDchar* szVersionFileUrl );
	    
		/**
		 *	@brief Creates a AssetsManager with new package url, version code url and storage path.
		 *
		 *	@param packageUrl URL of new package, the package should be a zip file.
		 *	@param versionFileUrl URL of version file. It should contain version code of new package.
		 *	@param storagePath The path to store downloaded resources.
		 */
		AssetsManager ( const KDchar* szPackageUrl, const KDchar* szVersionFileUrl, const KDchar* szStoragePath );
	    
		/**
		 *	@brief Check out if there is a new version resource.
		 *         You may use this method before updating, then let user determine whether
		 *         he wants to update resources.
		 */
		virtual KDbool		checkUpdate ( KDvoid );
	    
		/** 
		 *	@brief Download new package if there is a new version, and uncompress downloaded zip file.
		 *         Ofcourse it will set search path that stores downloaded files.
		 */
		virtual KDvoid		update ( KDvoid );
	    
		/** 
		 *	@brief Gets url of package.
		 */
		const KDchar*		getPackageUrl ( KDvoid ) const;
	    
		/**
		 *	@brief Sets package url.
		 */
		KDvoid				setPackageUrl ( const KDchar* szPackageUrl );
	    
		/** 
		 *	@brief Gets version file url.
		 */
		const KDchar*		getVersionFileUrl ( KDvoid ) const;
	    
		/**
		 *	@brief Gets version file url.
		 */
		KDvoid				setVersionFileUrl ( const KDchar* szVersionFileUrl );
	    
		/**
		 *	@brief Gets current version code.
		 */
		std::string			getVersion ( KDvoid );
	    
		/**
		 *	@brief Deletes recorded version code.
		 */
		KDvoid				deleteVersion ( KDvoid );
	    
		/**
		 *	@brief Gets storage path.
		 */
		const KDchar*		getStoragePath ( KDvoid ) const;
	    
		/**
		 *	@brief Sets storage path.
		 *
		 *	@param storagePath The path to store downloaded resources.
		 *	@warm The path should be a valid path.
		 */
		KDvoid				setStoragePath ( const KDchar* szStoragePath );
	    
	protected :

		KDbool				downLoad ( KDvoid );

		KDvoid				checkStoragePath ( KDvoid );

		KDbool				uncompress_ ( KDvoid );

		KDbool				createDirectory ( const KDchar* szPath );

		KDvoid				setSearchPath ( KDvoid );
	    
	private :

		//! The path to store downloaded resources.
		std::string			m_sStoragePath;
	    
		//! The version of downloaded resources.
		std::string			m_sVersion;
	    
		std::string			m_sPackageUrl;
		std::string			m_sVersionFileUrl;
	    
		CURL*				m_pCurl;
};

NS_CC_END

#endif	// __AssetsManager_h__
