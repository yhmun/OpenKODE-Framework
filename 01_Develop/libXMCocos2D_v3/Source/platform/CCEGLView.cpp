/* -----------------------------------------------------------------------------------
 *
 *      File            CCEGLView.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013		cocos2d-x.org
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
 * --------------------------------------------------------------------------------- */ 

#include "platform/CCEGLView.h"
#include "base/CCSet.h"
#include "base/CCDictionary.h"
#include "ccMacros.h"
#include "CCEventType.h"
#include "2d/CCDirector.h"
#include "2d/text_input_node/CCIMEDispatcher.h"
#include "platform/CCApplication.h"
#include "2d/draw_nodes/CCDrawingPrimitives.h"
#include "2d/event_dispatcher/CCTouch.h"
#include "2d/event_dispatcher/CCEventDispatcher.h"
#include "2d/event_dispatcher/CCEventKeyboard.h"
#include "2d/event_dispatcher/CCEventMouse.h"
#include "2d/event_dispatcher/CCEventAcceleration.h"
#include "2d/event_dispatcher/CCEventCustom.h"
#include "2d/textures/CCTextureCache.h"
#include "support/CCNotificationCenter.h"
#include "shaders/CCShaderCache.h"

NS_CC_BEGIN

namespace 
{    
	static KDbool					l_bIsRunning = KD_FALSE;
	static EGLView*					l_pEGLView	 = KD_NULL;
	static Size						l_tDeviceSize;

    static Touch*					l_aTouches [ EventTouch::MAX_TOUCHES ] = { NULL };
    static KDuint					l_uIndexBitsUsed = 0;
    // System touch pointer ID (It may not be ascending order number) <-> Ascending order number from 0
    static std::map<KDint, KDint>	l_aTouchIdReorderMap;
    
	static Point					l_tMousePos;

    static KDint	getUnUsedIndex ( KDvoid )
    {
        KDint	i;
        KDint	nTemp = l_uIndexBitsUsed;
        
        for ( i = 0; i < EventTouch::MAX_TOUCHES; i++ )
		{
            if ( !( nTemp & 0x00000001 ) )
			{
                l_uIndexBitsUsed |= ( 1 <<  i );
                return i;
            }
            
            nTemp >>= 1;
        }
        
        // all bits are used
        return -1;
    }
    
    static KDvoid	removeUsedIndexBit ( KDint nIndex )
    {
        if ( nIndex < 0 || nIndex >= EventTouch::MAX_TOUCHES )
        {
            return;
        }
        
        KDuint	uTemp = 1 << nIndex;
        uTemp = ~uTemp;
        l_uIndexBitsUsed &= uTemp;
    }    
}

//////////////////////////////////////////////////////////////////////////
// impliment EGLView
//////////////////////////////////////////////////////////////////////////

EGLView::EGLView ( KDvoid )
{
	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
	m_eResolutionPolicy = ResolutionPolicy::EXACT_FIT;
}

KDvoid EGLView::setDesignResolutionSize ( KDfloat fWidth, KDfloat fHeight )
{
	this->setDesignResolutionSize ( fWidth, fHeight, m_eResolutionPolicy );
}

KDvoid EGLView::setDesignResolutionSize ( KDfloat fWidth, KDfloat fHeight, ResolutionPolicy eResolutionPolicy )
{
    CCASSERT ( eResolutionPolicy != ResolutionPolicy::UNKNOWN, "should set resolutionPolicy" );
    
    if ( fWidth == 0.0f || fHeight == 0.0f )
    {
        return;
    }

    m_tDesignResolutionSize.setSize ( fWidth, fHeight );
    
    m_fScaleX = m_tScreenSize.cx / m_tDesignResolutionSize.cx;
    m_fScaleY = m_tScreenSize.cy / m_tDesignResolutionSize.cy;
    
	switch ( eResolutionPolicy )
	{
		case ResolutionPolicy::NO_BORDER :
			m_fScaleX = m_fScaleY = KD_MAX ( m_fScaleX, m_fScaleY );
			break;

		case ResolutionPolicy::SHOW_ALL :
			m_fScaleX = m_fScaleY = KD_MIN ( m_fScaleX, m_fScaleY );
			break;

		case ResolutionPolicy::FIXED_HEIGHT :
    		m_fScaleX = m_fScaleY;
    		m_tDesignResolutionSize.cx = kdCeilf ( m_tScreenSize.cx / m_fScaleX );
			break;

		case ResolutionPolicy::FIXED_WIDTH :
    		m_fScaleY = m_fScaleX;
    		m_tDesignResolutionSize.cy = kdCeilf ( m_tScreenSize.cy / m_fScaleY );
			break;

		default : 
			break;
	}

    // calculate the rect of viewport    
    KDfloat		fViewPortW = m_tDesignResolutionSize.cx * m_fScaleX;
    KDfloat		fViewPortH = m_tDesignResolutionSize.cy * m_fScaleY;

    m_tViewPortRect.setRect ( ( m_tScreenSize.cx - fViewPortW ) / 2, ( m_tScreenSize.cy - fViewPortH ) / 2, fViewPortW, fViewPortH );
    
    m_eResolutionPolicy = eResolutionPolicy;
    
	// reset director's member variables to fit visible rect
    Director::getInstance ( )->m_tWinSizeInPoints = this->getDesignResolutionSize ( );
    Director::getInstance ( )->createStatsLabel ( );
    Director::getInstance ( )->setGLDefaultValues ( );
    
    
    // For Ime Information
    xmSetContentScaleFactor( m_fScaleX, m_fScaleY );
    xmSetViewportRect( m_tViewPortRect.origin.x, m_tViewPortRect.origin.y, m_tViewPortRect.size.cx, m_tViewPortRect.size.cy );
}

const Size& EGLView::getDesignResolutionSize ( KDvoid ) const 
{
    return m_tDesignResolutionSize;
}

KDbool EGLView::isOpenGLReady ( KDvoid )
{
	return true;
}

