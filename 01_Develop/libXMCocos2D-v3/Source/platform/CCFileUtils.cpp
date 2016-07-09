/* -----------------------------------------------------------------------------------
 *
 *      File            CCFileUtils.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#include "platform/CCFileUtils.h"
#include "2d/CCDirector.h"
#include "base/CCDictionary.h"
#include "base/CCString.h"
#include "platform/CCSAXParser.h"
#include <XMTinyXml/tinyxml2.h>
#include <XMZlib/contrib/minizip/unzip.h>
#include <stack>

using namespace std;

#if 1 //(CC_TARGET_PLATFORM != CC_PLATFORM_IOS) && (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

NS_CC_BEGIN

typedef enum 
{
    SAX_NONE = 0,
    SAX_KEY,
    SAX_DICT,
    SAX_INT,
    SAX_REAL,
    SAX_STRING,
    SAX_ARRAY
}SAXState;

typedef enum
{
    SAX_RESULT_NONE = 0,
    SAX_RESULT_DICT,
    SAX_RESULT_ARRAY
}SAXResult;

class DictMaker : public SAXDelegator
{
public:
    SAXResult _resultType;
    Array* _rootArray;
    Dictionary *_rootDict;
    Dictionary *_curDict;
    std::stack<Dictionary*> _dictStack;
    std::string _curKey;   ///< parsed key
    std::string _curValue; // parsed value
    SAXState _state;
    Array* _array;

    std::stack<Array*> _arrayStack;
    std::stack<SAXState>  _stateStack;

public:
    DictMaker()        
        : _resultType(SAX_RESULT_NONE),
          _rootArray(NULL), 
          _rootDict(NULL),
          _curDict(NULL),
          _state(SAX_NONE),
          _array(NULL)
    {
    }

    ~DictMaker()
    {
    }

    Dictionary* dictionaryWithContentsOfFile(const char *pFileName)
    {
        _resultType = SAX_RESULT_DICT;
        SAXParser parser;

        if (false == parser.init("UTF-8"))
        {
            return NULL;
        }
        parser.setDelegator(this);

        parser.parse(pFileName);
        return _rootDict;
    }

    Array* arrayWithContentsOfFile(const char* pFileName)
    {
        _resultType = SAX_RESULT_ARRAY;
        SAXParser parser;

        if (false == parser.init("UTF-8"))
        {
            return NULL;
        }
        parser.setDelegator(this);

        parser.parse(pFileName);
        return _array;
    }

    void startElement(void *ctx, const char *name, const char **atts)
    {
        CC_UNUSED_PARAM(ctx);
        CC_UNUSED_PARAM(atts);
        std::string sName((char*)name);
        if( sName == "dict" )
        {
            _curDict = new Dictionary();
            if(_resultType == SAX_RESULT_DICT && _rootDict == NULL)
            {
                // Because it will call _curDict->release() later, so retain here.
                _rootDict = _curDict;
                _rootDict->retain();
            }
            _state = SAX_DICT;

            SAXState preState = SAX_NONE;
            if (! _stateStack.empty())
            {
                preState = _stateStack.top();
            }

            if (SAX_ARRAY == preState)
            {
                // add the dictionary into the array
                _array->addObject(_curDict);
            }
            else if (SAX_DICT == preState)
            {
                // add the dictionary into the pre dictionary
                CCASSERT(! _dictStack.empty(), "The state is wrong!");
                Dictionary* pPreDict = _dictStack.top();
                pPreDict->setObject(_curDict, _curKey.c_str());
            }

            _curDict->release();

            // record the dict state
            _stateStack.push(_state);
            _dictStack.push(_curDict);
        }
        else if(sName == "key")
        {
            _state = SAX_KEY;
        }
        else if(sName == "integer")
        {
            _state = SAX_INT;
        }
        else if(sName == "real")
        {
            _state = SAX_REAL;
        }
        else if(sName == "string")
        {
            _state = SAX_STRING;
        }
        else if (sName == "array")
        {
            _state = SAX_ARRAY;
            _array = new Array();
            _array->init();
            if (_resultType == SAX_RESULT_ARRAY && _rootArray == NULL)
            {
                _rootArray = _array;
                _rootArray->retain();
            }
            SAXState preState = SAX_NONE;
            if (! _stateStack.empty())
            {
                preState = _stateStack.top();
            }

            if (preState == SAX_DICT)
            {
                _curDict->setObject(_array, _curKey.c_str());
            }
            else if (preState == SAX_ARRAY)
            {
                CCASSERT(! _arrayStack.empty(), "The state is wrong!");
                Array* pPreArray = _arrayStack.top();
                pPreArray->addObject(_array);
            }
            _array->release();
            // record the array state
            _stateStack.push(_state);
            _arrayStack.push(_array);
        }
        else
        {
            _state = SAX_NONE;
        }
    }

    void endElement(void *ctx, const char *name)
    {
        CC_UNUSED_PARAM(ctx);
        SAXState curState = _stateStack.empty() ? SAX_DICT : _stateStack.top();
        std::string sName((char*)name);
        if( sName == "dict" )
        {
            _stateStack.pop();
            _dictStack.pop();
            if ( !_dictStack.empty())
            {
                _curDict = _dictStack.top();
            }
        }
        else if (sName == "array")
        {
            _stateStack.pop();
            _arrayStack.pop();
            if (! _arrayStack.empty())
            {
                _array = _arrayStack.top();
            }
        }
        else if (sName == "true")
        {
            String *str = new String("1");
            if (SAX_ARRAY == curState)
            {
                _array->addObject(str);
            }
            else if (SAX_DICT == curState)
            {
                _curDict->setObject(str, _curKey.c_str());
            }
            str->release();
        }
        else if (sName == "false")
        {
            String *str = new String("0");
            if (SAX_ARRAY == curState)
            {
                _array->addObject(str);
            }
            else if (SAX_DICT == curState)
            {
                _curDict->setObject(str, _curKey.c_str());
            }
            str->release();
        }
        else if (sName == "string" || sName == "integer" || sName == "real")
        {
            String* pStrValue = new String(_curValue);

            if (SAX_ARRAY == curState)
            {
                _array->addObject(pStrValue);
            }
            else if (SAX_DICT == curState)
            {
                _curDict->setObject(pStrValue, _curKey.c_str());
            }

            pStrValue->release();
            _curValue.clear();
        }
        
        _state = SAX_NONE;
    }

    void textHandler(void *ctx, const char *ch, int len)
    {
        CC_UNUSED_PARAM(ctx);
        if (_state == SAX_NONE)
        {
            return;
        }

        SAXState curState = _stateStack.empty() ? SAX_DICT : _stateStack.top();
        String *pText = new String(std::string((char*)ch,0,len));

        switch(_state)
        {
        case SAX_KEY:
            _curKey = pText->getCString();
            break;
        case SAX_INT:
        case SAX_REAL:
        case SAX_STRING:
            {
                if (curState == SAX_DICT)
                {
                    CCASSERT(!_curKey.empty(), "key not found : <integer/real>");
                }
                
                _curValue.append(pText->getCString());
            }
            break;
        default:
            break;
        }
        pText->release();
    }
};

Dictionary* FileUtils::createDictionaryWithContentsOfFile(const std::string& filename)
{
    std::string fullPath = fullPathForFilename(filename.c_str());
    DictMaker tMaker;
    return tMaker.dictionaryWithContentsOfFile(fullPath.c_str());
}

Array* FileUtils::createArrayWithContentsOfFile(const std::string& filename)
{
    std::string fullPath = fullPathForFilename(filename.c_str());
    DictMaker tMaker;
    return tMaker.arrayWithContentsOfFile(fullPath.c_str());
}


/*
 * forward statement
 */
