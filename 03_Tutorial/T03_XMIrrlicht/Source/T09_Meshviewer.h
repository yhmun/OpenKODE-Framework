/* --------------------------------------------------------------------------
 *
 *      File            T09_Meshviewer.h
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

#ifndef __T09_Meshviewer_h__
#define __T09_Meshviewer_h__

// Values used to identify individual GUI elements
enum
{
	GUI_ID_DIALOG_ROOT_WINDOW  = 0x10000,

	GUI_ID_X_SCALE,
	GUI_ID_Y_SCALE,
	GUI_ID_Z_SCALE,

	GUI_ID_OPEN_MODEL,
	GUI_ID_SET_MODEL_ARCHIVE,
	GUI_ID_LOAD_AS_OCTREE,

	GUI_ID_SKY_BOX_VISIBLE,
	GUI_ID_TOGGLE_DEBUG_INFO,

	GUI_ID_DEBUG_OFF,
	GUI_ID_DEBUG_BOUNDING_BOX,
	GUI_ID_DEBUG_NORMALS,
	GUI_ID_DEBUG_SKELETON,
	GUI_ID_DEBUG_WIRE_OVERLAY,
	GUI_ID_DEBUG_HALF_TRANSPARENT,
	GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES,
	GUI_ID_DEBUG_ALL,

	GUI_ID_MODEL_MATERIAL_SOLID,
	GUI_ID_MODEL_MATERIAL_TRANSPARENT,
	GUI_ID_MODEL_MATERIAL_REFLECTION,

	GUI_ID_CAMERA_MAYA,
	GUI_ID_CAMERA_FIRST_PERSON,

	GUI_ID_POSITION_TEXT,

	GUI_ID_ABOUT,
	GUI_ID_QUIT,

	GUI_ID_TEXTUREFILTER,
	GUI_ID_SKIN_TRANSPARENCY,
	GUI_ID_SKIN_ANIMATION_FPS,

	GUI_ID_BUTTON_SET_SCALE,
	GUI_ID_BUTTON_SCALE_MUL10,
	GUI_ID_BUTTON_SCALE_DIV10,
	GUI_ID_BUTTON_OPEN_MODEL,
	GUI_ID_BUTTON_SHOW_ABOUT,
	GUI_ID_BUTTON_SHOW_TOOLBOX,
	GUI_ID_BUTTON_SELECT_ARCHIVE,

	GUI_ID_ANIMATION_INFO,

	// And some magic numbers
	MAX_FRAMERATE = 80,
	DEFAULT_FRAMERATE = 30
};

class CT09_Meshviewer : public CTutorial
{
	public :
	
		CT09_Meshviewer ( KDvoid );

		virtual ~CT09_Meshviewer ( KDvoid );

	public :

		virtual  KDvoid  Draw ( KDvoid );

		virtual  video::SColor  getClear ( KDvoid );	

		virtual const wchar_t*  getTitle ( KDvoid );

		virtual  bool OnEvent ( const SEvent& event );

		bool  OnKeyUp ( irr::EKEY_CODE keyCode );

		void  OnMenuItemSelected ( gui::IGUIContextMenu* menu );

		void  OnTextureFilterSelected ( gui::IGUIComboBox* combo );

	private :

		void  setActiveCamera ( scene::ICameraSceneNode* newActive );

		void  setSkinTransparency ( s32 alpha, irr::gui::IGUISkin* skin );

		void  showAboutText ( );

		void  updateScaleInfo ( scene::ISceneNode* model );

		void  loadModel ( const c8* fn );

		void  createToolBox ( );

		void  updateToolBox ( );

		void  onKillFocus ( );

	private :

		core::stringc				StartUpModelFile;
		core::stringw				MessageText;
		core::stringw				Caption;
		scene::ISceneNode*			Model;
		scene::ISceneNode*			SkyBox;
		bool						Octree;
		bool						UseLight;
		scene::ICameraSceneNode*	Camera[2];

		gui::IGUIStaticText*		m_pTxtFps;
		gui::IGUIStaticText*		m_pTxtPos;
};

#endif