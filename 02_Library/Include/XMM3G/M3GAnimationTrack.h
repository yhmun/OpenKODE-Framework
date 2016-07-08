/* --------------------------------------------------------------------------
 *
 *      File            M3GAnimationTrack.h
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

#ifndef __M3GAnimationTrack_h__
#define __M3GAnimationTrack_h__

#include "M3GObject3D.h"

class M3GKeyframeSequence;
class M3GAnimationController;

/**
 * @~English	Associates a KeyframeSequence with an AnimationController and an animatable property.			
 */
class M3GAnimationTrack : public M3GObject3D
{
	public :

		/**
		 * @~English	Specifies the alpha factor of a Node, or the alpha component of the Background color,
		 *				Material diffuse color, or VertexBuffer default color as an animation target.
		 */
		static const KDint	ALPHA             = 256;

		/**
		 * @~English	Specifies the ambient color of a Material as an animation target.
		 */
		static const KDint	AMBIENT_COLOR     = 257;

		/**
		 * @~English	Specifies the color of a Light, Background, or Fog, or the texture blend color in Texture2D,
		 *				or the VertexBuffer default color as an animation target.
		 */
		static const KDint	COLOR             = 258;

		/**
		 * @~English	Specifies the cropping parameters of a Sprite3D or Background as an animation target.
		 */
		static const KDint	CROP              = 259;

		/**
		 * @~English	Specifies the fog density in Fog as an animation target.
		 */
		static const KDint	DENSITY           = 260;

		/**
		 * @~English	Specifies the diffuse color of a Material as an animation target.
		 */
		static const KDint	DIFFUSE_COLOR     = 261;

		/**
		 * @~English	Specifies the emissive color of a Material as an animation target.
		 */
		static const KDint	EMISSIVE_COLOR    = 262;

		/**
		 * @~English	Specifies the far distance of a Camera or Fog as an animation target.
		 */
		static const KDint	FAR_DISTANCE      = 263;

		/**
		 * @~English	Specifies the field of view of a Camera as an animation target.
		 */
		static const KDint	FIELD_OF_VIEW     = 264;

		/**
		 * @~English	Specifies the intensity of a Light as an animation target.
		 */
		static const KDint	INTENSITY         = 265;

		/**
		 * @~English	Specifies the morph target weights of a MorphingMesh as an animation target.
		 */
		static const KDint	MORPH_WEIGHTS     = 266;

		/**
		 * @~English	Specifies the near distance of a Camera or Fog as an animation target.
		 */
		static const KDint	NEAR_DISTANCE     = 267;

		/**
		 * @~English	Specifies the orientation (R) component of a Transformable object as an animation target.
		 */
		static const KDint	ORIENTATION       = 268;

		/**
		 * @~English	Specifies the picking enable flag of a Node as an animation target.
		 */
		static const KDint	PICKABILITY       = 269;

		/**
		 * @~English	Specifies the scale (S) component of a Transformable object as an animation target.
		 */
		static const KDint	SCALE             = 270;

		/**
		 * @~English	Specifies the shininess of a Material as an animation target.
		 */
		static const KDint	SHININESS         = 271;

		/**
		 * @~English	Specifies the specular color of a Material as an animation target.
		 */
		static const KDint	SPECULAR_COLOR    = 272;

		/**
		 * @~English	Specifies the spot angle of a Light as an animation target.
		 */
		static const KDint	SPOT_ANGLE        = 273;

		/**
		 * @~English	Specifies the spot exponent of a Light as an animation target.
		 */
		static const KDint	SPOT_EXPONENT     = 274;

		/**
		 * @~English	Specifies the translation (T) component of a Transformable object as an animation target.
		 */
		static const KDint	TRANSLATION       = 275;

		/**
		 * @~English	Specifies the rendering enable flag of a Node as an animation target.
		 */
		static const KDint	VISIBILITY        = 276;

	protected :

		M3GAnimationTrack ( KDvoid );

	public :

		/**
		 * @~English	Creates an animation track with the given keyframe sequence targeting the given property.
		 * @param[in]	sequence	
		 * @param[in]	property	
		 */
		M3GAnimationTrack ( M3GKeyframeSequence* sequence, KDint _property );

		/**
		 * @~English	
		 */
		virtual ~M3GAnimationTrack ( KDvoid );

	public :

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Retrieves the animation controller used for controlling this animation track.
		 * @return	
		 */
		M3GAnimationController*  getController ( KDvoid ) const;

		/**
		 * @~English	Returns the keyframe sequence object which defines the keyframe values for this animation track.
		 * @return	
		 */
		M3GKeyframeSequence*  getKeyframeSequence ( KDvoid ) const;

		/**
		 * @~English	Returns the property targeted by this AnimationTrack.
		 */
		KDint  getTargetProperty ( KDvoid ) const;

		/**
		 * @~English	Specifies the animation controller to be used for controlling this animation track.
		 * @param[in]	controller	
		 */
		KDvoid	setController ( M3GAnimationController* controller );

	protected :

		KDvoid  init ( M3GKeyframeSequence* sequence, KDint _property );

		KDvoid  _duplicate ( M3GAnimationTrack* pObj ) const;

	private :

        KDint						m_nProperty;
        M3GKeyframeSequence*		m_pKeyframeSequence;
        M3GAnimationController*		m_pAnimationController;

		friend class M3GLoader;
};


#endif
