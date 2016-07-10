/* --------------------------------------------------------------------------
 *
 *      File            GameOver.h
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

#ifndef __GameOver_h__
#define __GameOver_h__

class GameOver : public CCLayer, public CCIMEDelegate
{
	public :

		CREATE_FUNC ( GameOver );

	protected :

		virtual KDbool			init					( KDvoid );

		virtual KDbool			ccTouchBegan			( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid					onSaveClick				( CCObject*	pSender );					// 저장버튼 클릭

		KDvoid					onHttpRequestCompleted	( CCNode* pSender, KDvoid* pData );		// 서버전송 완료

		KDvoid					insertData				( KDfloat fDelta );						// 실제 서버입력
				
	public :

		KDvoid					setGameScore			( KDint nScore );						// 점수 설정

	private :

		CCTextFieldTTF*			m_pTextField;	// 닉네임 입력
		CCLabelTTF*				m_pScoreField;	// 점수
		CCProgressTimer*		m_pLoading;		// 로딩이미지
};

#endif	//__GameOver_h__