/* --------------------------------------------------------------------------
 *
 *      File            CDOpenALSupport.h
 *      Description     
 *      Author          Y.H Mun
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 *
 *      Contact Email: xmsoft77@gmail.com
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (C) 2009 Apple Inc. All Rights Reserved.
 *
 * --------------------------------------------------------------------------
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 *
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */

/**
 *  This file contains code from version 1.1 and 1.4 of MyOpenALSupport.h taken from Apple's oalTouch version.
 *  The 1.4 version code is used for loading IMA4 files, however, this code causes very noticeable clicking
 *  when used to load wave files that are looped so the 1.1 version code is used specifically for loading
 *  wav files.
 */

#ifndef __CDOpenALSupport_h__
#define __CDOpenALSupport_h__

#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <CoreFoundation/CFURL.h>

// Taken from oalTouch MyOpenALSupport 1.1
void*  CDloadWaveAudioData  ( CFURLRef inFileURL, ALsizei* outDataSize, ALenum* outDataFormat, ALsizei* outSampleRate );
void*  CDloadCafAudioData   ( CFURLRef inFileURL, ALsizei* outDataSize, ALenum* outDataFormat, ALsizei* outSampleRate );
void*  CDGetOpenALAudioData ( CFURLRef inFileURL, ALsizei* outDataSize, ALenum* outDataFormat, ALsizei* outSampleRate );

#endif  // __CDOpenALSupport_h__


