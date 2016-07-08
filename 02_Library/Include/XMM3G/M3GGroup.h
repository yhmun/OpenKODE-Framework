/* --------------------------------------------------------------------------
 *
 *      File            M3GGroup.h
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

#ifndef __M3GGroup_h__
#define __M3GGroup_h__

#include "M3GNode.h"

class M3GCamera;
class M3GRayIntersection;

/**
 * @~English	A scene graph node that stores an unordered set of nodes as its children.
 */
class M3GGroup : public M3GNode
{
	public :

		/**
		 * @~English	Constructs a new Group node and initializes it with an empty list of children.
		 */
		M3GGroup ( KDvoid );

		/**
		 * @~English	Destruct a this Group node.
		 */
		virtual ~M3GGroup ( KDvoid );

	public :

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Adds the given node to this Group, potentially changing the order and indices of the previously added children.
		 * @param[in]	child
		 */
		KDvoid	addChild ( M3GNode* child );

		/**
		 * @~English	Gets a child by index.
		 * @param[in]	index
		 * @return
		 */
		M3GNode*  getChild ( KDint index ) const;

		/**
		 * @~English	Gets the number of children in this Group.
		 * @return
		 */
		KDint	getChildCount ( KDvoid ) const;

		/**
		 * @~English	Picks the first Mesh or scaled Sprite3D in this Group that is enabled for picking,
		 *				is intercepted by the given pick ray, and is in the specified scope.
		 * @param[in]	scope
		 * @param[in]	x
		 * @param[in]	y
		 * @param[in]	camera
		 * @param[out]	ri
		 * @return
		 */
		KDbool	pick ( KDint scope, KDfloat x, KDfloat y, M3GCamera* camera, M3GRayIntersection* ri ) const;

		/**
		 * @~English	Picks the first Mesh in this Group that is intercepted by the given pick ray and is in the specified scope.
		 * @param[in]	scope
		 * @param[in]	ox
		 * @param[in]	oy
		 * @param[in]	oz
		 * @param[in]	dx
		 * @param[in]	dy
		 * @param[in]	dz
		 * @param[out]	ri
		 * @return
		 */
		KDbool	pick ( KDint scope, KDfloat ox, KDfloat oy, KDfloat oz, KDfloat dx, KDfloat dy, KDfloat dz, M3GRayIntersection* ri ) const;

		/**
		 * @~English	Removes the given node from this Group, potentially changing the order and indices of the remaining children.
		 * @param[in]	child
		 */
		KDvoid	removeChild ( M3GNode* child );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

		KDbool  isDescendant ( const M3GNode* node ) const;

	protected :

		KDvoid  _duplicate ( M3GGroup* pObj ) const;

	private :

		std::vector<M3GNode*>		m_vChildren;
};

#endif
