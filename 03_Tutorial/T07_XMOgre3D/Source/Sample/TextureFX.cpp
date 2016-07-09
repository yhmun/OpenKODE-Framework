/* ------------------------------------------------------------------------------------
 *
 *      File            TextureFX.cpp
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
#include "TextureFX.h"

Sample_TextureFX::Sample_TextureFX ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Texture Effects";
    m_aInfo [ "Description" ] = "Demonstrates OGRE's many different animated texture effects.";
    m_aInfo [ "Thumbnail"   ] = "thumb_texfx.png";
    m_aInfo [ "Category"    ] = "Unsorted";
    m_aInfo [ "Help"        ] = "Top Left: Multi-frame\nTop Right: Scrolling\nBottom Left: Rotation\nBottom Right: Scaling";
}

KDvoid Sample_TextureFX::setupContent ( KDvoid )
{
    m_pSceneMgr->setSkyBox ( true, "Examples/TrippySkyBox" );
    
    // set our camera to orbit around the origin and show cursor
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pTrayMgr->showCursor ( );
    
    // the names of the four materials we will use
    String  aMatNames[] = { "Examples/OgreDance", "Examples/OgreParade", "Examples/OgreSpin", "Examples/OgreWobble" };
    
    for ( KDuint i = 0; i < 4; i++ )
    {
        // create a standard plane entity
        Entity*  pEntity = m_pSceneMgr->createEntity ( "Plane" + StringConverter::toString ( i + 1 ), SceneManager::PT_PLANE );
        
        // attach it to a node, scale it, and position appropriately
        SceneNode*  pNode = m_pSceneMgr->getRootSceneNode ( )->createChildSceneNode ( );
        pNode->setPosition ( i % 2 ? 25 : -25, i / 2 ? -25 : 25, 0 );
        pNode->setScale ( 0.25, 0.25, 0.25 );
        pNode->attachObject ( pEntity );
        
        pEntity->setMaterialName ( aMatNames[i] );  // give it the material we prepared
    }
}

