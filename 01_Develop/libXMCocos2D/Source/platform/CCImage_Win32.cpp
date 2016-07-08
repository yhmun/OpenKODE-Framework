/* --------------------------------------------------------------------------
 *
 *      File            CCImage_Win32.cpp
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
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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
#include "ccConfig.h"
#include "platform.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

NS_CC_BEGIN

/**
 *	@brief    A memory DC which uses to draw text on bitmap.
 */
#ifndef DEFAULT_GUI_FONT
#define DEFAULT_GUI_FONT 18
#endif

class BitmapDC
{
	public:

		BitmapDC ( HWND hWnd = NULL )
		{
			m_hBmp  = NULL;
			m_hFont = (HFONT) GetStockObject ( DEFAULT_GUI_FONT );
			m_hWnd  = hWnd;
			HDC hDC = GetDC ( hWnd );
			m_hDC   = CreateCompatibleDC ( hDC );
			ReleaseDC ( hWnd, hDC );			
		}

		~BitmapDC ( KDvoid )
		{
			prepareBitmap ( 0, 0 );

			if ( m_hDC )
			{
				DeleteDC ( m_hDC );
			}

			HFONT  hDefFont = (HFONT) GetStockObject ( DEFAULT_GUI_FONT );
			if ( hDefFont != m_hFont )
			{
				DeleteObject ( m_hFont );
				m_hFont = hDefFont;
			}

			// release temp font resource	
			if ( m_sFontPath.size ( ) > 0 )
			{
				wchar_t*  pwszBuffer = utf8ToUtf16 ( m_sFontPath );
				if ( pwszBuffer )
				{
					RemoveFontResource ( pwszBuffer );
					SendMessage ( m_hWnd, WM_FONTCHANGE, 0, 0 );
					delete[] pwszBuffer;
					pwszBuffer = NULL;
				}
			}
		}

		wchar_t*  utf8ToUtf16 ( std::string sString )
		{
			wchar_t* pwszBuffer = NULL;
			do 
			{
				if ( sString.size ( ) < 0 )
				{
					break;
				}

				// utf-8 to utf-16
				KDint  nLen    = sString.size ( );
				KDint  nBufLen = nLen + 1;			
				pwszBuffer = new wchar_t [ nBufLen ];
				CC_BREAK_IF ( !pwszBuffer );
				kdMemset ( pwszBuffer, 0, nBufLen );
				nLen = MultiByteToWideChar ( CP_UTF8, 0, sString.c_str ( ), nLen, pwszBuffer, nBufLen );		
				pwszBuffer [ nLen ] = '\0';
			} while ( 0 );

			return pwszBuffer;

		}

		KDbool setFont ( const KDchar* pFontName = NULL, KDint nSize = 0 )
		{
			KDbool bRet = KD_FALSE;
			do 
			{
				std::string  sFontName = pFontName;
				std::string  sFontPath;
				HFONT        hDefFont = (HFONT) GetStockObject ( DEFAULT_GUI_FONT );
				LOGFONTA     tNewFont = {0};
				LOGFONTA     tOldFont = {0};
				GetObjectA ( hDefFont, sizeof ( tNewFont ), &tNewFont );
				if ( sFontName.c_str ( ) )
				{    
					// create font from ttf file
					KDint nFindttf = sFontName.find ( ".ttf" );
					KDint nFindTTF = sFontName.find ( ".TTF" );
					if ( nFindttf >= 0 || nFindTTF >= 0 )
					{
						KDchar*  szPath = xmGetNativePath ( CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( sFontName.c_str ( ) ).c_str ( ), 0 );
						sFontPath = szPath;
						kdFree ( szPath );

						KDint  nFindPos = sFontName.rfind ( "/" );
						sFontName = &sFontName [ nFindPos + 1 ];
						nFindPos = sFontName.rfind ( "." );
						sFontName = sFontName.substr ( 0, nFindPos );                
					}
					
					tNewFont.lfCharSet = DEFAULT_CHARSET;
					kdStrcpy_s ( tNewFont.lfFaceName, LF_FACESIZE, sFontName.c_str ( ) );
				}

				if ( nSize )
				{
					tNewFont.lfHeight = -nSize;
				}
				GetObjectA ( m_hFont, sizeof ( tOldFont ), &tOldFont );

				if ( tOldFont.lfHeight == tNewFont.lfHeight
					&& !kdStrcpy ( tOldFont.lfFaceName, tNewFont.lfFaceName ) )
				{
					// already has the font 
					bRet = KD_TRUE;
					break;
				}

				// delete old font
				if ( m_hFont != hDefFont )
				{
					DeleteObject ( m_hFont );

					// release old font register
					if ( m_sFontPath.size ( ) > 0 )
					{
						wchar_t*  pwszBuffer = utf8ToUtf16 ( m_sFontPath );
						if ( pwszBuffer )
						{
							if ( RemoveFontResource ( pwszBuffer ) )
							{
								SendMessage ( m_hWnd, WM_FONTCHANGE, 0, 0 );
							}						
							delete [] pwszBuffer;
							pwszBuffer = NULL;
						}
					}
					sFontPath.size ( ) > 0 ? ( m_sFontPath = sFontPath ) : ( m_sFontPath.clear ( ) );

					// register temp font
					if ( m_sFontPath.size ( ) > 0 )
					{
						wchar_t*  pwszBuffer = utf8ToUtf16 ( m_sFontPath );
						if ( pwszBuffer )
						{
							if ( AddFontResource ( pwszBuffer ) )
							{
								SendMessage ( m_hWnd, WM_FONTCHANGE, 0, 0 );
							}						
							delete [] pwszBuffer;
							pwszBuffer = NULL;
						}
					}
				}
				m_hFont = NULL;

				// disable Cleartype
				tNewFont.lfQuality = ANTIALIASED_QUALITY;

				// create new font
				m_hFont = CreateFontIndirectA ( &tNewFont );
				if ( !m_hFont )
				{
					// create failed, use default font
					m_hFont = hDefFont;
					break;
				}
	            
				bRet = KD_TRUE;

			} while ( 0 );

			return bRet;
		}