static tinyxml2::XMLElement* generateElementForArray(cocos2d::Array *array, tinyxml2::XMLDocument *pDoc);
static tinyxml2::XMLElement* generateElementForDict(cocos2d::Dictionary *dict, tinyxml2::XMLDocument *pDoc);

/*
 * Use tinyxml2 to write plist files
 */
bool FileUtils::writeToFile(cocos2d::Dictionary *dict, const std::string &fullPath)
{
    //CCLOG("tinyxml2 Dictionary %d writeToFile %s", dict->_ID, fullPath.c_str());
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    if (NULL == pDoc)
        return false;
    
    tinyxml2::XMLDeclaration *pDeclaration = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    if (NULL == pDeclaration)
    {
        delete pDoc;
        return false;
    }
    
    pDoc->LinkEndChild(pDeclaration);
    tinyxml2::XMLElement *docType = pDoc->NewElement("!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"");
    pDoc->LinkEndChild(docType);
    
    tinyxml2::XMLElement *pRootEle = pDoc->NewElement("plist");
    pRootEle->SetAttribute("version", "1.0");
    if (NULL == pRootEle)
    {
        delete pDoc;
        return false;
    }
    pDoc->LinkEndChild(pRootEle);
    
    tinyxml2::XMLElement *innerDict = generateElementForDict(dict, pDoc);
    if (NULL == innerDict )
    {
        delete pDoc;
        return false;
    }
    pRootEle->LinkEndChild(innerDict);
    
    bool bRet = tinyxml2::XML_SUCCESS == pDoc->SaveFile(fullPath.c_str());
    
    delete pDoc;
    return bRet;
}

/*
 * Generate tinyxml2::XMLElement for Object through a tinyxml2::XMLDocument
 */
static tinyxml2::XMLElement* generateElementForObject(cocos2d::Object *object, tinyxml2::XMLDocument *pDoc)
{
    // object is String
    if (String *str = dynamic_cast<String *>(object))
    {
        tinyxml2::XMLElement* node = pDoc->NewElement("string");
        tinyxml2::XMLText* content = pDoc->NewText(str->getCString());
        node->LinkEndChild(content);
        return node;
    }
    
    // object is Array
    if (Array *array = dynamic_cast<Array *>(object))
        return generateElementForArray(array, pDoc);
    
    // object is Dictionary
    if (Dictionary *innerDict = dynamic_cast<Dictionary *>(object))
        return generateElementForDict(innerDict, pDoc);
    
    CCLOG("This type cannot appear in property list");
    return NULL;
}

/*
 * Generate tinyxml2::XMLElement for Dictionary through a tinyxml2::XMLDocument
 */
static tinyxml2::XMLElement* generateElementForDict(cocos2d::Dictionary *dict, tinyxml2::XMLDocument *pDoc)
{
    tinyxml2::XMLElement* rootNode = pDoc->NewElement("dict");
    
    DictElement *dictElement = NULL;
    CCDICT_FOREACH(dict, dictElement)
    {
        tinyxml2::XMLElement* tmpNode = pDoc->NewElement("key");
        rootNode->LinkEndChild(tmpNode);
        tinyxml2::XMLText* content = pDoc->NewText(dictElement->getStrKey());
        tmpNode->LinkEndChild(content);
        
        Object *object = dictElement->getObject();
        tinyxml2::XMLElement *element = generateElementForObject(object, pDoc);
        if (element)
            rootNode->LinkEndChild(element);
    }
    return rootNode;
}

/*
 * Generate tinyxml2::XMLElement for Array through a tinyxml2::XMLDocument
 */
static tinyxml2::XMLElement* generateElementForArray(cocos2d::Array *array, tinyxml2::XMLDocument *pDoc)
{
    tinyxml2::XMLElement* rootNode = pDoc->NewElement("array");
    
    Object *object = NULL;
    CCARRAY_FOREACH(array, object)
    {
        tinyxml2::XMLElement *element = generateElementForObject(object, pDoc);
        if (element)
            rootNode->LinkEndChild(element);
    }
    return rootNode;
}


#else
NS_CC_BEGIN

/* The subclass FileUtilsApple should override these two method. */
Dictionary* FileUtils::createDictionaryWithContentsOfFile(const std::string& filename) {return NULL;}
bool FileUtils::writeToFile(cocos2d::Dictionary *dict, const std::string &fullPath) {return false;}
Array* FileUtils::createArrayWithContentsOfFile(const std::string& filename) {return NULL;}

#endif /* (CC_TARGET_PLATFORM != CC_PLATFORM_IOS) && (CC_TARGET_PLATFORM != CC_PLATFORM_MAC) */


FileUtils* FileUtils::s_pSharedFileUtils = NULL;

FileUtils* FileUtils::getInstance()
{
	if (s_pSharedFileUtils == NULL)
	{
		s_pSharedFileUtils = new FileUtils();
		if (!s_pSharedFileUtils->init())
		{
			delete s_pSharedFileUtils;
			s_pSharedFileUtils = NULL;
			CCLOG("ERROR: Could not init CCFileUtilsWin32");
		}
	}
	return s_pSharedFileUtils;
}

void FileUtils::destroyInstance()
{
    CC_SAFE_DELETE(s_pSharedFileUtils);
}

FileUtils::FileUtils()
: m_pFilenameLookupDict(NULL)
{
}

FileUtils::~FileUtils()
{
    CC_SAFE_RELEASE(m_pFilenameLookupDict);
}


bool FileUtils::init()
{
	m_sDefaultResRootPath = "/res/";
    m_aSearchPathArray.push_back(m_sDefaultResRootPath);
    m_aSearchResolutionsOrderArray.push_back("");
    return true;
}

void FileUtils::purgeCachedEntries()
{
    m_aFullPathCache.clear();
}

unsigned char* FileUtils::getFileData(const char* filename, const char* mode, KDint32 *size)
{
    unsigned char * buffer = NULL;
    CCASSERT(filename != NULL && size != NULL && mode != NULL, "Invalid parameters.");
    *size = 0;
    do
    {
        // read the file from hardware
        std::string fullPath = fullPathForFilename(filename);
        KDFile* fp = kdFopen(fullPath.c_str(), mode);
        CC_BREAK_IF(!fp);
        
        kdFseek ( fp, 0, KD_SEEK_END );
        *size = kdFtell ( fp );
        kdFseek ( fp, 0, KD_SEEK_SET );
        buffer = (unsigned char*) kdMalloc(*size);
        *size = kdFread(buffer,sizeof(unsigned char), *size,fp);
        kdFclose ( fp );
    } while (0);
    
    if (! buffer)
    {
        std::string msg = "Get data from file(";
        msg.append(filename).append(") failed!");
        
        CCLOG("%s", msg.c_str());
    }
    return buffer;
}

unsigned char* FileUtils::getFileDataFromZip(const char* zipFilePath, const char* filename, KDint32 *size)
{
    unsigned char * buffer = NULL;
    unzFile pFile = NULL;
    *size = 0;

    do 
    {
        CC_BREAK_IF(!zipFilePath || !filename);
        CC_BREAK_IF(strlen(zipFilePath) == 0);

        pFile = unzOpen(zipFilePath);
        CC_BREAK_IF(!pFile);

        int nRet = unzLocateFile(pFile, filename, 1);
        CC_BREAK_IF(UNZ_OK != nRet);

        char szFilePathA[260];
        unz_file_info FileInfo;
        nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
        CC_BREAK_IF(UNZ_OK != nRet);

        nRet = unzOpenCurrentFile(pFile);
        CC_BREAK_IF(UNZ_OK != nRet);

        buffer = (unsigned char*) kdMalloc(FileInfo.uncompressed_size);
        int CC_UNUSED nSize = unzReadCurrentFile(pFile, buffer, FileInfo.uncompressed_size);
        CCASSERT(nSize == 0 || nSize == (int)FileInfo.uncompressed_size, "the file size is wrong");

        *size = FileInfo.uncompressed_size;
        unzCloseCurrentFile(pFile);
    } while (0);

    if (pFile)
    {
        unzClose(pFile);
    }

    return buffer;
}

std::string FileUtils::getNewFilename(const std::string &filename)
{
    std::string newFileName;
    
    // in Lookup Filename dictionary ?
    String* fileNameFound = m_pFilenameLookupDict ? (String*)m_pFilenameLookupDict->objectForKey(filename) : NULL;
    if( NULL == fileNameFound || fileNameFound->length() == 0) {
        newFileName = filename;
    }
    else {
        newFileName = fileNameFound->getCString();
    }
    return newFileName;
}

std::string FileUtils::getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath)
{
    std::string file = filename;
    std::string file_path = "";
    size_t pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        file_path = filename.substr(0, pos+1);
        file = filename.substr(pos+1);
    }
    
    // searchPath + file_path + resourceDirectory
    std::string path = searchPath;
    path += file_path;
    path += resolutionDirectory;
    
    path = getFullPathForDirectoryAndFilename(path, file);
    
    //CCLOG("getPathForFilename, fullPath = %s", path.c_str());
    return path;
}


