/* -----------------------------------------------------------------------------------
 *
 *      File            CCDiamoneTiledLayer.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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

#ifndef __CCDiamoneTiledLayer_h__
#define __CCDiamoneTiledLayer_h__

#include "../layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_BEGIN
    
class CCDiamondTiledLayer : public CCLayer
{
	public :
				 CCDiamondTiledLayer ( KDvoid );
		virtual ~CCDiamondTiledLayer ( KDvoid );

		static   CCDiamondTiledLayer*	create ( const ccGridSize& tGridSize, const ccGridSize& tFrameSize, KDuint uFrameCount );

	public :

		virtual KDbool					initWithInfo ( const ccGridSize& tGridSize, const ccGridSize& tFrameSize, KDuint uFrameCount );

		KDvoid							loadFrame ( KDuint uFrameIndex, const KDchar* szFilePath ); 

		KDvoid							setFrame ( const ccGridSize& tGID, KDuint uFrameIndex );
		KDuint							getFrame ( const ccGridSize& tGID );

		ccColor3B						getColor ( const ccGridSize& tGID );
		KDvoid							setColor ( const ccGridSize& tGID, const ccColor3B& tColor );

		KDvoid							setOpacity ( const ccGridSize& tGID, KDubyte cOpacity );
		KDubyte							getOpacity ( const ccGridSize& tGID );

		CCPoint							getPosition ( const ccGridSize& tGID );

		ccGridSize						getIndex ( const CCPoint& tPosition );	

		inline const ccGridSize&		getGridSize ( KDvoid ) { return m_tGridSize; }
		inline const ccGridSize&		getFrameSize ( KDvoid ) { return m_tFrameSize; }
		inline KDuint					getFrameCount ( KDvoid ) { return m_uFrameCount; }

		inline KDuint					convertIndex ( const ccGridSize& tGID ) { return tGID.y * m_tGridSize.x + tGID.x; }
		inline ccGridSize				convertIndex ( KDuint uIndex ) { return ccg ( uIndex / m_tGridSize.x, uIndex % m_tGridSize.x ); }

	protected :

		virtual KDvoid					draw ( KDvoid );

	private :

		KDvoid							purge ( KDvoid );

	protected :

		ccGridSize						m_tGridSize;
		ccGridSize						m_tFrameSize;
		KDuint							m_uFrameCount;
		
		GLuint							m_aGIDs [ 2 ];
		GLushort*						m_pIndices;
		CCTexture2D*					m_pTexture;
		KDuint*							m_pFrameIndices;
		ccV2F_C4B_T2F*					m_pVertices;	
		KDubyte*						m_pPixels;
};

NS_CC_END

#endif // __CCDiamoneTiledLayer_h__

