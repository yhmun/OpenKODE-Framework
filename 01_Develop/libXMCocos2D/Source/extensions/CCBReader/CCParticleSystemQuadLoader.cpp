/* --------------------------------------------------------------------------
 *
 *      File            CCParticleSystemQuadLoader.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org   
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
#include "extensions/CCBReader/CCParticleSystemQuadLoader.h"

NS_CC_BEGIN

#define PROPERTY_EMITERMODE			"emitterMode"
#define PROPERTY_POSVAR				"posVar"
#define PROPERTY_EMISSIONRATE		"emissionRate"
#define PROPERTY_DURATION			"duration"
#define PROPERTY_TOTALPARTICLES		"totalParticles"
#define PROPERTY_LIFE				"life"
#define PROPERTY_STARTSIZE			"startSize"
#define PROPERTY_ENDSIZE			"endSize"
#define PROPERTY_STARTSPIN			"startSpin"
#define PROPERTY_ENDSPIN			"endSpin"
#define PROPERTY_ANGLE				"angle"
#define PROPERTY_STARTCOLOR			"startColor"
#define PROPERTY_ENDCOLOR			"endColor"
#define PROPERTY_BLENDFUNC			"blendFunc"
#define PROPERTY_GRAVITY			"gravity"
#define PROPERTY_SPEED				"speed"
#define PROPERTY_TANGENTIALACCEL	"tangentialAccel"
#define PROPERTY_RADIALACCEL		"radialAccel"
#define PROPERTY_TEXTURE			"texture"
#define PROPERTY_STARTRADIUS		"startRadius"
#define PROPERTY_ENDRADIUS			"endRadius"
#define PROPERTY_ROTATEPERSECOND	"rotatePerSecond"

KDvoid CCParticleSystemQuadLoader::onHandlePropTypeIntegerLabeled ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDint pIntegerLabeled, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_EMITERMODE ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setEmitterMode ( pIntegerLabeled );
    } 
	else 
	{
        CCNodeLoader::onHandlePropTypeIntegerLabeled ( pNode, pParent, pPropertyName, pIntegerLabeled, pCCBReader );
    }
}

KDvoid CCParticleSystemQuadLoader::onHandlePropTypePoint ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint pPoint, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_POSVAR ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setPosVar ( pPoint );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_GRAVITY ) == 0 )
	{
        ( (CCParticleSystemQuad*) pNode )->setGravity ( pPoint );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypePoint ( pNode, pParent, pPropertyName, pPoint, pCCBReader );
    }
}

KDvoid CCParticleSystemQuadLoader::onHandlePropTypeFloat ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat pFloat, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_EMISSIONRATE ) == 0 )
	{
        ( (CCParticleSystemQuad*) pNode )->setEmissionRate ( pFloat );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_DURATION ) == 0 )
	{
        ( (CCParticleSystemQuad*) pNode )->setDuration ( pFloat );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeFloat ( pNode, pParent, pPropertyName, pFloat, pCCBReader );
    }
}

KDvoid CCParticleSystemQuadLoader::onHandlePropTypeInteger ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDint pInteger, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_TOTALPARTICLES ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setTotalParticles ( pInteger );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeInteger ( pNode, pParent, pPropertyName, pInteger, pCCBReader );
    }
}

KDvoid CCParticleSystemQuadLoader::onHandlePropTypeFloatVar ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat* pFloatVar, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_LIFE ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setLife    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setLifeVar ( pFloatVar[1] );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_STARTSIZE ) == 0 )
	{
        ( (CCParticleSystemQuad*) pNode )->setStartSize    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setStartSizeVar ( pFloatVar[1] );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ENDSIZE ) == 0 )
	{
        ( (CCParticleSystemQuad*) pNode )->setEndSize    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setEndSizeVar ( pFloatVar[1] );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_STARTSPIN ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setStartSpin    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setStartSpinVar ( pFloatVar[1] );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ENDSPIN ) == 0 )
	{
        ( (CCParticleSystemQuad*) pNode )->setEndSpin    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setEndSpinVar ( pFloatVar[1] );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ANGLE ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setAngle    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setAngleVar ( pFloatVar[1] );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_SPEED ) == 0 )
	{
        ( (CCParticleSystemQuad*) pNode )->setSpeed    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setSpeedVar ( pFloatVar[1] );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_TANGENTIALACCEL ) == 0 )
	{
        ( (CCParticleSystemQuad*) pNode )->setTangentialAccel    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setTangentialAccelVar ( pFloatVar[1] );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_RADIALACCEL ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setRadialAccel    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setRadialAccelVar ( pFloatVar[1] );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_STARTRADIUS ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setStartRadius    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setStartRadiusVar ( pFloatVar[1] );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ENDRADIUS ) == 0 )
	{
        ( (CCParticleSystemQuad*) pNode )->setEndRadius    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setEndRadiusVar ( pFloatVar[1] );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ROTATEPERSECOND ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setRotatePerSecond    ( pFloatVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setRotatePerSecondVar ( pFloatVar[1] );
    }
	else
	{
        CCNodeLoader::onHandlePropTypeFloatVar ( pNode, pParent, pPropertyName, pFloatVar, pCCBReader );
    }
}

KDvoid CCParticleSystemQuadLoader::onHandlePropTypeColor4FVar ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor4F * pCCColor4FVar, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_STARTCOLOR ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setStartColor ( pCCColor4FVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setStartColorVar ( pCCColor4FVar[1] );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ENDCOLOR ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setEndColor ( pCCColor4FVar[0] );
        ( (CCParticleSystemQuad*) pNode )->setEndColorVar ( pCCColor4FVar[1] );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeColor4FVar ( pNode, pParent, pPropertyName, pCCColor4FVar, pCCBReader );
    }
}

KDvoid CCParticleSystemQuadLoader::onHandlePropTypeBlendFunc ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc pCCBlendFunc, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_BLENDFUNC ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setBlendFunc ( pCCBlendFunc );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeBlendFunc ( pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader );
    }
}

KDvoid CCParticleSystemQuadLoader::onHandlePropTypeTexture ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCTexture2D* pCCTexture2D, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_TEXTURE ) == 0 ) 
	{
        ( (CCParticleSystemQuad*) pNode )->setTexture ( pCCTexture2D );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeTexture ( pNode, pParent, pPropertyName, pCCTexture2D, pCCBReader );
    }
}

NS_CC_END