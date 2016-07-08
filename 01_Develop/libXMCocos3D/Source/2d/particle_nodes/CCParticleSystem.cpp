/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleSystem.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

// ideas taken from:
//     . The ocean spray in your face [Jeff Lander]
//        http://www.double.co.nz/dust/col0798.pdf
//     . Building an Advanced Particle System [John van der Burg]
//        http://www.gamasutra.com/features/20000623/vanderburg_01.htm
//   . LOVE game engine
//        http://love2d.org/
//
//
// Radius mode support, from 71 squared
//        http://particledesigner.71squared.com/
//
// IMPORTANT: Particle Designer is supported by cocos2d, but
// 'Radius Mode' in Particle Designer uses a fixed emit rate of 30 hz. Since that can't be guaranteed in cocos2d,
//  cocos2d uses a another approach, but the results are almost identical. 
//

#include "2d/particle_nodes/CCParticleSystem.h"
#include "2d/particle_nodes/CCParticleBatchNode.h"
#include "ccTypes.h"
#include "2d/textures/CCTextureCache.h"
#include "2d/textures/CCTextureAtlas.h"
#include "support/base64.h"
#include "platform/CCFileUtils.h"
#include "platform/CCImage.h"
#include "support/zip_support/ZipUtils.h"
#include "2d/CCDirector.h"
#include "support/CCProfiling.h"

using namespace std;


NS_CC_BEGIN

// ideas taken from:
//     . The ocean spray in your face [Jeff Lander]
//        http://www.double.co.nz/dust/col0798.pdf
//     . Building an Advanced Particle System [John van der Burg]
//        http://www.gamasutra.com/features/20000623/vanderburg_01.htm
//   . LOVE game engine
//        http://love2d.org/
//
//
// Radius mode support, from 71 squared
//        http://particledesigner.71squared.com/
//
// IMPORTANT: Particle Designer is supported by cocos2d, but
// 'Radius Mode' in Particle Designer uses a fixed emit rate of 30 hz. Since that can't be guaranteed in cocos2d,
//  cocos2d uses a another approach, but the results are almost identical. 
//

ParticleSystem::ParticleSystem()
: m_bIsBlendAdditive(false)
, m_bIsAutoRemoveOnFinish(false)
, m_sPlistFile("")
, m_fElapsed(0)
, m_pParticles(nullptr)
, m_sConfigName("")
, m_fEmitCounter(0)
, m_nParticleIdx(0)
, m_pBatchNode(nullptr)
, m_nAtlasIndex(0)
, m_bTransformSystemDirty(false)
, m_nAllocatedParticles(0)
, m_bIsActive(true)
, m_nParticleCount(0)
, m_fDuration(0)
, m_tSourcePosition(Point::ZERO)
, m_tPosVar(Point::ZERO)
, m_fLife(0)
, m_fLifeVar(0)
, m_fAngle(0)
, m_fAngleVar(0)
, m_eEmitterMode(Mode::GRAVITY)
, m_fStartSize(0)
, m_fStartSizeVar(0)
, m_fEndSize(0)
, m_fEndSizeVar(0)
, m_fStartSpin(0)
, m_fStartSpinVar(0)
, m_fEndSpin(0)
, m_fEndSpinVar(0)
, m_fEmissionRate(0)
, m_nTotalParticles(0)
, m_pTexture(nullptr)
, m_tBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED)
, m_bOpacityModifyRGB(false)
, m_nCoordFlippedY(0)
, m_ePositionType(PositionType::FREE)
{
    m_tModeA.tGravity = Point::ZERO;
    m_tModeA.fSpeed = 0;
    m_tModeA.fSpeedVar = 0;
    m_tModeA.fTangentialAccel = 0;
    m_tModeA.fTangentialAccelVar = 0;
    m_tModeA.fRadialAccel = 0;
    m_tModeA.fRadialAccelVar = 0;
    m_tModeA.bRotationIsDir = false;
    m_tModeB.fStartRadius = 0;
    m_tModeB.fStartRadiusVar = 0;
    m_tModeB.fEndRadius = 0;
    m_tModeB.fEndRadiusVar = 0;            
    m_tModeB.fRotatePerSecond = 0;
    m_tModeB.fRotatePerSecondVar = 0;
}
// implementation ParticleSystem

