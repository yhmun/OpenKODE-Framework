/* -----------------------------------------------------------------------------------
 *
 *      File            ZipUtils.h
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

#ifndef __ZipUtils_h__
#define __ZipUtils_h__

#include "../../ccTypes.h"

#include <string>

NS_CC_BEGIN
    
/** @struct CCZHeader */
struct CCZHeader 
{
	KDuint8			sig[4];				// signature. Should be 'CCZ!' 4 bytes
	KDuint16		compression_type;	// should 0
	KDuint8		    version;			// should be 2 (although version type==1 is also supported)
	KDuint 		    reserved;			// Reserved for users.
	KDuint		    len;				// size of the uncompressed file
};

enum 
{
	CCZ_COMPRESSION_ZLIB,				// zlib format.
	CCZ_COMPRESSION_BZIP2,				// bzip2 format (not supported yet)
	CCZ_COMPRESSION_GZIP,				// gzip format (not supported yet)
	CCZ_COMPRESSION_NONE,				// plain (not supported yet)
};

class ZipUtils
{
	public :
	
		/** 
		 *	Inflates either zlib or gzip deflated memory. The inflated memory is
		 *	expected to be freed by the caller.
		 *
		 *	It will allocate 256k for the destination buffer. If it is not enough it will multiply the previous buffer size per 2, until there is enough memory.
		 *	@returns the length of the deflated buffer
		 *
		 *	@since   v0.8.1
		 */
		static KDint ccInflateMemory ( KDubyte* pSrc, KDuint uLenSrc, KDubyte** ppDst );
		
		/** 
		 *  Inflates either zlib or gzip deflated memory. The inflated memory is
		 *  expected to be freed by the caller.
		 *
		 *	outLenghtHint is assumed to be the needed room to allocate the inflated buffer.
		 *
		 *	@returns the length of the deflated buffer
		 *
		 *	@since	 v1.0.0
		 */
		static KDint ccInflateMemoryWithHint ( KDubyte* pSrc, KDuint uLenSrc, KDubyte** ppDst, KDuint uLenHintDst );
		
		/** 
		 *	inflates a GZip file into memory
		 *
		 *	@returns the length of the deflated buffer
		 *
		 *	@since	 v0.99.5
		 */
		static KDint ccInflateGZipFile ( const KDchar* szFilePath, KDubyte** ppDst );

		/** 
		 *	inflates a CCZ file into memory
		 *
		 *	@returns the length of the deflated buffer
		 *
		 *	@since	 v0.99.5
		 */
		static KDint ccInflateCCZFile ( const KDchar* szFilePath, KDubyte** ppDst );

	private :
		
		static KDint ccInflateMemoryWithHint ( KDubyte* pSrc, KDuint uLenSrc, KDubyte** ppDst, KDuint* pLenDst, KDuint uLenHintDst );

};
 
// forward declaration
class ZipFilePrivate;

/**
 *	Zip file - reader helper class.
 *
 *	It will cache the file list of a particular zip file with positions inside an archive,
 *	so it would be much faster to read some particular files or to check their existance.
 *
 *	@since v2.0.5
 */
class ZipFile
{
	public :

		/**
		 *	Constructor, open zip file and store file list.
		 *
		 *	@param zipFile	Zip file name
		 *	@param filter	The first part of file names, which should be accessible.
		 *		            For example, "assets/". Other files will be missed.
		 *
		 *	@since v2.0.5
		 */
		ZipFile ( const std::string& sZipFile, const std::string& sFilter = std::string ( ) );
		virtual ~ZipFile ( KDvoid );

		/**
		 *	Regenerate accessible file list based on a new filter string.
		 *
		 *	@param  filter New filter string (first part of files names)
		 *	@return true	whenever zip file is open successfully and it is possible to locate
		 *					at least the first file, false otherwise
		 *
		 *	@since v2.0.5
		 */
		KDbool	setFilter ( const std::string& sFilter );

		/**
		 *	Check does a file exists or not in zip file
		 *
		 *	@param  fileName File to be checked on existance
		 *	@return true whenever file exists, false otherwise
		 *
		 *	@since v2.0.5
		 */
		KDbool  fileExists ( const std::string& sFileName ) const;

		/**
		 *	Get resource file data from a zip file.
		 *	@param fileName File name
		 *	@param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
		 *	@return Upon success, a pointer to the data is returned, otherwise NULL.
		 *	@warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
		 *
		 *	@since v2.0.5
		 */
		KDubyte*  getFileData ( const std::string& sFileName, KDsize* pSize );

	private :

		/** Internal data like zip file pointer / file list array and so on */
		ZipFilePrivate*		m_pData;
};

NS_CC_END

#endif // __ZipUtils_h__

