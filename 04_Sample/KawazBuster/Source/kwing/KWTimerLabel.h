/* --------------------------------------------------------------------------
 *
 *      File            KWTimerLabel.h
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

#ifndef __KWTimerLabel_h__
#define __KWTimerLabel_h__

typedef struct
{
	KDint		nHour;
	KDint		nMinute;
	KDint		nSecond;
	KDint		nMilisecond;
} KWTime;

class KWTimerLabel : public CCLabelTTF
{
	public :

		static  KWTimerLabel*	create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize );

	public :

		virtual KDbool		init ( KDvoid );
		
		virtual KDvoid		play ( KDvoid );
		
		virtual KDvoid		stop ( KDvoid );
		
		virtual KDvoid		setTime ( KDint nHour, KDint nMinute, KDint nSecond );

		virtual KDbool		isActive ( KDvoid );

		virtual KDint		leaveSecond ( KDvoid );

		virtual KDvoid		setTimerCompletionListener ( CCObject* pListener, SEL_CallFunc pSelector );

	private :

		KDvoid				tick ( KDfloat fDelta );

		const KDchar*		humalize ( KDvoid );		

		KWTime				convertToTime ( KDfloat fSecond );

		KDfloat				convertToSecond ( KWTime tTime );

	protected :

		KDbool				m_bActive;
		KDbool				m_bDisplayMiliSecond;
		KWTime				m_tInitial;
		KWTime				m_tCurrent;

		CCObject*			m_pFinishListener;
		SEL_CallFunc		m_pFinishSelector;
};

#endif	// __KWTimerLabel_h__
