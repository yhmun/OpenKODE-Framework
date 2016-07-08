/* --------------------------------------------------------------------------
 *
 *      File            s3tc_decode.c
 *      Author          Young-Hwan Mun
 *                      Jin-Su Park
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

#include "s3tc_decode.h"

/*****************************************************************************
 * Useful structs
 *****************************************************************************/

KDvoid s3tc_decode_block ( KDuint8** blockData, KDuint32* decodeBlockData, KDuint32 stride, KDbool oneBitAlphaFlag, KDuint64 alpha, S3TCDecodeFlag decodeFlag )
{
	KDuint colorValue0 = 0 , colorValue1 = 0, initAlpha = (!oneBitAlphaFlag * 255u) << 24;
	KDuint rb0 = 0, rb1 = 0, rb2 = 0, rb3 = 0, g0 = 0, g1 = 0, g2 = 0, g3 = 0;

	KDuint32 colors[4];
	KDuint32 pixelsIndex = 0;

	/* load the two color values*/
	kdMemcpy ( (KDvoid*) &colorValue0, *blockData, 2 );
	(*blockData) += 2;

	kdMemcpy ( (KDvoid*) &colorValue1, *blockData, 2 );
	(*blockData) += 2;

	/* the channel is r5g6b5 , 16 bits */
	rb0  = (colorValue0 << 19 | colorValue0 >> 8) & 0xf800f8;
	rb1  = (colorValue1 << 19 | colorValue1 >> 8) & 0xf800f8;
	g0   = (colorValue0 << 5) & 0x00fc00;
	g1   = (colorValue1 << 5) & 0x00fc00;
	g0  += (g0 >> 6) & 0x000300;
	g1  += (g1 >> 6) & 0x000300;

	colors[0] = rb0 + g0 + initAlpha;
	colors[1] = rb1 + g1 + initAlpha;

	/* interpolate the other two color values */
	if (colorValue0 > colorValue1 || oneBitAlphaFlag)
	{
		rb2 = (((2*rb0 + rb1) * 21) >> 6) & 0xff00ff;
		rb3 = (((2*rb1 + rb0) * 21) >> 6) & 0xff00ff;
		g2  = (((2*g0 + g1 ) * 21) >> 6) & 0x00ff00;
		g3  = (((2*g1 + g0 ) * 21) >> 6) & 0x00ff00;
		colors[3] = rb3 + g3 + initAlpha;
	}
	else
	{
		rb2 = ((rb0+rb1) >> 1) & 0xff00ff;
		g2  = ((g0 +g1 ) >> 1) & 0x00ff00;
		colors[3] = 0 ;
	}
	colors[2] = rb2 + g2 + initAlpha;

	/*read the pixelsIndex , 2bits per pixel, 4 bytes */
	kdMemcpy((KDvoid*)&pixelsIndex, *blockData, 4);
	(*blockData) += 4;

	if (DXT5 == decodeFlag)
	{
		//dxt5 use interpolate alpha
		// 8-Alpha block: derive the other six alphas.
		// Bit code 000 = alpha0, 001 = alpha1, other are interpolated.

		KDuint alphaArray[8];
		KDint x, y;

		alphaArray[0] = (alpha ) & 0xff ;
		alphaArray[1] = (alpha >> 8) & 0xff ;

		if (alphaArray[0] >= alphaArray[1])
		{
			alphaArray[2] = (alphaArray[0]*6 + alphaArray[1]*1) / 7;
			alphaArray[3] = (alphaArray[0]*5 + alphaArray[1]*2) / 7;
			alphaArray[4] = (alphaArray[0]*4 + alphaArray[1]*3) / 7;
			alphaArray[5] = (alphaArray[0]*3 + alphaArray[1]*4) / 7;
			alphaArray[6] = (alphaArray[0]*2 + alphaArray[1]*5) / 7;
			alphaArray[7] = (alphaArray[0]*1 + alphaArray[1]*6) / 7;
		}
		else if (alphaArray[0] < alphaArray[1])
		{
			alphaArray[2] = (alphaArray[0]*4 + alphaArray[1]*1) / 5;
			alphaArray[3] = (alphaArray[0]*3 + alphaArray[1]*2) / 5;
			alphaArray[4] = (alphaArray[0]*2 + alphaArray[1]*3) / 5;
			alphaArray[5] = (alphaArray[0]*1 + alphaArray[1]*4) / 5;
			alphaArray[6] = 0;
			alphaArray[7] = 255;
		}

		// read the flowing 48bit indices (16*3)
		alpha >>= 16;

		for ( y = 0; y < 4; ++y)
		{
			for ( x = 0; x < 4; ++x)
			{
				decodeBlockData[x] = (alphaArray[alpha & 5] << 24) + colors[pixelsIndex & 3];
				pixelsIndex >>= 2;
				alpha >>= 3;
			}
			decodeBlockData += stride;
		}
	} //if (dxt5 == comFlag)
	else
	{ //dxt1 dxt3 use explicit alpha
		KDint x, y;
		for ( y = 0; y < 4; ++y)
		{
			for ( x = 0; x < 4; ++x)
			{
				initAlpha   = ((alpha) & 0x0f) << 28;
				initAlpha   += initAlpha >> 4;
				decodeBlockData[x] = initAlpha + colors[pixelsIndex & 3];
				pixelsIndex >>= 2;
				alpha       >>= 4;
			}
			decodeBlockData += stride;
		}
	}
}

// Decode S3TC encode data to RGB32
KDvoid s3tc_decode ( KDuint8* encodeData, KDuint8* decodeData, const KDint pixelsWidth, const KDint pixelsHeight, S3TCDecodeFlag decodeFlag )
{
	KDint block_x, block_y;
	KDuint32 *decodeBlockData = (KDuint32 *)decodeData;
	for ( block_y = 0; block_y < pixelsHeight / 4; ++block_y, decodeBlockData += 3 * pixelsWidth)   //stride = 3*width
	{
		for( block_x = 0; block_x < pixelsWidth / 4; ++block_x, decodeBlockData += 4)            //skip 4 pixels
		{
			KDuint64 blockAlpha = 0;

			switch (decodeFlag)
			{
			case DXT1:
				{
					s3tc_decode_block(&encodeData, decodeBlockData, pixelsWidth, 0, 0LL, DXT1);
				}
				break;
			case DXT3:
				{
					kdMemcpy ( (KDvoid*) &blockAlpha, encodeData, 8);
					encodeData += 8;
					s3tc_decode_block(&encodeData, decodeBlockData, pixelsWidth, 1, blockAlpha, DXT3);
				}
				break;
			case DXT5:
				{
					kdMemcpy ( (KDvoid*) &blockAlpha, encodeData, 8);
					encodeData += 8;
					s3tc_decode_block(&encodeData, decodeBlockData, pixelsWidth, 1, blockAlpha, DXT5);
				}
				break;
			default:
				break;
			}//switch
		}//for block_x
	}//for block_y
}

KDuint32 makeFourCC ( KDchar ch0, KDchar ch1, KDchar ch2, KDchar ch3 )
{
	const KDuint32 fourCC = ((KDuint32)(KDchar)(ch0) | ((KDuint32)(KDchar)(ch1) << 8) | ((KDuint32)(KDchar)(ch2) << 16) | ((KDuint32)(KDchar)(ch3) << 24 ));
	return fourCC;
}