std::string FileUtils::fullPathForFilename(const std::string &filename)
{
    if (isAbsolutePath(filename))
    {
        return filename;
    }

    // Already Cached ?
    auto cacheIter = m_aFullPathCache.find(filename);
    if( cacheIter != m_aFullPathCache.end() )
    {
        return cacheIter->second;
    }
    
    // Get the new file name.
    std::string newFilename( getNewFilename(filename) );
    
    string fullpath = "";
    
    for (auto searchIt = m_aSearchPathArray.begin(); searchIt != m_aSearchPathArray.end(); ++searchIt) {
        for (auto resolutionIt = m_aSearchResolutionsOrderArray.begin(); resolutionIt != m_aSearchResolutionsOrderArray.end(); ++resolutionIt) {
            
            fullpath = this->getPathForFilename(newFilename, *resolutionIt, *searchIt);
            
            if (fullpath.length() > 0)
            {
                // Using the filename passed in as key.
                m_aFullPathCache.insert(std::pair<std::string, std::string>(filename, fullpath));
                return fullpath;
            }
        }
    }
    
//	CCLOG("cocos2d: fullPathForFilename: No file found at %s. Possible missing file.", filename.c_str());

    // XXX: Should it return nullptr ? or an empty string ?
    // The file wasn't found, return the file name passed in.
    return filename;
}

