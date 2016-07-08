/* -----------------------------------------------------------------------------------
 *
 *      File            AssetsManager.cpp
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

#include <XMCurl/curl.h>
#include <XMCurl/easy.h>
#include "extensions/assets-manager/AssetsManager.h"
#include "support/user_default/CCUserDefault.h"
#include "platform/CCFileUtils.h"
#include "2d/CCDirector.h"
#include "2d/CCScheduler.h"
#include <vector>
#include <thread>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#endif

#include <XMZlib/contrib/minizip/unzip.h>

using namespace cocos2d;
using namespace std;

NS_CC_EXT_BEGIN;

#define KEY_OF_VERSION										"current-version-code"
#define KEY_OF_DOWNLOADED_VERSION							"downloaded-version-code"
#define TEMP_PACKAGE_FILE_NAME								"cocos2dx-update-temp-package.zip"
#define BUFFER_SIZE											8192
#define MAX_FILENAME										512

// Message type
#define ASSETSMANAGER_MESSAGE_UPDATE_SUCCEED                0
#define ASSETSMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION     1
#define ASSETSMANAGER_MESSAGE_PROGRESS                      2
#define ASSETSMANAGER_MESSAGE_ERROR                         3

// Some data struct for sending messages

struct ErrorMessage
{
    AssetsManager::ErrorCode	code;
    AssetsManager*				manager;
};

struct ProgressMessage
{
    KDint						percent;
    AssetsManager*				manager;
};

// Implementation of AssetsManager

AssetsManager::AssetsManager(const char* packageUrl/* =NULL */, const char* versionFileUrl/* =NULL */, const char* storagePath/* =NULL */)
:  m_sStoragePath(storagePath)
, m_sVersion("")
, m_sPackageUrl(packageUrl)
, m_sVersionFileUrl(versionFileUrl)
, m_sDownloadedVersion("")
, m_pCurl(NULL)
, m_uConnectionTimeout(0)
, m_pDelegate(NULL)
, _isDownloading(false)
, _shouldDeleteDelegateWhenExit(false)
{
    checkStoragePath();
    m_pSchedule = new Helper();
}

AssetsManager::~AssetsManager()
{
    if (m_pSchedule)
    {
        m_pSchedule->release();
    }
    if (_shouldDeleteDelegateWhenExit)
    {
        delete m_pDelegate;
    }
}

void AssetsManager::checkStoragePath()
{
    if (m_sStoragePath.size() > 0 && m_sStoragePath[m_sStoragePath.size() - 1] != '/')
    {
        m_sStoragePath.append("/");
    }
}

// Multiple key names
static std::string keyWithHash( const char* prefix, const std::string& url )
{
    char buf[256];
    kdSprintf(buf,"%s%zd",prefix,std::hash<std::string>()(url));
    return buf;
}

// hashed version
std::string AssetsManager::keyOfVersion() const
{
    return keyWithHash(KEY_OF_VERSION,m_sPackageUrl);
}

// hashed version
std::string AssetsManager::keyOfDownloadedVersion() const
{
    return keyWithHash(KEY_OF_DOWNLOADED_VERSION,m_sPackageUrl);
}

static size_t getVersionCode(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    string *version = (string*)userdata;
    version->append((char*)ptr, size * nmemb);
    
    return (size * nmemb);
}

bool AssetsManager::checkUpdate()
{
    if (m_sVersionFileUrl.size() == 0) return false;
    
    m_pCurl = curl_easy_init();
    if (! m_pCurl)
    {
        CCLOG("can not init curl");
        return false;
    }
    
    // Clear _version before assign new value.
    m_sVersion.clear();
    
    CURLcode res;
    curl_easy_setopt(m_pCurl, CURLOPT_URL, m_sVersionFileUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, getVersionCode);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &m_sVersion);
    if (m_uConnectionTimeout) curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, m_uConnectionTimeout);
    res = curl_easy_perform(m_pCurl);
    
    if (res != 0)
    {
        sendErrorMessage(ErrorCode::NETWORK);
        CCLOG("can not get version file content, error code is %d", res);
        curl_easy_cleanup(m_pCurl);
        return false;
    }
    
    string recordedVersion = UserDefault::getInstance()->getStringForKey(keyOfVersion().c_str());
    if (recordedVersion == m_sVersion)
    {
        sendErrorMessage(ErrorCode::NO_NEW_VERSION);
        CCLOG("there is not new version");
        // Set resource search path.
        setSearchPath();
        return false;
    }
    
    CCLOG("there is a new version: %s", m_sVersion.c_str());
    
    return true;
}

