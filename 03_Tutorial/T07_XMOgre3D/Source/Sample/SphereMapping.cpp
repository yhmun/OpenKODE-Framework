/* ------------------------------------------------------------------------------------
 *
 *      File            SphereMapping.cpp
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
#include "SphereMapping.h"

Sample_SphereMapping::Sample_SphereMapping ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Sphere Mapping";
    m_aInfo [ "Description" ] = "Shows the sphere mapping feature of materials. "
                                "Sphere maps are not wrapped, and look the same from all directions.";
    m_aInfo [ "Thumbnail"   ] = "thumb_spheremap.png";
    m_aInfo [ "Category"    ] = "Unsorted";
}

KDvoid Sample_SphereMapping::setupContent ( KDvoid )
{
    m_pViewport->setBackgroundColour ( ColourValue::White );
    
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.3, 0.3 ) );
    m_pSceneMgr->createLight ( )->setPosition ( 20, 80, 50 );
    
    // set our camera to orbit around the origin and show cursor
    m_pCameraMan->setStyle ( CS_ORBIT );
    m_pTrayMgr->showCursor ( );
    
    // create our model, give it the environment mapped material, and place it at the origin
    Entity*  pEnt = m_pSceneMgr->createEntity ( "Head", "ogrehead.mesh" );
    pEnt->setMaterialName ( "Examples/SphereMappedRustySteel" );
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( pEnt );
}

