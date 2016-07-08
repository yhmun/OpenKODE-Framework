/* -----------------------------------------------------------------------------------
 *
 *      File            CCProtocols.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCProtocols_h__
#define __CCProtocols_h__

#include "ccTypes.h"
#include "2d/textures/CCTexture2D.h"
#include <string>

NS_CC_BEGIN

/**
 *	RGBA protocol that affects Node's color and opacity
 */
class CC_DLL RGBAProtocol
{
	public :

		virtual ~RGBAProtocol ( KDvoid ) { }

		/**
		 *	Changes the color with R,G,B bytes
		 *
		 *	@param color Example: Color3B(255,100,0) means R=255, G=100, B=0
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setColor ( const Color3B& tColor ) = 0;

		/**
		 *	Returns color that is currently used.
		 *
		 *	@return The Color3B contains R,G,B bytes.
		 *	@js NA
		 *	@lua NA
		 */
		virtual const Color3B&		getColor ( KDvoid ) const = 0;
    
		/**
		 *	Returns the displayed color.
		 *
		 *	@return The Color3B contains R,G,B bytes.
		 *	@js NA
		 *	@lua NA
		 */
		virtual const Color3B&		getDisplayedColor ( KDvoid ) const = 0;
    
		/**
		 *	Returns the displayed opacity.
		 *
		 *	@return  The opacity of sprite, from 0 ~ 255
		 *	@js NA
		 *	@lua NA
		 */
		virtual GLubyte				getDisplayedOpacity ( KDvoid ) const = 0;

		/**
		 *	Returns the opacity.
		 *
		 *	The opacity which indicates how transparent or opaque this node is.
		 *	0 indicates fully transparent and 255 is fully opaque.
		 *
		 *	@return  The opacity of sprite, from 0 ~ 255
		 *	@js NA
		 *	@lua NA
		 */
		virtual GLubyte				getOpacity ( KDvoid ) const = 0;

		/**
		 *	Changes the opacity.
		 *
		 *	@param opacity   Goes from 0 to 255, where 255 means fully opaque and 0 means fully transparent.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setOpacity ( GLubyte cOpacity ) = 0;

		// optional

		/**
		 *	Changes the OpacityModifyRGB property. 
		 *	If thie property is set to true, then the rendered color will be affected by opacity.
		 *	Normally, r = r * opacity/255, g = g * opacity/255, b = b * opacity/255.
		 *
		 *	@param value If true, then the opacity will be applied as: glColor(R,G,B,opacity);
		 *               If false, then the opacity will be applied as: glColor(opacity, opacity, opacity, opacity);
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setOpacityModifyRGB ( KDbool bValue ) = 0;

		/**
		 *	Returns whether or not the opacity will be applied using glColor(R,G,B,opacity) 
		 *	or glColor(opacity, opacity, opacity, opacity)
		 *
		 *	@return  Returns opacity modify flag.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool				isOpacityModifyRGB ( KDvoid ) const = 0;
    
		/**
		 *  whether or not color should be propagated to its children.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool				isCascadeColorEnabled ( KDvoid ) const = 0;
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setCascadeColorEnabled ( KDbool bCascadeColorEnabled ) = 0;
    
		/** 
		 *  recursive method that updates display color 
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				updateDisplayedColor ( const Color3B& tColor ) = 0;
    
		/** 
		 *  whether or not opacity should be propagated to its children.
		 *  @js NA
		 *  @lua NA
		 */
		virtual KDbool				isCascadeOpacityEnabled ( KDvoid ) const = 0;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled ) = 0;
    
		/**
		 *  recursive method that updates the displayed opacity.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				updateDisplayedOpacity ( GLubyte cOpacity ) = 0;
};

/**
 *	Specify the blending function according glBlendFunc
 *	Please refer to glBlendFunc in OpenGL ES Manual
 *	http://www.khronos.org/opengles/sdk/docs/man/xhtml/glBlendFunc.xml for more details.
 */
class CC_DLL BlendProtocol
{
	public :

		virtual ~BlendProtocol ( KDvoid ) { }

		/**
		 *	Sets the source blending function.
		 *
		 *	@param blendFunc A structure with source and destination factor to specify pixel arithmetic, 
		 *	                  e.g. {GL_ONE, GL_ONE}, {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setBlendFunc ( const BlendFunc& tBlendFunc ) = 0;

		/**
		 *	Returns the blending function that is currently being used.
		 * 
		 *	@return A BlendFunc structure with source and destination factor which specified pixel arithmetic.
		 *	@js NA
		 *	@lua NA
		 */
		virtual const BlendFunc&	getBlendFunc ( KDvoid ) const = 0;
};

/** 
 *	Node objects that uses a Texture2D to render the images.
 *	The texture can have a blending function.
 *	If the texture has alpha premultiplied the default blending function is:
 *	   src=GL_ONE dst= GL_ONE_MINUS_SRC_ALPHA
 *	else
 *	   src=GL_SRC_ALPHA dst= GL_ONE_MINUS_SRC_ALPHA
 *	But you can change the blending function at any time.
 */
class CC_DLL TextureProtocol : public BlendProtocol
{
	public :

		virtual ~TextureProtocol ( KDvoid ) { }
		
		/**
		 *	Returns the currently used texture
		 *
		 *	@return  The texture that is currenlty being used.
		 *	@js NA
		 *	@lua NA
		 */
		virtual Texture2D*			getTexture ( KDvoid ) const = 0;

		/**
		 *	Sets a new texuture. It will be retained.
		 *
		 *	@param   texture A valid Texture2D object, which will be applied to this sprite object.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setTexture ( Texture2D* pTexture ) = 0;
};

/**
 *	Common interface for Labels
 */
class CC_DLL LabelProtocol
{
	public :

		virtual ~LabelProtocol ( KDvoid ) { }

		/**
		 *	Sets a new label using a string
		 *
		 *	@param label A null terminated string
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				setString ( const std::string& sLabel ) = 0;

		/** 
		 *	Returns the string that is currently being used in this label 
		 *
		 *	@return The string that is currently being used in this label
		 *	@js NA
		 *	@lua NA
		 */
		virtual const std::string&	getString ( KDvoid ) const = 0;
};

/** 
 *	OpenGL projection protocol 
 */
class CC_DLL DirectorDelegate
{
	public :

		virtual ~DirectorDelegate ( KDvoid ) { }

		/**
		 *	Will be called by Director when the projection is updated, and "custom" projection is used
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				updateProjection ( KDvoid ) = 0;
};

NS_CC_END

#endif	// __CCProtocols_h__
