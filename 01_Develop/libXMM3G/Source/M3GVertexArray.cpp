/* --------------------------------------------------------------------------
 *
 *      File            M3GVertexArray.cpp   
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
#include <XMM3G/M3GVertexArray.h>

M3GVertexArray::M3GVertexArray ( KDvoid )
: m_uID ( 0 )
, m_nVertexCount ( 0 )
, m_nComponentCount ( 0 )
, m_nComponentSize ( 0 )
, m_pValues8 ( KD_NULL )
{

}

M3GVertexArray::M3GVertexArray ( KDint numVertices, KDint numComponents, KDint componentSize )
: m_uID ( 0 )
, m_nVertexCount ( 0 )
, m_nComponentCount ( 0 )
, m_nComponentSize ( 0 )
, m_pValues8 ( KD_NULL )
{
	this->init ( numVertices, numComponents, componentSize );
}

M3GVertexArray::~M3GVertexArray ( KDvoid )
{
    delete [ ] m_pValues8;

    if ( glIsBuffer ( m_uID ) )
	{
        glDeleteBuffers ( 1, &m_uID );
    }
}

KDvoid M3GVertexArray::init ( KDint numVertices, KDint numComponents, KDint componentSize )
{
	if ( numVertices < 1 || numVertices > 65535 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, numVertices = %d.", numVertices );    
	}

	if ( numComponents < 2 || numComponents > 4 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Component count is invalid, numComponents = %d.", numComponents );    
	}

	if ( componentSize != 1 && componentSize != 2 && componentSize != 4 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Component size is invalid, componentSize = %d.", componentSize );    
	}

	KDsize  nSize = numVertices * numComponents * componentSize;

	m_nVertexCount    = numVertices;
	m_nComponentCount = numComponents;
	m_nComponentSize  = componentSize;

	m_pValues8 = new KDubyte [ nSize ];
    kdMemset ( m_pValues8, 0, nSize );

    glGenBuffers ( 1, &m_uID );
    glBindBuffer ( GL_ARRAY_BUFFER, m_uID );
    glBufferData ( GL_ARRAY_BUFFER, nSize, 0, GL_STATIC_DRAW );

	GLenum  nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__ , "Can't make vertex buffer object, err = %d", nErr );    
    }
}

M3GObject3D* M3GVertexArray::duplicate ( KDvoid ) const
{
	M3GVertexArray*  pObj = new M3GVertexArray ( m_nVertexCount, m_nComponentCount, m_nComponentSize );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GVertexArray::_duplicate ( M3GVertexArray* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	pObj->set ( 0, m_nVertexCount, m_nComponentSize, m_pValues8 );
}

KDvoid M3GVertexArray::get ( KDint firstVertex, KDint numVertices, KDubyte* values ) const
{
	this->get ( firstVertex, numVertices, 1, values );
}

KDvoid M3GVertexArray::get ( KDint firstVertex, KDint numVertices, KDshort* values ) const
{
	this->get ( firstVertex, numVertices, 2, values );
}

KDvoid M3GVertexArray::get ( KDint firstVertex, KDint numVertices, KDfloat* values ) const
{
	this->get ( firstVertex, numVertices, 4, values );
}

KDvoid M3GVertexArray::get ( KDint firstVertex, KDint numVertices, KDfloat scale, KDfloat* bias, KDfloat* values ) const
{
    if ( firstVertex < 0 || firstVertex >= m_nVertexCount ) 
	{
		M3GException ( "IllegalStateException", __FUNCTION__ , "First vertex is invalid, firstVertex = %d.", firstVertex ); 
    }

    if ( numVertices < 0 || firstVertex + numVertices > m_nVertexCount )
	{
		M3GException ( "IllegalStateException", __FUNCTION__ , "Number of vertices is invalid, firstVertex = %d, numVertices = %d.", firstVertex, numVertices ); 
    }

    if ( bias == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__ , "Bias is NULL." );
    }

    if ( values == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__ , "Values is NULL." );
    }

    KDint  cc = m_nComponentCount;
	KDint  cs = m_nComponentSize;

	if ( cs != 1 && cs != 2 && cs != 4 )
	{
		M3GException ( "IllegalStateException", __FUNCTION__ , "Comonent size is invalid, size = %d.", cs );
	}

	for ( KDint  v = 0; v < numVertices; v++ )
	{
		for ( KDint  i = 0; i < cc; i++ ) 
		{
			values [ v * cc + i ] = m_pValues8 [ ( ( firstVertex + v ) * cc + i ) * cs ] * scale + bias [ i ];
		}
	}
}

KDint M3GVertexArray::getComponentCount ( KDvoid ) const
{
	return m_nComponentCount;
}

KDint M3GVertexArray::getComponentType ( KDvoid ) const
{
	return m_nComponentSize;
}

KDint M3GVertexArray::getVertexCount ( KDvoid ) const
{
	return m_nVertexCount;
}

KDvoid M3GVertexArray::set ( KDint firstVertex, KDint numVertices, const KDubyte* values )
{
	this->set ( firstVertex, numVertices, 1, values );
}

KDvoid M3GVertexArray::set ( KDint firstVertex, KDint numVertices, const KDshort* values )
{
	this->set ( firstVertex, numVertices, 2, values );
}

KDvoid M3GVertexArray::set ( KDint firstVertex, KDint numVertices, const KDfloat* values )
{
	this->set ( firstVertex, numVertices, 4, values );
}

KDvoid M3GVertexArray::get ( KDint firstVertex, KDint numVertices, KDint valueSize, KDvoid* values ) const
{
    if ( values == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__ , "Values is NULL." ); 
    }

    if ( firstVertex < 0 || firstVertex >= m_nVertexCount )
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "First vertex is invalid, firstVertex = %d.", firstVertex );
    }

    if ( numVertices < 0 || firstVertex + numVertices > m_nVertexCount ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Number of vertices is Invalid, firstVertex = %d, numVertices = %d.", firstVertex, numVertices );
    }

    if ( m_nComponentSize != valueSize ) 
	{
		M3GException ( "IllegalStateException", __FUNCTION__ , "Component size of this vertex array is not %d byte, componentSize = %d.", m_nComponentSize, valueSize ); 
    }

    KDint  nOffset = firstVertex * m_nComponentCount * valueSize;
    KDint  nSize   = numVertices * m_nComponentCount * valueSize;

    kdMemcpy ( values, &m_pValues8 [ nOffset ], nSize );
}

KDvoid M3GVertexArray::set ( KDint firstVertex, KDint numVertices, KDint valueSize, const KDvoid* values )
{
    if ( values == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__ , "Values is NULL." ); 
    }

    if ( numVertices < 0 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Number of vertices is invalid, numVertices = %d.", numVertices );
    }

    if ( firstVertex < 0 || firstVertex + numVertices > m_nVertexCount )
	{
        M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Vertex is out of bounds, [%d, %d] in [0, %d].", firstVertex, firstVertex + numVertices, m_nVertexCount );
    }

    if ( m_nComponentSize != valueSize ) 
	{
		M3GException ( "IllegalStateException", __FUNCTION__ , "Component size of this vertex array is not %d byte, componentSize = %d.", m_nComponentSize, valueSize ); 
    }

	if ( m_uID == 0 ) 
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Vertex Buffre Object is not ready." );
	}

    KDint  nOffset = firstVertex * m_nComponentCount * valueSize;
    KDint  nSize   = numVertices * m_nComponentCount * valueSize;

    kdMemcpy ( &m_pValues8 [ nOffset ], values, nSize );

    glBindBuffer ( GL_ARRAY_BUFFER, m_uID );
    glBufferSubData ( GL_ARRAY_BUFFER, nOffset, nSize, &m_pValues8 [ nOffset ] );

	GLenum  nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Can't send to GPU, err = %d.", nErr );
    }
}

KDvoid M3GVertexArray::updateOpenGLData ( const KDvoid* value ) const
{
    if ( m_pValues8 == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Values is NULL." );
    }

    if ( m_uID == 0 )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Vertex Buffre Object is not ready." );
    }

    glBindBuffer    ( GL_ARRAY_BUFFER, m_uID );
    glBufferSubData ( GL_ARRAY_BUFFER, 0, m_nVertexCount * m_nComponentCount * m_nComponentSize, m_pValues8 );

    GLenum  nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Can't send to GPU, err = %d.", nErr );
    }
}

KDuint M3GVertexArray::getOpenGLVBO ( KDvoid ) const
{
	return m_uID;
}

KDuint M3GVertexArray::getOpenGLFormat ( KDvoid ) const
{
	KDuint  uFormat = 0;

	switch ( m_nComponentSize )
	{
		case 1 : uFormat = GL_BYTE;  break;
		case 2 : uFormat = GL_SHORT; break;
		case 4 : uFormat = GL_FLOAT; break;

		default : 

			M3GException ( "IllegalStateException", __FUNCTION__, "Component type is invalid, type = %d.", m_nComponentSize );
	}

	return uFormat;
}

KDvoid M3GVertexArray::convert ( KDint to )
{
    if ( m_nComponentSize == to )
	{
        return;
	}

    KDint  from = m_nComponentSize;
    KDint   num = m_nVertexCount * m_nComponentCount;

    union 
	{
        KDubyte*	pValues8;
        KDshort*	pValues16;
        KDfloat*	pValues32;
    };

	pValues8 = new KDubyte [ num * to ];

    switch ( from )
	{
		case 1 : 
			
			switch ( to )
			{
				case 1: for ( KDint i = 0; i < num; i++) pValues8  [i] = (KDubyte) m_pValues8 [i]; break;
				case 2: for ( KDint i = 0; i < num; i++) pValues16 [i] = (KDshort) m_pValues8 [i]; break;
				case 4: for ( KDint i = 0; i < num; i++) pValues32 [i] = (KDfloat) m_pValues8 [i]; break;
			}
			
			break;
		
		case 2 : 

			switch ( to ) 
			{
				case 1: for ( KDint i = 0; i < num; i++) pValues8  [i] = (KDubyte) m_pValues16 [i]; break;
				case 2: for ( KDint i = 0; i < num; i++) pValues16 [i] = (KDshort) m_pValues16 [i]; break;
				case 4: for ( KDint i = 0; i < num; i++) pValues32 [i] = (KDfloat) m_pValues16 [i]; break;
			}

			break;
		
		case 4 :
			
			switch ( to )
			{
				case 1: for ( KDint i = 0; i < num; i++) pValues8  [i] = (KDubyte) m_pValues32 [i]; break;
				case 2: for ( KDint i = 0; i < num; i++) pValues16 [i] = (KDshort) m_pValues32 [i]; break;
				case 4: for ( KDint i = 0; i < num; i++) pValues32 [i] = (KDfloat) m_pValues32 [i]; break;
			}

			break;
		
		default : 

			M3GException ( "IllegalStateException", __FUNCTION__, "Source component type is invalid, type = %d.", from );
    }

    m_nComponentSize = to;
    delete [ ] m_pValues8;
	m_pValues8 = pValues8;

    if ( glIsBuffer ( m_uID ) )
	{
        glDeleteBuffers ( 1, &m_uID );
    }

    KDint  nSize = m_nVertexCount * m_nComponentCount * m_nComponentSize;

    glGenBuffers ( 1, &m_uID );
    glBindBuffer ( GL_ARRAY_BUFFER, m_uID );
    glBufferData ( GL_ARRAY_BUFFER, nSize, m_pValues8, GL_STATIC_DRAW );
}

KDvoid M3GVertexArray::setSkinning ( const M3GVertexArray*                              basePositions,
									 const std::vector< std::vector<M3GBoneWeight> >&	boneIndices,
									 const std::vector<M3GMatrix>&                      matrixPalette )
{
    if ( basePositions == KD_NULL )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Base vertex array is NULL" );
    }

    if ( m_uID == 0 )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Vertex Buffre Object is not ready." );
    }

    for ( KDint v = 0; v < m_nVertexCount; v++ )
	{
        M3GVector  v0;
        switch ( basePositions->m_nComponentSize )
		{
			case 1 : 
				
				v0 = M3GVector 
				(
					basePositions->m_pValues8 [ v*3   ],
					basePositions->m_pValues8 [ v*3+1 ],
					basePositions->m_pValues8 [ v*3+2 ] 
				);

				break;

			case 2 :
				
				v0 = M3GVector 
				(
					basePositions->m_pValues16 [ v*3   ],
					basePositions->m_pValues16 [ v*3+1 ],
					basePositions->m_pValues16 [ v*3+2 ] 
				);
				
				break;

			case 4 :
				
				v0 = M3GVector
				(
					basePositions->m_pValues32 [ v*3   ],
					basePositions->m_pValues32 [ v*3+1 ],
					basePositions->m_pValues32 [ v*3+2 ] 
				);

				break;

			default :
				
				M3GException ( "IllegalStateException", __FUNCTION__, "Component size is invalid, size = %d.", basePositions->m_nComponentSize );
        }

        M3GVector  v1       = M3GVector ( 0, 0, 0 );
        KDfloat  fWeight    = 0;
        KDint    nBoneCount = boneIndices [v].size ( );
        
		for ( KDint  b = 0; b < nBoneCount; b++ )
		{
			fWeight += boneIndices [v][b].m_nWeight;
        }
        
		for ( KDint  b = 0; b < nBoneCount; b++ ) 
		{
			KDint  i = boneIndices [v][b].m_nIndex;
            v1 += matrixPalette [i] * v0 * ( boneIndices [v][b].m_nWeight / fWeight );
        }

        if ( fWeight > 0 )
		{
            for ( KDint  i = 0; i < 3; i++ )
			{
                switch ( m_nComponentSize )
				{
					case 1 : m_pValues8  [ v*3+i ] = (KDubyte) v1 [i]; break;
					case 2 : m_pValues16 [ v*3+i ] = (KDshort) v1 [i]; break;
					case 4 : m_pValues32 [ v*3+i ] = (KDfloat) v1 [i]; break;
                }
            }
        }
    }   

    glBindBuffer    ( GL_ARRAY_BUFFER, m_uID );
    glBufferSubData ( GL_ARRAY_BUFFER, 0, m_nVertexCount * m_nComponentCount * m_nComponentSize, m_pValues8 );
/*
    GLenum  nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Can't send to GPU, err = %d.", nErr );
    }
*/
}

