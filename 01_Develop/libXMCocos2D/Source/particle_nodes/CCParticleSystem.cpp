/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleSystem.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
 *
 *     ideas taken from :
 *
 *       . The ocean spray in your face [Jeff Lander]
 *          http://www.double.co.nz/dust/col0798.pdf
 *
 *       . Building an Advanced Particle System [John van der Burg]
 *          http://www.gamasutra.com/features/20000623/vanderburg_01.htm
 *
 *       . LOVE game engine
 *          http://love2d.org/
 *
 *
 *       Radius mode support, from 71 squared
 *          http://particledesigner.71squared.com/
 *
 *       IMPORTANT: Particle Designer is supported by cocos2d, but
 *       'Radius Mode' in Particle Designer uses a fixed emit rate of 30 hz. Since that can't be guaranteed in cocos2d,
 *        cocos2d uses a another approach, but the results are almost identical.  
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
#include "particle_nodes/CCParticleSystem.h"
#include "particle_nodes/CCParticleBatchNode.h"
#include "textures/CCTextureCache.h"
#include "textures/CCTextureAtlas.h"
#include "support/zip_support/ZipUtils.h"
#include "support/base64.h"
#include "support/CCImage.h"
#include "support/CCFileUtils.h"
#include "support/CCPointExtension.h"
#include "support/CCDirector.h"

NS_CC_BEGIN

//
// implementation CCParticleSystem
//

CCParticleSystem::CCParticleSystem ( KDvoid )
{
    m_sPlistPath                    = "";
    m_fElapsed                      = 0;
    m_pParticles                    = KD_NULL;
    m_fEmitCounter                  = 0;
    m_uParticleIdx                  = 0;
    m_bIsActive                     = KD_TRUE;
    m_uParticleCount                = 0;
    m_fDuration                     = 0;
    m_tSourcePosition               = CCPointZero;
    m_tPosVar                       = CCPointZero;
    m_fLife                         = 0;
    m_fLifeVar                      = 0;
    m_fAngle                        = 0;
    m_fAngleVar                     = 0;
    m_fStartSize                    = 0;
    m_fStartSizeVar                 = 0;
    m_fEndSize                      = 0;
    m_fEndSizeVar                   = 0;
    m_fStartSpin                    = 0;
    m_fStartSpinVar                 = 0;
    m_fEndSpin                      = 0;
    m_fEndSpinVar                   = 0;
    m_fEmissionRate                 = 0;
    m_uTotalParticles               = 0;
    m_pTexture                      = KD_NULL;
	m_bOpacityModifyRGB				= KD_FALSE;
    m_bIsBlendAdditive              = KD_FALSE;
    m_ePositionType                 = kCCPositionTypeFree;
    m_bIsAutoRemoveOnFinish         = KD_FALSE;
    m_nEmitterMode                  = kCCParticleModeGravity;
	m_pBatchNode					= KD_NULL;
	m_uAtlasIndex					= 0;
	m_bTransformSystemDirty			= KD_FALSE;
	m_uAllocatedParticles			= 0;
 
	m_tModeA.tGravity               = CCPointZero;
	m_tModeA.fSpeed                 = 0;
	m_tModeA.fSpeedVar              = 0;
	m_tModeA.fTangentialAccel       = 0;
	m_tModeA.fTangentialAccelVar    = 0;
	m_tModeA.fRadialAccel           = 0;
	m_tModeA.fRadialAccelVar        = 0;
	m_tModeA.bRotationIsDir			= KD_FALSE;
	m_tModeB.fStartRadius           = 0;
	m_tModeB.fStartRadiusVar        = 0;
	m_tModeB.fEndRadius             = 0;
	m_tModeB.fEndRadiusVar          = 0;			
	m_tModeB.fRotatePerSecond       = 0;
	m_tModeB.fRotatePerSecondVar    = 0;
	m_tBlendFunc.src				= CC_BLEND_SRC;
	m_tBlendFunc.dst				= CC_BLEND_DST;
}

