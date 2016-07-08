/* -----------------------------------------------------------------------------------
 *
 *      File            CCEGLView.cpp
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

#include "Precompiled.h"
#include "support/CCEGLView.h"
#include "support/CCDirector.h"
#include "support/CCNotificationCenter.h"
#include "cocoa/CCSet.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCInteger.h"
#include "cocoa/CCAutoreleasePool.h"
#include "textures/CCTextureCache.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouch.h"
#include "text_input_node/CCIMEDispatcher.h"
#include "accelerometer_dispatcher/CCAccelerometer.h"
#include "keypad_dispatcher/CCKeypadDispatcher.h"
#include "event_dispatcher/CCEventDispatcher.h"
#include "CCApplication.h"
#include "ccConfig.h"

NS_CC_BEGIN

static KDbool			l_bIsRunning = KD_FALSE;
static CCEGLView*		l_pEGLView	 = KD_NULL;

static CCSize			l_tDeviceSize;

static CCDictionary		l_aTouchesDict;
static KDbool			l_aTouches [ CC_MAX_TOUCHES ] = { KD_FALSE };

CCEGLView* CCEGLView::sharedOpenGLView ( KDvoid )
{
	if ( !l_pEGLView )
	{
		l_pEGLView = new CCEGLView ( );
	}

	return l_pEGLView;
}

CCEGLView::CCEGLView ( KDvoid )
{
	m_fScaleY			= 1.0f;
	m_fScaleX			= 1.0f;
	m_eResolutionPolicy = kResolutionExactFit;
}

KDvoid CCEGLView::setDesignResolutionSize ( const CCSize& tSize )
{
	this->setDesignResolutionSize ( tSize.cx, tSize.cy, m_eResolutionPolicy );
}

KDvoid CCEGLView::setDesignResolutionSize ( const CCSize& tSize, ResolutionPolicy eResolutionPolicy )
{
	this->setDesignResolutionSize ( tSize.cx, tSize.cy, eResolutionPolicy );
}

KDvoid CCEGLView::setDesignResolutionSize ( KDfloat fWidth, KDfloat fHeight, ResolutionPolicy eResolutionPolicy )
{
    CCAssert ( eResolutionPolicy != kResolutionUnKnown, "should set resolutionPolicy" );
    
    if ( fWidth == 0.0f || fHeight == 0.0f )
    {
        return;
    }

    m_tDesignResolutionSize.setSize ( fWidth, fHeight );
    
    m_fScaleX = (KDfloat) m_tScreenSize.cx / m_tDesignResolutionSize.cx;
    m_fScaleY = (KDfloat) m_tScreenSize.cy / m_tDesignResolutionSize.cy;
    
	switch ( eResolutionPolicy )
	{
		case kResolutionNoBorder :
			m_fScaleX = m_fScaleY = KD_MAX ( m_fScaleX, m_fScaleY );
			break;

		case kResolutionShowAll :
			m_fScaleX = m_fScaleY = KD_MIN ( m_fScaleX, m_fScaleY );
			break;

		case kResolutionFixedHeight :
    		m_fScaleX = m_fScaleY;
    		m_tDesignResolutionSize.cx = kdCeilf ( m_tScreenSize.cx / m_fScaleX );
			break;

		case kResolutionFixedWidth :
    		m_fScaleY = m_fScaleX;
    		m_tDesignResolutionSize.cy = kdCeilf ( m_tScreenSize.cy / m_fScaleY );
			break;

		default : ;
	}

    // calculate the rect of viewport    
	CCSize  tViewPort
	(
		m_tDesignResolutionSize.cx * m_fScaleX,
		m_tDesignResolutionSize.cy * m_fScaleY 
	);

    m_tViewPortRect.setRect 
	(
		( m_tScreenSize.cx - tViewPort.cx ) / 2,
		( m_tScreenSize.cy - tViewPort.cy ) / 2, 
		tViewPort.cx,
		tViewPort.cy
	);
    
    m_eResolutionPolicy = eResolutionPolicy;
    
	// reset director's member variables to fit visible rect
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	pDirector->m_tWinSizeInPoints = m_tDesignResolutionSize;
	CCDirector::sharedDirector ( )->createStatsLabel ( );
    pDirector->setGLDefaultValues ( );
	pDirector->setProjection ( pDirector->getProjection ( ) );
}

const CCSize& CCEGLView::getDesignResolutionSize ( KDvoid ) const 
{
    return m_tDesignResolutionSize;
}

ResolutionPolicy CCEGLView::getDesignResolutionPolicy ( KDvoid )
{
	return m_eResolutionPolicy;
}

const CCSize& CCEGLView::getDeviceSize ( KDvoid ) const
{
	return l_tDeviceSize;
}

const CCSize& CCEGLView::getFrameSize ( KDvoid ) const
{
    return m_tScreenSize;
}

KDvoid CCEGLView::setFrameSize ( KDfloat fWidth, KDfloat fHeight )
{
	m_tDesignResolutionSize = m_tScreenSize = ccs ( fWidth, fHeight );
}

CCSize CCEGLView::getVisibleSize ( KDvoid ) const
{
    if ( m_eResolutionPolicy == kResolutionNoBorder )
    {
        return CCSize ( m_tScreenSize.cx / m_fScaleX, m_tScreenSize.cy / m_fScaleY );
    }
    else 
    {
        return m_tDesignResolutionSize;
    }
}

CCPoint CCEGLView::getVisibleOrigin ( KDvoid ) const
{
    if ( m_eResolutionPolicy == kResolutionNoBorder )
    {
        return CCPoint ( ( m_tDesignResolutionSize.cx - m_tScreenSize.cx / m_fScaleX ) * 0.5f, 
                         ( m_tDesignResolutionSize.cy - m_tScreenSize.cy / m_fScaleY ) * 0.5f );
    }
    else 
    {
        return CCPointZero;
    }
}

CCRect CCEGLView::getVisibleRect ( KDvoid ) const
{
	return CCRect ( this->getVisibleOrigin ( ), this->getVisibleSize ( ) );
}

CCRect CCEGLView::setViewPortInPoints ( KDfloat x, KDfloat y, KDfloat w, KDfloat h )
{
	CCRect  tRect 
	(
		x * m_fScaleX + m_tViewPortRect.origin.x,
		y * m_fScaleY + m_tViewPortRect.origin.y,
		w * m_fScaleX,
		h * m_fScaleY
	);

	glViewport ( (GLint) tRect.origin.x, (GLint) tRect.origin.y, (GLsizei) tRect.size.cx, (GLsizei) tRect.size.cy );

	return tRect;
}

CCRect CCEGLView::setScissorInPoints ( KDfloat x, KDfloat y, KDfloat w, KDfloat h )
{
	CCRect  tRect 
	(
		x * m_fScaleX + m_tViewPortRect.origin.x,
		y * m_fScaleY + m_tViewPortRect.origin.y,
		w * m_fScaleX,
		h * m_fScaleY
	);

    glScissor ( (GLint) tRect.origin.x, (GLint) tRect.origin.y, (GLsizei) tRect.size.cx, (GLsizei) tRect.size.cy );

	return tRect;
}

KDbool CCEGLView::isScissorEnabled ( KDvoid )
{
	return glIsEnabled ( GL_SCISSOR_TEST ) ? KD_TRUE : KD_FALSE;
}

CCRect CCEGLView::getScissorRect ( KDvoid )
{
	GLfloat  aParams[4];
	glGetFloatv ( GL_SCISSOR_BOX, aParams );

	KDfloat  x = ( aParams[0] - m_tViewPortRect.origin.x ) / m_fScaleX;
	KDfloat  y = ( aParams[1] - m_tViewPortRect.origin.y ) / m_fScaleY;
	KDfloat  w = aParams[2] / m_fScaleX;
	KDfloat  h = aParams[3] / m_fScaleY;

	return CCRectMake ( x, y, w, h );
}

const CCRect& CCEGLView::getViewPortRect ( KDvoid ) const
{
    return m_tViewPortRect;
}

KDfloat CCEGLView::getScaleX ( KDvoid ) const
{
    return m_fScaleX;
}

KDfloat CCEGLView::getScaleY ( KDvoid ) const
{
    return m_fScaleY;
}

KDvoid CCEGLView::handleTouchesBegin ( const KDEventTouches& tTouches )
{  
	CCSet  tSet;
    
	for ( KDint i = 0; i < tTouches.count; i++ )
    {
		const KDTouch&  tTouch = tTouches.touches [ i ];
        
		for ( KDint nID = 0; nID < CC_MAX_TOUCHES; nID++ )
		{
			if ( l_aTouches [ nID ] == KD_NULL )
			{
				CCTouch*  pTouch = new CCTouch ( );  

				l_aTouchesDict.setObject ( pTouch, (KDint) tTouch.id );
				l_aTouches [ nID ] = KD_TRUE;

				pTouch->setTouchInfo 
				(
					nID,
					( tTouch.x - m_tViewPortRect.origin.x ) / m_fScaleX,
					( tTouch.y - m_tViewPortRect.origin.y ) / m_fScaleY 
				);

				tSet.addObject ( pTouch ); 

				pTouch->release ( );
				
				break;
			}
		}      	
    }

	if ( tSet.count ( ) > 0 )
	{
		CCDirector::sharedDirector ( )->getTouchDispatcher ( )->touchesBegan ( &tSet, KD_NULL );  
	}
}

KDvoid CCEGLView::handleTouchesMoved ( const KDEventTouches& tTouches )
{
	CCSet  tSet;
    
	for ( KDint i = 0; i < tTouches.count; i++ )
    {
		const KDTouch&  tTouch = tTouches.touches [ i ];
        CCTouch*        pTouch = (CCTouch*) l_aTouchesDict.objectForKey ( (KDint) tTouch.id );                                        					
        
		if ( pTouch )
		{
			pTouch->setTouchInfo 
			(				
				( tTouch.x - m_tViewPortRect.origin.x ) / m_fScaleX,
				( tTouch.y - m_tViewPortRect.origin.y ) / m_fScaleY 
			);

			tSet.addObject ( pTouch );
		}
    }

	if ( tSet.count ( ) > 0 )
	{
		CCDirector::sharedDirector ( )->getTouchDispatcher ( )->touchesMoved ( &tSet, KD_NULL );  
	} 
}

KDvoid CCEGLView::handleTouchesEnded ( const KDEventTouches& tTouches )
{
	CCSet  tSet;
    
	for ( KDint i = 0; i < tTouches.count; i++ )
    {
		const KDTouch&  tTouch = tTouches.touches [ i ];
        CCTouch*        pTouch = (CCTouch*) l_aTouchesDict.objectForKey ( (KDint) tTouch.id );                                        					
        
		if ( pTouch )
		{			
			pTouch->setTouchInfo 
			(
				( tTouch.x - m_tViewPortRect.origin.x ) / m_fScaleX,
				( tTouch.y - m_tViewPortRect.origin.y ) / m_fScaleY 
			);

			tSet.addObject ( pTouch );

			l_aTouches [ pTouch->getID ( ) ] = KD_FALSE;
		}
    }

	if ( tSet.count ( ) > 0 )
	{
		CCDirector::sharedDirector ( )->getTouchDispatcher ( )->touchesEnded ( &tSet, KD_NULL );  
	} 

	for ( KDint i = 0; i < tSet.count ( ); i++ )
	{
		const KDTouch&  tTouch = tTouches.touches [ i ];

		l_aTouchesDict.removeObjectForKey ( (KDint) tTouch.id );
	}
}

NS_CC_END

USING_NS_CC;

KD_API KDvoid KD_APIENTRY  ccEventProc ( const KDEvent* pEvent )
{
	switch ( pEvent->type )
	{
		case KD_EVENT_CREATE :

			if ( !l_bIsRunning )
			{	
				CCSize   tSize = ccs ( pEvent->data.size.width, pEvent->data.size.height );

			#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS  
				KDint    nScale;
				KDfloat  fScale;
				    
				kdGetWindowPropertyiv ( 0, KD_WINDOWPROPERTY_SCALE, &nScale );
				fScale = (KDfloat) nScale;
				l_tDeviceSize = CCSize ( tSize.cx / fScale, tSize.cy / fScale );
			#endif

				CCEGLView::sharedOpenGLView ( )->setFrameSize ( tSize.cx, tSize.cy );
				CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( tSize );  

				if ( CCApplication::sharedApplication ( )->applicationDidFinishLaunching ( ) )
				{
					l_bIsRunning = KD_TRUE;
				}
			}

			return;

		case KD_EVENT_DESTROY :

			if ( l_bIsRunning )
			{
				l_bIsRunning = KD_FALSE;	
				CCApplication::sharedApplication ( )->applicationWillTerminate ( );
				CCDirector::sharedDirector ( )->purgeDirector ( );
			}
			
			return;

		case KD_EVENT_REDRAW :

			if ( l_bIsRunning )
			{
				if ( !( l_bIsRunning = CCDirector::sharedDirector ( )->mainLoop ( ) ) )
				{
					CCApplication::sharedApplication ( )->applicationWillTerminate ( );
					CCDirector::sharedDirector ( )->purgeDirector ( );					
				}				
			}

			break;

		case KD_EVENT_VISIBLE :

			if ( l_bIsRunning )
			{
				if ( pEvent->data.value.i == 1 )
				{
					CCApplication::sharedApplication ( )->applicationWillEnterForeground ( );					
				}
				else
				{
					CCApplication::sharedApplication ( )->applicationDidEnterBackground ( );					
				#if CC_ENABLE_CACHE_TEXTURE_DATA 
					CCTextureCache::sharedTextureCache ( )->removeUnusedTextures ( );
					CCNotificationCenter::sharedNotificationCenter ( )->postNotification ( EVENT_COME_TO_BACKGROUND, KD_NULL );
				#endif
				}
			}

			break;

		case KD_EVENT_RESIZE :       

			if ( l_bIsRunning )
			{
			#if CC_ENABLE_CACHE_TEXTURE_DATA        
				CCTextureCache::sharedTextureCache ( )->reloadAllTextures ( ); 
				CCNotificationCenter::sharedNotificationCenter ( )->postNotification ( EVENT_COME_TO_FOREGROUND, KD_NULL );
			#endif

				CCSize   tSize = ccs ( pEvent->data.size.width, pEvent->data.size.height );
				CCSize   tWinSize = CCEGLView::sharedOpenGLView ( )->getDesignResolutionSize ( );

				CCEGLView::sharedOpenGLView ( )->setFrameSize ( tSize.cx, tSize.cy );
				CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( tWinSize );     				
			}
            
			break;

		case KD_EVENT_INSERT_TEXT :   

			if ( l_bIsRunning )
			{                
                CCIMEDispatcher::sharedDispatcher ( )->dispatchInsertText 
				(
					pEvent->data.insert.utf8, kdStrlen ( pEvent->data.insert.utf8 ) 
				);
			}

			break;

		case KD_EVENT_ACCELEROMETER :   

			if ( l_bIsRunning )
			{        
				CCDirector::sharedDirector ( )->getAccelerometer ( )->update 
				(
					pEvent->data.accelerometer.x,
					pEvent->data.accelerometer.y,
					pEvent->data.accelerometer.z,
					KD_GET_UST2MSEC / 1000.f
				);          
			}

			break;

		case KD_EVENT_KEY_RELEASED :    

			if ( l_bIsRunning )
			{               
				CCDirector::sharedDirector ( )->getKeypadDispatcher ( )->dispatchKeypadMSG ( 0, pEvent->data.keypad.code );
			}

			break;

		case KD_EVENT_KEY_PRESSED :    

			if ( l_bIsRunning )
			{
                switch ( pEvent->data.keypad.code )
                {
                    case KD_INPUT_XMSOFT_BACKSPACE :
                        
                        CCIMEDispatcher::sharedDispatcher ( )->dispatchDeleteBackward ( );
                        
                        break;
                        
                    case KD_INPUT_XMSOFT_RETURN :
                        
                        CCIMEDispatcher::sharedDispatcher ( )->dispatchInsertText ( "\n", 1 );
                        
                        break;
                        
                    default :
                        
                        break;
                }
                
				CCDirector::sharedDirector ( )->getKeypadDispatcher ( )->dispatchKeypadMSG ( 1, pEvent->data.keypad.code );                                
			}

			break;

		case KD_EVENT_TOUCH_BEGAN :    

			if ( l_bIsRunning )
			{
				CCEGLView::sharedOpenGLView ( )->handleTouchesBegin ( pEvent->data.touch );			     
			}

			break;

		case KD_EVENT_TOUCH_MOVED :       

			if ( l_bIsRunning )
			{
				CCEGLView::sharedOpenGLView ( )->handleTouchesMoved ( pEvent->data.touch );              
			}

			break;

		case KD_EVENT_TOUCH_ENDED :    
        case KD_EVENT_TOUCH_CANCELLED :   

			if ( l_bIsRunning )
			{
				CCEGLView::sharedOpenGLView ( )->handleTouchesEnded ( pEvent->data.touch );
			}
			break;
            
        case KD_EVENT_MEMORY_WARNING :
            
			if ( l_bIsRunning )
			{
				CCTextureCache::sharedTextureCache ( )->removeUnusedTextures ( );
			}
            
            break;
	}

	if ( l_bIsRunning )
	{
		CCDirector::sharedDirector ( )->getEventDispatcher ( )->dispatchEvent ( pEvent );
	}
}
