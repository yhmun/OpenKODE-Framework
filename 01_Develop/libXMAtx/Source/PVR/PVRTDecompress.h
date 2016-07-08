/* --------------------------------------------------------------------------
 *
 *      File            PVRTDecompress.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
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

#ifndef __PVRTDecompress_h__
#define __PVRTDecompress_h__

#include "../atx_library.h"

/*****************************************************************************
 * Useful structs
 *****************************************************************************/

typedef KDuint8				PVRTuint8;
typedef KDint32				PVRTint32;
typedef KDuint32			PVRTuint32;
typedef KDuint64			PVRTuint64;

/*****************************************************************************
* Legacy constants (V1/V2)
*****************************************************************************/

static const PVRTuint32 PVRTEX_MIPMAP				= (1<<8);		// has mip map levels
static const PVRTuint32 PVRTEX_TWIDDLE				= (1<<9);		// is twiddled
static const PVRTuint32 PVRTEX_BUMPMAP				= (1<<10);		// has normals encoded for a bump map
static const PVRTuint32 PVRTEX_TILING				= (1<<11);		// is bordered for tiled pvr
static const PVRTuint32 PVRTEX_CUBEMAP				= (1<<12);		// is a cubemap/skybox
static const PVRTuint32 PVRTEX_FALSEMIPCOL			= (1<<13);		// are there false coloured MIP levels
static const PVRTuint32 PVRTEX_VOLUME				= (1<<14);		// is this a volume texture
static const PVRTuint32 PVRTEX_ALPHA				= (1<<15);		// v2.1 is there transparency info in the texture
static const PVRTuint32 PVRTEX_VERTICAL_FLIP		= (1<<16);		// v2.1 is the texture vertically flipped

static const PVRTuint32 PVRTEX_PIXELTYPE			= 0xff;			// pixel type is always in the last 16bits of the flags
static const PVRTuint32 PVRTEX_IDENTIFIER			= 0x21525650;	// the pvr identifier is the characters 'P','V','R'

static const PVRTuint32 PVRTEX_V1_HEADER_SIZE		= 44;			// old header size was 44 for identification purposes
static const PVRTuint32 PVRTEX_V2_HEADER_SIZE		= 52;
static const PVRTuint32 PVRTEX_V3_HEADER_SIZE		= 52;

static const PVRTuint32 PVRTC2_MIN_TEXWIDTH			= 16;
static const PVRTuint32 PVRTC2_MIN_TEXHEIGHT		= 8;
static const PVRTuint32 PVRTC4_MIN_TEXWIDTH			= 8;
static const PVRTuint32 PVRTC4_MIN_TEXHEIGHT		= 8;
static const PVRTuint32 ETC_MIN_TEXWIDTH			= 4;
static const PVRTuint32 ETC_MIN_TEXHEIGHT			= 4;
static const PVRTuint32 DXT_MIN_TEXWIDTH			= 4;
static const PVRTuint32 DXT_MIN_TEXHEIGHT			= 4;

static const KDchar		PVRTexIdentifier[5]			= "PVR!";

// V3 Header Identifiers.
static const PVRTuint32 PVRTEX3_IDENT				= 0x03525650;	// 'P''V''R'3
static const PVRTuint32 PVRTEX3_IDENT_REV			= 0x50565203;   

// PVR Header file flags.											Condition if true. If false, opposite is true unless specified.
static const PVRTuint32 PVRTEX3_FILE_COMPRESSED		= (1<<0);		//	Texture has been file compressed using PVRTexLib (currently unused)
static const PVRTuint32 PVRTEX3_PREMULTIPLIED		= (1<<1);		//	Texture has been premultiplied by alpha value.	

// v3
//Compressed pixel formats
enum EPVRTPixelFormat
{
	ePVRTPF_PVRTCI_2bpp_RGB,
	ePVRTPF_PVRTCI_2bpp_RGBA,
	ePVRTPF_PVRTCI_4bpp_RGB,
	ePVRTPF_PVRTCI_4bpp_RGBA,
	ePVRTPF_PVRTCII_2bpp,
	ePVRTPF_PVRTCII_4bpp,
	ePVRTPF_ETC1,
	ePVRTPF_DXT1,
	ePVRTPF_DXT2,
	ePVRTPF_DXT3,
	ePVRTPF_DXT4,
	ePVRTPF_DXT5,

