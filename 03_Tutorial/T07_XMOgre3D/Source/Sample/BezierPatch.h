/* ------------------------------------------------------------------------------------
 *
 *      File            BezierPatch.h
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

#ifndef __BezierPatch_h__
#define __BezierPatch_h__

#include "../SdkSample.h"

class Sample_BezierPatch : public SdkSample
{
    public :
    
        struct PatchVertex
        {
            KDfloat  x, y, z;
            KDfloat  nx, ny, nz;
            KDfloat  u, v;
        };
    
    public :
    
        Sample_BezierPatch ( KDvoid );
    
        KDvoid  checkBoxToggled ( CheckBox* pBox );
    
        KDvoid  sliderMoved ( Slider* pSlider );
    
    protected :
    
        KDvoid  setupContent ( KDvoid );
    
        KDvoid  cleanupContent ( KDvoid );
    
    protected :
    
        VertexDeclaration*  m_pDecl;
        PatchMeshPtr        m_pPatch;
        Pass*               m_pPatchPass;
};

#endif // __BezierPatch_h__
