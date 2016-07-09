/* ------------------------------------------------------------------------------------
 *
 *      File            TextureArray.cpp
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
#include "TextureArray.h"

Sample_TextureArray::Sample_TextureArray ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Texture Array";
    m_aInfo [ "Description" ] = "Demonstrates texture array support.";
    m_aInfo [ "Thumbnail"   ] = "thumb_texarray.png";
    m_aInfo [ "Category"    ] = "Unsorted";
    m_aInfo [ "Help"        ] = "Top Left: Multi-frame\nTop Right: Scrolling\nBottom Left: Rotation\nBottom Right: Scaling";
}

KDvoid Sample_TextureArray::testCapabilities( const RenderSystemCapabilities* pCaps )
{
    if ( !pCaps->hasCapability ( RSC_VERTEX_PROGRAM ) || !pCaps->hasCapability ( RSC_FRAGMENT_PROGRAM ) )
    {
        OGRE_EXCEPT ( Exception::ERR_NOT_IMPLEMENTED, "Your graphics card does not support vertex and "
                      "fragment programs, so you cannot run this sample. Sorry!",
                      "TextureArray::testCapabilities" );
    }
    
    if ( !GpuProgramManager::getSingleton().isSyntaxSupported ( "vs_4_0" ) &&
         !GpuProgramManager::getSingleton().isSyntaxSupported ( "ps_2_0" ) &&
         !GpuProgramManager::getSingleton().isSyntaxSupported ( "glsl"   ) &&
         !GpuProgramManager::getSingleton().isSyntaxSupported ( "gp4fp"  ) )
    {
        OGRE_EXCEPT ( Exception::ERR_NOT_IMPLEMENTED, "Your card does not support the shader model needed for this sample, "
                      "so you cannot run this sample. Sorry!", "TextureArray::testCapabilities" );
    }
}

KDvoid Sample_TextureArray::setupContent ( KDvoid )
{
    m_pSceneMgr->setSkyBox ( true, "Examples/TrippySkyBox" );
    
    // set our camera to orbit around the origin and show cursor
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pTrayMgr->showCursor ( );

    // the names of the textures we will use (all need to be the same size: 512*512 in our case)
    vector<String>::type  aTexNames;
    aTexNames.push_back ( "BeachStones.jpg"       );
    aTexNames.push_back ( "BumpyMetal.jpg"        );
    aTexNames.push_back ( "egyptrockyfull.jpg"    );
    aTexNames.push_back ( "frost.png"             );
    aTexNames.push_back ( "MtlPlat2.jpg"          );
    aTexNames.push_back ( "nskingr.jpg"           );
    aTexNames.push_back ( "Panels_Diffuse.png"    );
    aTexNames.push_back ( "Panels_reflection.png" );
    aTexNames.push_back ( "RustedMetal.jpg"       );
    aTexNames.push_back ( "spacesky.jpg"          );
    aTexNames.push_back ( "terrain_texture.jpg"   );
    aTexNames.push_back ( "texmap2.jpg"           );
    aTexNames.push_back ( "Water01.jpg"           );
    aTexNames.push_back ( "Water02.jpg"           );
    aTexNames.push_back ( "body.jpg"              );
    aTexNames.push_back ( "stone1.jpg"            );
    aTexNames.push_back ( "wall3.jpg"             );
    aTexNames.push_back ( "sinbad_body.tga"       );
    aTexNames.push_back ( "sinbad_clothes.tga"    );
    aTexNames.push_back ( "stevecube_BK.jpg"      );

    // create the 2d texture array (the depth is the size of the array - number of textures)
    TexturePtr  pTex = TextureManager::getSingleton ( ).createManual
    (
        "TextureArrayTex", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        TEX_TYPE_2D_ARRAY, 512, 512, aTexNames.size ( ), 0, PF_X8R8G8B8
    );
    
    // add all the textures to a 2d texture array
    for ( KDuint i = 0; i < aTexNames.size(); i++ )
    {
        Image  tTerrainTex;
        tTerrainTex.load ( aTexNames[i], ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
        
        HardwarePixelBufferSharedPtr  pPixelBufferBuf = pTex->getBuffer ( 0 );
        const PixelBox&  tCurrImage = pPixelBufferBuf->lock
        (
            Box ( 0, 0, i, tTerrainTex.getHeight ( ), tTerrainTex.getHeight ( ), i + 1 ), HardwareBuffer::HBL_DISCARD
        );
        PixelUtil::bulkPixelConversion ( tTerrainTex.getPixelBox ( ), tCurrImage );
        pPixelBufferBuf->unlock ( );
    }
    
    // create material and set the texture unit to our texture
    MaterialManager&  tMatMgr = MaterialManager::getSingleton ( );
    MaterialPtr  pTexArrayMat = tMatMgr.createOrRetrieve ( "Examples/TextureArray", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME ).first;
    pTexArrayMat->compile ( );
    
    Pass*  pPass = pTexArrayMat->getBestTechnique ( )->getPass ( 0 );
    pPass->setLightingEnabled ( false );
    TextureUnitState*  pState = pPass->createTextureUnitState ( );
    pState->setTextureName ( pTex->getName ( ), TEX_TYPE_2D_ARRAY );
    
    // create a plane with float3 tex coord - the third value will be the texture index in our case
    ManualObject*  pTextureArrayObject = m_pSceneMgr->createManualObject ( "TextureAtlasObject" );
    
    // create a quad that uses our material
    KDint  nQuadSize = 100;
    pTextureArrayObject->begin ( pTexArrayMat->getName ( ), RenderOperation::OT_TRIANGLE_LIST );
    
    // triangle 0 of the quad
    pTextureArrayObject->position     ( 0, 0, 0 );
    pTextureArrayObject->textureCoord ( 0, 0, 0 );
    pTextureArrayObject->position     ( nQuadSize, 0, 0 );
    pTextureArrayObject->textureCoord ( 1, 0, 0 );
    pTextureArrayObject->position     ( nQuadSize, nQuadSize, 0 );
    pTextureArrayObject->textureCoord ( 1, 1, aTexNames.size ( ) );
    
    // triangle 1 of the quad
    pTextureArrayObject->position     ( 0, 0, 0 );
    pTextureArrayObject->textureCoord ( 0, 0, 0 );
    pTextureArrayObject->position     ( nQuadSize, nQuadSize, 0 );
    pTextureArrayObject->textureCoord ( 1, 1, aTexNames.size ( ) );
    pTextureArrayObject->position     ( 0, nQuadSize, 0);
    pTextureArrayObject->textureCoord ( 0, 1, aTexNames.size ( ) );
    
    pTextureArrayObject->end ( );
    
    // attach it to a node and position appropriately
    SceneNode*  pNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    pNode->setPosition ( -nQuadSize / 2, -nQuadSize / 2, 0 );
    pNode->attachObject ( pTextureArrayObject );
}

KDvoid Sample_TextureArray::cleanupContent ( KDvoid )
{
    TextureManager::getSingleton ( ).remove ( "TextureArrayTex" );

}

