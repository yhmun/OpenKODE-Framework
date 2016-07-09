/* ------------------------------------------------------------------------------------
 *
 *      File            CameraTrack.cpp
 *      Description     This source file is part of OGRE
 *                      (Object-oriented Graphics Rendering Engine)
 *      Author          Y.H Mun
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 *
 *      Contact Email: xmsoft77@gmail.com
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2012 Torus Knot Software Ltd.
 *
 *      For the latest info, see http://www.ogre3d.org/
 *
 * ------------------------------------------------------------------------------------
 *
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ------------------------------------------------------------------------------------ */

#include "Precompiled.h"
#include "CharacterSample.h"

Sample_Character::Sample_Character ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Character";
    m_aInfo [ "Description" ] = "A demo showing 3rd-person character control and use of TagPoints.";
    m_aInfo [ "Thumbnail"   ] = "thumb_char.png";
    m_aInfo [ "Category"    ] = "Animation";
    m_aInfo [ "Help"        ] = "Use the WASD keys to move Sinbad, and the space bar to jump. "
    "Use mouse to look around and mouse wheel to zoom. Press Q to take out or put back "
    "Sinbad's swords. With the swords equipped, you can left click to slice vertically or "
    "right click to slice horizontally. When the swords are not equipped, press E to "
    "start/stop a silly dance routine.";
}

bool Sample_Character::frameRenderingQueued ( const FrameEvent& tEvent )
{
    // let character update animations and camera
    m_pChara->addTime ( tEvent.timeSinceLastFrame );
    return SdkSample::frameRenderingQueued ( tEvent );
}

bool Sample_Character::touchMoved ( const KDEventTouches& tTouches )
{
    // relay input events to character controller
    if ( !m_pTrayMgr->isDialogVisible ( ) )
    {
        m_pChara->injectTouchMoved ( tTouches );
    }
    
    return SdkSample::touchMoved ( tTouches );
}

bool Sample_Character::touchPressed ( const KDEventTouches& tTouches )
{
    // relay input events to character controller
    if ( !m_pTrayMgr->isDialogVisible ( ) )
    {
        m_pChara->injectTouchPressed ( tTouches );
    }
    
    return SdkSample::touchPressed ( tTouches );
}

KDvoid Sample_Character::setupContent ( KDvoid )
{
    // set background and some fog
    m_pViewport->setBackgroundColour ( ColourValue ( 1.0f, 1.0f, 0.8f ) );
    m_pSceneMgr->setFog ( FOG_LINEAR, ColourValue ( 1.0f, 1.0f, 0.8f ), 0, 15, 100 );
    
    // set shadow properties
    m_pSceneMgr->setShadowTechnique ( SHADOWTYPE_TEXTURE_MODULATIVE );
    m_pSceneMgr->setShadowColour ( ColourValue ( 0.5, 0.5, 0.5 ) );
    m_pSceneMgr->setShadowTextureSize ( 1024 );
    m_pSceneMgr->setShadowTextureCount ( 1 );
    
    // disable default camera control so the character can do its own
    m_pCameraMan->setStyle ( CS_MANUAL );
    
    // use a small amount of ambient lighting
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.3, 0.3 ) );
    
    // add a bright light above the scene
    Light*  pLight = m_pSceneMgr->createLight ( );
    pLight->setType ( Light::LT_POINT );
    pLight->setPosition ( -10, 40, 20 );
    pLight->setSpecularColour ( ColourValue::White );
    
    // create a floor mesh resource
    MeshManager::getSingleton ( ).createPlane
    (
        "floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Plane ( Vector3::UNIT_Y, 0 ), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z
    );
    
    // create a floor entity, give it a material, and place it at the origin
    Entity*  pFloor = m_pSceneMgr->createEntity ( "Floor", "floor" );
    pFloor->setMaterialName ( "Examples/Rockwall" );
    pFloor->setCastShadows ( false );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pFloor );
    
    // create our character controller
    m_pChara = new SinbadCharacterController ( m_pCamera );
    
    m_pTrayMgr->toggleAdvancedFrameStats();
    
    StringVector  aItems;
    aItems.push_back ( "Help" );
    ParamsPanel*  pHelp = m_pTrayMgr->createParamsPanel ( TL_TOPLEFT, "HelpMessage", 100, aItems );
    pHelp->setParamValue ( "Help", "H / F1" );
}

KDvoid Sample_Character::cleanupContent ( KDvoid )
{
    // clean up character controller and the floor mesh
    if ( m_pChara )
    {
        delete m_pChara;
        m_pChara = 0;
    }
    MeshManager::getSingleton ( ).remove ( "floor" );
}
