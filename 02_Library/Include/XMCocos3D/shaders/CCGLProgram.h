/* -----------------------------------------------------------------------------------
 *
 *      File            CCGLProgram.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011 Jeff Lamarche
 *      Copyright (c) 2012 Goffredo Marocchi
 *      Copyright (c) 2012 Ricardo Quesada
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

#ifndef __CCGLProgram_h__
#define __CCGLProgram_h__

#include "../ccMacros.h"
#include "../base/CCObject.h"

#include "../platform/CCGL.h"

NS_CC_BEGIN

/**
 *	@addtogroup shaders
 *	@{
 */

struct _hashUniformEntry;

typedef KDvoid (*GLInfoFunction) (GLuint program, GLenum pname, GLint* params );
typedef KDvoid (*GLLogFunction)  (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog );

/** 
 *	GLProgram
 *	Class that implements a glProgram
 *
 *
 *	@since v2.0.0
 */
class CC_DLL GLProgram : public Object
{
	public :

		enum
		{
			VERTEX_ATTRIB_POSITION	,
			VERTEX_ATTRIB_COLOR		,
			VERTEX_ATTRIB_TEX_COORDS,
        
			VERTEX_ATTRIB_MAX		,
		};
    
		enum
		{
			UNIFORM_P_MATRIX		,
			UNIFORM_MV_MATRIX		,
			UNIFORM_MVP_MATRIX		,
			UNIFORM_TIME			,
			UNIFORM_SIN_TIME		,
			UNIFORM_COS_TIME		,
			UNIFORM_RANDOM01		,
			UNIFORM_SAMPLER			,
        
			UNIFORM_MAX				,
		};
    
		static const KDchar*		SHADER_NAME_POSITION_TEXTURE_COLOR;
		static const KDchar*		SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST;
		static const KDchar*		SHADER_NAME_POSITION_COLOR;
		static const KDchar*		SHADER_NAME_POSITION_TEXTURE;
		static const KDchar*		SHADER_NAME_POSITION_TEXTURE_U_COLOR;
		static const KDchar*		SHADER_NAME_POSITION_TEXTURE_A8_COLOR;
		static const KDchar*		SHADER_NAME_POSITION_U_COLOR;
		static const KDchar*		SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR;
    
		// uniform names
		static const KDchar*		UNIFORM_NAME_P_MATRIX;
		static const KDchar*		UNIFORM_NAME_MV_MATRIX;
		static const KDchar*		UNIFORM_NAME_MVP_MATRIX;
		static const KDchar*		UNIFORM_NAME_TIME;
		static const KDchar*		UNIFORM_NAME_SIN_TIME;
		static const KDchar*		UNIFORM_NAME_COS_TIME;
		static const KDchar*		UNIFORM_NAME_RANDOM01;
		static const KDchar*		UNIFORM_NAME_SAMPLER;
		static const KDchar*		UNIFORM_NAME_ALPHA_TEST_VALUE;
    
		// Attribute names
		static const KDchar*		ATTRIBUTE_NAME_COLOR;
		static const KDchar*		ATTRIBUTE_NAME_POSITION;
		static const KDchar*		ATTRIBUTE_NAME_TEX_COORD;

		/**
		 *	@js ctor
		 */
		GLProgram ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~GLProgram ( KDvoid );

		/**
		 *	Initializes the GLProgram with a vertex and fragment with bytes array 
		 *	@js initWithString
		 *	@lua initWithString
		 */
		KDbool						initWithVertexShaderByteArray ( const GLchar* vShaderByteArray, const GLchar* fShaderByteArray );
		
		/**
		 *	Initializes the GLProgram with a vertex and fragment with contents of filenames 
		 *	@js init
		 *	@lua init
		 */
		KDbool						initWithVertexShaderFilename ( const KDchar* vShaderFilename, const KDchar* fShaderFilename );

		/**  It will add a new attribute to the shader */
		KDvoid						addAttribute ( const KDchar* attributeName, GLuint index );
		
		/** links the glProgram */
		KDbool						link ( KDvoid );
		
		/** it will call glUseProgram() */
		KDvoid						use ( KDvoid );
	
		/**
		 *	It will create 4 uniforms:
		 *		- kUniformPMatrix
		 *		- kUniformMVMatrix
		 *		- kUniformMVPMatrix
		 *		- GLProgram::UNIFORM_SAMPLER
		 *
		 *	And it will bind "GLProgram::UNIFORM_SAMPLER" to 0
		 *
		 */
		KDvoid						updateUniforms ( KDvoid );
    
		/** calls retrieves the named uniform location for this shader program. */
		GLint						getUniformLocationForName ( const KDchar* name ) const;
    
