/* --------------------------------------------------------------------------
 *
 *      File            M3GObject3D.cpp
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
 */ 

#include "M3GInternal.h"
#include <XMM3G/M3GObject3D.h>
#include <XMM3G/M3GAnimationTrack.h>

M3GObject3D::M3GObject3D ( KDvoid )
: m_nUserID ( 0 )
, m_pUserObject ( KD_NULL )
{

}

M3GObject3D::~M3GObject3D ( KDvoid )
{

}

KDvoid M3GObject3D::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
	if ( !animationTrack )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Animation track is Null." );
	}

	M3GException ( "IllegalArgumentException", __FUNCTION__, "AnimationTrack property is invalid, pro = %d", animationTrack->getTargetProperty ( ) );
}

KDvoid M3GObject3D::_addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
	if ( !animationTrack )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Animation track is Null." );
	}

	m_vAnimationTracks.push_back ( animationTrack );
}

KDint M3GObject3D::animate ( KDint time )
{
	return 0;
}

M3GObject3D* M3GObject3D::duplicate ( KDvoid ) const
{
	M3GObject3D*  pObj = new M3GObject3D ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GObject3D::_duplicate ( M3GObject3D* pObj ) const
{
	pObj->m_nUserID = m_nUserID;
	pObj->m_pUserObject = m_pUserObject;
	pObj->m_vAnimationTracks = m_vAnimationTracks;	
}

M3GObject3D* M3GObject3D::find ( KDint userID ) const
{
	if ( m_nUserID == userID )
	{
		return const_cast<M3GObject3D*> ( this );
	}

	for ( KDuint  i = 0; i < m_vAnimationTracks.size ( ); i++ )
	{
		if ( m_vAnimationTracks [ i ]->getUserID ( ) == userID )
		{
			return m_vAnimationTracks [ i ];
		}
	}

	return KD_NULL;
}

M3GAnimationTrack* M3GObject3D::getAnimationTrack ( KDint index ) const
{
	if ( index < 0 || index >= (KDint) m_vAnimationTracks.size ( ) ) 
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d, max = %d.", index, m_vAnimationTracks.size ( ) );
	}

	return m_vAnimationTracks [ index ];
}

KDint M3GObject3D::getAnimationTrackCount ( KDvoid ) const
{
	return m_vAnimationTracks.size ( );
}

KDint M3GObject3D::getReferences ( M3GObject3D** references ) const
{
	KDint  nNumber = m_vAnimationTracks.size ( );

	if ( references )
	{
        for ( KDint  i = 0; i < nNumber; i++ )
		{
            references [ i ] = m_vAnimationTracks [ i ];
		}
	}

	return nNumber;
}

KDint M3GObject3D::getUserID ( KDvoid ) const
{
	return m_nUserID;
}

KDvoid* M3GObject3D::getUserObject ( KDvoid ) const
{
	return m_pUserObject;
}

KDvoid M3GObject3D::removeAnimationTrack ( M3GAnimationTrack* animationTrack )
{
	std::vector<M3GAnimationTrack*>::iterator  iter;

	for ( iter = m_vAnimationTracks.begin ( ); iter != m_vAnimationTracks.end ( ); iter++ )
	{
        if ( *iter == animationTrack ) 
		{
            m_vAnimationTracks.erase ( iter );
            break;
        }
    }
}

KDvoid M3GObject3D::setUserID ( KDint userID )
{
	m_nUserID = userID;
}

KDvoid M3GObject3D::setUserObject ( KDvoid* userObject )
{
	m_pUserObject = userObject;
}

KDvoid M3GObject3D::render ( M3GRenderState& state ) const
{

}


