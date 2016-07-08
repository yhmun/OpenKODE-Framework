/* -----------------------------------------------------------------------------------
*
*      File            CCImage_Win32.cpp
*      Ported By       Young-Hwan Mun
*      Contact         xmsoft77@gmail.com
*
* -----------------------------------------------------------------------------------
*
*      Copyright (c) 2010-2014 XMSoft
*      Copyright (c) 2010-2013 cocos2d-x.org
*
*         http://www.cocos2d-x.org
*
* -----------------------------------------------------------------------------------
*
*     Permission is hereby granted, free of charge, to any person obtaining a copy
*     of this software and associated documentation files (the "Software"), to deal
*     in the Software without restriction, including without limitation the rights
*     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*     copies of the Software, and to permit persons to whom the Software is
*     furnished to do so, subject to the following conditions:
*
*     The above copyright notice and this permission notice shall be included in
*     all copies or substantial portions of the Software.
*
*     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*     THE SOFTWARE.
*
* ----------------------------------------------------------------------------------- */

#include "platform/CCImage.h"
#include "platform/CCFileUtils.h"
#include "2d/CCConfiguration.h"
#include "ccConfig.h"
#include "platform.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

NS_CC_BEGIN

/**
 *	@brief    A memory DC which uses to draw text on bitmap.
 */

class BitmapDC
{
	public:

		BitmapDC ( HWND hWnd = KD_NULL )
		{
			m_hBmp  = NULL;
			m_hFont = (HFONT) GetStockObject ( DEFAULT_GUI_FONT );
			m_hWnd  = hWnd;
			HDC hDC = GetDC ( hWnd );
			m_hDC   = CreateCompatibleDC ( hDC );
			ReleaseDC ( hWnd, hDC );			
		}

		virtual ~BitmapDC ( KDvoid )
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
						KDchar*  szPath = xmGetNativePath(FileUtils::getInstance()->fullPathForFilename(sFontName.c_str()).c_str(), 0);
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

		KDint drawText(const KDchar* pszText, SIZE& tSize, Image::TextAlign eAlign)
		{
			KDint    nRet = 0;
			wchar_t* pwszBuffer = 0;
			do 
			{
				CC_BREAK_IF ( !pszText );

				DWORD  dwFmt = DT_WORDBREAK;				
				DWORD  dwHoriFlag = (KDint)eAlign & 0x0f;
				DWORD  dwVertFlag = ((KDint)eAlign & 0xf0) >> 4;

				switch (dwHoriFlag)
				{
				case 1: // left
					dwFmt |= DT_LEFT;
					break;
				case 2: // right
					dwFmt |= DT_RIGHT;
					break;
				case 3: // center
					dwFmt |= DT_CENTER;
					break;
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
					if (1 != dwHoriFlag					// and text isn't align to left
						&& tNewSize.cx < tSize.cx)		// and text's width less then content width,
					{									// then need adjust offset of X.
						nOffsetX = (2 == dwHoriFlag) ? tSize.cx - tNewSize.cx		// align to right
							: (tSize.cx - tNewSize.cx) / 2;							// align to center
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
						nOffsetY = (2 == dwVertFlag) ? tSize.cy - tNewSize.cy		// align to bottom
							: (3 == dwVertFlag) ? (tSize.cy - tNewSize.cy) / 2		// align to middle
							: 0;													// align to top
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

		friend class Image;

		HFONT					m_hFont;
		HWND					m_hWnd;
		std::string				m_sFontPath;
};

static BitmapDC& sharedBitmapDC ( KDvoid )
{
    static BitmapDC  sBitmapDC;

    return sBitmapDC;
}

//////////////////////////////////////////////////////////////////////////
// Impliment CCImage
//////////////////////////////////////////////////////////////////////////

KDbool Image::initWithPlatformFont (	const KDchar*	szText,
										KDint           nWidth,
										KDint           nHeight,
										TextAlign       eAlignMask,
										const KDchar*	szFontName,
										KDint			nSize,
										KDfloat			fTextTintR,
										KDfloat			fTextTintG,
										KDfloat			fTextTintB,
										KDbool			bShadow,
										KDfloat			fShadowOffsetX,
										KDfloat			fShadowOffsetY,
										KDfloat			fShadowOpacity,
										KDfloat			fShadowBlur,
										KDbool			bStroke,
										KDfloat			fStrokeR,
										KDfloat			fStrokeG,
										KDfloat			fStrokeB,
										KDfloat			fStrokeSize )
{
    KDbool    bRet  = KD_FALSE;
    do 
    {
        CC_BREAK_IF ( !szText );       

        BitmapDC&  tDC = sharedBitmapDC ( );

        if ( !tDC.setFont ( szFontName, nSize ) )
        {
            CCLOG ( "Can't found font(%s), use system default", szFontName );
        }

        // draw text
        SIZE  tSize = { nWidth, nHeight };
        CC_BREAK_IF ( !tDC.drawText ( szText, tSize, eAlignMask ) );

		m_nDataLen = tSize.cx * tSize.cy * 4;
		m_pData = (KDubyte*)kdMalloc(m_nDataLen);
		CC_BREAK_IF(!m_pData);

        struct
        {
            BITMAPINFOHEADER  bmiHeader;
            KDint             mask[4];
        } tBI = { 0 };

        tBI.bmiHeader.biSize = sizeof ( tBI.bmiHeader );
        CC_BREAK_IF ( !GetDIBits ( tDC.getDC ( ), tDC.getBitmap ( ), 0, 0, NULL, (LPBITMAPINFO) &tBI, DIB_RGB_COLORS ) );

        m_nWidth				 = tSize.cx;
        m_nHeight				 = tSize.cy;
		m_bHasPremultipliedAlpha = KD_FALSE;
		m_eRenderFormat			 = Texture2D::PixelFormat::RGBA8888;

        // copy pixed data
        tBI.bmiHeader.biHeight = ( tBI.bmiHeader.biHeight > 0 )
           ? - tBI.bmiHeader.biHeight : tBI.bmiHeader.biHeight;
		GetDIBits(tDC.getDC(), tDC.getBitmap(), 0, m_nHeight, m_pData, (LPBITMAPINFO)&tBI, DIB_RGB_COLORS);

        // change pixel's alpha value to 255, when it's RGB != 0
        COLORREF*  pPixel = NULL;
		for (KDint y = 0; y < m_nHeight; ++y)
        {
			pPixel = (COLORREF*)m_pData + y * m_nWidth;
			for (KDint x = 0; x < m_nWidth; ++x)
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