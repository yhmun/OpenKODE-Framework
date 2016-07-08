/* -----------------------------------------------------------------------------------
 *
 *      File            CCConfiguration.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Ricardo Quesada
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

#include "2d/CCConfiguration.h"
#include "cocos2d.h"

using namespace std;

NS_CC_BEGIN


Configuration* Configuration::s_pSharedConfiguration = nullptr;

Configuration::Configuration()
: m_nMaxTextureSize(0) 
, m_nMaxModelviewStackDepth(0)
, m_bSupportsPVRTC(false)
, m_bSupportsETC1(false)
, m_bSupportsS3TC(false)
, m_bSupportsATITC(false)
, m_bSupportsNPOT(false)
, m_bSupportsBGRA8888(false)
, m_bSupportsDiscardFramebuffer(false)
, m_bSupportsShareableVAO(false)
, m_nMaxSamplesAllowed(0)
, m_nMaxTextureUnits(0)
, m_pGlExtensions(nullptr)
, m_pValueDict(nullptr)
{
}

bool Configuration::init()
{
	m_pValueDict = Dictionary::create();
	m_pValueDict->retain();

	m_pValueDict->setObject(String::create( cocos2dVersion() ), "cocos2d.x.version");


#if CC_ENABLE_PROFILERS
	m_pValueDict->setObject(Bool::create(true), "cocos2d.x.compiled_with_profiler");
#else
	m_pValueDict->setObject(Bool::create(false), "cocos2d.x.compiled_with_profiler");
#endif

#if CC_ENABLE_GL_STATE_CACHE == 0
	m_pValueDict->setObject(Bool::create(false), "cocos2d.x.compiled_with_gl_state_cache");
#else
	m_pValueDict->setObject(Bool::create(true), "cocos2d.x.compiled_with_gl_state_cache");
#endif

	return true;
}

Configuration::~Configuration()
{
	m_pValueDict->release();
}

void Configuration::dumpInfo() const
{
	// Dump
	PrettyPrinter visitor(0);
	m_pValueDict->acceptVisitor(visitor);

	CCLOG("%s", visitor.getResult().c_str());


	// And Dump some warnings as well
#if CC_ENABLE_PROFILERS
    CCLOG("cocos2d: **** WARNING **** CC_ENABLE_PROFILERS is defined. Disable it when you finish profiling (from ccConfig.h)");
    printf("\n");
#endif

#if CC_ENABLE_GL_STATE_CACHE == 0
    CCLOG("");
    CCLOG("cocos2d: **** WARNING **** CC_ENABLE_GL_STATE_CACHE is disabled. To improve performance, enable it (from ccConfig.h)");
    printf("\n");
#endif

}

void Configuration::gatherGPUInfo()
{
	m_pValueDict->setObject(String::create((const char*)glGetString(GL_VENDOR)), "gl.vendor");
	m_pValueDict->setObject(String::create((const char*)glGetString(GL_RENDERER)), "gl.renderer");
	m_pValueDict->setObject(String::create((const char*)glGetString(GL_VERSION)), "gl.version");

    m_pGlExtensions = (char *)glGetString(GL_EXTENSIONS);

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_nMaxTextureSize);
	m_pValueDict->setObject(Integer::create((int)m_nMaxTextureSize), "gl.max_texture_size");

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_nMaxTextureUnits);
	m_pValueDict->setObject(Integer::create((int)m_nMaxTextureUnits), "gl.max_texture_units");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    glGetIntegerv(GL_MAX_SAMPLES_APPLE, &m_nMaxSamplesAllowed);
	m_pValueDict->setObject(Integer::create((int)m_nMaxSamplesAllowed), "gl.max_samples_allowed");
#endif
    
    m_bSupportsETC1 = checkForGLExtension("GL_OES_compressed_ETC1_RGB8_texture");
    m_pValueDict->setObject(Bool::create(m_bSupportsETC1), "gl.supports_ETC1");
    
    m_bSupportsS3TC = checkForGLExtension("GL_EXT_texture_compression_s3tc");
    m_pValueDict->setObject(Bool::create(m_bSupportsS3TC), "gl.supports_S3TC");
    
    m_bSupportsATITC = checkForGLExtension("GL_AMD_compressed_ATC_texture");
    m_pValueDict->setObject(Bool::create(m_bSupportsATITC), "gl.supports_ATITC");
    
    m_bSupportsPVRTC = checkForGLExtension("GL_IMG_texture_compression_pvrtc");
	m_pValueDict->setObject(Bool::create(m_bSupportsPVRTC), "gl.supports_PVRTC");

    m_bSupportsNPOT = true;
	m_pValueDict->setObject(Bool::create(m_bSupportsNPOT), "gl.supports_NPOT");
	
    m_bSupportsBGRA8888 = checkForGLExtension("GL_IMG_texture_format_BGRA888");
	m_pValueDict->setObject(Bool::create(m_bSupportsBGRA8888), "gl.supports_BGRA8888");

    m_bSupportsDiscardFramebuffer = checkForGLExtension("GL_EXT_discard_framebuffer");
	m_pValueDict->setObject(Bool::create(m_bSupportsDiscardFramebuffer), "gl.supports_discard_framebuffer");

    m_bSupportsShareableVAO = checkForGLExtension("vertex_array_object");
	m_pValueDict->setObject(Bool::create(m_bSupportsShareableVAO), "gl.supports_vertex_array_object");
    
    CHECK_GL_ERROR_DEBUG();
}

Configuration* Configuration::getInstance()
{
    if (! s_pSharedConfiguration)
    {
        s_pSharedConfiguration = new Configuration();
        s_pSharedConfiguration->init();
    }
    
    return s_pSharedConfiguration;
}

void Configuration::destroyInstance()
{
    CC_SAFE_RELEASE(s_pSharedConfiguration);
}

bool Configuration::checkForGLExtension(const string &searchName) const
{
    bool ret = false;
    const char *kSearchName = searchName.c_str();
    
    if (m_pGlExtensions && 
        strstr(m_pGlExtensions, kSearchName))
    {
        ret = true;
    }
    
    return ret;
}

//
// getters for specific variables.
// Mantained for backward compatiblity reasons only.
//
int Configuration::getMaxTextureSize() const
{
	return m_nMaxTextureSize;
}

int Configuration::getMaxModelviewStackDepth() const
{
	return m_nMaxModelviewStackDepth;
}

int Configuration::getMaxTextureUnits() const
{
	return m_nMaxTextureUnits;
}

bool Configuration::supportsNPOT() const
{
	return m_bSupportsNPOT;
}

bool Configuration::supportsPVRTC() const
{
	return m_bSupportsPVRTC;
}

bool Configuration::supportsETC() const
{
    //GL_ETC1_RGB8_OES is not defined in old opengl version
#ifdef GL_ETC1_RGB8_OES
    return m_bSupportsETC1;
#else
    return false;
#endif
}

bool Configuration::supportsS3TC() const
{
    return m_bSupportsS3TC;
}

bool Configuration::supportsATITC() const
{
    return m_bSupportsATITC;
}

bool Configuration::supportsBGRA8888() const
{
	return m_bSupportsBGRA8888;
}

bool Configuration::supportsDiscardFramebuffer() const
{
	return m_bSupportsDiscardFramebuffer;
}

bool Configuration::supportsShareableVAO() const
{
	#if CC_TEXTURE_ATLAS_USE_VAO
    	return m_bSupportsShareableVAO;
 	#else
 		return false;
 	#endif
}

//
// generic getters for properties
//
const char *Configuration::getCString(const char *key, const char *defaultValue) const
{
	Object *ret = m_pValueDict->objectForKey(key);
	if (ret)
    {
		if (String *str=dynamic_cast<String*>(ret))
			return str->getCString();

		CCASSERT(false, "Key found, but from different type");
	}

	// XXX: Should it throw an exception ?
	return defaultValue;
}

/** returns the value of a given key as a boolean */
bool Configuration::getBool(const char *key, bool defaultValue) const
{
	Object *ret = m_pValueDict->objectForKey(key);
	if (ret)
    {
		if (Bool *boolobj=dynamic_cast<Bool*>(ret))
			return boolobj->getValue();
		if (String *strobj=dynamic_cast<String*>(ret))
			return strobj->boolValue();
		CCASSERT(false, "Key found, but from different type");
	}

	// XXX: Should it throw an exception ?
	return defaultValue;
}