void AssetsManager::downloadAndUncompress()
{
    do
    {
        if (m_sDownloadedVersion != m_sVersion)
        {
            if (! downLoad()) break;
            
            // Record downloaded version.
            AssetsManager::Message *msg1 = new AssetsManager::Message();
            msg1->what = ASSETSMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION;
            msg1->obj = this;
            m_pSchedule->sendMessage(msg1);
        }
        
        // Uncompress zip file.
        if (!uncompress_())
        {
            sendErrorMessage(ErrorCode::UNCOMPRESS);
            break;
        }
        
        // Record updated version and remove downloaded zip file
        AssetsManager::Message *msg2 = new AssetsManager::Message();
        msg2->what = ASSETSMANAGER_MESSAGE_UPDATE_SUCCEED;
        msg2->obj = this;
        m_pSchedule->sendMessage(msg2);
    } while (0);
    
    _isDownloading = false;
}

void AssetsManager::update()
{
    if (_isDownloading) return;
    
    _isDownloading = true;
    
    // 1. Urls of package and version should be valid;
    // 2. Package should be a zip file.
    if (m_sVersionFileUrl.size() == 0 ||
        m_sPackageUrl.size() == 0 ||
        std::string::npos == m_sPackageUrl.find(".zip"))
    {
        CCLOG("no version file url, or no package url, or the package is not a zip file");
        _isDownloading = false;
        return;
    }
    
    // Check if there is a new version.
    if (! checkUpdate())
    {
        _isDownloading = false;
        return;
    }
    
    // Is package already downloaded?
    m_sDownloadedVersion = UserDefault::getInstance()->getStringForKey(keyOfDownloadedVersion().c_str());
    
    auto t = std::thread(&AssetsManager::downloadAndUncompress, this);
    t.detach();
}

bool AssetsManager::uncompress_()
{
    // Open the zip file
    string outFileName = m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        string fullPath = m_sStoragePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            //There are not directory entry in some case.
            //So we need to test whether the file directory exists when uncompressing file entry
            //, if does not exist then create directory
            string fileNameStr(fileName);
            
            size_t startIndex=0;
            
            size_t index=fileNameStr.find("/",startIndex);
            
            while(index!=-1)
            {
                string dir=m_sStoragePath+fileNameStr.substr(0,index);
                
                FILE *out = fopen(dir.c_str(), "r");
                
                if(!out)
                {
                    if (!createDirectory(dir.c_str()))
                    {
                        CCLOG("can not create directory %s", dir.c_str());
                        unzClose(zipfile);
                        return false;
                    }
                    else
                    {
                        CCLOG("create directory %s",dir.c_str());
                    }
                }
                else
                {
                    fclose(out);
                }
                
                startIndex=index+1;
                
                index=fileNameStr.find("/",startIndex);
                
            }
            
            
            
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    
    return true;
}

/*
 * Create a direcotry is platform depended.
 */
bool AssetsManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}

void AssetsManager::setSearchPath()
{
    vector<string> searchPaths = FileUtils::getInstance()->getSearchPaths();
    vector<string>::iterator iter = searchPaths.begin();
    searchPaths.insert(iter, m_sStoragePath);
    FileUtils::getInstance()->setSearchPaths(searchPaths);
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

int assetsManagerProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    AssetsManager* manager = (AssetsManager*)ptr;
    AssetsManager::Message *msg = new AssetsManager::Message();
    msg->what = ASSETSMANAGER_MESSAGE_PROGRESS;
    
    ProgressMessage *progressData = new ProgressMessage();
    progressData->percent = (int)(nowDownloaded/totalToDownload*100);
    progressData->manager = manager;
    msg->obj = progressData;
    
    manager->m_pSchedule->sendMessage(msg);
    
    CCLOG("downloading... %d%%", (int)(nowDownloaded/totalToDownload*100));
    
    return 0;
}

