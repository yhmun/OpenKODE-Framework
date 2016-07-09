/* --------------------------------------------------------------------------
 *
 *      File            ColoredCircleSprite.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      CJ Hanson on 2010/02/17
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Hanson Interactive. All rights reserved.
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

#ifndef __ColoredCircleSprite_h__
#define __ColoredCircleSprite_h__

NS_CC_BEGIN

class ColoredCircleSprite : public CCNode, public CCRGBAProtocol, public CCBlendProtocol 
{
	public :
				 ColoredCircleSprite ( KDvoid );
		virtual ~ColoredCircleSprite ( KDvoid );

		static ColoredCircleSprite*		create ( const ccColor4B& tColor, KDfloat fRadius );

	public :

		const KDchar*				description ( KDvoid );

		virtual KDbool				init ( KDvoid );

		virtual KDbool				initWithColor ( const ccColor4B& tColor, KDfloat fRadius );

		virtual KDvoid				draw ( KDvoid );

		virtual KDvoid				setContentSize ( const CCSize& tContentSize );

		virtual KDbool				containsPoint ( const CCPoint& tPoint );

		virtual KDfloat				getRadius ( KDvoid );
		virtual KDvoid				setRadius ( KDfloat fRadius );

		virtual KDbool				 isOpacityModifyRGB ( KDvoid );
		virtual KDvoid				setOpacityModifyRGB ( KDbool bIsOpacityModifyRGB ); 

		virtual GLubyte				getOpacity ( KDvoid );
		virtual KDvoid				setOpacity ( GLubyte cOpacity );
		
		virtual GLubyte				getDisplayedOpacity ( KDvoid );
		virtual KDvoid				updateDisplayedOpacity ( GLubyte cParentOpacity );
		
		virtual KDbool				 isCascadeOpacityEnabled ( KDvoid );
		virtual KDvoid				setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled );

		virtual const ccColor3B&	getColor ( KDvoid );
		virtual const ccColor3B&	getDisplayedColor ( KDvoid );
		
		virtual KDvoid				setColor ( const ccColor3B& tColor );
		virtual KDvoid				updateDisplayedColor ( const ccColor3B& tParentColor );
		
		virtual KDbool				 isCascadeColorEnabled ( KDvoid );
		virtual KDvoid				setCascadeColorEnabled ( KDbool bCascadeColorEnabled );

		virtual KDvoid				setBlendFunc ( ccBlendFunc tBlendFunc );
		virtual ccBlendFunc			getBlendFunc ( KDvoid );

	private :

		KDvoid						updateContentSize ( KDvoid );

		KDvoid						updateColor ( KDvoid );


	protected :

		KDfloat						m_fRadius;
		GLubyte						m_cOpacity;
		ccColor3B					m_tColor;
		KDuint						m_uNumberOfSegments;
		GLfloat*					m_pCircleVertices;
		ccBlendFunc					m_tBlendFunc;
};

NS_CC_END

#endif	// __ColoredCircleSprite_h__