		SIZE  sizeWithText ( const wchar_t* pszText, KDint nLen, DWORD dwFmt, LONG nWidthLimit )
		{
			SIZE  tRet = { 0 };
			do 
			{
				CC_BREAK_IF ( !pszText || nLen <= 0 );

				RECT  tRC = { 0, 0, 0, 0 };
				DWORD dwCalcFmt = DT_CALCRECT;

				if ( nWidthLimit > 0 )
				{
					tRC.right = nWidthLimit;
					dwCalcFmt |= DT_WORDBREAK
						| ( dwFmt & DT_CENTER )
						| ( dwFmt & DT_RIGHT  );
				}

				// use current font to measure text extent
				HGDIOBJ  hOld = SelectObject ( m_hDC, m_hFont );

				// measure text size
				DrawTextW ( m_hDC, pszText, nLen, &tRC, dwCalcFmt );
				SelectObject ( m_hDC, hOld );

				tRet.cx = tRC.right;
				tRet.cy = tRC.bottom;

			} while ( 0 );

			return tRet;
		}

		KDbool prepareBitmap ( KDint nWidth, KDint nHeight )
		{
			// release bitmap
			if ( m_hBmp )
			{
				DeleteObject ( m_hBmp );
				m_hBmp = NULL;
			}
			if ( nWidth > 0 && nHeight > 0 )
			{
				m_hBmp = CreateBitmap ( nWidth, nHeight, 1, 32, NULL );
				if ( !m_hBmp )
				{
					return KD_FALSE;
				}
			}

			return KD_TRUE;
		}

		KDint drawText ( const KDchar* pszText, SIZE& tSize, CCAlignment eAlign )
		{
			KDint    nRet = 0;
			wchar_t* pwszBuffer = 0;
			do 
			{
				CC_BREAK_IF ( !pszText );

				DWORD  dwFmt = DT_WORDBREAK;				
				
				if ( eAlign & kCCAlignmentLeft )
				{
					dwFmt |= DT_LEFT;	
				}
				else if ( eAlign & kCCAlignmentRight )
				{
					dwFmt |= DT_RIGHT;
				}
				else
				{
					dwFmt |= DT_CENTER;
				}

				KDint nLen = kdStrlen ( pszText );

				// utf-8 to utf-16
				KDint  nBufLen  = nLen + 1;
				pwszBuffer = new wchar_t [ nBufLen ];
				CC_BREAK_IF ( !pwszBuffer );

				kdMemset ( pwszBuffer, 0, sizeof ( wchar_t ) * nBufLen );
				nLen = MultiByteToWideChar ( CP_UTF8, 0, pszText, nLen, pwszBuffer, nBufLen );

				SIZE  tNewSize = sizeWithText ( pwszBuffer, nLen, dwFmt, tSize.cx );
				RECT  tRCText = { 0 };

				// if content width is 0, use text size as content size
				if ( tSize.cx <= 0 )
				{
					tSize = tNewSize;
					tRCText.right  = tNewSize.cx;
					tRCText.bottom = tNewSize.cy;
				}
				else
				{
					LONG  nOffsetX = 0;
					LONG  nOffsetY = 0;
					tRCText.right = tNewSize.cx; // store the text width to rectangle

					// calculate text horizontal offset
					if ( !( eAlign & kCCAlignmentLeft )		// and text isn't align to left
						 && tNewSize.cx < tSize.cx )	// and text's width less then content width,
					{									// then need adjust offset of X.
						nOffsetX = ( eAlign & kCCAlignmentRight ) ? tSize.cx - tNewSize.cx	// align to right
							: ( tSize.cx - tNewSize.cx ) / 2;								// align to center
					}

					// if content height is 0, use text height as content height
					// else if content height less than text height, use content height to draw text
					if ( tSize.cy <= 0 )
					{
						tSize.cy = tNewSize.cy;
						dwFmt   |= DT_NOCLIP;
						tRCText.bottom = tNewSize.cy; // store the text height to rectangle
					}
					else if ( tSize.cy < tNewSize.cy )
					{
						// content height larger than text height need, clip text to rect
						tRCText.bottom = tSize.cy;
					}
					else
					{
						tRCText.bottom = tNewSize.cy; // store the text height to rectangle

						// content larger than text, need adjust vertical position
						dwFmt |= DT_NOCLIP;

						// calculate text vertical offset
						nOffsetY = ( eAlign & kCCAlignmentBottom ) ? tSize.cy - tNewSize.cy			// align to bottom
							: ( eAlign & kCCAlignmentTop ) ? 0 										// align to top
							: ( tSize.cy - tNewSize.cy ) / 2;										// align to middle
					}

					if ( nOffsetX || nOffsetY )
					{
						OffsetRect ( &tRCText, nOffsetX, nOffsetY );
					}
				}

				CC_BREAK_IF ( !prepareBitmap ( tSize.cx, tSize.cy ) );

				// draw text
				HGDIOBJ  hOldFont = SelectObject ( m_hDC, m_hFont );
				HGDIOBJ  hOldBmp  = SelectObject ( m_hDC, m_hBmp );
	            
				SetBkMode ( m_hDC, TRANSPARENT );
				SetTextColor ( m_hDC, RGB ( 255, 255, 255 ) ); // white color

				// draw text
				nRet = DrawTextW ( m_hDC, pwszBuffer, nLen, &tRCText, dwFmt );
				//DrawTextA ( m_hDC, pszText, nLen, &tRCText, dwFmt );

				SelectObject ( m_hDC, hOldBmp );
				SelectObject ( m_hDC, hOldFont );

			} while ( 0 );

			CC_SAFE_DELETE_ARRAY ( pwszBuffer );

			return nRet;
		}