KDvoid EGLView::setIMEKeyboardState ( KDbool bOpen )
{
	xmSetIMEKeyboardState ( bOpen );
}

const Size& EGLView::getFrameSize ( KDvoid ) const
{
    return m_tScreenSize;
}

KDvoid EGLView::setFrameSize ( KDfloat fWidth, KDfloat fHeight )
{
    m_tDesignResolutionSize = m_tScreenSize = Size ( fWidth, fHeight );
}

Size EGLView::getVisibleSize ( KDvoid ) const
{
    if ( m_eResolutionPolicy == ResolutionPolicy::NO_BORDER )
    {
        return Size ( m_tScreenSize.width / m_fScaleX, m_tScreenSize.height / m_fScaleY );
    }
    else 
    {
        return m_tDesignResolutionSize;
    }
}

Point EGLView::getVisibleOrigin ( KDvoid ) const
{
    if ( m_eResolutionPolicy == ResolutionPolicy::NO_BORDER )
    {
        return Point ( ( m_tDesignResolutionSize.cx - m_tScreenSize.cx / m_fScaleX ) / 2, 
					   ( m_tDesignResolutionSize.cy - m_tScreenSize.cy / m_fScaleY ) / 2);
    }
    else 
    {
        return Point::ZERO;
    }
}

KDvoid EGLView::setViewPortInPoints ( KDfloat x , KDfloat y , KDfloat w , KDfloat h )
{
    glViewport ( (GLint)   ( x * m_fScaleX + m_tViewPortRect.origin.x ),
                 (GLint)   ( y * m_fScaleY + m_tViewPortRect.origin.y ),
                 (GLsizei) ( w * m_fScaleX ),
                 (GLsizei) ( h * m_fScaleY ) );
}

KDvoid EGLView::setScissorInPoints ( KDfloat x , KDfloat y , KDfloat w , KDfloat h )
{
    glScissor ( (GLint)   ( x * m_fScaleX + m_tViewPortRect.origin.x ),
                (GLint)   ( y * m_fScaleY + m_tViewPortRect.origin.y ),
                (GLsizei) ( w * m_fScaleX ),
                (GLsizei) ( h * m_fScaleY ) );
}

KDbool EGLView::isScissorEnabled ( KDvoid )
{
	return ( GL_FALSE == glIsEnabled ( GL_SCISSOR_TEST ) ) ? KD_FALSE : KD_TRUE;
}

Rect EGLView::getScissorRect ( KDvoid )
{
	GLfloat		aParams [ 4 ];
	glGetFloatv ( GL_SCISSOR_BOX, aParams );

	KDfloat		x = ( aParams [ 0 ] - m_tViewPortRect.origin.x ) / m_fScaleX;
	KDfloat		y = ( aParams [ 1 ] - m_tViewPortRect.origin.y ) / m_fScaleY;
	KDfloat		w =   aParams [ 2 ] / m_fScaleX;
	KDfloat		h =   aParams [ 3 ] / m_fScaleY;
	return Rect(x, y, w, h);
}

KDvoid EGLView::setViewName ( const KDchar* szViewName )
{
    if ( szViewName != KD_NULL && kdStrlen ( szViewName ) > 0 )
    {
        kdStrncpy ( m_szViewName, szViewName, sizeof ( m_szViewName ) );
    }
}

const KDchar* EGLView::getViewName ( KDvoid )
{
    return m_szViewName;
}

KDvoid EGLView::handleTouchesBegin ( const KDEventTouches& tTouches )
{
	KDint		nNum = tTouches.count;
    KDint		nId = 0;
    KDfloat		x = 0.0f;
    KDfloat		y = 0.0f;
    KDint		nUnusedIndex = 0;
    EventTouch	tTouchEvent;
    
    for ( KDint i = 0; i < nNum; ++i )
    {
		const KDTouch&  tTouch = tTouches.touches [ i ];

        nId = (KDint) tTouch.id;
        x = tTouch.x;
        y = m_tScreenSize.cy - tTouch.y;

        auto	pIter = l_aTouchIdReorderMap.find ( nId );
        nUnusedIndex = 0;

        // it is a new touch
        if ( pIter == l_aTouchIdReorderMap.end ( ) )
        {
            nUnusedIndex = getUnUsedIndex ( );

            // The touches is more than MAX_TOUCHES ?
            if ( nUnusedIndex == -1 )
			{
                CCLOG ( "The touches is more than MAX_TOUCHES, nUnusedIndex = %d", nUnusedIndex );
                continue;
            }

            Touch*		pTouch = l_aTouches [ nUnusedIndex ] = new Touch ( );
			pTouch->setTouchInfo ( nUnusedIndex, ( x - m_tViewPortRect.origin.x ) / m_fScaleX, ( y - m_tViewPortRect.origin.y ) / m_fScaleY );
            
            CCLOGINFO ( "x = %f y = %f", pTouch->getLocationInView ( ).x, pTouch->getLocationInView ( ).y );
            
            l_aTouchIdReorderMap.insert ( std::make_pair ( nId, nUnusedIndex ) );
            tTouchEvent.m_aTouches.push_back ( pTouch );
        }
    }

    if ( tTouchEvent.m_aTouches.size ( ) == 0 )
    {
        CCLOG ( "touchesBegan: size = 0" );
        return;
    }
    
    tTouchEvent.m_eEventCode = EventTouch::EventCode::BEGAN;
    auto	pDispatcher = Director::getInstance ( )->getEventDispatcher ( );
    pDispatcher->dispatchEvent ( &tTouchEvent );
}

