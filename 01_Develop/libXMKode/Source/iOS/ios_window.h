/* --------------------------------------------------------------------------
 *
 *      File            ios_window.h
 *      Description     Windowing
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

#ifndef __ios_window_h__
#define __ios_window_h__

#include "platform.h"

#import  <Foundation/Foundation.h>
#import  <CoreLocation/CoreLocation.h>
#import	 <UIKit/UIKit.h>
#import  <QuartzCore/QuartzCore.h>
#import  <OpenGLES/EAGL.h>
#import  <OpenGLES/EAGLDrawable.h>
#import  <OpenGLES/ES1/gl.h>
#import  <OpenGLES/ES1/glext.h>
#import  <OpenGLES/ES2/gl.h>
#import  <OpenGLES/ES2/glext.h>

@interface XMLocation : CLLocationManager <CLLocationManagerDelegate>
{
    
}
@end

@interface XMMagnetometer : CLLocationManager <CLLocationManagerDelegate>
{
    
}
@end

@interface XMAccelerometer : NSObject <UIAccelerometerDelegate>
{
    
}
@end

@interface XMEGLView : UIView <UIKeyInput, UITextInput>
{
@public
	EAGLContext*         m_context;
	
	GLuint               m_frame;
	GLuint               m_color;
	GLuint               m_depth;
    GLuint               m_msaa_frame;
    GLuint               m_msaa_color;
	
    KDTouch              m_touches [ KD_TOUCH_MAX ];
    KDfloat              m_scale;
    KDbool               m_live;
    KDbool               m_visible;
    KDint                m_cx;
    KDint                m_cy;
    
    BOOL                 m_isUsedUITextField;
    NSString*            m_markedText;
    CGRect               m_caretRect;
    CGRect               m_originalRect;
    NSNotification*      m_keyboardShowNotification;
    BOOL                 m_isKeyboardShown;
}

@property(nonatomic, readonly) UITextPosition* beginningOfDocument;
@property(nonatomic, readonly) UITextPosition* endOfDocument;
@property(nonatomic, assign  ) id<UITextInputDelegate> inputDelegate;
@property(nonatomic, readonly) UITextRange*  markedTextRange;
@property(nonatomic, copy    ) NSDictionary* markedTextStyle;
@property(readwrite, copy    ) UITextRange*  selectedTextRange;
@property(nonatomic, readonly) id<UITextInputTokenizer> tokenizer;
@property(nonatomic, readonly, getter = isKeyboardShown) BOOL isKeyboardShown;
@property(nonatomic, copy) NSNotification* keyboardShowNotification;

+ (Class) layerClass;

@end

@interface XMRootView : UIViewController
{
    
}
@end

@interface XMDelegate : NSObject<UIApplicationDelegate>
{
@public
    UIWindow*            m_window;
	NSTimer*             m_timer;
    
    XMRootView*          m_root;
    XMEGLView*           m_view;
    XMLocation*          m_location;
    XMMagnetometer*      m_magnetometer;
    XMAccelerometer*     m_accelerometer;
    
    KDfloat              m_brightness;
}
@end

#endif
