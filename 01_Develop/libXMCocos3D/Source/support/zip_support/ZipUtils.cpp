/* -----------------------------------------------------------------------------------
 *
 *      File            ZipUtils.cpp
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

#include "support/zip_support/ZipUtils.h"
#include "platform/CCFileUtils.h"
#include <XMZlib/zlib.h>
#include <XMZlib/contrib/minizip/unzip.h>
#include <map>

NS_CC_BEGIN

unsigned int ZipUtils::s_uEncryptedPvrKeyParts[4] = {0,0,0,0};
unsigned int ZipUtils::s_uEncryptionKey[1024];
bool ZipUtils::s_bEncryptionKeyIsValid = false;

// --------------------- ZipUtils ---------------------

inline void ZipUtils::decodeEncodedPvr(unsigned int *data, KDint32 len)
{
    const int enclen = 1024;
    const int securelen = 512;
    const int distance = 64;
    
    // check if key was set
    // make sure to call caw_setkey_part() for all 4 key parts
    CCASSERT(s_uEncryptedPvrKeyParts[0] != 0, "Cocos2D: CCZ file is encrypted but key part 0 is not set. Did you call ZipUtils::setPvrEncryptionKeyPart(...)?");
    CCASSERT(s_uEncryptedPvrKeyParts[1] != 0, "Cocos2D: CCZ file is encrypted but key part 1 is not set. Did you call ZipUtils::setPvrEncryptionKeyPart(...)?");
    CCASSERT(s_uEncryptedPvrKeyParts[2] != 0, "Cocos2D: CCZ file is encrypted but key part 2 is not set. Did you call ZipUtils::setPvrEncryptionKeyPart(...)?");
    CCASSERT(s_uEncryptedPvrKeyParts[3] != 0, "Cocos2D: CCZ file is encrypted but key part 3 is not set. Did you call ZipUtils::setPvrEncryptionKeyPart(...)?");
    
    // create long key
    if(!s_bEncryptionKeyIsValid)
    {
        unsigned int y, p, e;
        unsigned int rounds = 6;
        unsigned int sum = 0;
        unsigned int z = s_uEncryptionKey[enclen-1];
        
        do
        {
#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (s_uEncryptedPvrKeyParts[(p&3)^e] ^ z)))
            
            sum += DELTA;
            e = (sum >> 2) & 3;
            
            for (p = 0; p < enclen - 1; p++)
            {
                y = s_uEncryptionKey[p + 1];
                z = s_uEncryptionKey[p] += MX;
            }
            
            y = s_uEncryptionKey[0];
            z = s_uEncryptionKey[enclen - 1] += MX;
            
        } while (--rounds);
        
        s_bEncryptionKeyIsValid = true;
    }
    
    int b = 0;
    int i = 0;
    
    // encrypt first part completely
    for(; i < len && i < securelen; i++)
    {
        data[i] ^= s_uEncryptionKey[b++];
        
        if(b >= enclen)
        {
            b = 0;
        }
    }
    
    // encrypt second section partially
    for(; i < len; i += distance)
    {
        data[i] ^= s_uEncryptionKey[b++];
        
        if(b >= enclen)
        {
            b = 0;
        }
    }
}

inline unsigned int ZipUtils::checksumPvr(const unsigned int *data, KDint32 len)
{
    unsigned int cs = 0;
    const int cslen = 128;
    
    len = (len < cslen) ? len : cslen;
    
    for(int i = 0; i < len; i++)
    {
        cs = cs ^ data[i];
    }
    
    return cs;
}

// memory in iPhone is precious
// Should buffer factor be 1.5 instead of 2 ?
#define BUFFER_INC_FACTOR (2)

int ZipUtils::inflateMemoryWithHint(unsigned char *in, KDint32 inLength, unsigned char **out, KDint32 *outLength, KDint32 outLenghtHint)
{
    /* ret value */
    int err = Z_OK;
    
    long bufferSize = outLenghtHint;
    *out = (unsigned char*) kdMalloc(bufferSize);
    
    z_stream d_stream; /* decompression stream */
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    
    d_stream.next_in  = in;
    d_stream.avail_in = inLength;
    d_stream.next_out = *out;
    d_stream.avail_out = bufferSize;
    
    /* window size to hold 256k */
    if( (err = inflateInit2(&d_stream, 15 + 32)) != Z_OK )
        return err;
    
    for (;;)
    {
        err = inflate(&d_stream, Z_NO_FLUSH);
        
        if (err == Z_STREAM_END)
        {
            break;
        }
        
        switch (err)
        {
            case Z_NEED_DICT:
                err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&d_stream);
                return err;
        }
        
        // not enough memory ?
        if (err != Z_STREAM_END)
        {
            *out = (unsigned char*) kdRealloc(*out, bufferSize * BUFFER_INC_FACTOR);
            
            /* not enough memory, ouch */
            if (! *out )
            {
                CCLOG("cocos2d: ZipUtils: realloc failed");
                inflateEnd(&d_stream);
                return Z_MEM_ERROR;
            }
            
            d_stream.next_out = *out + bufferSize;
            d_stream.avail_out = bufferSize;
            bufferSize *= BUFFER_INC_FACTOR;
        }
    }
    
    *outLength = bufferSize - d_stream.avail_out;
    err = inflateEnd(&d_stream);
    return err;
}

