/* --------------------------------------------------------------------------
 *
 *      File            M3GObject3D.h
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

#ifndef __M3GObject3D_h__
#define __M3GObject3D_h__

#include <vector>

class M3GAnimationTrack;
class M3GRenderState;

static const KDint	MAX_TEXTURE_UNITS = 2;

/**
 * @~English	An abstract base class for all objects that can be part of a 3D world.
 */
class M3GObject3D
{
    protected :
 
		/**
		 * @~English	Construct a new Objec3D object.
		 */
        M3GObject3D ( KDvoid );

	public :

		/**
		 * @~English	Destruct this object.
		 */
        virtual ~M3GObject3D ( KDvoid );

	public :

		/**
		 * @~English	Adds the given AnimationTrack to this Object3D, potentially changing the order and indices of the previously added tracks.
		 * @param[in]	animationTrack   
		 */
        virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		/**
		 * @~English	Updates all animated properties in this Object3D and all Object3Ds that are reachable from this Object3D.
		 * @param[in]	time 
		 */
        virtual KDint  animate ( KDint time );

		/**
		 * @~English	Creates a duplicate of this Object3D.
		 * @return
		 */ 
        virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Retrieves an object that has the given user ID and is reachable from this object.
		 * @param[in]	userID 
		 * @return
		 */
        M3GObject3D*  find ( KDint userID ) const;

		/**
		 * @~English	Gets an AnimationTrack by index.
		 * @param[in]	index 
		 * @return
		 */
        M3GAnimationTrack*  getAnimationTrack ( KDint index ) const;

		/**
		 * @~English	Gets the number of AnimationTracks currently associated with this Object3D.
		 * @return
		 */
        KDint	getAnimationTrackCount ( KDvoid ) const;
		
		/**
		 * @~English	Returns the number of direct Object3D references in this object, and fills in the objects to the given array.
		 * @param[out]	references 
		 * @return
		 */
        virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Gets the user ID of this object.
		 * @return
		 */
        KDint	getUserID ( KDvoid ) const;

		/**
		 * @~English	Retrieves the user object that is currently associated with this Object3D.
		 * @return
		 */
        KDvoid*  getUserObject ( KDvoid ) const;

		/**
		 * @~English	Removes the given AnimationTrack from this Object3D, potentially changing the order and indices of the remaining tracks.
		 * @param[in]	animationTrack 
		 */
        KDvoid	removeAnimationTrack ( M3GAnimationTrack* animationTrack );

		/**
		 * @~English	Sets the user ID for this object.
		 * @param[in]	userID 
		 */
        KDvoid	setUserID ( KDint userID );

		/**
		 * @~English	Associates an arbitrary, application specific Object with this Object3D.
		 * @param[in]	userObject 
		 */
        KDvoid	setUserObject ( KDvoid* userObject );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _addAnimationTrack ( M3GAnimationTrack* animationTrack );

		KDvoid  _duplicate ( M3GObject3D* pObj ) const;

    private :

        KDint								m_nUserID;
        KDvoid*								m_pUserObject;
        std::vector<M3GAnimationTrack*>		m_vAnimationTracks;
};

typedef std::vector<M3GObject3D*>			M3GObject3DArray;

#endif