ParticleSystem * ParticleSystem::create(const std::string& plistFile)
{
    ParticleSystem *pRet = new ParticleSystem();
    if (pRet && pRet->initWithFile(plistFile))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

ParticleSystem* ParticleSystem::createWithTotalParticles(unsigned int numberOfParticles)
{
    ParticleSystem *pRet = new ParticleSystem();
    if (pRet && pRet->initWithTotalParticles(numberOfParticles))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool ParticleSystem::init()
{
    return initWithTotalParticles(150);
}

bool ParticleSystem::initWithFile(const std::string& plistFile)
{
    bool bRet = false;
    m_sPlistFile = FileUtils::getInstance()->fullPathForFilename(plistFile);
    Dictionary *dict = Dictionary::createWithContentsOfFileThreadSafe(m_sPlistFile.c_str());

    CCASSERT( dict != NULL, "Particles: file not found");
    
    // XXX compute path from a path, should define a function somewhere to do it
    string listFilePath = plistFile;
    if (listFilePath.find('/') != string::npos)
    {
        listFilePath = listFilePath.substr(0, listFilePath.rfind('/') + 1);
        bRet = this->initWithDictionary(dict, listFilePath.c_str());
    }
    else
    {
        bRet = this->initWithDictionary(dict, "");
    }
    
    dict->release();

    return bRet;
}

bool ParticleSystem::initWithDictionary(Dictionary *dictionary)
{
    return initWithDictionary(dictionary, "");
}

bool ParticleSystem::initWithDictionary(Dictionary *dictionary, const std::string& dirname)
{
    bool bRet = false;
    unsigned char *buffer = NULL;
    unsigned char *deflated = NULL;
    Image *image = NULL;
    do 
    {
        int maxParticles = dictionary->valueForKey("maxParticles")->intValue();
        // self, not super
        if(this->initWithTotalParticles(maxParticles))
        {
            // Emitter name in particle designer 2.0
            const String * configNameConstStr = dictionary->valueForKey("configName");
            m_sConfigName = configNameConstStr->getCString();

            // angle
            m_fAngle = dictionary->valueForKey("angle")->floatValue();
            m_fAngleVar = dictionary->valueForKey("angleVariance")->floatValue();

            // duration
            m_fDuration = dictionary->valueForKey("duration")->floatValue();

            // blend function 
            if (m_sConfigName.length()>0)
            {
                m_tBlendFunc.src = dictionary->valueForKey("blendFuncSource")->floatValue();
            }
            else
            {
                m_tBlendFunc.src = dictionary->valueForKey("blendFuncSource")->intValue();
            }
            m_tBlendFunc.dst = dictionary->valueForKey("blendFuncDestination")->intValue();

            // color
            m_tStartColor.r = dictionary->valueForKey("startColorRed")->floatValue();
            m_tStartColor.g = dictionary->valueForKey("startColorGreen")->floatValue();
            m_tStartColor.b = dictionary->valueForKey("startColorBlue")->floatValue();
            m_tStartColor.a = dictionary->valueForKey("startColorAlpha")->floatValue();

            m_tStartColorVar.r = dictionary->valueForKey("startColorVarianceRed")->floatValue();
            m_tStartColorVar.g = dictionary->valueForKey("startColorVarianceGreen")->floatValue();
            m_tStartColorVar.b = dictionary->valueForKey("startColorVarianceBlue")->floatValue();
            m_tStartColorVar.a = dictionary->valueForKey("startColorVarianceAlpha")->floatValue();

            m_tEndColor.r = dictionary->valueForKey("finishColorRed")->floatValue();
            m_tEndColor.g = dictionary->valueForKey("finishColorGreen")->floatValue();
            m_tEndColor.b = dictionary->valueForKey("finishColorBlue")->floatValue();
            m_tEndColor.a = dictionary->valueForKey("finishColorAlpha")->floatValue();

            m_tEndColorVar.r = dictionary->valueForKey("finishColorVarianceRed")->floatValue();
            m_tEndColorVar.g = dictionary->valueForKey("finishColorVarianceGreen")->floatValue();
            m_tEndColorVar.b = dictionary->valueForKey("finishColorVarianceBlue")->floatValue();
            m_tEndColorVar.a = dictionary->valueForKey("finishColorVarianceAlpha")->floatValue();

            // particle size
            m_fStartSize = dictionary->valueForKey("startParticleSize")->floatValue();
            m_fStartSizeVar = dictionary->valueForKey("startParticleSizeVariance")->floatValue();
            m_fEndSize = dictionary->valueForKey("finishParticleSize")->floatValue();
            m_fEndSizeVar = dictionary->valueForKey("finishParticleSizeVariance")->floatValue();

            // position
            float x = dictionary->valueForKey("sourcePositionx")->floatValue();
            float y = dictionary->valueForKey("sourcePositiony")->floatValue();
            this->setPosition( Point(x,y) );            
            m_tPosVar.x = dictionary->valueForKey("sourcePositionVariancex")->floatValue();
            m_tPosVar.y = dictionary->valueForKey("sourcePositionVariancey")->floatValue();

            // Spinning
            m_fStartSpin = dictionary->valueForKey("rotationStart")->floatValue();
            m_fStartSpinVar = dictionary->valueForKey("rotationStartVariance")->floatValue();
            m_fEndSpin= dictionary->valueForKey("rotationEnd")->floatValue();
            m_fEndSpinVar= dictionary->valueForKey("rotationEndVariance")->floatValue();

            m_eEmitterMode = (Mode) dictionary->valueForKey("emitterType")->intValue();

            // Mode A: Gravity + tangential accel + radial accel
            if (m_eEmitterMode == Mode::GRAVITY)
            {
                // gravity
                m_tModeA.tGravity.x = dictionary->valueForKey("gravityx")->floatValue();
                m_tModeA.tGravity.y = dictionary->valueForKey("gravityy")->floatValue();

                // speed
                m_tModeA.fSpeed = dictionary->valueForKey("speed")->floatValue();
                m_tModeA.fSpeedVar = dictionary->valueForKey("speedVariance")->floatValue();

                // radial acceleration
                m_tModeA.fRadialAccel = dictionary->valueForKey("radialAcceleration")->floatValue();
                m_tModeA.fRadialAccelVar = dictionary->valueForKey("radialAccelVariance")->floatValue();

                // tangential acceleration
                m_tModeA.fTangentialAccel = dictionary->valueForKey("tangentialAcceleration")->floatValue();
                m_tModeA.fTangentialAccelVar = dictionary->valueForKey("tangentialAccelVariance")->floatValue();
                
                // rotation is dir
                m_tModeA.bRotationIsDir = dictionary->valueForKey("rotationIsDir")->boolValue();
            }

            // or Mode B: radius movement
            else if (m_eEmitterMode == Mode::RADIUS)
            {
                if (m_sConfigName.length()>0)
                {
                    m_tModeB.fStartRadius = dictionary->valueForKey("maxRadius")->intValue();
                }
                else
                {
                    m_tModeB.fStartRadius = dictionary->valueForKey("maxRadius")->floatValue();
                }
                m_tModeB.fStartRadiusVar = dictionary->valueForKey("maxRadiusVariance")->floatValue();
                if (m_sConfigName.length()>0)
                {
                    m_tModeB.fEndRadius = dictionary->valueForKey("minRadius")->intValue();
                }
                else
                {
                    m_tModeB.fEndRadius = dictionary->valueForKey("minRadius")->floatValue();
                }
                m_tModeB.fEndRadiusVar = 0.0f;
               if (m_sConfigName.length()>0)
                {
                    m_tModeB.fRotatePerSecond = dictionary->valueForKey("rotatePerSecond")->intValue();
                }
                else
                {
                    m_tModeB.fRotatePerSecond = dictionary->valueForKey("rotatePerSecond")->floatValue();
                }
                m_tModeB.fRotatePerSecondVar = dictionary->valueForKey("rotatePerSecondVariance")->floatValue();

            } else {
                CCASSERT( false, "Invalid emitterType in config file");
                CC_BREAK_IF(true);
            }

            // life span
            m_fLife = dictionary->valueForKey("particleLifespan")->floatValue();
            m_fLifeVar = dictionary->valueForKey("particleLifespanVariance")->floatValue();

            // emission Rate
            m_fEmissionRate = m_nTotalParticles / m_fLife;

            //don't get the internal texture if a batchNode is used
            if (!m_pBatchNode)
            {
                // Set a compatible default for the alpha transfer
                m_bOpacityModifyRGB = false;

                // texture        
                // Try to get the texture from the cache
                std::string textureName = dictionary->valueForKey("textureFileName")->getCString();
                
                size_t rPos = textureName.rfind('/');
               
                if (rPos != string::npos)
                {
                    string textureDir = textureName.substr(0, rPos + 1);
                    
                    if (dirname.size()>0 && textureDir != dirname)
                    {
                        textureName = textureName.substr(rPos+1);
                        textureName = dirname + textureName;
                    }
                }
                else
                {
                    if (dirname.size()>0)
                    {
                        textureName = dirname + textureName;
                    }
                }
                
                Texture2D *tex = NULL;
                
                if (textureName.length() > 0)
                {
                    // set not pop-up message box when load image failed
                    bool bNotify = FileUtils::getInstance()->isPopupNotify();
                    FileUtils::getInstance()->setPopupNotify(false);
                    tex = Director::getInstance()->getTextureCache()->addImage(textureName.c_str());
                    // reset the value of UIImage notify
                    FileUtils::getInstance()->setPopupNotify(bNotify);
                }
                
                if (tex)
                {
                    setTexture(tex);
                }
                else
                {                        
                    const char *textureData = dictionary->valueForKey("textureImageData")->getCString();
                    CCASSERT(textureData, "");
                    
                    long dataLen = strlen(textureData);
                    if(dataLen != 0)
                    {
                        // if it fails, try to get it from the base64-gzipped data    
                        int decodeLen = base64Decode((unsigned char*)textureData, (unsigned int)dataLen, &buffer);
                        CCASSERT( buffer != NULL, "CCParticleSystem: error decoding textureImageData");
                        CC_BREAK_IF(!buffer);
                        
                        int deflatedLen = ZipUtils::inflateMemory(buffer, decodeLen, &deflated);
                        CCASSERT( deflated != NULL, "CCParticleSystem: error ungzipping textureImageData");
                        CC_BREAK_IF(!deflated);
                        
                        // For android, we should retain it in VolatileTexture::addImage which invoked in Director::getInstance()->getTextureCache()->addUIImage()
                        image = new Image();
                        bool isOK = image->initWithImageData(deflated, deflatedLen);
                        CCASSERT(isOK, "CCParticleSystem: error init image with Data");
                        CC_BREAK_IF(!isOK);
                        
                        setTexture(Director::getInstance()->getTextureCache()->addImage(image, textureName.c_str()));

                        image->release();
                    }
                }
                if (m_sConfigName.length()>0)
                {
                  m_nCoordFlippedY = dictionary->valueForKey("yCoordFlipped")->intValue();
                }
                CCASSERT( this->m_pTexture != NULL, "CCParticleSystem: error loading the texture");
            }
            bRet = true;
        }
    } while (0);
    kdFree(buffer);
    kdFree(deflated);
    return bRet;
}

bool ParticleSystem::initWithTotalParticles(unsigned int numberOfParticles)
{
    m_nTotalParticles = numberOfParticles;

    CC_SAFE_FREE(m_pParticles);
    
    m_pParticles = (tParticle*) kdCalloc(m_nTotalParticles, sizeof(tParticle));

    if( ! m_pParticles )
    {
        CCLOG("Particle system: not enough memory");
        this->release();
        return false;
    }
    m_nAllocatedParticles = numberOfParticles;

    if (m_pBatchNode)
    {
        for (int i = 0; i < m_nTotalParticles; i++)
        {
            m_pParticles[i].atlasIndex=i;
        }
    }
    // default, active
    m_bIsActive = true;

    // default blend function
    m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

    // default movement type;
    m_ePositionType = PositionType::FREE;

    // by default be in mode A:
    m_eEmitterMode = Mode::GRAVITY;

    // default: modulate
    // XXX: not used
    //    colorModulate = YES;

    m_bIsAutoRemoveOnFinish = false;

    // Optimization: compile updateParticle method
    //updateParticleSel = @selector(updateQuadWithParticle:newPosition:);
    //updateParticleImp = (CC_UPDATE_PARTICLE_IMP) [self methodForSelector:updateParticleSel];
    //for batchNode
    m_bTransformSystemDirty = false;
    // update after action in run!
    this->scheduleUpdateWithPriority(1);

    return true;
}

ParticleSystem::~ParticleSystem()
{
    // Since the scheduler retains the "target (in this case the ParticleSystem)
	// it is not needed to call "unscheduleUpdate" here. In fact, it will be called in "cleanup"
    //unscheduleUpdate();
    CC_SAFE_FREE(m_pParticles);
    CC_SAFE_RELEASE(m_pTexture);
}

bool ParticleSystem::addParticle()
{
    if (this->isFull())
    {
        return false;
    }

    tParticle * particle = &m_pParticles[ m_nParticleCount ];
    this->initParticle(particle);
    ++m_nParticleCount;

    return true;
}

void ParticleSystem::initParticle(tParticle* particle)
{
    // timeToLive
    // no negative life. prevent division by 0
    particle->timeToLive = m_fLife + m_fLifeVar * CCRANDOM_MINUS1_1();
    particle->timeToLive = KD_MAX(0, particle->timeToLive);

    // position
    particle->pos.x = m_tSourcePosition.x + m_tPosVar.x * CCRANDOM_MINUS1_1();

    particle->pos.y = m_tSourcePosition.y + m_tPosVar.y * CCRANDOM_MINUS1_1();


    // Color
    Color4F start;
    start.r = clampf(m_tStartColor.r + m_tStartColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
    start.g = clampf(m_tStartColor.g + m_tStartColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
    start.b = clampf(m_tStartColor.b + m_tStartColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
    start.a = clampf(m_tStartColor.a + m_tStartColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);

    Color4F end;
    end.r = clampf(m_tEndColor.r + m_tEndColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
    end.g = clampf(m_tEndColor.g + m_tEndColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
    end.b = clampf(m_tEndColor.b + m_tEndColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
    end.a = clampf(m_tEndColor.a + m_tEndColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);

    particle->color = start;
    particle->deltaColor.r = (end.r - start.r) / particle->timeToLive;
    particle->deltaColor.g = (end.g - start.g) / particle->timeToLive;
    particle->deltaColor.b = (end.b - start.b) / particle->timeToLive;
    particle->deltaColor.a = (end.a - start.a) / particle->timeToLive;

    // size
    float startS = m_fStartSize + m_fStartSizeVar * CCRANDOM_MINUS1_1();
    startS = KD_MAX(0, startS); // No negative value

    particle->size = startS;

    if (m_fEndSize == START_SIZE_EQUAL_TO_END_SIZE)
    {
        particle->deltaSize = 0;
    }
    else
    {
        float endS = m_fEndSize + m_fEndSizeVar * CCRANDOM_MINUS1_1();
        endS = KD_MAX(0, endS); // No negative values
        particle->deltaSize = (endS - startS) / particle->timeToLive;
    }

    // rotation
    float startA = m_fStartSpin + m_fStartSpinVar * CCRANDOM_MINUS1_1();
    float endA = m_fEndSpin + m_fEndSpinVar * CCRANDOM_MINUS1_1();
    particle->rotation = startA;
    particle->deltaRotation = (endA - startA) / particle->timeToLive;

    // position
    if (m_ePositionType == PositionType::FREE)
    {
        particle->startPos = this->convertToWorldSpace(Point::ZERO);
    }
    else if (m_ePositionType == PositionType::RELATIVE)
    {
        particle->startPos = m_tPosition;
    }

    // direction
    float a = CC_DEGREES_TO_RADIANS( m_fAngle + m_fAngleVar * CCRANDOM_MINUS1_1() );    

    // Mode Gravity: A
    if (m_eEmitterMode == Mode::GRAVITY)
    {
        Point v(kdCosf( a ), kdSinf( a ));
        float s = m_tModeA.fSpeed + m_tModeA.fSpeedVar * CCRANDOM_MINUS1_1();

        // direction
        particle->modeA.dir = v * s ;

        // radial accel
        particle->modeA.radialAccel = m_tModeA.fRadialAccel + m_tModeA.fRadialAccelVar * CCRANDOM_MINUS1_1();
 

        // tangential accel
        particle->modeA.tangentialAccel = m_tModeA.fTangentialAccel + m_tModeA.fTangentialAccelVar * CCRANDOM_MINUS1_1();

        // rotation is dir
        if(m_tModeA.bRotationIsDir)
            particle->rotation = -CC_RADIANS_TO_DEGREES(particle->modeA.dir.getAngle());
    }

    // Mode Radius: B
    else 
    {
        // Set the default diameter of the particle from the source position
        float startRadius = m_tModeB.fStartRadius + m_tModeB.fStartRadiusVar * CCRANDOM_MINUS1_1();
        float endRadius = m_tModeB.fEndRadius + m_tModeB.fEndRadiusVar * CCRANDOM_MINUS1_1();

        particle->modeB.radius = startRadius;

        if (m_tModeB.fEndRadius == START_RADIUS_EQUAL_TO_END_RADIUS)
        {
            particle->modeB.deltaRadius = 0;
        }
        else
        {
            particle->modeB.deltaRadius = (endRadius - startRadius) / particle->timeToLive;
        }

        particle->modeB.angle = a;
        particle->modeB.degreesPerSecond = CC_DEGREES_TO_RADIANS(m_tModeB.fRotatePerSecond + m_tModeB.fRotatePerSecondVar * CCRANDOM_MINUS1_1());
    }    
}

void ParticleSystem::stopSystem()
{
    m_bIsActive = false;
    m_fElapsed = m_fDuration;
    m_fEmitCounter = 0;
}

void ParticleSystem::resetSystem()
{
    m_bIsActive = true;
    m_fElapsed = 0;
    for (m_nParticleIdx = 0; m_nParticleIdx < m_nParticleCount; ++m_nParticleIdx)
    {
        tParticle *p = &m_pParticles[m_nParticleIdx];
        p->timeToLive = 0;
    }
}
bool ParticleSystem::isFull()
{
    return (m_nParticleCount == m_nTotalParticles);
}

// ParticleSystem - MainLoop
void ParticleSystem::update(float dt)
{
    CC_PROFILER_START_CATEGORY(kProfilerCategoryParticles , "CCParticleSystem - update");

    if (m_bIsActive && m_fEmissionRate)
    {
        float rate = 1.0f / m_fEmissionRate;
        //issue #1201, prevent bursts of particles, due to too high emitCounter
        if (m_nParticleCount < m_nTotalParticles)
        {
            m_fEmitCounter += dt;
        }
        
        while (m_nParticleCount < m_nTotalParticles && m_fEmitCounter > rate) 
        {
            this->addParticle();
            m_fEmitCounter -= rate;
        }

        m_fElapsed += dt;
        if (m_fDuration != -1 && m_fDuration < m_fElapsed)
        {
            this->stopSystem();
        }
    }

    m_nParticleIdx = 0;

    Point currentPosition = Point::ZERO;
    if (m_ePositionType == PositionType::FREE)
    {
        currentPosition = this->convertToWorldSpace(Point::ZERO);
    }
    else if (m_ePositionType == PositionType::RELATIVE)
    {
        currentPosition = m_tPosition;
    }

    if (m_bVisible)
    {
        while (m_nParticleIdx < m_nParticleCount)
        {
            tParticle *p = &m_pParticles[m_nParticleIdx];

            // life
            p->timeToLive -= dt;

            if (p->timeToLive > 0) 
            {
                // Mode A: gravity, direction, tangential accel & radial accel
                if (m_eEmitterMode == Mode::GRAVITY)
                {
                    Point tmp, radial, tangential;

                    radial = Point::ZERO;
                    // radial acceleration
                    if (p->pos.x || p->pos.y)
                    {
                        radial = p->pos.normalize();
                    }
                    tangential = radial;
                    radial = radial * p->modeA.radialAccel;

                    // tangential acceleration
                    float newy = tangential.x;
                    tangential.x = -tangential.y;
                    tangential.y = newy;
                    tangential = tangential * p->modeA.tangentialAccel;

                    // (gravity + radial + tangential) * dt
                    tmp = radial + tangential + m_tModeA.tGravity;
                    tmp = tmp * dt;
                    p->modeA.dir = p->modeA.dir + tmp;
					if (m_sConfigName.length()>0)
					{
						if (m_nCoordFlippedY == -1)
						{
							 tmp = p->modeA.dir * dt;
						}
						else
						{
							 tmp = p->modeA.dir * -dt;
						}
					}
					else
					{
						 tmp = p->modeA.dir * dt;
					}
                    p->pos = p->pos + tmp;
                }

                // Mode B: radius movement
                else 
                {                
                    // Update the angle and radius of the particle.
                    p->modeB.angle += p->modeB.degreesPerSecond * dt;
                    p->modeB.radius += p->modeB.deltaRadius * dt;

                    p->pos.x = - kdCosf(p->modeB.angle) * p->modeB.radius;
                    p->pos.y = - kdSinf(p->modeB.angle) * p->modeB.radius;
                    if (m_nCoordFlippedY == 1)
                    {
                      p->pos.y = -p->pos.y;
                    }
				}

                // color
                p->color.r += (p->deltaColor.r * dt);
                p->color.g += (p->deltaColor.g * dt);
                p->color.b += (p->deltaColor.b * dt);
                p->color.a += (p->deltaColor.a * dt);

                // size
                p->size += (p->deltaSize * dt);
                p->size = KD_MAX( 0, p->size );

                // angle
                p->rotation += (p->deltaRotation * dt);

                //
                // update values in quad
                //

                Point    newPos;

                if (m_ePositionType == PositionType::FREE || m_ePositionType == PositionType::RELATIVE)
                {
                    Point diff = currentPosition - p->startPos;
                    newPos = p->pos - diff;
                } 
                else
                {
                    newPos = p->pos;
                }

                // translate newPos to correct position, since matrix transform isn't performed in batchnode
                // don't update the particle with the new position information, it will interfere with the radius and tangential calculations
                if (m_pBatchNode)
                {
                    newPos.x+=m_tPosition.x;
                    newPos.y+=m_tPosition.y;
                }

                updateQuadWithParticle(p, newPos);
                //updateParticleImp(self, updateParticleSel, p, newPos);

                // update particle counter
                ++m_nParticleIdx;
            } 
            else 
            {
                // life < 0
                int currentIndex = p->atlasIndex;
                if( m_nParticleIdx != m_nParticleCount-1 )
                {
                    m_pParticles[m_nParticleIdx] = m_pParticles[m_nParticleCount-1];
                }
                if (m_pBatchNode)
                {
                    //disable the switched particle
                    m_pBatchNode->disableParticle(m_nAtlasIndex+currentIndex);

                    //switch indexes
                    m_pParticles[m_nParticleCount-1].atlasIndex = currentIndex;
                }


                --m_nParticleCount;

                if( m_nParticleCount == 0 && m_bIsAutoRemoveOnFinish )
                {
                    this->unscheduleUpdate();
                    m_pParent->removeChild(this, true);
                    return;
                }
            }
        } //while
        m_bTransformSystemDirty = false;
    }
    if (! m_pBatchNode)
    {
        postStep();
    }

    CC_PROFILER_STOP_CATEGORY(kProfilerCategoryParticles , "CCParticleSystem - update");
}

void ParticleSystem::updateWithNoTime(void)
{
    this->update(0.0f);
}

void ParticleSystem::updateQuadWithParticle(tParticle* particle, const Point& newPosition)
{
    CC_UNUSED_PARAM(particle);
    CC_UNUSED_PARAM(newPosition);
    // should be overridden
}

void ParticleSystem::postStep()
{
    // should be overridden
}

// ParticleSystem - Texture protocol
void ParticleSystem::setTexture(Texture2D* var)
{
    if (m_pTexture != var)
    {
        CC_SAFE_RETAIN(var);
        CC_SAFE_RELEASE(m_pTexture);
        m_pTexture = var;
        updateBlendFunc();
    }
}

void ParticleSystem::updateBlendFunc()
{
    CCASSERT(! m_pBatchNode, "Can't change blending functions when the particle is being batched");

    if(m_pTexture)
    {
        bool premultiplied = m_pTexture->hasPremultipliedAlpha();
        
        m_bOpacityModifyRGB = false;
        
        if( m_pTexture && ( m_tBlendFunc.src == CC_BLEND_SRC && m_tBlendFunc.dst == CC_BLEND_DST ) )
        {
            if( premultiplied )
            {
                m_bOpacityModifyRGB = true;
            }
            else
            {
                m_tBlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
            }
        }
    }
}

Texture2D * ParticleSystem::getTexture() const
{
    return m_pTexture;
}

// ParticleSystem - Additive Blending
void ParticleSystem::setBlendAdditive(bool additive)
{
    if( additive )
    {
        m_tBlendFunc = BlendFunc::ADDITIVE;
    }
    else
    {
        if( m_pTexture && ! m_pTexture->hasPremultipliedAlpha() )
            m_tBlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
        else 
            m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
    }
}

bool ParticleSystem::isBlendAdditive() const
{
    return( m_tBlendFunc.src == GL_SRC_ALPHA && m_tBlendFunc.dst == GL_ONE);
}

// ParticleSystem - Properties of Gravity Mode 
void ParticleSystem::setTangentialAccel(float t)
{
    CCASSERT( m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    m_tModeA.fTangentialAccel = t;
}

float ParticleSystem::getTangentialAccel() const
{
    CCASSERT( m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    return m_tModeA.fTangentialAccel;
}

void ParticleSystem::setTangentialAccelVar(float t)
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    m_tModeA.fTangentialAccelVar = t;
}

float ParticleSystem::getTangentialAccelVar() const
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    return m_tModeA.fTangentialAccelVar;
}    

void ParticleSystem::setRadialAccel(float t)
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    m_tModeA.fRadialAccel = t;
}

float ParticleSystem::getRadialAccel() const
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    return m_tModeA.fRadialAccel;
}

void ParticleSystem::setRadialAccelVar(float t)
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    m_tModeA.fRadialAccelVar = t;
}

float ParticleSystem::getRadialAccelVar() const
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    return m_tModeA.fRadialAccelVar;
}

void ParticleSystem::setRotationIsDir(bool t)
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    m_tModeA.bRotationIsDir = t;
}

bool ParticleSystem::getRotationIsDir() const
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    return m_tModeA.bRotationIsDir;
}

void ParticleSystem::setGravity(const Point& g)
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    m_tModeA.tGravity = g;
}

const Point& ParticleSystem::getGravity()
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    return m_tModeA.tGravity;
}

