/* --------------------------------------------------------------------------
 *
 *      File            TestShader.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
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

#include "Precompiled.h"
#include "TestShader.h"

TestShader* TestShader::create ( KDvoid )
{
	TestShader*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case 0  :	pLayer = new ShaderMonjori      ( );  break;
		case 1	:	pLayer = new ShaderMandelbrot   ( );  break;
		case 2  :	pLayer = new ShaderJulia        ( );  break;
		case 3  :	pLayer = new ShaderHeart        ( );  break;
		case 4	:	pLayer = new ShaderFlower       ( );  break;
		case 5  :	pLayer = new ShaderPlasma       ( );  break;
		case 6  :	pLayer = new ShaderBlur         ( );  break;
		case 7  :	pLayer = new ShaderRetroEffect  ( );  break;                
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestShader::count ( KDvoid )
{
	return 8;
}

//---------------------------------------
// 
// ShaderNode
// 
//---------------------------------------
enum 
{
    SIZE_X = 256,
    SIZE_Y = 256,
};

ShaderNode::ShaderNode ( KDvoid )
{
    m_tResolution          = vertex2 ( 0.0f, 0.0f );
    m_fTime                = 0.0f;
    m_uniformCenter        = 0;
    m_uniformResolution    = 0;
    m_uniformTime          = 0;
}

ShaderNode* ShaderNode::shaderNodeWithVertex ( const KDchar* pVert, const KDchar* pFrag )
{
    ShaderNode*  pRet = new ShaderNode ( );
    
    if ( pRet && pRet->initWithVertex ( pVert, pFrag ) )
    {
        pRet->autorelease ( );
    }
    else 
    {
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

KDbool ShaderNode::initWithVertex ( const KDchar* pVert, const KDchar* pFrag )
{    
    loadShaderVertex ( pVert, pFrag );
    
    m_fTime = 0;
    m_tResolution = vertex2 ( SIZE_X, SIZE_Y );
    
    scheduleUpdate ( );
    
    setContentSize ( CCSize ( SIZE_X, SIZE_Y ) );
    setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    
    return true;
}

KDvoid ShaderNode::loadShaderVertex ( const KDchar* pVert, const KDchar* pFrag )
{
    CCGLProgram*  pShader = new CCGLProgram ( );
    
    pShader->initWithVertexShaderFilename ( pVert, pFrag );
    pShader->addAttribute ( "aVertex", kCCVertexAttrib_Position );
    pShader->link ( );
    pShader->updateUniforms ( );
    
    m_uniformCenter     = glGetUniformLocation ( pShader->getProgram ( ), "center" );
    m_uniformResolution = glGetUniformLocation ( pShader->getProgram ( ), "resolution" );
    m_uniformTime       = glGetUniformLocation ( pShader->getProgram ( ), "time" );
    
    this->setShaderProgram ( pShader );
    
    pShader->release ( );
}

KDvoid ShaderNode::update ( KDfloat fDelta )
{
    m_fTime += fDelta;
}

KDvoid ShaderNode::draw ( KDvoid )
{
    CC_NODE_DRAW_SETUP ( );
    
    GLfloat  fW = SIZE_X;
    GLfloat  fH = SIZE_Y;
    
    GLfloat  aVertices[ 12 ] = { 0, 0, fW, 0, fW, fH, 0, 0, 0, fH, fW, fH };
    
    //
    // Uniforms
    //
    getShaderProgram ( )->setUniformLocationWith2f ( m_uniformCenter, m_tPosition.x, m_tPosition.y );
    getShaderProgram ( )->setUniformLocationWith2f ( m_uniformResolution, m_tResolution.x, m_tResolution.y );
    
    // time changes all the time, so it is Ok to call OpenGL directly, and not the "cached" version
    glUniform1f ( m_uniformTime, m_fTime );
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    glVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, aVertices );
    
    glDrawArrays ( GL_TRIANGLES, 0, 6 );
    
    CC_INCREMENT_GL_DRAWS ( 1 );
}


//---------------------------------------
// 
// SpriteBlur
// 
//---------------------------------------

SpriteBlur* SpriteBlur::create ( const KDchar* szFileName )
{
    SpriteBlur*  pRet = new SpriteBlur ( );
    
    if ( pRet && pRet->initWithFile ( szFileName ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

KDbool SpriteBlur::initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
    if ( !CCSprite::initWithTexture ( pTexture, tRect ) ) 
    {
        return KD_FALSE;
    }
    
    
    CCSize  tSize = getTexture ( )->getContentSize ( );
        
    m_tBlur = ccp ( 1 / tSize.cx, 1 / tSize.cy );
    m_fSub[0] = m_fSub[1] = m_fSub[2] = m_fSub[3] = 0;
        
    GLchar*  fragSource = (GLchar*) CCString::createWithContentsOfFile
    (
        CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( "Shaders/example_Blur.fsh" ).c_str ( )
    )->getCString ( );
    
    CCGLProgram*   pProgram = new CCGLProgram ( );
    pProgram->initWithVertexShaderByteArray ( ccPositionTextureColor_vert, fragSource );
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
        
    m_uSubLocation  = glGetUniformLocation ( getShaderProgram ( )->getProgram ( ), "substract" );
    m_uBlurLocation = glGetUniformLocation ( getShaderProgram ( )->getProgram ( ), "blurSize"  );
        
    CHECK_GL_ERROR_DEBUG ( );
    
    return KD_TRUE;
}

KDvoid SpriteBlur::draw ( KDvoid )
{
    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColorTex );
    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );
    
    getShaderProgram ( )->use();
    getShaderProgram ( )->setUniformsForBuiltins ( );
    getShaderProgram ( )->setUniformLocationWith2f  ( m_uBlurLocation, m_tBlur.x, m_tBlur.y );
    getShaderProgram ( )->setUniformLocationWith4fv ( m_uSubLocation, m_fSub, 1);
    
    ccGLBindTexture2D(  getTexture()->getName() );
    
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position	, 3, GL_FLOAT        , GL_FALSE, sizeof ( m_tQuad.bl ), (GLvoid *) ( (GLintptr) &m_tQuad + offsetof ( ccV3F_C4B_T2F, vertices  ) ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords	, 2, GL_FLOAT        , GL_FALSE, sizeof ( m_tQuad.bl ), (GLvoid *) ( (GLintptr) &m_tQuad + offsetof ( ccV3F_C4B_T2F, texCoords ) ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Color		, 4, GL_UNSIGNED_BYTE, GL_TRUE , sizeof ( m_tQuad.bl ), (GLvoid *) ( (GLintptr) &m_tQuad + offsetof ( ccV3F_C4B_T2F, colors    ) ) );
    
    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
    
    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid SpriteBlur::setBlurSize( KDfloat fSize )
{
    const CCSize&  tSize = getTexture ( )->getContentSize ( );
    
    m_tBlur = ccp ( 1 / tSize.cx, 1 / tSize.cy );
    m_tBlur = ccpMult ( m_tBlur, fSize );
}


//
// ShaderMonjori
//
KDbool ShaderMonjori::init ( KDvoid )
{
    if ( TestShader::init ( ) == KD_FALSE )
    {
        return KD_FALSE;        
    }
    
    ShaderNode*  pNode = ShaderNode::shaderNodeWithVertex
    (
        "Shaders/example_Monjori.vsh", "Shaders/example_Monjori.fsh"
    );
    do 
    {
        CC_BREAK_IF( !pNode );
        
        pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
                           
    } while ( 0 );
        
    addChild ( pNode );
    
    return KD_TRUE;
}

const KDchar* ShaderMonjori::subtitle ( KDvoid )
{
    return "Monjori plane deformations";
}

//
// ShaderMandelbrot
//
KDbool ShaderMandelbrot::init ( KDvoid )
{
    if ( TestShader::init ( ) == KD_FALSE )
    {
        return KD_FALSE;        
    }
    
    ShaderNode*  pNode = ShaderNode::shaderNodeWithVertex
    (
        "Shaders/example_Mandelbrot.vsh", "Shaders/example_Mandelbrot.fsh"
    );
    do 
    {
        CC_BREAK_IF( !pNode );
        
        pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
        
    } while ( 0 );
    
    addChild ( pNode );
    
    return KD_TRUE;
}

const KDchar* ShaderMandelbrot::subtitle ( KDvoid )
{
    return "Mandelbrot shader with Zoom";
}


//
// ShaderJulia
//
KDbool ShaderJulia::init ( KDvoid )
{
    if ( TestShader::init ( ) == KD_FALSE )
    {
        return KD_FALSE;        
    }
    
    ShaderNode*  pNode = ShaderNode::shaderNodeWithVertex
    (
        "Shaders/example_Julia.vsh", "Shaders/example_Julia.fsh"
    );
    do 
    {
        CC_BREAK_IF( !pNode );
        
        pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
        
    } while ( 0 );
    
    addChild ( pNode );
    
    return KD_TRUE;
}

const KDchar* ShaderJulia::subtitle ( KDvoid )
{
    return "Julia shader";
}


//
// ShaderHeart
//
KDbool ShaderHeart::init ( KDvoid )
{
    if ( TestShader::init ( ) == KD_FALSE )
    {
        return KD_FALSE;        
    }
    
    ShaderNode*  pNode = ShaderNode::shaderNodeWithVertex
    (
        "Shaders/example_Heart.vsh", "Shaders/example_Heart.fsh"
    );
    do 
    {
        CC_BREAK_IF( !pNode );
        
        pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
        
    } while ( 0 );
    
    addChild ( pNode );
    
    return KD_TRUE;
}

const KDchar* ShaderHeart::subtitle ( KDvoid )
{
    return "Heart";
}


//
// ShaderFlower
//
KDbool ShaderFlower::init ( KDvoid )
{
    if ( TestShader::init ( ) == KD_FALSE )
    {
        return KD_FALSE;        
    }
    
    ShaderNode*  pNode = ShaderNode::shaderNodeWithVertex
    (
        "Shaders/example_Flower.vsh", "Shaders/example_Flower.fsh"
    );
    do 
    {
        CC_BREAK_IF( !pNode );
        
        pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
        
    } while ( 0 );
    
    addChild ( pNode );
    
    return KD_TRUE;
}

const KDchar* ShaderFlower::subtitle ( KDvoid )
{
    return "Flower";
}


//
// ShaderPlasma
//
KDbool ShaderPlasma::init ( KDvoid )
{
    if ( TestShader::init ( ) == KD_FALSE )
    {
        return KD_FALSE;        
    }
    
    ShaderNode*  pNode = ShaderNode::shaderNodeWithVertex
    (
        "Shaders/example_Plasma.vsh", "Shaders/example_Plasma.fsh"
    );
    do 
    {
        CC_BREAK_IF( !pNode );
        
        pNode->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
        
    } while ( 0 );
    
    addChild ( pNode );
    
    return KD_TRUE;
}

const KDchar* ShaderPlasma::subtitle ( KDvoid )
{
    return "Plasma";
}


//
// ShaderBlur
//
KDbool ShaderBlur::init ( KDvoid )
{
    if ( TestShader::init ( ) == KD_FALSE )
    {
        return KD_FALSE;        
    }
    
    const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
    
    CCSprite*  pSprite = CCSprite::create ( "Images/grossini.png" );
    m_pBlurSprite = SpriteBlur::create ( "Images/grossini.png" );
    
    m_pBlurSprite->setPosition ( ccp ( tWinSize.cx / 3 * 1, tWinSize.cy / 2 ) );
          pSprite->setPosition ( ccp ( tWinSize.cx / 3 * 2, tWinSize.cy / 2 ) );
    
    addChild ( m_pBlurSprite );
    addChild ( pSprite );
    
    m_pSliderCtl = createSliderCtl ( );
    addChild ( m_pSliderCtl );
    
    return KD_TRUE;
}

const KDchar* ShaderBlur::subtitle ( KDvoid )
{
    return "Gaussian blur";
}


CCControlSlider* ShaderBlur::createSliderCtl ( KDvoid )
{
    const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
    
    CCControlSlider*  pSlider = CCControlSlider::create
    (
        "extensions/sliderTrack.png","extensions/sliderProgress.png" ,"extensions/sliderThumb.png"
    );
    
    pSlider->setAnchorPoint ( ccp ( 0.5f, 1.0f ) );
    pSlider->setMinimumValue ( 0.0f ); // Sets the min value of range
    pSlider->setMaximumValue ( 3.0f ); // Sets the max value of range
    pSlider->setValue ( 1.0f );
    pSlider->setPosition ( ccp ( tWinSize.cx / 2.0f, tWinSize.cy / 3.0f ) );
    
    // When the value of the slider will change, the given selector will be call
    pSlider->addTargetWithActionForControlEvents ( this, cccontrol_selector ( ShaderBlur::sliderAction ), kCCControlEventValueChanged );
    
    return pSlider;
}

KDvoid ShaderBlur::sliderAction ( CCObject* pSender, CCControlEvent uControlEvent )
{
    CCControlSlider*  pSlider = (CCControlSlider*) pSender;
    
    m_pBlurSprite->setBlurSize ( pSlider->getValue ( ) );
}

//
// ShaderRetroEffect
//
KDbool ShaderRetroEffect::init ( KDvoid )
{
    if ( TestShader::init ( ) == KD_FALSE )
    {
        return KD_FALSE;        
    }
    
    GLchar*  fragSource = (GLchar*) CCString::createWithContentsOfFile
    (
        CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( "Shaders/example_HorizontalColor.fsh" ).c_str ( )
    )->getCString ( );
    
    CCGLProgram*  pShader = new CCGLProgram ( );
    
    pShader->initWithVertexShaderByteArray ( ccPositionTexture_vert, fragSource );    
    pShader->addAttribute ( kCCAttributeNamePosition, kCCVertexAttrib_Position );
    pShader->addAttribute ( kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords );    
    pShader->link ( );
    pShader->updateUniforms ( );
    
    m_pLabel = CCLabelBMFont::create ( "RETRO EFFECT", "fonts/west_england-64.fnt" );
    m_pLabel->setShaderProgram ( pShader );
    m_pLabel->setPosition( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
    
    addChild ( m_pLabel );
    
    pShader->release ( );
    
    scheduleUpdate ( );
    
    return KD_TRUE;
}

const KDchar* ShaderRetroEffect::title ( KDvoid )
{
    return "Shader : Retro test";
}

const KDchar* ShaderRetroEffect::subtitle ( KDvoid )
{
    return "sin() effect with moving colors";
}

KDvoid ShaderRetroEffect::update ( KDfloat fDelta )
{
    m_fAccum += fDelta;
    
    KDint      i = 0;
    CCArray*   pArray = m_pLabel->getChildren ( );
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( pArray, pObject )
    {
        i++;
        
        CCSprite*  pSprite = (CCSprite*) pObject;        
        CCPoint  tPosition = pSprite->getPosition ( );
        
        pSprite->setPosition ( ccp ( tPosition.x, kdSinf ( m_fAccum * 2 + i / 2.0f ) * 20 ) );
        
        // add fabs() to prevent negative scaling
        KDfloat fScaleY = ( kdSinf ( m_fAccum * 2 + i / 2.0f + 0.707f ) );
        
        pSprite->setScaleY ( fScaleY );
    }
}

