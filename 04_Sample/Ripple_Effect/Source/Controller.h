/* --------------------------------------------------------------------------
 *
 *      File            Controller.cpp
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

#ifndef __Controller_h__
#define __Controller_h__

class Video
{
	public :

		 Video ( KDvoid );
		~Video ( KDvoid );

		KDbool  openVideo ( const KDchar* szPath );
		KDvoid  freeVideo ( KDvoid );

		KDbool  readFrame ( KDvoid );

	public :

		struct SwsContext*		m_pSwsCtx;
		AVFormatContext*		m_pFormatCtx;
		AVCodecContext*			m_pCodecCtx;
		AVCodec*				m_pCodec;
		AVFrame*				m_pFrame;
		AVFrame*				m_pFrameRGB;
		AVPacket				m_tPacket;
		KDint					m_nIndex;
};

class Controller : public CCLayer
{
	public :

		CREATE_FUNC ( Controller );

	protected :

		virtual KDbool  init ( KDvoid );

		virtual KDvoid  onExit ( KDvoid );
		virtual KDvoid  draw ( KDvoid );
		virtual KDvoid  ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid  onRestart ( CCObject* pSender );
		KDvoid  onPrev    ( CCObject* pSender );
		KDvoid  onNext    ( CCObject* pSender );	

		KDvoid  openRipple ( const KDchar* szPath );
		KDvoid  freeRipple ( KDvoid );

		KDvoid  setRipple  ( KDint nIndex );

	public :

		Video*			m_pVideo;
		XMGTexture*		m_pTexture;
		XMGRipple*		m_pRipple;
		CCRect			m_tRect;
		KDint			m_nIndex;


};

#endif