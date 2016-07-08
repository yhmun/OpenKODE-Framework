/* --------------------------------------------------------------------------
 *
 *      File            atx_dxtcomp.c
 *      Description     Functions for compressing RGBA image data to a DXT format. 
 *                      It extends the functionality of the image decoder extensions
 *                      KD_ATX_imgdec and KD_ATX_imgdec_pvr 
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

#include "atx_library.h"

// kdDXTCompressImageATX, kdDXTCompressBufferATX : Compresses an image into a DXT format.
KD_API KDImageATX KD_APIENTRY kdDXTCompressImageATX ( KDImageATX image, KDint32 comptype )
{
	kdSetError ( KD_ENOSYS );

	return 0;
}

KD_API KDImageATX KD_APIENTRY kdDXTCompressBufferATX ( const KDvoid* buffer, KDint32 width, KDint32 height, KDint32 compType, KDint32 levels )
{
	kdSetError ( KD_ENOSYS );

	return 0;
}
