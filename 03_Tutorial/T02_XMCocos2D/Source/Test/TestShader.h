/* --------------------------------------------------------------------------
 *
 *      File            TestShader.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2011 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

#ifndef __TestShader_h__
#define __TestShader_h__

#include "../TestBasic.h"

class TestShader : public TestBasic
{
	public :

		static  TestShader*  create ( KDvoid );

	protected :		

		virtual KDuint  count ( KDvoid );
};

class ShaderNode : public CCNode
{
    public :
    
        ShaderNode ( KDvoid );
    
        static ShaderNode*  shaderNodeWithVertex ( const KDchar* pVert, const KDchar* pFrag );
    
    public :
    
        KDbool  initWithVertex ( const KDchar* pVert, const KDchar* pFrag );
    
        KDvoid  loadShaderVertex ( const KDchar* pVert, const KDchar* pFrag );
    
        virtual KDvoid  update ( KDfloat fDelta );
    
        virtual KDvoid  draw ( KDvoid );
    
    private :
    
        ccVertex2F      m_tResolution;
        KDfloat         m_fTime;
        GLuint          m_uniformCenter;
        GLuint          m_uniformResolution;
        GLuint          m_uniformTime;
};

class SpriteBlur : public CCSprite
{
    public :
    
        static SpriteBlur*  create ( const KDchar* szFileName );
    
    public :
    
        KDbool  initWithTexture ( CCTexture2D* pTexture, const CCRect&  tRect );
    
        virtual KDvoid  draw ( );
    
        KDvoid  setBlurSize ( KDfloat fSize );
    
    protected :
    
        CCPoint     m_tBlur;
        GLfloat     m_fSub[4];
    
        GLuint      m_uBlurLocation;
        GLuint      m_uSubLocation;
};

class ShaderMonjori : public TestShader
{
	public :

		virtual KDbool  init ( KDvoid );
    
		virtual const KDchar*  subtitle ( KDvoid );
};

class ShaderMandelbrot : public TestShader
{
	public :
    
        virtual KDbool  init ( KDvoid );
    
        virtual const KDchar*  subtitle ( KDvoid );
};

class ShaderJulia : public TestShader
{
	public :
    
        virtual KDbool  init ( KDvoid );
    
        virtual const KDchar*  subtitle ( KDvoid );
};

class ShaderHeart : public TestShader
{
	public :
    
        virtual KDbool  init ( KDvoid );
    
        virtual const KDchar*  subtitle ( KDvoid );
};

class ShaderFlower : public TestShader
{
	public :
    
        virtual KDbool  init ( KDvoid );
    
        virtual const KDchar*  subtitle ( KDvoid );
};

class ShaderPlasma : public TestShader
{
	public :
    
        virtual KDbool  init ( KDvoid );
    
        virtual const KDchar*  subtitle ( KDvoid );
};

class ShaderBlur : public TestShader
{
	public :
    
        virtual KDbool  init ( KDvoid );
    
        virtual const KDchar*  subtitle ( KDvoid );
    
        CCControlSlider*  createSliderCtl ( KDvoid );
    
        KDvoid  sliderAction ( CCObject* pSender, CCControlEvent uControlEvent );
    
    protected :
    
        SpriteBlur*         m_pBlurSprite;
        CCControlSlider*    m_pSliderCtl;
};

class ShaderRetroEffect : public TestShader
{
	public :
    
        virtual KDbool  init ( KDvoid );
    
        virtual const KDchar*  title ( KDvoid );
    
        virtual const KDchar*  subtitle ( KDvoid );
    
        KDvoid  update ( KDfloat fDelta );
    
    protected :
    
        CCLabelBMFont*      m_pLabel;
        KDfloat             m_fAccum; 
};


#endif
