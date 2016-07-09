/* ------------------------------------------------------------------------------------
 *
 *      File            SamplePlugin.h
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

#ifndef __SamplePlugin_h__
#define __SamplePlugin_h__

#include "Sample.h"

/**
 *  Utility class used to hold a set of samples in an OGRE plugin.
 */ 
class SamplePlugin : public Plugin
{
    public :
    
        SamplePlugin ( const String& sName );
    
        const Ogre::String&  getName ( KDvoid ) const;
    
        KDvoid  install    ( KDvoid );
        KDvoid  uninstall  ( KDvoid );
        KDvoid  initialise ( KDvoid );
        KDvoid  shutdown   ( KDvoid );
    
        /**
         *  Adds a sample to the queue.
         */
        KDvoid  addSample ( Sample* pSample );

    
        /**
         *  Retrieves the queue of samples.
         */
        const SampleSet&  getSamples ( KDvoid );

    protected :
    
        Ogre::String    m_sName;
        SampleSet       m_aSamples;
};

#endif // __SamplePlugin_h__
