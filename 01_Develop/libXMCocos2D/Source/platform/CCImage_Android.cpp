/* --------------------------------------------------------------------------
 *
 *      File            CCImage_Android.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010      cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com	
 * 
 * --------------------------------------------------------------------------
 * 
 *      This m_pLibrary is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This m_pLibrary is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this m_pLibrary in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "support/CCImage.h"
#include "support/CCFileUtils.h"
#include "support/CCConfiguration.h"
#include "platform.h"

NS_CC_BEGIN

class BitmapDC
{
	public :

		BitmapDC ( KDvoid )
		{
			m_pData   = NULL;
			m_nWidth  = 0;
			m_nHeight = 0;
		}

		jmethodID getMethodID ( const char* szMethod, const char* szParam )
		{
			jmethodID  ret = 0;

			m_pVM  = (JavaVM*) xmGetWindow ( );
			m_pEnv = 0;

			if ( m_pVM->GetEnv ( (void**) &m_pEnv, JNI_VERSION_1_4 ) != JNI_OK )
			{
				kdLogMessagefKHR ( "Failed to get the environment using GetEnv()" );
				return 0;
			}

			if ( m_pVM->AttachCurrentThread ( &m_pEnv, 0 ) < 0 )
			{
				kdLogMessagefKHR ( "Failed to get the environment using AttachCurrentThread()" );
				return 0;
			}

			m_pCls = m_pEnv->FindClass ( "XMKode/XMNative" );
			if ( !m_pCls )
			{
				kdLogMessagefKHR ( "Failed to find class of XMKode/XMNative" );
				return 0;
			}

			if ( m_pEnv != 0 && m_pCls != 0 )
			{
				ret = m_pEnv->GetStaticMethodID ( m_pCls, szMethod, szParam );
			}

			if ( !ret )
			{
				kdLogMessagefKHR ( "get method id of %s error", szMethod );
			}

			return ret;
		}

		KDbool getBitmapFromJava ( const char* szText, KDint nWidth, KDint nHeight, CCAlignment eAlignMask, const KDchar* szFontName, KDfloat fFontSize )
		{
			jmethodID  pMethod = getMethodID ( "createTextBitmap", "(Ljava/lang/String;Ljava/lang/String;IIII)V" );
			jstring    jszText = m_pEnv->NewStringUTF ( szText );
			jstring    jszFont = m_pEnv->NewStringUTF ( szFontName );

			m_pData = 0;

			if ( pMethod )
			{
				m_pEnv->CallStaticVoidMethod ( m_pCls, pMethod, jszText, jszFont, (KDint) fFontSize, eAlignMask, nWidth, nHeight );
			}

			m_pEnv->DeleteLocalRef ( jszText );
			m_pEnv->DeleteLocalRef ( jszFont );
			m_pEnv->DeleteLocalRef ( m_pCls );

			return m_pData ? KD_TRUE : KD_FALSE;
		}

	public :

		KDint		m_nWidth;
		KDint		m_nHeight;
		KDubyte*	m_pData;

		JavaVM*     m_pVM;
		JNIEnv*     m_pEnv;
		jclass      m_pCls;
};

static BitmapDC& sharedBitmapDC ( KDvoid )
{
    static BitmapDC  s_BitmapDC;

    return s_BitmapDC;
}

KDbool CCImage::initWithFontName ( const KDchar* szText, KDuint uWidth, KDuint uHeight, CCAlignment eAlignMask, const KDchar* szFontName, KDuint uSize )
{
	KDbool  bRet = KD_FALSE;
	do
	{
		CC_BREAK_IF ( !szText );

		BitmapDC&  tDC = sharedBitmapDC ( );

		CC_BREAK_IF ( !tDC.getBitmapFromJava ( szText, uWidth, uHeight, eAlignMask, szFontName, uSize ) );

		m_pData		= tDC.m_pData;
        m_uWidth	= tDC.m_nWidth;
        m_uHeight	= tDC.m_nHeight;
        m_bHasAlpha	= KD_TRUE;
        m_bPreMulti	= KD_FALSE;
        m_uBpp		= 8;
		m_eFormat	= kCCTexture2DPixelFormat_A8;

		bRet = KD_TRUE;

	} while ( 0 );

	return bRet;
}

NS_CC_END

// this method is called by Cocos2dxBitmap
extern "C"
{
    //
    // this method is called by java code to init width, height and pixels data
    //
    void JNICALL Java_XMKode_XMNative_InitBitmapDC ( JNIEnv* env, jobject thiz, int width, int height, jbyteArray pixels )
    {
		cocos2d::BitmapDC&  tDC = cocos2d::sharedBitmapDC ( );
		KDint     size = width * height * 4;
		KDubyte*  data = (KDubyte*) kdMalloc ( size );

		tDC.m_nWidth  = width;
		tDC.m_nHeight = height;

		env->GetByteArrayRegion ( pixels, 0, size, (jbyte*) data );

        tDC.m_pData = (KDubyte*) kdMalloc ( width * height );
        for ( KDuint y = 0; y < height; y++ )
        {
            for ( KDuint x = 0; x < width; x++ )
            {
                tDC.m_pData [ width * y + x ] = data [ ( width * y + x ) * 4 ];
            }
        }

		kdFree ( data );
    }
};