	//These formats are identical to some DXT formats.
	ePVRTPF_BC1 = ePVRTPF_DXT1,
	ePVRTPF_BC2 = ePVRTPF_DXT3,
	ePVRTPF_BC3 = ePVRTPF_DXT5,

	//These are currently unsupported:
	ePVRTPF_BC4,
	ePVRTPF_BC5,
	ePVRTPF_BC6,
	ePVRTPF_BC7,

	//These are supported
	ePVRTPF_UYVY,
	ePVRTPF_YUY2,
	ePVRTPF_BW1bpp,
	ePVRTPF_SharedExponentR9G9B9E5,
	ePVRTPF_RGBG8888,
	ePVRTPF_GRGB8888,
	ePVRTPF_ETC2_RGB,
	ePVRTPF_ETC2_RGBA,
	ePVRTPF_ETC2_RGB_A1,
	ePVRTPF_EAC_R11,
	ePVRTPF_EAC_RG11,

	//Invalid value
	ePVRTPF_NumCompressedPFs
};

/* supported channel type formats */
#define kPVR3TexturePixelFormat_BGRA_8888			0x0808080861726762ULL
#define kPVR3TexturePixelFormat_RGBA_8888			0x0808080861626772ULL
#define kPVR3TexturePixelFormat_RGBA_4444			0x0404040461626772ULL
#define kPVR3TexturePixelFormat_RGBA_5551			0x0105050561626772ULL
#define kPVR3TexturePixelFormat_RGB_565				0x0005060500626772ULL
#define kPVR3TexturePixelFormat_RGB_888				0x0008080800626772ULL
#define kPVR3TexturePixelFormat_A_8					0x0000000800000061ULL
#define kPVR3TexturePixelFormat_L_8					0x000000080000006cULL
#define kPVR3TexturePixelFormat_LA_88				0x000008080000616cULL

/*****************************************************************************
* Legacy (V2 and V1) ENUMS
*****************************************************************************/

enum PVRTPixelType
{
	MGLPT_ARGB_4444 = 0x00,
	MGLPT_ARGB_1555,
	MGLPT_RGB_565,
	MGLPT_RGB_555,
	MGLPT_RGB_888,
	MGLPT_ARGB_8888,
	MGLPT_ARGB_8332,
	MGLPT_I_8,
	MGLPT_AI_88,
	MGLPT_1_BPP,
	MGLPT_VY1UY0,
	MGLPT_Y1VY0U,
	MGLPT_PVRTC2,
	MGLPT_PVRTC4,

	// OpenGL version of pixel types
	OGL_RGBA_4444= 0x10,
	OGL_RGBA_5551,
	OGL_RGBA_8888,
	OGL_RGB_565,
	OGL_RGB_555,
	OGL_RGB_888,
	OGL_I_8,
	OGL_AI_88,
	OGL_PVRTC2,
	OGL_PVRTC4,
	OGL_BGRA_8888,
	OGL_A_8,
	OGL_PVRTCII4,	//Not in use
	OGL_PVRTCII2,	//Not in use

	// S3TC Encoding
	D3D_DXT1 = 0x20,
	D3D_DXT2,
	D3D_DXT3,
	D3D_DXT4,
	D3D_DXT5,

	//RGB Formats
	D3D_RGB_332,
	D3D_AL_44,
	D3D_LVU_655,
	D3D_XLVU_8888,
	D3D_QWVU_8888,
	
	//10 bit integer - 2 bit alpha
	D3D_ABGR_2101010,
	D3D_ARGB_2101010,
	D3D_AWVU_2101010,

	//16 bit integers
	D3D_GR_1616,
	D3D_VU_1616,
	D3D_ABGR_16161616,

	//Float Formats
	D3D_R16F,
	D3D_GR_1616F,
	D3D_ABGR_16161616F,

	//32 bits per channel
	D3D_R32F,
	D3D_GR_3232F,
	D3D_ABGR_32323232F,
	
	// Ericsson
	ETC_RGB_4BPP,
	ETC_RGBA_EXPLICIT,				// unimplemented
	ETC_RGBA_INTERPOLATED,			// unimplemented
	
	D3D_A8 = 0x40,
	D3D_V8U8,
	D3D_L16,
			
