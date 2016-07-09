/* --------------------------------------------------------------------------
 *
 *      File            Ch8_JSONWorldBuilder.h
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

#ifndef __Ch8_JSONWorldBuilder_h__
#define __Ch8_JSONWorldBuilder_h__

#include "Libraries/GameArea2D.h"
#include "Libraries/DrawLayer.h"

class Ch8_JSONWorldBuilder : public GameArea2D
{
	public :

		SCENE_FUNC ( Ch8_JSONWorldBuilder );

	protected :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onExit ( KDvoid );

		virtual KDvoid		drawLayer ( KDvoid );

		KDvoid				finishInit ( KDvoid );

		KDvoid				loadMap ( const KDchar* szMapStr );
		
		KDvoid				preProcessMapData ( KDvoid );
		
		KDvoid				processSprite ( CCDictionary* pMapNode );
		
		KDvoid				processTiledSprite ( CCDictionary* pMapNode );
		
		KDvoid				processLine ( CCDictionary* pMapNode );

		KDvoid				processPoint ( CCDictionary* pMapNode );

		KDvoid				addDrawLayer ( KDvoid );

	private :

		CCDictionary*		m_pMapData;
		CCPoint				m_tCanvasSize;
		DrawLayer*			m_pDrawLayer;
		CCArray*			m_pLineVerticesA;
		CCArray*			m_pLineVerticesB;
		CCArray*			m_pPoints;
};

#endif	// __Ch8_JSONWorldBuilder_h__
