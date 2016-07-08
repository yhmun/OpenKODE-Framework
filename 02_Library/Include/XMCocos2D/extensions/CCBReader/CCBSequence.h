/* --------------------------------------------------------------------------
 *
 *      File            CCBSequence.h
 *      Author          Young-Hwan Mun
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

#ifndef __CCBSequence_h__
#define __CCBSequence_h__

#include "../../cocoa/CCObject.h"
#include "CCBSequenceProperty.h"
#include <string>

NS_CC_BEGIN

class CCBSequence : public CCObject
{	    
	public :
				 CCBSequence ( KDvoid );
		virtual ~CCBSequence ( KDvoid );
	    
	public :

		KDfloat					getDuration ( KDvoid );
		KDvoid					setDuration ( KDfloat fDuration );
	    
		CCBSequenceProperty*	getCallbackChannel ( KDvoid );
		KDvoid					setCallbackChannel ( CCBSequenceProperty* pCallbackChannel );

		CCBSequenceProperty*	getSoundChannel ( KDvoid );
		KDvoid					setSoundChannel ( CCBSequenceProperty* pSoundChannel );

		const KDchar*			getName ( KDvoid );
		KDvoid					setName ( const KDchar* szName );
	    
		KDint					getSequenceId ( KDvoid );
		KDvoid					setSequenceId ( KDint nSequenceId );
	    
		KDint					getChainedSequenceId ( KDvoid );
		KDvoid					setChainedSequenceId ( KDint nChainedSequenceId );

	private :

		KDfloat					m_fDuration;
		std::string				m_sName;
		KDint					m_nSequenceId;
		KDint					m_nChainedSequenceId;
		CCBSequenceProperty*	m_pCallbackChannel;
		CCBSequenceProperty*	m_pSoundChannel;
};

NS_CC_END

#endif	// __CCBSequence_h__
