/* ------------------------------------------------------------------------------------
 *
 *      File            CelShading.cpp
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
#include "CelShading.h"

Sample_CelShading::Sample_CelShading ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Cel-shading";
    m_aInfo [ "Description" ] = "A demo of cel-shaded graphics using vertex & fragment programs.";
    m_aInfo [ "Thumbnail"   ] = "thumb_cel.png";
    m_aInfo [ "Category"    ] = "Lighting";
}

StringVector Sample_CelShading::getRequiredPlugins ( KDvoid )
{
    StringVector  aNames;
    if ( !GpuProgramManager::getSingleton ( ).isSyntaxSupported ( "glsles" ) )
    {
        aNames.push_back ( "Cg Program Manager" );
    }
    return aNames;
}

KDvoid Sample_CelShading::testCapabilities ( const RenderSystemCapabilities* pCaps )
{
    if ( !pCaps->hasCapability ( RSC_VERTEX_PROGRAM ) || !pCaps->hasCapability ( RSC_FRAGMENT_PROGRAM ) )
    {
        OGRE_EXCEPT
        (
            Exception::ERR_NOT_IMPLEMENTED, "Your graphics card does not support vertex and fragment"
            " programs, so you cannot run this sample. Sorry!", "Sample_CelShading::testCapabilities"
        );
    }
}

bool Sample_CelShading::frameRenderingQueued ( const FrameEvent& tEvent )
{
    // make the light revolve around our model if and only if the check box is checked
    if ( m_pMoveLight->isChecked ( ) )
    {
        m_pLightPivot->yaw ( Degree ( tEvent.timeSinceLastFrame * 30 ) );
    }
    
    return SdkSample::frameRenderingQueued ( tEvent );  // don't forget the parent class updates!
}

KDvoid Sample_CelShading::setupContent ( KDvoid )
{
    m_pViewport->setBackgroundColour ( ColourValue::White );
    
    // set our camera to orbit around the origin and show cursor
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pTrayMgr->showCursor ( );
    
    // create a basic point light with an offset
    Light*  pLight = m_pSceneMgr->createLight ( );
    pLight->setPosition ( 20, 40, 50 );
    
    // attach the light to a pivot node
    m_pLightPivot = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    m_pLightPivot->attachObject ( pLight );
    
    // create our model, give it the shader material, and place it at the origin
    Entity*  pEntity = m_pSceneMgr->createEntity ( "Head", "ogrehead.mesh" );
    pEntity->setMaterialName ( "Examples/CelShading" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pEntity );
    
    /* We set the same material for all parts of the head, but use custom shader parameters to set the
     colours for each part. See Examples-Advanced.material for how these are bound to GPU parameters. */
    
    SubEntity*  pSub;
    
    pSub = pEntity->getSubEntity ( 0 );    // eyes
    pSub->setCustomParameter ( SP_SHININESS , Vector4 (  35,   0,   0, 0 ) );
    pSub->setCustomParameter ( SP_DIFFUSE   , Vector4 (   1, 0.3, 0.3, 1 ) );
    pSub->setCustomParameter ( SP_SPECULAR  , Vector4 (   1, 0.6, 0.6, 1 ) );
    
    pSub = pEntity->getSubEntity ( 1 );    // skin
    pSub->setCustomParameter ( SP_SHININESS , Vector4 (  10,   0,   0, 0 ) );
    pSub->setCustomParameter ( SP_DIFFUSE   , Vector4 (   0, 0.5,   0, 1 ) );
    pSub->setCustomParameter ( SP_SPECULAR  , Vector4 ( 0.3, 0.5, 0.3, 1 ) );
    
    pSub = pEntity->getSubEntity ( 2 );    // earring
    pSub->setCustomParameter ( SP_SHININESS , Vector4 (  25,   0,   0, 0 ) );
    pSub->setCustomParameter ( SP_DIFFUSE   , Vector4 (   1,   1,   0, 1 ) );
    pSub->setCustomParameter ( SP_SPECULAR  , Vector4 (   1,   1, 0.7, 1 ) );
    
    pSub = pEntity->getSubEntity ( 3 );    // teeth
    pSub->setCustomParameter ( SP_SHININESS , Vector4 (  20,   0,   0, 0 ) );
    pSub->setCustomParameter ( SP_DIFFUSE   , Vector4 (   1,   1, 0.7, 1 ) );
    pSub->setCustomParameter ( SP_SPECULAR  , Vector4 (   1,   1,   1, 1 ) );
    
    // create a check box to toggle light movement
    m_pMoveLight = m_pTrayMgr->createCheckBox ( TL_TOPLEFT, "MoveLight", "Move Light" );
    m_pMoveLight->setChecked ( true );
}

