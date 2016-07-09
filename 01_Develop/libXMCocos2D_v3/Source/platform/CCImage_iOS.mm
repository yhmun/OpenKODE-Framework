/* -----------------------------------------------------------------------------------
 *
 *      File            CCImage_iOS.mm
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 * -----------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft
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
#include "platform/CCCommon.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include <string>

typedef struct
{
    unsigned int height;
    unsigned int width;
    bool         isPremultipliedAlpha;
    bool         hasShadow;
    CGSize       shadowOffset;
    float        shadowBlur;
    float        shadowOpacity;
    bool         hasStroke;
    float        strokeColorR;
    float        strokeColorG;
    float        strokeColorB;
    float        strokeSize;
    float        tintColorR;
    float        tintColorG;
    float        tintColorB;
    
    unsigned char*  data;
    
} tImageInfo;

static CGSize _calculateStringSize(NSString *str, id font, CGSize *constrainSize)
{
    NSArray *listItems = [str componentsSeparatedByString: @"\n"];
    CGSize dim = CGSizeZero;
    CGSize textRect = CGSizeZero;
    textRect.width = constrainSize->width > 0 ? constrainSize->width
    : 0x7fffffff;
    textRect.height = constrainSize->height > 0 ? constrainSize->height
    : 0x7fffffff;
    
    
    for (NSString *s in listItems)
    {
        CGSize tmp = [s sizeWithFont:font constrainedToSize:textRect];
        
        if (tmp.width > dim.width)
        {
            dim.width = tmp.width;
        }
        
        dim.height += tmp.height;
    }
    
    return dim;
}

// refer Image::ETextAlign
#define ALIGN_TOP    1
#define ALIGN_CENTER 3
#define ALIGN_BOTTOM 2

static bool _initWithString(const char * pText, cocos2d::Image::TextAlign eAlign, const char * pFontName, int nSize, tImageInfo* pInfo)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! pText || ! pInfo);
        
        NSString * str          = [NSString stringWithUTF8String:pText];
        NSString * fntName      = [NSString stringWithUTF8String:pFontName];
        
        CGSize dim, constrainSize;
        
        constrainSize.width     = pInfo->width;
        constrainSize.height    = pInfo->height;
        
        // On iOS custom fonts must be listed beforehand in the App info.plist (in order to be usable) and referenced only the by the font family name itself when
        // calling [UIFont fontWithName]. Therefore even if the developer adds 'SomeFont.ttf' or 'fonts/SomeFont.ttf' to the App .plist, the font must
        // be referenced as 'SomeFont' when calling [UIFont fontWithName]. Hence we strip out the folder path components and the extension here in order to get just
        // the font family name itself. This stripping step is required especially for references to user fonts stored in CCB files; CCB files appear to store
        // the '.ttf' extensions when referring to custom fonts.
        fntName = [[fntName lastPathComponent] stringByDeletingPathExtension];
        
        // create the font
        id font = [UIFont fontWithName:fntName size:nSize];
        
        if (font)
        {
            dim = _calculateStringSize(str, font, &constrainSize);
        }
        else
        {
            if (!font)
            {
                font = [UIFont systemFontOfSize:nSize];
            }
            
            if (font)
            {
                dim = _calculateStringSize(str, font, &constrainSize);
            }
        }
        
        CC_BREAK_IF(! font);
        
        // compute start point
        int startH = 0;
        if (constrainSize.height > dim.height)
        {
            // vertical alignment
            unsigned int vAlignment = ((int)eAlign >> 4) & 0x0F;
            if (vAlignment == ALIGN_TOP)
            {
                startH = 0;
            }
            else if (vAlignment == ALIGN_CENTER)
            {
                startH = (constrainSize.height - dim.height) / 2;
            }
            else
            {
                startH = constrainSize.height - dim.height;
            }
        }
        
        // adjust text rect
        if (constrainSize.width > 0 && constrainSize.width > dim.width)
        {
            dim.width = constrainSize.width;
        }
        if (constrainSize.height > 0 && constrainSize.height > dim.height)
        {
            dim.height = constrainSize.height;
        }
        
        
        // compute the padding needed by shadow and stroke
        float shadowStrokePaddingX = 0.0f;
        float shadowStrokePaddingY = 0.0f;
        
        if ( pInfo->hasStroke )
        {
            shadowStrokePaddingX = ceilf(pInfo->strokeSize);
            shadowStrokePaddingY = ceilf(pInfo->strokeSize);
        }
        
        if ( pInfo->hasShadow )
        {
            shadowStrokePaddingX = std::max(shadowStrokePaddingX, (float)fabs(pInfo->shadowOffset.width));
            shadowStrokePaddingY = std::max(shadowStrokePaddingY, (float)fabs(pInfo->shadowOffset.height));
        }
        
        // add the padding (this could be 0 if no shadow and no stroke)
        dim.width  += shadowStrokePaddingX;
        dim.height += shadowStrokePaddingY;
        
        
        unsigned char* data = new unsigned char[(int)(dim.width * dim.height * 4)];
        memset(data, 0, (int)(dim.width * dim.height * 4));
        
        // draw text
        CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
        CGContextRef context        = CGBitmapContextCreate(data,
                                                            dim.width,
                                                            dim.height,
                                                            8,
                                                            (int)(dim.width) * 4,
                                                            colorSpace,
                                                            kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        if (!context)
        {
            CGColorSpaceRelease(colorSpace);
            delete[] data;
            break;
        }
        
        // text color
        CGContextSetRGBFillColor(context, pInfo->tintColorR, pInfo->tintColorG, pInfo->tintColorB, 1);
        // move Y rendering to the top of the image
        CGContextTranslateCTM(context, 0.0f, (dim.height - shadowStrokePaddingY) );
        CGContextScaleCTM(context, 1.0f, -1.0f); //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
        
        // store the current context
        UIGraphicsPushContext(context);
        
        // measure text size with specified font and determine the rectangle to draw text in
        unsigned uHoriFlag = (int)eAlign & 0x0f;
        UITextAlignment align = (UITextAlignment)((2 == uHoriFlag) ? UITextAlignmentRight
                                                  : (3 == uHoriFlag) ? UITextAlignmentCenter
                                                  : UITextAlignmentLeft);
        
        
        // take care of stroke if needed
        if ( pInfo->hasStroke )
        {
            CGContextSetTextDrawingMode(context, kCGTextFillStroke);
            CGContextSetRGBStrokeColor(context, pInfo->strokeColorR, pInfo->strokeColorG, pInfo->strokeColorB, 1);
            CGContextSetLineWidth(context, pInfo->strokeSize);
        }
        
        // take care of shadow if needed
        if ( pInfo->hasShadow )
        {
            CGSize offset;
            offset.height = pInfo->shadowOffset.height;
            offset.width  = pInfo->shadowOffset.width;
            CGFloat shadowColorValues[] = {0, 0, 0, pInfo->shadowOpacity};
            CGColorRef shadowColor = CGColorCreate (colorSpace, shadowColorValues);
            
            CGContextSetShadowWithColor(context, offset, pInfo->shadowBlur, shadowColor);
            
            CGColorRelease (shadowColor);
        }
        
        CGColorSpaceRelease(colorSpace);
        
        
        // normal fonts
        //if( [font isKindOfClass:[UIFont class] ] )
        //{
        //    [str drawInRect:CGRectMake(0, startH, dim.width, dim.height) withFont:font lineBreakMode:(UILineBreakMode)UILineBreakModeWordWrap alignment:align];
        //}
        //else // ZFont class
        //{
        //    [FontLabelStringDrawingHelper drawInRect:str rect:CGRectMake(0, startH, dim.width, dim.height) withZFont:font lineBreakMode:(UILineBreakMode)UILineBreakModeWordWrap
        ////alignment:align];
        //}
        
        
        
        // compute the rect used for rendering the text
        // based on wether shadows or stroke are enabled
        
        float textOriginX  = 0.0;
        float textOrigingY = 0.0;
        
        float textWidth    = dim.width  - shadowStrokePaddingX;
        float textHeight   = dim.height - shadowStrokePaddingY;
        
        
        if ( pInfo->shadowOffset.width < 0 )
        {
            textOriginX = shadowStrokePaddingX;
        }
        else
        {
            textOriginX = 0.0;
        }
        
        if (pInfo->shadowOffset.height > 0)
        {
            textOrigingY = startH;
        }
        else
        {
            textOrigingY = startH - shadowStrokePaddingY;
        }
        
        CGRect rect = CGRectMake(textOriginX, textOrigingY, textWidth, textHeight);
        
        CGContextBeginTransparencyLayerWithRect(context, rect, NULL);
        // actually draw the text in the context
		// XXX: ios7 casting
        [str drawInRect: rect withFont:font lineBreakMode:NSLineBreakByWordWrapping alignment:(NSTextAlignment)align];
        
        CGContextEndTransparencyLayer(context);
        
        // pop the context
        UIGraphicsPopContext();
        
        // release the context
        CGContextRelease(context);
        
        // output params
        pInfo->data                 = data;
        pInfo->isPremultipliedAlpha = true;
        pInfo->width                = dim.width;
        pInfo->height               = dim.height;
        bRet                        = true;
        
    } while (0);
    
    return bRet;
}

NS_CC_BEGIN

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
    
    
    
    tImageInfo info = {0};
    info.width                  = nWidth;
    info.height                 = nHeight;
    info.hasShadow              = shadow;
    info.shadowOffset.width     = fShadowOffsetX;
    info.shadowOffset.height    = fShadowOffsetY;
    info.shadowBlur             = fShadowBlur;
    info.shadowOpacity          = fShadowOpacity;
    info.hasStroke              = bStroke;
    info.strokeColorR           = fStrokeR;
    info.strokeColorG           = fStrokeG;
    info.strokeColorB           = fStrokeB;
    info.strokeSize             = fStrokeSize;
    info.tintColorR             = fTextTintR;
    info.tintColorG             = fTextTintG;
    info.tintColorB             = fTextTintB;
    
    
    if (! _initWithString ( szText, eAlignMask, szFontName, nSize, &info ) )
    {
        return false;
    }
    m_nHeight       = (short)info.height;
    m_nWidth        = (short)info.width;
    m_eRenderFormat = Texture2D::PixelFormat::RGBA8888;
    m_bPreMulti     = info.isPremultipliedAlpha;
    m_pData         = info.data;
    m_nDataLen      = m_nWidth * m_nHeight * 4;
    
    return true;
}

NS_CC_END