void ParticleSystem::setSpeed(float speed)
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    m_tModeA.fSpeed = speed;
}

float ParticleSystem::getSpeed() const
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    return m_tModeA.fSpeed;
}

void ParticleSystem::setSpeedVar(float speedVar)
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    m_tModeA.fSpeedVar = speedVar;
}

float ParticleSystem::getSpeedVar() const
{
    CCASSERT(m_eEmitterMode == Mode::GRAVITY, "Particle Mode should be Gravity");
    return m_tModeA.fSpeedVar;
}

// ParticleSystem - Properties of Radius Mode
void ParticleSystem::setStartRadius(float startRadius)
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    m_tModeB.fStartRadius = startRadius;
}

float ParticleSystem::getStartRadius() const
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    return m_tModeB.fStartRadius;
}

void ParticleSystem::setStartRadiusVar(float startRadiusVar)
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    m_tModeB.fStartRadiusVar = startRadiusVar;
}

float ParticleSystem::getStartRadiusVar() const
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    return m_tModeB.fStartRadiusVar;
}

void ParticleSystem::setEndRadius(float endRadius)
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    m_tModeB.fEndRadius = endRadius;
}

float ParticleSystem::getEndRadius() const
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    return m_tModeB.fEndRadius;
}

void ParticleSystem::setEndRadiusVar(float endRadiusVar)
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    m_tModeB.fEndRadiusVar = endRadiusVar;
}