	D3D_L8,
	D3D_AL_88,

	//Y'UV Colourspace
	D3D_UYVY,
	D3D_YUY2,
	
	// DX10
	DX10_R32G32B32A32_FLOAT= 0x50,
	DX10_R32G32B32A32_UINT , 
	DX10_R32G32B32A32_SINT,

	DX10_R32G32B32_FLOAT,
	DX10_R32G32B32_UINT,
	DX10_R32G32B32_SINT,

	DX10_R16G16B16A16_FLOAT ,
	DX10_R16G16B16A16_UNORM,
	DX10_R16G16B16A16_UINT ,
	DX10_R16G16B16A16_SNORM ,
	DX10_R16G16B16A16_SINT ,

	DX10_R32G32_FLOAT ,
	DX10_R32G32_UINT ,
	DX10_R32G32_SINT ,

	DX10_R10G10B10A2_UNORM ,
	DX10_R10G10B10A2_UINT ,

	DX10_R11G11B10_FLOAT ,				// unimplemented

	DX10_R8G8B8A8_UNORM , 
	DX10_R8G8B8A8_UNORM_SRGB ,
	DX10_R8G8B8A8_UINT ,
	DX10_R8G8B8A8_SNORM ,
	DX10_R8G8B8A8_SINT ,

	DX10_R16G16_FLOAT , 
	DX10_R16G16_UNORM , 
	DX10_R16G16_UINT , 
	DX10_R16G16_SNORM ,
	DX10_R16G16_SINT ,

	DX10_R32_FLOAT ,
	DX10_R32_UINT ,
	DX10_R32_SINT ,

	DX10_R8G8_UNORM ,
	DX10_R8G8_UINT ,
	DX10_R8G8_SNORM , 
	DX10_R8G8_SINT ,

	DX10_R16_FLOAT ,
	DX10_R16_UNORM ,
	DX10_R16_UINT ,
	DX10_R16_SNORM ,
	DX10_R16_SINT ,

	DX10_R8_UNORM, 
	DX10_R8_UINT,
	DX10_R8_SNORM,
	DX10_R8_SINT,

	DX10_A8_UNORM, 
	DX10_R1_UNORM, 
	DX10_R9G9B9E5_SHAREDEXP,	// unimplemented
	DX10_R8G8_B8G8_UNORM,		// unimplemented
	DX10_G8R8_G8B8_UNORM,		// unimplemented

	DX10_BC1_UNORM,	
	DX10_BC1_UNORM_SRGB,

	DX10_BC2_UNORM,	
	DX10_BC2_UNORM_SRGB,

	DX10_BC3_UNORM,	
	DX10_BC3_UNORM_SRGB,

	DX10_BC4_UNORM,				// unimplemented
	DX10_BC4_SNORM,				// unimplemented

	DX10_BC5_UNORM,				// unimplemented
	DX10_BC5_SNORM,				// unimplemented

	// OpenVG

	/* RGB{A,X} channel ordering */
	ePT_VG_sRGBX_8888  = 0x90,
	ePT_VG_sRGBA_8888,
	ePT_VG_sRGBA_8888_PRE,
	ePT_VG_sRGB_565,
	ePT_VG_sRGBA_5551,
	ePT_VG_sRGBA_4444,
	ePT_VG_sL_8,
	ePT_VG_lRGBX_8888,
	ePT_VG_lRGBA_8888,
	ePT_VG_lRGBA_8888_PRE,
	ePT_VG_lL_8,
	ePT_VG_A_8,
	ePT_VG_BW_1,

	/* {A,X}RGB channel ordering */
	ePT_VG_sXRGB_8888,
	ePT_VG_sARGB_8888,
	ePT_VG_sARGB_8888_PRE,
	ePT_VG_sARGB_1555,
	ePT_VG_sARGB_4444,
	ePT_VG_lXRGB_8888,
	ePT_VG_lARGB_8888,
	ePT_VG_lARGB_8888_PRE,

	/* BGR{A,X} channel ordering */
	ePT_VG_sBGRX_8888,
	ePT_VG_sBGRA_8888,
	ePT_VG_sBGRA_8888_PRE,
	ePT_VG_sBGR_565,
	ePT_VG_sBGRA_5551,
	ePT_VG_sBGRA_4444,
	ePT_VG_lBGRX_8888,
	ePT_VG_lBGRA_8888,
	ePT_VG_lBGRA_8888_PRE,