KDvoid EGLView::handleTouchesMoved ( const KDEventTouches& tTouches )
{
	KDint		nNum = tTouches.count;
    KDint		nId = 0;
    KDfloat		x = 0.0f;
    KDfloat		y = 0.0f;
    EventTouch	tTouchEvent;
    
    for ( KDint i = 0; i < nNum; ++i )
    {
		const KDTouch&  tTouch = tTouches.touches [ i ];

        nId = (KDint) tTouch.id;
        x = tTouch.x;
        y = m_tScreenSize.cy - tTouch.y;

        auto		pIter = l_aTouchIdReorderMap.find ( nId );
        if ( pIter == l_aTouchIdReorderMap.end ( ) )
        {
            CCLOG ( "if the index doesn't exist, it is an error" );
            continue;
        }

        CCLOGINFO ( "Moving touches with id: %d, x=%f, y=%f", nId, x, y );
        Touch*		pTouch = l_aTouches [ pIter->second ];

        if ( pTouch )
        {
			pTouch->setTouchInfo ( pIter->second, ( x - m_tViewPortRect.origin.x ) / m_fScaleX, ( y - m_tViewPortRect.origin.y ) / m_fScaleY );
            
            tTouchEvent.m_aTouches.push_back ( pTouch );
        }
        else
        {
            // It is error, should return.
            CCLOG ( "Moving touches with id: %d error", nId );
            return;
        }
    }

    if ( tTouchEvent.m_aTouches.size ( ) == 0 )
    {
        CCLOG ( "touchesMoved: size = 0" );
        return;
    }
    
    tTouchEvent.m_eEventCode = EventTouch::EventCode::MOVED;
    auto	pDispatcher = Director::getInstance ( )->getEventDispatcher ( );
    pDispatcher->dispatchEvent ( &tTouchEvent );
}

KDvoid EGLView::handleTouchesEnded ( const KDEventTouches& tTouches )
{
	this->handleTouchesOfEndOrCancel ( EventTouch::EventCode::ENDED, tTouches );
}

KDvoid EGLView::handleTouchesCancelled ( const KDEventTouches& tTouches )
{
	this->handleTouchesOfEndOrCancel ( EventTouch::EventCode::CANCELLED, tTouches );
}

KDvoid EGLView::handleTouchesOfEndOrCancel ( EventTouch::EventCode eEventCode, const KDEventTouches& tTouches )
{
	KDint		nNum = tTouches.count;
    KDint		nId = 0;
    KDfloat		x = 0.0f;
    KDfloat		y = 0.0f;
    EventTouch	tTouchEvent;
    
    for ( KDint i = 0; i < nNum; ++i )
    {
		const KDTouch&  tTouch = tTouches.touches [ i ];

        nId = (KDint) tTouch.id;
        x = tTouch.x;
        y = m_tScreenSize.cy - tTouch.y;

        auto	pIter = l_aTouchIdReorderMap.find ( nId );
        if ( pIter == l_aTouchIdReorderMap.end ( ) )
        {
            CCLOG ( "if the index doesn't exist, it is an error" );
            continue;
        }
        
        // Add to the set to send to the director 
        Touch*	pTouch = l_aTouches [ pIter->second ];
        if ( pTouch )
        {
            CCLOGINFO ( "Ending touches with id: %d, x=%f, y=%f", nId, x, y );
			pTouch->setTouchInfo ( pIter->second, ( x - m_tViewPortRect.origin.x ) / m_fScaleX, ( y - m_tViewPortRect.origin.y ) / m_fScaleY );

            tTouchEvent.m_aTouches.push_back ( pTouch );
            
            l_aTouches [ pIter->second ] = KD_NULL;
            removeUsedIndexBit ( pIter->second );

            l_aTouchIdReorderMap.erase ( nId );
        } 
        else
        {
            CCLOG ( "Ending touches with id: %d error", nId );
            return;
        } 

    }

    if ( tTouchEvent.m_aTouches.size ( ) == 0 )
    {
        CCLOG ( "touchesEnded or touchesCancel: size = 0" );
        return;
    }
    
    tTouchEvent.m_eEventCode = eEventCode;
    auto	pDispatcher = Director::getInstance ( )->getEventDispatcher ( );
    pDispatcher->dispatchEvent ( &tTouchEvent );
    
    for ( auto& tTouch : tTouchEvent.m_aTouches )
    {
        // release the touch object.
        tTouch->release ( );
    }
}

const Rect& EGLView::getViewPortRect ( KDvoid ) const
{
    return m_tViewPortRect;
}

KDfloat EGLView::getScaleX ( KDvoid ) const
{
    return m_fScaleX;
}

KDfloat EGLView::getScaleY ( KDvoid ) const
{
    return m_fScaleY;
}