int ZipUtils::inflateMemoryWithHint(unsigned char *in, KDint32 inLength, unsigned char **out, KDint32 outLengthHint)
{
    KDint32 outLength = 0;
    int err = inflateMemoryWithHint(in, inLength, out, &outLength, outLengthHint);
    
    if (err != Z_OK || *out == NULL) {
        if (err == Z_MEM_ERROR)
        {
            CCLOG("cocos2d: ZipUtils: Out of memory while decompressing map data!");
        } else
            if (err == Z_VERSION_ERROR)
            {
                CCLOG("cocos2d: ZipUtils: Incompatible zlib version!");
            } else
                if (err == Z_DATA_ERROR)
                {
                    CCLOG("cocos2d: ZipUtils: Incorrect zlib compressed data!");
                }
                else
                {
                    CCLOG("cocos2d: ZipUtils: Unknown error while decompressing map data!");
                }

        if(*out) {
            kdFree(*out);
            *out = NULL;
        }
        outLength = 0;
    }
    
    return outLength;
}

int ZipUtils::inflateMemory(unsigned char *in, KDint32 inLength, unsigned char **out)
{
    // 256k for hint
    return inflateMemoryWithHint(in, inLength, out, 256 * 1024);
}

int ZipUtils::inflateGZipFile(const char *path, unsigned char **out)
{
    int len;
    unsigned int offset = 0;
    
    CCASSERT(out, "");
    CCASSERT(&*out, "");
    
    gzFile inFile = gzopen(path, "rb");
    if( inFile == NULL ) {
        CCLOG("cocos2d: ZipUtils: error open gzip file: %s", path);
        return -1;
    }
    
    /* 512k initial decompress buffer */
    unsigned int bufferSize = 512 * 1024;
    unsigned int totalBufferSize = bufferSize;
    
    *out = (unsigned char*) kdMalloc( bufferSize );
    if( ! out )
    {
        CCLOG("cocos2d: ZipUtils: out of memory");
        return -1;
    }
    
    for (;;) {
        len = gzread(inFile, *out + offset, bufferSize);
        if (len < 0)
        {
            CCLOG("cocos2d: ZipUtils: error in gzread");
            kdFree( *out );
            *out = NULL;
            return -1;
        }
        if (len == 0)
        {
            break;
        }
        
        offset += len;
        
        // finish reading the file
        if( (unsigned int)len < bufferSize )
        {
            break;
        }
        
        bufferSize *= BUFFER_INC_FACTOR;
        totalBufferSize += bufferSize;
        unsigned char *tmp = (unsigned char*) kdRealloc(*out, totalBufferSize );
        
        if( ! tmp )
        {
            CCLOG("cocos2d: ZipUtils: out of memory");
            kdFree( *out );
            *out = NULL;
            return -1;
        }
        
        *out = tmp;
    }
    
    if (gzclose(inFile) != Z_OK)
    {
        CCLOG("cocos2d: ZipUtils: gzclose failed");
    }
    
    return offset;
}

bool ZipUtils::isCCZFile(const char *path)
{
    // load file into memory
    unsigned char* compressed = NULL;

    KDint32 fileLen = 0;
    compressed = FileUtils::getInstance()->getFileData(path, "rb", &fileLen);

    if(compressed == NULL || fileLen == 0)
    {
        CCLOG("cocos2d: ZipUtils: loading file failed");
        return false;
    }

    bool ret = isCCZBuffer(compressed, fileLen);
    kdFree(compressed);

    return ret;
}

bool ZipUtils::isCCZBuffer(const unsigned char *buffer, KDint32 len)
{
    if (static_cast<size_t>(len) < sizeof(struct CCZHeader))
    {
        return false;
    }
    
    struct CCZHeader *header = (struct CCZHeader*) buffer;
    return header->sig[0] == 'C' && header->sig[1] == 'C' && header->sig[2] == 'Z' && (header->sig[3] == '!' || header->sig[3] == 'p');
}


