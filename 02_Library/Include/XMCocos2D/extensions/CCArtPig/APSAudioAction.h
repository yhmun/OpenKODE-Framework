/* --------------------------------------------------------------------------
 *
 *      File            APSAudioAction.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
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

#ifndef __APSAudioAction_h__
#define __APSAudioAction_h__

#include "APSAction.h"

NS_APS_BEGIN

class APSAudioHolder;

/**
 *	@brief	APSAudioAction manages to play audio in APSAudioHolder.
 */
class APSAudioAction : public APSAction
{		           
	public :

		/** Constructor of APSAudioAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSAudioAction ( APSDictionary* pProperties = KD_NULL );	    
	    
		APS_CLASS_NAME ( APSAudioAction );

	public :

		/** Initializes APSAudioAction with given properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** Preloads audio data before it is used. */
		virtual KDvoid  preloadData ( KDvoid );

		/** Returns the type of action. */
		virtual APSActionType  getActionType ( KDvoid ) const { return kAPSActionTypeAudio; }
	    
		/** Plays audio in audioHolder. */
		KDvoid  play ( KDvoid );
	    
		/** Returns APSAudioHolder object for audioHolderCode property. */
		APSAudioHolder*  getAudioHolder ( KDvoid );
	    
		/** 
		 *	startFunc ( ) is supposed to be called at the beggining of CCAction's execution.
		 *	@warning startFunc ( ) should be only called by CCAction's execution.
		 */
		virtual KDvoid  stopFunc ( KDvoid );
	    
		virtual KDvoid  _interrupt ( KDvoid );

	protected :
	    
		/** Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run between pre and post actions.
		 */
		CCFiniteTimeAction*  makeMidCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );

		/** Code reference of APSAudioHolder object. */
		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sAudioHolderCode, AudioHolderCode );
	    
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bTrimmed, Trimmed );

	protected :

		APSAudioHolder*		m_pAudioHolder;
};

NS_APS_END

#endif	// __APSAudioAction_h__
