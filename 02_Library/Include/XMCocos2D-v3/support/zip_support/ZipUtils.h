/* -----------------------------------------------------------------------------------
 *
 *      File            ZipUtils.h
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

#ifndef __ZipUtils_h__
#define __ZipUtils_h__

#include <string>
#include "../../platform/CCPlatformConfig.h"
#include "../../platform/CCPlatformDefine.h"
#include "../../platform/CCPlatformMacros.h"

NS_CC_BEGIN

/* XXX: pragma pack ??? */
/** 
 *	@struct CCZHeader
 */
struct CCZHeader 
{
    KDubyte			sig [ 4 ];          // signature. Should be 'CCZ!' 4 bytes
    KDushort		compression_type;   // should 0
    KDushort		version;            // should be 2 (although version type==1 is also supported)
    KDuint			reserved;           // Reserved for users.
    KDuint			len;                // size of the uncompressed file
};

enum
{
    CCZ_COMPRESSION_ZLIB,               // zlib format.
    CCZ_COMPRESSION_BZIP2,              // bzip2 format (not supported yet)
    CCZ_COMPRESSION_GZIP,               // gzip format (not supported yet)
    CCZ_COMPRESSION_NONE,               // plain (not supported yet)
};

class CC_DLL ZipUtils
{
    public :

        /** 
         *	Inflates either zlib or gzip deflated memory. The inflated memory is
         *	expected to be freed by the caller.
         *
         *	It will allocate 256k for the destination buffer. If it is not enough it will multiply the previous buffer size per 2, until there is enough memory.
         *	@returns the length of the deflated buffer
         *
         *	@since v0.8.1
         */
        static KDint			inflateMemory ( KDubyte* in, KDint32 inLength, KDubyte** out );

        /** 
         *	Inflates either zlib or gzip deflated memory. The inflated memory is
         *	expected to be freed by the caller.
         *
         *	outLenghtHint is assumed to be the needed room to allocate the inflated buffer.
         *
         *	@returns the length of the deflated buffer
         *
         *	@since v1.0.0
         */
        static KDint			inflateMemoryWithHint ( KDubyte* in, KDint32 inLength, KDubyte** out, KDint32 outLengthHint );

        /**
		 *	inflates a GZip file into memory
         *
         *	@returns the length of the deflated buffer
         *
         *	@since v0.99.5
         */        
        static KDint			inflateGZipFile ( const KDchar* filename, KDubyte** out );
        
        /** 
		 *	test a file is a GZip format file or not
         *
         *	@returns true is a GZip format file. false is not
         *
         *	@since v3.0
         */        
        static KDbool			isGZipFile ( const KDchar* filename );

        /** 
		 *	test the buffer is GZip format or not
         *
         *	@returns true is GZip format. false is not
         *
         *	@since v3.0
         */        
        static KDbool			isGZipBuffer ( const KDubyte* buffer, KDint32 len );

        /**
		 *	inflates a CCZ file into memory
         *
         *	@returns the length of the deflated buffer
         *
         *	@since v0.99.5
         */        
        static KDint			inflateCCZFile ( const KDchar* filename, KDubyte** out );

        /**
		 *	inflates a buffer with CCZ format into memory
         *
         *	@returns the length of the deflated buffer
         *
         *	@since v3.0
         */        
        static KDint			inflateCCZBuffer ( const KDubyte* buffer, KDint32 len, KDubyte **out );
        
        /** 
		 *	test a file is a CCZ format file or not
         *
         *	@returns true is a CCZ format file. false is not
         *
         *	@since v3.0
         */
        static KDbool			isCCZFile ( const KDchar* filename );

        /** 
		 *	test the buffer is CCZ format or not
         *
         *	@returns true is CCZ format. false is not
         *
         *	@since v3.0
         */        
        static KDbool			isCCZBuffer ( const KDubyte* buffer, KDint32 len );

