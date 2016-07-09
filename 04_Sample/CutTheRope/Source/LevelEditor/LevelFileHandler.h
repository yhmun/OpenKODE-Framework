/* --------------------------------------------------------------------------
 *
 *      File            LevelFileHandler.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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

#ifndef __LevelFileHandler_h__
#define __LevelFileHandler_h__

#include "../Constants.h"

class RopeModel;
class PineappleModel;
class AbstractModel;

class LevelFileHelper : public CCObject
{
	private :
				 LevelFileHelper ( KDvoid );
	public :
				 LevelFileHelper ( const KDchar* szLevelFileName );
		virtual ~LevelFileHelper ( KDvoid );
	    

		PineappleModel*		getPineappleWithID	( KDint nId );
		AbstractModel*		getModelWithIDArray	( KDint nId, CCArray* pArray );
	    
		KDvoid				sortArrayById		( CCArray* pArray );
		KDint				findUnusedIdInArray ( CCArray* pArray );
	    
		PineappleModel*		addPineappleAt		( CCPoint tPos );
		KDvoid				addRopeFromModel	( RopeModel* pModel );

		KDvoid				removeRopeWithID	  ( KDint nId );
		KDvoid				removePineappleWithID ( KDint nId );
	    
		KDvoid				moveRopeWithId		( KDint nRopeID, EAnchorType eType, KDint nAnchorID, CCPoint tPos );
		KDvoid				movePineappleWithID	( KDint nPineappleID, CCPoint tPos );
	    
		KDvoid				saveFile			( KDvoid );

		KDvoid				reset				( KDvoid );
	    
	private :

		KDvoid				loadLevel			( KDvoid );

	public :

		CCArray*			m_pPineapples;
		CCArray*			m_pRopes;

	private :

		const KDchar*		m_szLevelFile;	   
};

#endif	// __LevelFileHandler_h__
