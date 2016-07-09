/* ------------------------------------------------------------------------------------
 *
 *      File            NewInstancing.cpp
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
#include "NewInstancing.h"

Sample_NewInstancing::Sample_NewInstancing ( KDvoid )
{
    m_aInfo [ "Title"       ] = "New Instancing";
    m_aInfo [ "Description" ] = "Demonstrates how to use the new InstancedManager to setup many dynamic"
                                " instances of the same mesh with much less performance impact";
    m_aInfo [ "Thumbnail"   ] = "thumb_newinstancing.png";
    m_aInfo [ "Category"    ] = "Environment";
    m_aInfo [ "Help"        ] = "Press Space to switch Instancing Techniques.\n"
                                "Press B to toggle bounding boxes.\n\n"
                                "Changes in the slider take effect after switching instancing technique\n"
                                "Different batch sizes give different results depending on CPU culling"
                                " and instance numbers on the scene.\n\n"
                                "If performance is too slow, try defragmenting batches once in a while";
}

KDvoid Sample_NewInstancing::setupContent ( KDvoid )
{
 
}