float ParticleSystem::getEndRadiusVar() const
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    return m_tModeB.fEndRadiusVar;
}

void ParticleSystem::setRotatePerSecond(float degrees)
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    m_tModeB.fRotatePerSecond = degrees;
}

float ParticleSystem::getRotatePerSecond() const
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    return m_tModeB.fRotatePerSecond;
}

void ParticleSystem::setRotatePerSecondVar(float degrees)
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    m_tModeB.fRotatePerSecondVar = degrees;
}

float ParticleSystem::getRotatePerSecondVar() const
{
    CCASSERT(m_eEmitterMode == Mode::RADIUS, "Particle Mode should be Radius");
    return m_tModeB.fRotatePerSecondVar;
}

bool ParticleSystem::isActive() const
{
    return m_bIsActive;
}

int ParticleSystem::getTotalParticles() const
{
    return m_nTotalParticles;
}

void ParticleSystem::setTotalParticles(int var)
{
    CCASSERT( var <= m_nAllocatedParticles, "Particle: resizing particle array only supported for quads");
    m_nTotalParticles = var;
}

const BlendFunc& ParticleSystem::getBlendFunc() const
{
    return m_tBlendFunc;
}

void ParticleSystem::setBlendFunc(const BlendFunc &blendFunc)
{
    if( m_tBlendFunc.src != blendFunc.src || m_tBlendFunc.dst != blendFunc.dst ) {
        m_tBlendFunc = blendFunc;
        this->updateBlendFunc();
    }
}

