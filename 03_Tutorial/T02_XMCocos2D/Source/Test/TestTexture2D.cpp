/* --------------------------------------------------------------------------
 *
 *      File            TestTexture2D.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org. All rights reserved.
 *
 *         http://www.cocos2d-x.org      
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

#include "Precompiled.h"
#include "TestTexture2D.h"

TestTexture2D* TestTexture2D::create ( KDvoid )
{
	CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_Default );

	TestTexture2D*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 :	pLayer = new TextureMemoryAlloc		( ); break;
		case  1 :	pLayer = new TextureAlias			( ); break;
		case  2 :	pLayer = new TexturePVRMipMap		( ); break;
		case  3 :	pLayer = new TexturePVRMipMap2		( ); break;
		case  4 :	pLayer = new TexturePVRNonSquare	( ); break;
		case  5 :	pLayer = new TexturePVRNPOT4444		( ); break;
		case  6 :	pLayer = new TexturePVRNPOT8888		( ); break;
		case  7 :	pLayer = new TexturePVR				( ); break;
		case  8 :	pLayer = new TexturePVR2BPP			( ); break;
		case  9 :	pLayer = new TexturePVR2BPPv3		( ); break;
		case 10 :	pLayer = new TexturePVR4BPP			( ); break;
		case 11 :	pLayer = new TexturePVR4BPPv3		( ); break;
		case 12 :	pLayer = new TexturePVRII4BPPv3		( ); break;
		case 13 :	pLayer = new TexturePVRRGBA8888		( ); break;
		case 14 :	pLayer = new TexturePVRRGBA8888v3	( ); break;
		case 15 :	pLayer = new TexturePVRBGRA8888		( ); break;
		case 16 :	pLayer = new TexturePVRBGRA8888v3	( ); break;
		case 17 :	pLayer = new TexturePVRRGBA4444		( ); break;
		case 18 :	pLayer = new TexturePVRRGBA4444v3	( ); break;
		case 19 :	pLayer = new TexturePVRRGBA4444GZ	( ); break;
		case 20 :	pLayer = new TexturePVRRGBA4444CCZ	( ); break;
		case 21 :	pLayer = new TexturePVRRGBA5551		( ); break;
		case 22 :	pLayer = new TexturePVRRGBA5551v3	( ); break;
		case 23 :	pLayer = new TexturePVRRGB565		( ); break;
		case 24 :	pLayer = new TexturePVRRGB565v3		( ); break;
		case 25 :	pLayer = new TexturePVRRGB888		( ); break;
		case 26 :	pLayer = new TexturePVRRGB888v3		( ); break;
		case 27 :	pLayer = new TexturePVRA8			( ); break;
		case 28 :	pLayer = new TexturePVRA8v3			( ); break;
		case 29 :	pLayer = new TexturePVRI8			( ); break;
		case 30 :	pLayer = new TexturePVRI8v3			( ); break;
		case 31 :	pLayer = new TexturePVRAI88			( ); break;
		case 32 :	pLayer = new TexturePVRAI88v3		( ); break;
		case 33 :	pLayer = new TexturePVRv3Premult	( ); break;
		case 34 :	pLayer = new TexturePVRBadEncoding	( ); break;			
		case 35 :	pLayer = new TexturePNG				( ); break;
		case 36 :	pLayer = new TextureJPEG			( ); break;
		case 37 :	pLayer = new TextureTIFF			( ); break;
		case 38 :	pLayer = new TextureWEBP			( ); break;
		case 39 :	pLayer = new TexturePixelFormat		( ); break;
		case 40 :	pLayer = new TextureBlend			( ); break;
		case 41 :	pLayer = new TextureAsync			( ); break;
		case 42 :	pLayer = new TextureGlClamp			( ); break;
		case 43 :	pLayer = new TextureGlRepeat		( ); break;
		case 44 :	pLayer = new TextureSizeTest		( ); break;
		case 45 :	pLayer = new TextureCache1			( ); break;
		case 46 :	pLayer = new TextureDrawAtPoint		( ); break;
		case 47 :	pLayer = new TextureDrawInRect		( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestTexture2D::count ( KDvoid )
{
	return 48;
}

enum 
{
    kTagLabel   = 1,
    kTagSprite1 = 2,
    kTagSprite2 = 3,
};

//------------------------------------------------------------------
//
// TextureTIFF
//
//------------------------------------------------------------------

KDvoid TextureTIFF::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    CCNode*  pNode = CCSprite::create ( "Images/test_image.tiff" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode );
}

const KDchar* TextureTIFF::title ( KDvoid )
{
    return "TIFF Test";
}

//------------------------------------------------------------------
//
// TexturePNG
//
//------------------------------------------------------------------
KDvoid TexturePNG::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );    

    CCNode*  pNode = CCSprite::create ( "Images/test_image.png" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode );
}

const KDchar* TexturePNG::title ( KDvoid )
{
    return "PNG Test";
}

//------------------------------------------------------------------
//
// TextureJPEG
//
//------------------------------------------------------------------
void TextureJPEG::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    CCNode*  pNode = CCSprite::create ( "Images/test_image.jpeg" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode );   
}

const KDchar* TextureJPEG::title ( KDvoid )
{
    return "JPEG Test";
}

//------------------------------------------------------------------
//
// TextureWEBP
//
//------------------------------------------------------------------
void TextureWEBP::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    CCNode*  pNode = CCSprite::create ( "Images/test_image.webp" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode );   
}

const KDchar* TextureWEBP::title ( KDvoid )
{
    return "WEBP Test";
}

//------------------------------------------------------------------
//
// TextureMipMap
//
//------------------------------------------------------------------
KDvoid TextureMipMap::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    CCTexture2D *texture0 = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/grossini_dance_atlas.png" );
    texture0->generateMipmap ( );
    ccTexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };    
    texture0->setTexParameters ( texParams );

    CCTexture2D *texture1 = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/grossini_dance_atlas_nomipmap.png" );

    CCSprite*  img0 = CCSprite::createWithTexture ( texture0 );
    img0->setTextureRect ( CCRectMake ( 85, 121, 85, 121 ) );
    img0->setPosition ( ccp ( s.cx/3.0f, s.cy / 2.0f ) );
    addChild ( img0 );

    CCSprite*  img1 = CCSprite::createWithTexture ( texture1 );
    img1->setTextureRect ( CCRectMake ( 85, 121, 85, 121 ) );
    img1->setPosition ( ccp ( 2*s.cx/3.0f, s.cy / 2.0f ) );
    addChild ( img1 );
    
    
    CCEaseOut* scale1 = CCEaseOut::create ( CCScaleBy::create ( 4, 0.01f ), 3 );
    CCActionInterval* sc_back = scale1->reverse ( );
    
    CCEaseOut* scale2 = ( CCEaseOut* ) ( scale1->copy ( ) );
    scale2->autorelease ( );
    CCActionInterval* sc_back2 = scale2->reverse ( );

    img0->runAction ( CCRepeatForever::create ( CCSequence::create ( scale1, sc_back , KD_NULL ) ) );
    img1->runAction ( CCRepeatForever::create ( CCSequence::create ( scale2, sc_back2, KD_NULL ) ) );
    CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
}

const KDchar* TextureMipMap::title ( KDvoid )
{
    return "Texture Mipmap";
}

const KDchar* TextureMipMap::subtitle ( KDvoid )
{
    return "Left image uses mipmap. Right image doesn't";
}

//------------------------------------------------------------------
//
// TexturePVRMipMap
// To generate PVR images read this article:
// http://developer.apple.com/iphone/library/qa/qa2008/qa1611.html
//
//------------------------------------------------------------------
void TexturePVRMipMap::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    CCSprite*  imgMipMap = CCSprite::create ( "Images/logo-mipmap.pvr" );
    if ( imgMipMap )
    {
        imgMipMap->setPosition ( ccp ( s.cx / 2.0f-100, s.cy / 2.0f ) );
        addChild ( imgMipMap );

        // support mipmap filtering
        ccTexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };    
        imgMipMap->getTexture ( )->setTexParameters ( texParams );
    }

    CCSprite*  pImage = CCSprite::create ( "Images/logo-nomipmap.pvr" );
    if ( pImage )
    {
        pImage->setPosition ( ccp ( s.cx / 2.0f+100, s.cy / 2.0f ) );
        addChild ( pImage );

        CCEaseOut* scale1 = CCEaseOut::create ( CCScaleBy::create ( 4, 0.01f ), 3 );
        CCActionInterval* sc_back = scale1->reverse ( );

        CCEaseOut* scale2 = ( CCEaseOut* ) ( scale1->copy ( ) );
        scale2->autorelease ( );
        CCActionInterval* sc_back2 = scale2->reverse ( );
        
        imgMipMap->runAction ( CCRepeatForever::create ( CCSequence::create ( scale1, sc_back, KD_NULL ) ) );
           pImage->runAction ( CCRepeatForever::create ( CCSequence::create ( scale2, sc_back2, KD_NULL ) ) );
    }
    CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
}

const KDchar* TexturePVRMipMap::title ( KDvoid )
{
    return "PVRTC MipMap Test";
}
const KDchar* TexturePVRMipMap::subtitle ( KDvoid )
{
    return "Left image uses mipmap. Right image doesn't";
}

//------------------------------------------------------------------
//
// TexturePVRMipMap2
//
//------------------------------------------------------------------
KDvoid TexturePVRMipMap2::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    const CCSize&  s = this->getContentSize ( );
    
    CCSprite*  imgMipMap = CCSprite::create ( "Images/test_image_rgba4444_mipmap.pvr" );
    imgMipMap->setPosition ( ccp ( s.cx / 2.0f - 100, s.cy / 2.0f ) );
    addChild ( imgMipMap );
    
    // support mipmap filtering
    ccTexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };    
    imgMipMap->getTexture ( )->setTexParameters ( texParams );

    CCSprite*  pImage = CCSprite::create ( "Images/test_image.png" );
    pImage->setPosition ( ccp ( s.cx / 2.0f + 100, s.cy / 2.0f ) );
    addChild ( pImage );
    
    CCEaseOut* scale1 = CCEaseOut::create ( CCScaleBy::create ( 4, 0.01f ), 3 );
    CCActionInterval* sc_back = scale1->reverse ( );

    CCEaseOut* scale2 = ( CCEaseOut* ) ( scale1->copy ( ) );
    scale2->autorelease ( );
    CCActionInterval* sc_back2 = scale2->reverse ( );
    
    imgMipMap->runAction ( CCRepeatForever::create ( CCSequence::create ( scale1, sc_back , KD_NULL ) ) );
       pImage->runAction ( CCRepeatForever::create ( CCSequence::create ( scale2, sc_back2, KD_NULL ) ) );
    CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
}

const KDchar* TexturePVRMipMap2::title ( KDvoid )
{
    return "PVR MipMap Test #2";
}

const KDchar* TexturePVRMipMap2::subtitle ( KDvoid )
{
    return "Left image uses mipmap. Right image doesn't";
}

//------------------------------------------------------------------
//
// TexturePVR2BPP
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVR2BPP::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    CCNode*  pNode = CCSprite::create ( "Images/test_image_pvrtc2bpp.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode );   
}

const KDchar* TexturePVR2BPP::title ( KDvoid )
{
    return "PVR TC 2bpp Test";
}

//------------------------------------------------------------------
//
// TexturePVR
// To generate PVR images read this article:
// http://developer.apple.com/iphone/library/qa/qa2008/qa1611.html
//
//------------------------------------------------------------------
KDvoid TexturePVR::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    CCNode*  pNode = CCSprite::create ( "Images/test_image.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode );   
}

const KDchar* TexturePVR::title ( KDvoid )
{
    return "PVR TC 4bpp Test";
}

//------------------------------------------------------------------
//
// TexturePVR4BPP
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVR4BPP::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    CCNode*  pNode = CCSprite::create ( "Images/test_image_pvrtc4bpp.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVR4BPP::title ( KDvoid )
{
    return "PVR TC 4bpp Test #3";
}

//------------------------------------------------------------------
//
// TexturePVRRGBA8888
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRRGBA8888::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgba8888.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGBA8888::title ( KDvoid )
{
    return "PVR + RGBA  8888 Test";
}

//------------------------------------------------------------------
//
// TexturePVRBGRA8888
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRBGRA8888::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_bgra8888.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "BGRA8888 images are not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRBGRA8888::title ( KDvoid )
{
    return "PVR + BGRA 8888 Test";
}

//------------------------------------------------------------------
//
// TexturePVRRGBA5551
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRRGBA5551::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgba5551.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGBA5551::title ( KDvoid )
{
    return "PVR + RGBA 5551 Test";
}

//------------------------------------------------------------------
//
// TexturePVRRGBA4444
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRRGBA4444::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgba4444.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGBA4444::title ( KDvoid )
{
    return "PVR + RGBA 4444 Test";
}

//------------------------------------------------------------------
//
// TexturePVRRGBA4444GZ
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRRGBA4444GZ::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgba4444.pvr.gz_" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "PVRRGBA4444GZ images are not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGBA4444GZ::title ( KDvoid )
{
    return "PVR + RGBA 4444 + GZ Test";
}

const KDchar* TexturePVRRGBA4444GZ::subtitle ( KDvoid )
{
    return "This is a gzip PVR image.";
}

//------------------------------------------------------------------
//
// TexturePVRRGBA4444CCZ
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRRGBA4444CCZ::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgba4444.pvr.ccz" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGBA4444CCZ::title ( KDvoid )
{
    return "PVR + RGBA 4444 + CCZ Test";
}

const KDchar* TexturePVRRGBA4444CCZ::subtitle ( KDvoid )
{
    return "This is a ccz PVR image";
}

//------------------------------------------------------------------
//
// TexturePVRRGB565
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRRGB565::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgb565.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGB565::title ( KDvoid )
{
    return "PVR + RGB 565 Test";
}

//------------------------------------------------------------------
//
// TexturePVR RGB888
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRRGB888::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgb888.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 

}

const KDchar* TexturePVRRGB888::title ( KDvoid )
{
    return "PVR + RGB 888 Test";
}

//------------------------------------------------------------------
//
// TexturePVRA8
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRA8::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_a8.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRA8::title ( KDvoid )
{
    return "PVR + A8 Test";
}

//------------------------------------------------------------------
//
// TexturePVRI8
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRI8::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_i8.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRI8::title ( KDvoid )
{
    return "PVR + I8 Test";
}

//------------------------------------------------------------------
//
// TexturePVRAI88
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRAI88::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_ai88.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRAI88::title ( KDvoid )
{
    return "PVR + AI88 Test";
}

//------------------------------------------------------------------
//
// TexturePVR2BPPv3
//
//------------------------------------------------------------------
KDvoid TexturePVR2BPPv3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_pvrtc2bpp_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVR2BPPv3::title ( KDvoid )
{
    return "PVR TC 2bpp Test";
}

const KDchar* TexturePVR2BPPv3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRII2BPPv3
//
//------------------------------------------------------------------
KDvoid TexturePVRII2BPPv3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_pvrtcii2bpp_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRII2BPPv3::title ( KDvoid )
{
    return "PVR TC II 2bpp Test";
}

const KDchar* TexturePVRII2BPPv3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVR4BPPv3
//
//------------------------------------------------------------------
KDvoid TexturePVR4BPPv3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_pvrtc4bpp_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVR4BPPv3::title ( KDvoid )
{
    return "PVR TC 4bpp Test";
}

const KDchar* TexturePVR4BPPv3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRII4BPPv3
//
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRII4BPPv3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_pvrtcii4bpp_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRII4BPPv3::title ( KDvoid )
{
    return "PVR TC II 4bpp Test";
}

const KDchar* TexturePVRII4BPPv3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRRGBA8888v3
//
//------------------------------------------------------------------
KDvoid TexturePVRRGBA8888v3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgba8888_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGBA8888v3::title ( KDvoid )
{
    return "PVR + RGBA  8888 Test";
}

const KDchar* TexturePVRRGBA8888v3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRBGRA8888v3
//
//------------------------------------------------------------------
KDvoid TexturePVRBGRA8888v3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_bgra8888_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRBGRA8888v3::title ( KDvoid )
{
    return "PVR + BGRA 8888 Test";
}

const KDchar* TexturePVRBGRA8888v3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRRGBA5551v3
//
//------------------------------------------------------------------
KDvoid TexturePVRRGBA5551v3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgba5551_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGBA5551v3::title ( KDvoid )
{
    return "PVR + RGBA 5551 Test";
}

const KDchar* TexturePVRRGBA5551v3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRRGBA4444v3
//
//------------------------------------------------------------------
KDvoid TexturePVRRGBA4444v3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgba4444_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGBA4444v3::title ( KDvoid )
{
    return "PVR + RGBA 4444 Test";
}

const KDchar* TexturePVRRGBA4444v3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRRGB565v3
//
//------------------------------------------------------------------
KDvoid TexturePVRRGB565v3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgb565_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGB565v3::title ( KDvoid )
{
    return "PVR + RGB 565 Test";
}

const KDchar* TexturePVRRGB565v3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRRGB888v3
//
//------------------------------------------------------------------
KDvoid TexturePVRRGB888v3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_rgb888_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRRGB888v3::title ( KDvoid )
{
    return "PVR + RGB 888 Test";
}

const KDchar* TexturePVRRGB888v3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRA8v3
//
//------------------------------------------------------------------
KDvoid TexturePVRA8v3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_a8_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRA8v3::title ( KDvoid )
{
    return "PVR + A8 Test";
}

const KDchar* TexturePVRA8v3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRI8v3
//
//------------------------------------------------------------------
KDvoid TexturePVRI8v3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_i8_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRI8v3::title ( KDvoid )
{
    return "PVR + I8 Test";
}

const KDchar* TexturePVRI8v3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRAI88v3
//
//------------------------------------------------------------------
KDvoid TexturePVRAI88v3::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image_ai88_v3.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRAI88v3::title ( KDvoid )
{
    return "PVR + AI88 Test";
}

const KDchar* TexturePVRAI88v3::subtitle ( KDvoid )
{
    return "Testing PVR File Format v3";
}

//------------------------------------------------------------------
//
// TexturePVRBadEncoding
// Image generated using PVRTexTool:
// http://www.imgtec.com/powervr/insider/powervr-pvrtextool.asp
//
//------------------------------------------------------------------
KDvoid TexturePVRBadEncoding::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/test_image-bad_encoding.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRBadEncoding::title ( KDvoid )
{
    return "PVR Unsupported encoding";
}

const KDchar* TexturePVRBadEncoding::subtitle ( KDvoid )
{
    return "You should not see any image";
}

//------------------------------------------------------------------
//
// TexturePVRNonSquare
//
//------------------------------------------------------------------
KDvoid TexturePVRNonSquare::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/grossini_128x256_mipmap.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRNonSquare::title ( KDvoid )
{
    return "PVR + Non square texture";
}

const KDchar* TexturePVRNonSquare::subtitle ( KDvoid )
{
    return "Loading a 128x256 texture";
}

//------------------------------------------------------------------
//
// TexturePVRNPOT4444
//
//------------------------------------------------------------------
KDvoid TexturePVRNPOT4444::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/grossini_pvr_rgba4444.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRNPOT4444::title ( KDvoid )
{
    return "PVR RGBA4 + NPOT texture";
}

const KDchar* TexturePVRNPOT4444::subtitle ( KDvoid )
{
    return "Loading a 81x121 RGBA4444 texture.";
}

//------------------------------------------------------------------
//
// TexturePVRNPOT8888
//
//------------------------------------------------------------------
KDvoid TexturePVRNPOT8888::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	CCNode*  pNode = CCSprite::create ( "Images/grossini_pvr_rgba8888.pvr" );

	if ( pNode )
	{
		CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
	}
	else
	{
		pNode = CCLabelTTF::create ( "This test is not supported.", "fonts/Marker Felt.ttf", 28 );
	}
		
	pNode->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pNode ); 
}

const KDchar* TexturePVRNPOT8888::title ( KDvoid )
{
    return "PVR RGBA8 + NPOT texture";
}

const KDchar* TexturePVRNPOT8888::subtitle ( KDvoid )
{
    return "Loading a 81x121 RGBA8888 texture.";
}

//------------------------------------------------------------------
//
// TextureAlias
//
//------------------------------------------------------------------
KDvoid TextureAlias::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    const CCSize&  s = this->getContentSize ( );
    
    //
    // Sprite 1: GL_LINEAR
    //
    // Default filter is GL_LINEAR
    
    CCSprite*  pSprite = CCSprite::create ( "Images/grossinis_sister1.png" );
    pSprite->setPosition ( ccp ( s.cx/3.0f, s.cy / 2.0f ) );
    addChild ( pSprite );
    
    // this is the default filterting
    pSprite->getTexture ( )->setAntiAliasTexParameters ( );
    
    //
    // Sprite 1: GL_NEAREST
    //    
    
    CCSprite* pSprite2 = CCSprite::create ( "Images/grossinis_sister2.png" );
    pSprite2->setPosition ( ccp ( 2 * s.cx / 3.0f, s.cy / 2.0f ) );
    addChild ( pSprite2 );
    
    // Use Nearest in this one
    pSprite2->getTexture ( )->setAliasTexParameters ( );
        
    // scale them to show
    CCScaleBy* sc = CCScaleBy::create ( 3, 8.0f );
    CCScaleBy* sc_back = (CCScaleBy*) ( sc->reverse ( ) );
    CCRepeatForever* scaleforever = CCRepeatForever::create ( CCSequence::create ( sc, sc_back, KD_NULL ) );
    CCRepeatForever* scaleToo = ( CCRepeatForever* ) ( scaleforever->copy ( ) );
    scaleToo->autorelease ( );

    pSprite2->runAction ( scaleforever );
    pSprite->runAction ( scaleToo );
    CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
}

const KDchar* TextureAlias::title ( KDvoid )
{
    return "AntiAlias / Alias textures";
}

const KDchar* TextureAlias::subtitle ( KDvoid )
{
    return "Left image is antialiased. Right image is aliases";
}

//------------------------------------------------------------------
//
// TexturePixelFormat
//
//------------------------------------------------------------------
KDvoid TexturePixelFormat::onEnter ( KDvoid )
{
    //
    // This example displays 1 png images 4 times.
    // Each time the image is generated using:
    // 1- 32-bit RGBA8
    // 2- 16-bit RGBA4
    // 3- 16-bit RGB5A1
    // 4- 16-bit RGB565
    TestTexture2D::onEnter ( );
    
    const CCSize&  s = this->getContentSize ( );

    CCLayerColor *background = CCLayerColor::create ( ccc4 ( 128,128,128,255 ), s.cx, s.cy );
    addChild ( background, -1 );
    
    // RGBA 8888 image ( 32-bit )
    CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGBA8888 );
    CCSprite *sprite1 = CCSprite::create ( "Images/test-rgba1.png" );
    sprite1->setPosition ( ccp ( 1*s.cx/7, s.cy/2+32 ) );
    addChild ( sprite1, 0 );

    // remove texture from texture manager    
    CCTextureCache::sharedTextureCache ( )->removeTexture ( sprite1->getTexture ( ) );

    // RGBA 4444 image ( 16-bit )
    CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGBA4444 );
    CCSprite *pSprite2 = CCSprite::create ( "Images/test-rgba1.png" );
    pSprite2->setPosition ( ccp ( 2*s.cx/7, s.cy/2-32 ) );
    addChild ( pSprite2, 0 );

    // remove texture from texture manager    
    CCTextureCache::sharedTextureCache ( )->removeTexture ( pSprite2->getTexture ( ) );

    // RGB5A1 image ( 16-bit )
    CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGB5A1 );
    CCSprite *sprite3 = CCSprite::create ( "Images/test-rgba1.png" );
    sprite3->setPosition ( ccp ( 3*s.cx/7, s.cy/2+32 ) );
    addChild ( sprite3, 0 );

    // remove texture from texture manager    
    CCTextureCache::sharedTextureCache ( )->removeTexture ( sprite3->getTexture ( ) );

    // RGB888 image
    CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGB888 );
    CCSprite *sprite4 = CCSprite::create ( "Images/test-rgba1.png" );
    sprite4->setPosition ( ccp ( 4*s.cx/7, s.cy/2-32 ) );
    addChild ( sprite4, 0 );

    // remove texture from texture manager    
    CCTextureCache::sharedTextureCache ( )->removeTexture ( sprite4->getTexture ( ) );

    // RGB565 image ( 16-bit )
    CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGB565 );
    CCSprite *sprite5 = CCSprite::create ( "Images/test-rgba1.png" );
    sprite5->setPosition ( ccp ( 5*s.cx/7, s.cy/2+32 ) );
    addChild ( sprite5, 0 );

    // remove texture from texture manager    
    CCTextureCache::sharedTextureCache ( )->removeTexture ( sprite5->getTexture ( ) );

    // A8 image ( 8-bit )
    CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_A8 );
    CCSprite *sprite6 = CCSprite::create ( "Images/test-rgba1.png" );
    sprite6->setPosition ( ccp ( 6*s.cx/7, s.cy/2-32 ) );
    addChild ( sprite6, 0 );
    
    // remove texture from texture manager    
    CCTextureCache::sharedTextureCache ( )->removeTexture ( sprite6->getTexture ( ) );

    CCFadeOut* fadeout = CCFadeOut::create ( 2 );
    CCFadeIn*  fadein  = CCFadeIn::create ( 2 );
    CCSequence*  seq = CCSequence::create ( CCDelayTime::create ( 2 ), fadeout, fadein, KD_NULL );
    CCRepeatForever* seq_4ever = CCRepeatForever::create ( seq );
    CCRepeatForever* seq_4ever2 = (CCRepeatForever*) ( seq_4ever->copy ( ) ); seq_4ever2->autorelease ( );
    CCRepeatForever* seq_4ever3 = (CCRepeatForever*) ( seq_4ever->copy ( ) ); seq_4ever3->autorelease ( );
    CCRepeatForever* seq_4ever4 = (CCRepeatForever*) ( seq_4ever->copy ( ) ); seq_4ever4->autorelease ( );
    CCRepeatForever* seq_4ever5 = (CCRepeatForever*) ( seq_4ever->copy ( ) ); seq_4ever5->autorelease ( );
    
    sprite1->runAction ( seq_4ever );
    pSprite2->runAction ( seq_4ever2 );
    sprite3->runAction ( seq_4ever3 );
    sprite4->runAction ( seq_4ever4 );
    sprite5->runAction ( seq_4ever5 );

    // restore default
    CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_Default );
    CCTextureCache::sharedTextureCache ( )->dumpCachedTextureInfo ( );
}

const KDchar* TexturePixelFormat::title ( KDvoid )
{
    return "Texture Pixel Formats";
}

const KDchar* TexturePixelFormat::subtitle ( KDvoid )
{
    return "Textures: RGBA8888, RGBA4444, RGB5A1, RGB888, RGB565, A8";
}

//------------------------------------------------------------------
//
// TextureBlend
//
//------------------------------------------------------------------
KDvoid TextureBlend::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

    for ( KDint i = 0; i < 15; i++ )
    {
        // BOTTOM sprites have alpha pre-multiplied
        // they use by default GL_ONE, GL_ONE_MINUS_SRC_ALPHA
        CCSprite*  pCloud = CCSprite::create ( "Images/test_blend.png" );
		addChild ( pCloud, i + 1, 100 + i );
		pCloud->setPosition ( ccp ( tLyrSize.cx / 4 + 25 * i, tLyrSize.cy / 3 ) );
        ccBlendFunc  tBlendFunc1 = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
        pCloud->setBlendFunc ( tBlendFunc1 );

        // CENTER sprites have also alpha pre-multiplied
        // they use by default GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
        pCloud = CCSprite::create ( "Images/test_blend.png" );
        addChild ( pCloud, i + 1, 200 + i );
        pCloud->setPosition ( ccp ( tLyrSize.cx / 4 + 25 * i, tLyrSize.cy / 3 + 80 ) );
        ccBlendFunc  tBlendFunc2 = { GL_ONE_MINUS_DST_COLOR, GL_ZERO };
        pCloud->setBlendFunc ( tBlendFunc2 );

        // UPPER sprites are using custom blending function
        // You can set any blend function to your sprites
        pCloud = CCSprite::create ( "Images/test_blend.png" );
        addChild ( pCloud, i + 1, 200 + i );
        pCloud->setPosition ( ccp ( tLyrSize.cx / 4 + 25 * i, tLyrSize.cy / 3 + 160 ) );
        ccBlendFunc  tBlendFunc3 = { GL_SRC_ALPHA, GL_ONE };
        pCloud->setBlendFunc ( tBlendFunc3 );  // additive blending
    }
}

const KDchar* TextureBlend::title ( KDvoid )
{
    return "Texture Blending";
}

const KDchar* TextureBlend::subtitle ( KDvoid )
{
    return "Testing 3 different blending modes";
}


//------------------------------------------------------------------
//
// TextureAsync
//
//------------------------------------------------------------------

KDvoid TextureAsync::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    m_nImageOffset = 0;

	CCDirector::sharedDirector ( )->purgeCachedData ( );

    const CCSize&  size = this->getContentSize ( );

    CCLabelTTF *label = CCLabelTTF::create ( "Loading...", "fonts/Marker Felt.ttf", 32 );
    label->setPosition ( ccp ( size.cx/2, size.cy/2 ) );
    addChild ( label, 10 );

    CCScaleBy* scale = CCScaleBy::create ( 0.3f, 2 );
    CCScaleBy* scale_back = (CCScaleBy*)scale->reverse ( );
    CCSequence* seq = CCSequence::create ( scale, scale_back, KD_NULL );
    label->runAction ( CCRepeatForever::create ( seq ) );

    scheduleOnce ( schedule_selector ( TextureAsync::loadImages ), 1.0f );
}

KDvoid TextureAsync::onExit ( KDvoid )
{
    CCTextureCache::sharedTextureCache ( )->removeAllTextures ( );

	TestTexture2D::onExit ( );
}

KDvoid TextureAsync::loadImages ( KDfloat dt )
{
    for ( KDint i = 0; i < 8; i++ ) 
	{
        for ( KDint j = 0 ; j < 8; j++ ) 
		{
            KDchar  szSpriteName[100] = {0};
            kdSprintf ( szSpriteName, "Images/sprites_test/sprite-%d-%d.png", i, j );
            CCTextureCache::sharedTextureCache ( )->addImageAsync ( szSpriteName, this, callfuncO_selector ( TextureAsync::imageLoaded ) );
        }
    }

    CCTextureCache::sharedTextureCache ( )->addImageAsync ( "Images/background1.jpg"		 , this, callfuncO_selector ( TextureAsync::imageLoaded ) );
    CCTextureCache::sharedTextureCache ( )->addImageAsync ( "Images/background2.jpg"		 , this, callfuncO_selector ( TextureAsync::imageLoaded ) );
    CCTextureCache::sharedTextureCache ( )->addImageAsync ( "Images/background.png"			 , this, callfuncO_selector ( TextureAsync::imageLoaded ) );
    CCTextureCache::sharedTextureCache ( )->addImageAsync ( "Images/atlastest.png"			 , this, callfuncO_selector ( TextureAsync::imageLoaded ) );
    CCTextureCache::sharedTextureCache ( )->addImageAsync ( "Images/grossini_dance_atlas.png", this, callfuncO_selector ( TextureAsync::imageLoaded ) );
}


KDvoid TextureAsync::imageLoaded ( CCObject* pObj )
{
    CCTexture2D*  pTex = (CCTexture2D*) pObj;

    //CCAssert ( [NSThread currentThread] == [director runningThread], @"FAIL. Callback should be on cocos2d thread" );

    // IMPORTANT: The order on the callback is not guaranteed. Don't depend on the callback

    // This test just creates a sprite based on the Texture

    CCSprite*  pSprite = CCSprite::createWithTexture ( pTex );
    pSprite->setAnchorPoint ( ccp ( 0, 0 ) );
    addChild ( pSprite, -1 );

	const CCSize&  tLyrSize = this->getContentSize ( );

    KDint  i = m_nImageOffset * 32;
    pSprite->setPosition ( ccp ( i % (KDint) tLyrSize.cx, ( i / (KDint) tLyrSize.cx ) * 32 ) );

    m_nImageOffset++;

    CCLOG ( "Image loaded: %p", pTex );
}

const KDchar* TextureAsync::title ( KDvoid )
{
    return "Texture Async Load";
}

const KDchar* TextureAsync::subtitle ( KDvoid )
{
    return "Textures should load while an animation is being run";
}


//------------------------------------------------------------------
//
// TextureGlClamp
//
//------------------------------------------------------------------
KDvoid TextureGlClamp::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    CCSize size = this->getContentSize ( );

    // The .png image MUST be power of 2 in order to create a continue effect.
    // eg: 32x64, 512x128, 256x1024, 64x64, etc..
    CCSprite*  pSprite = CCSprite::create ( "Images/pattern1.png", CCRectMake ( 0,0,512,256 ) );
    addChild ( pSprite, -1, kTagSprite1 );
    pSprite->setPosition ( ccp ( size.cx/2,size.cy/2 ) );
    ccTexParams params = {GL_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    pSprite->getTexture ( )->setTexParameters ( params );

    CCRotateBy* rotate = CCRotateBy::create ( 4, 360 );
    pSprite->runAction ( rotate );
    CCScaleBy* scale = CCScaleBy::create ( 2, 0.04f );
    CCScaleBy* scaleBack = (CCScaleBy*) ( scale->reverse ( ) );
    CCSequence* seq = CCSequence::create ( scale, scaleBack, KD_NULL );
    pSprite->runAction ( seq );
}

const KDchar* TextureGlClamp::title ( KDvoid )
{
    return "Texture GL_CLAMP";
}

KDvoid TextureGlClamp::onExit ( KDvoid )
{
    CCTextureCache::sharedTextureCache ( )->removeUnusedTextures ( );

	TestTexture2D::onExit ( );
}

//------------------------------------------------------------------
//
// TextureGlRepeat
//
//------------------------------------------------------------------
KDvoid TextureGlRepeat::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    CCSize  size = this->getContentSize ( );
    
    // The .png image MUST be power of 2 in order to create a continue effect.
    // eg: 32x64, 512x128, 256x1024, 64x64, etc..
    CCSprite*  pSprite = CCSprite::create ( "Images/pattern1.png", CCRectMake ( 0, 0, 4096, 4096 ) );
    addChild ( pSprite, -1, kTagSprite1 );
    pSprite->setPosition ( ccp ( size.cx / 2,size.cy / 2 ) );
    ccTexParams params = { GL_LINEAR,GL_LINEAR, GL_REPEAT, GL_REPEAT };
    pSprite->getTexture ( )->setTexParameters ( params );
    
    CCRotateBy* rotate = CCRotateBy::create ( 4, 360 );
    pSprite->runAction ( rotate );
    CCScaleBy* scale = CCScaleBy::create ( 2, 0.04f );
    CCScaleBy* scaleBack = (CCScaleBy*) ( scale->reverse ( ) );
    CCFiniteTimeAction* seq = CCSequence::create ( scale, scaleBack, NULL );
    pSprite->runAction ( seq );
}

const KDchar* TextureGlRepeat::title ( KDvoid )
{
    return "Texture GL_REPEAT";
}

KDvoid TextureGlRepeat::onExit ( KDvoid )
{
    CCTextureCache::sharedTextureCache ( )->removeUnusedTextures ( );

	TestTexture2D::onExit ( );
}

//------------------------------------------------------------------
//
// TextureSizeTest
//
//------------------------------------------------------------------
KDvoid TextureSizeTest::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    KDchar     szOutput [ 256 ] = "";
	CCSprite*  pSprite = KD_NULL;

    kdStrcat ( szOutput, "Loading 512x512 image...\n" );
    pSprite = CCSprite::create ( "Images/texture512x512.png" );
	kdStrcat ( szOutput, pSprite ? "Ok" : "Error" );

	kdStrcat ( szOutput, "\n\n" );

    kdStrcat ( szOutput, "Loading 1024x1024 image...\n" );
    pSprite = CCSprite::create ( "Images/texture1024x1024.png" );
	kdStrcat ( szOutput, pSprite ? "Ok" : "Error" );

	kdStrcat ( szOutput, "\n" );

	CCLabelTTF*  pLabel = CCLabelTTF::create ( szOutput, "fonts/Marker Felt.ttf", 28 );		
	pLabel->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pLabel );
}

const KDchar* TextureSizeTest::title ( KDvoid )
{
    return "Different Texture Sizes";
}

const KDchar* TextureSizeTest::subtitle ( KDvoid )
{
    return "512x512, 1024x1024.";
}

//------------------------------------------------------------------
//
// TextureCache1
//
//------------------------------------------------------------------
KDvoid TextureCache1::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    const CCSize&  s = this->getContentSize ( );
    
    CCSprite*  pSprite;

    pSprite = CCSprite::create ( "Images/grossinis_sister1.png" );
    pSprite->setPosition ( ccp ( s.cx / 5 * 1, s.cy / 2 ) );
    pSprite->getTexture ( )->setAliasTexParameters ( );
    pSprite->setScale ( 2 );
    addChild ( pSprite );

    CCTextureCache::sharedTextureCache ( )->removeTexture ( pSprite->getTexture ( ) );
    
    pSprite = CCSprite::create ( "Images/grossinis_sister1.png" );
    pSprite->setPosition ( ccp ( s.cx / 5 * 2, s.cy / 2 ) );
    pSprite->getTexture ( )->setAntiAliasTexParameters ( );
    pSprite->setScale ( 2 );
    addChild ( pSprite );

    // 2nd set of sprites
    
    pSprite = CCSprite::create ( "Images/grossinis_sister2.png" );
    pSprite->setPosition ( ccp ( s.cx / 5 * 3, s.cy / 2 ) );
    pSprite->getTexture ( )->setAliasTexParameters ( );
    pSprite->setScale ( 2 );
    addChild ( pSprite );
    
    CCTextureCache::sharedTextureCache ( )->removeTextureForKey ( "Images/grossinis_sister2.png" );

    pSprite = CCSprite::create ( "Images/grossinis_sister2.png" );
    pSprite->setPosition ( ccp ( s.cx / 5 * 4, s.cy / 2 ) );
    pSprite->getTexture ( )->setAntiAliasTexParameters ( );
    pSprite->setScale ( 2 );
    addChild ( pSprite );
}

const KDchar* TextureCache1::title ( KDvoid )
{
    return "CCTextureCache: remove";
}

const KDchar* TextureCache1::subtitle ( KDvoid )
{
    return "4 images should appear: alias, antialias, alias, antilias";
}

//------------------------------------------------------------------
//
// TextureDrawAtPoint
//
//------------------------------------------------------------------
KDvoid TextureDrawAtPoint::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    m_pTex1 = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/grossinis_sister1.png" );
    m_pTex2 = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/grossinis_sister2.png" );

    m_pTex1->retain ( );
    m_pTex2->retain ( );
}

KDvoid TextureDrawAtPoint::onExit ( KDvoid )
{
    m_pTex1->release ( );
    m_pTex2->release ( );

	TestTexture2D::onExit ( );
}

const KDchar* TextureDrawAtPoint::title ( KDvoid )
{
    return "CCTexture2D: drawAtPoint";
}

const KDchar* TextureDrawAtPoint::subtitle ( KDvoid )
{
    return "draws 2 textures using drawAtPoint";
}

KDvoid TextureDrawAtPoint::draw ( KDvoid )
{
    TestTexture2D::draw ( );

    const CCSize&  s = this->getContentSize ( );

    m_pTex1->drawAtPoint ( ccp ( s.cx/2-50, s.cy/2 - 50 ) );
    m_pTex2->drawAtPoint ( ccp ( s.cx/2+50, s.cy/2 - 50 ) );

}

//------------------------------------------------------------------
//
// TextureDrawInRect
//
//------------------------------------------------------------------
KDvoid TextureDrawInRect::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );
    m_pTex1 = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/grossinis_sister1.png" );
    m_pTex2 = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/grossinis_sister2.png" );

    m_pTex1->retain ( );
    m_pTex2->retain ( );
}

KDvoid TextureDrawInRect::onExit ( KDvoid )
{
    m_pTex1->release ( );
    m_pTex2->release ( );

	TestTexture2D::onExit ( );
}

KDvoid TextureDrawInRect::draw ( KDvoid )
{
    TestTexture2D::draw ( );

    const CCSize&  s = this->getContentSize ( );

    CCRect rect1 = CCRectMake ( s.cx / 2 - 80, 20, m_pTex1->getContentSize ( ).cx * 0.5f, m_pTex1->getContentSize ( ).cy *2 );
    CCRect rect2 = CCRectMake ( s.cx / 2 + 80, s.cy / 2, m_pTex1->getContentSize ( ).cx * 2, m_pTex1->getContentSize ( ).cy * 0.5f );

    m_pTex1->drawInRect ( rect1 );
    m_pTex2->drawInRect ( rect2 );
}

const KDchar* TextureDrawInRect::title ( KDvoid )
{
    return "CCTexture2D: drawInRect";
}

const KDchar* TextureDrawInRect::subtitle ( KDvoid )
{
    return "draws 2 textures using drawInRect";
}

//------------------------------------------------------------------
//
// TextureMemoryAlloc
//
//------------------------------------------------------------------
KDvoid TextureMemoryAlloc::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );

    m_pBackground = NULL;
    
	const CCSize&  tLyrSize = this->getContentSize ( );
    CCMenuItemFont::setFontSize ( 24 );
	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
    
    CCMenuItem*  pItem1 = CCMenuItemFont::create ( "PNG", this, menu_selector ( TextureMemoryAlloc::updateImage ) );
    pItem1->setTag ( 0 );
    
    CCMenuItem*  pItem2 = CCMenuItemFont::create ( "RGBA8", this, menu_selector ( TextureMemoryAlloc::updateImage ) );
    pItem2->setTag ( 1 );
    
    CCMenuItem*  pItem3 = CCMenuItemFont::create ( "RGB8", this, menu_selector ( TextureMemoryAlloc::updateImage ) );
    pItem3->setTag ( 2 );
    
    CCMenuItem*  pItem4 = CCMenuItemFont::create ( "RGBA4", this, menu_selector ( TextureMemoryAlloc::updateImage ) );
    pItem4->setTag ( 3 );
    
    CCMenuItem*  pItem5 = CCMenuItemFont::create ( "A8", this, menu_selector ( TextureMemoryAlloc::updateImage ) );
    pItem5->setTag ( 4 );
    
    CCMenu*  pMenu = CCMenu::create ( pItem1, pItem2, pItem3, pItem4, pItem5, KD_NULL );
    pMenu->alignItemsHorizontally ( );    
	pMenu->setPosition ( ccpMid ( tLyrSize ) );
    this->addChild ( pMenu );
    
    CCMenuItemFont*  pWarmup = CCMenuItemFont::create ( "warm up texture", this, menu_selector ( TextureMemoryAlloc::changeBackgroundVisible ) );
   
    CCMenu*  pMenu2 = CCMenu::create ( pWarmup, KD_NULL );  
	pMenu2->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 4 ) );
    this->addChild ( pMenu2 );    
}

KDvoid TextureMemoryAlloc::changeBackgroundVisible ( CCObject* pSender )
{
    if ( m_pBackground )
    {
        m_pBackground->setVisible ( KD_TRUE );
    }
}

KDvoid TextureMemoryAlloc::updateImage ( CCObject* pSender )
{
    if ( m_pBackground )
    {
        m_pBackground->removeFromParentAndCleanup ( KD_TRUE );
    }
    
    CCTextureCache::sharedTextureCache ( )->removeUnusedTextures ( );
	
	const KDchar*  szPath = KD_NULL;
	KDint			 nTag = ( (CCNode*) pSender )->getTag ( );

	switch ( nTag ) 
    {
		//case 0 :	szPath = "Images/test_1021x1024.png";				break;
		//case 1 :	szPath = "Images/test_1021x1024_rgba8888.pvr";		break;
		//case 2 :	szPath = "Images/test_1021x1024_rgb888.pvr";		break;
		//case 3 :	szPath = "Images/test_1021x1024_rgba4444.pvr";		break;
		//case 4 :	szPath = "Images/test_1021x1024_a8.pvr";			break;

		case 1 :	szPath = "Images/test_1021x1024_rgba8888.pvr.gz_";	break;
		case 2 :	szPath = "Images/test_1021x1024_rgb888.pvr.gz_";	break;
		case 3 :	szPath = "Images/test_1021x1024_rgba4444.pvr.gz_";	break;
		case 4 :	szPath = "Images/test_1021x1024_a8.pvr.gz_";		break;
	}

    m_pBackground = CCSprite::create ( szPath );
	if ( m_pBackground )
	{
		this->addChild ( m_pBackground, -10 );
		m_pBackground->setVisible ( KD_TRUE );

		m_pBackground->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	}
}

const KDchar* TextureMemoryAlloc::title ( KDvoid )
{
    return "Texture memory";
}

const KDchar* TextureMemoryAlloc::subtitle ( KDvoid )
{
    return "Testing Texture Memory allocation.\nUse Instruments + VM Tracker";
}

//------------------------------------------------------------------
//
// TexturePVRv3Premult
//
//------------------------------------------------------------------
KDvoid TexturePVRv3Premult::onEnter ( KDvoid )
{
    TestTexture2D::onEnter ( );
    
	const CCSize&	tLyrSize = this->getContentSize ( );

    // PVR premultiplied
    CCSprite*	pPvr1 = CCSprite::create ( "Images/grossinis_sister1-testalpha_premult.pvr" );
	pPvr1->setPosition ( ccp ( tLyrSize.cx / 4 * 1, tLyrSize.cy / 2 ) );
    this->addChild ( pPvr1, 0 );    
    this->transformSprite ( pPvr1 );
    
    // PVR non-premultiplied
    CCSprite*	pPvr2 = CCSprite::create ( "Images/grossinis_sister1-testalpha_nopremult.pvr" );
    this->addChild ( pPvr2, 0 );
	this->transformSprite ( pPvr2 );
    pPvr2->setPosition ( ccp ( tLyrSize.cx / 4 * 2, tLyrSize.cy / 2 ) );
    
    
    // PNG
    CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGBA8888 );
    CCTextureCache::sharedTextureCache ( )->removeTextureForKey ( "Images/grossinis_sister1-testalpha.png" );
    CCSprite*	pPng = CCSprite::create ( "Images/grossinis_sister1-testalpha.png" );
    this->addChild ( pPng, 0 );
	this->transformSprite ( pPng );
    pPng->setPosition ( ccp ( tLyrSize.cx / 4 * 3, tLyrSize.cy / 2 ) );
    
}

const KDchar* TexturePVRv3Premult::title ( KDvoid )
{
    return "PVRv3 Premult Flag";
}

const KDchar* TexturePVRv3Premult::subtitle ( KDvoid )
{
    return "All images should look exactly the same";
}

KDvoid TexturePVRv3Premult::transformSprite ( CCSprite* pSprite )
{
    pSprite->runAction 
	( 
		CCRepeatForever::create 
		(
			CCSequence::create
			(
				CCFadeOut	::create ( 2 ),
				CCFadeIn	::create ( 2 ), 
				CCDelayTime	::create ( 2 ),
				KD_NULL
			) 
		)
	);
}