CCParticleSystem * CCParticleSystem::create ( const KDchar* szPlistFile )
{
	CCParticleSystem*  pRet = new CCParticleSystem ( );

	if ( pRet && pRet->initWithFile ( szPlistFile ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCParticleSystem* CCParticleSystem::createWithTotalParticles ( KDuint uNumberOfParticles )
{
    CCParticleSystem*  pRet = new CCParticleSystem ( );
    
	if ( pRet && pRet->initWithTotalParticles ( uNumberOfParticles ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDbool CCParticleSystem::init ( KDvoid )
{
    return initWithTotalParticles ( 150 );
}

KDbool CCParticleSystem::initWithFile ( const KDchar* szPlistFile )
{	
	KDbool  bRet = KD_FALSE;
	m_sPlistPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szPlistFile );
	CCDictionary*  pDictionary = CCDictionary::createWithContentsOfFileThreadSafe ( m_sPlistPath.c_str ( ) );

	CCAssert ( pDictionary != KD_NULL, "Particles: file not found" );
	// XXX compute path from a path, should define a function somewhere to do it
	std::string  sListFilePath = szPlistFile;
	if ( sListFilePath.find ( '/' ) != std::string::npos )
    {
        sListFilePath = sListFilePath.substr ( 0, sListFilePath.rfind ( '/' ) + 1 );
        bRet = this->initWithDictionary ( pDictionary, sListFilePath.c_str ( ) );
    }
    else
    {
        bRet = this->initWithDictionary ( pDictionary, "" );
    }

	pDictionary->release ( );

	return bRet;
}

KDbool CCParticleSystem::initWithDictionary ( CCDictionary* pDictionary )
{
    return initWithDictionary ( pDictionary, "" );
}

KDbool CCParticleSystem::initWithDictionary ( CCDictionary* pDictionary, const KDchar* szDirname )
{
	KDbool  bRet = KD_FALSE;
	
	do 
	{
		KDint  nMaxParticles = pDictionary->valueForKey ( "maxParticles" )->intValue ( );

		// self, not super
		if ( this->initWithTotalParticles ( nMaxParticles ) )
		{
			// angle
			m_fAngle            = pDictionary->valueForKey ( "angle"                        )->floatValue ( );
			m_fAngleVar         = pDictionary->valueForKey ( "angleVariance"                )->floatValue ( );

			// duration
			m_fDuration         = pDictionary->valueForKey ( "duration"                     )->floatValue ( );

			// blend function 
			m_tBlendFunc.src    = pDictionary->valueForKey ( "blendFuncSource"              )->intValue ( );
			m_tBlendFunc.dst    = pDictionary->valueForKey ( "blendFuncDestination"         )->intValue ( );

			// color
			m_tStartColor.r     = pDictionary->valueForKey ( "startColorRed"                )->floatValue ( );
			m_tStartColor.g     = pDictionary->valueForKey ( "startColorGreen"              )->floatValue ( );
			m_tStartColor.b     = pDictionary->valueForKey ( "startColorBlue"               )->floatValue ( );
			m_tStartColor.a     = pDictionary->valueForKey ( "startColorAlpha"              )->floatValue ( );

			m_tStartColorVar.r  = pDictionary->valueForKey ( "startColorVarianceRed"        )->floatValue ( );
			m_tStartColorVar.g  = pDictionary->valueForKey ( "startColorVarianceGreen"      )->floatValue ( );
			m_tStartColorVar.b  = pDictionary->valueForKey ( "startColorVarianceBlue"       )->floatValue ( );
			m_tStartColorVar.a  = pDictionary->valueForKey ( "startColorVarianceAlpha"      )->floatValue ( );

			m_tEndColor.r       = pDictionary->valueForKey ( "finishColorRed"               )->floatValue ( );
			m_tEndColor.g       = pDictionary->valueForKey ( "finishColorGreen"             )->floatValue ( );
			m_tEndColor.b       = pDictionary->valueForKey ( "finishColorBlue"              )->floatValue ( );
			m_tEndColor.a       = pDictionary->valueForKey ( "finishColorAlpha"             )->floatValue ( );

			m_tEndColorVar.r    = pDictionary->valueForKey ( "finishColorVarianceRed"       )->floatValue ( );
			m_tEndColorVar.g    = pDictionary->valueForKey ( "finishColorVarianceGreen"     )->floatValue ( );
			m_tEndColorVar.b    = pDictionary->valueForKey ( "finishColorVarianceBlue"      )->floatValue ( );
			m_tEndColorVar.a    = pDictionary->valueForKey ( "finishColorVarianceAlpha"     )->floatValue ( );

			// particle size
			m_fStartSize        = pDictionary->valueForKey ( "startParticleSize"            )->floatValue ( );
			m_fStartSizeVar     = pDictionary->valueForKey ( "startParticleSizeVariance"    )->floatValue ( );
			m_fEndSize          = pDictionary->valueForKey ( "finishParticleSize"           )->floatValue ( );
			m_fEndSizeVar       = pDictionary->valueForKey ( "finishParticleSizeVariance"   )->floatValue ( );

			// position
			KDfloat  fX         = pDictionary->valueForKey ( "sourcePositionx"				)->floatValue ( );
			KDfloat  fY         = pDictionary->valueForKey ( "sourcePositiony"				)->floatValue ( );

			this->setPosition ( ccp ( fX, fY ) );
            
			m_tPosVar.x         = pDictionary->valueForKey ( "sourcePositionVariancex"      )->floatValue ( );
			m_tPosVar.y         = pDictionary->valueForKey ( "sourcePositionVariancey"      )->floatValue ( );

			// Spinning
			m_fStartSpin        = pDictionary->valueForKey ( "rotationStart"                )->floatValue ( );
			m_fStartSpinVar     = pDictionary->valueForKey ( "rotationStartVariance"        )->floatValue ( );
			m_fEndSpin          = pDictionary->valueForKey ( "rotationEnd"                  )->floatValue ( );
			m_fEndSpinVar       = pDictionary->valueForKey ( "rotationEndVariance"          )->floatValue ( );

			m_nEmitterMode      = pDictionary->valueForKey ( "emitterType"                  )->intValue ( );

			// Mode A: Gravity + tangential accel + radial accel
			if ( m_nEmitterMode == kCCParticleModeGravity ) 
			{
				// gravity
				m_tModeA.tGravity.x          = pDictionary->valueForKey ( "gravityx"                )->floatValue ( );
				m_tModeA.tGravity.y          = pDictionary->valueForKey ( "gravityy"                )->floatValue ( );

				// speed
				m_tModeA.fSpeed              = pDictionary->valueForKey ( "speed"                   )->floatValue ( );
				m_tModeA.fSpeedVar           = pDictionary->valueForKey ( "speedVariance"           )->floatValue ( );

				// radial acceleration
                m_tModeA.fRadialAccel        = pDictionary->valueForKey ( "radialAcceleration"      )->floatValue ( );
				m_tModeA.fRadialAccelVar     = pDictionary->valueForKey ( "radialAccelVariance"     )->floatValue ( );

				// tangential acceleration
				m_tModeA.fTangentialAccel    = pDictionary->valueForKey ( "tangentialAcceleration"  )->floatValue ( );
				m_tModeA.fTangentialAccelVar = pDictionary->valueForKey ( "tangentialAccelVariance" )->floatValue ( );

				// rotation is dir
                m_tModeA.bRotationIsDir		 = pDictionary->valueForKey ( "rotationIsDir"			)->boolValue  ( );
			}
			// or Mode B: radius movement
			else if ( m_nEmitterMode == kCCParticleModeRadius ) 
			{
				m_tModeB.fStartRadius        = pDictionary->valueForKey ( "maxRadius"               )->floatValue ( );
				m_tModeB.fStartRadiusVar     = pDictionary->valueForKey ( "maxRadiusVariance"       )->floatValue ( );
				m_tModeB.fEndRadius          = pDictionary->valueForKey ( "minRadius"               )->floatValue ( );
				m_tModeB.fEndRadiusVar       = 0.0f;
				m_tModeB.fRotatePerSecond    = pDictionary->valueForKey ( "rotatePerSecond"         )->floatValue ( );
				m_tModeB.fRotatePerSecondVar = pDictionary->valueForKey ( "rotatePerSecondVariance" )->floatValue ( );
			}
			else 
			{
				CCAssert ( KD_FALSE, "Invalid emitterType in config file" );
				CC_BREAK_IF ( KD_TRUE );
			}

			// life span
			m_fLife         = pDictionary->valueForKey ( "particleLifespan"          )->floatValue ( );
			m_fLifeVar      = pDictionary->valueForKey ( "particleLifespanVariance"  )->floatValue ( );

			// emission Rate
			m_fEmissionRate = m_uTotalParticles / m_fLife;

			// don't get the internal texture if a batchNode is used
			if ( !m_pBatchNode )
			{
				// Set a compatible default for the alpha transfer
                m_bOpacityModifyRGB = KD_FALSE;

				// texture        
                // Try to get the texture from the cache
                std::string  sTextureName = pDictionary->valueForKey ( "textureFileName" )->getCString ( );                
                KDsize  rPos = sTextureName.rfind ( '/' );               
				if ( rPos != std::string::npos )
                {
					std::string  sTextureDir = sTextureName.substr ( 0, rPos + 1 );
                    
                    if ( szDirname != KD_NULL && sTextureDir != szDirname )
                    {
                        sTextureName = sTextureName.substr ( rPos + 1 );
						sTextureName = std::string ( szDirname ) + sTextureName;
                    }
                }
				else
				{
                    if ( szDirname != KD_NULL )
                    {
						sTextureName = std::string ( szDirname ) + sTextureName;
                    }
				}

				CCTexture2D*   pTexture  = KD_NULL;

				if ( sTextureName.length ( ) > 0 )
				{
					// set not pop-up message box when load image failed
					KDbool  bNotify = CCFileUtils::sharedFileUtils ( )->isPopupNotify ( );
					CCFileUtils::sharedFileUtils ( )->setPopupNotify ( KD_FALSE );
	                
					pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( sTextureName.c_str ( ) );
	                
					// reset the value of CCImage notify
					CCFileUtils::sharedFileUtils ( )->setPopupNotify ( bNotify );
				}

				if ( pTexture )
				{
					setTexture ( pTexture );
				}
				else
				{
					const KDchar*  pTextureData = pDictionary->valueForKey ( "textureImageData" )->getCString ( );	
					CCAssert ( pTextureData, "" );

					KDsize  uDataLen = kdStrlen ( pTextureData );
					if ( uDataLen != 0 )
					{
						KDubyte*  pBuffer   = KD_NULL;
						KDubyte*  pDeflated = KD_NULL;

						// if it fails, try to get it from the base64-gzipped data	
						KDint  nDecodeLen = base64Decode ( (KDubyte *) pTextureData, uDataLen, &pBuffer );
						CCAssert ( pBuffer != KD_NULL, "CCParticleSystem: error decoding textureImageData" );
						CC_BREAK_IF ( !pBuffer );

						KDint  nDeflatedLen = ZipUtils::ccInflateMemory ( pBuffer, nDecodeLen, &pDeflated );
						CCAssert ( pDeflated != KD_NULL, "CCParticleSystem: error ungzipping textureImageData" );
						CC_BREAK_IF ( !pDeflated );

                        // For android, we should retain it in VolatileTexture::addCCImage which invoked in CCTextureCache::sharedTextureCache()->addUIImage()
						CCImage*  pImage = new CCImage ( );
                        KDbool    bIsOK  = pImage->initWithData ( pDeflated, nDeflatedLen );
                        //CCAssert ( bIsOK, "CCParticleSystem: error init image with Data" );
                        CC_BREAK_IF ( !bIsOK );
                        
                        setTexture ( CCTextureCache::sharedTextureCache ( )->addCCImage ( pImage, sTextureName.c_str ( ) ) );

                        pImage->release ( );

						CC_SAFE_DELETE_ARRAY ( pBuffer );
						CC_SAFE_DELETE_ARRAY ( pDeflated );  
					}
				}

				CCAssert ( this->m_pTexture != KD_NULL, "CCParticleSystem: error loading the texture" );
			}
	 
			bRet = KD_TRUE;
		}

	} while ( 0 );

	return bRet;
}

KDbool CCParticleSystem::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	m_uTotalParticles = uNumberOfParticles;

    CC_SAFE_FREE ( m_pParticles );
	
	m_pParticles = (tCCParticle*) kdCalloc ( m_uTotalParticles, sizeof ( tCCParticle ) );

	if ( !m_pParticles )
	{
		CCLOG ( "Particle system: not enough memory" );
		this->release ( );
		return KD_FALSE;
	}
	m_uAllocatedParticles = uNumberOfParticles;

    if ( m_pBatchNode )
    {
        for ( KDuint i = 0; i < m_uTotalParticles; i++ )
        {
            m_pParticles [ i ].uAtlasIndex = i;
        }
    }

	// default, active
	m_bIsActive = KD_TRUE;

	// default blend function
	m_tBlendFunc.src = CC_BLEND_SRC;
	m_tBlendFunc.dst = CC_BLEND_DST;

	// default movement type;
	m_ePositionType = kCCPositionTypeFree;

	// by default be in mode A:
	m_nEmitterMode = kCCParticleModeGravity;

    // default: modulate
    // XXX: not used
    //    colorModulate = YES;

	m_bIsAutoRemoveOnFinish = KD_FALSE;

    // Optimization: compile updateParticle method
    //updateParticleSel = @selector(updateQuadWithParticle:newPosition:);
    //updateParticleImp = (CC_UPDATE_PARTICLE_IMP) [self methodForSelector:updateParticleSel];
    //for batchNode
    m_bTransformSystemDirty = KD_FALSE;

	// update after action in run!
	this->scheduleUpdateWithPriority ( 1 );

	return KD_TRUE;
}

CCParticleSystem::~CCParticleSystem ( KDvoid )
{
	// Since the scheduler retains the "target (in this case the ParticleSystem)
	// it is not needed to call "unscheduleUpdate" here. In fact, it will be called in "cleanup"
    // unscheduleUpdate ( );

    CC_SAFE_FREE ( m_pParticles );
	CC_SAFE_RELEASE ( m_pTexture );
}

KDbool CCParticleSystem::addParticle ( KDvoid )
{
	if ( this->isFull ( ) )
	{
		return KD_FALSE;
	}

	tCCParticle*  particle = &m_pParticles [ m_uParticleCount ];
	this->initParticle ( particle );
	++m_uParticleCount;

	return KD_TRUE;
}

KDvoid CCParticleSystem::initParticle ( tCCParticle* pParticle )
{
	// timeToLive
	// no negative life. prevent division by 0
	pParticle->fTimeToLive = m_fLife + m_fLifeVar * CCRANDOM_MINUS1_1 ( );
	pParticle->fTimeToLive = KD_MAX ( 0, pParticle->fTimeToLive );

	// position
	pParticle->tPos.x = m_tSourcePosition.x + m_tPosVar.x * CCRANDOM_MINUS1_1 ( );
	pParticle->tPos.y = m_tSourcePosition.y + m_tPosVar.y * CCRANDOM_MINUS1_1 ( );

	// Color
	ccColor4F  tStart;
	tStart.r = clampf ( m_tStartColor.r + m_tStartColorVar.r * CCRANDOM_MINUS1_1 ( ), 0, 1 );
	tStart.g = clampf ( m_tStartColor.g + m_tStartColorVar.g * CCRANDOM_MINUS1_1 ( ), 0, 1 );
	tStart.b = clampf ( m_tStartColor.b + m_tStartColorVar.b * CCRANDOM_MINUS1_1 ( ), 0, 1 );
	tStart.a = clampf ( m_tStartColor.a + m_tStartColorVar.a * CCRANDOM_MINUS1_1 ( ), 0, 1 );

	ccColor4F  tEnd;
	tEnd.r = clampf ( m_tEndColor.r + m_tEndColorVar.r * CCRANDOM_MINUS1_1 ( ), 0, 1 );
	tEnd.g = clampf ( m_tEndColor.g + m_tEndColorVar.g * CCRANDOM_MINUS1_1 ( ), 0, 1 );
	tEnd.b = clampf ( m_tEndColor.b + m_tEndColorVar.b * CCRANDOM_MINUS1_1 ( ), 0, 1 );
	tEnd.a = clampf ( m_tEndColor.a + m_tEndColorVar.a * CCRANDOM_MINUS1_1 ( ), 0, 1 );

	pParticle->tColor = tStart;
	pParticle->tDeltaColor.r = ( tEnd.r - tStart.r ) / pParticle->fTimeToLive;
	pParticle->tDeltaColor.g = ( tEnd.g - tStart.g ) / pParticle->fTimeToLive;
	pParticle->tDeltaColor.b = ( tEnd.b - tStart.b ) / pParticle->fTimeToLive;
	pParticle->tDeltaColor.a = ( tEnd.a - tStart.a ) / pParticle->fTimeToLive;

	// size
	KDfloat  fStartSize = m_fStartSize + m_fStartSizeVar * CCRANDOM_MINUS1_1 ( );
	fStartSize = KD_MAX ( 0, fStartSize ); // No negative value

	pParticle->fSize = fStartSize;

	if ( m_fEndSize == kCCParticleStartSizeEqualToEndSize )
	{
		pParticle->fDeltaSize = 0;
	}
	else
	{
		KDfloat  fEndSize = m_fEndSize + m_fEndSizeVar * CCRANDOM_MINUS1_1 ( );
		fEndSize = KD_MAX ( 0, fEndSize ); // No negative values
        
		pParticle->fDeltaSize = ( fEndSize - fStartSize ) / pParticle->fTimeToLive;
	}

	// rotation
	KDfloat  fStartAngle = m_fStartSpin + m_fStartSpinVar * CCRANDOM_MINUS1_1 ( );
	KDfloat  fEndAngle   = m_fEndSpin + m_fEndSpinVar * CCRANDOM_MINUS1_1 ( );
    
	pParticle->fRotation        = fStartAngle;
	pParticle->fDeltaRotation   = ( fEndAngle - fStartAngle ) / pParticle->fTimeToLive;

	// position
	if ( m_ePositionType == kCCPositionTypeFree )
	{
		pParticle->tStartPos = this->convertToWorldSpace ( CCPointZero );
	}
    else if ( m_ePositionType == kCCPositionTypeRelative )
    {
        pParticle->tStartPos = m_tPosition;
    }

	// direction
	KDfloat  fAngle = CC_DEGREES_TO_RADIANS ( m_fAngle + m_fAngleVar * CCRANDOM_MINUS1_1 ( ) );	

	// Mode Gravity: A
	if ( m_nEmitterMode == kCCParticleModeGravity ) 
	{
		CCPoint  tVector ( kdCosf ( fAngle ), kdSinf ( fAngle ) );
		KDfloat  fSpeed = m_tModeA.fSpeed + m_tModeA.fSpeedVar * CCRANDOM_MINUS1_1 ( );

		// direction
		pParticle->tModeA.tDir = ccpMult ( tVector, fSpeed );

		// radial accel
		pParticle->tModeA.fRadialAccel = m_tModeA.fRadialAccel + m_tModeA.fRadialAccelVar * CCRANDOM_MINUS1_1 ( );

		// tangential accel
		pParticle->tModeA.fTangentialAccel = m_tModeA.fTangentialAccel + m_tModeA.fTangentialAccelVar * CCRANDOM_MINUS1_1 ( );

		// rotation is dir
        if ( m_tModeA.bRotationIsDir )
		{
            pParticle->fRotation = -CC_RADIANS_TO_DEGREES ( ccpToAngle ( pParticle->tModeA.tDir ) );
		}
    }

	// Mode Radius: B
	else 
	{
		// Set the default diameter of the particle from the source position
		KDfloat fStartRadius = m_tModeB.fStartRadius + m_tModeB.fStartRadiusVar * CCRANDOM_MINUS1_1 ( );
		KDfloat   fEndRadius = m_tModeB.fEndRadius + m_tModeB.fEndRadiusVar * CCRANDOM_MINUS1_1 ( );

		pParticle->tModeB.fRadius = fStartRadius;

		if ( m_tModeB.fEndRadius == kCCParticleStartRadiusEqualToEndRadius )
		{
			pParticle->tModeB.fDeltaRadius = 0;
		}
		else
		{
			pParticle->tModeB.fDeltaRadius = ( fEndRadius - fStartRadius ) / pParticle->fTimeToLive;
		}

		pParticle->tModeB.fAngle = fAngle;
		pParticle->tModeB.fDegreesPerSecond = CC_DEGREES_TO_RADIANS ( m_tModeB.fRotatePerSecond + m_tModeB.fRotatePerSecondVar * CCRANDOM_MINUS1_1 ( ) );
	}	
}

KDvoid CCParticleSystem::stopSystem ( KDvoid )
{
	m_bIsActive    = KD_FALSE;
	m_fElapsed     = m_fDuration;
	m_fEmitCounter = 0;
}

KDvoid CCParticleSystem::resetSystem ( KDvoid )
{
	m_bIsActive = KD_TRUE;
	m_fElapsed  = 0;
    
	for ( m_uParticleIdx = 0; m_uParticleIdx < m_uParticleCount; ++m_uParticleIdx )
	{
		tCCParticle*  pParticle = &m_pParticles [ m_uParticleIdx ];
		pParticle->fTimeToLive = 0;
	}
}

KDbool CCParticleSystem::isFull( KDvoid )
{
	return ( m_uParticleCount == m_uTotalParticles );
}

/// ParticleSystem - MainLoop
KDvoid CCParticleSystem::update ( KDfloat fElapsed )
{
	CC_PROFILER_START_CATEGORY ( kCCProfilerCategoryParticles , "CCParticleSystem - update" );

	if ( m_bIsActive && m_fEmissionRate )
	{
		KDfloat  fRate = 1.0f / m_fEmissionRate;
		// issue #1201, prevent bursts of particles, due to too high emitCounter
		if ( m_uParticleCount < m_uTotalParticles )
        {
            m_fEmitCounter += fElapsed;
        }

		while ( m_uParticleCount < m_uTotalParticles && m_fEmitCounter > fRate ) 
		{
			this->addParticle ( );
			m_fEmitCounter -= fRate;
		}

		m_fElapsed += fElapsed;
		if ( m_fDuration != -1 && m_fDuration < m_fElapsed )
		{
			this->stopSystem ( );
		}
	}

	m_uParticleIdx = 0;

	CCPoint  tCurrentPosition = CCPointZero;
	if ( m_ePositionType == kCCPositionTypeFree )
	{
		tCurrentPosition = this->convertToWorldSpace ( CCPointZero );
	}
    else if ( m_ePositionType == kCCPositionTypeRelative )
    {
        tCurrentPosition = m_tPosition;
    }

	if ( m_bVisible )
	{
		while ( m_uParticleIdx < m_uParticleCount )
		{
			tCCParticle*  pParticle = &m_pParticles [ m_uParticleIdx ];

			// life
			pParticle->fTimeToLive -= fElapsed;

			if ( pParticle->fTimeToLive > 0 ) 
			{
				// Mode A: gravity, direction, tangential accel & radial accel
				if ( m_nEmitterMode == kCCParticleModeGravity ) 
				{
					CCPoint  tTemp, tRadial, tTangential;

					tRadial = CCPointZero;
					// radial acceleration
					if ( pParticle->tPos.x || pParticle->tPos.y )
					{
						tRadial = ccpNormalize ( pParticle->tPos );
					}
					tTangential = tRadial;
					tRadial = ccpMult ( tRadial, pParticle->tModeA.fRadialAccel );

					// tangential acceleration
					KDfloat fNewY = tTangential.x;
					tTangential.x = -tTangential.y;
					tTangential.y = fNewY;
					tTangential   = ccpMult ( tTangential, pParticle->tModeA.fTangentialAccel );

					// (gravity + radial + tangential) * dt
					tTemp = ccpAdd ( ccpAdd ( tRadial, tTangential ), m_tModeA.tGravity );
					tTemp = ccpMult ( tTemp, fElapsed );
					pParticle->tModeA.tDir = ccpAdd ( pParticle->tModeA.tDir, tTemp );
					tTemp = ccpMult ( pParticle->tModeA.tDir, fElapsed );
					pParticle->tPos = ccpAdd ( pParticle->tPos, tTemp );
				}

				// Mode B: radius movement
				else
				{				
					// Update the angle and radius of the particle.
					pParticle->tModeB.fAngle  += pParticle->tModeB.fDegreesPerSecond * fElapsed;
					pParticle->tModeB.fRadius += pParticle->tModeB.fDeltaRadius * fElapsed;

					pParticle->tPos.x = -kdCosf ( pParticle->tModeB.fAngle ) * pParticle->tModeB.fRadius;
					pParticle->tPos.y = -kdSinf ( pParticle->tModeB.fAngle ) * pParticle->tModeB.fRadius;
				}

				// color
				pParticle->tColor.r += ( pParticle->tDeltaColor.r * fElapsed );
				pParticle->tColor.g += ( pParticle->tDeltaColor.g * fElapsed );
				pParticle->tColor.b += ( pParticle->tDeltaColor.b * fElapsed );
				pParticle->tColor.a += ( pParticle->tDeltaColor.a * fElapsed );

				// size
				pParticle->fSize += ( pParticle->fDeltaSize * fElapsed );
				pParticle->fSize  = KD_MAX ( 0, pParticle->fSize );

				// angle
				pParticle->fRotation += ( pParticle->fDeltaRotation * fElapsed );

				//
				// update values in quad
				//

				CCPoint	 tNewPos;
				if ( m_ePositionType == kCCPositionTypeFree || m_ePositionType == kCCPositionTypeRelative ) 
				{
					CCPoint  tDiff = ccpSub ( tCurrentPosition, pParticle->tStartPos );
					tNewPos = ccpSub ( pParticle->tPos, tDiff );
				} 
				else
				{
					tNewPos = pParticle->tPos;
				}

                // translate newPos to correct position, since matrix transform isn't performed in batchnode
                // don't update the particle with the new position information, it will interfere with the radius and tangential calculations
                if ( m_pBatchNode )
                {
                    tNewPos.x += m_tPosition.x;
                    tNewPos.y += m_tPosition.y;
                }

				updateQuadWithParticle ( pParticle, tNewPos );
				//updateParticleImp(self, updateParticleSel, p, newPos);

				// update particle counter
				++m_uParticleIdx;

			} 
			else 
			{
				// life < 0
				KDint  nCurrentIndex = pParticle->uAtlasIndex;
				if ( m_uParticleIdx != m_uParticleCount - 1 )
				{
					m_pParticles[ m_uParticleIdx ] = m_pParticles[ m_uParticleCount - 1 ];
				}

				if ( m_pBatchNode )
                {
                    //disable the switched particle
                    m_pBatchNode->disableParticle ( m_uAtlasIndex + nCurrentIndex );

                    //switch indexes
                    m_pParticles [ m_uParticleCount - 1 ].uAtlasIndex = nCurrentIndex;
                }

				--m_uParticleCount;

				if ( m_uParticleCount == 0 && m_bIsAutoRemoveOnFinish )
				{
					this->unscheduleUpdate ( );
					m_pParent->removeChild ( this, KD_TRUE );
					return;
				}
			}
		} // while

		m_bTransformSystemDirty = KD_FALSE;
	}

	if ( !m_pBatchNode )
    {
        postStep ( );
    }

	CC_PROFILER_STOP_CATEGORY ( kCCProfilerCategoryParticles , "CCParticleSystem - update" );
}

KDvoid CCParticleSystem::updateWithNoTime ( KDvoid )
{
    this->update ( 0.0f );
}

KDvoid CCParticleSystem::updateQuadWithParticle ( tCCParticle* pParticle, const CCPoint& tNewPosition )
{
	CC_UNUSED_PARAM ( pParticle );
    CC_UNUSED_PARAM ( tNewPosition );

	// should be overridden
}

KDvoid CCParticleSystem::postStep ( KDvoid )
{
	// should be overridden
}

// ParticleSystem - CCTexture protocol
KDvoid CCParticleSystem::setTexture ( CCTexture2D* pTexture )
{
    if ( m_pTexture != pTexture )
    {
        CC_SAFE_RETAIN  ( pTexture );
        CC_SAFE_RELEASE ( m_pTexture );
        m_pTexture = pTexture;
        updateBlendFunc ( );
    }
}

KDvoid CCParticleSystem::updateBlendFunc ( KDvoid )
{
	CCAssert ( !m_pBatchNode, "Can't change blending functions when the particle is being batched" );

	if ( m_pTexture )
    {
        KDbool  bPremultiplied = m_pTexture->hasPremultipliedAlpha ( );
        
        m_bOpacityModifyRGB = KD_FALSE;
        
        if ( m_pTexture && ( m_tBlendFunc.src == CC_BLEND_SRC && m_tBlendFunc.dst == CC_BLEND_DST ) )
        {
            if ( bPremultiplied )
            {
                m_bOpacityModifyRGB = KD_TRUE;
            }
            else
            {
                m_tBlendFunc.src = GL_SRC_ALPHA;
                m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
            }
        }
    }
}

CCTexture2D* CCParticleSystem::getTexture ( KDvoid )
{
	return m_pTexture;
}

/// ParticleSystem - Additive Blending
KDvoid CCParticleSystem::setBlendAdditive ( KDbool bAdditive )
{
	if ( bAdditive )
	{
		m_tBlendFunc.src = GL_SRC_ALPHA;
		m_tBlendFunc.dst = GL_ONE;
	}
	else
	{
		if ( m_pTexture && ! m_pTexture->hasPremultipliedAlpha ( ) )
		{
			m_tBlendFunc.src = GL_SRC_ALPHA;
			m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
		} 
		else 
		{
			m_tBlendFunc.src = CC_BLEND_SRC;
			m_tBlendFunc.dst = CC_BLEND_DST;
		}
	}
}

KDbool CCParticleSystem::isBlendAdditive ( KDvoid )
{
	return ( m_tBlendFunc.src == GL_SRC_ALPHA && m_tBlendFunc.dst == GL_ONE );
}

/// ParticleSystem - Properties of Gravity Mode 
KDvoid CCParticleSystem::setTangentialAccel ( KDfloat fTangentialAccel )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	m_tModeA.fTangentialAccel = fTangentialAccel;
}

KDfloat CCParticleSystem::getTangentialAccel ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	return m_tModeA.fTangentialAccel;
}

KDvoid CCParticleSystem::setTangentialAccelVar ( KDfloat fTangentialAccelVar )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	m_tModeA.fTangentialAccelVar = fTangentialAccelVar;
}

KDfloat CCParticleSystem::getTangentialAccelVar ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	return m_tModeA.fTangentialAccelVar;
}