bool ParticleSystem::isAutoRemoveOnFinish() const
{
    return m_bIsAutoRemoveOnFinish;
}

void ParticleSystem::setAutoRemoveOnFinish(bool var)
{
    m_bIsAutoRemoveOnFinish = var;
}


// ParticleSystem - methods for batchNode rendering

ParticleBatchNode* ParticleSystem::getBatchNode(void) const
{
    return m_pBatchNode;
}

void ParticleSystem::setBatchNode(ParticleBatchNode* batchNode)
{
    if( m_pBatchNode != batchNode ) {

        m_pBatchNode = batchNode; // weak reference

        if( batchNode ) {
            //each particle needs a unique index
            for (int i = 0; i < m_nTotalParticles; i++)
            {
                m_pParticles[i].atlasIndex=i;
            }
        }
    }
}

//don't use a transform matrix, this is faster
void ParticleSystem::setScale(float s)
{
    m_bTransformSystemDirty = true;
    Node::setScale(s);
}

void ParticleSystem::setRotation(float newRotation)
{
    m_bTransformSystemDirty = true;
    Node::setRotation(newRotation);
}

void ParticleSystem::setScaleX(float newScaleX)
{
    m_bTransformSystemDirty = true;
    Node::setScaleX(newScaleX);
}

void ParticleSystem::setScaleY(float newScaleY)
{
    m_bTransformSystemDirty = true;
    Node::setScaleY(newScaleY);
}


NS_CC_END

