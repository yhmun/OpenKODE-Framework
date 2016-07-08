/* --------------------------------------------------------------------------
 *
 *      File            CDConfig.h
 *      Description
 *      Author          Y.H Mun
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 *
 *      Contact Email: xmsoft77@gmail.com
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (c) 2010 Steve Oldmeadow
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

#ifndef __CDConfig_h__
#define __CDConfig_h__

#define COCOSDENSHION_VERSION "Aphex.rc"


/**
 *  If enabled code useful for debugging such as parameter check assertions will be performed.
 *  If you experience any problems you should enable this and test your code with a debug build.
 */
#define CD_DEBUG                0

/**
 *  The total number of sounds/buffers that can be loaded assuming memory is sufficient
 */
// Number of buffers slots that will be initially created
#define CD_BUFFERS_START        64
// Number of buffers that will be added
#define CD_BUFFERS_INCREMENT    16

/**
 *  If enabled, OpenAL code will use static buffers. When static buffers are used the audio
 *  data is managed outside of OpenAL, this eliminates a memcpy operation which leads to
 *  higher performance when loading sounds.
 *
 *  However, the downside is that when the audio data is freed you must
 *  be certain that it is no longer being accessed otherwise your app will crash. Testing on OS 2.2.1
 *  and 3.1.2 has shown that this may occur if a buffer is being used by a source with state = AL_PLAYING
 *  when the buffer is deleted. If the data is freed too quickly after the source is stopped then
 *  a crash will occur. The implemented workaround is that when static buffers are used the unloadBuffer code will wait for
 *  any playing sources to finish playing before the associated buffer and data are deleted, however, this delay may negate any
 *  performance gains that are achieved during loading.
 *
 *  Performance tests on a 1st gen iPod running OS 2.2.1 loading the CocosDenshionDemo sounds were ~0.14 seconds without
 *  static buffers and ~0.12 seconds when using static buffers.
 *
 */
#define CD_USE_STATIC_BUFFERS   0


#endif  // __CDConfig_h__

