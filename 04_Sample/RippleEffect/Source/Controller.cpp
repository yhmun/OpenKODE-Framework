/* --------------------------------------------------------------------------
 *
 *      File            Main.cpp
 *      Description     Application Entrance.
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

#include "Precompiled.h"
#include "Controller.h"

Video::Video ( KDvoid )
{
	static KDbool  bInit = 0;

	if ( !bInit )
	{
		avcodec_init ( );
		avcodec_register_all ( );
		av_register_all ( );
	}

	m_pSwsCtx		= 0;
	m_pFormatCtx	= 0;
	m_pCodecCtx		= 0;
	m_pCodec		= 0;
	m_pFrame		= 0;
	m_pFrameRGB		= 0;
	m_nIndex		= 0;
}

Video::~Video ( KDvoid )
{
	this->freeVideo ( );
}

KDbool Video::openVideo ( const KDchar* szPath )
{
	// Open av file
	if ( av_open_input_file ( &m_pFormatCtx, szPath, 0, 0, 0 ) != 0 )
	{
		kdLogMessage ( "Couldn't open file." );
		return KD_FALSE;
	}

	// Retrieve stream information
	if ( av_find_stream_info ( m_pFormatCtx ) < 0 )
	{
		kdLogMessage ( "Couldn't find stream information" );
		return KD_FALSE;
	}

	for ( KDuint i = 0; i < m_pFormatCtx->nb_streams; i++ )
	{
		if ( m_pFormatCtx->streams [ i ]->codec->codec_type == AVMEDIA_TYPE_VIDEO )
		{
			m_nIndex = i;

			// Get a pointer to the codec context for the video stream
			m_pCodecCtx = m_pFormatCtx->streams[i]->codec;
			break;
		}
	}

	if ( m_pCodecCtx == 0 )
	{
		kdLogMessage ( "Couldn't find a video stream" );
		return KD_FALSE;
	}

	m_pCodec = avcodec_find_decoder ( m_pCodecCtx->codec_id );
	if ( !m_pCodec )
	{
		kdLogMessage ( "Couldn't find codec" );
		return KD_FALSE;
	}

	if ( m_pCodec->capabilities & CODEC_CAP_TRUNCATED )
	{
		m_pCodecCtx->flags |= CODEC_CAP_TRUNCATED;
	}

	// Open codec
	if ( avcodec_open ( m_pCodecCtx, m_pCodec ) < 0 )
	{
		kdLogMessage ( "Couldn't open codec" );
		return KD_FALSE;
	}

	KDint  nBytes = avpicture_get_size ( PIX_FMT_RGB24, m_pCodecCtx->width, m_pCodecCtx->height );

	m_pFrame	= avcodec_alloc_frame ( );
	m_pFrameRGB = avcodec_alloc_frame ( );
	m_pFrameRGB->data[0] = (uint8_t *) av_malloc ( nBytes * sizeof (uint8_t) );

	avpicture_fill ( (AVPicture *) m_pFrameRGB, m_pFrameRGB->data[0], PIX_FMT_RGB24, m_pCodecCtx->width, m_pCodecCtx->height );

	m_pSwsCtx = sws_getContext
	(
		m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt,
        m_pCodecCtx->width, m_pCodecCtx->height, PIX_FMT_RGB24,
		SWS_BICUBIC, NULL, NULL, NULL
	);

	return KD_TRUE;
}

KDvoid Video::freeVideo ( KDvoid )
{
	// Free the RGB image
	if ( m_pFrameRGB )
	{
		av_free ( m_pFrameRGB->data[0] );
		av_free ( m_pFrameRGB );
	}

	// Free the YUV frame
	if ( m_pFrame )
	{
		av_free ( m_pFrame );
	}

	// Close the codec
	if ( m_pCodecCtx )
	{
		avcodec_close ( m_pCodecCtx );
	}

	// Close the video file
	if ( m_pFormatCtx )
	{
		av_close_input_file ( m_pFormatCtx );
	}

	if ( m_pSwsCtx )
	{
		sws_freeContext ( m_pSwsCtx );
	}
}

KDbool Video::readFrame ( KDvoid )
{
	while ( av_read_frame ( m_pFormatCtx, &m_tPacket) >=0 )
	{
		if ( m_tPacket.stream_index == m_nIndex )
		{
			int  nFinish;

			// Decode video frame
			avcodec_decode_video2 ( m_pCodecCtx, m_pFrame, &nFinish, &m_tPacket );

			if ( nFinish )
			{
				sws_scale
				(
					m_pSwsCtx, m_pFrame->data, m_pFrame->linesize, 0, 
					m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize
				);

				return KD_TRUE;
			}
		}
	}

	return KD_FALSE;
}

KDbool Controller::init ( KDvoid )
{
	CCLayer::init ( );

	const CCSize&  tSize = CCDirector::sharedDirector ( )->getWinSize ( );	

	CCMenu*  pMenu = CCMenu::create
	(
		CCMenuItemImage::create 
        (
            "/res/image/btn_prev_0.png"	 ,
            "/res/image/btn_prev_1.png"	 ,
            this, menu_selector ( Controller::onPrev ) 
        ),
		CCMenuItemImage::create
        (
            "/res/image/btn_restart_0.png",
            "/res/image/btn_restart_1.png",
            this, menu_selector ( Controller::onRestart ) 
        ),
		CCMenuItemImage::create
        (
            "/res/image/btn_next_0.png"	 , 
            "/res/image/btn_next_1.png"	 , 
            this, menu_selector ( Controller::onNext ) 
        ),				
		KD_NULL
	);
	do
	{
		CC_BREAK_IF ( !pMenu );

		pMenu->setPosition ( ccp ( tSize.cx / 2, 35 ) );
		pMenu->alignItemsHorizontally ( 30 );

		this->addChild ( pMenu );

	} while ( 0 );	

	CCLabelTTF*  pTitle = CCLabelTTF::create
    (
        "Ripple Effect", "/res/font/Schwarzwald Regular.ttf", 28, CCSize ( tSize.cx - 20.f, 0 ), kCCTextAlignmentCenter 
    );
	do
	{
		CC_BREAK_IF ( !pTitle );

		pTitle->setPosition ( ccp ( tSize.cx / 2, tSize.cy - 50 ) );
		pTitle->setColor ( ccGREEN );

		this->addChild ( pTitle );
		
	} while ( 0 );	

	this->setTouchEnabled ( KD_TRUE );
    
	m_pVideo	= 0;
	m_pTexture	= 0;
	m_pRipple	= 0;

	setRipple ( 0 );

	return KD_TRUE;
}

KDvoid Controller::onExit ( KDvoid )
{
	freeRipple ( );

	CCLayer::onExit ( );
}

KDvoid Controller::openRipple ( const KDchar* szPath )
{
	const CCSize&  tSize = CCDirector::sharedDirector ( )->getWinSize ( );	

	m_pVideo = new Video ( );
	if ( m_pVideo && m_pVideo->openVideo ( szPath ) )
	{
		CCSize  tSize2 = CCSizeMake ( m_pVideo->m_pCodecCtx->width, m_pVideo->m_pCodecCtx->height );

		m_pTexture = new XMGTexture ( (GLsizei) tSize2.cx, (GLsizei) tSize2.cy, XMG_FORMAT_RGBA888 );
		m_pRipple  = new XMGRipple  ( );

		m_pTexture->SetWrap ( XMG_WRAP_CLAMP_TO_EDGE );
		m_pRipple->SetTexture ( m_pTexture, XMG_FLIP_Y );
		m_pRipple->SetVertexArray ( tSize2.cx, tSize2.cy );

		m_tRect = CCRect((tSize.cx - tSize2.cx) / 2, (tSize.cy - tSize2.cy) / 2, tSize2.cx, tSize2.cy);
		m_tRect = CCRect(0, 0, tSize2.cx, tSize2.cy);
		m_pRipple->SetPosition(XMGVector3F(m_tRect.origin.x, m_tRect.origin.y));		
	}
}

KDvoid Controller::freeRipple ( KDvoid )
{
	CC_SAFE_DELETE ( m_pVideo );
	CC_SAFE_DELETE ( m_pTexture );
	CC_SAFE_DELETE ( m_pRipple );
}

KDvoid Controller::setRipple ( KDint nIndex )
{
	static const KDchar*  aFiles [] =
	{
		"media/CF_2NE1_Etude.avi",
		"media/CF_Lotte_Department.avi",
		"media/CF_LG_Phone.avi",
		"media/CF_LG_Phone_Cooky.avi",
		"media/CF_Caribbean_Bay.avi",
	};

	KDchar  szPath [ 256 ];

	if ( nIndex < 0 )
	{
		nIndex = 4;
	}
	else if ( nIndex > 4 )
	{
		nIndex = 0;
	}
	m_nIndex = nIndex;

	kdSprintfKHR ( szPath, "/res/%s", aFiles [ nIndex ] );

	freeRipple ( );
	openRipple ( szPath );
}

KDvoid Controller::draw ( KDvoid )
{
	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_None );

	if ( m_pVideo && m_pVideo->readFrame ( ) )
	{
		m_pTexture->SetPixels ( m_pVideo->m_pFrameRGB->data[0] );
	}

	m_pRipple->Render ( );

	CCDirector::sharedDirector ( )->setGLDefaultValues ( );
}

KDvoid Controller::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCSetIterator iter;
	for ( iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ ) 
	{
		CCTouch*  pTouch = (CCTouch *) ( *iter );
		const CCPoint&  tPosition = pTouch->getLocation ( );

		if ( m_tRect.containsPoint ( tPosition ) )
		{
			m_pRipple->SetRipple
			(
				(GLint) ( tPosition.x - m_tRect.origin.x ), 
				(GLint) ( tPosition.y - m_tRect.origin.y ),
				12
			);
		}
	}
}

KDvoid Controller::onRestart ( CCObject* pSender )
{
	setRipple ( m_nIndex );
}

KDvoid Controller::onPrev ( CCObject* pSender )
{
	setRipple ( --m_nIndex );
}

KDvoid Controller::onNext ( CCObject* pSender )
{
	setRipple ( ++m_nIndex );
}

