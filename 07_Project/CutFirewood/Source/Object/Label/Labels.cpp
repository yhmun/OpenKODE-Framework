/* -----------------------------------------------------------------------------------
 *
 *      File            Labels.cpp
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
#include "Labels.h"
/*
#import "CCLabelTTF.h"



//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
cLabels::cLabels(CCNode* node)
: m_pNode(node)
{

}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
cLabels::~cLabels()
{
	clear();
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::clear()
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		[m_pNode removeChild:*node cleanup:YES];
	}
	
	m_labels.clear();
	m_moves .clear();
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::changeNode(CCNode* node)
{
	m_pNode = node;
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : id�?리턴.
//-----------------------------------------------------------
id cLabels::add(NSString* fontName, float fontSize, UITextAlignment alignment, CGRect rc, NSString* str, int z)
{
	CCLabel* label = [CCLabel labelWithString:str dimensions:rc.size alignment:alignment fontName:fontName fontSize:fontSize];

	label.position = rc.origin;

	[ m_pNode addChild:label z:z ];

	m_labels.push_back(label);

	return label;
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::del(id _id)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node == _id )
		{
			[m_pNode removeChild:*node cleanup:YES];
			*node = nil;

			break;
		}
	}
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::setVisible(id _id, bool isVisible)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node == _id )
		{
			(* node).visible = isVisible;
			break;
		}
	}
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::setPt(id _id, CGPoint pt)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node == _id )
		{
			(* node).position = pt;
			break;
		}
	}
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::setAnchorPt(id _id, CGPoint pt)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node == _id )
		{
			(* node).anchorPoint = pt;
			break;
		}
	}
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::setStr(id _id, NSString* str)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node == _id )
		{
			[ (* node) setString:str ];

			break;
		}
	}
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::setAllColor(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		ccColor3B color;
		color.r = r;
		color.g = g;
		color.b = b;

		(* node).opacity = a;
		(* node).color	 = color;

		break;
	}
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::setColor(id _id, unsigned char r, unsigned char g, unsigned char b)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node != _id )	continue;

		ccColor3B color;
		color.r = r;
		color.g = g;
		color.b = b;
		
		(* node).color = color;

		break;
	}
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::setAlpha(id _id, unsigned char alpha)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node != _id )	continue;

		(* node).opacity = alpha;

		break;
	}
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::setPtTime(id _id, CGPoint pt, double leftTime)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node == _id )
		{
			sMove move;
			move._id		  = _id;
			move.ptMove		  = CGPointMake( pt.x - (* node).position.x, pt.y - (* node).position.y );
			move.ptMoveBefore = (* node).position;
			move.timeMoveFull = leftTime;
			move.timeMoveLeft = leftTime;

			m_moves.push_back(move);

			break;
		}
	}
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
CGPoint cLabels::getPt(id _id)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node == _id )
		{
			return (* node).position;
		}
	}

	return CGPointZero;
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
CGSize cLabels::getSize(id _id)
{
	for(std::vector<CCLabel *>::iterator node = m_labels.begin(); node != m_labels.end(); ++node)
	{
		if( *node == _id )
		{
			return (* node).texture.contentSize;
		}
	}

	return CGSizeZero;
}

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
void cLabels::update(double leftTime)
{
	std::vector<sMove>::iterator node = m_moves.begin();
	while( node != m_moves.end() )
	{
		sMove* pMove	= &(* node);
		bool   isErase	= false;

		for(std::vector<CCLabel *>::iterator node2 = m_labels.begin(); node2 != m_labels.end(); ++node2)
		{
			CCLabel* pLabel = *node2;

			if(pLabel == pMove->_id)
			{
				pMove->timeMoveLeft -= leftTime;

				if(pMove->timeMoveLeft < 0)
				{
					CGPoint pt		= CGPointMake(pMove->ptMoveBefore.x + pMove->ptMove.x, pMove->ptMoveBefore.y + pMove->ptMove.y);
					pLabel.position = pt;
					isErase			= true;
				}
				else
				{
					CGPoint pt = CGPointMake( pMove->ptMoveBefore.x + (pMove->timeMoveFull-pMove->timeMoveLeft)*pMove->ptMove.x / pMove->timeMoveFull,
											  pMove->ptMoveBefore.y + (pMove->timeMoveFull-pMove->timeMoveLeft)*pMove->ptMove.y / pMove->timeMoveFull	);

					pLabel.position = pt;
				}

				break;
			}
		}

		if(isErase == true)
		{
			node = m_moves.erase(node);
		}
		else
		{
			++node;
		}
	}
}
*/
