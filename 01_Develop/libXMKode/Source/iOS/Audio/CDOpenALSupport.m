/* --------------------------------------------------------------------------
 *
 *      File            CDOpenALSupport.m
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

#import "CDOpenALSupport.h"
#import "CocosDenshion.h"
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>

// Taken from oalTouch MyOpenALSupport 1.1
void* CDloadWaveAudioData ( CFURLRef inFileURL, ALsizei* outDataSize, ALenum* outDataFormat, ALsizei* outSampleRate )
{
    OSStatus                        err = noErr;    
    UInt64                          fileDataSize = 0;
    AudioStreamBasicDescription     theFileFormat;
    UInt32                          thePropertySize = sizeof ( theFileFormat );
    AudioFileID                     afid = 0;
    void*                           theData = NULL;
    
    // Open a file with ExtAudioFileOpen()
    err = AudioFileOpenURL ( inFileURL, kAudioFileReadPermission, 0, &afid );
    if ( err )
    {
        CDLOG ( @"MyGetOpenALAudioData: AudioFileOpenURL FAILED, Error = %ld\n", err );
        goto Exit;
    }
    
    // Get the audio data format
    err = AudioFileGetProperty ( afid, kAudioFilePropertyDataFormat, &thePropertySize, &theFileFormat );
    if ( err )
    {
        CDLOG ( @"MyGetOpenALAudioData: AudioFileGetProperty(kAudioFileProperty_DataFormat) FAILED, Error = %ld\n", err );
        goto Exit;
    }
    
    if ( theFileFormat.mChannelsPerFrame > 2 )
    {
        CDLOG ( @"MyGetOpenALAudioData - Unsupported Format, channel count is greater than stereo\n" );
        goto Exit;
    }
    
    if ( ( theFileFormat.mFormatID != kAudioFormatLinearPCM ) || ( !TestAudioFormatNativeEndian ( theFileFormat ) ) )
    {
        CDLOG ( @"MyGetOpenALAudioData - Unsupported Format, must be little-endian PCM\n" );
        goto Exit;
    }
    
    if ( ( theFileFormat.mBitsPerChannel != 8 ) && ( theFileFormat.mBitsPerChannel != 16 ) )
    {
        CDLOG ( @"MyGetOpenALAudioData - Unsupported Format, must be 8 or 16 bit PCM\n" );
        goto Exit;
    }
        
    thePropertySize = sizeof ( fileDataSize );
    err = AudioFileGetProperty ( afid, kAudioFilePropertyAudioDataByteCount, &thePropertySize, &fileDataSize );
    if ( err )
    {
        CDLOG ( @"MyGetOpenALAudioData: AudioFileGetProperty(kAudioFilePropertyAudioDataByteCount) FAILED, Error = %ld\n", err );
        goto Exit;
    }
    
    // Read all the data into memory
    UInt32  dataSize = (UInt32) fileDataSize;
    theData = malloc ( dataSize );
    if ( theData )
    {
        memset ( theData, 0, dataSize );
        AudioFileReadBytes ( afid, false, 0, &dataSize, theData );
        if ( err == noErr )
        {
            // success
            *outDataSize = (ALsizei) dataSize;
            
            //This fix was added by me, however, 8 bit sounds have a clipping sound at the end so aren't really usable (SO)
            if ( theFileFormat.mBitsPerChannel == 16 )
            {
                *outDataFormat = ( theFileFormat.mChannelsPerFrame > 1 ) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
            }
            else
            {
                *outDataFormat = ( theFileFormat.mChannelsPerFrame > 1 ) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;    
            }    
            *outSampleRate = (ALsizei) theFileFormat.mSampleRate;
        }
        else 
        { 
            // failure
            free ( theData );
            theData = NULL; // make sure to return NULL
            CDLOG ( @"MyGetOpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", err );
            goto Exit;
        }    
    }
    
Exit :
    
    // Dispose the ExtAudioFileRef, it is no longer needed
    if ( afid )
    {
        AudioFileClose ( afid );
    }
    
    return theData;
}
 
// Taken from oalTouch MyOpenALSupport 1.4
void* CDloadCafAudioData ( CFURLRef inFileURL, ALsizei* outDataSize, ALenum* outDataFormat, ALsizei* outSampleRate )
{
    OSStatus                        status = noErr;
    BOOL                            abort  = NO;
    SInt64                          theFileLengthInFrames = 0;
    AudioStreamBasicDescription     theFileFormat;
    UInt32                          thePropertySize = sizeof ( theFileFormat );
    ExtAudioFileRef                 extRef  = NULL;
    void*                           theData = NULL;
    AudioStreamBasicDescription     theOutputFormat;
    UInt32                          dataSize = 0;
    
    // Open a file with ExtAudioFileOpen()
    status = ExtAudioFileOpenURL ( inFileURL, &extRef );
    if ( status != noErr )
    {
        CDLOG ( @"MyGetOpenALAudioData: ExtAudioFileOpenURL FAILED, Error = %ld\n", status );
        abort = YES;
    }
    
    if ( abort )
    {
        goto Exit;
    }
    
    // Get the audio data format
    status = ExtAudioFileGetProperty ( extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat );
    if ( status != noErr )
    {
        CDLOG ( @"MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ld\n", status );
        abort = YES;
    }
    
    if ( abort )
    {
        goto Exit;
    }
    
    if ( theFileFormat.mChannelsPerFrame > 2 )
    {
        CDLOG ( @"MyGetOpenALAudioData - Unsupported Format, channel count is greater than stereo\n" );
        abort = YES;
    }
    
    if ( abort )
    {
        goto Exit;
    }
    
    // Set the client format to 16 bit signed integer (native-endian) data
    // Maintain the channel count and sample rate of the original source format
    theOutputFormat.mSampleRate         = theFileFormat.mSampleRate;
    theOutputFormat.mChannelsPerFrame   = theFileFormat.mChannelsPerFrame;
    
    theOutputFormat.mFormatID           = kAudioFormatLinearPCM;
    theOutputFormat.mBytesPerPacket     = 2 * theOutputFormat.mChannelsPerFrame;
    theOutputFormat.mFramesPerPacket    = 1;
    theOutputFormat.mBytesPerFrame      = 2 * theOutputFormat.mChannelsPerFrame;
    theOutputFormat.mBitsPerChannel     = 16;
    theOutputFormat.mFormatFlags        = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
    
    // Set the desired client (output) data format
    status = ExtAudioFileSetProperty ( extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat );
    if ( status != noErr )
    {
        CDLOG ( @"MyGetOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %ld\n", status );
        abort = YES;
    }
    
    if ( abort )
    {
        goto Exit;
    }
    
    // Get the total frame count
    thePropertySize = sizeof ( theFileLengthInFrames );
    status = ExtAudioFileGetProperty ( extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames );
    if ( status != noErr )
    {
        CDLOG ( @"MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %ld\n", status );
        abort = YES;
    }
    
    if ( abort )
    {
        goto Exit;
    }
    
    // Read all the data into memory
    dataSize = (UInt32) theFileLengthInFrames * theOutputFormat.mBytesPerFrame;
    theData = malloc ( dataSize );
    if ( theData )
    {
        memset ( theData, 0, dataSize );
        AudioBufferList     theDataBuffer;
        
        theDataBuffer.mNumberBuffers                = 1;
        theDataBuffer.mBuffers[0].mDataByteSize     = dataSize;
        theDataBuffer.mBuffers[0].mNumberChannels   = theOutputFormat.mChannelsPerFrame;
        theDataBuffer.mBuffers[0].mData             = theData;
        
        // Read the data into an AudioBufferList
        status = ExtAudioFileRead ( extRef, (UInt32*) &theFileLengthInFrames, &theDataBuffer );
        if ( status == noErr )
        {
            // success
            *outDataSize    = (ALsizei) dataSize;
            *outDataFormat  = ( theOutputFormat.mChannelsPerFrame > 1 ) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
            *outSampleRate  = (ALsizei) theOutputFormat.mSampleRate;
        }
        else
        {
            // failure
            free ( theData );
            theData = NULL; // make sure to return NULL
            CDLOG ( @"MyGetOpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", status );
            abort = YES;
        }
    }
    
    if ( abort )
    {
        goto Exit;
    }
    
Exit :
    // Dispose the ExtAudioFileRef, it is no longer needed
    if ( extRef )
    {
        ExtAudioFileDispose ( extRef );
    }
    
    return theData;
}

void* CDGetOpenALAudioData ( CFURLRef inFileURL, ALsizei* outDataSize, ALenum* outDataFormat, ALsizei* outSampleRate )
{    
    CFStringRef         extension = CFURLCopyPathExtension ( inFileURL );
    CFComparisonResult  isWavFile = 0;
    
    if ( extension != NULL )
    {
        isWavFile = CFStringCompare ( extension, (CFStringRef) @"wav", kCFCompareCaseInsensitive );
        CFRelease ( extension );
    }    
    
    if ( isWavFile == kCFCompareEqualTo )
    {
        return CDloadWaveAudioData ( inFileURL, outDataSize, outDataFormat, outSampleRate );    
    }
    else
    {
        return CDloadCafAudioData ( inFileURL, outDataSize, outDataFormat, outSampleRate );        
    }
}

