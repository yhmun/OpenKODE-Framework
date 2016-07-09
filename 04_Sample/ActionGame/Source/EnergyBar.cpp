/* --------------------------------------------------------------------------
 *
 *      File            EnergyBar.cpp
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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
#include "EnergyBar.h"

EnergyBar* EnergyBar::create ( const CCSize& tSize, KDfloat fMaxVal )
{
	EnergyBar*		pRet = new EnergyBar ( );

	if ( pRet && pRet->initWithMaxSize ( tSize, fMaxVal ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool EnergyBar::initWithMaxSize ( const CCSize& tSize, KDfloat fMaxVal )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	// AnchorPoint를 왼쪽 아래로 잡습니다.
	this->setAnchorPoint ( ccpz );

	// 에너지 바의 이미지 크기
	m_tMaxSize = tSize;
         
    // 전체 에너지 바에 몇개의 에너지가 있는지 설정합니다.
    m_fCurValue = m_fMaxValue = fMaxVal;

    return KD_TRUE;
}

KDvoid EnergyBar::draw ( KDvoid )
{
//	CCNode::draw ( );

    // 그려야 할 길이를 픽셀 값으로 계산합니다.
    KDfloat		fWidth = m_tMaxSize.cx * m_fCurValue / m_fMaxValue;
    
    // 선의 두께를 설정합니다.
    ccLineWidth ( m_tMaxSize.cy );
    
    // 전체 에너지 값의 40% 이하로 떨어지면 빨간색으로 표시합니다.
    ccDrawColor4B ( 57, 248, 11, 255 );
    
    if ( m_fCurValue / m_fMaxValue <= 0.4f ) 
	{
        ccDrawColor4B ( 217, 79, 62, 255 );
    }
    
    ccDrawLine ( ccp ( 0, 0 ), ccp ( fWidth, 0 ) );
}

KDvoid EnergyBar::updateBar ( KDfloat fCurVal )
{
    m_fCurValue = fCurVal;

    if ( m_fCurValue < 0 )
	{
        m_fCurValue = 0;
	}
    else if ( m_fCurValue > m_fMaxValue )
	{
        m_fCurValue = m_fMaxValue;
	}
}
