/* --------------------------------------------------------------------------
 *
 *      File            ScoreScene.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/08/10). All rights reserved.
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

#ifndef __ScoreScene_h__
#define __ScoreScene_h__

class ScoreScene : public CCLayerColor 
{
	public :

		static CCScene*		scene ( KDint nSlow, KDint nQuike, KDint nStrong, KDint nStrongY,
									KDint nStrongG, KDint nLevel, KDint nKind, KDint nLife );

	public :

		virtual	KDbool		initWithNumber ( KDint nSlow, KDint nQuike, KDint nStrong, KDint nStrongY,
											 KDint nStrongG, KDint nLevel, KDint nKind, KDint nLife );

	private :

		KDvoid				score1 ( KDvoid );
		KDvoid				score2 ( KDvoid );
		KDvoid				score3 ( KDvoid );
		KDvoid				score4 ( KDvoid );
		KDvoid				score5 ( KDvoid );

		KDvoid				scoreSum ( KDvoid );

		KDvoid				onButtonClicked ( CCObject* pSender );

	private :

		KDint				m_nSlow;
		KDint				m_nQuike;
		KDint				m_nStrong;
		KDint				m_nStrongYe;
		KDint				m_nStrongG;
		KDint				m_nLevel;
		KDint				m_nLife;
		KDint				m_nKind;

		CCSize				m_tLyrSize;
};

#endif	// __ScoreScene_h__
