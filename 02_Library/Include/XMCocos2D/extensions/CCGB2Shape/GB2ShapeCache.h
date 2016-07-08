/* -----------------------------------------------------------------------------------
 *
 *      File            GB2ShapeCache.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2013      Created by Thomas Broquist. All rights reserved.
 *
 *         http://www.PhysicsEditor.de  
 *         http://texturepacker.com
 *         http://www.code-and-web.de
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __GB2ShapeCache_h__
#define __GB2ShapeCache_h__

#include <map>
#include <string>
#include "../../cocoa/CCGeometry.h"

class b2Body;

NS_CC_BEGIN

class BodyDef;

class GB2ShapeCache 
{
	public :

		// Static interface
		static GB2ShapeCache*	sharedGB2ShapeCache ( KDvoid );
		
	public :

		KDbool		init ( KDvoid );			

		KDvoid		addShapesWithFile ( const std::string& sPlist );

		KDvoid		addFixturesToBody ( b2Body* pBody, const std::string& sShape );

		CCPoint		anchorPointForShape ( const std::string& sShape );
		
		KDvoid		reset ( KDvoid );

		KDfloat		getPtmRatio ( KDvoid );
				
	private :

		std::map<std::string, BodyDef*>		m_aShapeObjects;
		KDfloat								m_fPtmRatio;
};


NS_CC_END

#endif	// __GB2ShapeCache_h__
