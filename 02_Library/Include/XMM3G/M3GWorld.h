/* --------------------------------------------------------------------------
 *
 *      File            M3GWorld.h 
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

#ifndef __M3GWorld_h__
#define __M3GWorld_h__

#include "M3GGroup.h"

class M3GCamera;
class M3GBackground;

/**
 * @~English	A special Group node that is a top-level container for scene graphs.
 */
class M3GWorld : public M3GGroup
{
	public :

		/**
		 * @~English	Creates an empty World with default values.
		 */
		M3GWorld ( KDvoid );

		/**
		 * @~English	Destructs this objects.
		 */
		virtual ~M3GWorld ( KDvoid );

	public :

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Gets the currently active camera.
		 * @return
		 */
		M3GCamera*  getActiveCamera ( KDvoid ) const;

		/**
		 * @~English	Retrieves the background settings of this World.
		 * @return
		 */
		M3GBackground*  getBackground ( KDvoid ) const;

		/**
		 * @~English	Sets the Camera to use when rendering this World.
		 * @param[in]	camera
		 */
		KDvoid	setActiveCamera ( M3GCamera* camera );

		/**
		 * @~English	Sets the Background object for this World.
		 * @param[in]	background
		 */
		KDvoid	setBackground ( M3GBackground* background );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GWorld* pObj ) const;

	private :

        M3GBackground*		m_pBackground;
        M3GCamera*			m_pActiveCamera;
};


#endif
