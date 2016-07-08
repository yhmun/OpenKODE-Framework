/* -----------------------------------------------------------------------------------
 *
 *      File            AssetsManager.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __AssetsManager_h__
#define __AssetsManager_h__

#include <string>
#include <mutex>

#include "../ExtensionMacros.h"
#include "../../2d/base_nodes/CCNode.h"

NS_CC_EXT_BEGIN

class AssetsManagerDelegateProtocol;

/**
 *  This class is used to auto update resources, such as pictures or scripts.
 *  The updated package should be a zip file. And there should be a file named
 *  version in the server, which contains version code.
 */
class AssetsManager : public Node
{
	public :

		enum class ErrorCode
		{
			// Error caused by creating a file to store downloaded data
			CREATE_FILE,
			/**
			 *	Error caused by network
			 *		-- network unavaivable
			 *		-- timeout
			 *		-- ...
			 */
			NETWORK,
			/** 
			 *	There is not a new version
			 */
			NO_NEW_VERSION,
			/** 
			 *	Error caused in uncompressing stage
			 *		-- can not open zip file
			 *		-- can not read file global information
			 *		-- can not read file information
			 *		-- can not create a directory
			 *		-- ...
			 */
			UNCOMPRESS,
		};
    
		/**
		 *	@brief Creates a AssetsManager with new package url, version code url and storage path.
		 *
		 *	@param packageUrl URL of new package, the package should be a zip file.
		 *	@param versionFileUrl URL of version file. It should contain version code of new package.
		 *	@param storagePath The path to store downloaded resources.
		 *	@js NA
		 */
		AssetsManager ( const KDchar* pPackageUrl = KD_NULL, const KDchar* pVersionFileUrl = KD_NULL, const KDchar* pStoragePath = KD_NULL );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~AssetsManager ( KDvoid );
    
		typedef std::function<KDvoid(KDint)>	ErrorCallback;
		typedef std::function<KDvoid(KDint)>	ProgressCallback;
		typedef std::function<KDvoid(KDvoid)>	SuccessCallback;

		/** 
		 *	@brief To access within scripting environment
		 */
		static AssetsManager*			create ( const KDchar* pPackageUrl, const KDchar* pVersionFileUrl, const KDchar* pStoragePath, ErrorCallback pErrorCallback, ProgressCallback pProgressCallback, SuccessCallback pSuccessCallback );

		/**
		 *	@brief Check out if there is a new version resource.
		 *         You may use this method before updating, then let user determine whether
		 *         he wants to update resources.
		 */
		virtual KDbool					checkUpdate ( KDvoid );
    
		using Node::update;

		/**
		 *	@brief Download new package if there is a new version, and uncompress downloaded zip file.
		 *         Ofcourse it will set search path that stores downloaded files.
		 */
		virtual KDvoid					update ( KDvoid );
    
		/**
		 *	@brief Gets url of package.
		 */
		const KDchar*					getPackageUrl ( KDvoid ) const;
    
		/**
		 *	@brief Sets package url.
		 */
		KDvoid							setPackageUrl ( const KDchar* pPackageUrl );
    
		/**
		 *	@brief Gets version file url.
		 */
		const KDchar*					getVersionFileUrl ( KDvoid ) const;
    
		/**
		 *	@brief Gets version file url.
		 */
		KDvoid							setVersionFileUrl ( const KDchar* pVersionFileUrl );
    
		/**
		 *	@brief Gets current version code.
		 */
		std::string						getVersion ( KDvoid );
    
		/**
		 *	@brief Deletes recorded version code.
		 */
		KDvoid							deleteVersion ( KDvoid );
    
		/**
		 *	@brief Gets storage path.
		 */
		const KDchar*					getStoragePath ( KDvoid ) const;
    
		/**
		 *	@brief Sets storage path.
		 *
		 *	@param storagePath The path to store downloaded resources.
		 *	@warm The path should be a valid path.
		 */
		KDvoid							setStoragePath ( const KDchar* pStoragePath );
    
