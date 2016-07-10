/* -----------------------------------------------------------------------------------
 *
 *      File            LabelAtlas.h
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

#ifndef __cLabelAtlas_h__
#define __cLabelAtlas_h__


/*
#include "draw.h"
#include "define.h"
#include <vector>



@class CCLabelAtlas;
DefineIntrusivePtr(cCamera);
DefineIntrusivePtr(cLabelAtlas);
class cLabelAtlas : public cDraw
{
public :
	cLabelAtlas(NSString* str, NSString* charMapFile, int width, int height, char startChar, int z, CGPoint pt);
	virtual ~cLabelAtlas();


	//visible
	inline void setIsVisible(bool isVisible)	{	m_isVisible = isVisible;	}
	inline bool getIsVisible()					{	return m_isVisible;			}
	
	//static
	inline void setIsStatic(bool isStatic)	{	m_isStatic = isStatic;	}
	inline bool getIsStatic()				{	return m_isStatic;		}
	
	//position
	inline void		setAnchorPt(CGPoint pt)	{	m_anchorPoint = pt;		}
	inline void		setPt	   (CGPoint pt)	{	m_pt		  = pt;		}
	inline CGPoint	getAnchorPt()			{	return m_anchorPoint;	}
	inline CGPoint	getPt	   ()			{	return m_pt;			}
	CGPoint getScreenPt(const cCameraPtr camera);
	
	//opacity
	void setOpacity(int opacity);
	inline int getOpacity()	{	return m_opacity;	}
	
	//color
	void setColor(int r, int g, int b);
	inline ccColor3B getColor()	{	return m_color;		}
	
	//rotation
	inline void	 setRotation(float rotation)	{	m_rotation = rotation;	}
	inline float getRotation()					{	return m_rotation;		}
	
	//scale
	inline void  setScale (float scale)	{	m_scale_x = scale;	m_scale_y = scale;	}
	inline void  setScaleX(float scale)	{	m_scale_x = scale;						}
	inline void  setScaleY(float scale)	{	m_scale_y = scale;						}
	inline float getScaleX()			{	return m_scale_x;						}
	inline float getScaleY()			{	return m_scale_y;						}
	
	//update
	void update(double leftTime)	{	}
	
	//draw
	void draw(const cCameraPtr camera);


private :
	//label
	CCLabelAtlas* m_pLabel;

	//visible
	bool m_isVisible;
	
	//static
	bool m_isStatic;
	
	//pt
	CGPoint m_anchorPoint;
	CGPoint m_pt;
	
	//opacity
	int m_opacity;
	
	//color
	ccColor3B m_color;
	
	//rotation angle
	float m_rotation;
	
	//scaling factors
	float m_scale_x;
	float m_scale_y;
};
*/


#endif	// __cLabelAtlas_h__
