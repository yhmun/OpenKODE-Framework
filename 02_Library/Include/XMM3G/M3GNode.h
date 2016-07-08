/* --------------------------------------------------------------------------
 *
 *      File            M3GNode.h
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

#ifndef __M3GNode_h__
#define __M3GNode_h__

#include "M3GTransformable.h"

class M3GMatrix;

/**
 * @~English	
 */
class M3GNode : public M3GTransformable
{
	public :

		/**
		 * @~English	Specifies for the setAlignment method that no alignment should be done for the specified axis.
		 */
		static const KDint	NONE   = 144;

		/**
		 * @~English	Specifies the origin of the reference node as an orientation reference for the setAlignment method.
		 */
		static const KDint	ORIGIN = 145;

		/**
		 * @~English	Specifies the X axis of the reference node as an orientation reference for the setAlignment method.
		 */
		static const KDint	X_AXIS = 146;

		/**
		 * @~English	Specifies the Y axis of the reference node as an orientation reference for the setAlignment method.
		 */
		static const KDint	Y_AXIS = 147;

		/**
		 * @~English	Specifies the Z axis of the reference node as an orientation reference for the setAlignment method.
		 */
		static const KDint	Z_AXIS = 148;

	public :

		/**
		 * @~English	Construct a new Node object. 
		 */
		M3GNode ( KDvoid );

		/**
		 * @~English	Destruct this object.
		 */
		virtual ~M3GNode ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Applies alignments to this Node and its descendants.
		 * @param[in]	reference
		 */
		KDvoid	align ( const M3GNode* reference );

		/**
		 * @~English	Returns the alignment reference node for the given axis.
		 * @param[in]	axis
		 * @return
		 */
		M3GNode*  getAlignmentReference ( KDint axis ) const;

		/**
		 * @~English	Returns the alignment target for the given axis.
		 * @param[in]	axis
		 * @return
		 */
		KDint	getAlignmentTarget ( KDint axis ) const;

		/**
		 * @~English	Retrieves the alpha factor of this Node.
		 * @return
		 */
		KDfloat getAlphaFactor ( KDvoid ) const;

		/**
		 * @~English	Returns the scene graph parent of this node.
		 */
		M3GNode*  getParent ( KDvoid ) const;

		/**
		 * @~English	Retrieves the scope of this Node.
		 * @return
		 */
		KDint	getScope ( KDvoid ) const;

		/**
		 * @~English	Gets the composite transformation from this node to the given node.
		 * @param[in]	target
		 * @param[out]	transform
		 * @return
		 */
		KDbool	getTransformTo ( const M3GNode* target, M3GTransform* transform ) const;

		/**
		 * @~English	Retrieves the picking enable flag of this Node.
		 * @return
		 */
		KDbool	isPickingEnabled ( KDvoid ) const;

		/**
		 * @~English	Retrieves the rendering enable flag of this Node.
		 * @return
		 */
		KDbool	isRenderingEnabled ( KDvoid ) const;

		/**
		 * @~English	Sets this node to align with the given other node(s), or disables alignment.
		 * @param[in]	zRef
		 * @param[in]	zTarget
		 * @param[in]	yRef
		 * @param[in]	yTarget
		 */
		KDvoid	setAlignment ( M3GNode* zRef, KDint zTarget, M3GNode* yRef, KDint yTarget );

		/**
		 * @~English	Sets the alpha factor for this Node.
		 * @param[in]	alphaFactor
		 */
		KDvoid	setAlphaFactor ( KDfloat alphaFactor );

		/**
		 * @~English	Sets the picking enable flag of this Node.
		 * @param[in]	enable
		 */
		KDvoid	setPickingEnable ( KDbool enable );

		/**
		 * @~English	Sets the rendering enable flag of this Node.
		 * @param[in]	enable 
		 */
		KDvoid	setRenderingEnable ( KDbool enable );

		/**
		 * @~English	Sets the scope of this node.
		 * @param[in]	scope 
		 */
		KDvoid	setScope ( KDint scope );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

        KDfloat  getGlobalAlphaFactor ( KDvoid ) const;

        M3GMatrix  getGlobalPose ( const M3GNode* target ) const;

        M3GNode*  getGlobalParent ( KDvoid ) const;

		KDbool  isGlobalPickingEnabled ( KDvoid ) const;

		KDbool  isGlobalRenderingEnabled ( KDvoid ) const;

		KDvoid  setParent ( M3GNode* node );

	protected :

		KDvoid  _duplicate ( M3GNode* pObj ) const;

	private :

        M3GNode*			m_pParent;
		M3GNode*			m_pDuplicated;
        KDbool				m_bRenderingEnable;
        KDbool				m_bPickingEnable;
        KDfloat				m_fAlphaFactor;
        KDint				m_nScope;
		M3GNode*			m_pZRef;
		M3GNode*			m_pYRef;
		KDint				m_nZTarget;
		KDint				m_nYTarget;
};


#endif