std::string FileUtils::fullPathFromRelativeFile(const std::string &filename, const std::string &relativeFile)
{
    return relativeFile.substr(0, relativeFile.rfind('/')+1) + getNewFilename(filename);
}

void FileUtils::setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder)
{
    bool existDefault = false;
    m_aFullPathCache.clear();
    m_aSearchResolutionsOrderArray.clear();
    for(auto iter = searchResolutionsOrder.begin(); iter != searchResolutionsOrder.end(); ++iter)
    {
        std::string resolutionDirectory = *iter;
        if (!existDefault && resolutionDirectory == "")
        {
            existDefault = true;
        }
        
        if (resolutionDirectory.length() > 0 && resolutionDirectory[resolutionDirectory.length()-1] != '/')
        {
            resolutionDirectory += "/";
        }
        
        m_aSearchResolutionsOrderArray.push_back(resolutionDirectory);
    }
    if (!existDefault)
    {
        m_aSearchResolutionsOrderArray.push_back("");
    }
}

void FileUtils::addSearchResolutionsOrder(const std::string &order)
{
    m_aSearchResolutionsOrderArray.push_back(order);
}

const std::vector<std::string>& FileUtils::getSearchResolutionsOrder()
{
    return m_aSearchResolutionsOrderArray;
}

const std::vector<std::string>& FileUtils::getSearchPaths() const
{
    return m_aSearchPathArray;
}

void FileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
{
    bool existDefaultRootPath = false;
    
    m_aFullPathCache.clear();
    m_aSearchPathArray.clear();
    for (auto iter = searchPaths.begin(); iter != searchPaths.end(); ++iter)
    {
        std::string strPrefix;
        std::string path;
        
        if (!isAbsolutePath(*iter))
        { // Not an absolute path
            strPrefix = m_sDefaultResRootPath;
        }
        path = strPrefix + (*iter);
        if (path.length() > 0 && path[path.length()-1] != '/')
        {
            path += "/";
        }
        if (!existDefaultRootPath && path == m_sDefaultResRootPath)
        {
            existDefaultRootPath = true;
        }
        m_aSearchPathArray.push_back(path);
    }
    
    if (!existDefaultRootPath)
    {
        //CCLOG("Default root path doesn't exist, adding it.");
        m_aSearchPathArray.push_back(m_sDefaultResRootPath);
    }
}

void FileUtils::addSearchPath(const std::string &searchpath)
{
    std::string strPrefix;
    if (!isAbsolutePath(searchpath))
        strPrefix = m_sDefaultResRootPath;

    std::string path = strPrefix + searchpath;
    if (path.length() > 0 && path[path.length()-1] != '/')
    {
        path += "/";
    }
    m_aSearchPathArray.push_back(path);
}

