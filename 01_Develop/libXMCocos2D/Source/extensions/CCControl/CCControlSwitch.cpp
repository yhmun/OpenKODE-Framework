/* --------------------------------------------------------------------------
 *
 *      File            CCControlSwitch.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      starfair       ( Converted to c++ )
 *
 *         http://www.cocos2d-x.org   
 *         http://creativewax.co.uk
 *         http://yannickloriot.com
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "extensions/CCControl/CCControlSwitch.h"
#include "actions/CCActionTween.h"
#include "label_nodes/CCLabelTTF.h"
#include "touch_dispatcher/CCTouch.h"
#include "misc_nodes/CCRenderTexture.h"
#include "support/CCPointExtension.h"
#include "shaders/CCGLProgram.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/ccShaders.h"

NS_CC_BEGIN

class CCControlSwitchSprite : public CCSprite, public CCActionTweenDelegate
{
	public :

		CCControlSwitchSprite ( KDvoid );			

		virtual ~CCControlSwitchSprite ( KDvoid );

	public :

		KDbool   initWithMaskSprite ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite, CCNode* pOnLabel, CCNode* pOffLabel );

		KDvoid   draw ( KDvoid );

		KDvoid   needsLayout ( KDvoid );

		KDvoid   setSliderXPosition ( KDfloat fSliderXPosition );
		KDfloat  getSliderXPosition ( KDvoid );

		KDfloat  onSideWidth  ( KDvoid );
		KDfloat  offSideWidth ( KDvoid );

		virtual  KDvoid  updateTweenAction ( KDfloat fValue, const KDchar* szKey );

		KDvoid   setSelectedOn  ( KDvoid );
		KDvoid   setSelectedOff ( KDvoid );
 
		CC_SYNTHESIZE ( KDfloat, m_fOnPosition, OnPosition)
		CC_SYNTHESIZE ( KDfloat, m_fOffPosition, OffPosition)
	    
		CC_SYNTHESIZE_RETAIN ( CCTexture2D*, m_pMaskTexture, MaskTexture );

		CC_SYNTHESIZE ( GLuint, m_uTextureLocation, TextureLocation )
		CC_SYNTHESIZE ( GLuint, m_uMaskLocation   , MaskLocation    )
	    
		CC_SYNTHESIZE_RETAIN ( CCSprite*, m_pOnSprite   , OnSprite    );
		CC_SYNTHESIZE_RETAIN ( CCSprite*, m_pOffSprite  , OffSprite   );
		CC_SYNTHESIZE_RETAIN ( CCSprite*, m_pThumbSprite, ThumbSprite );

		CC_SYNTHESIZE_RETAIN ( CCNode*, m_pOnLabel , OnLabel  );
		CC_SYNTHESIZE_RETAIN ( CCNode*, m_pOffLabel, OffLabel );		

		// Contains the position (in x-axis) of the slider inside the receiver. 
		KDfloat   m_fSliderXPosition;
};

CCControlSwitchSprite::CCControlSwitchSprite ( KDvoid )
{
	m_fSliderXPosition	= 0.0f;
	m_fOnPosition		= 0.0f;
	m_fOffPosition		= 0.0f;
	m_pMaskTexture		= KD_NULL;
	m_uTextureLocation	= 0;
	m_uMaskLocation		= 0;
	m_pOnSprite			= KD_NULL;
	m_pOffSprite		= KD_NULL;
	m_pThumbSprite		= KD_NULL;
	m_pOnLabel			= KD_NULL;
	m_pOffLabel			= KD_NULL;
}

CCControlSwitchSprite::~CCControlSwitchSprite ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pOnSprite     );
    CC_SAFE_RELEASE ( m_pOffSprite    );
    CC_SAFE_RELEASE ( m_pThumbSprite  );
    CC_SAFE_RELEASE ( m_pOnLabel      );
    CC_SAFE_RELEASE ( m_pOffLabel     );
    CC_SAFE_RELEASE ( m_pMaskTexture  );
}

KDbool CCControlSwitchSprite::initWithMaskSprite ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite, CCNode* pOnLabel, CCNode* pOffLabel )
{
    if ( CCSprite::initWithTexture ( pMaskSprite->getTexture ( ) ) )
    {
        // Sets the default values
        m_fOnPosition		= 0;
        m_fOffPosition		= -pOnSprite->getContentSize ( ).cx + pThumbSprite->getContentSize ( ).cx / 2;
        m_fSliderXPosition	= m_fOnPosition; 

        setOnSprite    ( pOnSprite );
        setOffSprite   ( pOffSprite );
        setThumbSprite ( pThumbSprite );
        setOnLabel     ( pOnLabel  );
        setOffLabel    ( pOffLabel );

        addChild ( m_pThumbSprite );

        // Set up the mask with the Mask shader
        setMaskTexture ( pMaskSprite->getTexture ( ) );
        CCGLProgram*  pProgram = new CCGLProgram ( );
        pProgram->initWithVertexShaderByteArray ( ccPositionTextureColor_vert, ccExSwitchMask_frag );
        setShaderProgram ( pProgram );
        pProgram->release ( );

        CHECK_GL_ERROR_DEBUG ( );

        getShaderProgram ( )->addAttribute ( kCCAttributeNamePosition, kCCVertexAttrib_Position  );
        getShaderProgram ( )->addAttribute ( kCCAttributeNameColor   , kCCVertexAttrib_Color     );
        getShaderProgram ( )->addAttribute ( kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords );
        CHECK_GL_ERROR_DEBUG ( );

        getShaderProgram ( )->link ( );
        CHECK_GL_ERROR_DEBUG ( );

        getShaderProgram ( )->updateUniforms ( );
        CHECK_GL_ERROR_DEBUG ( );                

#if defined ( USE_OPEN_GLES2 )
        m_uTextureLocation = glGetUniformLocation ( getShaderProgram ( )->getProgram ( ), "u_texture" );
        m_uMaskLocation    = glGetUniformLocation ( getShaderProgram ( )->getProgram ( ), "u_mask"    );
        CHECK_GL_ERROR_DEBUG ( );
#endif

        setContentSize ( m_pMaskTexture->getContentSize ( ) );

        needsLayout ( );

        return KD_TRUE;
    }

    return KD_FALSE;
}

KDvoid CCControlSwitchSprite::updateTweenAction ( KDfloat fValue, const KDchar* szKey )
{
//	CCLOG ( "key = %s, value = %f", szKey, fValue );
    setSliderXPosition ( fValue );
}

KDvoid CCControlSwitchSprite::draw ( KDvoid )
{
    CC_NODE_DRAW_SETUP ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColorTex );
    ccGLBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	getShaderProgram ( )->setUniformsForBuiltins ( );

    glActiveTexture ( GL_TEXTURE0 );

    ccGLBindTexture2D ( getTexture ( )->getName ( ) );
#if defined ( USE_OPEN_GLES2 )
    glUniform1i ( m_uTextureLocation, 0 );
#else
	glClientActiveTexture ( GL_TEXTURE0 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords	, 2, GL_FLOAT        , GL_FALSE, sizeof ( m_tQuad.bl ), (GLvoid *) ( (GLintptr) &m_tQuad + offsetof ( ccV3F_C4B_T2F, texCoords ) ) );
#endif

    glActiveTexture ( GL_TEXTURE1 );
    ccGLBindTexture2D ( m_pMaskTexture->getName ( ) );
#if defined ( USE_OPEN_GLES2 )
    glUniform1i ( m_uMaskLocation, 1 );
#else
	glClientActiveTexture ( GL_TEXTURE1 );
#endif

	ccGLVertexAttribPointer ( kCCVertexAttrib_Position	, 3, GL_FLOAT        , GL_FALSE, sizeof ( m_tQuad.bl ), (GLvoid *) ( (GLintptr) &m_tQuad + offsetof ( ccV3F_C4B_T2F, vertices  ) ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords	, 2, GL_FLOAT        , GL_FALSE, sizeof ( m_tQuad.bl ), (GLvoid *) ( (GLintptr) &m_tQuad + offsetof ( ccV3F_C4B_T2F, texCoords ) ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Color		, 4, GL_UNSIGNED_BYTE, GL_TRUE , sizeof ( m_tQuad.bl ), (GLvoid *) ( (GLintptr) &m_tQuad + offsetof ( ccV3F_C4B_T2F, colors    ) ) );
	
    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );    
    glActiveTexture ( GL_TEXTURE0 );

#if !defined ( USE_OPEN_GLES2 )
	glClientActiveTexture ( GL_TEXTURE0 );
#endif
}

KDvoid CCControlSwitchSprite::needsLayout ( KDvoid )
{
    m_pOnSprite->setPosition ( ccp ( m_pOnSprite->getContentSize ( ).cx / 2 + m_fSliderXPosition,
									 m_pOnSprite->getContentSize ( ).cy / 2 ) );

    m_pOffSprite->setPosition ( ccp ( m_pOnSprite->getContentSize ( ).cx + m_pOffSprite->getContentSize ( ).cx / 2 + m_fSliderXPosition, 
									  m_pOffSprite->getContentSize ( ).cy / 2 ) );

    m_pThumbSprite->setPosition ( ccp ( m_pOnSprite->getContentSize ( ).cx + m_fSliderXPosition,
										m_pMaskTexture->getContentSize ( ).cy / 2 ) );
    
    if ( m_pOnLabel )
    {
        m_pOnLabel->setPosition ( ccp ( m_pOnSprite->getPosition ( ).x - m_pThumbSprite->getContentSize ( ).cx / 6, 
										m_pOnSprite->getContentSize ( ).cy / 2 ) );
    }
    if ( m_pOffLabel )
    {
        m_pOffLabel->setPosition ( ccp ( m_pOffSprite->getPosition ( ).x + m_pThumbSprite->getContentSize ( ).cx / 6,
										 m_pOffSprite->getContentSize ( ).cy / 2 ) );
    }

	CCRenderTexture*  pRT =  CCRenderTexture::create
	(
		(KDuint) m_pMaskTexture->getContentSize ( ).cx, (KDuint) m_pMaskTexture->getContentSize ( ).cy 
	);
   

	pRT->begin ( );

	m_pOnSprite ->visit ( );
	m_pOffSprite->visit ( );

	if ( m_pOnLabel )
	{
		m_pOnLabel->visit ( );
	}

	if ( m_pOffLabel )
	{
		m_pOffLabel->visit ( );
	}

	pRT->end ( );

    setTexture ( pRT->getSprite ( )->getTexture ( ) );

	setFlipY ( KD_TRUE );
}

KDvoid CCControlSwitchSprite::setSliderXPosition ( KDfloat fSliderXPosition )
{
    if ( fSliderXPosition <= m_fOffPosition )
    {        
        fSliderXPosition = m_fOffPosition;	
    } 
	else if ( fSliderXPosition >= m_fOnPosition )
    {       
        fSliderXPosition = m_fOnPosition;	
    }    

    m_fSliderXPosition  = fSliderXPosition;    

    needsLayout ( );
}

KDfloat CCControlSwitchSprite::getSliderXPosition ( KDvoid )
{
	return m_fSliderXPosition;
}

KDfloat CCControlSwitchSprite::onSideWidth ( KDvoid )
{
    return m_pOnSprite->getContentSize ( ).cx;
}

KDfloat CCControlSwitchSprite::offSideWidth ( KDvoid )
{
    return m_pOffSprite->getContentSize ( ).cy;
}

KDvoid CCControlSwitchSprite::setSelectedOn ( KDvoid )
{
	if ( m_pThumbSprite )
	{
		m_pThumbSprite->setColor ( ccGRAY );
	}
}


//
// CCControlSwitch
//

CCControlSwitch::CCControlSwitch ( KDvoid )
{
	m_pSwitchSprite				= KD_NULL;
	m_fInitialTouchXPosition	= 0.0f;
	m_bMoved					= KD_FALSE;
	m_bOn						= KD_FALSE;
}

CCControlSwitch::~CCControlSwitch ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSwitchSprite );
}

KDbool CCControlSwitch::initWithMaskSprite ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite )
{
    return initWithMaskSprite ( pMaskSprite, pOnSprite, pOffSprite, pThumbSprite, KD_NULL, KD_NULL );
}

CCControlSwitch* CCControlSwitch::create ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite )
{
	CCControlSwitch*  pRet = new CCControlSwitch ( );

	if ( pRet && pRet->initWithMaskSprite ( pMaskSprite, pOnSprite, pOffSprite, pThumbSprite ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCControlSwitch::initWithMaskSprite ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite, CCNode* pOnLabel, CCNode* pOffLabel )
{
	if ( CCControl::init ( ) )
    {
        CCAssert ( pMaskSprite	, "Mask must not be null." );
		CCAssert ( pOnSprite	, "pOnSprite must not be null." );
        CCAssert ( pOffSprite	, "pOffSprite must not be null." );
        CCAssert ( pThumbSprite	, "pThumbSprite must not be null." );        
        
		setTouchEnabled ( KD_TRUE );
		m_bOn = KD_TRUE;

		m_pSwitchSprite  = new CCControlSwitchSprite ( );
		m_pSwitchSprite->initWithMaskSprite ( pMaskSprite, pOnSprite, pOffSprite, pThumbSprite, pOnLabel, pOffLabel );
		m_pSwitchSprite->setPosition ( ccpMid ( m_pSwitchSprite->getContentSize ( ) ) );
		addChild ( m_pSwitchSprite );        
        
        ignoreAnchorPointForPosition ( KD_FALSE );
        setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
        setContentSize ( m_pSwitchSprite->getContentSize ( ) );
		return KD_TRUE;
    }

    return KD_FALSE;
}

CCControlSwitch* CCControlSwitch::create ( CCSprite* pMaskSprite, CCSprite* pOnSprite, CCSprite* pOffSprite, CCSprite* pThumbSprite, CCNode* pOnLabel, CCNode* pOffLabel )
{
	CCControlSwitch*  pRet = new CCControlSwitch ( );

	if ( pRet && pRet->initWithMaskSprite ( pMaskSprite, pOnSprite, pOffSprite, pThumbSprite, pOnLabel, pOffLabel ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDvoid CCControlSwitch::setOn ( KDbool bIsOn )
{
    setOn ( bIsOn, KD_FALSE );
}

KDvoid CCControlSwitch::setOn ( KDbool bIsOn, KDbool bAnimated )
{
    m_bOn = bIsOn;

	if ( bAnimated ) 
	{
		m_pSwitchSprite->runAction
		(
			CCActionTween::create
			(
				0.2f, 
				"sliderXPosition",
				m_pSwitchSprite->getSliderXPosition ( ),
				( m_bOn ) ? m_pSwitchSprite->getOnPosition ( ) : m_pSwitchSprite->getOffPosition ( )
			)
		);
	}
	else
	{
		m_pSwitchSprite->setSliderXPosition ( ( m_bOn ) ? m_pSwitchSprite->getOnPosition ( ) : m_pSwitchSprite->getOffPosition ( ) );
	}

	sendActionsForControlEvents ( kCCControlEventValueChanged );
}

KDvoid CCControlSwitch::setEnabled ( KDbool bEnabled )
{
	m_bEnabled = bEnabled;

	if ( m_pSwitchSprite != KD_NULL )
	{
		m_pSwitchSprite->setOpacity ( ( bEnabled ) ? 255 : 128 );
	}
}

CCPoint CCControlSwitch::locationFromTouch ( CCTouch* pTouch )
{
	return convertToNodeSpace ( pTouch->getLocation ( ) );
}

KDbool CCControlSwitch::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( !isTouchInside ( pTouch ) || !isEnabled ( ) || !isVisible ( ) )
    {
        return KD_FALSE;
    }    

    m_bMoved = KD_FALSE;    

    CCPoint  tLocation = locationFromTouch ( pTouch );    

    m_fInitialTouchXPosition = tLocation.x - m_pSwitchSprite->getSliderXPosition ( );    

    m_pSwitchSprite->getThumbSprite ( )->setColor ( ccGRAY );
    m_pSwitchSprite->needsLayout ( );  

    return KD_TRUE;
}

KDvoid CCControlSwitch::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
    CCPoint  tLocation = locationFromTouch ( pTouch );
    tLocation          = ccp ( tLocation.x - m_fInitialTouchXPosition, 0 );   
    m_bMoved           = KD_TRUE;    

    m_pSwitchSprite->setSliderXPosition ( tLocation.x );
}

KDvoid CCControlSwitch::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    CCPoint  tLocation = locationFromTouch ( pTouch );  

	m_pSwitchSprite->getThumbSprite ( )->setColor ( ccWHITE );
    
	if ( m_bMoved )
    {
		setOn ( !(tLocation.x < m_pSwitchSprite->getContentSize ( ).cx / 2 ) , KD_TRUE );
	} 
	else
    {
        setOn ( !m_bOn, KD_TRUE );
    }
}

NS_CC_END