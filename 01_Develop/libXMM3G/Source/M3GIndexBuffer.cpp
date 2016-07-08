/* --------------------------------------------------------------------------
 *
 *      File            M3GIndexBuffer.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 UEDA.Takashi
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 *
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
#include <XMM3G/M3GIndexBuffer.h>

M3GIndexBuffer::M3GIndexBuffer ( KDvoid )
: m_uID ( 0 )
, m_nType ( 0 )
, m_nNumIndices ( 0 )
, m_nNumLengths ( 0 )
{

}

M3GIndexBuffer::M3GIndexBuffer ( KDint type, KDint numIndices, const KDint* indices, KDint numStripLengths, const KDint* stripLengths )
: m_uID ( 0 )
, m_nType ( 0 )
, m_nNumIndices ( 0 )
, m_nNumLengths ( 0 )
{
	this->init ( type, numIndices, indices, numStripLengths, stripLengths );
}

M3GIndexBuffer::M3GIndexBuffer ( KDint type, KDint firstIndex, KDint numStripLengths, const KDint* stripLengths )
: m_uID ( 0 )
, m_nType ( 0 )
, m_nNumIndices ( 0 )
, m_nNumLengths ( 0 )
{
	this->init ( type, firstIndex, numStripLengths, stripLengths );
}

M3GIndexBuffer::~M3GIndexBuffer ( KDvoid )
{
	delete [] m_pIndices;
	delete [] m_pLengths;

    if ( glIsBuffer ( m_uID ) ) 
	{
        glDeleteBuffers ( 1, &m_uID );
    }
}

KDvoid M3GIndexBuffer::init ( KDint type, KDint numIndices, const KDint* indices, KDint numStripLengths, const KDint* stripLengths )
{
	KDint  i;

    if ( type != TRIANGLES ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Primitive type is invalid, type = %d.", type );      
    }

    if ( indices == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__ , "Indices is NULL." );
    }

    if ( stripLengths == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__ , "Strip lengths is NULL." );
    }

    if ( numStripLengths < 1 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Number of strips is invalid, numStripLengths = %d.", numStripLengths );
    }

	KDint  nSum = 0;
	for ( i = 0; i < numStripLengths; i++) 
	{
		KDint  nLenth = stripLengths [ i ];
		if ( nLenth < 0 || nLenth > KDINT16_MAX )
		{
			M3GException ( "IndexOutOfBoundsException", __FUNCTION__ , "Strip lengths [ %d ] is out of index, length = %d.", i, nLenth );
		}
		
		if ( nLenth < 3) 
		{
			M3GException ( "IllegalArgumentException", __FUNCTION__ , "Strip lengths [ %d ] is invalid, length = %d.", i, nLenth );
		}

		nSum += nLenth;
	}
	
	if ( numIndices < nSum )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Too few indices, indices = %d < sum of lengths = %d.", numIndices, nSum );
	}

	m_nType    = type;
	m_pIndices = new KDuint16 [ numIndices ];
	m_pLengths = new KDuint16 [ numStripLengths ];

	m_nNumIndices = numIndices;
	m_nNumLengths = numStripLengths;

	for ( KDint  i = 0; i < numIndices; i++ )
	{
		m_pIndices [ i ] = (KDuint16) indices [ i ];
	}

	for ( KDint  i = 0; i < numStripLengths; i++ )
	{
		m_pLengths [ i ] = (KDuint16) stripLengths [ i ];
	}

    glGenBuffers ( 1, &m_uID );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_uID ); 
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( KDuint16 ) * numIndices, m_pIndices, GL_STATIC_DRAW );

	GLenum  nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__ , "Can't make index buffer object, err = %d", nErr );    
    }
}

KDvoid M3GIndexBuffer::init ( KDint type, KDint firstIndex, KDint numStripLengths, const KDint* stripLengths )
{
	KDint  i;

    if ( type != TRIANGLES ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Primitive type is invalid, type = %d.", type );      
    }

    if ( stripLengths == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__ , "Strip lengths is NULL." );
    }

    if ( numStripLengths < 1 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Number of strips is invalid, numStripLengths = %d.", numStripLengths );
    }

	KDint  numIndices = 0;
	for ( i = 0; i < numStripLengths; i++) 
	{
		KDint  nLenth = stripLengths [ i ];
		if ( nLenth < 0 || nLenth > KDINT16_MAX )
		{
			M3GException ( "IndexOutOfBoundsException", __FUNCTION__ , "Strip lengths [ %d ] is out of index, length = %d.", i, nLenth );
		}
		
		if ( nLenth < 3) 
		{
			M3GException ( "IllegalArgumentException", __FUNCTION__ , "Strip lengths [ %d ] is invalid, length = %d.", i, nLenth );
		}

		numIndices += nLenth;
	}
	
	if ( firstIndex + numIndices > KDINT16_MAX )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Number of indices is invalid, num = %d > %d.", firstIndex + numIndices, KDINT16_MAX );
	}

	m_nType    = type;
	m_pIndices = new KDuint16 [ numIndices ];
	m_pLengths = new KDuint16 [ numStripLengths ];

	m_nNumIndices = numIndices;
	m_nNumLengths = numStripLengths;

	for ( i = 0; i < numIndices; i++ ) 
	{
        m_pIndices [ i ] = firstIndex + i;
    }

	for ( KDint  i = 0; i < numStripLengths; i++ )
	{
		m_pLengths [ i ] = (KDuint16) stripLengths [ i ];
	}

    glGenBuffers ( 1, &m_uID );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_uID ); 
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( KDuint16 ) * numIndices, m_pIndices, GL_STATIC_DRAW );

	GLenum  nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__ , "Can't make index buffer object, err = %d", nErr );    
    }
}

M3GObject3D* M3GIndexBuffer::duplicate ( KDvoid ) const
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

	M3GIndexBuffer*  pObj = new M3GIndexBuffer ( m_nType, m_nNumIndices, pIndices, m_nNumLengths, pLengths );

	this->_duplicate ( pObj );

	delete [] pIndices;
	delete [] pLengths;

	return pObj;
}

KDvoid M3GIndexBuffer::_duplicate ( M3GIndexBuffer* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );
}

KDint M3GIndexBuffer::getIndexCount ( KDvoid ) const
{
    KDint  nCount = 0;

    switch ( m_nType )
	{
		case TRIANGLES :

			for ( KDint  i = 0; i < m_nNumLengths; i++) 
			{
				nCount += ( m_pLengths [ i ] - 2 ) * 3;
			}

			break;

		case LINES :
		case POINT_SPRITES :
		default :

			M3GException ( "InternalException", __FUNCTION__ , "Primitive type is invalid, type = %d.", m_nType );
    };

    return nCount;
}

KDvoid M3GIndexBuffer::getIndices ( KDint* indices ) const
{
    KDint  i = 0;
    KDint  j = 0;

    switch ( m_nType ) 
	{
		case TRIANGLES :

			for ( KDint  s = 0; s < m_nNumLengths; s++) 
			{
				for ( KDint  t = 0; t < m_pLengths [ s ] - 2; t++ ) 
				{
					indices [ j++ ] = m_pIndices [ i + 0 ];
					indices [ j++ ] = m_pIndices [ i + 1 ];
					indices [ j++ ] = m_pIndices [ i + 2 ];

					i++;
				}

				i += 2;
			}   

			break;

		case LINES :
		case POINT_SPRITES :
		default :

			M3GException ( "InternalException", __FUNCTION__ , "Primitive type is invalid, type = %d.", m_nType );
    };
}

KDvoid M3GIndexBuffer::render ( M3GRenderState& state ) const
{
	if ( state.m_nPass != 2 )
	{
		return;
	}

	if ( m_uID == 0 )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Buffer object of index is not ready, vbo = %d.", m_uID );
	}

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_uID );

	GLintptr  offset = 0;
	for ( KDint  i = 0; i < m_nNumLengths; i++ )
	{
		glDrawElements ( GL_TRIANGLE_STRIP,
						 m_pLengths [ i ],
						 GL_UNSIGNED_SHORT,
						 (GLvoid*) offset );

		offset += m_pLengths [ i ] * sizeof ( KDuint16 );
	}
}

KDint M3GIndexBuffer::getPrimitiveType ( KDvoid ) const
{
	return m_nType;
}