        /**
		 *	Sets the pvr.ccz encryption key parts separately for added
         *	security.
         *
         *	Example: If the key used to encrypt the pvr.ccz file is
         *	0xaaaaaaaabbbbbbbbccccccccdddddddd you will call this function 4 
         *	different times, preferably from 4 different source files, as follows
         *
         *	ZipUtils::setPvrEncryptionKeyPart(0, 0xaaaaaaaa);
         *	ZipUtils::setPvrEncryptionKeyPart(1, 0xbbbbbbbb);
         *	ZipUtils::setPvrEncryptionKeyPart(2, 0xcccccccc);
         *	ZipUtils::setPvrEncryptionKeyPart(3, 0xdddddddd);
         *
         *	Splitting the key into 4 parts and calling the function
         *	from 4 different source files increases the difficulty to
         *	reverse engineer the encryption key. Be aware that encrpytion 
         *	is *never* 100% secure and the key code can be cracked by
         *	knowledgable persons.
         *
         *	IMPORTANT: Be sure to call setPvrEncryptionKey or
         *	setPvrEncryptionKeyPart with all of the key parts *before* loading
         *	the spritesheet or decryption will fail and the spritesheet
         *	will fail to load.
         *
         *	@param index part of the key [0..3]
         *	@param value value of the key part
         */        
        static KDvoid			setPvrEncryptionKeyPart ( KDint index, KDuint value );
        
        /** 
		 *	Sets the pvr.ccz encryption key.
         *
         *	Example: If the key used to encrypt the pvr.ccz file is
         *	0xaaaaaaaabbbbbbbbccccccccdddddddd you will call this function with
         *	the key split into 4 parts as follows
         *
         *	ZipUtils::setPvrEncryptionKey(0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc, 0xdddddddd);
         *
         *	Note that using this function makes it easier to reverse engineer and
         *	discover the complete key because the key parts are present in one 
         *	function call.
         *
         *	IMPORTANT: Be sure to call setPvrEncryptionKey or
         *	setPvrEncryptionKeyPart with all of the key parts *before* loading
         *	the spritesheet or decryption will fail and the spritesheet
         *	will fail to load.
         *
         *	@param keyPart1 the key value part 1.
         *	@param keyPart2 the key value part 2.
         *	@param keyPart3 the key value part 3.
         *	@param keyPart4 the key value part 4.
         */        
        static KDvoid			setPvrEncryptionKey ( KDuint keyPart1, KDuint keyPart2, KDuint keyPart3, KDuint keyPart4 );

    private :

        static KDint			inflateMemoryWithHint ( KDubyte* in, KDint32 inLength, KDubyte** out, KDint32* outLength, KDint32 outLenghtHint );
        static inline KDvoid	decodeEncodedPvr ( KDuint* pData, KDint32 nLen );
        static inline KDuint	checksumPvr ( const KDuint* pData, KDint32 nLen );

        static KDuint			s_uEncryptedPvrKeyParts [ 4 ];
        static KDuint			s_uEncryptionKey [ 1024 ];
        static KDbool			s_bEncryptionKeyIsValid;
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
         *	@param zipFile Zip file name
         *	@param filter The first part of file names, which should be accessible.
         *                For example, "assets/". Other files will be missed.
         *
         *	@since v2.0.5
         */
        ZipFile ( const std::string& sZipFile, const std::string& sFilter = std::string ( ) );

        virtual ~ZipFile ( KDvoid );

        /**
         *	Regenerate accessible file list based on a new filter string.
         *
         *	@param filter New filter string (first part of files names)
         *	@return true whenever zip file is open successfully and it is possible to locate
         *               at least the first file, false otherwise
         *
         *	@since v2.0.5
         */
        KDbool				setFilter ( const std::string& sFilter );

        /**
         *	Check does a file exists or not in zip file
         *
         *	@param fileName File to be checked on existance
         *	@return true whenever file exists, false otherwise
         *
         *	@since v2.0.5
         */
        KDbool				fileExists ( const std::string& sFileName ) const;

        /**
         *	Get resource file data from a zip file.
         *	@param fileName File name
         *	@param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
         *	@return Upon success, a pointer to the data is returned, otherwise NULL.
         *	@warning Recall: you are responsible for calling free() on any Non-NULL pointer returned.
         *
         *	@since v2.0.5
         */
        KDubyte*			getFileData ( const std::string& sFileName, KDint32* pSize );

    private :

        /** Internal data like zip file pointer / file list array and so on */
        ZipFilePrivate*		m_pData;
};

NS_CC_END

#endif	// __ZipUtils_h__
