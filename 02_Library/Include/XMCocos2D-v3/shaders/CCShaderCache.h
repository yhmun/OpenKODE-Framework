/* -----------------------------------------------------------------------------------
 *
 *      File            CCShaderCache.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

#ifndef __CCShaderCache_h__
#define __CCShaderCache_h__

#include <string>
#include <unordered_map>

#include "../base/CCDictionary.h"

NS_CC_BEGIN

class GLProgram;

/**
 *	@addtogroup shaders
 *	@{
 */

/** 
 *	ShaderCache
 *	Singleton that stores manages GL shaders
 *	@since v2.0
 */
class CC_DLL ShaderCache : public Object 
{
	public :

		/**
		 *	@js ctor
		 */
		ShaderCache ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ShaderCache ( KDvoid );

		/** returns the shared instance */
		static ShaderCache*		getInstance ( KDvoid );

		/** purges the cache. It releases the retained instance. */
		static KDvoid			destroyInstance ( KDvoid );

		/** loads the default shaders */
		KDvoid					loadDefaultShaders ( KDvoid );
    
		/** reload the default shaders */
		KDvoid					reloadDefaultShaders ( KDvoid );

		/**
		 *	returns a GL program for a given key 
		 */
		GLProgram*				getProgram ( const std::string& key );

		/** adds a GLProgram to the cache for a given name */
		KDvoid					addProgram ( GLProgram* program, const std::string& key );

	private :

		KDbool					init ( KDvoid );

		KDvoid					loadDefaultShader ( GLProgram* program, KDint type );

		std::unordered_map<std::string, GLProgram*>		m_aPrograms;
};

//	end of shaders group
/// @}

NS_CC_END

#endif	// __CCShaderCache_h__