		/** 
		 *	calls glUniform1i only if the values are different than the previous call for this same shader program. 
		 *	@js setUniformLocationI32
		 *	@lua setUniformLocationI32
		 */
		KDvoid						setUniformLocationWith1i ( GLint location, GLint i1 );
    
		/** calls glUniform2i only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWith2i ( GLint location, GLint i1, GLint i2 );
    
		/** calls glUniform3i only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWith3i ( GLint location, GLint i1, GLint i2, GLint i3 );
    
		/** calls glUniform4i only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWith4i ( GLint location, GLint i1, GLint i2, GLint i3, GLint i4 );
    
		/** calls glUniform2iv only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWith2iv ( GLint location, GLint* ints, KDuint numberOfArrays );
    
		/** calls glUniform3iv only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWith3iv ( GLint location, GLint* ints, KDuint numberOfArrays );
    
		/** calls glUniform4iv only if the values are different than the previous call for this same shader program. */    
		KDvoid						setUniformLocationWith4iv ( GLint location, GLint* ints, KDuint numberOfArrays );

		/** 
		 *	calls glUniform1f only if the values are different than the previous call for this same shader program. 
		 *	In js or lua,please use setUniformLocationF32
		 *	@js NA
		 */
		KDvoid						setUniformLocationWith1f ( GLint location, GLfloat f1 );

		/**
		 *	calls glUniform2f only if the values are different than the previous call for this same shader program. 
		 *	In js or lua,please use setUniformLocationF32
		 *	@js NA
		 */
		KDvoid						setUniformLocationWith2f ( GLint location, GLfloat f1, GLfloat f2 );

		/** 
		 *	calls glUniform3f only if the values are different than the previous call for this same shader program. 
		 *	In js or lua,please use setUniformLocationF32
		 *	@js NA
		 */
		KDvoid						setUniformLocationWith3f ( GLint location, GLfloat f1, GLfloat f2, GLfloat f3 );

		/** 
		 *	calls glUniform4f only if the values are different than the previous call for this same shader program. 
		 *	In js or lua,please use setUniformLocationF32
		 *	@js NA
		 */
		KDvoid						setUniformLocationWith4f ( GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4 );

		/** calls glUniform2fv only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWith2fv ( GLint location, GLfloat* floats, KDuint numberOfArrays );

		/** calls glUniform3fv only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWith3fv ( GLint location, GLfloat* floats, KDuint numberOfArrays );

		/** calls glUniform4fv only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWith4fv ( GLint location, GLfloat* floats, KDuint numberOfArrays );

		/** calls glUniformMatrix2fv only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWithMatrix2fv ( GLint location, GLfloat* matrixArray, KDuint numberOfMatrices );
    
		/** calls glUniformMatrix3fv only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWithMatrix3fv ( GLint location, GLfloat* matrixArray, KDuint numberOfMatrices );
    
		/** calls glUniformMatrix4fv only if the values are different than the previous call for this same shader program. */
		KDvoid						setUniformLocationWithMatrix4fv ( GLint location, GLfloat* matrixArray, KDuint numberOfMatrices );
    
		/** will update the builtin uniforms if they are different than the previous call for this same shader program. */
		KDvoid						setUniformsForBuiltins ( KDvoid );

		/** returns the vertexShader error log */
		const KDchar*				getVertexShaderLog ( KDvoid ) const;

		/** returns the fragmentShader error log */
		const KDchar*				getFragmentShaderLog ( KDvoid ) const;

		/** returns the program error log */
		const KDchar*				getProgramLog ( KDvoid ) const;
    
		// reload all shaders, this function is designed for android
		// when opengl context lost, so don't call it.
		KDvoid						reset ( KDvoid );
    
		inline const GLuint			getProgram ( KDvoid ) const { return m_uProgram; }

	private :

		KDbool						updateUniformLocation ( GLint location, GLvoid* data, KDuint bytes );
		const KDchar*				description ( KDvoid ) const;
		KDbool						compileShader ( GLuint * shader, GLenum type, const GLchar* source );
		const KDchar*				logForOpenGLObject ( GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc ) const;

	private :

		GLuint						m_uProgram;
		GLuint						m_uVertShader;
		GLuint						m_uFragShader;
		GLint						m_aUniforms [ UNIFORM_MAX ];
		struct _hashUniformEntry*	m_pHashForUniforms;

		struct flag_struct 
		{
			KDuint					usesTime:1;
			KDuint					usesMVP:1;
			KDuint					usesMV:1;
			KDuint					usesRandom:1;

			// handy way to initialize the bitfield
			flag_struct ( KDvoid ) { kdMemset ( this, 0, sizeof ( *this ) ); }
		} m_tFlags;
};

// end of shaders group
/// @}

NS_CC_END

#endif	// __CCGLProgram_h__