/*
struct keyCodeItem
{
    int glfwKeyCode;
    EventKeyboard::KeyCode keyCode;
};

static std::map<int, EventKeyboard::KeyCode> g_keyCodeMap;

static keyCodeItem g_keyCodeStructArray[] = {
    // The unknown key 
    { GLFW_KEY_UNKNOWN         , EventKeyboard::KeyCode::KEY_NONE          },
    
    // Printable keys 

    { GLFW_KEY_SPACE           , EventKeyboard::KeyCode::KEY_SPACE         },
    { GLFW_KEY_APOSTROPHE      , EventKeyboard::KeyCode::KEY_APOSTROPHE    },
    { GLFW_KEY_COMMA           , EventKeyboard::KeyCode::KEY_COMMA         },
    { GLFW_KEY_MINUS           , EventKeyboard::KeyCode::KEY_MINUS         },
    { GLFW_KEY_PERIOD          , EventKeyboard::KeyCode::KEY_PERIOD        },
    { GLFW_KEY_SLASH           , EventKeyboard::KeyCode::KEY_SLASH         },
    { GLFW_KEY_0               , EventKeyboard::KeyCode::KEY_0             },
    { GLFW_KEY_1               , EventKeyboard::KeyCode::KEY_1             },
    { GLFW_KEY_2               , EventKeyboard::KeyCode::KEY_2             },
    { GLFW_KEY_3               , EventKeyboard::KeyCode::KEY_3             },
    { GLFW_KEY_4               , EventKeyboard::KeyCode::KEY_4             },
    { GLFW_KEY_5               , EventKeyboard::KeyCode::KEY_5             },
    { GLFW_KEY_6               , EventKeyboard::KeyCode::KEY_6             },
    { GLFW_KEY_7               , EventKeyboard::KeyCode::KEY_7             },
    { GLFW_KEY_8               , EventKeyboard::KeyCode::KEY_8             },
    { GLFW_KEY_9               , EventKeyboard::KeyCode::KEY_9             },
    { GLFW_KEY_SEMICOLON       , EventKeyboard::KeyCode::KEY_SEMICOLON     },
    { GLFW_KEY_EQUAL           , EventKeyboard::KeyCode::KEY_EQUAL         },
    { GLFW_KEY_A               , EventKeyboard::KeyCode::KEY_A             },
    { GLFW_KEY_B               , EventKeyboard::KeyCode::KEY_B             },
    { GLFW_KEY_C               , EventKeyboard::KeyCode::KEY_C             },
    { GLFW_KEY_D               , EventKeyboard::KeyCode::KEY_D             },
    { GLFW_KEY_E               , EventKeyboard::KeyCode::KEY_E             },
    { GLFW_KEY_F               , EventKeyboard::KeyCode::KEY_F             },
    { GLFW_KEY_G               , EventKeyboard::KeyCode::KEY_G             },
    { GLFW_KEY_H               , EventKeyboard::KeyCode::KEY_H             },
    { GLFW_KEY_I               , EventKeyboard::KeyCode::KEY_I             },
    { GLFW_KEY_J               , EventKeyboard::KeyCode::KEY_J             },
    { GLFW_KEY_K               , EventKeyboard::KeyCode::KEY_K             },
    { GLFW_KEY_L               , EventKeyboard::KeyCode::KEY_L             },
    { GLFW_KEY_M               , EventKeyboard::KeyCode::KEY_M             },
    { GLFW_KEY_N               , EventKeyboard::KeyCode::KEY_N             },
    { GLFW_KEY_O               , EventKeyboard::KeyCode::KEY_O             },
    { GLFW_KEY_P               , EventKeyboard::KeyCode::KEY_P             },
    { GLFW_KEY_Q               , EventKeyboard::KeyCode::KEY_Q             },
    { GLFW_KEY_R               , EventKeyboard::KeyCode::KEY_R             },
    { GLFW_KEY_S               , EventKeyboard::KeyCode::KEY_S             },
    { GLFW_KEY_T               , EventKeyboard::KeyCode::KEY_T             },
    { GLFW_KEY_U               , EventKeyboard::KeyCode::KEY_U             },
    { GLFW_KEY_V               , EventKeyboard::KeyCode::KEY_V             },
    { GLFW_KEY_W               , EventKeyboard::KeyCode::KEY_W             },
    { GLFW_KEY_X               , EventKeyboard::KeyCode::KEY_X             },
    { GLFW_KEY_Y               , EventKeyboard::KeyCode::KEY_Y             },
    { GLFW_KEY_Z               , EventKeyboard::KeyCode::KEY_Z             },
    { GLFW_KEY_LEFT_BRACKET    , EventKeyboard::KeyCode::KEY_LEFT_BRACKET  },
    { GLFW_KEY_BACKSLASH       , EventKeyboard::KeyCode::KEY_BACK_SLASH    },
    { GLFW_KEY_RIGHT_BRACKET   , EventKeyboard::KeyCode::KEY_RIGHT_BRACKET },
    { GLFW_KEY_GRAVE_ACCENT    , EventKeyboard::KeyCode::KEY_GRAVE         },
    { GLFW_KEY_WORLD_1         , EventKeyboard::KeyCode::KEY_GRAVE         },
    { GLFW_KEY_WORLD_2         , EventKeyboard::KeyCode::KEY_NONE          },
    
    // Function keys 
    { GLFW_KEY_ESCAPE          , EventKeyboard::KeyCode::KEY_ESCAPE        },
    { GLFW_KEY_ENTER           , EventKeyboard::KeyCode::KEY_KP_ENTER      },
    { GLFW_KEY_TAB             , EventKeyboard::KeyCode::KEY_TAB           },
    { GLFW_KEY_BACKSPACE       , EventKeyboard::KeyCode::KEY_BACKSPACE     },
    { GLFW_KEY_INSERT          , EventKeyboard::KeyCode::KEY_INSERT        },
    { GLFW_KEY_DELETE          , EventKeyboard::KeyCode::KEY_DELETE        },
    { GLFW_KEY_RIGHT           , EventKeyboard::KeyCode::KEY_RIGHT_ARROW   },
    { GLFW_KEY_LEFT            , EventKeyboard::KeyCode::KEY_LEFT_ARROW    },
    { GLFW_KEY_DOWN            , EventKeyboard::KeyCode::KEY_DOWN_ARROW    },
    { GLFW_KEY_UP              , EventKeyboard::KeyCode::KEY_UP_ARROW      },
    { GLFW_KEY_PAGE_UP         , EventKeyboard::KeyCode::KEY_KP_PG_UP      },
    { GLFW_KEY_PAGE_DOWN       , EventKeyboard::KeyCode::KEY_KP_PG_DOWN    },
    { GLFW_KEY_HOME            , EventKeyboard::KeyCode::KEY_KP_HOME       },
    { GLFW_KEY_END             , EventKeyboard::KeyCode::KEY_END           },
    { GLFW_KEY_CAPS_LOCK       , EventKeyboard::KeyCode::KEY_CAPS_LOCK     },
    { GLFW_KEY_SCROLL_LOCK     , EventKeyboard::KeyCode::KEY_SCROLL_LOCK   },
    { GLFW_KEY_NUM_LOCK        , EventKeyboard::KeyCode::KEY_NUM_LOCK      },
    { GLFW_KEY_PRINT_SCREEN    , EventKeyboard::KeyCode::KEY_PRINT         },
    { GLFW_KEY_PAUSE           , EventKeyboard::KeyCode::KEY_PAUSE         },
    { GLFW_KEY_F1              , EventKeyboard::KeyCode::KEY_F1            },
    { GLFW_KEY_F2              , EventKeyboard::KeyCode::KEY_F2            },
    { GLFW_KEY_F3              , EventKeyboard::KeyCode::KEY_F3            },
    { GLFW_KEY_F4              , EventKeyboard::KeyCode::KEY_F4            },
    { GLFW_KEY_F5              , EventKeyboard::KeyCode::KEY_F5            },
    { GLFW_KEY_F6              , EventKeyboard::KeyCode::KEY_F6            },
    { GLFW_KEY_F7              , EventKeyboard::KeyCode::KEY_F7            },
    { GLFW_KEY_F8              , EventKeyboard::KeyCode::KEY_F8            },
    { GLFW_KEY_F9              , EventKeyboard::KeyCode::KEY_F9            },
    { GLFW_KEY_F10             , EventKeyboard::KeyCode::KEY_F10           },
    { GLFW_KEY_F11             , EventKeyboard::KeyCode::KEY_F11           },
    { GLFW_KEY_F12             , EventKeyboard::KeyCode::KEY_F12           },
    { GLFW_KEY_F13             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F14             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F15             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F16             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F17             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F18             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F19             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F20             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F21             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F22             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F23             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F24             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F25             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_KP_0            , EventKeyboard::KeyCode::KEY_0             },
    { GLFW_KEY_KP_1            , EventKeyboard::KeyCode::KEY_1             },
    { GLFW_KEY_KP_2            , EventKeyboard::KeyCode::KEY_2             },
    { GLFW_KEY_KP_3            , EventKeyboard::KeyCode::KEY_3             },
    { GLFW_KEY_KP_4            , EventKeyboard::KeyCode::KEY_4             },
    { GLFW_KEY_KP_5            , EventKeyboard::KeyCode::KEY_5             },
    { GLFW_KEY_KP_6            , EventKeyboard::KeyCode::KEY_6             },
    { GLFW_KEY_KP_7            , EventKeyboard::KeyCode::KEY_7             },
    { GLFW_KEY_KP_8            , EventKeyboard::KeyCode::KEY_8             },
    { GLFW_KEY_KP_9            , EventKeyboard::KeyCode::KEY_9             },
    { GLFW_KEY_KP_DECIMAL      , EventKeyboard::KeyCode::KEY_PERIOD        },
    { GLFW_KEY_KP_DIVIDE       , EventKeyboard::KeyCode::KEY_KP_DIVIDE     },
    { GLFW_KEY_KP_MULTIPLY     , EventKeyboard::KeyCode::KEY_KP_MULTIPLY   },
    { GLFW_KEY_KP_SUBTRACT     , EventKeyboard::KeyCode::KEY_KP_MINUS      },
    { GLFW_KEY_KP_ADD          , EventKeyboard::KeyCode::KEY_KP_PLUS       },
    { GLFW_KEY_KP_ENTER        , EventKeyboard::KeyCode::KEY_KP_ENTER      },
    { GLFW_KEY_KP_EQUAL        , EventKeyboard::KeyCode::KEY_EQUAL         },
    { GLFW_KEY_LEFT_SHIFT      , EventKeyboard::KeyCode::KEY_SHIFT         },
    { GLFW_KEY_LEFT_CONTROL    , EventKeyboard::KeyCode::KEY_CTRL          },
    { GLFW_KEY_LEFT_ALT        , EventKeyboard::KeyCode::KEY_ALT           },
    { GLFW_KEY_LEFT_SUPER      , EventKeyboard::KeyCode::KEY_HYPER         },
    { GLFW_KEY_RIGHT_SHIFT     , EventKeyboard::KeyCode::KEY_SHIFT         },
    { GLFW_KEY_RIGHT_CONTROL   , EventKeyboard::KeyCode::KEY_CTRL          },
    { GLFW_KEY_RIGHT_ALT       , EventKeyboard::KeyCode::KEY_ALT           },
    { GLFW_KEY_RIGHT_SUPER     , EventKeyboard::KeyCode::KEY_HYPER         },
    { GLFW_KEY_MENU            , EventKeyboard::KeyCode::KEY_MENU          },
    { GLFW_KEY_LAST            , EventKeyboard::KeyCode::KEY_NONE          }
};


#if(_MSC_VER >= 1600) // Visual Studio 2010 or higher version.
// Windows Touch define
#define MOUSEEVENTF_FROMTOUCH 0xFF515700

// Windows Touch functions
// Workaround to be able tu run app on Windows XP
typedef WINUSERAPI BOOL (WINAPI *RegisterTouchWindowFn)(_In_ HWND hwnd, _In_ ULONG ulFlags);
typedef WINUSERAPI BOOL (WINAPI *UnregisterTouchWindowFn)(_In_ HWND hwnd);
typedef WINUSERAPI LPARAM (WINAPI *GetMessageExtraInfoFn)(VOID);
typedef WINUSERAPI BOOL (WINAPI *GetTouchInputInfoFn)(_In_ HTOUCHINPUT hTouchInput, _In_ UINT cInputs, __out_ecount(cInputs) PTOUCHINPUT pInputs, _In_ int cbSize);
typedef WINUSERAPI BOOL (WINAPI *CloseTouchInputHandleFn)(_In_ HTOUCHINPUT hTouchInput);

static RegisterTouchWindowFn s_pfRegisterTouchWindowFunction = NULL;
static UnregisterTouchWindowFn s_pfUnregisterTouchWindowFunction = NULL;
static GetMessageExtraInfoFn s_pfGetMessageExtraInfoFunction = NULL;
static GetTouchInputInfoFn s_pfGetTouchInputInfoFunction = NULL;
static CloseTouchInputHandleFn s_pfCloseTouchInputHandleFunction = NULL;

static bool CheckTouchSupport()
{
	s_pfRegisterTouchWindowFunction = (RegisterTouchWindowFn)GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "RegisterTouchWindow");
	s_pfUnregisterTouchWindowFunction = (UnregisterTouchWindowFn)GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "UnregisterTouchWindow");
	s_pfGetMessageExtraInfoFunction = (GetMessageExtraInfoFn)GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "GetMessageExtraInfo");
	s_pfGetTouchInputInfoFunction = (GetTouchInputInfoFn)GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "GetTouchInputInfo");
	s_pfCloseTouchInputHandleFunction = (CloseTouchInputHandleFn)GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "CloseTouchInputHandle");

	return (s_pfRegisterTouchWindowFunction && s_pfUnregisterTouchWindowFunction && s_pfGetMessageExtraInfoFunction && s_pfGetTouchInputInfoFunction && s_pfCloseTouchInputHandleFunction);
}

#endif	// #if(_MSC_VER >= 1600) 

static bool glew_dynamic_binding()
{
	const char *gl_extensions = (const char*)glGetString(GL_EXTENSIONS);

	// If the current opengl driver doesn't have framebuffers methods, check if an extension exists
	if (glGenFramebuffers == NULL)
	{
		log("OpenGL: glGenFramebuffers is NULL, try to detect an extension");
		if (strstr(gl_extensions, "ARB_framebuffer_object"))
		{
			log("OpenGL: ARB_framebuffer_object is supported");

			glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbuffer");
			glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbuffer");
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffers");
			glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffers");
			glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorage");
			glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameteriv");
			glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebuffer");
			glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebuffer");
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffers");
			glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffers");
			glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatus");
			glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1D");
			glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2D");
			glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3D");
			glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbuffer");
			glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
			glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
		}
		else
		if (strstr(gl_extensions, "EXT_framebuffer_object"))
		{
			log("OpenGL: EXT_framebuffer_object is supported");
			glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbufferEXT");
			glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbufferEXT");
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffersEXT");
			glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffersEXT");
			glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorageEXT");
			glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameterivEXT");
			glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebufferEXT");
			glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebufferEXT");
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffersEXT");
			glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffersEXT");
			glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
			glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1DEXT");
			glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2DEXT");
			glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3DEXT");
			glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
			glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
			glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmapEXT");
		}
		else
		{
			log("OpenGL: No framebuffers extension is supported");
			log("OpenGL: Any call to Fbo will crash!");
			return false;
		}
	}
	return true;
}
//begin EGLViewEventHandler
class EGLViewEventHandler
{
public:
    static bool s_captured;
    static float s_mouseX;
    static float s_mouseY;

    static void OnGLFWError(int errorID, const char* errorDesc);
    static void OnGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify);
    static void OnGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y);
    static void OnGLFWMouseScrollCallback(GLFWwindow* window, double x, double y);
    static void OnGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void OnGLFWCharCallback(GLFWwindow* window, unsigned int character);
    static void OnGLFWWindowPosCallback(GLFWwindow* windows, int x, int y);
};

bool EGLViewEventHandler::s_captured = false;
float EGLViewEventHandler::s_mouseX = 0;
float EGLViewEventHandler::s_mouseY = 0;

void EGLViewEventHandler::OnGLFWError(int errorID, const char* errorDesc)
{
    CCLOGERROR("GLFWError #%d Happen, %s\n", errorID, errorDesc);
}

void EGLViewEventHandler::OnGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
{
    EGLView* eglView = EGLView::getInstance();
    if(nullptr == eglView) return;
    if(GLFW_MOUSE_BUTTON_LEFT == button)
    {
        if(GLFW_PRESS == action)
        {
            s_captured = true;
            if (eglView->getViewPortRect().equals(Rect::ZERO) || eglView->getViewPortRect().containsPoint(Point(s_mouseX,s_mouseY)))
            {
                long id = 0;
                eglView->handleTouchesBegin(1, &id, &s_mouseX, &s_mouseY);
            }
        }
        else if(GLFW_RELEASE == action)
        {
            s_captured = false;
            if (eglView->getViewPortRect().equals(Rect::ZERO) || eglView->getViewPortRect().containsPoint(Point(s_mouseX,s_mouseY)))
            {
                long id = 0;
                eglView->handleTouchesEnd(1, &id, &s_mouseX, &s_mouseY);
            }
        }
    }

    if(GLFW_PRESS == action)
    {
        EventMouse event(EventMouse::MouseEventType::MOUSE_DOWN);
        event.setCursorPosition(s_mouseX, eglView->getViewPortRect().size.height - s_mouseY);
        event.setMouseButton(button);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
    else if(GLFW_RELEASE == action)
    {
        EventMouse event(EventMouse::MouseEventType::MOUSE_UP);
        event.setCursorPosition(s_mouseX, eglView->getViewPortRect().size.height - s_mouseY);
        event.setMouseButton(button);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void EGLViewEventHandler::OnGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
{
    s_mouseX = (float)x;
    s_mouseY = (float)y;
    EGLView* eglView = EGLView::getInstance();
    if(nullptr == eglView) return;

    s_mouseX /= eglView->getFrameZoomFactor();
    s_mouseY /= eglView->getFrameZoomFactor();

    if(s_captured)
    {
        if (eglView->getViewPortRect().equals(Rect::ZERO) || eglView->getViewPortRect().containsPoint(Point(s_mouseX,eglView->getFrameSize().height - s_mouseY)))
        {
            long id = 0;
            eglView->handleTouchesMove(1, &id, &s_mouseX, &s_mouseY);
        }
    }

    EventMouse event(EventMouse::MouseEventType::MOUSE_MOVE);
    //Because OpenGL use upper left as origin point, we need to revert the mouse y coordinate here
    event.setCursorPosition(s_mouseX, eglView->getViewPortRect().size.height - s_mouseY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void EGLViewEventHandler::OnGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
{
    EGLView* eglView = EGLView::getInstance();
    if(nullptr == eglView) return;

    EventMouse event(EventMouse::MouseEventType::MOUSE_SCROLL);
    //Because OpenGL use upper left as origin point, we need to revert the mouse y coordinate here
    event.setScrollData((float)x, -(float)y);
    event.setCursorPosition(s_mouseX, eglView->getViewPortRect().size.height - s_mouseY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void EGLViewEventHandler::OnGLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    EventKeyboard event(g_keyCodeMap[key], GLFW_PRESS == action || GLFW_REPEAT == action);
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->dispatchEvent(&event);
}

void EGLViewEventHandler::OnGLFWCharCallback(GLFWwindow *window, unsigned int character)
{
    IMEDispatcher::sharedDispatcher()->dispatchInsertText((const char*) &character, 1);
}

void EGLViewEventHandler::OnGLFWWindowPosCallback(GLFWwindow *windows, int x, int y)
{
    if(Director::getInstance())
    {
        Director::getInstance()->setViewport();
    }
}
*/

