/* --------------------------------------------------------------------------
 *
 *      File            APXAnimation.h
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

#ifndef __APXAnimation_h__
#define __APXAnimation_h__

#include "APSActionObserver.h"
#include "APXAnimationObserver.h"
#include "../../cocoa/CCGeometry.h"
#include "../../layers_scenes_transitions_nodes/CCLayer.h"

NS_APS_BEGIN

class APXAnimation : public CCObject, public APSActionObserver
{
	public :

		// Allocates and initilaizes an animation with given 'pid' and 'shouldScaleModelToDevice'. If 'shouldScale' is YES, animation scales to fit from model screen to runtime device screen.
		APXAnimation ( const KDchar* szProjectID, KDbool bShouldScaleModelToDevice = KD_TRUE );
	    
		virtual ~APXAnimation ( KDvoid );

		// Returns an autoreleased APXAnimation object with given 'pid' and 'shouldScaleModelToDevice'. If 'shouldScaleModelToDevice' is YES, animation scales to fit from model screen to runtime device screen.
		static   APXAnimation*  animationWithProjectId ( const KDchar* szProjectID, KDbool bShouldScaleModelToDevice = KD_TRUE );

	public :

		// Adds an observer that implements 'willStartAnimation' and 'didFinishAnimation' methods.
		KDvoid	 addObserver ( APXAnimationObserver* pObserver );
	    
		// Removes an observer that has been added.
		KDvoid	 removeObserver ( APXAnimationObserver* pObserver );

		// Returns CCLayer object which is the root of graphics tree.
		CCLayer* getRootNode ( KDvoid );
	    
		// Resets all CCNode objects in the animation to become original state.
		KDvoid	 resetNodes ( KDvoid );
	    
		KDvoid	 play ( KDvoid );
	    
		KDvoid	 stop ( KDvoid );
	    
		KDfloat	 getDuration ( KDvoid );

		const KDchar*  getProjectId ( KDvoid );
	    
		KDbool   getShouldScaleModelToDevice ( KDvoid ) const;
		KDvoid   setShouldScaleModelToDevice ( KDbool bShouldScale );
	    
		// Returns true while the animation is playing.
		KDbool   isPlaying ( KDvoid );
	    
		KDvoid   willStartAction ( APSAction* pAction );
		KDvoid   didFinishAction ( APSAction* pAction );

	protected :
	    
		KDvoid   notifyObservers ( KDvoid );
	    
		KDvoid   setIsPlaying ( KDbool bIsPlaying );
	    
	protected :

		KDbool								m_bShouldScaleModelToDevice;
		KDbool								m_bIsPlaying;
		KDbool								m_bHasPlayed;
		APSSymbol*							m_pSymbol;
		std::set<APXAnimationObserver*>		m_aObservers;
};

NS_APS_END

#endif	// __APXAnimation_h__
