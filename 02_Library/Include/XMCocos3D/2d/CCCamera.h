/* -----------------------------------------------------------------------------------
 *
 *      File            CCCamera.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCCamera_h__
#define __CCCamera_h__

#include "../base/CCObject.h"
#include "XMKazmath/mat4.h"


NS_CC_BEGIN

/**
 *	@addtogroup base_nodes
 *	@{
 */

/** 
 *	A Camera is used in every Node.
 *	Useful to look at the object from different views.
 *	The OpenGL gluLookAt() function is used to locate the
 *	camera.
 *
 *	If the object is transformed by any of the scale, rotation or
 *	position attributes, then they will override the camera.
 *
 *	IMPORTANT: Either your use the camera or the rotation/scale/position properties. You can't use both.
 *	World coordinates won't work if you use the camera.
 *
 *	Limitations:
 *
 *		- Some nodes, like ParallaxNode, Particle uses world node coordinates, and they won't work properly if you move them (or any of their ancestors)
 *		  using the camera.
 *
 *		- It doesn't work on batched nodes like Sprite objects when they are parented to a SpriteBatchNode object.
 *
 *		- It is recommended to use it ONLY if you are going to create 3D effects. For 2D effects, use the action Follow or position/scale/rotate.
 *
 */
class CC_DLL Camera : public Object
{
	public :

		/** returns the Z eye */
		static KDfloat		getZEye ( KDvoid );

		/**
		 *	@js ctor
		 */
		Camera ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~Camera ( KDvoid  );

		KDvoid				init ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		const KDchar*		description ( KDvoid ) const;

		/** sets the dirty value */
		inline KDvoid		setDirty ( KDbool bValue ) { m_bDirty = bValue; }

		/** get the dirty value */
		inline KDbool		isDirty ( KDvoid ) const { return m_bDirty; }

		/** sets the camera in the default position */
		KDvoid				restore ( KDvoid );

		/** Sets the camera using gluLookAt using its eye, center and up_vector */
		KDvoid				locate ( KDvoid );

		/** sets the eye values in points */
		KDvoid				setEye ( KDfloat fEyeX, KDfloat fEyeY, KDfloat fEyeZ );

		/** sets the center values in points */
		KDvoid				setCenter ( KDfloat fCenterX, KDfloat fCenterY, KDfloat fCenterZ );

		/** sets the up values */
		KDvoid				setUp ( KDfloat fUpX, KDfloat fUpY, KDfloat fUpZ );

		/** 
		 *	get the eye vector values in points 
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js: var getEye()
		 *	in lua:local getEye()
		 *	@endcode
		 */
		KDvoid				getEye ( KDfloat* pEyeX, KDfloat* pEyeY, KDfloat* pEyeZ ) const;

		/** 
		 *	get the center vector values int points 
		 *	when this function bound to js or lua,the input params are changed
		 *	in js: var getCenter()
		 *	in lua:local getCenter()
		 */
		KDvoid				getCenter ( KDfloat* pCenterX, KDfloat* pCenterY, KDfloat* pCenterZ ) const;

		/** 
		 *	get the up vector values 
		 *	when this function bound to js or lua,the input params are changed
		 *	in js: var getUp()
		 *	in lua:local getUp()
		 */
		KDvoid				getUp ( KDfloat* pUpX, KDfloat* pUpY, KDfloat* pUpZ ) const;

	protected :

		KDfloat				m_fEyeX;
		KDfloat				m_fEyeY;
		KDfloat				m_fEyeZ;

		KDfloat				m_fCenterX;
		KDfloat				m_fCenterY;
		KDfloat				m_fCenterZ;

		KDfloat				m_fUpX;
		KDfloat				m_fUpY;
		KDfloat				m_fUpZ;

		KDbool				m_bDirty;
		kmMat4				m_tLookupMatrix;

	private :

		DISALLOW_COPY_AND_ASSIGN ( Camera );
};

//	end of base_node group
/// @}

NS_CC_END

#endif // __CCCamera_h__
