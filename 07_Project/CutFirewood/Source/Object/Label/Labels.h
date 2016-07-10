/* -----------------------------------------------------------------------------------
 *
 *      File            Labels.h
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

#ifndef __cLabels_h__
#define __cLabels_h__

//#include "../Draw/Draw.h"


/*

#include <vector>
#include "RefPointee.h"



typedef struct _sMove
{
	id		_id;
	CGPoint ptMoveBefore;
	CGPoint ptMove;
	double	timeMoveFull;
	double	timeMoveLeft;
} sMove;



@class CCLabel;
@class CCNode;
DefineIntrusivePtr(cLabels);
class cLabels : public RefPointee
{
public :
	cLabels(CCNode* node);
	virtual ~cLabels();

	void clear();
	void changeNode(CCNode* node);

	id	 add(NSString* fontName, float fontSize, UITextAlignment alignment, CGRect rc, NSString* str, int z);	//id�?리턴.
	void del(id _id);

	void setVisible	(id _id, bool isVisible);
	void setPt		(id _id, CGPoint pt);
	void setAnchorPt(id _id, CGPoint pt);
	void setStr		(id _id, NSString* str);
	void setAllColor(unsigned char a, unsigned char r, unsigned char g, unsigned char b);
	void setColor	(id _id, unsigned char r, unsigned char g, unsigned char b);
	void setAlpha	(id _id, unsigned char alpha);
	void setPtTime	(id _id, CGPoint pt, double leftTime);

	CGPoint getPt  (id _id);
	CGSize  getSize(id _id);

	void update(double leftTime);

private :
	CCNode* m_pNode;

	std::vector<CCLabelTTF *> m_labels;
	std::vector<sMove		> m_moves;
};

*/

#endif	// __cLabels_h__