void FileUtils::setFilenameLookupDictionary(Dictionary* pFilenameLookupDict)
{
    m_aFullPathCache.clear();    
    CC_SAFE_RELEASE(m_pFilenameLookupDict);
    m_pFilenameLookupDict = pFilenameLookupDict;
    CC_SAFE_RETAIN(m_pFilenameLookupDict);
}

void FileUtils::loadFilenameLookupDictionaryFromFile(const std::string &filename)
{
    std::string fullPath = fullPathForFilename(filename);
    if (fullPath.length() > 0)
    {
        Dictionary* dict = Dictionary::createWithContentsOfFile(fullPath.c_str());
        if (dict)
        {
            Dictionary* metadata = static_cast<Dictionary*>( dict->objectForKey("metadata") );
            int version = static_cast<String*>( metadata->objectForKey("version"))->intValue();
            if (version != 1)
            {
                CCLOG("cocos2d: ERROR: Invalid filenameLookup dictionary version: %ld. Filename: %s", (long)version, filename.c_str());
                return;
            }
            setFilenameLookupDictionary( static_cast<Dictionary*>( dict->objectForKey("filenames")) );
        }
    }
}

std::string FileUtils::getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename)
{
    // get directory+filename, safely adding '/' as necessary 
    std::string ret = strDirectory;
    if (strDirectory.size() && strDirectory[strDirectory.size()-1] != '/'){
        ret += '/';
    }
    ret += strFilename;
    
    // if the file doesn't exist, return an empty string
    if (!isFileExist(ret)) {
        ret = "";
    }
    return ret;
}

std::string FileUtils::getWritablePath() const
{
	return std::string ("/data/");
}

bool FileUtils::isFileExist(const std::string& filePath)
{
	KDbool   bFound = KD_FALSE;
	KDFile*  pFile = kdFopen(filePath.c_str(), "r");
	if (pFile)
	{
		bFound = KD_TRUE;
		kdFclose(pFile);
	}
	return bFound;
}

bool FileUtils::isAbsolutePath(const std::string& strPath) const
{
    return (strPath[0] == '/');
}

//////////////////////////////////////////////////////////////////////////
// Notification support when getFileData from invalid file path.
//////////////////////////////////////////////////////////////////////////
static bool s_popupNotify = true;

void FileUtils::setPopupNotify(bool notify)
{
    s_popupNotify = notify;
}

bool FileUtils::isPopupNotify()
{
    return s_popupNotify;
}

NS_CC_END