	/* {A,X}BGR channel ordering */
	ePT_VG_sXBGR_8888,
	ePT_VG_sABGR_8888 ,
	ePT_VG_sABGR_8888_PRE,
	ePT_VG_sABGR_1555,
	ePT_VG_sABGR_4444,
	ePT_VG_lXBGR_8888,
	ePT_VG_lABGR_8888,
	ePT_VG_lABGR_8888_PRE,

	// max cap for iterating
	END_OF_PIXEL_TYPES,

	MGLPT_NOTYPE = 0xffffffff

};



#pragma pack(push,4)
typedef struct 
{
	PVRTuint32	u32Version;			//Version of the file header, used to identify it.
	PVRTuint32	u32Flags;			//Various format flags.
	PVRTuint64	u64PixelFormat;		//The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
	PVRTuint32	u32ColourSpace;		//The Colour Space of the texture, currently either linear RGB or sRGB.
	PVRTuint32	u32ChannelType;		//Variable type that the channel is stored in. Supports signed/unsigned int/short/byte or float for now.
	PVRTuint32	u32Height;			//Height of the texture.
	PVRTuint32	u32Width;			//Width of the texture.
	PVRTuint32	u32Depth;			//Depth of the texture. (Z-slices)
	PVRTuint32	u32NumSurfaces;		//Number of members in a Texture Array.
	PVRTuint32	u32NumFaces;		//Number of faces in a Cube Map. Maybe be a value other than 6.
	PVRTuint32	u32MIPMapCount;		//Number of MIP Maps in the texture - NB: Includes top level.
	PVRTuint32	u32MetaDataSize;	//Size of the accompanying meta data.
} PVRTextureHeaderV3;
#pragma pack(pop)


/*!***************************************************************************
Describes the Version 2 header of a PVR texture header.
*****************************************************************************/
typedef struct 
{
	PVRTuint32  uHeaderSize;		/*!< size of the structure */
	PVRTuint32  uHeight;			/*!< height of surface to be created */
	PVRTuint32  uWidth;				/*!< width of input surface */
	PVRTuint32  uMipMapCount;		/*!< number of mip-map levels requested */
	PVRTuint32  upfFlags;			/*!< pixel format flags */
	PVRTuint32  uTextureDataSize;	/*!< Total size in bytes */
	PVRTuint32  uBitCount;			/*!< number of bits per pixel  */
	PVRTuint32  uRBitMask;			/*!< mask for red bit */
	PVRTuint32  uGBitMask;			/*!< mask for green bits */
	PVRTuint32  uBBitMask;			/*!< mask for blue bits */
	PVRTuint32  uAlphaBitMask;		/*!< mask for alpha channel */
	PVRTuint32  uPVR;				/*!< magic number identifying pvr file */
	PVRTuint32  uNumSurfs;			/*!< the number of surfaces present in the pvr */
} PVR_Texture_Header;

/*!***********************************************************************
 @Function		PVRTDecompressPVRTC
 @Input			pCompressedData The PVRTC texture data to decompress
 @Input			Do2bitMode Signifies whether the data is PVRTC2 or PVRTC4
 @Input			XDim X dimension of the texture
 @Input			YDim Y dimension of the texture
 @Return		Returns the amount of data that was decompressed.
 @Modified		pResultImage The decompressed texture data
 @Description	Decompresses PVRTC to RGBA 8888
*************************************************************************/
int PVRTDecompressPVRTC(const void *pCompressedData,
				const int Do2bitMode,
				const int XDim,
				const int YDim,
				unsigned char* pResultImage);

/*!***********************************************************************
@Function		PVRTDecompressETC
@Input			pSrcData The ETC texture data to decompress
@Input			x X dimension of the texture
@Input			y Y dimension of the texture
@Modified		pDestData The decompressed texture data
@Input			nMode The format of the data
@Returns		The number of bytes of ETC data decompressed
@Description	Decompresses ETC to RGBA 8888
*************************************************************************/
int PVRTDecompressETC(const void * const pSrcData,
						 const unsigned int* x,
						 const unsigned int* y,
						 void *pDestData,
						 const int* nMode);


#endif	// __PVRTDecompress_h__