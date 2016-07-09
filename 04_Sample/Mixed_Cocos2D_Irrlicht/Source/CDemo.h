/* --------------------------------------------------------------------------
 *
 *      File            CDemo.h
 *      Description     This is a Demo of the Irrlicht Engine (c) 2006 by N.Gebhardt.
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

// 
// This file is not documented.

#ifndef __C_DEMO_H_INCLUDED__
#define __C_DEMO_H_INCLUDED__

const int CAMERA_COUNT = 7;

class CDemo : public IEventReceiver
{
	public :

		CDemo ( KDint nWidth, KDint nHeight );

		virtual ~CDemo ( KDvoid );

	public :

		KDvoid  EventProc ( const KDEvent* event );

		KDbool  Draw ( KDvoid );

		virtual bool  OnEvent ( const SEvent& event );

	public :

		KDvoid  CreateLoadingScreen ( KDvoid );
		KDvoid  LoadSceneData ( KDvoid );
		KDvoid  SwitchToNextScene ( KDvoid );
		KDvoid  Shoot ( KDvoid );
		KDvoid  CreateParticleImpacts ( KDvoid );

	public :

		irr::IrrlichtDevice*				m_pDevice;
		video::IVideoDriver*				m_pDriver;
		scene::ISceneManager*				m_pScrMgr;
		gui::IGUIEnvironment*				m_pGuiEnv;

		s32									m_nSceneStartTime;
		s32									m_nTimeForThisScene;

		video::SColor						m_tBackColor;

		s32									m_nCurrentScene;

		gui::IGUIStaticText*				m_pStatusText;
		gui::IGUIInOutFader*				m_pInOutFader;
		scene::IAnimatedMeshSceneNode*		m_pModel1;
		scene::IAnimatedMeshSceneNode*		m_pModel2;
		scene::IParticleSystemSceneNode*	m_pCampFire;

		scene::IQ3LevelMesh*				m_pQuakeLevelMesh;
		scene::ISceneNode*					m_pQuakeLevelNode;
		scene::ISceneNode*					m_pSkyboxNode;

		scene::IMetaTriangleSelector*		m_pMetaSelector;
		scene::ITriangleSelector*			m_pMapSelector;

		bool								m_bAdditive;

		struct SParticleImpact
		{
			u32					when;
			core::vector3df		pos;
			core::vector3df		outVector;
		};

		core::array<SParticleImpact>		m_aImpacts;

		KDint								m_nKeyCode;
};

#endif