bool ZipUtils::isGZipFile(const char *path)
{
    // load file into memory
    unsigned char* compressed = NULL;

    KDint32 fileLen = 0;
    compressed = FileUtils::getInstance()->getFileData(path, "rb", &fileLen);

    if(NULL == compressed || 0 == fileLen)
    {
        CCLOG("cocos2d: ZipUtils: loading file failed");
        return false;
    }

    bool ret = isGZipBuffer(compressed, fileLen);
    kdFree(compressed);
    return ret;
}

bool ZipUtils::isGZipBuffer(const unsigned char *buffer, KDint32 len)
{
    if (len < 2)
    {
        return false;
    }

    return buffer[0] == 0x1F && buffer[1] == 0x8B;
}


int ZipUtils::inflateCCZBuffer(const unsigned char *buffer, KDint32 bufferLen, unsigned char **out)
{
    struct CCZHeader *header = (struct CCZHeader*) buffer;

    // verify header
    if( header->sig[0] == 'C' && header->sig[1] == 'C' && header->sig[2] == 'Z' && header->sig[3] == '!' )
    {
        // verify header version
        unsigned int version = CC_SWAP_INT16_BIG_TO_HOST( header->version );
        if( version > 2 )
        {
            CCLOG("cocos2d: Unsupported CCZ header format");
            return -1;
        }

        // verify compression format
        if( CC_SWAP_INT16_BIG_TO_HOST(header->compression_type) != CCZ_COMPRESSION_ZLIB )
        {
            CCLOG("cocos2d: CCZ Unsupported compression method");
            return -1;
        }
    }
    else if( header->sig[0] == 'C' && header->sig[1] == 'C' && header->sig[2] == 'Z' && header->sig[3] == 'p' )
    {
        // encrypted ccz file
        header = (struct CCZHeader*) buffer;

        // verify header version
        unsigned int version = CC_SWAP_INT16_BIG_TO_HOST( header->version );
        if( version > 0 )
        {
            CCLOG("cocos2d: Unsupported CCZ header format");
            return -1;
        }

        // verify compression format
        if( CC_SWAP_INT16_BIG_TO_HOST(header->compression_type) != CCZ_COMPRESSION_ZLIB )
        {
            CCLOG("cocos2d: CCZ Unsupported compression method");
            return -1;
        }

        // decrypt
        unsigned int* ints = (unsigned int*)(buffer+12);
        int enclen = (bufferLen-12)/4;

        decodeEncodedPvr(ints, enclen);

#if COCOS2D_DEBUG > 0
        // verify checksum in debug mode
        unsigned int calculated = checksumPvr(ints, enclen);
        unsigned int required = CC_SWAP_INT32_BIG_TO_HOST( header->reserved );

        if(calculated != required)
        {
            CCLOG("cocos2d: Can't decrypt image file. Is the decryption key valid?");
            return -1;
        }
#endif
    }
    else
    {
        CCLOG("cocos2d: Invalid CCZ file");
        return -1;
    }

    unsigned int len = CC_SWAP_INT32_BIG_TO_HOST( header->len );

    *out = (unsigned char*) kdMalloc( len );
    if(! *out )
    {
        CCLOG("cocos2d: CCZ: Failed to allocate memory for texture");
        return -1;
    }

    unsigned long destlen = len;
    unsigned long source = (unsigned long) buffer + sizeof(*header);
	int ret = uncompress(*out, (uLongf*)&destlen, (Bytef*)source, bufferLen - sizeof(*header));

    if( ret != Z_OK )
    {
        CCLOG("cocos2d: CCZ: Failed to uncompress data");
        kdFree( *out );
        *out = NULL;
        return -1;
    }

    return len;
}

int ZipUtils::inflateCCZFile(const char *path, unsigned char **out)
{
    CCAssert(out, "");
    CCAssert(&*out, "");
    
    // load file into memory
    unsigned char* compressed = NULL;
    
    KDint32 fileLen = 0;
    compressed = FileUtils::getInstance()->getFileData(path, "rb", &fileLen);
    
    if(NULL == compressed || 0 == fileLen)
    {
        CCLOG("cocos2d: Error loading CCZ compressed file");
        return -1;
    }
    
    int ret = inflateCCZBuffer(compressed, fileLen, out);
    kdFree(compressed);
    return ret;
}

void ZipUtils::setPvrEncryptionKeyPart(int index, unsigned int value)
{
    CCASSERT(index >= 0, "Cocos2d: key part index cannot be less than 0");
    CCASSERT(index <= 3, "Cocos2d: key part index cannot be greater than 3");
    
    if(s_uEncryptedPvrKeyParts[index] != value)
    {
        s_uEncryptedPvrKeyParts[index] = value;
        s_bEncryptionKeyIsValid = false;
    }
}

