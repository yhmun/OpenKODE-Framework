/* --------------------------------------------------------------------------
 *
 *      File            AssetsManager.cpp
 *      Author          Y.H Mun
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

#include "Precompiled.h"
#include "extensions/CCAssetsManager/AssetsManager.h"
#include "support/CCFileUtils.h"
#include "support/user_default/CCUserDefault.h"
#include <XMZlib/contrib/minizip/unzip.h>
#include <XMCurl/curl.h>
#include <XMCurl/easy.h>

NS_CC_BEGIN

#define KEY_OF_VERSION					"current-version-code"
#define KEY_OF_DOWNLOADED_VERSION		"downloaded-version-code"
#define TEMP_PACKAGE_FILE_NAME			"cocos2dx-update-temp-package.zip"
#define BUFFER_SIZE						8192
#define MAX_FILENAME					512

AssetsManager::AssetsManager ( KDvoid )
{
	m_sPackageUrl		= "";
	m_sVersionFileUrl	= "";
	m_sVersion			= "";
	m_pCurl				= KD_NULL;
    m_sStoragePath		= CCFileUtils::sharedFileUtils ( )->getWritablePath ( );
    checkStoragePath ( );
}

AssetsManager::AssetsManager ( const KDchar* szPackageUrl, const KDchar* szVersionFileUrl )
{
	m_sPackageUrl		= szPackageUrl;
	m_sVersionFileUrl	= szVersionFileUrl;
	m_sVersion			= "";
	m_pCurl				= KD_NULL;
    m_sStoragePath		= CCFileUtils::sharedFileUtils ( )->getWritablePath ( );
    checkStoragePath ( );
}

AssetsManager::AssetsManager ( const KDchar* szPackageUrl, const KDchar* szVersionFileUrl, const KDchar* szStoragePath )
{
	m_sPackageUrl		= szPackageUrl;
	m_sVersionFileUrl	= szVersionFileUrl;
	m_sVersion			= "";
	m_pCurl				= KD_NULL;
	m_sStoragePath		= szStoragePath;
    checkStoragePath ( );
}

KDvoid AssetsManager::checkStoragePath ( KDvoid )
{
    if ( m_sStoragePath.size ( ) > 0 && m_sStoragePath [ m_sStoragePath.size ( ) - 1 ] != '/' )
    {
        m_sStoragePath.append ( "/" );
    }
}

static KDsize getVersionCode ( KDvoid* pPtr, KDsize uSize, KDsize uNmemb, KDvoid* pUserdata )
{
	std::string*  pVersion = (std::string*) pUserdata;
    pVersion->append ( (KDchar*) pPtr, uSize * uNmemb );
    
    return ( uSize * uNmemb );
}

KDbool AssetsManager::checkUpdate ( KDvoid )
{
    if ( m_sVersionFileUrl.size ( ) == 0 )
	{
		return KD_FALSE;
	}
    
    m_pCurl = curl_easy_init ( );
    if ( !m_pCurl )
    {
        CCLOG ( "can not init curl" );
        return KD_FALSE;
    }
    
    // Clear m_sVersion before assign new value.
    m_sVersion.clear ( );
    
    CURLcode  eRes;
    curl_easy_setopt ( m_pCurl, CURLOPT_URL, m_sVersionFileUrl.c_str ( ) );
    curl_easy_setopt ( m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L );
    curl_easy_setopt ( m_pCurl, CURLOPT_WRITEFUNCTION, getVersionCode );
    curl_easy_setopt ( m_pCurl, CURLOPT_WRITEDATA, &m_sVersion );
    eRes = curl_easy_perform ( m_pCurl );
    
    if ( eRes != 0 )
    {
        CCLOG ( "can not get version file content, error code is %d", eRes );
        curl_easy_cleanup ( m_pCurl );
        return KD_FALSE;
    }
    
	std::string  sRecordedVersion = CCUserDefault::sharedUserDefault ( )->getStringForKey ( KEY_OF_VERSION );
    if ( sRecordedVersion == m_sVersion )
    {
        CCLOG ( "there is not new version" );
        // Set resource search path.
        setSearchPath ( );
        return KD_FALSE;
    }
    
    CCLOG ( "there is a new version: %s", m_sVersion.c_str ( ) );
    
    return KD_TRUE;
}

KDvoid AssetsManager::update ( KDvoid )
{
    // 1. Urls of package and version should be valid;
    // 2. Package should be a zip file.
    if ( m_sVersionFileUrl.size ( ) == 0 ||
         m_sPackageUrl.size ( ) == 0 ||
         std::string::npos == m_sPackageUrl.find ( ".zip" ) )
    {
        CCLOG ( "no version file url, or no package url, or the package is not a zip file" );
        return;
    }
    
    // Check if there is a new version.
    if ( !checkUpdate ( ) )
	{
		return;
	}
    
    // Is package already downloaded?
	std::string  sDownloadedVersion = CCUserDefault::sharedUserDefault ( )->getStringForKey ( KEY_OF_DOWNLOADED_VERSION );
    if ( sDownloadedVersion != m_sVersion )
    {
        if ( !downLoad ( ) )
		{
			return;
		}
        
        // Record downloaded version.
        CCUserDefault::sharedUserDefault ( )->setStringForKey ( KEY_OF_DOWNLOADED_VERSION, m_sVersion.c_str ( ) );
        CCUserDefault::sharedUserDefault ( )->flush ( );
    }
    
    // Uncompress zip file.
    if ( !uncompress_ ( ) )
	{
		return;
	}
    
    // Record new version code.
    CCUserDefault::sharedUserDefault ( )->setStringForKey ( KEY_OF_VERSION, m_sVersion.c_str ( ) );
    
    // Unrecord downloaded version code.
    CCUserDefault::sharedUserDefault ( )->setStringForKey ( KEY_OF_DOWNLOADED_VERSION, "" );
    
    CCUserDefault::sharedUserDefault ( )->flush ( );
    
    // Set resource search path.
    setSearchPath ( );
    
    // Delete unloaded zip file.
	std::string  sZipfileName = m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
    if ( kdRemove ( sZipfileName.c_str ( ) ) != 0 )
    {
        CCLOG ( "can not remove downloaded zip file" );
    }
}

KDbool AssetsManager::uncompress_ ( KDvoid )
{
    // Open the zip file
	std::string  sOutFileName = m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
    unzFile      pZipFile = unzOpen ( sOutFileName.c_str ( ) );
    if ( !pZipFile )
    {
        CCLOG ( "can not open downloaded zip file %s", sOutFileName.c_str ( ) );
        return KD_FALSE;
    }
    
    // Get info about the zip file
    unz_global_info  tGlobalInfo;
    if ( unzGetGlobalInfo ( pZipFile, &tGlobalInfo ) != UNZ_OK )
    {
        CCLOG ( "can not read file global info of %s", sOutFileName.c_str ( ) );
        unzClose ( pZipFile );
    }
    
    // Buffer to hold data read from the zip file
    KDchar  aReadBuffer [ BUFFER_SIZE ];
    
    CCLOG ( "start uncompressing" );
    
    // Loop to extract all files.
    uLong i;
    for ( i = 0; i < tGlobalInfo.number_entry; ++i )
    {
        // Get info about current file.
        unz_file_info  tFileInfo;
        KDchar  szFileName[MAX_FILENAME];

        if ( unzGetCurrentFileInfo ( pZipFile,
                                   &tFileInfo,
                                   szFileName,
                                   MAX_FILENAME,
                                   NULL,
                                   0,
                                   NULL,
                                   0 ) != UNZ_OK )
        {
            CCLOG ( "can not read file info" );
            unzClose ( pZipFile );
            return KD_FALSE;
        }
        
		std::string  sFullPath = m_sStoragePath + szFileName;
        
        // Check if this entry is a directory or a file.
        const KDsize  uFilenameLength = kdStrlen ( szFileName );
        if ( szFileName [ uFilenameLength - 1 ] == '/' )
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if ( !createDirectory ( sFullPath.c_str ( ) ) )
            {
                CCLOG ( "can not create directory %s", sFullPath.c_str ( ) );
                unzClose ( pZipFile );
                return KD_FALSE;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if ( unzOpenCurrentFile ( pZipFile ) != UNZ_OK )
            {
                CCLOG ( "can not open file %s", szFileName );
                unzClose ( pZipFile );
                return KD_FALSE;
            }
            
            // Create a file to store current file.
            KDFile*  pOut = kdFopen ( sFullPath.c_str ( ), "wb" );
            if ( !pOut )
            {
                CCLOG ( "can not open destination file %s", sFullPath.c_str ( ) );
                unzCloseCurrentFile ( pZipFile );
                unzClose ( pZipFile );
                return KD_FALSE;
            }
            
            // Write current file content to destinate file.
            KDint  nError = UNZ_OK;
            do
            {
                nError = unzReadCurrentFile ( pZipFile, aReadBuffer, BUFFER_SIZE );
                if ( nError < 0 )
                {
                    CCLOG ( "can not read zip file %s, error code is %d", szFileName, nError );
                    unzCloseCurrentFile ( pZipFile );
                    unzClose( pZipFile );
                    return KD_FALSE;
                }
                
                if ( nError > 0 )
                {
                    kdFwrite ( aReadBuffer, nError, 1, pOut );
                }

            } while ( nError > 0 );
            
            kdFclose ( pOut );
        }
        
        unzCloseCurrentFile ( pZipFile );
        
        // Goto next entry listed in the zip file.
        if ( ( i + 1 ) < tGlobalInfo.number_entry )
        {
            if ( unzGoToNextFile ( pZipFile ) != UNZ_OK )
            {
                CCLOG ( "can not read next file" );
                unzClose ( pZipFile );
                return KD_FALSE;
            }
        }
    }
    
    CCLOG ( "end uncompressing" );
    
    return KD_TRUE;
}

//
// Create a direcotry is platform depended.
//
KDbool AssetsManager::createDirectory ( const KDchar* szPath )
{   
    KDint  nRet = kdMkdir ( szPath );
    
    if ( nRet != 0 && ( kdGetError ( ) != KD_EEXIST ) )
    {
        return KD_FALSE;
    }
    
    return KD_TRUE;
}

KDvoid AssetsManager::setSearchPath ( KDvoid )
{
	std::vector<std::string>   aSearchPaths = CCFileUtils::sharedFileUtils ( )->getSearchPaths ( );
	std::vector<std::string>::iterator iter = aSearchPaths.begin ( );
    aSearchPaths.insert ( iter, m_sStoragePath );
    CCFileUtils::sharedFileUtils ( )->setSearchPaths ( aSearchPaths );
}

static KDsize downLoadPackage ( KDvoid* pPtr, KDsize uSize, KDsize uNmemb, KDvoid* pUserdata )
{
    KDFile*  pFile = (KDFile*) pUserdata;
    KDsize   uWritten = kdFwrite ( pPtr, uSize, uNmemb, pFile );
    return uWritten;
}

static KDint progressFunc ( KDvoid* pPtr, KDdouble dTotalToDownload, KDdouble dNowDownloaded, KDdouble dTotalToUpLoad, KDdouble dNowUpLoaded )
{
    CCLOG ( "downloading... %d%%", (KDint) ( dNowDownloaded / dTotalToDownload * 100 ) );
    
    return 0;
}

KDbool AssetsManager::downLoad ( KDvoid )
{
    // Create a file to save package.
	std::string  sOutFileName = m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
    KDFile*  pFile = kdFopen ( sOutFileName.c_str ( ), "wb" );
    if ( !pFile )
    {
        CCLOG ( "can not create file %s", sOutFileName.c_str ( ) );
        return KD_FALSE;
    }
    
    // Download pacakge
    CURLcode   eRes;
    curl_easy_setopt ( m_pCurl, CURLOPT_URL, m_sPackageUrl.c_str ( ) );
    curl_easy_setopt ( m_pCurl, CURLOPT_WRITEFUNCTION, downLoadPackage );
    curl_easy_setopt ( m_pCurl, CURLOPT_WRITEDATA, pFile );
    curl_easy_setopt ( m_pCurl, CURLOPT_NOPROGRESS, KD_FALSE );
    curl_easy_setopt ( m_pCurl, CURLOPT_PROGRESSFUNCTION, progressFunc );
    eRes = curl_easy_perform ( m_pCurl );
    curl_easy_cleanup ( m_pCurl );
    if ( eRes != 0 )
    {
        CCLOG ( "error when download package" );
        kdFclose ( pFile );
        return KD_FALSE;
    }
    
    CCLOG ( "succeed downloading package %s", m_sPackageUrl.c_str ( ) );
    
    kdFclose ( pFile );
    return KD_TRUE;
}

const KDchar* AssetsManager::getPackageUrl ( KDvoid ) const
{
    return m_sPackageUrl.c_str ( );
}

KDvoid AssetsManager::setPackageUrl ( const KDchar* szPackageUrl )
{
    m_sPackageUrl = szPackageUrl;
}

const KDchar* AssetsManager::getStoragePath ( KDvoid ) const
{
    return m_sStoragePath.c_str ( );
}

KDvoid AssetsManager::setStoragePath ( const KDchar* szStoragePath )
{
    m_sStoragePath = szStoragePath;
    checkStoragePath ( );
}

const KDchar* AssetsManager::getVersionFileUrl ( KDvoid ) const
{
    return m_sVersionFileUrl.c_str ( );
}

KDvoid AssetsManager::setVersionFileUrl ( const KDchar* szVersionFileUrl )
{
    m_sVersionFileUrl = szVersionFileUrl;
}

std::string AssetsManager::getVersion ( KDvoid )
{
    return CCUserDefault::sharedUserDefault ( )->getStringForKey ( KEY_OF_VERSION );
}

KDvoid AssetsManager::deleteVersion ( KDvoid )
{
    CCUserDefault::sharedUserDefault ( )->setStringForKey ( KEY_OF_VERSION, "" );
}

NS_CC_END
