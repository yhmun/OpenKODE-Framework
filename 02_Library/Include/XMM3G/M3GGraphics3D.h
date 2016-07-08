/* --------------------------------------------------------------------------
 *
 *      File            M3GGraphics3D.h
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

#ifndef __M3GGraphics3D_h__
#define __M3GGraphics3D_h__

#include <map>

class M3GLight;
class M3GTransform;
class M3GBackground;
class M3GCamera;
class M3GNode;
class M3GVertexBuffer;
class M3GIndexBuffer;
class M3GAppearance;
class M3GWorld;

/**
 * @~English	A singleton 3D graphics context that can be bound to a rendering target.
 */
class M3GGraphics3D 
{
	public :

        /**
         * @~English	A parameter to bindTarget, specifying that antialiasing should be turned on.
         */
		static const KDint	ANTIALIAS   = 1 << 1;

        /**
         * @~English	A parameter to bindTarget, specifying that dither should be turned on.
         */
		static const KDint	DITHER      = 1 << 2;

        /**
         * @~English	A parameter to bindTarget, specifying that existing contents of the rendering targte need not be preserved.
         */
		static const KDint	OVERWRITE   = 1 << 4;

        /**
         * @~English	A parameter to bindTarget, specifying that true color rendering should be turned on.
         */
		static const KDint	TRUE_COLOR  = 1 << 3;

	protected :

		/**
		 * @~English	Forbiddened constructor of Graphics3D.
		 */
		M3GGraphics3D ( KDvoid );

	public :

		/**
		 * @~English	Destruct this object.
		 */
		~M3GGraphics3D ( KDvoid );

	public :

		/**
		 * @~English	Retrieves the singleton Graphics3D instance that is associated with this application.
		 * @return
		 */
		static M3GGraphics3D*  getInstance ( KDvoid );	  

	public :

		/**
		 * @~English	Binds a Light to use in subsequent immediate mode rendering.
         * @param[in]	light
         * @param[in]	transform	  
		 * @return
		 */
		KDint  addLight ( M3GLight* light, const M3GTransform& transform );

		/**
		 * @~English	Binds the given Graphics or mutable Image2D as the rendering target of this Graphics3D.
         * @param[in]	target	
		 */
		KDvoid	bindTarget ( KDvoid* target );

		/**
		 * @~English	Binds the given Graphics or mutable Image2D as the rendering target of this Graphics3D.
         * @param[in]	target
         * @param[in]	depthBuffer
         * @param[in]	hints
		 */
		KDvoid	bindTarget ( KDvoid* target, KDbool depthBuffer, KDint hints );

		/**
		 * @~English	Clears the viewport as specified in the given Background object.
         * @param[in]	target
		 */
		KDvoid	clear ( M3GBackground* background );

		/**
		 * @~English	Returns the current camera.
         * @param[in]	transform
		 * @return
		 */
		M3GCamera*  getCamera ( M3GTransform* transform );

		/**
		 * @~English	Returns the far distance of the depth range.
		 * @return
		 */
		KDfloat	 getDepthRangeFar ( KDvoid ) const;

		/**
		 * @~English	Returns the near distance of the depth range.
		 * @return
		 */
		KDfloat	 getDepthRangeNear ( KDvoid ) const;

		/**
		 * @~English	Returns the rendering hints given for the current rendering target.
		 * @return
		 */
		KDint  getHints ( KDvoid ) const;

		/**
		 * @~English	Returns a light in the current light array.
         * @param[in]	index
         * @param[in]	transform
		 * @return
		 */
		M3GLight*  getLight ( KDint index, M3GTransform* transform ) const;

		/**
		 * @~English	Returns the size of the current light array.
		 * @return
		 */
		KDint  getLightCount ( KDvoid ) const;

		/**
		 * @~English	Retrieves implementation specific properties.
		 * @return
		 */
		std::map<const KDchar*, KDint>  getProperties ( KDvoid ) const;

		/**
		 * @~English	Returns the current rendering target.
		 */
		KDvoid*	 getTarget ( KDvoid ) const;

		/**
		 * @~English	Returns the viewport.
		 * @return
		 */
		KDint*  getViewport ( KDvoid ) const;

		/**
		 * @~English	Returns the horizontal position of the viewport.
		 * @return
		 */
        KDint  getViewportX ( KDvoid ) const;

		/**
		 * @~English	Returns the vertical position of the viewport.
		 * @return
		 */
        KDint  getViewportY ( KDvoid ) const;

		/**
		 * @~English	Returns the width of the viewport.
		 * @return
		 */
        KDint  getViewportWidth ( KDvoid ) const;

		/**
		 * @~English	Returns the height of the viewport.
		 * @return
		 */
        KDint  getViewportHeight ( KDvoid ) const;

		/**
		 * @~English	Queries whether depth buffering is enabled for the current rendering target.
		 * @return
		 */
		KDbool	isDepthBufferEnabled ( KDvoid ) const;

		/**
		 * @~English	Flushes the rendered 3D image to the currently bound target and then releases the target.
		 */
		KDvoid	releaseTarget ( KDvoid );

		/**
		 * @~English	Renders the given Sprite3D, Mesh, or Group node with the given transformation from local coordinates to world coordinates.
         * @param[in]	node
         * @param[in]	transform
		 */
        KDvoid	render ( const M3GNode* node, const M3GTransform* transform ) const;

		/**
		 * @~English	Renders the given submesh with the given transformation from local coordinates to world coordinates.
         * @param[in]	vertices
         * @param[in]	triangles
         * @param[in]	apperance
         * @param[in]	transform
		 */
        KDvoid	render ( const M3GVertexBuffer* vertices, const M3GIndexBuffer* triangles, const M3GAppearance* apperance, const M3GTransform* transform ) const;

		/**
		 * @~English	Renders the given submesh with the given scope and the given transformation from local coordinates to world coordinates.
         * @param[in]	vertices
         * @param[in]	triangles
         * @param[in]	apperance
         * @param[in]	transform
		 * @param[in]	scope
		 */
        KDvoid	render ( const M3GVertexBuffer* vertices, const M3GIndexBuffer* triangles, const M3GAppearance* apperance, const M3GTransform* transform, KDint scope ) const;

		/**
		 * @~English	Renders an image of world as viewed by the active camera of that World.
         * @param[in]	world
		 */
		KDvoid	render ( const M3GWorld* world ) const;

		/**
		 * @~English	Clears the array of current Lights.
		 */
		KDvoid	resetLights ( KDvoid );

		/**
		 * @~English	Sets the Camera to use in subsequent immediate mode rendering.
         * @param[in]	camera
         * @param[in]	transform
		 */
		KDvoid	setCamera ( M3GCamera* camera, const M3GTransform& transform );

		/**
		 * @~English	Specifies the mapping of depth values from normalized device coordinates to window coordinates.
         * @param[in]	near
         * @param[in]	far
		 */
		KDvoid	setDepthRange ( KDfloat near, KDfloat far );

		/**
		 * @~English	Replaces or modifies a Light currently bound for immediate mode rendering.
         * @param[in]	index
         * @param[in]	light
         * @param[in]	transform
		 */
		KDvoid	setLight ( KDint index, M3GLight* light, const M3GTransform& transform );

		/**
		 * @~English	Specifies a rectangular viewport on the currently bound rendering target.
         * @param[in]	x
         * @param[in]	y
         * @param[in]	width
         * @param[in]	height
		 */
		KDvoid	setViewport ( KDint x, KDint y, KDint width, KDint height );

    private :

        std::map<const KDchar*, KDint>	m_mProperties;
};

#endif