void ZipUtils::setPvrEncryptionKey(unsigned int keyPart1, unsigned int keyPart2, unsigned int keyPart3, unsigned int keyPart4)
{
    setPvrEncryptionKeyPart(0, keyPart1);
    setPvrEncryptionKeyPart(1, keyPart2);
    setPvrEncryptionKeyPart(2, keyPart3);
    setPvrEncryptionKeyPart(3, keyPart4);
}

// --------------------- ZipFile ---------------------
// from unzip.cpp
#define UNZ_MAXFILENAMEINZIP 256

struct ZipEntryInfo
{
    unz_file_pos pos;
    uLong uncompressed_size;
};

class ZipFilePrivate
{
public:
    unzFile zipFile;
    
    // std::unordered_map is faster if available on the platform
    typedef std::map<std::string, struct ZipEntryInfo> FileListContainer;
    FileListContainer fileList;
};

ZipFile::ZipFile(const std::string &zipFile, const std::string &filter)
: m_pData(new ZipFilePrivate)
{
    m_pData->zipFile = unzOpen(zipFile.c_str());
    setFilter(filter);
}

ZipFile::~ZipFile()
{
    if (m_pData && m_pData->zipFile)
    {
        unzClose(m_pData->zipFile);
    }

    CC_SAFE_DELETE(m_pData);
}

bool ZipFile::setFilter(const std::string &filter)
{
    bool ret = false;
    do
    {
        CC_BREAK_IF(!m_pData);
        CC_BREAK_IF(!m_pData->zipFile);
        
        // clear existing file list
        m_pData->fileList.clear();
        
        // UNZ_MAXFILENAMEINZIP + 1 - it is done so in unzLocateFile
        char szCurrentFileName[UNZ_MAXFILENAMEINZIP + 1];
        unz_file_info64 fileInfo;
        
        // go through all files and store position information about the required files
        int err = unzGoToFirstFile64(m_pData->zipFile, &fileInfo,
                                     szCurrentFileName, sizeof(szCurrentFileName) - 1);
        while (err == UNZ_OK)
        {
            unz_file_pos posInfo;
            int posErr = unzGetFilePos(m_pData->zipFile, &posInfo);
            if (posErr == UNZ_OK)
            {
                std::string currentFileName = szCurrentFileName;
                // cache info about filtered files only (like 'assets/')
                if (filter.empty()
                    || currentFileName.substr(0, filter.length()) == filter)
                {
                    ZipEntryInfo entry;
                    entry.pos = posInfo;
                    entry.uncompressed_size = (uLong)fileInfo.uncompressed_size;
                    m_pData->fileList[currentFileName] = entry;
                }
            }
            // next file - also get the information about it
            err = unzGoToNextFile64(m_pData->zipFile, &fileInfo,
                                    szCurrentFileName, sizeof(szCurrentFileName) - 1);
        }
        ret = true;
        
    } while(false);
    
    return ret;
}

bool ZipFile::fileExists(const std::string &fileName) const
{
    bool ret = false;
    do
    {
        CC_BREAK_IF(!m_pData);
        
        ret = m_pData->fileList.find(fileName) != m_pData->fileList.end();
    } while(false);
    
    return ret;
}

unsigned char *ZipFile::getFileData(const std::string &fileName, KDint32 *size)
{
    unsigned char * buffer = NULL;
    if (size)
        *size = 0;

    do
    {
        CC_BREAK_IF(!m_pData->zipFile);
        CC_BREAK_IF(fileName.empty());
        
        ZipFilePrivate::FileListContainer::const_iterator it = m_pData->fileList.find(fileName);
        CC_BREAK_IF(it ==  m_pData->fileList.end());
        
        ZipEntryInfo fileInfo = it->second;
        
        int nRet = unzGoToFilePos(m_pData->zipFile, &fileInfo.pos);
        CC_BREAK_IF(UNZ_OK != nRet);
        
        nRet = unzOpenCurrentFile(m_pData->zipFile);
        CC_BREAK_IF(UNZ_OK != nRet);
        
        buffer = (unsigned char*) kdMalloc(fileInfo.uncompressed_size);
        int CC_UNUSED nSize = unzReadCurrentFile(m_pData->zipFile, buffer, fileInfo.uncompressed_size);
        CCASSERT(nSize == 0 || nSize == (int)fileInfo.uncompressed_size, "the file size is wrong");
        
        if (size)
        {
            *size = fileInfo.uncompressed_size;
        }
        unzCloseCurrentFile(m_pData->zipFile);
    } while (0);
    
    return buffer;
}

NS_CC_END
