/* --------------------------------------------------------------------------
 *
 *      File            M3GLoader.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 UEDA.Takashi
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#include "M3GInternal.h"
#include <XMM3G/M3G.h>
#include <XMZlib/zlib.h>
#include <algorithm>

M3GObject3DArray M3GLoader::load ( KDFile* pFile, KDoff nOffset )
{
    if ( pFile == NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "File stream is Null." );
    }

	kdFseek ( pFile, 0, KD_SEEK_END );
	KDsize  uSize = kdFtell ( pFile );

    if ( nOffset < 0 || nOffset >= (KDoff) uSize )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Offset is invalid, offset = %d, size = %d.", nOffset, uSize );
    }

	M3GLoader  tLoader = M3GLoader ( pFile );
	
	kdFseek ( pFile, nOffset, KD_SEEK_SET );
	if ( !tLoader.loadM3G ( ) )
	{
		kdFseek ( pFile, nOffset, KD_SEEK_SET );
		tLoader.loadIMG ( );
	}

	return tLoader.m_aObjects;
}

M3GObject3DArray M3GLoader::load ( KDsize uSize, const KDbyte* pData, KDoff nOffset )
{
    if ( !pData ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Data is Null." );
    }

    if ( nOffset < 0 || nOffset >= (KDoff) uSize )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Offset is invalid, offset = %d, size = %d.", nOffset, uSize );
    }

	KDFile*  pFile = kdFmemopen ( (KDvoid*) pData, uSize, "rb" );
	if ( !pFile )
	{
		M3GException ( "IOException", __FUNCTION__, "Can't open the memory file." );
	}

	return load ( pFile, nOffset );
}

M3GObject3DArray M3GLoader::load ( const KDchar* szPath )
{
    if ( !szPath ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "File path is Null." );
    }

	KDFile*  pFile = kdFopen ( szPath, "rb" );
	if ( !pFile )
	{
		M3GException ( "IOException", __FUNCTION__, "Can't open the file, File path = %s.", szPath );
	}

    return load ( pFile, 0 );
}

M3GLoader::M3GLoader ( KDFile* pFile )
: m_pFile ( pFile )
{

}

M3GLoader::~M3GLoader ( KDvoid )
{
	if ( m_pFile )
	{
		kdFclose ( m_pFile );
	}
}

KDbool M3GLoader::loadM3G ( KDvoid )
{
	// M3G(JSR184) identifier
	const KDubyte  M3G_SIG [ 12 ] = { 0xab, 0x4a, 0x53, 0x52, 0x31, 0x38, 0x34, 0xbb, 0x0d, 0x0a, 0x1a, 0x0a };
	KDubyte  btSIG [ 12 ];

	readUbytes ( btSIG, 12 );
	if ( kdMemcmp ( M3G_SIG, btSIG, 12 ) != 0 )
	{
		return KD_FALSE;
	}
	m_aObjects.push_back ( KD_NULL );

	// Read Header
	if ( readSection ( ) == KD_FALSE )
	{
		return KD_TRUE;
	}

	// Read Sections
	while ( kdFEOF ( m_pFile ) != KD_EOF )
	{
		if ( readSection ( ) == KD_FALSE )
		{
			return KD_TRUE;
		}
	}

	m_aObjects.erase ( std::remove ( m_aObjects.begin ( ), m_aObjects.end ( ), (M3GObject3D*) 0 ), m_aObjects.end ( ) );

	return KD_TRUE;
}

KDbool M3GLoader::loadIMG ( KDvoid )
{
	KDFile*         pFile       = m_pFile;
	KDoff			uOffset     = kdFtell ( pFile );
    
	KDImageATX      pImage      = 0;
	KDubyte*        pPixels     = 0;
    
	KDint			nFormat     = 0;
	KDint			nWidth		= 0;
	KDint			nHeight		= 0;
    
	pImage = kdGetImageInfoFromStreamATX ( pFile );
	if ( !pImage )
	{
		M3GException ( "IOException", __FUNCTION__, "This file is unknown." );
		return KD_FALSE;
	}
    
	switch ( kdGetImageIntATX ( pImage, KD_IMAGE_FORMAT_ATX ) )
	{
		case KD_IMAGE_FORMAT_RGBA_ATX :
		case KD_IMAGE_FORMAT_COMPRESSED_ATX :
            
			nFormat = KD_IMAGE_FORMAT_RGBA8888_ATX;
            
			break;
            
        case KD_IMAGE_FORMAT_RGB_ATX  : 
		case KD_IMAGE_FORMAT_PALETTED_ATX :
            
            nFormat = KD_IMAGE_FORMAT_RGB888_ATX;
            
            break;

        case KD_IMAGE_FORMAT_LUMINANCE_ATX :
            
            nFormat = KD_IMAGE_FORMAT_LUM8_ATX;
            
            break;
            
        case KD_IMAGE_FORMAT_LUMALPHA_ATX :
            
            nFormat = KD_IMAGE_FORMAT_LUMALPHA88_ATX;
            
            break;
            
		default :
            
			kdFreeImageATX ( pImage );
			M3GException ( "IOException", __FUNCTION__, "This file is unknown." );

			return KD_FALSE;
	}
    
	kdFreeImageATX ( pImage );
	kdFseek ( pFile, uOffset, KD_SEEK_SET );

	pImage = kdGetImageFromStreamATX ( pFile, nFormat, 0 );
	if ( !pImage )
	{
		M3GException ( "IOException", __FUNCTION__, "This file is invalid." );
		return KD_FALSE;
	}
    
	pPixels = (KDubyte *) kdGetImagePointerATX ( pImage, KD_IMAGE_POINTER_BUFFER_ATX );
	
	switch ( kdGetImageIntATX ( pImage, KD_IMAGE_FORMAT_ATX ) )
	{
		case KD_IMAGE_FORMAT_RGBA8888_ATX	:	nFormat = M3GImage2D::RGBA;				break;
		case KD_IMAGE_FORMAT_RGB888_ATX		:	nFormat = M3GImage2D::RGB;				break;
		case KD_IMAGE_FORMAT_LUM8_ATX		:	nFormat = M3GImage2D::LUMINANCE;		break;
		case KD_IMAGE_FORMAT_LUMALPHA88_ATX :	nFormat = M3GImage2D::LUMINANCE_ALPHA;	break;
	}

	nWidth  = kdGetImageIntATX ( pImage, KD_IMAGE_WIDTH_ATX  );
	nHeight = kdGetImageIntATX ( pImage, KD_IMAGE_HEIGHT_ATX );

	m_aObjects.push_back ( new M3GImage2D ( nFormat, nWidth, nHeight, pPixels ) );

	kdFreeImageATX ( pImage );
    
	return KD_TRUE;
}

KDoff M3GLoader::getOffset ( KDvoid )
{
	return kdFtell ( m_pFile );
}

KDbool M3GLoader::readSection ( KDvoid )
{
	KDbyte  btCompressionScheme = readByte ( );
	KDuint  uTotalSectionLength = readUint ( );
	KDuint  uUncompressedLength = readUint ( );
	KDuint  uChecksum = 0;

	if ( uTotalSectionLength > 13 )
	{
		if ( btCompressionScheme == 0 )
		{
			KDoff  nOffset = getOffset ( );

			do 
			{ 
				if( !readObject ( ) )
				{
					return KD_FALSE;
				}

			} while ( getOffset ( ) < (KDoff) ( nOffset + uTotalSectionLength - 13 ) );

			// Checksum
			uChecksum = readUint ( );

			return KD_TRUE;
		}
		else if ( btCompressionScheme == 1 )
		{
			// Compressed
			KDint	  nCompressLength = uTotalSectionLength - 13;
			KDubyte*  pCompress   = new KDubyte [ nCompressLength ];
			KDubyte*  pUncompress = new KDubyte [ uUncompressedLength ];
			KDFile*   pFile = m_pFile;

			readUbytes ( pCompress, nCompressLength );
			uncompress ( pUncompress, (uLongf*) &uUncompressedLength, pCompress, nCompressLength );

			m_pFile = kdFmemopen ( pUncompress, uUncompressedLength, "rb" );

			do 
			{ 
				if( !readObject ( ) )
				{
					return KD_FALSE;
				}

			} while ( getOffset ( ) < (KDoff) ( uUncompressedLength ) );

			kdFclose ( m_pFile );
			m_pFile = pFile;

			// Checksum
			uChecksum = readUint ( );

			return KD_TRUE;

		}
	}

	return KD_FALSE;
}

KDbool M3GLoader::readObject ( KDvoid )
{
	M3GObject3D*  pObject = KD_NULL;

	KDubyte  btType  = readUbyte ( );
	KDuint   uLength = readUint  ( );
	KDoff    nOffset = getOffset ( );
	KDoff    nOffset2 = 0;

	do
	{
		switch ( btType )
		{
			case   0 : pObject = readHeader				 ( ); break;
			case   1 : pObject = readAnimationController ( ); break;
			case   2 : pObject = readAnimationTrack		 ( ); break;
			case   3 : pObject = readAppearance			 ( ); break;
			case   4 : pObject = readBackground			 ( ); break;
			case   5 : pObject = readCamera				 ( ); break;
			case   6 : pObject = readCompositingMode	 ( ); break;
			case   7 : pObject = readFog	  			 ( ); break;
			case   8 : pObject = readPolygonMode		 ( ); break;
			case   9 : pObject = readGroup				 ( ); break;
			case  10 : pObject = readImage2D			 ( ); break;
			case  11 : pObject = readTriangleStripArray	 ( ); break;
			case  12 : pObject = readLight				 ( ); break;
			case  13 : pObject = readMaterial			 ( ); break;
			case  14 : pObject = readMesh				 ( ); break;
			case  15 : pObject = readMorphingMesh		 ( ); break;
			case  16 : pObject = readSkinnedMesh		 ( ); break;
			case  17 : pObject = readTexture2D			 ( ); break;
			case  18 : pObject = readSprite3D			 ( ); break;
			case  19 : pObject = readKeyframeSequence	 ( ); break;
			case  20 : pObject = readVertexArray		 ( ); break;
			case  21 : pObject = readVertexBuffer		 ( ); break;
			case  22 : pObject = readWorld				 ( ); break;
			case 255 : pObject = readExternalReference	 ( ); break;

			default : 

				kdAssert ( KD_FALSE );

				break;
		}

		m_aObjects.push_back ( pObject );

	} while ( ( nOffset2 = getOffset ( ) ) < (KDoff) ( nOffset + uLength ) );

	return ( nOffset2 == ( nOffset + uLength ) ) ? KD_TRUE : KD_FALSE;
}

M3GObject3D* M3GLoader::readHeader ( KDvoid )
{
	KDubyte  btVersionMajor			 = readUbyte ( );
	KDubyte  btVersionMinor			 = readUbyte ( );
	KDbool   bHasExternalReferences	 = readBool  ( );
	KDuint   uTotalFileSize			 = readUint  ( );             
	KDuint   uApproximateContentSize = readUint  ( );    

	kdLogMessagefKHR 
	(
		"\n[M3G Information]\n* Version = %d.%d\n* Has External References = %d\n* File Size = %d\n* Content Size = %d.\n* Authoring Field\n", 
		btVersionMajor, btVersionMinor,
		bHasExternalReferences, 
		uTotalFileSize, 
		uApproximateContentSize
	);

	kdLogMessage ( readString ( ).c_str ( ) );

	return KD_NULL;
}

M3GObject3D* M3GLoader::readAnimationController ( KDvoid )
{
	M3GAnimationController*  pObject = new M3GAnimationController ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDfloat  fSpeed			= readFloat ( );
	KDfloat  fWeight		= readFloat ( );
	KDint    nIntervalStart = readInt   ( );
	KDint    nIntervalEnd   = readInt   ( );
	KDfloat  fSequenceTime  = readFloat ( );
	KDint    nWorldTime		= readInt   ( );

	pObject->setSpeed ( fSpeed, nWorldTime );
	pObject->setWeight ( fWeight );
	pObject->setActiveInterval ( nIntervalStart, nIntervalEnd );
	pObject->setPosition ( fSequenceTime, nWorldTime );

	return pObject;
}

M3GObject3D* M3GLoader::readAnimationTrack ( KDvoid )
{
	M3GAnimationTrack*  pObject = new M3GAnimationTrack ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDuint  uIdxSequence	= readUint ( );
	KDuint  uIdxController	= readUint ( );
	KDuint  uPropertyID		= readUint ( );

    M3GKeyframeSequence*     pSequence   = dynamic_cast<M3GKeyframeSequence*>    ( m_aObjects [ uIdxSequence ] );
	M3GAnimationController*  pController = dynamic_cast<M3GAnimationController*> ( m_aObjects [ uIdxController ] );

	pObject->init ( pSequence, uPropertyID );
	pObject->setController ( pController );

	return pObject;
}

M3GObject3D* M3GLoader::readAppearance ( KDvoid )
{
	M3GAppearance*  pObject = new M3GAppearance ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDubyte  btLayer		 = readByte ( );
	KDuint   uIdxCompositing = readUint ( );
	KDuint   uIdxFog		 = readUint ( );
	KDuint   uIdxPolygon	 = readUint ( );
	KDuint	 uIdxMaterial	 = readUint ( );

    M3GCompositingMode*  pCompositing = dynamic_cast<M3GCompositingMode*> ( m_aObjects [ uIdxCompositing ] );
    M3GFog*				 pFog		  = dynamic_cast<M3GFog*>			  ( m_aObjects [ uIdxFog ] );
    M3GPolygonMode*      pPolygon	  = dynamic_cast<M3GPolygonMode*>	  ( m_aObjects [ uIdxPolygon ] );
	M3GMaterial*         pMaterial	  = dynamic_cast<M3GMaterial*>		  ( m_aObjects [ uIdxMaterial ] );

	pObject->setLayer ( btLayer );

	if ( pCompositing )
	{
		pObject->setCompositingMode ( pCompositing );
	}

	if ( pFog )
	{
		pObject->setFog ( pFog );
	}

	if ( pPolygon )
	{
		pObject->setPolygonMode ( pPolygon );
	}

	if ( pMaterial )
	{
		pObject->setMaterial ( pMaterial );
	}

	KDuint  uCount = readUint ( );
	for ( KDuint  i = 0; i < uCount; ++i )
	{
		KDuint		   uIdxTexture = readUint ( );
		M3GTexture2D*  pTexture = dynamic_cast<M3GTexture2D*> ( m_aObjects [ uIdxTexture ] );

		if ( pTexture && i < MAX_TEXTURE_UNITS )
		{
			pObject->setTexture ( i, pTexture );
		}
	}

	return pObject;
}

M3GObject3D* M3GLoader::readBackground ( KDvoid )
{
	M3GBackground*  pObject = new M3GBackground ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDuint   uColor		 = readRGBA  ( );
	KDuint   uIdxImage	 = readUint  ( );
	KDubyte  btModeX	 = readUbyte ( ); 
	KDubyte  btModeY	 = readUbyte ( );
	KDuint   uCropX		 = readUint  ( );
	KDuint   uCropY		 = readUint  ( );
	KDuint   uCropW      = readUint  ( );
	KDuint   uCropH      = readUint  ( );
	KDbool   bDepthClear = readBool  ( );
	KDbool   bColorClear = readBool  ( );

	M3GImage2D*  pImage  = dynamic_cast<M3GImage2D*> ( m_aObjects [ uIdxImage ] );

	if ( pImage )
	{
		pObject->setImage ( pImage );
	}

	pObject->setColor ( uColor );
	pObject->setImageMode ( btModeX, btModeY );
	pObject->setCrop ( uCropX, uCropY, uCropW, uCropH ); 
	pObject->setDepthClearEnable ( bDepthClear );
	pObject->setColorClearEnable ( bColorClear );

	return pObject;
}

M3GObject3D* M3GLoader::readCamera ( KDvoid )
{
	M3GCamera*  pObject = new M3GCamera ( );

	if ( !readSubNode ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDubyte  btType = readUbyte ( );

	switch ( btType )
	{
		case M3GCamera::GENERIC :

			{
				M3GTransform  tTransform;
				KDfloat  tMatrix [ 16 ];

				readFloats ( tMatrix, 16 );
				tTransform.set ( tMatrix );

				pObject->setGeneric ( &tTransform );
			}

			break;

		case M3GCamera::PARALLEL :

			{
				KDfloat  fFovy   = readFloat ( );
				KDfloat  fAspect = readFloat ( );
				KDfloat  fNear   = readFloat ( );
				KDfloat  fFar    = readFloat ( );

				pObject->setParallel ( fFovy, fAspect, fNear, fFar );
			}

			break;

		case M3GCamera::PERSPECTIVE :

			{
				KDfloat  fFovy   = readFloat ( );
				KDfloat  fAspect = readFloat ( );
				KDfloat  fNear   = readFloat ( );
				KDfloat  fFar    = readFloat ( );

				pObject->setPerspective ( fFovy, fAspect, fNear, fFar );
			}

			break;
	}

	return pObject;
}

M3GObject3D* M3GLoader::readCompositingMode ( KDvoid )
{
	M3GCompositingMode*  pObject = new M3GCompositingMode ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDbool   bDepthTest			= readBool  ( );
	KDbool   bDepthWrite		= readBool  ( );
	KDbool   bColorWrite		= readBool  ( );
	KDbool   bAlphaWrite		= readBool  ( );
	KDubyte  btBlend			= readUbyte ( );
	KDfloat  fAlphaThreshold	= readUbyte ( ) / 255.f;
	KDfloat  fOffsetFactor		= readFloat ( );
	KDfloat  fOffsetUnits		= readFloat ( );

	pObject->setDepthTestEnable  ( bDepthTest  );
	pObject->setDepthWriteEnable ( bDepthWrite );
	pObject->setColorWriteEnable ( bColorWrite );
	pObject->setAlphaWriteEnable ( bAlphaWrite );
	pObject->setBlending ( btBlend );
	pObject->setAlphaThreshold ( fAlphaThreshold );
	pObject->setDepthOffset ( fOffsetFactor, fOffsetUnits );

	return pObject;
}

M3GObject3D* M3GLoader::readFog ( KDvoid )
{
	M3GFog*  pObject = new M3GFog ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDuint   uColor = readRGB   ( );
	KDubyte  btMode = readUbyte ( );

	pObject->setColor ( uColor );
	pObject->setMode  ( btMode );

	switch ( btMode )
	{
		case M3GFog::EXPONENTIAL :

			{
				KDfloat  fDensity = readFloat ( );

				pObject->setDensity ( fDensity );
			}

			break;

		case M3GFog::LINEAR :

			{
				KDfloat  fNear = readFloat ( );
				KDfloat  fFar  = readFloat ( );

				pObject->setLinear ( fNear, fFar );
			}

			break;
	}

	return pObject;
}

M3GObject3D* M3GLoader::readPolygonMode	( KDvoid )
{
	M3GPolygonMode*  pObject = new M3GPolygonMode ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDubyte  btCulling = readUbyte ( );
	KDubyte  btShading = readUbyte ( );
	KDubyte  btWinding = readUbyte ( );

	KDbool   bTwoSidedLighting		= readBool ( );
	KDbool   bLocalCameraLighting	= readBool ( );
	KDbool   bPerspectiveCorrection = readBool ( );

	pObject->setCulling ( btCulling );
	pObject->setShading ( btShading );
	pObject->setWinding ( btWinding );

	pObject->setTwoSidedLightingEnable ( bTwoSidedLighting );
	pObject->setLocalCameraLightingEnable ( bLocalCameraLighting );
	pObject->setPerspectiveCorrectionEnable ( bPerspectiveCorrection );

	return pObject;
}

M3GObject3D* M3GLoader::readGroup ( KDvoid )
{
	M3GGroup*  pObject = new M3GGroup ( );

	if ( !readSubGroup ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	return pObject;
}

M3GObject3D* M3GLoader::readImage2D ( KDvoid )
{
	M3GImage2D*  pObject = new M3GImage2D ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDubyte   btFormat	 = readUbyte ( );
	KDbool    bIsMutable = readBool  ( );
	KDuint    uWidth	 = readUint  ( );
	KDuint    uHeight	 = readUint  ( );
	KDuint    uSizePalette = 0;
	KDuint    uSizePixels  = 0;
	KDubyte*  pPalette	   = KD_NULL;
	KDubyte*  pPixels	   = KD_NULL;

	if ( !bIsMutable )
	{
		uSizePalette = readUint ( );
		if ( uSizePalette > 0 )
		{
			pPalette = new KDubyte [ uSizePalette ];
			readUbytes ( pPalette, uSizePalette );
		}

		uSizePixels = readUint ( );
		if ( uSizePixels > 0 )
		{
			pPixels = new KDubyte [ uSizePixels ];
			readUbytes ( pPixels, uSizePixels );
		}
	}

	pObject->init ( btFormat, uWidth, uHeight, pPixels, pPalette );

	if ( pPalette )
	{
		delete [] pPalette;
	}

	if ( pPixels )
	{
		delete [] pPixels;
	}

	return pObject;
}

M3GObject3D* M3GLoader::readTriangleStripArray ( KDvoid )
{
	M3GTriangleStripArray*  pObject = new M3GTriangleStripArray ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDubyte  btEncoding = readByte ( );
	KDuint   uStart     = 0;
	KDuint   uCount     = 0;
	KDint*   pIndices   = KD_NULL;
	KDuint   i;

	switch ( btEncoding )
	{
		case 0 :

			uStart = readUint ( );

			break;

		case 1 :

			uStart = readUbyte ( );

			break;

		case 2 :

			uStart = readUshort ( );

			break;

		case 128 :

			uCount = readUint ( );
			pIndices = new KDint [ uCount ];

			readUbytes ( (KDubyte*) pIndices, uCount * 4 );

			break;

		case 129 :

			uCount = readUint ( );
			pIndices = new KDint [ uCount ];

			{
				KDubyte*  pValues = new KDubyte [ uCount ];

				readUbytes ( pValues, uCount );

				for ( i = 0; i < uCount; i++ )
				{
					pIndices [ i ] = pValues [ i ];
				}

				delete [] pValues;
			}

			break;

		case 130 :

			uCount = readUint ( );
			pIndices = new KDint [ uCount ];

			{
				KDushort*  pValues = new KDushort [ uCount ];

				readUshorts ( pValues, uCount );

				for ( i = 0; i < uCount; i++ )
				{
					pIndices [ i ] = pValues [ i ];
				}

				delete [] pValues;
			}

			break;

	}
    
	KDuint  uLengths = readUint ( );
	KDint*  pLengths = new KDint [ uLengths ];
	readUbytes ( (KDubyte*) pLengths, uLengths * 4 );

	if ( pIndices )
	{
		pObject->init ( uCount, pIndices, uLengths, pLengths );
	}
	else
	{
		pObject->init ( uStart, uLengths, pLengths );
	}

	return pObject;
}

M3GObject3D* M3GLoader::readLight ( KDvoid )
{
	M3GLight*  pObject = new M3GLight ( );

	if ( !readSubNode ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDfloat  fConstant	 = readFloat ( );
	KDfloat  fLinear	 = readFloat ( );
	KDfloat  fQuardratic = readFloat ( );
	KDuint   uColor		 = readRGB   ( );
	KDubyte  btMode		 = readUbyte ( );
	KDfloat  fIntensity  = readFloat ( );
	KDfloat  fAngle		 = readFloat ( );
	KDfloat  fExponent   = readFloat ( );

	pObject->setAttenuation ( fConstant, fLinear, fQuardratic );
	pObject->setColor ( uColor );
	pObject->setMode ( btMode );
	pObject->setIntensity ( fIntensity );
	pObject->setSpotAngle ( fAngle );
	pObject->setSpotExponent ( fExponent );

	return pObject;
}

M3GObject3D* M3GLoader::readMaterial ( KDvoid )
{
	M3GMaterial*  pObject = new M3GMaterial ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

    KDuint  uAmbient	= readRGB   ( );
    KDuint  uDiffuse	= readRGBA  ( );
    KDuint  uEmissive	= readRGB   ( );
    KDuint  uSpecular	= readRGB   ( );
    KDfloat fShininess	= readFloat ( );
    KDbool  bTracking	= readBool  ( );

    pObject->setColor     ( M3GMaterial::DIFFUSE , uDiffuse  );
    pObject->setColor     ( M3GMaterial::AMBIENT , uAmbient  );
    pObject->setColor     ( M3GMaterial::EMISSIVE, uEmissive );
    pObject->setColor     ( M3GMaterial::SPECULAR, uSpecular );
    pObject->setShininess ( fShininess );
    pObject->setVertexColorTrackingEnable ( bTracking );

	return pObject;
}

M3GObject3D* M3GLoader::readMesh ( KDvoid )
{
	M3GMesh*  pObject = new M3GMesh ( );

	if ( !readSubMesh ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	return pObject;
}

M3GObject3D* M3GLoader::readMorphingMesh ( KDvoid )
{
	M3GMorphingMesh*  pObject = new M3GMorphingMesh ( );

	if ( !readSubMesh ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDuint			   uCount = readUint ( );
	M3GVertexBuffer**  aTargets = new M3GVertexBuffer* [ uCount ];
	KDfloat*		   pWeights = new KDfloat [ uCount ];
	KDuint             i;

	for ( i = 0; i < uCount; i++ )
	{
		KDuint   uIndex  = readUint  ( );
		KDfloat  fWeight = readFloat ( );

		aTargets [ i ] = dynamic_cast<M3GVertexBuffer*> ( m_aObjects [ uIndex ] );
		pWeights [ i ] = fWeight;
	}

	pObject->init ( uCount, aTargets );
	pObject->setWeights ( uCount, pWeights );

	delete [] aTargets;
	delete [] pWeights;

	return pObject;
}

M3GObject3D* M3GLoader::readSkinnedMesh ( KDvoid )
{
	M3GSkinnedMesh*  pObject = new M3GSkinnedMesh ( );

	if ( !readSubMesh ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDuint  uIdxSkeleton   = readUint ( );
	KDuint  uTransRefCount = readUint ( );
	KDuint  i;

	M3GGroup*  pSkeleton = dynamic_cast<M3GGroup*> ( m_aObjects [ uIdxSkeleton ] );
	pObject->init ( pSkeleton );

    for ( i = 0; i < uTransRefCount; i++ ) 
	{
		KDuint   uIndex  = readUint ( );
		KDuint   uFirst  = readUint ( );
		KDuint   uCount  = readUint ( );
		KDint    nWeight = readInt  ( );
		M3GNode*  pNode  = dynamic_cast<M3GNode*> ( m_aObjects [ uIndex ] );

		pObject->addTransform ( pNode, nWeight, uFirst, uCount );
    }

	return pObject;
}

M3GObject3D* M3GLoader::readTexture2D ( KDvoid )
{
	M3GTexture2D*  pObject = new M3GTexture2D ( );

	if ( !readSubTransformable ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDuint   uIdxImage		= readUint  ( );
	KDuint   uBlendColor	= readRGB   ( );
	KDubyte  btBlending		= readUbyte ( );
	KDubyte  btWrappingS	= readUbyte ( );
	KDubyte  btWrappingT	= readUbyte ( );
	KDubyte  btLevelFilter	= readUbyte ( );
	KDubyte  btImageFilter	= readUbyte ( );

    M3GImage2D*  pImage = dynamic_cast<M3GImage2D*> ( m_aObjects [ uIdxImage ] );
	
	pObject->init ( pImage );
	pObject->setBlendColor ( uBlendColor );
	pObject->setBlending   ( btBlending );
	pObject->setWrapping   ( btWrappingS, btWrappingT );
	pObject->setFiltering  ( btLevelFilter, btImageFilter );

	return pObject;
}

M3GObject3D* M3GLoader::readSprite3D ( KDvoid )
{
	M3GSprite3D*  pObject = new M3GSprite3D ( );

	if ( !readSubNode ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDuint  uIdxImage		= readUint ( );
	KDuint  uIdxAppearance	= readUint ( );
	KDbool  bScaled			= readBool ( );
	KDint   nCropX			= readInt  ( );
	KDint   nCropY			= readInt  ( );
	KDint   nCropW			= readInt  ( );
	KDint   nCropH			= readInt  ( );

    M3GImage2D*     pImage	    = dynamic_cast<M3GImage2D*>    ( m_aObjects [ uIdxImage ] );
    M3GAppearance*  pAppearance = dynamic_cast<M3GAppearance*> ( m_aObjects [ uIdxAppearance ] );

	pObject->init ( bScaled, pImage, pAppearance );
	pObject->setCrop ( nCropX, nCropY, nCropW, nCropH );

	return pObject;
}

M3GObject3D* M3GLoader::readKeyframeSequence ( KDvoid )
{
	M3GKeyframeSequence*  pObject = new M3GKeyframeSequence ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDubyte	  btInterpolation	= readUbyte ( );
	KDubyte   btRepeatMode		= readUbyte ( );
	KDubyte   btEncoding		= readUbyte ( );
	KDuint    uDuration			= readUint ( );
	KDuint    uValidRangeFirst	= readUint ( );
	KDuint    uValidRangeLast	= readUint ( );
	KDuint    uComponentCount	= readUint ( );
	KDuint	  uKeyframeCount	= readUint ( );

	KDfloat*  fValues = new KDfloat [ uComponentCount ];
	KDuint    uTime	  = 0;

	KDuint    i, j;

	pObject->init ( uKeyframeCount, uComponentCount, btInterpolation );
	pObject->setDuration ( uDuration );
	pObject->setRepeatMode ( btRepeatMode );
	pObject->setValidRange ( uValidRangeFirst, uValidRangeLast );

	switch ( btEncoding )
	{
		case 0 :
		
			for ( i = 0; i < uKeyframeCount; ++i )
			{
				uTime = readUint ( );
				readFloats ( fValues, uComponentCount );

				pObject->setKeyframe ( i, uTime, fValues );
			}

			break;

		case 1 :
		{
			KDfloat*  fBias	  = new KDfloat [ uComponentCount ];
			KDfloat*  fScale  = new KDfloat [ uComponentCount ];
			KDubyte*  pValues = new KDubyte [ uComponentCount ];

			readFloats ( fBias , uComponentCount );
			readFloats ( fScale, uComponentCount );
			
			for ( i = 0; i < uKeyframeCount; ++i )
			{
				uTime = readUint ( );
				readUbytes ( pValues, uComponentCount );

				for ( j = 0 ; j < uComponentCount ; ++j )
				{
					fValues [ j ] = pValues [ i ] * fBias [ i ] * fScale [ i ];
				}

				pObject->setKeyframe ( i, uTime, fValues );
			}
			
			delete [] fBias;
			delete [] fScale;
			delete [] pValues;
		
			break;
		}

		case 2 :
		{
			KDfloat*   fBias   = new KDfloat  [ uComponentCount ];
			KDfloat*   fScale  = new KDfloat  [ uComponentCount ];
			KDushort*  pValues = new KDushort [ uComponentCount ];

			for ( i = 0; i < uKeyframeCount; ++i )
			{
				uTime = readUint ( );
				readUshorts ( pValues, uComponentCount );

				for ( j = 0 ; j < uComponentCount ; ++j )
				{
					fValues [ j ] = pValues [ i ] * fBias [ i ] * fScale [ i ];
				}

				pObject->setKeyframe ( i, uTime, fValues );
			}

			delete [] fBias;
			delete [] fScale;
			delete [] pValues;

			break;
		}
	}

	delete [] fValues;

	return pObject;
}

M3GObject3D* M3GLoader::readVertexArray ( KDvoid )
{
	M3GVertexArray*  pObject = new M3GVertexArray ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDubyte   btSize	 = readUbyte  ( );
	KDubyte   btCount	 = readUbyte  ( );
	KDubyte   btEncoding = readUbyte  ( );
	KDushort  uVertex    = readUshort ( );

	union 
	{
		KDubyte*	pValues8;
		KDshort*	pValues16;
		KDfloat*	pValues32;
	};	

	KDuint    i, j;

	pValues8 = new KDubyte [ btSize * btCount * uVertex ];
	readUbytes ( pValues8, btSize * btCount * uVertex );

	if ( btEncoding != 0 )
	{
		KDuint  uOffset = 0;
		KDint   nDelta = 0;
		KDfloat fDelta = 0;

		switch ( btSize )
		{
			case 1 :

				for ( i = 0; i < uVertex; ++i )
				{
					for ( j = 0; j < btCount; ++j )
					{
						uOffset = i * btCount + j;
						pValues8 [ uOffset ] += nDelta; 
						nDelta = pValues8 [ uOffset ];
					}
				}

				break;

			case 2 :

				for ( i = 0; i < uVertex; ++i )
				{
					for ( j = 0; j < btCount; ++j )
					{
						uOffset = i * btCount + j;
						pValues16 [ uOffset ] += nDelta; 
						nDelta = pValues16 [ uOffset ];
					}
				}

				break;

			case 4 :

				for ( i = 0; i < uVertex; ++i )
				{
					for ( j = 0; j < btCount; ++j )
					{
						uOffset = i * btCount + j;
						pValues32 [ uOffset ] += fDelta; 
						fDelta = pValues32 [ uOffset ];
					}
				}

				break;
		}
	}

	pObject->init ( uVertex, btCount, btSize );

	switch ( btSize )
	{
		case 1 : pObject->set ( 0, uVertex, pValues8  ); break;
		case 2 : pObject->set ( 0, uVertex, pValues16 ); break;
		case 4 : pObject->set ( 0, uVertex, pValues32 ); break;
	}

	delete pValues8;

	return pObject;
}

M3GObject3D* M3GLoader::readVertexBuffer ( KDvoid )
{
	M3GVertexBuffer*  pObject = new M3GVertexBuffer ( );

	if ( !readSubObject3D ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDuint  uDefaultColor  = readRGBA  ( );
	KDuint  uIdxPositions  = readUint  ( );

	KDfloat fPositionBias [ 3 ];
	readFloats ( fPositionBias, 3 );
	KDfloat fPositionScale = readFloat ( );

	KDuint  uIdxNormals    = readUint  ( );
	KDuint  uIdxColors     = readUint  ( );

    M3GVertexArray*  pColors    = dynamic_cast<M3GVertexArray*> ( m_aObjects [ uIdxColors ] );
    M3GVertexArray*  pNormals   = dynamic_cast<M3GVertexArray*> ( m_aObjects [ uIdxNormals ] );
    M3GVertexArray*  pPositions = dynamic_cast<M3GVertexArray*> ( m_aObjects [ uIdxPositions ] );

	pObject->setDefaultColor ( uDefaultColor );

	if ( pPositions )
	{
        if ( pPositions->getComponentType ( ) == 1 )
		{
            pPositions->convert ( 2 );
        }

        pObject->setPositions ( pPositions, fPositionScale, fPositionBias );
	}

	if ( pNormals )
	{
		pObject->setNormals ( pNormals );
	}

	if ( pColors )
	{
		pObject->setColors ( pColors );
	}

	KDuint  uCount = readUint ( );
	for ( KDuint  i = 0; i < uCount; i++ )
	{
		KDuint     uIdxCoords = readUint ( );
		KDfloat	   fCoordBias [ 3 ];
		readFloats ( fCoordBias, 3 );
		KDfloat	   fCoordScale = readFloat ( );

		M3GVertexArray*  pCoords = dynamic_cast<M3GVertexArray*> ( m_aObjects [ uIdxCoords ] );

		if ( pCoords && i < MAX_TEXTURE_UNITS )
		{
			pObject->setTexCoords ( i, pCoords, fCoordScale, fCoordBias );
		}
	}

	return pObject;
}

M3GObject3D* M3GLoader::readWorld ( KDvoid )
{
	M3GWorld*  pObject = new M3GWorld ( );

	if ( !readSubGroup ( pObject ) )
	{
		delete pObject;
		return KD_NULL;
	}

	KDuint  uIdxCamera     = readUint ( );
	KDuint  uIdxBackground = readUint ( );

    M3GCamera*      pCamera     = dynamic_cast<M3GCamera*>     ( m_aObjects [ uIdxCamera ] );
    M3GBackground*  pBackground = dynamic_cast<M3GBackground*> ( m_aObjects [ uIdxBackground ] );

    if ( pCamera )
	{
        pObject->setActiveCamera ( pCamera );
    }

    if ( pBackground )
	{
        pObject->setBackground ( pBackground );
    }

	return pObject;
}

M3GObject3D* M3GLoader::readExternalReference ( KDvoid )
{
	std::string  sUrl = readString ( );

	kdLogMessage ( sUrl.c_str ( ) );

	return KD_NULL;
}

KDbool M3GLoader::readSubObject3D ( M3GObject3D* pObject )
{
	KDuint  uUserID;
	KDuint  uCount;
	KDuint  i;
	
	// User ID
	uUserID = readUint ( );
	pObject->setUserID ( uUserID );

	// AnimationTracks Table
	uCount = readUint ( );
	for ( i = 0; i < uCount; ++i )
	{
		KDuint  uIndex = readUint ( );

		M3GAnimationTrack*  pTrack = dynamic_cast<M3GAnimationTrack*> ( m_aObjects [ uIndex ] );
		pObject->addAnimationTrack ( pTrack );
	}

	// User parameters
	uCount = readUint ( );
	if ( uCount > 0 )
	{
		std::map<KDuint, KDbyte*>*  aUserObjects = new std::map<KDuint, KDbyte*> ( );
		for ( i = 0; i < uCount; ++i )
		{
			KDuint   uID   = readUint ( );
			KDuint   uSize = readUint ( );
			KDbyte*  pData = new KDbyte [ uSize ];

			readBytes ( pData, uSize );

			aUserObjects->insert ( std::map<KDint, KDbyte*>::value_type ( uID, pData ) );
		}

		pObject->setUserObject ( aUserObjects );
	}

	return KD_TRUE;
}

KDbool M3GLoader::readSubTransformable ( M3GTransformable* pObject )
{
	if ( !readSubObject3D ( pObject ) )
	{
		return KD_FALSE;
	}

	KDbool  bTransform;

	// Component
	bTransform = readBool ( );
	if ( bTransform )
	{
		KDfloat  fTrans [ 3 ];
		KDfloat  fScale [ 3 ];
		KDfloat  fAngle [ 4 ];

		readFloats ( fTrans, 3 );
		readFloats ( fScale, 3 );
		readFloats ( fAngle, 4 );

		pObject->setTranslation ( fTrans [ 0 ], fTrans [ 1 ], fTrans [ 2 ] );
		pObject->setScale		( fScale [ 0 ], fScale [ 1 ], fScale [ 2 ] );
		pObject->setOrientation ( fAngle [ 0 ], fAngle [ 1 ], fAngle [ 2 ], fAngle [ 3 ] );
	}

	// General
	bTransform = readBool ( );
	if ( bTransform )
	{
		KDfloat  fMatrix [ 16 ];
		M3GTransform  tTransform;

		readFloats ( fMatrix, 16 );

		tTransform.set ( fMatrix );
		pObject->setTransform ( &tTransform );
	}

	return KD_TRUE;
}

KDbool M3GLoader::readSubNode ( M3GNode* pObject )
{
	if ( !readSubTransformable ( pObject ) )
	{
		return KD_FALSE;
	}

	KDbool   bRendering		= readBool  ( );
	KDbool   bPicking		= readBool  ( );
	KDfloat  fAlphaFactor	= readUbyte ( ) / 255.f;
	KDuint   uScope			= readUint  ( );
	KDbool	 bAlignment		= readBool  ( );

	pObject->setRenderingEnable ( bRendering );
	pObject->setPickingEnable ( bPicking );
	pObject->setAlphaFactor ( fAlphaFactor );
	pObject->setScope ( uScope );

    if ( bAlignment ) 
	{
		KDubyte   btZTarget = readUbyte ( );
		KDubyte   btYTarget = readUbyte ( );
		KDuint    uIdxZRef  = readUint  ( );
		KDuint    uIdxYRef  = readUint  ( );

        M3GNode*  pZRef = dynamic_cast<M3GNode*> ( m_aObjects [ uIdxZRef ] );
        M3GNode*  pYRef = dynamic_cast<M3GNode*> ( m_aObjects [ uIdxYRef ] );

        pObject->setAlignment ( pZRef, btZTarget, pYRef, btYTarget );
    }

	return KD_TRUE;
}

KDbool M3GLoader::readSubGroup ( M3GGroup* pObject )
{
	if ( !readSubNode ( pObject ) )
	{
		return KD_FALSE;
	}

	KDuint  uCount = readUint ( );
	KDuint  i;

	for ( i = 0; i < uCount; i++ )
	{
		KDuint    uIndex = readUint ( );
		M3GNode*  pNode = dynamic_cast<M3GNode*> ( m_aObjects [ uIndex ] );

		pObject->addChild ( pNode );
	}

	return KD_TRUE;
}

KDbool M3GLoader::readSubMesh ( M3GMesh* pObject )
{
	if ( !readSubNode ( pObject ) )
	{
		return KD_FALSE;
	}

	KDuint  uIdxVertex  = readUint ( );
	KDuint  uCount      = readUint ( );

	M3GVertexBuffer*   pVertices    = dynamic_cast<M3GVertexBuffer*> ( m_aObjects [ uIdxVertex ] );
	M3GIndexBuffer**   aSubMeshes   = new M3GIndexBuffer* [ uCount ];
	M3GAppearance**    aAppearances = new M3GAppearance* [ uCount ];

	for ( KDuint  i = 0; i < uCount; ++i )
	{
		KDuint  uIdxSubMesh    = readUint ( );
		KDuint  uIdxAppearance = readUint ( );

		aSubMeshes   [ i ] = dynamic_cast<M3GIndexBuffer*> ( m_aObjects [ uIdxSubMesh ] );
		aAppearances [ i ] = dynamic_cast<M3GAppearance*>  ( m_aObjects [ uIdxAppearance ] );
	}

	pObject->init ( pVertices, uCount, aSubMeshes, uCount, aAppearances );

	delete [] aSubMeshes;
	delete [] aAppearances;

	return KD_TRUE;
}

KDbool M3GLoader::readBool ( KDvoid )
{
	KDbool  bValue;

	kdFread ( &bValue, 1, 1, m_pFile );

	return bValue;
}

KDubyte M3GLoader::readUbyte ( KDvoid )
{
	KDubyte  btValue;

	kdFread ( &btValue, 1, 1, m_pFile );

	return btValue;
}

KDushort M3GLoader::readUshort ( KDvoid )
{
	KDushort  uValue;

	kdFread ( &uValue, 2, 1, m_pFile );

	return uValue;
}

KDuint M3GLoader::readUint ( KDvoid )
{
	KDuint  uValue;

	kdFread ( &uValue, 4, 1, m_pFile );

	return uValue;
}

KDbyte M3GLoader::readByte ( KDvoid )
{
	KDubyte  btValue;

	kdFread ( &btValue, 1, 1, m_pFile );

	return btValue;
}

KDshort M3GLoader::readShort ( KDvoid )
{
	KDshort  nValue;

	kdFread ( &nValue, 2, 1, m_pFile );

	return nValue;
}

KDint M3GLoader::readInt ( KDvoid )
{
	KDint  nValue;

	kdFread ( &nValue, 4, 1, m_pFile );

	return nValue;
}

KDfloat M3GLoader::readFloat ( KDvoid )
{
	KDfloat  fValue;

	kdFread ( &fValue, 4, 1, m_pFile );

	return fValue;
}

KDuint M3GLoader::readRGB ( KDvoid )
{
    KDubyte  r, g, b;

    kdFread ( &r, 1, 1, m_pFile );
    kdFread ( &g, 1, 1, m_pFile );
    kdFread ( &b, 1, 1, m_pFile );

    return ( r << 16 ) | ( g << 8 ) | ( b << 0 );
}

KDuint M3GLoader::readRGBA ( KDvoid )
{
    KDubyte  r, g, b, a;

    kdFread ( &r, 1, 1, m_pFile );
    kdFread ( &g, 1, 1, m_pFile );
    kdFread ( &b, 1, 1, m_pFile );
	kdFread ( &a, 1, 1, m_pFile );

    return ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b << 0 );
}

std::string	M3GLoader::readString ( KDvoid )
{
	std::string  sRet;
	KDchar       c;

	do 
	{
		kdFread ( &c, 1, 1, m_pFile );

		sRet.push_back ( c );

	} while( c != 0 );

	return sRet;
}

KDvoid M3GLoader::readUbytes ( KDubyte* pValues, KDsize uSize )
{
	kdFread ( pValues, 1 * uSize, 1, m_pFile );
}

KDvoid	M3GLoader::readBytes ( KDbyte* pValues, KDsize uSize )
{
	kdFread ( pValues, 1 * uSize, 1, m_pFile );
}

KDvoid M3GLoader::readUshorts ( KDushort* pValues, KDsize uSize )
{
	kdFread ( pValues, 2 * uSize, 1, m_pFile );
}

KDvoid M3GLoader::readFloats ( KDfloat* pValues, KDsize uSize )
{
	kdFread ( pValues, 4 * uSize, 1, m_pFile );
}
