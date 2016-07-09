/* ------------------------------------------------------------------------------------
 *
 *      File            BezierPatch.cpp
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
#include "BezierPatch.h"

Sample_BezierPatch::Sample_BezierPatch ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Bezier Patch";
    m_aInfo [ "Description" ] = "A demonstration of the Bezier patch support.";
    m_aInfo [ "Thumbnail"   ] = "thumb_bezier.png";
    m_aInfo [ "Category"    ] = "Geometry";
}

KDvoid Sample_BezierPatch::checkBoxToggled ( CheckBox* pBox )
{
    m_pPatchPass->setPolygonMode ( pBox->isChecked ( ) ? PM_WIREFRAME : PM_SOLID );
    
#ifdef USE_RTSHADER_SYSTEM
    // Invalidate material in order to reflect polygon mode change in the generated shader based pass.
//    m_pShaderGenerator->invalidateMaterial ( RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, m_pPatchPass->getParent ( )->getParent ( )->getName ( ) );
#endif
}

KDvoid Sample_BezierPatch::sliderMoved ( Slider* pSlider )
{
    m_pPatch->setSubdivision ( pSlider->getValue ( ) );
}

KDvoid Sample_BezierPatch::setupContent ( KDvoid )
{
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.5, 0.5, 0.5 ) );
    m_pSceneMgr->createLight ( )->setPosition ( 100, 100, 100 );
    
    // define the control point vertices for our patch
    PatchVertex  aVerts[9] =
    {
        {-50, -35, -50, -0.5, 0.5, 0.0, 0.0, 0.0},
        {  0,   0, -50,  0.0, 0.5, 0.0, 0.5, 0.0},
        { 50,  35, -50,  0.5, 0.5, 0.0, 1.0, 0.0},
        {-50,   0,   0, -0.5, 0.5, 0.0, 0.0, 0.5},
        {  0,   0,   0,  0.0, 0.5, 0.0, 0.5, 0.5},
        { 50,   0,   0,  0.5, 0.5, 0.0, 1.0, 0.5},
        {-50,  35,  50, -0.5, 0.5, 0.0, 0.0, 1.0},
        {  0,   0,  50,  0.0, 0.5, 0.0, 0.5, 1.0},
        { 50, -35,  50,  0.5, 0.5, 0.0, 1.0, 1.0}
    };
    
    // specify a vertex format declaration for our patch: 3 floats for position, 3 floats for normal, 2 floats for UV
    m_pDecl = HardwareBufferManager::getSingleton ( ).createVertexDeclaration ( );
    m_pDecl->addElement ( 0, 0, VET_FLOAT3, VES_POSITION );
    m_pDecl->addElement ( 0, sizeof ( KDfloat ) * 3, VET_FLOAT3, VES_NORMAL );
    m_pDecl->addElement ( 0, sizeof ( KDfloat ) * 6, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0 );
    
    // create a patch mesh using vertices and declaration
    m_pPatch = MeshManager::getSingleton ( ).createBezierPatch
    (
        "patch", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, (KDfloat*) aVerts, m_pDecl, 3, 3, 5, 5, PatchSurface::VS_BOTH
    );
    
    m_pPatch->setSubdivision ( 0 );   // start at 0 detail
    
    // create a patch entity from the mesh, give it a material, and attach it to the origin
    Entity* pEntity = m_pSceneMgr->createEntity ( "Patch", "patch" );
    pEntity->setMaterialName ( "Examples/BumpyMetal" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pEntity );
    
    // save the main pass of the material so we can toggle wireframe on it
    m_pPatchPass = pEntity->getSubEntity ( 0 )->getMaterial ( )->getTechnique ( 0 )->getPass ( 0 );
    
    // use an orbit style camera
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pCameraMan->setYawPitchDist ( Degree ( 0 ), Degree ( 30 ), 250 );
    
    m_pTrayMgr->showCursor ( );
    
    // create slider to adjust detail and checkbox to toggle wireframe
    m_pTrayMgr->createThickSlider ( TL_TOPLEFT, "Detail"   , "Detail"   , 120, 44, 0, 1, 6 );
    m_pTrayMgr->createCheckBox    ( TL_TOPLEFT, "Wireframe", "Wireframe", 120 );
}

KDvoid Sample_BezierPatch::cleanupContent ( KDvoid )
{
    HardwareBufferManager::getSingleton ( ).destroyVertexDeclaration ( m_pDecl );
    m_pPatchPass->setPolygonMode ( PM_SOLID );
    MeshManager::getSingleton ( ).remove ( m_pPatch->getHandle ( ) );
}