//end EGLViewEventHandler

KDvoid EGLView::setFrameZoomFactor ( KDfloat fZoomFactor )
{
    m_fFrameZoomFactor = fZoomFactor;
    Director::getInstance ( )->setProjection ( Director::getInstance ( )->getProjection ( ) );
}

KDfloat EGLView::getFrameZoomFactor ( KDvoid )
{
    return m_fFrameZoomFactor;
}
/*
void EGLView::setFrameSize(float width, float height)
{
    EGLView::setFrameSize(width, height);
}
*/

EGLView* EGLView::getInstance ( KDvoid )
{    
	if ( !l_pEGLView )
	{
		l_pEGLView = new EGLView ( );		
	}

	return l_pEGLView;
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
				l_tDeviceSize = Size ( pEvent->data.size.width, pEvent->data.size.height );
				/*
			#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS  
				KDint    nScale;
				KDfloat  fScale;
				    
				kdGetWindowPropertyiv ( 0, KD_WINDOWPROPERTY_SCALE, &nScale );
				fScale = (KDfloat) nScale;
				l_tDeviceSize = l_tDeviceSize / fScale;
			#endif
			*/

				EGLView ::getInstance ( )->setFrameSize ( l_tDeviceSize.cx, l_tDeviceSize.cy );
				Director::getInstance ( )->setOpenGLView ( l_pEGLView );

				if ( Application::getInstance ( )->applicationDidFinishLaunching ( ) )
				{
					l_bIsRunning = KD_TRUE;
				}
			}
			return;

		case KD_EVENT_DESTROY :
			if ( l_bIsRunning )
			{
				l_bIsRunning = KD_FALSE;	
				Application::getInstance ( )->applicationWillTerminate ( );
				Director::getInstance ( )->purgeDirector ( );
			}			
			return;

		case KD_EVENT_REDRAW :
			if ( l_bIsRunning )
			{
				if ( !( l_bIsRunning = Director::getInstance ( )->mainLoop ( ) ) )
				{
					Application::getInstance ( )->applicationWillTerminate ( );
					Director::getInstance ( )->purgeDirector ( );					
				}				
			}
			break;

		case KD_EVENT_VISIBLE :
			if ( l_bIsRunning )
			{
				if ( pEvent->data.value.i == 1 )
				{
					Application::getInstance ( )->applicationWillEnterForeground ( );					
				}
				else
				{
					Application::getInstance ( )->applicationDidEnterBackground ( );					
				#if CC_ENABLE_CACHE_TEXTURE_DATA 
					Director::getInstance ( )->getTextureCache ( )->removeUnusedTextures ( );
					NotificationCenter::getInstance ( )->postNotification ( EVENT_COME_TO_BACKGROUND, KD_NULL );
				#endif
				}
			}
			break;

		case KD_EVENT_RESIZE :       
			if ( l_bIsRunning )
			{
			#if CC_ENABLE_CACHE_TEXTURE_DATA        
				GL::invalidateStateCache ( );
				ShaderCache::getInstance ( )->reloadDefaultShaders ( );
				DrawPrimitives::init ( );
				VolatileTextureMgr::reloadAllTextures ( );
				NotificationCenter::getInstance ( )->postNotification ( EVENT_COME_TO_FOREGROUND, KD_NULL );
				Director::getInstance ( )->setGLDefaultValues ( );
			#endif

				Size   tWinSize = EGLView::getInstance ( )->getDesignResolutionSize ( );

				EGLView::getInstance ( )->setFrameSize ( pEvent->data.size.width, pEvent->data.size.height );
				EGLView::getInstance ( )->setDesignResolutionSize ( tWinSize.cx, tWinSize.cy );     				
			}          
			break;

		case KD_EVENT_INSERT_TEXT :   
			if ( l_bIsRunning )
			{                
				IMEDispatcher::sharedDispatcher ( )->dispatchInsertText ( pEvent->data.insert.utf8, kdStrlen ( pEvent->data.insert.utf8 ) );
			}
			break;

		case KD_EVENT_ACCELEROMETER :   
			if ( l_bIsRunning )
			{        
				Acceleration		tAcceleration;

				tAcceleration.x = pEvent->data.accelerometer.x;
				tAcceleration.y = pEvent->data.accelerometer.y;
				tAcceleration.z = pEvent->data.accelerometer.z;
				tAcceleration.timestamp = kdGetMilliseconds ( ) / 1000.0;

				EventAcceleration	tEvent ( tAcceleration );
	
				Director::getInstance ( )->getEventDispatcher ( )->dispatchEvent (  &tEvent );
			}
			break;

		case KD_EVENT_KEY_RELEASED :    
			if ( l_bIsRunning )
			{               
				if ( pEvent->data.keypad.code & KD_IOGROUP_KEY )
				{
					EventKeyboard	tEvent ( (EventKeyboard::KeyCode) pEvent->data.keypad.code, KD_FALSE );
					Director::getInstance ( )->getEventDispatcher ( )->dispatchEvent ( &tEvent );
				}   
			}
			break;

		case KD_EVENT_KEY_PRESSED :    
			if ( l_bIsRunning )
			{
                switch ( pEvent->data.keypad.code )
                {
                    case KD_INPUT_XMSOFT_BACKSPACE :                        
                        IMEDispatcher::sharedDispatcher ( )->dispatchDeleteBackward ( );                        
                        break;
                        
                    case KD_INPUT_XMSOFT_RETURN :                        
                        IMEDispatcher::sharedDispatcher ( )->dispatchInsertText ( "\n", 1 );                        
                        break;
                        
                    default :                        
                        break;
                }

				if ( pEvent->data.keypad.code & KD_IOGROUP_KEY )
				{
					EventKeyboard	tEvent ( (EventKeyboard::KeyCode) pEvent->data.keypad.code, KD_TRUE );
					Director::getInstance ( )->getEventDispatcher ( )->dispatchEvent ( &tEvent );
				}                            
			}
			break;

		case KD_EVENT_TOUCH_BEGAN :    
			if ( l_bIsRunning )
			{
				EGLView::getInstance ( )->handleTouchesBegin ( pEvent->data.touch );			     
			}
			break;

		case KD_EVENT_TOUCH_MOVED :       
			if ( l_bIsRunning )
			{
				EGLView::getInstance ( )->handleTouchesMoved ( pEvent->data.touch );              
			}
			break;

		case KD_EVENT_TOUCH_ENDED :    
			if ( l_bIsRunning )
			{
				EGLView::getInstance ( )->handleTouchesEnded ( pEvent->data.touch );
			}
			break;

        case KD_EVENT_TOUCH_CANCELLED :   
			if ( l_bIsRunning )
			{
				EGLView::getInstance ( )->handleTouchesCancelled ( pEvent->data.touch );
			}
			break;

		case KD_EVENT_MOUSE_PRESSED :
		{
			EventMouse		tEvent ( EventMouse::MouseEventType::MOUSE_DOWN );
			// Because OpenGL use upper left as origin point, we need to revert the mouse y coordinate here
			tEvent.setCursorPosition ( l_tMousePos.x, l_tMousePos.y );
			tEvent.setMouseButton ( pEvent->data.mouse.code );
			Director::getInstance ( )->getEventDispatcher ( )->dispatchEvent ( &tEvent );
		}	break;

		case KD_EVENT_MOUSE_RELEASED :
		{
			EventMouse		tEvent ( EventMouse::MouseEventType::MOUSE_UP );
			// Because OpenGL use upper left as origin point, we need to revert the mouse y coordinate here
			tEvent.setCursorPosition ( l_tMousePos.x, l_tMousePos.y );
			tEvent.setMouseButton ( pEvent->data.mouse.code );
			Director::getInstance ( )->getEventDispatcher()->dispatchEvent ( &tEvent );
		}	break;

		case KD_EVENT_MOUSE_MOVED :
		{
			l_tMousePos.x = (KDfloat) pEvent->data.mouse.x;
			l_tMousePos.y = (KDfloat) pEvent->data.mouse.y;

			EventMouse		tEvent ( EventMouse::MouseEventType::MOUSE_MOVE );
			// Because OpenGL use upper left as origin point, we need to revert the mouse y coordinate here
			tEvent.setCursorPosition ( l_tMousePos.x, l_tMousePos.y );
			Director::getInstance ( )->getEventDispatcher ( )->dispatchEvent ( &tEvent );
		}	break;

		case KD_EVENT_MOUSE_SCROLLED :
		{
			EventMouse		tEvent ( EventMouse::MouseEventType::MOUSE_SCROLL );
			// Because OpenGL use upper left as origin point, we need to revert the mouse y coordinate here
			tEvent.setScrollData ( (KDfloat) pEvent->data.mouse.x, -(KDfloat) pEvent->data.mouse.y );
			tEvent.setCursorPosition ( l_tMousePos.x, l_tMousePos.y );
			Director::getInstance ( )->getEventDispatcher()->dispatchEvent ( &tEvent );
		}	break;
            
            
        case KD_EVENT_IME_NOTIFICATION :
        {
            IMEDispatcher*                  pDispatcher = IMEDispatcher::sharedDispatcher ( );
            
            IMEKeyboardNotificationInfo     tInfo;
            tInfo.begin.origin.x = pEvent->data.imenotify.begin.x;
            tInfo.begin.origin.y = pEvent->data.imenotify.begin.y;
            tInfo.begin.size.cx  = pEvent->data.imenotify.begin.w;
            tInfo.begin.size.cy  = pEvent->data.imenotify.begin.h;
            
            tInfo.end.origin.x = pEvent->data.imenotify.end.x;
            tInfo.end.origin.y = pEvent->data.imenotify.end.y;
            tInfo.end.size.cx  = pEvent->data.imenotify.end.w;
            tInfo.end.size.cy  = pEvent->data.imenotify.end.h;
            
            tInfo.duration = pEvent->data.imenotify.duration;
            
            switch ( pEvent->data.imenotify.status )
            {
                case 0 : pDispatcher->dispatchKeyboardDidHide  ( tInfo ); break;
                case 1 : pDispatcher->dispatchKeyboardWillHide ( tInfo ); break;
                case 2 : pDispatcher->dispatchKeyboardDidShow  ( tInfo ); break;
                case 3 : pDispatcher->dispatchKeyboardWillShow ( tInfo ); break;
            }
            
        }   break;
            
        case KD_EVENT_MEMORY_WARNING :           
			if ( l_bIsRunning )
			{
				Director::getInstance ( )->getTextureCache ( )->removeUnusedTextures ( );
			}           
            break;
	}

	if ( l_bIsRunning )
	{
		EventCustom		tEvent ( "XMKode" );
		tEvent.setUserData ( &pEvent );
		Director::getInstance ( )->getEventDispatcher ( )->dispatchEvent ( &tEvent );
	}
}