		CC_SYNTHESIZE_READONLY ( HDC    , m_hDC , DC );
		CC_SYNTHESIZE_READONLY ( HBITMAP, m_hBmp, Bitmap );

	private :

		friend class CCImage;

		HFONT		m_hFont;
		HWND		m_hWnd;
		std::string m_sFontPath;
};

static BitmapDC& sharedBitmapDC ( KDvoid )
{
    static BitmapDC  sBitmapDC;

    return sBitmapDC;
}

//////////////////////////////////////////////////////////////////////////
// Impliment CCImage
//////////////////////////////////////////////////////////////////////////

KDbool CCImage::initWithFontName ( const KDchar* szText, KDuint uWidth, KDuint uHeight, CCAlignment eAlignMask, const KDchar* szFontName, KDuint uSize )
{
    KDbool    bRet  = KD_FALSE;
    KDubyte*  pData = 0;
    do 
    {
        CC_BREAK_IF ( !szText );       

        BitmapDC&  tDC = sharedBitmapDC ( );

        if ( !tDC.setFont ( szFontName, uSize ) )
        {
            CCLOG ( "Can't found font(%s), use system default", szFontName );
        }

        // draw text
        SIZE  tSize = { uWidth, uHeight };
        CC_BREAK_IF ( !tDC.drawText ( szText, tSize, eAlignMask ) );

        pData = (KDubyte*) kdMalloc ( tSize.cx * tSize.cy * 4 );
        CC_BREAK_IF ( !pData );

        struct
        {
            BITMAPINFOHEADER  bmiHeader;
            KDint             mask[4];
        } tBI = { 0 };

        tBI.bmiHeader.biSize = sizeof ( tBI.bmiHeader );
        CC_BREAK_IF ( !GetDIBits ( tDC.getDC ( ), tDC.getBitmap ( ), 0, 0, NULL, (LPBITMAPINFO) &tBI, DIB_RGB_COLORS ) );

        m_uWidth    = tSize.cx;
        m_uHeight   = tSize.cy;
        m_bHasAlpha = KD_TRUE;
        m_bPreMulti = KD_FALSE;
        m_pData     = pData;
        m_uBpp      = 32;
		m_eFormat   = kCCTexture2DPixelFormat_RGBA8888;

        // copy pixed data
        tBI.bmiHeader.biHeight = ( tBI.bmiHeader.biHeight > 0 )
           ? - tBI.bmiHeader.biHeight : tBI.bmiHeader.biHeight;
        GetDIBits ( tDC.getDC ( ), tDC.getBitmap ( ), 0, m_uHeight, m_pData, (LPBITMAPINFO) &tBI, DIB_RGB_COLORS );

        // change pixel's alpha value to 255, when it's RGB != 0
        COLORREF*  pPixel = NULL;
        for ( KDuint y = 0; y < m_uHeight; ++y )
        {
            pPixel = (COLORREF*) m_pData + y * m_uWidth;
            for ( KDuint x = 0; x < m_uWidth; ++x )
            {
                COLORREF&  dwColor = *pPixel;
                if ( GetRValue ( dwColor ) || GetGValue ( dwColor ) || GetBValue ( dwColor ) )
                {
                    dwColor |= 0xff000000;
                }
                ++pPixel;
            }
        }
 
        bRet = KD_TRUE;

    } while ( 0 );

	return bRet;
}

NS_CC_END

#endif	// CC_TARGET_PLATFORM == CC_PLATFORM_WIN32