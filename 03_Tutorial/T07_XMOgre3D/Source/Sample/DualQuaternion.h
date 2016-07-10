/* ------------------------------------------------------------------------------------
 *
 *      File            DualQuaternion.h
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

#ifndef __DualQuaternion_h__
#define __DualQuaternion_h__

#include "../SdkSample.h"

class Sample_DualQuaternion : public SdkSample
{
    public :
    
        Sample_DualQuaternion ( KDvoid );
    
        bool    frameRenderingQueued ( const FrameEvent& tEvent );
    
    protected :
    
        KDvoid  setupContent ( KDvoid );
    
        KDvoid  setupModels ( KDvoid );
    
        KDvoid  cleanupContent ( KDvoid );
    
    protected :
    
        Entity*     m_pEntity;
        Entity*     m_pEntityDQ;
    
        Real        m_fTotalTime;
    
    #ifdef RTSHADER_SYSTEM_BUILD_EXT_SHADERS
        RTShader::SubRenderState*   m_pSrsHardwareSkinning;
    #endif
};

#endif // __DualQuaternion_h__