KDvoid CCParticleSystem::setRadialAccel ( KDfloat fRadialAccel )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	m_tModeA.fRadialAccel = fRadialAccel;
}

KDfloat CCParticleSystem::getRadialAccel ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	return m_tModeA.fRadialAccel;
}

KDvoid CCParticleSystem::setRadialAccelVar ( KDfloat fRadialAccelVar )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	m_tModeA.fRadialAccelVar = fRadialAccelVar;
}

KDfloat CCParticleSystem::getRadialAccelVar ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	return m_tModeA.fRadialAccelVar;
}

KDvoid CCParticleSystem::setRotationIsDir ( KDbool bDir )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	m_tModeA.bRotationIsDir = bDir;
}

KDbool CCParticleSystem::getRotationIsDir ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	return m_tModeA.bRotationIsDir;
}

KDvoid CCParticleSystem::setGravity ( const CCPoint& tGravity )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	m_tModeA.tGravity = tGravity;
}

const CCPoint& CCParticleSystem::getGravity ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	return m_tModeA.tGravity;
}

KDvoid CCParticleSystem::setSpeed ( KDfloat fSpeed  )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	m_tModeA.fSpeed = fSpeed ;
}

KDfloat CCParticleSystem::getSpeed ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	return m_tModeA.fSpeed;
}