		/** 
		 *	@brief Sets delegate, the delegate will receive messages
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid							setDelegate ( AssetsManagerDelegateProtocol* pDelegate );
    
		/**
		 *	@js NA
		 *	@lua NA
		 */
		AssetsManagerDelegateProtocol*	getDelegate ( KDvoid ) const { return m_pDelegate; }
    
		/**
		 *	@brief Sets connection time out in seconds
		 */
		KDvoid							setConnectionTimeout ( KDuint uTimeout );
    
		/**
		 *	@brief Gets connection time out in secondes
		 */
		KDuint							getConnectionTimeout ( KDvoid );
    
		/** 
		 *	downloadAndUncompress is the entry of a new thread 
		 */
		friend KDint					assetsManagerProgressFunc ( KDvoid*, KDdouble, KDdouble, KDdouble, KDdouble );

	protected :

		KDbool							downLoad ( KDvoid );
		KDvoid							checkStoragePath ( KDvoid );
		KDbool							uncompress_ ( KDvoid );
		KDbool							createDirectory ( const KDchar* pPath );
		KDvoid							setSearchPath ( KDvoid );
		KDvoid							sendErrorMessage ( ErrorCode eCode );
		KDvoid							downloadAndUncompress ( KDvoid );
    
	private :

		typedef struct _Message
		{
			public :

				_Message ( KDvoid ) : what ( 0 ), obj ( KD_NULL ) { }

				KDuint					what; // message type
				KDvoid*					obj;
		} Message;
    
		class Helper : public cocos2d::Object
		{
			public :

				/**
				 *	@js ctor
				 */
				Helper ( KDvoid );

				/**
				 *	@js NA
				 *	@lua NA
				 */
				~Helper ( KDvoid );
        
				virtual KDvoid			update ( KDfloat dt );
				KDvoid					sendMessage ( Message* pMsg );
        
			private :

				KDvoid					handleUpdateSucceed ( Message* pMsg );
        
				std::list<Message*>*	m_pMessageQueue;
				std::mutex				m_tMessageQueueMutex;
		};

	private :

		/** 
		 *	@brief Initializes storage path.
		 */
		KDvoid							createStoragePath ( KDvoid );
    
		/**
		 *	@brief Destroys storage path.
		 */
		KDvoid							destroyStoragePath ( KDvoid );
    
	private :

		//! The path to store downloaded resources.
		std::string						m_sStoragePath;
    
		//! The version of downloaded resources.
		std::string						m_sVersion;
    
		std::string						m_sPackageUrl;
		std::string						m_sVersionFileUrl;
    
		std::string						m_sDownloadedVersion;
    
		KDvoid*							m_pCurl;

		Helper*							m_pSchedule;
		KDuint							m_uConnectionTimeout;
    
		AssetsManagerDelegateProtocol*	m_pDelegate; 
    
		bool _isDownloading;
		bool _shouldDeleteDelegateWhenExit;
    
		std::string						keyOfVersion ( KDvoid ) const;
		std::string						keyOfDownloadedVersion ( KDvoid ) const;
};

class AssetsManagerDelegateProtocol
{
	public :

		virtual ~AssetsManagerDelegateProtocol ( KDvoid ) { }

	public :

		/**
		 *	@brief Call back function for error
		 *  @param errorCode Type of error
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onError ( AssetsManager::ErrorCode eErrorCode ) { }

		/**
		 *	@brief Call back function for recording downloading percent
		 *	@param percent How much percent downloaded
		 *	@warning    This call back function just for recording downloading percent.
		 *		  AssetsManager will do some other thing after downloading, you should
		 *		  write code in onSuccess() after downloading. 
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onProgress ( KDint nPercent ) { }

		/** 
		 *	@brief Call back function for success
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid					onSuccess ( KDvoid ) { }
};

NS_CC_EXT_END;

#endif	// __AssetsManager_h__