bool AssetsManager::downLoad()
{
    // Create a file to save package.
    string outFileName = m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
    FILE *fp = fopen(outFileName.c_str(), "wb");
    if (! fp)
    {
        sendErrorMessage(ErrorCode::CREATE_FILE);
        CCLOG("can not create file %s", outFileName.c_str());
        return false;
    }
    
    // Download pacakge
    CURLcode res;
    curl_easy_setopt(m_pCurl, CURLOPT_URL, m_sPackageUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, downLoadPackage);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, assetsManagerProgressFunc);
    curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);
    res = curl_easy_perform(m_pCurl);
    curl_easy_cleanup(m_pCurl);
    if (res != 0)
    {
        sendErrorMessage(ErrorCode::NETWORK);
        CCLOG("error when download package");
        fclose(fp);
        return false;
    }
    
    CCLOG("succeed downloading package %s", m_sPackageUrl.c_str());
    
    fclose(fp);
    return true;
}

const char* AssetsManager::getPackageUrl() const
{
    return m_sPackageUrl.c_str();
}

void AssetsManager::setPackageUrl(const char *packageUrl)
{
    m_sPackageUrl = packageUrl;
}

const char* AssetsManager::getStoragePath() const
{
    return m_sStoragePath.c_str();
}

void AssetsManager::setStoragePath(const char *storagePath)
{
    m_sStoragePath = storagePath;
    checkStoragePath();
}

const char* AssetsManager::getVersionFileUrl() const
{
    return m_sVersionFileUrl.c_str();
}

void AssetsManager::setVersionFileUrl(const char *versionFileUrl)
{
    m_sVersionFileUrl = versionFileUrl;
}

string AssetsManager::getVersion()
{
    return UserDefault::getInstance()->getStringForKey(keyOfVersion().c_str());
}

void AssetsManager::deleteVersion()
{
    UserDefault::getInstance()->setStringForKey(keyOfVersion().c_str(), "");
}

void AssetsManager::setDelegate(AssetsManagerDelegateProtocol *delegate)
{
    m_pDelegate = delegate;
}

void AssetsManager::setConnectionTimeout(unsigned int timeout)
{
    m_uConnectionTimeout = timeout;
}

unsigned int AssetsManager::getConnectionTimeout()
{
    return m_uConnectionTimeout;
}

void AssetsManager::sendErrorMessage(AssetsManager::ErrorCode code)
{
    Message *msg = new Message();
    msg->what = ASSETSMANAGER_MESSAGE_ERROR;
    
    ErrorMessage *errorMessage = new ErrorMessage();
    errorMessage->code = code;
    errorMessage->manager = this;
    msg->obj = errorMessage;
    
    m_pSchedule->sendMessage(msg);
}

// Implementation of AssetsManagerHelper

AssetsManager::Helper::Helper()
{
    m_pMessageQueue = new list<Message*>();
    Director::getInstance()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

AssetsManager::Helper::~Helper()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    delete m_pMessageQueue;
}

void AssetsManager::Helper::sendMessage(Message *msg)
{
    m_tMessageQueueMutex.lock();
    m_pMessageQueue->push_back(msg);
    m_tMessageQueueMutex.unlock();
}

