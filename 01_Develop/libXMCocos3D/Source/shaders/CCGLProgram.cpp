/* -----------------------------------------------------------------------------------
 *
 *      File            CCGLProgram.cpp
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

#include "2d/CCDirector.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "ccMacros.h"
#include "platform/CCFileUtils.h"
#include "support/data_support/uthash.h"
#include "base/CCString.h"
// extern
#include "XMKazmath/GL/matrix.h"
#include "XMKazmath/kazmath.h"

NS_CC_BEGIN

typedef struct _hashUniformEntry
{
    GLvoid*         value;       // value
    unsigned int    location;    // Key
    UT_hash_handle  hh;          // hash entry
} tHashUniformEntry;

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR = "ShaderPositionTextureColor";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST = "ShaderPositionTextureColorAlphaTest";
const char* GLProgram::SHADER_NAME_POSITION_COLOR = "ShaderPositionColor";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE = "ShaderPositionTexture";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR = "ShaderPositionTexture_uColor";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_A8_COLOR = "ShaderPositionTextureA8Color";
const char* GLProgram::SHADER_NAME_POSITION_U_COLOR = "ShaderPosition_uColor";
const char* GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR = "ShaderPositionLengthTextureColor";

// uniform names
const char* GLProgram::UNIFORM_NAME_P_MATRIX = "CC_PMatrix";
const char* GLProgram::UNIFORM_NAME_MV_MATRIX = "CC_MVMatrix";
const char* GLProgram::UNIFORM_NAME_MVP_MATRIX  = "CC_MVPMatrix";
const char* GLProgram::UNIFORM_NAME_TIME = "CC_Time";
const char* GLProgram::UNIFORM_NAME_SIN_TIME = "CC_SinTime";
const char* GLProgram::UNIFORM_NAME_COS_TIME = "CC_CosTime";
const char* GLProgram::UNIFORM_NAME_RANDOM01 = "CC_Random01";
const char* GLProgram::UNIFORM_NAME_SAMPLER	= "CC_Texture0";
const char* GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE = "CC_alpha_value";

// Attribute names
const char* GLProgram::ATTRIBUTE_NAME_COLOR = "a_color";
const char* GLProgram::ATTRIBUTE_NAME_POSITION = "a_position";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD = "a_texCoord";

GLProgram::GLProgram()
: m_uProgram(0)
, m_uVertShader(0)
, m_uFragShader(0)
, m_pHashForUniforms(NULL)
, m_tFlags()
{
    kdMemset(m_aUniforms, 0, sizeof(m_aUniforms));
}

GLProgram::~GLProgram()
{
    CCLOGINFO("%s %d deallocing GLProgram: %p", __FUNCTION__, __LINE__, this);

    // there is no need to delete the shaders. They should have been already deleted.
    CCASSERT(m_uVertShader == 0, "Vertex Shaders should have been already deleted");
    CCASSERT(m_uFragShader == 0, "Fragment Shaders should have been already deleted");

    if (m_uProgram) 
    {
        GL::deleteProgram(m_uProgram);
    }

    tHashUniformEntry *current_element, *tmp;

    // Purge uniform hash
    HASH_ITER(hh, m_pHashForUniforms, current_element, tmp)
    {
        HASH_DEL(m_pHashForUniforms, current_element);
        kdFree(current_element->value);
        kdFree(current_element);
    }
}

bool GLProgram::initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    m_uProgram = glCreateProgram();
    CHECK_GL_ERROR_DEBUG();

    m_uVertShader = m_uFragShader = 0;

    if (vShaderByteArray)
    {
        if (!compileShader(&m_uVertShader, GL_VERTEX_SHADER, vShaderByteArray))
        {
            CCLOG("cocos2d: ERROR: Failed to compile vertex shader");
        }
    }

    // Create and compile fragment shader
    if (fShaderByteArray)
    {
        if (!compileShader(&m_uFragShader, GL_FRAGMENT_SHADER, fShaderByteArray))
        {
            CCLOG("cocos2d: ERROR: Failed to compile fragment shader");
        }
    }

    if (m_uVertShader)
    {
        glAttachShader(m_uProgram, m_uVertShader);
    }
    CHECK_GL_ERROR_DEBUG();

    if (m_uFragShader)
    {
        glAttachShader(m_uProgram, m_uFragShader);
    }
    m_pHashForUniforms = NULL;
    
    CHECK_GL_ERROR_DEBUG();

    return true;
}

bool GLProgram::initWithVertexShaderFilename(const char* vShaderFilename, const char* fShaderFilename)
{
    const GLchar * vertexSource = (GLchar*) String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(vShaderFilename).c_str())->getCString();
    const GLchar * fragmentSource = (GLchar*) String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(fShaderFilename).c_str())->getCString();

    return initWithVertexShaderByteArray(vertexSource, fragmentSource);
}

const char* GLProgram::description() const
{
    return String::createWithFormat("<GLProgram = "
                                      CC_FORMAT_PRINTF_SIZE_T
                                      " | Program = %i, VertexShader = %i, FragmentShader = %i>",
                                      (size_t)this, m_uProgram, m_uVertShader, m_uFragShader)->getCString();
}

bool GLProgram::compileShader(GLuint * shader, GLenum type, const GLchar* source)
{
    GLint status;
 
    if (!source)
    {
        return false;
    }
    
    const GLchar *sources[] = {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
        (type == GL_VERTEX_SHADER ? "precision highp float;\n" : "precision mediump float;\n"),
#endif
        "uniform mat4 CC_PMatrix;\n"
        "uniform mat4 CC_MVMatrix;\n"
        "uniform mat4 CC_MVPMatrix;\n"
        "uniform vec4 CC_Time;\n"
        "uniform vec4 CC_SinTime;\n"
        "uniform vec4 CC_CosTime;\n"
        "uniform vec4 CC_Random01;\n"
        "//CC INCLUDES END\n\n",
        source,
    };

    *shader = glCreateShader(type);
    glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (! status)
    {
        GLsizei length;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar *) kdMalloc(sizeof(GLchar) * length);
		
		glGetShaderSource(*shader, length, NULL, src);
		CCLOG("cocos2d: ERROR: Failed to compile shader:\n%s", src);
        
        if (type == GL_VERTEX_SHADER)
        {
            CCLOG("cocos2d: %s", getVertexShaderLog());
        }
        else
        {
            CCLOG("cocos2d: %s", getFragmentShaderLog());
        }
        kdFree(src);

        kdExit(1);
    }
    return (status == GL_TRUE);
}

void GLProgram::addAttribute(const char* attributeName, GLuint index)
{
    glBindAttribLocation(m_uProgram, index, attributeName);
}

void GLProgram::updateUniforms()
{
    m_aUniforms[UNIFORM_P_MATRIX] = glGetUniformLocation(m_uProgram, UNIFORM_NAME_P_MATRIX);
	m_aUniforms[UNIFORM_MV_MATRIX] = glGetUniformLocation(m_uProgram, UNIFORM_NAME_MV_MATRIX);
	m_aUniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(m_uProgram, UNIFORM_NAME_MVP_MATRIX);
	
	m_aUniforms[UNIFORM_TIME] = glGetUniformLocation(m_uProgram, UNIFORM_NAME_TIME);
	m_aUniforms[UNIFORM_SIN_TIME] = glGetUniformLocation(m_uProgram, UNIFORM_NAME_SIN_TIME);
	m_aUniforms[UNIFORM_COS_TIME] = glGetUniformLocation(m_uProgram, UNIFORM_NAME_COS_TIME);

    m_aUniforms[UNIFORM_RANDOM01] = glGetUniformLocation(m_uProgram, UNIFORM_NAME_RANDOM01);

    m_aUniforms[UNIFORM_SAMPLER] = glGetUniformLocation(m_uProgram, UNIFORM_NAME_SAMPLER);

    m_tFlags.usesMVP = m_aUniforms[UNIFORM_MVP_MATRIX] != -1;
	m_tFlags.usesMV = (m_aUniforms[UNIFORM_MV_MATRIX] != -1 && m_aUniforms[UNIFORM_P_MATRIX] != -1 );
	m_tFlags.usesTime = (
                       m_aUniforms[UNIFORM_TIME] != -1 ||
                       m_aUniforms[UNIFORM_SIN_TIME] != -1 ||
                       m_aUniforms[UNIFORM_COS_TIME] != -1
                       );
	m_tFlags.usesRandom = m_aUniforms[UNIFORM_RANDOM01] != -1;

    this->use();
    
    // Since sample most probably won't change, set it to 0 now.
    this->setUniformLocationWith1i(m_aUniforms[UNIFORM_SAMPLER], 0);
}

bool GLProgram::link()
{
    CCASSERT(m_uProgram != 0, "Cannot link invalid program");
    
    GLint status = GL_TRUE;
    
    glLinkProgram(m_uProgram);

    if (m_uVertShader)
    {
        glDeleteShader(m_uVertShader);
    }
    
    if (m_uFragShader)
    {
        glDeleteShader(m_uFragShader);
    }
    
    m_uVertShader = m_uFragShader = 0;
	
#if COCOS2D_DEBUG
    glGetProgramiv(m_uProgram, GL_LINK_STATUS, &status);
	
    if (status == GL_FALSE)
    {
        CCLOG("cocos2d: ERROR: Failed to link program: %i", m_uProgram);
        GL::deleteProgram(m_uProgram);
        m_uProgram = 0;
    }
#endif
	
    return (status == GL_TRUE);
}

void GLProgram::use()
{
    GL::useProgram(m_uProgram);
}

const char* GLProgram::logForOpenGLObject(GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc) const
{
    GLint logLength = 0, charsWritten = 0;

    infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1)
        return 0;

    char *logBytes = (char*) kdMalloc(logLength);
    logFunc(object, logLength, &charsWritten, logBytes);

    String* log = String::create(logBytes);

    kdFree(logBytes);
    return log->getCString();
}

const char* GLProgram::getVertexShaderLog() const
{
    return this->logForOpenGLObject(m_uVertShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

const char* GLProgram::getFragmentShaderLog() const
{
    return this->logForOpenGLObject(m_uFragShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

const char* GLProgram::getProgramLog() const
{
    return this->logForOpenGLObject(m_uProgram, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

// Uniform cache

bool GLProgram::updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes)
{
    if (location < 0)
    {
        return false;
    }
    
    bool updated = true;
    tHashUniformEntry *element = NULL;
    HASH_FIND_INT(m_pHashForUniforms, &location, element);

    if (! element)
    {
        element = (tHashUniformEntry*) kdMalloc( sizeof(*element) );

        // key
        element->location = location;

        // value
        element->value = kdMalloc( bytes );
        memcpy(element->value, data, bytes );

        HASH_ADD_INT(m_pHashForUniforms, location, element);
    }
    else
    {
        if ( kdMemcmp(element->value, data, bytes) == 0)
        {
            updated = false;
        }
        else
        {
            kdMemcpy(element->value, data, bytes);
        }
    }

    return updated;
}

GLint GLProgram::getUniformLocationForName(const char* name) const
{
    CCASSERT(name != NULL, "Invalid uniform name" );
    CCASSERT(m_uProgram != 0, "Invalid operation. Cannot get uniform location when program is not initialized");
    
    return glGetUniformLocation(m_uProgram, name);
}

void GLProgram::setUniformLocationWith1i(GLint location, GLint i1)
{
    bool updated =  updateUniformLocation(location, &i1, sizeof(i1)*1);
    
    if( updated )
    {
        glUniform1i( (GLint)location, i1);
    }
}

void GLProgram::setUniformLocationWith2i(GLint location, GLint i1, GLint i2)
{
    GLint ints[2] = {i1,i2};
    bool updated =  updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform2i( (GLint)location, i1, i2);
    }
}

void GLProgram::setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3)
{
    GLint ints[3] = {i1,i2,i3};
    bool updated =  updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform3i( (GLint)location, i1, i2, i3);
    }
}

void GLProgram::setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4)
{
    GLint ints[4] = {i1,i2,i3,i4};
    bool updated =  updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform4i( (GLint)location, i1, i2, i3, i4);
    }
}

void GLProgram::setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, ints, sizeof(int)*2*numberOfArrays);
    
    if( updated )
    {
        glUniform2iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void GLProgram::setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, ints, sizeof(int)*3*numberOfArrays);
    
    if( updated )
    {
        glUniform3iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void GLProgram::setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, ints, sizeof(int)*4*numberOfArrays);
    
    if( updated )
    {
        glUniform4iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void GLProgram::setUniformLocationWith1f(GLint location, GLfloat f1)
{
    bool updated =  updateUniformLocation(location, &f1, sizeof(f1)*1);

    if( updated )
    {
        glUniform1f( (GLint)location, f1);
    }
}

void GLProgram::setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2)
{
    GLfloat floats[2] = {f1,f2};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform2f( (GLint)location, f1, f2);
    }
}

void GLProgram::setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3)
{
    GLfloat floats[3] = {f1,f2,f3};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform3f( (GLint)location, f1, f2, f3);
    }
}

void GLProgram::setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
    GLfloat floats[4] = {f1,f2,f3,f4};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform4f( (GLint)location, f1, f2, f3,f4);
    }
}

void GLProgram::setUniformLocationWith2fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, floats, sizeof(float)*2*numberOfArrays);

    if( updated )
    {
        glUniform2fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void GLProgram::setUniformLocationWith3fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, floats, sizeof(float)*3*numberOfArrays);

    if( updated )
    {
        glUniform3fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void GLProgram::setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, floats, sizeof(float)*4*numberOfArrays);

    if( updated )
    {
        glUniform4fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void GLProgram::setUniformLocationWithMatrix2fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices) {
    bool updated =  updateUniformLocation(location, matrixArray, sizeof(float)*4*numberOfMatrices);
    
    if( updated )
    {
        glUniformMatrix2fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
    }
}

void GLProgram::setUniformLocationWithMatrix3fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices) {
    bool updated =  updateUniformLocation(location, matrixArray, sizeof(float)*9*numberOfMatrices);
    
    if( updated )
    {
        glUniformMatrix3fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
    }
}


void GLProgram::setUniformLocationWithMatrix4fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices)
{
    bool updated =  updateUniformLocation(location, matrixArray, sizeof(float)*16*numberOfMatrices);

    if( updated )
    {
        glUniformMatrix4fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
    }
}

void GLProgram::setUniformsForBuiltins()
{
    kmMat4 matrixP;
	kmMat4 matrixMV;

	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);
	

    if(m_tFlags.usesMVP) {
        kmMat4 matrixMVP;
        kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
        setUniformLocationWithMatrix4fv(m_aUniforms[UNIFORM_MVP_MATRIX], matrixMVP.mat, 1);
    }

    if(m_tFlags.usesMV) {
        setUniformLocationWithMatrix4fv(m_aUniforms[UNIFORM_P_MATRIX], matrixP.mat, 1);
        setUniformLocationWithMatrix4fv(m_aUniforms[UNIFORM_MV_MATRIX], matrixMV.mat, 1);
    }

	if(m_tFlags.usesTime) {
		Director *director = Director::getInstance();
		// This doesn't give the most accurate global time value.
		// Cocos2D doesn't store a high precision time value, so this will have to do.
		// Getting Mach time per frame per shader using time could be extremely expensive.
        float time = director->getTotalFrames() * director->getAnimationInterval();
		
        setUniformLocationWith4f(m_aUniforms[GLProgram::UNIFORM_TIME], time/10.0, time, time*2, time*4);
        setUniformLocationWith4f(m_aUniforms[GLProgram::UNIFORM_SIN_TIME], time/8.0, time/4.0, time/2.0, kdSinf(time));
        setUniformLocationWith4f(m_aUniforms[GLProgram::UNIFORM_COS_TIME], time/8.0, time/4.0, time/2.0, kdCosf(time));
	}
	
	if(m_tFlags.usesRandom)
        setUniformLocationWith4f(m_aUniforms[GLProgram::UNIFORM_RANDOM01], CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1());
}

void GLProgram::reset()
{
    m_uVertShader = m_uFragShader = 0;
    kdMemset(m_aUniforms, 0, sizeof(m_aUniforms));
    

    // it is already deallocated by android
    //GL::deleteProgram(_program);
    m_uProgram = 0;

    
    tHashUniformEntry *current_element, *tmp;
    
    // Purge uniform hash
    HASH_ITER(hh, m_pHashForUniforms, current_element, tmp) 
    {
        HASH_DEL(m_pHashForUniforms, current_element);
        kdFree(current_element->value);
        kdFree(current_element);
    }
    m_pHashForUniforms = NULL;
}

NS_CC_END
