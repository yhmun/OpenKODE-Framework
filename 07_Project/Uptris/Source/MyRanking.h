/* --------------------------------------------------------------------------
 *
 *      File            MyRanking.h
 *      Created By      Project B team
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2013 XMsoft. All rights reserved.
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

#ifndef __MyRanking_h__
#define __MyRanking_h__

class MyRanking : public CCLayer
{
	public :

		CREATE_FUNC ( MyRanking );

	protected :

		virtual KDbool		init						( KDvoid );

		KDvoid				onButtonClick				( CCObject*	pSender );				// 뒤로가기 버튼
		KDvoid				onHttpRequestCompleted	    ( CCNode* pSender, KDvoid* pData );	// 점수 수신완료

		KDvoid				selectData					( KDfloat fDelta );					// 점수 요청
		KDvoid				decoding					( std::string sData );				// 점수 형태변환

	protected :

		CCProgressTimer*	m_pLoading;		// 로딩이미지
};

#endif	//__MyRanking_h__