void AssetsManager::Helper::update(float dt)
{
    Message *msg = NULL;
    
    // Returns quickly if no message
    m_tMessageQueueMutex.lock();
    if (0 == m_pMessageQueue->size())
    {
        m_tMessageQueueMutex.unlock();
        return;
    }
    //remove unnecessary message
    std::list<Message*>::iterator it;
    Message *proMsg = nullptr;  
    for (it = m_pMessageQueue->begin(); it != m_pMessageQueue->end(); ++it)
    {          
        if((*it)->what == ASSETSMANAGER_MESSAGE_PROGRESS)
        {
            if (proMsg)
            {
                m_pMessageQueue->remove(proMsg); 
                delete (ProgressMessage*)proMsg->obj;
                delete proMsg;
            }
            proMsg = *it;        
        }       
    }
    // Gets message
    msg = *(m_pMessageQueue->begin());
    m_pMessageQueue->pop_front();
    m_tMessageQueueMutex.unlock();
    
    switch (msg->what) {
        case ASSETSMANAGER_MESSAGE_UPDATE_SUCCEED:
            handleUpdateSucceed(msg);
            
            break;
        case ASSETSMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION:
            UserDefault::getInstance()->setStringForKey(((AssetsManager*)msg->obj)->keyOfDownloadedVersion().c_str(),
                                                                ((AssetsManager*)msg->obj)->m_sVersion.c_str());
            UserDefault::getInstance()->flush();
            
            break;
        case ASSETSMANAGER_MESSAGE_PROGRESS:
            if (((ProgressMessage*)msg->obj)->manager->m_pDelegate)
            {
                ((ProgressMessage*)msg->obj)->manager->m_pDelegate->onProgress(((ProgressMessage*)msg->obj)->percent);
            }
            
            delete (ProgressMessage*)msg->obj;
            
            break;
        case ASSETSMANAGER_MESSAGE_ERROR:
            // error call back
            if (((ErrorMessage*)msg->obj)->manager->m_pDelegate)
            {
                ((ErrorMessage*)msg->obj)->manager->m_pDelegate->onError(((ErrorMessage*)msg->obj)->code);
            }
            
            delete ((ErrorMessage*)msg->obj);
            
            break;
        default:
            break;
    }
    
    delete msg;
}

void AssetsManager::Helper::handleUpdateSucceed(Message *msg)
{
    AssetsManager* manager = (AssetsManager*)msg->obj;
    
    // Record new version code.
    UserDefault::getInstance()->setStringForKey(manager->keyOfVersion().c_str(), manager->m_sVersion.c_str());
    
    // Unrecord downloaded version code.
    UserDefault::getInstance()->setStringForKey(manager->keyOfDownloadedVersion().c_str(), "");
    UserDefault::getInstance()->flush();
    
    // Set resource search path.
    manager->setSearchPath();
    
    // Delete unloaded zip file.
    string zipfileName = manager->m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
    if (remove(zipfileName.c_str()) != 0)
    {
        CCLOG("can not remove downloaded zip file %s", zipfileName.c_str());
    }
    
    if (manager->m_pDelegate) manager->m_pDelegate->onSuccess();
}

AssetsManager* AssetsManager::create(const char* packageUrl, const char* versionFileUrl, const char* storagePath, ErrorCallback errorCallback, ProgressCallback progressCallback, SuccessCallback successCallback )
{
    class DelegateProtocolImpl : public AssetsManagerDelegateProtocol 
    {
    public :
        DelegateProtocolImpl(ErrorCallback aErrorCallback, ProgressCallback aProgressCallback, SuccessCallback aSuccessCallback)
        : errorCallback(aErrorCallback), progressCallback(aProgressCallback), successCallback(aSuccessCallback)
        {}

        virtual void onError(AssetsManager::ErrorCode errorCode) { errorCallback(int(errorCode)); }
        virtual void onProgress(int percent) { progressCallback(percent); }
        virtual void onSuccess() { successCallback(); }

    private :
        ErrorCallback errorCallback;
        ProgressCallback progressCallback;
        SuccessCallback successCallback;
    };

    auto* manager = new AssetsManager(packageUrl,versionFileUrl,storagePath);
    auto* delegate = new DelegateProtocolImpl(errorCallback,progressCallback,successCallback);
    manager->setDelegate(delegate);
    manager->_shouldDeleteDelegateWhenExit = true;
    manager->autorelease();
    return manager;
}

KDvoid AssetsManager::createStoragePath ( KDvoid )
{
    // Remove downloaded files
	KDDir*	pDir = KD_NULL;

	pDir = kdOpenDir ( m_sStoragePath.c_str ( ) );
	if ( !pDir )
	{
		kdMkdir ( m_sStoragePath.c_str ( ) );
	}
}

KDvoid AssetsManager::destroyStoragePath ( KDvoid )
{
    // Delete recorded version codes.
    deleteVersion ( );

	kdRmdir ( m_sStoragePath.c_str ( ) );
    /*
    // Remove downloaded files
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    string command = "rm -r ";
    // Path may include space.
    command += "\"" + m_sStoragePath + "\"";
    system(command.c_str());    
#else
    string command = "rd /s /q ";
    // Path may include space.
    command += "\"" + m_sStoragePath + "\"";
    system(command.c_str());
#endif
	*/
}

NS_CC_EXT_END;
