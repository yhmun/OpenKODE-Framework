/* --------------------------------------------------------------------------
 *
 *      File            ios_sound.m
 *      Description     Simple sound function.
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

#include "../kd_library.h"
#import "CDAudioManager.h"

@interface XMAVAudioPlayer : NSObject <AVAudioPlayerDelegate>
{
    
}
@end

@implementation XMAVAudioPlayer

- (void) audioPlayerDidFinishPlaying:(AVAudioPlayer*) player successfully:(BOOL) flag
{
    xmSoundCompleted ( (KDvoid*) player );
}

@end

static CDAudioManager*      l_pAudioManager  = nil;
static CDSoundEngine*       l_pSoundEngine   = nil;
static CDBufferManager*     l_pBufferManager = nil;
static XMAVAudioPlayer*     l_pAVDelegate    = nil;

KDvoid xmiOSSoundInit ( KDvoid )
{
    l_pAudioManager  = [CDAudioManager sharedManager];
    l_pSoundEngine   = l_pAudioManager.soundEngine;
    l_pBufferManager = [[CDBufferManager alloc] initWithEngine:l_pSoundEngine];    
    l_pAVDelegate    = [[XMAVAudioPlayer alloc] init];
}

KDvoid xmiOSSoundFree ( KDvoid )
{
    [CDAudioManager end];
    [l_pBufferManager release];
    [l_pAVDelegate    release];
    
    l_pAudioManager  = nil;
    l_pSoundEngine   = nil;
    l_pBufferManager = nil;
    l_pAVDelegate    = nil;
}

KDint xmPlatformSoundOpen ( XMSound* pSound )
{    
    NSString*  pPath = [NSString stringWithUTF8String:pSound->fpath];
            
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        KDint  nSoundID = [l_pBufferManager bufferForFile:pPath create:YES];
                
        if ( nSoundID != kCDNoBuffer )
        {
            pSound->device = (KDvoid*) nSoundID;
            return 0;
        }                
    }
    else
    {    
        AVAudioPlayer*   pPlayer = KD_NULL;
        NSURL*           pUrl    = [[NSURL alloc] initFileURLWithPath:pPath];
    
        pPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:pUrl error:nil];
        [pUrl release];
        
        if ( pPlayer )
        {
            pPlayer.numberOfLoops = 0;
            pPlayer.delegate = l_pAVDelegate;
        
            if ( [pPlayer prepareToPlay] == NO )
            {
                [pPlayer release];
                pPlayer = KD_NULL;
            }
            
            pSound->device = (KDvoid *) pPlayer;
            return 0;
        }
    }
    
	return -1;
}

KDint xmPlatformSoundClose ( XMSound* pSound )
{        
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        NSString*  pPath = [NSString stringWithUTF8String:pSound->fpath];
        
        [l_pBufferManager releaseBufferForFile:pPath];
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
    
        [pPlayer release];    
    }
    
	return 0;
}

KDint xmPlatformSoundPlay ( XMSound* pSound )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        KDint  nSoundID = (KDint) pSound->device;
        [l_pSoundEngine playSound:nSoundID sourceGroupId:0 pitch:1.0f pan:0.0f gain:1.0f loop:pSound->repeat ? NO : NO];
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        [pPlayer play];
    }

	return 0;
}

KDint xmPlatformSoundStop ( XMSound* pSound )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        KDint  nSoundID = (KDint) pSound->device;
        [l_pSoundEngine stopSound:nSoundID];
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        [pPlayer stop];
        [pPlayer setCurrentTime:0.];
    }
    
	return 0;
}

KDint xmPlatformSoundPause ( XMSound* pSound )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        KDint  nSoundID = (KDint) pSound->device;
        [l_pSoundEngine pauseSound:nSoundID];
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        [pPlayer pause];
    }
    
	return 0;
}

KDint xmPlatformSoundResume ( XMSound* pSound )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        KDint  nSoundID = (KDint) pSound->device;
        [l_pSoundEngine resumeSound:nSoundID];
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        [pPlayer play];
    }

	return 0;
}

KDint xmPlatformSoundSetVolume ( XMSound* pSound, KDfloat volume )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        KDint  nSoundID = (KDint) pSound->device;
        alSourcef ( nSoundID, AL_GAIN, volume );
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        [pPlayer setVolume:volume];
    }

    return 0;
}

KDint xmPlatformSoundSetPitch ( XMSound* pSound, KDfloat pitch )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        KDint  nSoundID = (KDint) pSound->device;
        alSourcef ( nSoundID, AL_PITCH, pitch );
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        [pPlayer setRate:pitch];
    }
    
    return 0;
}

KDint xmPlatformSoundSetPan ( XMSound* pSound, KDfloat pan )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        KDint  nSoundID = (KDint) pSound->device;
        ALfloat alPos[] = { pan, 0.0f, 0.0f };
        
        alSourcefv ( nSoundID, AL_POSITION, alPos );
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        [pPlayer setPan:pan];
    }
    
    return 0;
}

KDint xmPlatformSoundGetDuration ( XMSound* pSound )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        return -1;
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        return (KDint) ( pPlayer.duration * 1000 );
    }
}

KDint xmPlatformSoundGetPosition ( XMSound* pSound )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        return -1;
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        return (KDint) ( pPlayer.currentTime * 1000 );
    }
}

KDint xmPlatformSoundSetPosition ( XMSound* pSound, KDint nPos )
{
    if ( pSound->mode & XM_SOUND_EFFECT )
    {
        
    }
    else
    {
        AVAudioPlayer*  pPlayer = (AVAudioPlayer*) pSound->device;
        
        [pPlayer playAtTime:(nPos / 1000.0)];
    }

    return 0;
}

KDvoid xmPlatformVibrate ( KDuint msec )
{
    AudioServicesPlaySystemSound ( kSystemSoundID_Vibrate );
}

