/* --------------------------------------------------------------------------
 *
 *      File            T12_TerrainRendering.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
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

#ifndef __T12_TerrainRendering_h__
#define __T12_TerrainRendering_h__

/** Example 012 Terrain Rendering

	This tutorial will briefly show how to use the terrain renderer of Irrlicht. It
	will also show the terrain renderer triangle selector to be able to do
	collision detection with terrain.

	Note that the Terrain Renderer in Irrlicht is based on Spintz'
	GeoMipMapSceneNode, lots of thanks go to him. DeusXL provided a new elegant
	simple solution for building larger area on small heightmaps -> terrain
	smoothing.

	In the beginning there is nothing special. We include the needed header files
	and create an event listener to listen if the user presses a key: The 'W' key
	switches to wireframe mode, the 'P' key to pointcloud mode, and the 'D' key
	toggles between solid and detail mapped material.
*/
class CT12_TerrainRendering : public CTutorial
{
	public :
	
		CT12_TerrainRendering ( KDvoid );

		virtual ~CT12_TerrainRendering ( KDvoid );

	public :

		virtual const wchar_t*  getTitle ( KDvoid );

		virtual bool OnEvent ( const SEvent& event );

	private :

		scene::ISceneNode*		m_pTerrain;
		scene::ISceneNode*		m_pSkybox;
		scene::ISceneNode*		m_pSkydome;
		bool					m_bShowBox;
		bool					m_bShowDebug;
};

#endif