KDvoid CCParticleSystem::setSpeedVar ( KDfloat fSpeedVar )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	m_tModeA.fSpeedVar = fSpeedVar;
}

KDfloat CCParticleSystem::getSpeedVar ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity" );
	return m_tModeA.fSpeedVar;
}

/// ParticleSystem - Properties of Radius Mode
KDvoid CCParticleSystem::setStartRadius ( KDfloat fStartRadius )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	m_tModeB.fStartRadius = fStartRadius;
}

KDfloat CCParticleSystem::getStartRadius ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	return m_tModeB.fStartRadius;
}

KDvoid CCParticleSystem::setStartRadiusVar ( KDfloat fStartRadiusVar )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	m_tModeB.fStartRadiusVar = fStartRadiusVar;
}

KDfloat CCParticleSystem::getStartRadiusVar ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	return m_tModeB.fStartRadiusVar;
}

KDvoid CCParticleSystem::setEndRadius ( KDfloat fEndRadius )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	m_tModeB.fEndRadius = fEndRadius;
}

KDfloat CCParticleSystem::getEndRadius ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	return m_tModeB.fEndRadius;
}

KDvoid CCParticleSystem::setEndRadiusVar ( KDfloat fEndRadiusVar )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	m_tModeB.fEndRadiusVar = fEndRadiusVar;
}

