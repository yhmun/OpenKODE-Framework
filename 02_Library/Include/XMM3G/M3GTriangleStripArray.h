/* --------------------------------------------------------------------------
 *
 *      File            M3GTriangleStripArray.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010 UEDA.Takashi
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __M3GTriangleStripArray_h__
#define __M3GTriangleStripArray_h__

#include "M3GIndexBuffer.h"

/**
 * @~English	TriangleStripArray defines an array  of triangle strips.
 */
class M3GTriangleStripArray : public M3GIndexBuffer
{
	protected :

		M3GTriangleStripArray ( KDvoid );

	public :

		/**
		 * @~English	TriangleStripArray defines an array  of triangle strips.
         * @param[in]	numIndices    
         * @param[in]	indices  
         * @param[in]	numStripLengths  
         * @param[in]	stripLengths  
		 */
		M3GTriangleStripArray ( KDint numIndices, const KDint* indices, KDint numStripLengths, const KDint* stripLengths );

		/**
		 * @~English	Constructs a triangle strip array with implicit indices.
         * @param[in]	firstIndex    
         * @param[in]	numStripLengths  
         * @param[in]	stripLengths  
		 */
		M3GTriangleStripArray ( KDint firstIndex, KDint numStripLengths, const KDint* stripLengths );

		/**
		 * @~English	Destruct this object.
		 */
		virtual ~M3GTriangleStripArray ( KDvoid );

	public :

        virtual M3GObject3D*  duplicate ( KDvoid ) const;

	protected :

		KDvoid  init ( KDint numIndices, const KDint* indices, KDint numStripLengths, const KDint* stripLengths );
		KDvoid  init ( KDint firstIndex, KDint numStripLengths, const KDint* stripLengths );
		KDvoid  _duplicate ( M3GTriangleStripArray* pObj ) const;

	private :

		friend class M3GLoader;

};

#endif
