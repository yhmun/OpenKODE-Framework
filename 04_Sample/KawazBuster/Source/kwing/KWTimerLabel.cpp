/* --------------------------------------------------------------------------
 *
 *      File            KWTimerLabel.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/06/21
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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
#include "KWTimerLabel.h"

KWTimerLabel* KWTimerLabel::create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize )
{
	KWTimerLabel*	pRet = new KWTimerLabel ( );

	if ( pRet && pRet->init ( ) && pRet->initWithString ( szString, szFontName, fFontSize ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool KWTimerLabel::init ( KDvoid )
{
	if ( !CCLabelTTF::init ( ) )
	{
		return KD_FALSE;
	}

	m_bActive			 = KD_FALSE;
	m_bDisplayMiliSecond = KD_FALSE;
	m_pFinishListener	 = KD_NULL;
	m_pFinishSelector	 = KD_NULL;

	this->setTime ( 0, 0, 0 );
	this->setString ( this->humalize ( ) );

	return KD_TRUE;
}

KDvoid KWTimerLabel::play ( KDvoid )
{
	m_bActive = KD_TRUE;
	this->schedule ( schedule_selector ( KWTimerLabel::tick ), 1.0f / FPS );	
}

KDvoid KWTimerLabel::stop ( KDvoid )
{
	m_bActive = KD_FALSE;
	this->unschedule ( schedule_selector ( KWTimerLabel::tick ) );	
}

KDvoid KWTimerLabel::setTime ( KDint nHour, KDint nMinute, KDint nSecond )
{
	m_tCurrent.nHour		= nHour;
	m_tCurrent.nMinute		= nMinute;
	m_tCurrent.nSecond		= nSecond;
	m_tCurrent.nMilisecond	= 0;
	m_tInitial.nHour		= nHour;
	m_tInitial.nMinute		= nMinute;
	m_tInitial.nSecond		= nSecond;
	m_tInitial.nMilisecond	= 0;

	this->setString ( this->humalize ( ) );
}

KDbool KWTimerLabel::isActive ( KDvoid )
{
	return m_bActive;
}

KDint KWTimerLabel::leaveSecond ( KDvoid )
{
	return (KDint) this->convertToSecond ( m_tCurrent );
}

KDvoid KWTimerLabel::setTimerCompletionListener ( CCObject* pListener, SEL_CallFunc pSelector )
{
	m_pFinishListener = pListener;
	m_pFinishSelector = pSelector;
}

KDvoid KWTimerLabel::tick ( KDfloat fDelta )
{
	if ( this->leaveSecond ( ) > 0 )
	{
		if ( m_bActive )
		{
			KDfloat	 fSecond = this->convertToSecond ( m_tCurrent );
			fSecond -= fDelta;
			m_tCurrent = this->convertToTime ( fSecond );
		}
	}
	else
	{
		if ( m_pFinishSelector && m_bActive ) 
		{
			( m_pFinishListener->*m_pFinishSelector ) ( );		
		}	
		m_bActive = KD_FALSE;
	}

	this->setString ( this->humalize ( ) );
}

const KDchar* KWTimerLabel::humalize ( KDvoid )
{
	KDint	nHour   = m_tCurrent.nHour;
	KDint	nMinute = m_tCurrent.nMinute;
	KDint	nSecond = m_tCurrent.nSecond;

	const KDchar*	szString = KD_NULL;

	if ( m_tInitial.nHour == 0 )
	{
		if ( m_tInitial.nMinute < 10 )
		{
			szString = ccszf ( "%d:%02d", nMinute, nSecond );		  
		}
		else
		{
			szString = ccszf ( "%02d:%02d", nMinute, nSecond );
		}
	}
	else if ( m_tInitial.nMinute == 0 )
	{
		szString = ccszf ( "%02d", nSecond );
	}
	else
	{
		szString = ccszf ( "%02d:%02d:%02d", nHour, nMinute, nSecond );
	}
	
	if ( m_bDisplayMiliSecond )
	{
		return ccszf ( "%s:%03d", szString, m_tCurrent.nMilisecond );	
	}

	return szString;
}

KWTime KWTimerLabel::convertToTime ( KDfloat fSecond )
{
	KWTime	tTime;

	tTime.nHour			= (KDint) ( fSecond / 3600 );
	tTime.nMinute		= (KDint) ( ( fSecond - tTime.nHour * 3600 ) / 60 );
	tTime.nSecond		= (KDint) ( fSecond - tTime.nHour * 3600 - tTime.nMinute * 60 );
	tTime.nMilisecond	= (KDint) ( ( fSecond - (KDint) kdFloor ( fSecond ) ) * 1000 );

	return tTime;
}

KDfloat KWTimerLabel::convertToSecond ( KWTime tTime )
{
	KDfloat		ms = tTime.nMilisecond / 1000.0f;
	return 3600 * tTime.nHour + 60 * tTime.nMinute + tTime.nSecond + ms;
}