KDfloat CCParticleSystem::getEndRadiusVar ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	return m_tModeB.fEndRadiusVar;
}

KDvoid CCParticleSystem::setRotatePerSecond ( KDfloat fRotatePerSecond )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	m_tModeB.fRotatePerSecond = fRotatePerSecond;
}

KDfloat CCParticleSystem::getRotatePerSecond ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	return m_tModeB.fRotatePerSecond;
}

KDvoid CCParticleSystem::setRotatePerSecondVar ( KDfloat fRotatePerSecondVar )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	m_tModeB.fRotatePerSecondVar = fRotatePerSecondVar;
}

KDfloat CCParticleSystem::getRotatePerSecondVar ( KDvoid )
{
	CCAssert ( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius" );
	return m_tModeB.fRotatePerSecondVar;
}

KDbool CCParticleSystem::isActive ( KDvoid )
{
	return m_bIsActive;
}

KDuint CCParticleSystem::getParticleCount ( KDvoid )
{
	return m_uParticleCount;
}

KDfloat CCParticleSystem::getDuration ( KDvoid )
{
	return m_fDuration;
}

KDvoid CCParticleSystem::setDuration ( KDfloat fDuration )
{
	m_fDuration = fDuration;
}

const CCPoint& CCParticleSystem::getSourcePosition ( KDvoid )
{
	return m_tSourcePosition;
}

KDvoid CCParticleSystem::setSourcePosition ( const CCPoint& tSourcePosition )
{
	m_tSourcePosition = tSourcePosition;
}

const CCPoint& CCParticleSystem::getPosVar ( KDvoid )
{
	return m_tPosVar;
}

KDvoid CCParticleSystem::setPosVar ( const CCPoint& tPosVar )
{
	m_tPosVar = tPosVar;
}

KDfloat CCParticleSystem::getLife ( KDvoid )
{
	return m_fLife;
}

KDvoid CCParticleSystem::setLife ( KDfloat fLife )
{
	m_fLife = fLife;
}

KDfloat CCParticleSystem::getLifeVar ( KDvoid )
{
	return m_fLifeVar;
}

KDvoid CCParticleSystem::setLifeVar ( KDfloat fLifeVar )
{
	m_fLifeVar = fLifeVar;
}

KDfloat CCParticleSystem::getAngle ( KDvoid )
{
	return m_fAngle;
}

KDvoid CCParticleSystem::setAngle ( KDfloat fAngle )
{
	m_fAngle = fAngle;
}

KDfloat CCParticleSystem::getAngleVar ( KDvoid )
{
	return m_fAngleVar;
}

KDvoid CCParticleSystem::setAngleVar ( KDfloat fAngleVar )
{
	m_fAngleVar = fAngleVar;
}

KDfloat CCParticleSystem::getStartSize ( KDvoid )
{
	return m_fStartSize;
}

KDvoid CCParticleSystem::setStartSize ( KDfloat fStartSize )
{
	m_fStartSize = fStartSize;
}

KDfloat CCParticleSystem::getStartSizeVar ( KDvoid )
{
	return m_fStartSizeVar;
}

KDvoid CCParticleSystem::setStartSizeVar ( KDfloat fStartSizeVar )
{
	m_fStartSizeVar = fStartSizeVar;
}

KDfloat CCParticleSystem::getEndSize ( KDvoid )
{
	return m_fEndSize;
}

KDvoid CCParticleSystem::setEndSize ( KDfloat fEndSize )
{
	m_fEndSize = fEndSize;
}

KDfloat CCParticleSystem::getEndSizeVar ( KDvoid )
{
	return m_fEndSizeVar;
}

KDvoid CCParticleSystem::setEndSizeVar ( KDfloat fEndSizeVar )
{
	m_fEndSizeVar = fEndSizeVar;
}

const ccColor4F& CCParticleSystem::getStartColor ( KDvoid )
{
	return m_tStartColor;
}

KDvoid CCParticleSystem::setStartColor ( const ccColor4F& tStartColor )
{
	m_tStartColor = tStartColor;
}

const ccColor4F& CCParticleSystem::getStartColorVar ( KDvoid )
{
	return m_tStartColorVar;
}

KDvoid CCParticleSystem::setStartColorVar ( const ccColor4F& tStartColorVar )
{
	m_tStartColorVar = tStartColorVar;
}

const ccColor4F& CCParticleSystem::getEndColor ( KDvoid )
{
	return m_tEndColor;
}

KDvoid CCParticleSystem::setEndColor ( const ccColor4F& tEndColor )
{
	m_tEndColor = tEndColor;
}

const ccColor4F& CCParticleSystem::getEndColorVar ( KDvoid )
{
	return m_tEndColorVar;
}

KDvoid CCParticleSystem::setEndColorVar ( const ccColor4F& tEndColorVar )
{
	m_tEndColorVar = tEndColorVar;
}

KDfloat CCParticleSystem::getStartSpin ( KDvoid )
{
	return m_fStartSpin;
}

KDvoid CCParticleSystem::setStartSpin ( KDfloat fStartSpin )
{
	m_fStartSpin = fStartSpin;
}

KDfloat CCParticleSystem::getStartSpinVar ( KDvoid )
{
	return m_fStartSpinVar;
}

KDvoid CCParticleSystem::setStartSpinVar ( KDfloat fStartSpinVar )
{
	m_fStartSpinVar = fStartSpinVar;
}

KDfloat CCParticleSystem::getEndSpin ( KDvoid )
{
	return m_fEndSpin;
}

KDvoid CCParticleSystem::setEndSpin ( KDfloat fEndSpin )
{
	m_fEndSpin = fEndSpin;
}

KDfloat CCParticleSystem::getEndSpinVar ( KDvoid )
{
	return m_fEndSpinVar;
}

KDvoid CCParticleSystem::setEndSpinVar ( KDfloat fEndSpinVar )
{
	m_fEndSpinVar = fEndSpinVar;
}

KDfloat CCParticleSystem::getEmissionRate ( KDvoid )
{
	return m_fEmissionRate;
}

KDvoid CCParticleSystem::setEmissionRate ( KDfloat fEmissionRate )
{
	m_fEmissionRate = fEmissionRate;
}

KDuint CCParticleSystem::getTotalParticles ( KDvoid )
{
	return m_uTotalParticles;
}

KDvoid CCParticleSystem::setTotalParticles ( KDuint uTotalParticles )
{
	CCAssert ( uTotalParticles <= m_uAllocatedParticles, "Particle: resizing particle array only supported for quads" );
	m_uTotalParticles = uTotalParticles;
}

ccBlendFunc CCParticleSystem::getBlendFunc ( KDvoid )
{
	return m_tBlendFunc;
}

KDvoid CCParticleSystem::setBlendFunc ( ccBlendFunc tBlendFunc )
{
    if ( m_tBlendFunc.src != tBlendFunc.src || m_tBlendFunc.dst != tBlendFunc.dst )
	{
        m_tBlendFunc = tBlendFunc;
        this->updateBlendFunc ( );
    }
}

KDbool CCParticleSystem::getOpacityModifyRGB ( KDvoid )
{
    return m_bOpacityModifyRGB;
}

KDvoid CCParticleSystem::setOpacityModifyRGB ( KDbool bOpacityModifyRGB )
{
    m_bOpacityModifyRGB = bOpacityModifyRGB;
}

tCCPositionType CCParticleSystem::getPositionType ( KDvoid )
{
	return m_ePositionType;
}

KDvoid CCParticleSystem::setPositionType ( tCCPositionType ePositionType )
{
	m_ePositionType = ePositionType;
}

KDbool CCParticleSystem::isAutoRemoveOnFinish ( KDvoid )
{
	return m_bIsAutoRemoveOnFinish;
}

KDvoid CCParticleSystem::setAutoRemoveOnFinish ( KDbool bIsAutoRemoveOnFinish )
{
	m_bIsAutoRemoveOnFinish = bIsAutoRemoveOnFinish;
}

KDint CCParticleSystem::getEmitterMode ( KDvoid )
{
	return m_nEmitterMode;
}

KDvoid CCParticleSystem::setEmitterMode ( KDint nEmitterMode )
{
	m_nEmitterMode = nEmitterMode;
}

// ParticleSystem - methods for batchNode rendering

CCParticleBatchNode* CCParticleSystem::getBatchNode ( KDvoid )
{
    return m_pBatchNode;
}

KDvoid CCParticleSystem::setBatchNode ( CCParticleBatchNode* pBatchNode )
{
    if ( m_pBatchNode != pBatchNode ) 
	{
        m_pBatchNode = pBatchNode; // weak reference

        if ( pBatchNode ) 
		{
            // each particle needs a unique index
            for ( KDuint i = 0; i < m_uTotalParticles; i++ )
            {
                m_pParticles [ i ].uAtlasIndex = i;
            }
        }
    }
}

// don't use a transform matrix, this is faster
KDvoid CCParticleSystem::setScale ( KDfloat fScale )
{
    m_bTransformSystemDirty = KD_TRUE;
    CCNode::setScale ( fScale );
}

KDvoid CCParticleSystem::setRotation ( KDfloat fRotation )
{
    m_bTransformSystemDirty = KD_TRUE;
    CCNode::setRotation ( fRotation );
}

KDvoid CCParticleSystem::setScaleX ( KDfloat fScaleX )
{
    m_bTransformSystemDirty = KD_TRUE;
    CCNode::setScaleX ( fScaleX );
}

KDvoid CCParticleSystem::setScaleY ( KDfloat fScaleY )
{
    m_bTransformSystemDirty = KD_TRUE;
    CCNode::setScaleY ( fScaleY );
}


NS_CC_END

