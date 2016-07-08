/* --------------------------------------------------------------------------
 *
 *      File            M3GKeyframeSequence.cpp
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
#include <XMM3G/M3GKeyframeSequence.h>

M3GKeyframeSequence::M3GKeyframeSequence ( KDvoid )
: m_nRepeatMode ( CONSTANT )
, m_nDuration ( 0 )
, m_nRangeFirst ( -1 )
, m_nRangeLast ( -1 )
{

}

M3GKeyframeSequence::M3GKeyframeSequence ( KDint numKeyframes, KDint numComponents, KDint interpolation )
: m_nRepeatMode ( CONSTANT )
, m_nDuration ( 0 )
, m_nRangeFirst ( -1 )
, m_nRangeLast ( -1 )
{
	this->init ( numKeyframes, numComponents, interpolation );
}

M3GKeyframeSequence::~M3GKeyframeSequence ( KDvoid )
{
    for ( KDuint  i = 0; i < m_vKeyframes.size ( ); i++ )
	{
		delete [] m_vKeyframes [ i ].m_pValue;
    }
}

KDvoid M3GKeyframeSequence::init ( KDint numKeyframes, KDint numComponents, KDint interpolation )
{
    if ( numKeyframes < 1 )
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Number of keyframes is invalid, number = %d.", numKeyframes );
    }

    if ( numComponents < 1 ) 
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Number of components is invalid, conponent = %d.", numComponents );
    }

    if ( interpolation != LINEAR && interpolation != SLERP && interpolation != SPLINE && interpolation != SQUAD && interpolation != STEP )
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Interpolation type is invalid, type = %d.", interpolation );
    }

	m_nInterpType     = interpolation;
	m_nComponentCount = numComponents;
	m_nKeyframeCount  = numKeyframes;
    m_nRangeFirst	  = 0;
    m_nRangeLast	  = numKeyframes - 1;

    m_vKeyframes.reserve ( numKeyframes );

    for ( KDint  i = 0; i < numKeyframes; i++ )
	{
        KDint     nTime  = 0;
        KDfloat*  pValue = new KDfloat [ numComponents ] ( );

        m_vKeyframes.push_back ( M3GKeyframe ( nTime, pValue ) );
    }
}

M3GObject3D* M3GKeyframeSequence::duplicate ( KDvoid ) const
{
	M3GKeyframeSequence*  pObj = new M3GKeyframeSequence ( m_nKeyframeCount, m_nComponentCount, m_nInterpType );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GKeyframeSequence::_duplicate ( M3GKeyframeSequence* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

    pObj->m_nRepeatMode = m_nRepeatMode;
    pObj->m_nDuration   = m_nDuration;
    pObj->m_nRangeFirst = m_nRangeFirst;
	pObj->m_nRangeLast  = m_nRangeLast;

    for ( KDint  i = 0; i < m_nKeyframeCount; i++ )
	{
        pObj->m_vKeyframes [ i ].m_nTime = m_vKeyframes [ i ].m_nTime;
		kdMemcpy ( pObj->m_vKeyframes [ i ].m_pValue, m_vKeyframes [ i ].m_pValue, sizeof ( KDfloat) * m_nComponentCount );
    }
}

KDint M3GKeyframeSequence::getComponentCount ( KDvoid ) const
{
	return m_nComponentCount;
}

KDint M3GKeyframeSequence::getDuration ( KDvoid ) const
{
	return m_nDuration;
}

KDint M3GKeyframeSequence::getInterpolationType ( KDvoid ) const
{
	return m_nInterpType;
}

KDint M3GKeyframeSequence::getKeyframe ( KDint index, KDfloat* value ) const
{
    if ( index < 0 || index >= m_nKeyframeCount ) 
	{
        M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d.", index );
    }

    if ( value )
	{
        kdMemcpy ( value, m_vKeyframes [ index ].m_pValue, sizeof ( KDfloat ) * m_nComponentCount );
    }

    return m_vKeyframes [ index ].m_nTime;
}

KDint M3GKeyframeSequence::getKeyframeCount ( KDvoid ) const
{
	return m_nKeyframeCount;
}

KDint M3GKeyframeSequence::getRepeatMode ( KDvoid ) const
{
	return m_nRepeatMode;
}

KDint M3GKeyframeSequence::getValidRangeFirst ( KDvoid ) const
{
	return m_nRangeFirst;
}

KDint M3GKeyframeSequence::getValidRangeLast ( KDvoid ) const
{
	return m_nRangeLast;
}

KDvoid M3GKeyframeSequence::setDuration ( KDint duration )
{
    if ( duration <= 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Duration is invalid, duration = %d.", duration );
    }

    m_nDuration = duration;
}

KDvoid M3GKeyframeSequence::setKeyframe ( KDint index, KDint time, const KDfloat* value )
{
    if ( index < 0 || index >= m_nKeyframeCount ) 
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d, keyframeCount = %d.", index, m_nKeyframeCount );
    }

    if ( time < 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Time is Invalid, time = %d.", time );
    }

    if ( value == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Value is NULL." );
    }

    m_vKeyframes [ index ].m_nTime = time;
	kdMemcpy ( m_vKeyframes [ index ].m_pValue, value, sizeof ( KDfloat ) * m_nComponentCount );
}

KDvoid M3GKeyframeSequence::setRepeatMode ( KDint mode )
{
    if ( mode != CONSTANT && mode != LOOP ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Repeat mode is invalid, mode = %d.", mode );
    }

    m_nRepeatMode = mode;
}

KDvoid M3GKeyframeSequence::setValidRange ( KDint first, KDint last )
{
    if ( first < 0 || first >= m_nKeyframeCount )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "First of valid range is invalid, first = %d, keyframeCount = %d.", first, m_nKeyframeCount );
    }

    if ( last < 0 || last >= m_nKeyframeCount ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Last of valid range is invalid, last = %d, keyframeCount = %d.", last, m_nKeyframeCount );
    }

    if ( first > last )
	{
        M3GException ( "NotImplementedException", __FUNCTION__, "Sorry, valid_range.(frist > last) is not yet implemented, first = %d, last = %d.", first, last);
    }

	m_nRangeFirst = first;
	m_nRangeLast  = last;
}

KDvoid M3GKeyframeSequence::sample ( KDint sequence, KDfloat* value ) const
{
    if ( value == KD_NULL )
	{
        M3GException ( "NullPointerException", __FUNCTION__, "Value is NULL." );
    }

    if ( m_nRangeFirst > m_nRangeLast ) 
	{
		M3GException ( "NotImplementedException", __FUNCTION__, "Sorry, valid_range.(first > last) is not yet implemented, first = %d, last = %d.", m_nRangeFirst, m_nRangeLast );
    }

    if ( m_nDuration <= 0 )
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Duraion is invalid, duration = %d.", m_nDuration );
    }

    KDint  first = m_nRangeFirst;
    KDint  last  = m_nRangeLast;

    if ( m_nRepeatMode == LOOP )
	{
        sequence %= m_nDuration;
    }

	if ( sequence <= m_vKeyframes [ first ].m_nTime )
	{
        for ( KDint i = 0; i < m_nComponentCount; i++ ) 
		{
			value [ i ] = m_vKeyframes [ first ].m_pValue [ i ];
        }

        return;
    }

    if ( sequence >= m_vKeyframes [ last ].m_nTime ) 
	{
        for ( KDint  i = 0; i < m_nComponentCount; i++ ) 
		{
            value [ i ] = m_vKeyframes [ last ].m_pValue [ i ];
        }

        return;
    }

    KDint  left  = -1;
    KDint  right = -1;

    for ( KDint  i = first; i <= last; i++ )
	{
		if ( m_vKeyframes [ i ].m_nTime > sequence )
		{
            left  = i - 1;
            right = i;
            break;
        }
    }

	KDfloat            s  = ( sequence - m_vKeyframes [ left ].m_nTime ) / (KDfloat) ( m_vKeyframes [ right ].m_nTime - m_vKeyframes [ left ].m_nTime );
    const M3GKeyframe& k0 = ( left == first && m_nRepeatMode == LOOP)     ? M3GKeyframe ( -1, 0 ) : 
                            ( left == first && m_nRepeatMode == CONSTANT) ? M3GKeyframe ( -1, 0 ) :
                            m_vKeyframes [ left - 1 ];
    const M3GKeyframe& k1 = m_vKeyframes [ left ];
    const M3GKeyframe& k2 = m_vKeyframes [ right ];
    const M3GKeyframe& k3 = ( right == last && m_nRepeatMode == LOOP)     ? M3GKeyframe ( -1, 0) :
                            ( right == last && m_nRepeatMode == CONSTANT) ? M3GKeyframe ( -1, 0) :
                            m_vKeyframes [ right + 1 ];

    switch ( m_nInterpType ) 
	{
		case STEP	: m3gStep	( s, k1, k2, m_nComponentCount, value ); break;
		case LINEAR : m3gLinear ( s, k1, k2, m_nComponentCount, value ); break;
		case SLERP	: m3gSlerp  ( s, k1, k2, m_nComponentCount, value ); break;

		case SPLINE : m3gSpline ( s, k0, k1, k2, k3, m_nComponentCount, value ); break;
		case SQUAD	: m3gSquad  ( s, k0, k1, k2, k3, m_nComponentCount, value ); break;

		default :

			M3GException ( "InternalException", __FUNCTION__, "Interpolation type is unknown, type = %d.", m_nInterpType );
    }	
}