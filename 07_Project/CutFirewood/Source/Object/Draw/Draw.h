/* -----------------------------------------------------------------------------------
 *
 *      File            Draw.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __cDraw_h__
#define __cDraw_h__

class cCamera;

class cDraw : public Object
{
	public :

		typedef enum
		{
			eObjectType_Sprite			,
			eObjectType_Animation		,
			eObjectType_Animations		,
			eObjectType_Button			,
			eObjectType_LabelAtlas		,
			eObjectType_LabelAtlasCostom,
			eObjectType_ColorLayer		,
		} eObjectType;

	public :
	
		cDraw ( KDint nZorder, eObjectType eObjectType );
		virtual ~cDraw ( KDvoid );

		// z order
		inline KDvoid		setZorder ( KDint z )	{ m_nZorder = z; }
		inline KDint		getZorder ( KDvoid )	{ return m_nZorder; }

		// object type
		inline eObjectType	getObjectType ( KDvoid ) {	return m_eObjectType; }

		// far
		inline KDvoid		setFar ( KDint nFar ) {	m_nFar = nFar;	}
		inline KDint		getFar ( KDvoid ) {	return m_nFar;	}

		// update
		virtual KDvoid		update ( KDdouble dLeftTime ) = 0;

		// draw
		virtual KDvoid		draw ( cCamera* pCamera ) = 0;

		// point
		virtual KDvoid		setAnchorPoint ( Point tPoint )	{ }
		virtual KDvoid		setPoint	   ( Point tPoint )	{ }
		virtual Point		getAnchorPoint ( KDvoid ) {	return Point::ZERO;	}
		virtual Point		getPoint	   ( KDvoid ) {	return Point::ZERO; }

	private :

		// z order
		KDint				m_nZorder;

		// object type
		eObjectType			m_eObjectType;

		// far
		KDint				m_nFar;	
};

#endif	// __cDraw_h__