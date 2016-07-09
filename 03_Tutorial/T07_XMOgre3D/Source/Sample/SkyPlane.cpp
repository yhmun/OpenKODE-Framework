/* ------------------------------------------------------------------------------------
 *
 *      File            SkyPlane.cpp
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
#include "SkyPlane.h"

Sample_SkyPlane::Sample_SkyPlane ( KDvoid )
{
    m_aInfo [ "Title"       ] = "Sky Plane";
    m_aInfo [ "Description" ] = "Shows how to use skyplanes (fixed-distance planes used for backgrounds).";
    m_aInfo [ "Thumbnail"   ] = "thumb_skyplane.png";
    m_aInfo [ "Category"    ] = "Environment";
}

KDvoid Sample_SkyPlane::setupContent ( KDvoid )
{
    // setup some basic lighting for our scene
    m_pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.3, 0.3 ) );
    m_pSceneMgr->createLight ( )->setPosition ( 20, 80, 50 );
    
    // create a skyplane 5000 units away, facing down, 10000 square units large, with 3x texture tiling
    m_pSceneMgr->setSkyPlane ( true, Plane ( 0, -1, 0, 5000 ), "Examples/SpaceSkyPlane", 10000, 3 );
    
    // and finally... omg it's a DRAGON!
    m_pSceneMgr->getRootSceneNode ( )->attachObject ( m_pSceneMgr->createEntity ( "Dragon", "dragon.mesh" ) );
    
    // turn around and look at the DRAGON!
    m_pCamera->yaw   ( Degree ( 210 ) );
    m_pCamera->pitch ( Degree ( -10 ) );
}

