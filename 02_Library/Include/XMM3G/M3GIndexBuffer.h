/* --------------------------------------------------------------------------
 *
 *      File            M3GIndexBuffer.h  
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

#ifndef __M3GIndexBuffer_h__
#define __M3GIndexBuffer_h__

#include "M3GObject3D.h"

/**
 * @~English	An abstract class defining how to connect vertices to form a geometric object.
 */
class M3GIndexBuffer : public M3GObject3D
{
	public :

        /**
         * @~English	A constructor parameter specifying that 
         *				the new IndexBuffer is to contain an array of triangles or triangle strips..
         */
		static const KDint	TRIANGLES     = 8;

        /**
         * @~English	A constructor parameter specifying that 
         *				the new IndexBuffer is to contain an array of lines or line strips.
         */
		static const KDint	LINES         = 9;

        /**
         * @~English	A constructor parameter specifying that 
         *				the new IndexBuffer is to contain an array of point sprites.
         */
		static const KDint	POINT_SPRITES = 10;

	protected :

		M3GIndexBuffer ( KDvoid );

	public :

		/**
		 * @~English	Constructs a triangle strip array with explicit indices.
         * @param[in]	type    
         * @param[in]	numIndices  
         * @param[in]	indices  
         * @param[in]	numStripLengths  
         * @param[in]	stripLengths  
		 */ 
		M3GIndexBuffer ( KDint type, KDint numIndices, const KDint* indices, KDint numStripLengths, const KDint* stripLengths );

		/**
		 * @~English	Constructs a triangle strip array with explicit indices.
         * @param[in]	type    
         * @param[in]	firstIndex  
         * @param[in]	numStripLengths  
         * @param[in]	stripLengths  
		 */ 
		M3GIndexBuffer ( KDint type, KDint firstIndex, KDint numStripLengths, const KDint* stripLengths );

		/**
		 * @~English	Destruct this object.
		 */ 
		virtual ~M3GIndexBuffer ( KDvoid );

	public :

        virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Returns the number of indices in this buffer.
		 * @return		
		 */ 
		// 
		KDint	getIndexCount ( KDvoid ) const;

		/**
		 * @~English	Retrieves vertex indices for the rendering primitives stored in this buffer.
		 * @param[out]	indices
		 */ 
		KDvoid	getIndices ( KDint* indices ) const;

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

		KDint  getPrimitiveType ( KDvoid ) const;

	protected :

		KDvoid  init ( KDint type, KDint numIndices, const KDint* indices, KDint numStripLengths, const KDint* stripLengths );
		KDvoid  init ( KDint type, KDint firstIndex, KDint numStripLengths, const KDint* stripLengths );
		KDvoid  _duplicate ( M3GIndexBuffer* pObj ) const;

	protected :

		KDuint			m_uID;
		KDint			m_nType;
		KDuint16*		m_pIndices;
		KDuint16*		m_pLengths;
		KDint			m_nNumIndices;
		KDint			m_nNumLengths;
};


#endif
