/* --------------------------------------------------------------------------
 *
 *      File            M3GTriangleStripArray.cpp
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
#include <XMM3G/M3GTriangleStripArray.h>

M3GTriangleStripArray::M3GTriangleStripArray ( KDvoid )
{

}

M3GTriangleStripArray::M3GTriangleStripArray ( KDint numIndices, const KDint* indices, KDint numStripLengths, const KDint* stripLengths )
: M3GIndexBuffer ( M3GIndexBuffer::TRIANGLES, numIndices, indices, numStripLengths, stripLengths )
{

}

M3GTriangleStripArray::M3GTriangleStripArray ( KDint firstIndex, KDint numStripLengths, const KDint* stripLengths )
: M3GIndexBuffer ( M3GIndexBuffer::TRIANGLES, firstIndex, numStripLengths, stripLengths )
{

}

M3GTriangleStripArray::~M3GTriangleStripArray ( KDvoid )
{

}

KDvoid M3GTriangleStripArray::init ( KDint numIndices, const KDint* indices, KDint numStripLengths, const KDint* stripLengths )
{
	M3GIndexBuffer::init ( M3GIndexBuffer::TRIANGLES, numIndices, indices, numStripLengths, stripLengths );
}

KDvoid M3GTriangleStripArray::init ( KDint firstIndex, KDint numStripLengths, const KDint* stripLengths )
{
	M3GIndexBuffer::init ( M3GIndexBuffer::TRIANGLES, firstIndex, numStripLengths, stripLengths );
}

M3GObject3D* M3GTriangleStripArray::duplicate ( KDvoid ) const
{
	KDint*  pIndices = new KDint [ m_nNumIndices ];
	KDint*  pLengths = new KDint [ m_nNumLengths ];

	for ( KDint  i = 0; i < m_nNumIndices; i++ )
	{
		pIndices [ i ] = m_pIndices [ i ];
	}

	for ( KDint  i = 0; i < m_nNumLengths; i++ )
	{
		pLengths [ i ] = m_pLengths [ i ];
	}

	M3GTriangleStripArray*  pObj = new M3GTriangleStripArray ( m_nNumIndices, pIndices, m_nNumLengths, pLengths );

	this->_duplicate ( pObj );

	delete [] pIndices;
	delete [] pLengths;

	return pObj;
}

KDvoid M3GTriangleStripArray::_duplicate ( M3GTriangleStripArray* pObj ) const
{
	M3GIndexBuffer::_duplicate ( pObj );
}