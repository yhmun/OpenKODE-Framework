/* ------------------------------------------------------------------------------------
 *
 *      File            DynTex.cpp
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
#include "DynTex.h"

Sample_DynTex::Sample_DynTex ( KDvoid )
: TEXTURE_SIZE ( 128 )
, SQR_BRUSH_RADIUS ( Math::Sqr ( 12 ) )
{
    m_aInfo [ "Title"       ] = "Dynamic Texturing";
    m_aInfo [ "Description" ] = "Demonstrates how to create and use dynamically changing textures.";
    m_aInfo [ "Thumbnail"   ] = "thumb_dyntex.png";
    m_aInfo [ "Category"    ] = "Unsorted";
    m_aInfo [ "Help"        ] = "Use the left mouse button to wipe away the frost. "
                                "It's cold though, so the frost will return after a while.";
}

bool Sample_DynTex::frameRenderingQueued ( const FrameEvent& tEvent )
{
    // shoot a ray from the cursor to the plane
    Ray  tRay = m_pTrayMgr->getCursorRay ( m_pCamera );
    m_pCursorQuery->setRay ( tRay );
    RaySceneQueryResult&  tResult = m_pCursorQuery->execute ( );
    
    if ( !tResult.empty ( ) )
    {
        // using the point of intersection, find the corresponding texel on our texture
        Vector3  tPt = tRay.getPoint ( tResult.back ( ).distance );
        m_tBrushPos = ( Vector2 ( tPt.x, -tPt.y ) / m_fPlaneSize + Vector2 ( 0.5, 0.5 ) ) * TEXTURE_SIZE;
    }
    
    uint8  cFreezeAmount = 0;
    m_fTimeSinceLastFreeze += tEvent.timeSinceLastFrame;
    
    // find out how much to freeze the plane based on time passed
    while ( m_fTimeSinceLastFreeze >= 0.1 )
    {
        m_fTimeSinceLastFreeze -= 0.1;
        cFreezeAmount += 0x04;
    }
    
    updateTexture ( cFreezeAmount );  // rebuild texture contents
    
    m_pPenguinAnimState->addTime ( tEvent.timeSinceLastFrame );     // increment penguin idle animation time
    m_pPenguinNode->yaw ( Radian ( tEvent.timeSinceLastFrame ) );   // spin the penguin around
    
    return SdkSample::frameRenderingQueued ( tEvent );  // don't forget the parent class updates!
}

bool Sample_DynTex::touchPressed ( const KDEventTouches& tTouches )
{
    if ( m_pTrayMgr->injectTouchPressed ( tTouches ) )
    {
        return true;
    }
    m_bWiping = true;   // wipe frost if user left clicks in the scene
    return true;
}

bool Sample_DynTex::touchReleased ( const KDEventTouches& tTouches )
{
    if ( m_pTrayMgr->injectTouchReleased ( tTouches ) )
    {
        return true;
    }
    m_bWiping = false;  // stop wiping frost if user releases LMB
    return true;
}

KDvoid Sample_DynTex::setupContent ( KDvoid )
{
    m_pSceneMgr->setSkyBox ( true, "Examples/StormySkyBox" );  // add a skybox
    
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.5, 0.5, 0.5 ) );
    m_pSceneMgr->createLight ( )->setPosition ( 20, 80, 50 );
    
    // set initial camera position
    m_pCameraMan->setStyle ( CS_MANUAL );
    m_pCamera->setPosition ( 0, 0, 200 );
    
    m_pTrayMgr->showCursor ( );
    
    // create our dynamic texture with 8-bit luminance texels
    TexturePtr  pTex = TextureManager::getSingleton ( ).createManual
    (
        "thaw", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        TEX_TYPE_2D, TEXTURE_SIZE, TEXTURE_SIZE, 0, PF_L8, TU_DYNAMIC_WRITE_ONLY
    );
    
    m_pTexBuf = pTex->getBuffer ( );  // save off the texture buffer
    
    // initialise the texture to have full luminance
    m_pConstantTexBuf = new uint8 [ m_pTexBuf->getSizeInBytes ( ) ];
    kdMemset ( m_pConstantTexBuf, 0xff, m_pTexBuf->getSizeInBytes ( ) );
    kdMemcpy ( m_pTexBuf->lock ( HardwareBuffer::HBL_DISCARD ), m_pConstantTexBuf, TEXTURE_SIZE * TEXTURE_SIZE );
    m_pTexBuf->unlock ( );
    
    // create a penguin and attach him to our penguin node
    Entity*  pPenguin = m_pSceneMgr->createEntity ( "Penguin", "penguin.mesh" );
    m_pPenguinNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    m_pPenguinNode->attachObject ( pPenguin );
    
    // get and enable the penguin idle animation
    m_pPenguinAnimState = pPenguin->getAnimationState ( "amuse" );
    m_pPenguinAnimState->setEnabled ( true );
    
    // create a snowstorm over the scene, and fast forward it a little
    ParticleSystem*  pPaticle = m_pSceneMgr->createParticleSystem ( "Snow", "Examples/Snow" );
    m_pSceneMgr->getRootSceneNode()->attachObject ( pPaticle );
    pPaticle->fastForward ( 30 );
    
    // create a frosted screen in front of the camera, using our dynamic texture to "thaw" certain areas
    Entity*  pEntity = m_pSceneMgr->createEntity ( "Plane", SceneManager::PT_PLANE );
    pEntity->setMaterialName ( "Examples/Frost" );
    SceneNode*  pNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
    pNode->setPosition ( 0, 0, 50 );
    pNode->attachObject ( pEntity );
    
    m_fPlaneSize = pEntity->getBoundingBox ( ).getSize ( ).x;   // remember the size of the plane
    
    m_pCursorQuery = m_pSceneMgr->createRayQuery ( Ray ( ) );   // create a ray scene query for the cursor
    
    m_fTimeSinceLastFreeze = 0;
    m_bWiping = false;
}

KDvoid Sample_DynTex::updateTexture ( uint8 cFreezeAmount )
{
    // get access to raw texel data
    uint8*  pData = m_pConstantTexBuf;
    
    uint8   cTemperature;
    Real    fSqrDistToBrush;
    
    // go through every texel...
    for ( KDuint y = 0; y < TEXTURE_SIZE; y++ )
    {
        for ( KDuint x = 0; x < TEXTURE_SIZE; x++ )
        {
            if ( cFreezeAmount != 0 )
            {
                // gradually refreeze anything that isn't completely frozen
                cTemperature = 0xff - *pData;
                if ( cTemperature > cFreezeAmount )
                {
                    *pData += cFreezeAmount;
                }
                else
                {
                    *pData = 0xff;
                }
            }
            
            if ( m_bWiping )
            {
                // wipe frost from under the cursor
                fSqrDistToBrush = Math::Sqr ( x - m_tBrushPos.x ) + Math::Sqr ( y - m_tBrushPos.y );
                if ( fSqrDistToBrush <= SQR_BRUSH_RADIUS )
                {
                    *pData = std::min<uint8> ( fSqrDistToBrush / SQR_BRUSH_RADIUS * 0xff, *pData );
                }
            }
            
            pData++;
        }
    }
    
    kdMemcpy ( m_pTexBuf->lock ( HardwareBuffer::HBL_DISCARD ), m_pConstantTexBuf, TEXTURE_SIZE * TEXTURE_SIZE );
    m_pTexBuf->unlock ( );
}

KDvoid Sample_DynTex::cleanupContent ( KDvoid )
{
    delete [] m_pConstantTexBuf;
    TextureManager::getSingleton ( ).remove ( "thaw" );
    m_pSceneMgr->destroyQuery ( m_pCursorQuery );
}