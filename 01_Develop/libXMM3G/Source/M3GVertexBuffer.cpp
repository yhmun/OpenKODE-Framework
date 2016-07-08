/* --------------------------------------------------------------------------
 *
 *      File            M3GVertexBuffer.cpp
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
#include <XMM3G/M3GVertexBuffer.h>
#include <XMM3G/M3GVertexArray.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GAnimationController.h>
#include <XMM3G/M3GKeyframeSequence.h>

M3GVertexBuffer::M3GVertexBuffer ( KDvoid )
: m_pPositions ( 0 )
, m_pNormals ( 0 )
, m_pColors ( 0 )
, m_fPositionsScale ( 1 )
, m_nARGB ( 0xffffffff )
, m_fNodeAlpha ( 1 )
{
	KDint  i, j;

    for ( i = 0; i < 3; i++ )
	{
        m_fPositionsBias [ i ] = 0;
    }

    for ( i = 0; i < MAX_TEXTURE_UNITS; i++ ) 
	{
        m_pTexCoords [ i ] = 0;
        m_fTexCoordsScale [ i ] = 1;
        
		for ( j = 0; j < 3; j++ ) 
		{
            m_fTexCoordsBias [ i ][ j ] = 0;
        }
    }
}

M3GVertexBuffer::~M3GVertexBuffer ( KDvoid )
{

}

KDvoid M3GVertexBuffer::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{    
	if ( animationTrack->getTargetProperty ( ) == M3GAnimationTrack::COLOR ) 
	{
        this->_addAnimationTrack ( animationTrack );
    }
	else
	{
		M3GObject3D::addAnimationTrack ( animationTrack );
	}
}

KDint M3GVertexBuffer::animate ( KDint time )
{
	M3GObject3D::animate ( time );

    KDbool   isColorModefied = KD_FALSE;
    KDbool   isAlphaModefied = KD_FALSE;
    KDfloat  fRGB [ ] = { 0, 0, 0 };
    KDfloat  fAlpha = 0;

    for ( KDint  i = 0; i < getAnimationTrackCount ( ); i++ )
	{
		M3GAnimationTrack*       pTrack      = getAnimationTrack ( i );
        M3GKeyframeSequence*     pKeyframe   = pTrack->getKeyframeSequence ( );
        M3GAnimationController*  pController = pTrack->getController ( );

        if ( !pController || !pController->isActive ( time ) )
		{
            continue;
        }

        KDfloat  fWeight   = pController->getWeight ( );
        KDfloat  fSequence = pController->getPosition ( time );
    
        switch ( pTrack->getTargetProperty ( ) )
		{
			case M3GAnimationTrack::COLOR : 
			{
				KDfloat  fValue [ 3 ] = { 1.f, 1, 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );

				fRGB [ 0 ] += fValue [ 0 ] * fWeight;
				fRGB [ 1 ] += fValue [ 1 ] * fWeight;
				fRGB [ 2 ] += fValue [ 2 ] * fWeight;

				isColorModefied = KD_TRUE;
				
				break;
			}

			case M3GAnimationTrack::ALPHA :
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );

				fAlpha += fValue [ 0 ] * fWeight;

				isAlphaModefied = KD_TRUE;
				
				break;
			}

			default:
			{
				// Unknown target should be ignored.
				// animate() of Base class (of Derived class) retrieve it.
			}
        }
    }

    if ( isColorModefied )
	{
        KDubyte  r = (KDubyte) ( KD_CLAMP ( 0, 1, fRGB [ 0 ] ) * 255 );
        KDubyte  g = (KDubyte) ( KD_CLAMP ( 0, 1, fRGB [ 1 ] ) * 255 );
        KDubyte  b = (KDubyte) ( KD_CLAMP ( 0, 1, fRGB [ 2 ] ) * 255 );

        m_nARGB = ( m_nARGB & 0xff000000 ) | ( r << 16 ) | ( g << 8 ) | ( b << 0 );
    }

    if ( isAlphaModefied ) 
	{
        KDubyte  a = (KDubyte) ( KD_CLAMP ( 0, 1, fAlpha ) * 255 );

        m_nARGB = ( m_nARGB & 0x00ffffff ) | ( a << 24 );
    }

    return 0;
}

M3GObject3D* M3GVertexBuffer::duplicate ( KDvoid ) const
{
    M3GVertexBuffer*  pObj = new M3GVertexBuffer ( );
	
	this->_duplicate ( pObj );

    return pObj;
}

KDvoid M3GVertexBuffer::_duplicate ( M3GVertexBuffer* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	KDint  i, j;

    pObj->m_pPositions = m_pPositions;
    pObj->m_pNormals   = m_pNormals;
    pObj->m_pColors    = m_pColors;

    for ( i = 0; i < MAX_TEXTURE_UNITS; i++ ) 
	{
        pObj->m_pTexCoords [ i ] = m_pTexCoords [ i ];
    }

    pObj->m_fPositionsScale   = m_fPositionsScale;

	for ( i = 0; i < 3; i++ )
	{
		pObj->m_fPositionsBias [ i ] = m_fPositionsBias [ i ];
	}
    
	for ( i = 0; i < MAX_TEXTURE_UNITS; i++ ) 
	{
        pObj->m_fTexCoordsScale [ i ] = m_fTexCoordsScale [ i ];
    }

    for ( i = 0; i < MAX_TEXTURE_UNITS; i++ ) 
	{
		for ( j = 0; j < 3; j++ )
		{
			pObj->m_fTexCoordsBias [ i ][ j ] = m_fTexCoordsBias [ i ][ j ];
		}
    }

    pObj->m_nARGB = m_nARGB;
    pObj->m_fNodeAlpha = m_fNodeAlpha;
}

KDint M3GVertexBuffer::getReferences ( M3GObject3D** references ) const
{
    KDint  nNumber = M3GObject3D::getReferences ( references );
	KDint  i;

    if ( m_pPositions )
	{
		if ( references )
		{
			references [ nNumber ] = m_pPositions;
		}

		nNumber++;
	}

    if ( m_pNormals )
	{
		if ( references )
		{
			references [ nNumber ] = m_pNormals;
		}

		nNumber++;
	}

    if ( m_pColors )
	{
		if ( references )
		{
			references [ nNumber ] = m_pColors;
		}

		nNumber++;
	}

    for ( i = 0; i < MAX_TEXTURE_UNITS; i++ )
	{
        if ( m_pTexCoords [ i ] )
		{
			if ( references )
			{
				references [ nNumber ] = m_pTexCoords [ i ];
			}
 
			nNumber++;
		}
    }

    return nNumber;
}

M3GVertexArray* M3GVertexBuffer::getColors ( KDvoid ) const
{
    return m_pColors;
}

KDint M3GVertexBuffer::getDefaultColor ( KDvoid ) const
{
	return m_nARGB;
}

M3GVertexArray* M3GVertexBuffer::getNormals ( KDfloat* scaleBias ) const
{
	if ( m_pNormals ) 
	{
        if ( scaleBias )
		{
			KDint  nComponentSize = m_pNormals->getComponentType ( );

            scaleBias [ 0 ] = ( nComponentSize == 1 ) ? 2 / 255.f : ( nComponentSize == 2 ) ? 2 / 65535.f : 1.f;
            scaleBias [ 1 ] = ( nComponentSize == 1 ) ? 1 / 255.f : ( nComponentSize == 2 ) ? 1 / 65535.f : 0.f;
            scaleBias [ 2 ] = ( nComponentSize == 1 ) ? 1 / 255.f : ( nComponentSize == 2 ) ? 1 / 65535.f : 0.f;
            scaleBias [ 3 ] = ( nComponentSize == 1 ) ? 1 / 255.f : ( nComponentSize == 2 ) ? 1 / 65535.f : 0.f;
        }
    }

	return m_pNormals;
}

M3GVertexArray* M3GVertexBuffer::getPositions ( KDfloat* scaleBias ) const
{
    if ( m_pPositions )
	{
        if ( scaleBias )
		{
            scaleBias [ 0 ] = m_fPositionsScale;
            scaleBias [ 1 ] = m_fPositionsBias [ 0 ];
            scaleBias [ 2 ] = m_fPositionsBias [ 1 ];
            scaleBias [ 3 ] = m_fPositionsBias [ 2 ];
        }
    }

    return m_pPositions;
}

M3GVertexArray* M3GVertexBuffer::getTexCoords ( KDint index, KDfloat* scaleBias ) const
{
    if ( index < 0 || index >= MAX_TEXTURE_UNITS ) 
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__ , "Texture index is invalid." ); 
    }

    if ( m_pTexCoords [ index ] && scaleBias ) 
	{
        scaleBias [ 0 ] = m_fTexCoordsScale [ index ];

        for ( KDint  i = 0; i < m_pTexCoords [ index ]->getComponentCount ( ); i++ )
		{
            scaleBias [ i + 1 ] = m_fTexCoordsBias [ index ][ i ];
        }
    }

    return m_pTexCoords [ index ];
}

KDint M3GVertexBuffer::getVertexCount ( KDvoid ) const
{
	return m_pPositions ? m_pPositions->getVertexCount ( ) : 0;
}

KDvoid M3GVertexBuffer::setColors ( M3GVertexArray* colors )
{
    if ( colors == KD_NULL )
	{
        m_pColors = KD_NULL;
        return;
    }

    KDint  nComponentType  = colors->getComponentType  ( );
    KDint  nComponentCount = colors->getComponentCount ( );

    if ( nComponentType != 1 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Component size must be 1, size = %d", nComponentType ); 
    }

    if ( nComponentCount != 3 && nComponentCount != 4 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Component count must be 3 or 4, size = %d.", nComponentCount ); 
    }

    m_pColors = colors;
    m_fNodeAlpha = 1;
}

KDvoid M3GVertexBuffer::setDefaultColor ( KDint ARGB )
{
	m_nARGB = ARGB;
}

KDvoid M3GVertexBuffer::setNormals ( M3GVertexArray* normals )
{
    if ( normals == KD_NULL)
	{
        m_pNormals = KD_NULL;
        return;
    }

    KDint  nVertexCount    = normals->getVertexCount ( );
    KDint  nComponentCount = normals->getComponentCount ( );

    if ( nComponentCount != 3 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Component count must be 3, size = %d.", nComponentCount ); 
    }

    if ( m_pPositions && m_pPositions->getVertexCount ( ) != nVertexCount )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, %d <--> %d.", m_pPositions->getVertexCount ( ), nVertexCount ); 
    }

    if ( m_pColors && m_pColors->getVertexCount ( ) != nVertexCount )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, %d <--> %d.", m_pColors->getVertexCount ( ), nVertexCount ); 
    }

    for ( KDint i = 0; i < MAX_TEXTURE_UNITS; i++ )
	{
        if ( m_pTexCoords [ i ] && m_pTexCoords [ i ]->getVertexCount ( ) != nVertexCount )
		{
			M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, %d <--> %d.", m_pTexCoords [ i ]->getVertexCount ( ), nVertexCount ); 
        }
    }

    m_pNormals = normals;
}

KDvoid M3GVertexBuffer::setPositions ( M3GVertexArray* positions, KDfloat scale, KDfloat* bias )
{
    if ( positions == KD_NULL )
	{
        m_pPositions = KD_NULL;
        return;
    }

    if ( bias == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__ , "Bias is Null." ); 
    }

    KDint  nVertexCount    = positions->getVertexCount ( );
    KDint  nComponentCount = positions->getComponentCount ( );
    KDint  nComponentType  = positions->getComponentType ( );

    if ( nComponentCount != 3 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Component count must be 3, count = %d.", nComponentCount ); 
    }

    if ( nComponentType != 2 && nComponentType != 4 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Component size must be 2 or 4, size = %d.", nComponentType ); 
    }

    if ( m_pNormals && m_pNormals->getVertexCount ( ) != nVertexCount ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, %d <--> %d.", m_pNormals->getVertexCount ( ), nVertexCount ); 
    }

    if ( m_pColors && m_pColors->getVertexCount ( ) != nVertexCount )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, %d <--> %d.", m_pColors->getVertexCount ( ), nVertexCount ); 
    }

    for ( KDint i = 0; i < MAX_TEXTURE_UNITS; i++ ) 
	{
        if ( m_pTexCoords [ i ] && m_pTexCoords [ i ]->getVertexCount ( ) != nVertexCount )
		{
			M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, %d <--> %d.", m_pTexCoords [ i ]->getVertexCount ( ), nVertexCount ); 
        }
    }

    m_pPositions           = positions;
    m_fPositionsScale      = scale;
    m_fPositionsBias [ 0 ] = bias [ 0 ];
    m_fPositionsBias [ 1 ] = bias [ 1 ];
    m_fPositionsBias [ 2 ] = bias [ 2 ];

}

KDvoid M3GVertexBuffer::setTexCoords ( KDint index, M3GVertexArray* texCoords, KDfloat scale, KDfloat* bias )
{
    if ( index < 0 || index >= MAX_TEXTURE_UNITS ) 
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__ , "Index is invalid, index = %d.", index ); 
    }

    if ( texCoords == KD_NULL )
	{
        m_pTexCoords [ index ] = KD_NULL;
        return;
    }

    if ( bias == KD_NULL ) 
	{
        M3GException ( "NullPointerException", __FUNCTION__ , "Bias is Null." ); 
    }

    KDint  nVertexCount    = texCoords->getVertexCount ( );
    KDint  nComponentCount = texCoords->getComponentCount ( );
    KDint  nComponentType  = texCoords->getComponentType ( );

    if ( nComponentCount != 2 && nComponentCount != 3 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Component count must be 2 or 3, count = %d.", nComponentCount ); 
    }

    if ( nComponentType != 1 && nComponentType != 2 && nComponentType != 4 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Component size must be 1, 2 or 4, size = %d.", nComponentType ); 
    }

    if ( m_pPositions && m_pPositions->getVertexCount ( ) != nVertexCount )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, %d <--> %d.", m_pPositions->getVertexCount ( ), nVertexCount ); 
    }

    if ( m_pNormals && m_pNormals->getVertexCount ( ) != nVertexCount )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, %d <--> %d.", m_pNormals->getVertexCount ( ), nVertexCount ); 
    }

    if ( m_pColors && m_pColors->getVertexCount ( ) != nVertexCount ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Vertex count is invalid, %d <--> %d.", m_pColors->getVertexCount ( ), nVertexCount ); 
    }
 
    m_pTexCoords      [ index ]      = texCoords;
    m_fTexCoordsScale [ index ]      = scale;
    m_fTexCoordsBias  [ index ][ 0 ] = bias [ 0 ];
    m_fTexCoordsBias  [ index ][ 1 ] = bias [ 1 ];
    m_fTexCoordsBias  [ index ][ 2 ] = bias [ 2 ];
}

KDvoid M3GVertexBuffer::render ( M3GRenderState& state ) const
{
    if ( state.m_nPass != 2 )
	{
        return;
    }

    if ( m_pPositions )
	{
        glTranslatef ( m_fPositionsBias [ 0 ], m_fPositionsBias [ 1 ], m_fPositionsBias [ 2 ] );
        glScalef     ( m_fPositionsScale, m_fPositionsScale, m_fPositionsScale );

        glBindBuffer        ( GL_ARRAY_BUFFER, m_pPositions->getOpenGLVBO ( ) );
        glEnableClientState ( GL_VERTEX_ARRAY );
        glVertexPointer     ( m_pPositions->getComponentCount ( ), m_pPositions->getOpenGLFormat ( ), 0, 0 );
    } 
	else 
	{
        glDisableClientState ( GL_VERTEX_ARRAY );
    }

    if ( m_pNormals ) 
	{
        glBindBuffer        ( GL_ARRAY_BUFFER, m_pNormals->getOpenGLVBO ( ) );
        glEnableClientState ( GL_NORMAL_ARRAY );
        glNormalPointer     ( m_pNormals->getOpenGLFormat ( ), 0, 0 );
    }
	else
	{
        glDisableClientState ( GL_NORMAL_ARRAY );
    }

    if ( m_pColors ) 
	{
		if ( state.m_fAlpha != m_fNodeAlpha )
		{
            KDint  nVertexCount     = m_pColors->getVertexCount ( );
            KDint  nComponentCount  = m_pColors->getComponentCount ( );
            KDint  nComponentNumber = nVertexCount * nComponentCount;

            if ( nComponentCount == 4 )
			{               
                KDubyte*  pValues = new KDubyte [ nComponentNumber ];
                m_pColors->get ( 0, nVertexCount, pValues );

                for ( KDint  v = 0; v < nVertexCount; v++ )
				{
					KDint  off = v * nComponentCount + 3;
                    pValues [ off ] = (KDubyte) ( pValues [ off ] * state.m_fAlpha );
                }

                m_pColors->updateOpenGLData ( pValues );
                delete[] pValues;
            }

            m_fNodeAlpha = state.m_fAlpha;
        }

        glBindBuffer         ( GL_ARRAY_BUFFER, m_pColors->getOpenGLVBO ( ) );
        glEnableClientState  ( GL_COLOR_ARRAY );
        glColorPointer       ( m_pColors->getComponentCount ( ), GL_UNSIGNED_BYTE, 0, 0 );
        glDisableClientState ( GL_COLOR_ARRAY );

		state.m_bVertexColorBufferReady = KD_TRUE;
		state.m_nDefaultVertexColor     = 0xffffffff;
    } 
	else 
	{
        glDisableClientState ( GL_COLOR_ARRAY );
        state.m_bVertexColorBufferReady = KD_FALSE;
        state.m_nDefaultVertexColor     = m_nARGB;
    }

    glMatrixMode ( GL_TEXTURE );

    for ( KDint  i = 0; i < MAX_TEXTURE_UNITS; i++ )
	{
        if ( m_pTexCoords [ i ] ) 
		{
            glActiveTexture ( GL_TEXTURE0 + i );
            glEnable        ( GL_TEXTURE_2D );

            glLoadIdentity ( );  
            glTranslatef ( m_fTexCoordsBias [ i ][ 0 ], m_fTexCoordsBias [ i ][ 1 ], m_fTexCoordsBias [ i ][ 2 ] );
            glScalef ( m_fTexCoordsScale [ i ], m_fTexCoordsScale [ i ], m_fTexCoordsScale [ i ] );

            glBindBuffer          ( GL_ARRAY_BUFFER, m_pTexCoords [ i ]->getOpenGLVBO ( ) );    
            glClientActiveTexture ( GL_TEXTURE0 + i );           
            glEnableClientState   ( GL_TEXTURE_COORD_ARRAY );  
            glTexCoordPointer     ( m_pTexCoords [ i ]->getComponentCount ( ), m_pTexCoords [ i ]->getOpenGLFormat ( ), 0, 0 );
        }
		else 
		{            
            glClientActiveTexture ( GL_TEXTURE0 + i );          
            glDisableClientState  ( GL_TEXTURE_COORD_ARRAY ); 
        }
    }
  
    glMatrixMode ( GL_MODELVIEW );
}