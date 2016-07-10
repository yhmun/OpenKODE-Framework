/* -----------------------------------------------------------------------------------
 *
 *      File            LabelAtlas.cpp
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

#include "Precompiled.h"
//#include "CCLabelAtlas.h"
//#include "camera.h"
#include "LabelAtlas.h"

/*

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
cLabelAtlas::cLabelAtlas(NSString* str, NSString* charMapFile, int width, int height, char startChar, int z, CGPoint pt)
: cDraw(z, eObjectType_LabelAtlas)
, m_pLabel		( nil		)
, m_isVisible	( true		)
, m_isStatic	( false		)
, m_anchorPoint ( ccp(0, 0) )
, m_pt			( pt		)
, m_opacity		( 255		)
, m_rotation	( 0			)
, m_scale_x		( 1			)
, m_scale_y		( 1			)
{
	m_pLabel = [ [CCLabelAtlas alloc] initWithString:str charMapFile:charMapFile itemWidth:width itemHeight:height startCharMap:startChar ];
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
cLabelAtlas::~cLabelAtlas()
{
	[m_pLabel release];
	m_pLabel = nil;
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
CGPoint cLabelAtlas::getScreenPt(const cCameraPtr camera)
{
	if(m_isStatic == false)	return ccpSub( m_pt, CGPointMake( (int)( camera->get_x()/getFar() ), (int)( camera->get_y()/getFar() ) ) );
	else					return m_pt;
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabelAtlas::setOpacity(int opacity)
{
	if(opacity <   0)	opacity = 0;
	if(opacity > 255)	opacity = 255;
	
	m_opacity = opacity;
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabelAtlas::setColor(int r, int g, int b)
{
	if(r <	 0)	r = 0;
	if(r > 255)	r = 255;
	if(g <	 0)	g = 0;
	if(g > 255)	g = 255;
	if(b <	 0)	b = 0;
	if(b > 255)	b = 255;
	
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabelAtlas::draw(const cCameraPtr camera)
{
	//visible check
	if(m_isVisible == false)	return;
	
	//visible
	m_pLabel.visible = true;
	
	//rotation
	m_pLabel.rotation = m_rotation;
	
	//opacity
	m_pLabel.opacity = m_opacity;
	
	//color
	m_pLabel.color = m_color;
	
	//scale
	m_pLabel.scaleX = m_scale_x;
	m_pLabel.scaleY = m_scale_y;
	
	//anchor point
	m_pLabel.anchorPoint = m_anchorPoint;
	
	//pt
	if(m_isStatic == false)	m_pLabel.position = ccpSub( m_pt, CGPointMake( (int)( camera->get_x()/getFar() ), (int)( camera->get_y()/getFar() ) ) );
	else					m_pLabel.position = m_pt;
	
	//draw
	[m_pLabel visit];
}
*/