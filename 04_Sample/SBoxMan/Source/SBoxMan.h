/* --------------------------------------------------------------------------
 *
 *      File            SBoxMan.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-06
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

#ifndef __SBoxMan_h__
#define __SBoxMan_h__

#define kBoxManLength			30.0f
#define kBoxManLength_2			15.0f
#define kBoxManLength_3			10.0f
#define kBoxManLength_4			 7.5f
#define kBoxManLength_5			 6.0f

enum SDirection 
{
    SDirectionUp		,
    SDirectionDown		,
    SDirectionLeft		,
    SDirectionRight		,
    SDirectionUnknown	,
};

class SBoxMan : public CCSprite
{
	public :

		static SBoxMan*		create ( const CCPoint& tPosition );

	public :

		virtual KDbool		initWithPosition ( const CCPoint& tPosition );

		virtual KDvoid		moveWithDirection ( SDirection eDirection, const CCPoint& tDestination );

		virtual KDvoid		setDirection ( SDirection eDirection );

	protected :

		SDirection			m_eDirection;
};

#endif	// __SBoxMan_h__