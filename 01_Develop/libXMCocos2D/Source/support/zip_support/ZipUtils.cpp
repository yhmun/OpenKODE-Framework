/* -----------------------------------------------------------------------------------
 *
 *      File            ZipUtils.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "support/zip_support/ZipUtils.h"
#include "support/CCFileUtils.h"
#include "cocoa/CCString.h"

#include <XMZlib/zlib.h>
#include <XMZlib/contrib/minizip/unzip.h>

#include <map>

NS_CC_BEGIN
    
// --------------------- ZipUtils ---------------------
// memory in iPhone is precious
// Should buffer factor be 1.5 instead of 2 ?
#define BUFFER_INC_FACTOR (2)

KDint ZipUtils::ccInflateMemoryWithHint ( KDubyte* pSrc, KDuint uLenSrc, KDubyte** ppDst, KDuint* pLenDst, KDuint uLenHintDst )
{
	// ret value 
	KDint  nErr = Z_OK;

	KDint  nBufferSize = uLenHintDst;
	*ppDst = new KDubyte [ nBufferSize ];

	z_stream d_stream; // decompression stream
	d_stream.zalloc = (alloc_func) 0;
	d_stream.zfree  = (free_func) 0;
	d_stream.opaque = (voidpf) 0;

	d_stream.next_in   = pSrc;
	d_stream.avail_in  = uLenSrc;
	d_stream.next_out  = *ppDst;
	d_stream.avail_out = nBufferSize;

	// window size to hold 256k
	if ( ( nErr = inflateInit2 ( &d_stream, 15 + 32 ) ) != Z_OK )
	{
		return nErr;
	}

	for ( ; ; ) 
	{
		nErr = inflate ( &d_stream, Z_NO_FLUSH );

		if ( nErr == Z_STREAM_END )
		{
			break;
		}

		switch ( nErr ) 
		{
			case Z_NEED_DICT :
				nErr = Z_DATA_ERROR;
			case Z_DATA_ERROR :
			case Z_MEM_ERROR :
				inflateEnd ( &d_stream );
				return nErr;
	    }

		// not enough memory ?
		if ( nErr != Z_STREAM_END ) 
		{
            delete[] *ppDst;
            *ppDst = new KDubyte [ nBufferSize * BUFFER_INC_FACTOR ];

			// not enough memory, ouch
			if ( !*ppDst ) 
			{
				CCLOG ( "XMCocos2D : ZipUtils - realloc failed" );
				inflateEnd ( &d_stream );
				return Z_MEM_ERROR;
			}

			d_stream.next_out = *ppDst + nBufferSize;
			d_stream.avail_out = nBufferSize;
			nBufferSize *= BUFFER_INC_FACTOR;
		}
	}

	*pLenDst = nBufferSize - d_stream.avail_out;
	nErr = inflateEnd ( &d_stream );
	return nErr;
}

KDint ZipUtils::ccInflateMemoryWithHint ( KDubyte* pSrc, KDuint uLenSrc, KDubyte** ppDst, KDuint uLenHintDst )
{
	KDuint  uLenDst = 0;
	KDint   nErr = ccInflateMemoryWithHint ( pSrc, uLenSrc, ppDst, &uLenDst, uLenHintDst );

	if ( nErr != Z_OK || *ppDst == KD_NULL )
	{
		if ( nErr == Z_MEM_ERROR )
		{
			CCLOG ( "XMCocos2D : ZipUtils - Out of memory while decompressing map data!");
		} 
		else if ( nErr == Z_VERSION_ERROR )
		{
			CCLOG ( "XMCocos2D : ZipUtils - Incompatible zlib version!");
		} 
		else if ( nErr == Z_DATA_ERROR )
		{
			CCLOG ( "XMCocos2D : ZipUtils - Incorrect zlib compressed data!" );
		}
		else
		{
			CCLOG ( "XMCocos2D : ZipUtils - Unknown error while decompressing map data!" );
		}

		CC_SAFE_DELETE_ARRAY ( *ppDst );
		uLenDst = 0;
	}

	return uLenDst;
}

KDint ZipUtils::ccInflateMemory ( KDubyte* pSrc, KDuint uLenSrc, KDubyte** ppDst )
{
	// 256k for hint
	return ccInflateMemoryWithHint ( pSrc, uLenSrc, ppDst, 256 * 1024 );
}

KDint ZipUtils::ccInflateGZipFile ( const KDchar* szFilePath, KDubyte** ppDst )
{
	KDint   nLen;
	KDuint  uOffset = 0;

	CCAssert ( ppDst  , "" );
	CCAssert ( &*ppDst, "" );

	gzFile  pGzFile = gzopen ( szFilePath, "rb" );
	if ( pGzFile == KD_NULL ) 
	{
		//CCLOG ( "XMCocos2D : ZipUtils - error open gzip file : %s", szFilePath );
		return -1;
	}

	// 512k initial decompress buffer 
	KDuint  uBufferSize = 512 * 1024;
	KDuint  uTotalBufferSize = uBufferSize;

	*ppDst = (KDubyte *) kdMalloc ( uBufferSize );
	if ( !ppDst ) 
	{
		CCLOG ( "XMCocos2D : ZipUtils - out of memory");
		return -1;
	}

	for ( ; ; ) 
	{
		nLen = gzread ( pGzFile, *ppDst + uOffset, uBufferSize );
		if ( nLen < 0 ) 
		{
			// CCLOG ( "XMCocos2D : ZipUtils - error in gzread" );
			kdFree ( *ppDst );
			*ppDst = KD_NULL;
			return -1;
		}
		if ( nLen == 0 )
		{
			break;
		}

		uOffset += nLen;

		// finish reading the file
		if ( (KDuint) nLen < uBufferSize )
		{
			break;
		}

		uBufferSize *= BUFFER_INC_FACTOR;
		uTotalBufferSize += uBufferSize;
		KDubyte*  pTemp = (KDubyte *) kdRealloc ( *ppDst, uTotalBufferSize );

		if ( !pTemp ) 
		{
			CCLOG ( "XMCocos2D : ZipUtils - out of memory" );
			kdFree ( *ppDst );
			*ppDst = KD_NULL;
			return -1;
		}

		*ppDst = pTemp;
	}

	if ( gzclose ( pGzFile ) != Z_OK )
	{
		CCLOG ( "XMCocos2D : ZipUtils - gzclose failed" );
	}

	return uOffset;
}

KDint ZipUtils::ccInflateCCZFile ( const KDchar* szFilePath, KDubyte** ppDst )
{
	CCAssert ( ppDst  , "" );
	CCAssert ( &*ppDst, "" );

	// load file into memory
	KDubyte*  pCompressed = KD_NULL;
        
    KDint  nLenFile = 0;
    pCompressed = CCFileUtils::sharedFileUtils ( )->getFileData ( szFilePath, "rb", (KDsize *) ( &nLenFile ) );

	if ( KD_NULL == pCompressed || nLenFile == 0 ) 
	{
		//CCLOG ( "XMCocos2D : Error loading CCZ compressed file" );
        return -1;
	}
 
	struct CCZHeader*  pHeader = (struct CCZHeader*) pCompressed;

	// verify header
	if ( pHeader->sig[0] != 'C' || pHeader->sig[1] != 'C' || pHeader->sig[2] != 'Z' || pHeader->sig[3] != '!' ) 
	{
		// CCLOG ( "XMCocos2D : Invalid CCZ file" );
		CC_SAFE_DELETE_ARRAY ( pCompressed );
		return -1;
	}
 
	// verify header version
	KDuint  uVersion = CC_SWAP_INT16_BIG_TO_HOST ( pHeader->version );
	if ( uVersion > 2 ) 
	{
		CCLOG ( "XMCocos2D : Unsupported CCZ header format" );
		CC_SAFE_DELETE_ARRAY ( pCompressed );
		return -1;
	}
 
	// verify compression format
	if ( CC_SWAP_INT16_BIG_TO_HOST ( pHeader->compression_type ) != CCZ_COMPRESSION_ZLIB ) 
	{
		CCLOG ( "XMCocos2D : CCZ Unsupported compression method" );
		CC_SAFE_DELETE_ARRAY ( pCompressed );
		return -1;
	}

	KDuint  uLen = CC_SWAP_INT32_BIG_TO_HOST ( pHeader->len );
 
	*ppDst = (KDubyte*) kdMalloc ( uLen );
	if ( !*ppDst )
	{
		CCLOG ( "XMCocos2D : CCZ - Failed to allocate memory for texture" );
		CC_SAFE_DELETE_ARRAY ( pCompressed );
		return -1;
	}

	KDsize  uLenDst = uLen;
	KDsize  uSizeSrc = (KDsize) pCompressed + sizeof ( *pHeader );
	KDint   nRet = uncompress ( *ppDst, (uLongf *) &uLenDst, (Bytef *) uSizeSrc, nLenFile - sizeof ( *pHeader ) );

	CC_SAFE_DELETE_ARRAY ( pCompressed );

	if ( nRet != Z_OK )
	{
		CCLOG ( "XMCocos2D : CCZ - Failed to uncompress data" );
		CC_SAFE_FREE ( *ppDst );
		return -1;
	}

	return uLen;
}
 
// --------------------- ZipFile ---------------------
// from unzip.cpp
#define UNZ_MAXFILENAMEINZIP 256

struct ZipEntryInfo
{
    unz_file_pos		pos;
    uLong				uncompressed_size;
};

class ZipFilePrivate
{
	public :

		unzFile												zipFile;

		// std::unordered_map is faster if available on the platform
		typedef std::map<std::string, struct ZipEntryInfo>	FileListContainer;
		FileListContainer									fileList;
};

ZipFile::ZipFile ( const std::string& sZipFile, const std::string& sFilter )
{
	m_pData = new ZipFilePrivate;

    m_pData->zipFile = unzOpen ( sZipFile.c_str ( ) );
    if ( m_pData->zipFile )
    {
        setFilter ( sFilter );
    }
}

ZipFile::~ZipFile ( KDvoid )
{
    if ( m_pData && m_pData->zipFile )
    {
        unzClose ( m_pData->zipFile );
    }

    CC_SAFE_DELETE ( m_pData );
}

KDbool ZipFile::setFilter ( const std::string& sFilter )
{
    KDbool  bRet = KD_FALSE;
    do
    {
        CC_BREAK_IF ( !m_pData );
        CC_BREAK_IF ( !m_pData->zipFile );

        // clear existing file list
        m_pData->fileList.clear ( );

        // UNZ_MAXFILENAMEINZIP + 1 - it is done so in unzLocateFile
        KDchar  szCurrentFileName [ UNZ_MAXFILENAMEINZIP + 1 ];
        unz_file_info64  tFileInfo;

        // go through all files and store position information about the required files
        KDint   nErr = unzGoToFirstFile64 ( m_pData->zipFile, &tFileInfo, szCurrentFileName, sizeof ( szCurrentFileName ) - 1 );
        while ( nErr == UNZ_OK )
        {
            unz_file_pos  tPosInfo;
            KDint  nPosErr = unzGetFilePos ( m_pData->zipFile, &tPosInfo );
            if ( nPosErr == UNZ_OK )
            {
                std::string  sCurrentFileName = szCurrentFileName;
                // cache info about filtered files only (like 'assets/')
                if ( sFilter.empty ( ) || sCurrentFileName.substr ( 0, sFilter.length ( ) ) == sFilter )
                {
                    ZipEntryInfo  tEntry;
                    tEntry.pos = tPosInfo;
                    tEntry.uncompressed_size = (uLong) tFileInfo.uncompressed_size;
                    m_pData->fileList [ szCurrentFileName ] = tEntry;
                }
            }

            // next file - also get the information about it
            nErr = unzGoToNextFile64 ( m_pData->zipFile, &tFileInfo, szCurrentFileName, sizeof ( szCurrentFileName ) - 1 );
        }

        bRet = KD_TRUE;

    } while ( 0 );

    return bRet;
}

KDbool ZipFile::fileExists ( const std::string& sFileName ) const
{
    KDbool  bRet = KD_FALSE;
	
    do
    {
        CC_BREAK_IF ( !m_pData );

        bRet = m_pData->fileList.find ( sFileName) != m_pData->fileList.end ( );

    } while ( 0 );

    return bRet;
}

KDubyte* ZipFile::getFileData ( const std::string& sFileName, KDsize* pSize )
{
    KDubyte*  pBuffer = KD_NULL;

    if ( pSize )
    {
        *pSize = 0;
    }

    do
    {
        CC_BREAK_IF ( !m_pData->zipFile );
        CC_BREAK_IF ( sFileName.empty ( ) );

        ZipFilePrivate::FileListContainer::const_iterator  it = m_pData->fileList.find ( sFileName );
        CC_BREAK_IF ( it ==  m_pData->fileList.end ( ) );

        ZipEntryInfo  tFileInfo = it->second;

        KDint  nRet = unzGoToFilePos ( m_pData->zipFile, &tFileInfo.pos );
        CC_BREAK_IF ( UNZ_OK != nRet );

        nRet = unzOpenCurrentFile ( m_pData->zipFile );
        CC_BREAK_IF ( UNZ_OK != nRet );

        pBuffer = new KDubyte [ tFileInfo.uncompressed_size ];
        KDint  nSize = 0;
        nSize = unzReadCurrentFile ( m_pData->zipFile, pBuffer, tFileInfo.uncompressed_size );
        CCAssert ( nSize == 0 || nSize == (KDint) tFileInfo.uncompressed_size, "the file size is wrong" );

        if ( pSize )
        {
            *pSize = tFileInfo.uncompressed_size;
        }
        unzCloseCurrentFile ( m_pData->zipFile );

    } while ( 0 );

    return pBuffer;
}

NS_CC_END
