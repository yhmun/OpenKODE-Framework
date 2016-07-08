/* --------------------------------------------------------------------------
 *
 *      File            Minimap.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#ifndef __Minimap_h__
#define __Minimap_h__

class Minimap : public CCLayer
{
	public :
				 Minimap ( KDvoid );
		virtual ~Minimap ( KDvoid );

		static Minimap*			create ( KDvoid );

	public :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			draw ( KDvoid );

		virtual KDvoid			addWallWithVertex1 ( const CCPoint& tVertex1, const CCPoint& tVertex2 );

		virtual KDvoid			addStaticObject ( const CCPoint& tPoint ); 
		
		virtual KDvoid			setProjectile ( const CCPoint& tPoint, const KDchar* szKey );

		virtual const CCPoint&	getActor ( KDvoid );
		virtual KDvoid			setActor ( const CCPoint& tPoint ); 

		virtual CCArray*		getWallVertices1 ( KDvoid );
		virtual KDvoid			setWallVertices1 ( CCArray* pWallVertices1 );

		virtual CCArray*		getWallVertices2 ( KDvoid );
		virtual KDvoid			setWallVertices2 ( CCArray* pWallVertices2 );

		virtual CCDictionary*	getProjectiles ( KDvoid );
		virtual KDvoid			setProjectiles ( CCDictionary* pProjectiles );

		virtual CCArray*		getStaticObjects ( KDvoid );
		virtual KDvoid			setStaticObjects ( CCArray* pStaticObjects );

		virtual KDfloat			getScale ( KDvoid );
		virtual KDvoid			setScale ( KDfloat fScale );

	protected :

		CCArray*				m_pWallVertices1;
		CCArray*				m_pWallVertices2;
		CCDictionary*			m_pProjectiles;
		CCArray*				m_pStaticObjects;
		CCPoint					m_tActor;				// A pointer to the main actor we will follow
		KDfloat					m_fScale;				// How small the minimap is
};

#endif	// __Minimap_h__
