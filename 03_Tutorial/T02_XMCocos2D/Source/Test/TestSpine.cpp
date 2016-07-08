/* -----------------------------------------------------------------------------------
 *
 *      File            TestSpine.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2013      Esoteric Software. All rights reserved.
 *
 *         http://esotericsoftware.com/      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions are met:
 * 
 *     1. Redistributions of source code must retain the above copyright notice, this
 *        list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 * 
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "TestSpine.h"

KDbool TestSpine::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	m_pSkeletonNode = CCSkeleton::create ( "spine/spineboy.json", "spine/spineboy.atlas" );

	AnimationStateData_setMixByName		( m_pSkeletonNode->state->data	, "walk", "jump", 0.4f );
	AnimationStateData_setMixByName		( m_pSkeletonNode->state->data	, "jump", "walk", 0.4f );
	AnimationState_setAnimationByName	( m_pSkeletonNode->state		, "walk", KD_TRUE );

	m_pSkeletonNode->timeScale  = 0.3f;
	m_pSkeletonNode->debugBones = KD_TRUE;

	m_pSkeletonNode->runAction 
	(
		CCRepeatForever::create
		(
			CCSequence::create
			(
				CCFadeOut	::create ( 1 ),
				CCFadeIn	::create ( 1 ),
				CCDelayTime	::create ( 5 ),
				KD_NULL
			)
		)
	);

	const CCSize&  tLyrSize = this->getContentSize ( );
	m_pSkeletonNode->setPosition ( ccp ( tLyrSize.cx / 2, 30 ) );
	this->addChild ( m_pSkeletonNode );

	scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid TestSpine::update ( KDfloat fDelta )
{
    if ( m_pSkeletonNode->state->loop )
	{
        if ( m_pSkeletonNode->state->time > 2 ) 
		{
			AnimationState_setAnimationByName ( m_pSkeletonNode->state, "jump", KD_FALSE );
		}
    }
	else 
	{
        if ( m_pSkeletonNode->state->time > 1 ) 
		{
			AnimationState_setAnimationByName ( m_pSkeletonNode->state, "walk", KD_TRUE );
		}
    }
}