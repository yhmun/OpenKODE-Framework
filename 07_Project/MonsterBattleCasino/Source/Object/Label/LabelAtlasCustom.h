/* -----------------------------------------------------------------------------------
 *
 *      File            LabelAtlasCustom.h
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

#ifndef __LabelAtlasCustom_h__
#define __LabelAtlasCustom_h__

typedef struct 
{
	KDchar		c;
	KDint		width;
} sCharInfo;

class LabelAtlasCustom : public AtlasNode, public LabelProtocol
{
	public :

		static LabelAtlasCustom*	create ( const KDchar* pCharmapfile, KDfloat fHeight );

		virtual KDbool				initWithCharmapfile ( const KDchar* pCharmapfile, KDfloat fHeight );

		virtual KDvoid				draw ( KDvoid );

		virtual KDvoid				addCharInfo ( KDchar c, KDint nWidth );

		virtual KDvoid				setString ( const std::string& sLabel );

		virtual const std::string&	getString ( KDvoid ) const;

	protected :

		KDfloat						m_fHeight;
		KDint						m_nBeforeStringLen;

		Size						m_tContentSize;

		std::string					m_tString;
		std::vector<sCharInfo>		m_aMatchChars;
};

class cLabelAtlasCustom : public Object
{
	public :
				 cLabelAtlasCustom ( Node* pNode, const KDchar* pCharmapfile, KDint nHeight, KDint z );
		virtual ~cLabelAtlasCustom ( KDvoid );

		KDvoid						addCharInfo ( KDchar c, KDint nWidth );
		KDvoid						setVisible ( KDbool bIsVisible );
		KDvoid						setString ( const KDchar* pStr );
		KDvoid						setPosition ( Point tPoint );
		KDvoid						setPointTime ( Point tPoint, KDdouble dLeftTime );
		KDvoid						setAlpha ( KDubyte a );
		KDvoid						setColor ( KDubyte r, KDubyte g, KDubyte b );
		KDvoid						setColor ( KDubyte a, KDubyte r, KDubyte g, KDubyte b );

		Size						getSize ( KDvoid );
		Point						getPoint ( KDvoid );
		Color3B						getColor ( KDvoid );

		KDvoid						update ( KDdouble dLeftTime );

	private :

		Node*						m_pNode;
		LabelAtlasCustom*			m_pLabel;

		KDbool						m_bIsMove;
		Point						m_tMoveBefore;
		Point						m_tMove;
		KDdouble					m_dTimeMoveFull;
		KDdouble					m_dTimeMoveLeft;
};

#endif	// __LabelAtlasCustom_h__