KDvoid M3GVertexArray::setMorphing ( const M3GVertexArray*							basePositions,
									 const std::vector<const M3GVertexArray*>&		morphTarget,
									 const std::vector<KDfloat>&					morphWeights )
{
    if ( basePositions == KD_NULL )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Base vertex array is NULL" );
    }

    if ( m_uID == 0 )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Vertex Buffre Object is not ready." );
    }

    if ( morphTarget.size ( ) != morphWeights.size ( ) )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Size of targes and weight must be same, %d, %d.", morphTarget.size(), morphWeights.size ( ) );
    }

    if ( m_uID == 0 )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Vertex Buffre Object is not ready." );
    }

    for ( KDint  v = 0; v < m_nVertexCount; v++ ) 
	{
        M3GVector  v0, v2;

		switch ( basePositions->m_nComponentSize )
		{
			case 1 : 
				
				v2 = v0 = M3GVector
				(
					basePositions->m_pValues8 [ v*3   ],
					basePositions->m_pValues8 [ v*3+1 ],
					basePositions->m_pValues8 [ v*3+2 ]
				);
				
				break;

			case 2 : 
				
				v2 = v0 = M3GVector
				(
					basePositions->m_pValues16 [ v*3   ],
					basePositions->m_pValues16 [ v*3+1 ],
					basePositions->m_pValues16 [ v*3+2 ]
				);
				
				break;

			case 4 : 
				
				v2 = v0 = M3GVector
				(
					basePositions->m_pValues32 [ v*3   ],
					basePositions->m_pValues32 [ v*3+1 ],
					basePositions->m_pValues32 [ v*3+2 ]
				);
				
				break;
        }

        for ( KDuint  t = 0; t < morphTarget.size ( ); t++ ) 
		{
			M3GVector  v1;

			switch ( morphTarget [ t ]->m_nComponentSize )
			{
				case 1 : 
					
					v1 = M3GVector
					(
						morphTarget [ t ]->m_pValues8 [ v*3   ],
						morphTarget [ t ]->m_pValues8 [ v*3+1 ],
						morphTarget [ t ]->m_pValues8 [ v*3+2 ]
					);
					
					break;

				case 2 : 
					
					v1 = M3GVector
					(
						morphTarget [ t ]->m_pValues16 [ v*3   ],
						morphTarget [ t ]->m_pValues16 [ v*3+1 ],
						morphTarget [ t ]->m_pValues16 [ v*3+2 ]
					);
					
					break;

				case 4 : 
					
					v1 = M3GVector
					(
						morphTarget [ t ]->m_pValues32 [ v*3   ],
						morphTarget [ t ]->m_pValues32 [ v*3+1 ],
						morphTarget [ t ]->m_pValues32 [ v*3+2 ]
					);
					
					break;
			}

            v2 += morphWeights [ t ] * ( v1 - v0 );
        }

        for ( KDint i = 0; i < 3; i++ ) 
		{
            switch ( m_nComponentSize )
			{
				case 1 : m_pValues8  [ v*3+i ] = (KDubyte) v2 [i]; break;
				case 2 : m_pValues16 [ v*3+i ] = (KDshort) v2 [i]; break;
				case 4 : m_pValues32 [ v*3+i ] = (KDfloat) v2 [i]; break;
            }
        }
    }  

    glBindBuffer    ( GL_ARRAY_BUFFER, m_uID );
    glBufferSubData ( GL_ARRAY_BUFFER, 0, m_nVertexCount * m_nComponentCount * m_nComponentSize, m_pValues8 );

    GLenum  nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Can't send to GPU, err = %d.", nErr );
    }
}