/** returns the value of a given key as a double */
double Configuration::getNumber( const char *key, double defaultValue ) const
{
	Object *ret = m_pValueDict->objectForKey(key);
	if( ret )
    {
		if (Double *obj=dynamic_cast<Double*>(ret))
			return obj->getValue();

		if (Integer *obj=dynamic_cast<Integer*>(ret))
			return obj->getValue();

		if (String *strobj=dynamic_cast<String*>(ret))
			return strobj->doubleValue();

		CCASSERT(false, "Key found, but from different type");
	}

	// XXX: Should it throw an exception ?
	return defaultValue;
}

Object * Configuration::getObject(const char *key) const
{
	return m_pValueDict->objectForKey(key);
}

void Configuration::setObject(const char *key, Object *value)
{
	m_pValueDict->setObject(value, key);
}


//
// load file
//
void Configuration::loadConfigFile(const char *filename)
{
	Dictionary *dict = Dictionary::createWithContentsOfFile(filename);
	CCASSERT(dict, "cannot create dictionary");

	// search for metadata
	bool validMetadata = false;
	Object *metadata = dict->objectForKey("metadata");
	if (metadata && dynamic_cast<Dictionary*>(metadata))
    {
		Object *format_o = static_cast<Dictionary*>(metadata)->objectForKey("format");

		// XXX: cocos2d-x returns Strings when importing from .plist. This bug will be addressed in cocos2d-x v3.x
		if (format_o && dynamic_cast<String*>(format_o))
        {
			int format = static_cast<String*>(format_o)->intValue();

			// Support format: 1
			if (format == 1)
            {
				validMetadata = true;
			}
		}
	}

	if (! validMetadata)
    {
		CCLOG("Invalid config format for file: %s", filename);
		return;
	}

	Object *data = dict->objectForKey("data");
	if (!data || !dynamic_cast<Dictionary*>(data))
    {
		CCLOG("Expected 'data' dict, but not found. Config file: %s", filename);
		return;
	}

	// Add all keys in the existing dictionary
	Dictionary *data_dict = static_cast<Dictionary*>(data);
    DictElement* element;
    CCDICT_FOREACH(data_dict, element)
    {
		if(! m_pValueDict->objectForKey( element->getStrKey() ))
			m_pValueDict->setObject(element->getObject(), element->getStrKey());
		else
			CCLOG("Key already present. Ignoring '%s'", element->getStrKey());
    }
}

NS_CC_END
