/* --------------------------------------------------------------------------
 *
 *      File            kd_attribute.c
 *      Description     Versioning and attribute queries
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
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

#include "kd_library.h"

// kdQueryAttribi : Obtain the value of a numeric XMKode attribute. 
KD_API KDint KD_APIENTRY kdQueryAttribi ( KDint attribute, KDint* value )
{
	kdSetError ( KD_EINVAL );

	return -1;
}

// kdQueryAttribcv : Obtain the value of a string XMKode attribute. 
KD_API const KDchar* KD_APIENTRY kdQueryAttribcv ( KDint attribute )
{
	switch( attribute )
	{
		case KD_ATTRIB_VENDOR   : return XM_LIBRARY_VENDER;
		case KD_ATTRIB_VERSION  : return XM_LIBRARY_VERSION;
		case KD_ATTRIB_PLATFORM : return XM_LIBRARY_PLATFORM;			
	}

	kdSetError ( KD_EINVAL );

	return KD_NULL;
}

// kdQueryIndexedAttribcv : Obtain the value of an indexed string XMKode attribute.
KD_API const KDchar* KD_APIENTRY kdQueryIndexedAttribcv ( KDint attribute, KDint index )
{
	kdSetError ( KD_EINVAL );

	return KD_NULL;
}
