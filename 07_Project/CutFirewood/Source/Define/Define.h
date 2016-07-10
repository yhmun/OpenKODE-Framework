/* -----------------------------------------------------------------------------------
 *
 *      File            Define.h
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

#ifndef __Define_h__
#define __Define_h__

// define---------------------------------------------
#define _SCR_W					((KDint)480)
#define _SCR_H					((KDint)320)

#define _SCR_W_HARP				240.f
#define _SCR_H_HARP				160.f

enum eLookAtType
{
	eLookAtType_left ,
	eLookAtType_right,
};

// struct---------------------------------------------
typedef struct _sTouch
{
	KDint		nId;
	Point		tPoint;
} sTouch;

// function-------------------------------------------
KDint RectInPoint ( const Rect& tRect, const Point& tPoint );

KDint collisionBox ( const Rect& tRect1, const Rect& tRect );

KDbool readToData ( const KDubyte* pSrcData, const KDint nSrcDataSize, KDint& nSeek, KDvoid* pDestData, const KDint nDestDataSize );


#endif	// __Define_h__
