/* --------------------------------------------------------------------------
 *
 *      File            CCImage_iOS.mm
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

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_CC_BEGIN

static bool _isValidFontName ( const char* fontName )
{
    bool ret = false;
    
    NSString *fontNameNS = [NSString stringWithUTF8String:fontName];
    
    for (NSString *familiName in [UIFont familyNames])
    {
        if ([familiName isEqualToString:fontNameNS])
        {
            ret = true;
            goto out;
        }
        
        for(NSString *font in [UIFont fontNamesForFamilyName: familiName])
        {
            if ([font isEqualToString: fontNameNS])
            {
                ret = true;
                goto out;
            }
        }
    }
    
    out:
    return ret;
}

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

KDbool CCImage::initWithFontName ( const KDchar* szText, KDuint uWidth, KDuint uHeight, CCAlignment eAlignMask, const KDchar* szFontName, KDuint uSize )
{
    KDbool bRet = KD_FALSE;
    do
    {
        CC_BREAK_IF ( !szText || !szFontName );
        
        NSString* str  = [NSString stringWithUTF8String:szText];
        NSString* fntName = [NSString stringWithUTF8String:szFontName];
        CGSize dim, constrainSize;
        
        constrainSize.width  = uWidth;
        constrainSize.height = uHeight;
        
        // create the font
        id font;
        font = [UIFont fontWithName:fntName size:uSize];
        if ( font )
        {
            dim = _calculateStringSize(str, font, &constrainSize);
        }
        else
        {
            fntName = _isValidFontName(szFontName) ? fntName : @"MarkerFelt-Wide";
            font = [UIFont fontWithName:fntName size:uSize];
                
            if (! font) 
            {
                font = [UIFont systemFontOfSize:uSize];
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
            // vertical alignmen
            if ( eAlignMask & kCCAlignmentTop )
            {
                startH = 0;
            }
            else if ( eAlignMask & kCCAlignmentBottom )
            {
                startH = constrainSize.height - dim.height;
            }
            else
            {
                startH = (constrainSize.height - dim.height) / 2;
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
        
        unsigned char* data = (unsigned char*) calloc ( 1, (int)(dim.width * dim.height * 4) );
        
        // draw text
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGContextRef context = CGBitmapContextCreate(data, dim.width, dim.height, 8, dim.width * 4, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        CGColorSpaceRelease(colorSpace);
        
        if ( !context )
        {
            free ( data );
            break;
        }
        
        CGContextSetRGBFillColor(context, 1, 1, 1, 1);
        CGContextTranslateCTM(context, 0.0f, dim.height);
        CGContextScaleCTM(context, 1.0f, -1.0f); //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
        UIGraphicsPushContext(context);
        
        // measure text size with specified font and determine the rectangle to draw text in
        NSTextAlignment align = (NSTextAlignment)((kCCAlignmentRight & eAlignMask) ? NSTextAlignmentRight
                                : (kCCAlignmentLeft & eAlignMask) ? NSTextAlignmentLeft
                                : NSTextAlignmentCenter );
        // normal fonts
        [str drawInRect:CGRectMake(0, startH, dim.width, dim.height) withFont:font lineBreakMode:(NSLineBreakMode)NSLineBreakByWordWrapping alignment:align];
       
        UIGraphicsPopContext();
        
        CGContextRelease(context);
        
        m_uWidth    = dim.width;
        m_uHeight   = dim.height;
        m_uBpp      = 8;
        m_bHasAlpha = KD_TRUE;
        m_bPreMulti = KD_FALSE;
        m_eFormat   = kCCTexture2DPixelFormat_A8;
        
        m_pData = (KDubyte*) kdCalloc( 1, m_uWidth * m_uHeight );
        for ( KDuint y = 0; y < m_uHeight; y++ )
        {
            for ( KDuint x = 0; x < m_uWidth; x++ )
            {
                m_pData [ m_uWidth * y + x ] = data [ ( m_uWidth * y + x ) * 4 + 3 ];
            }
        }
        
        free ( data );
        
        bRet = KD_TRUE;
        
    } while (0);
    
    return bRet;
}

NS_CC_END
