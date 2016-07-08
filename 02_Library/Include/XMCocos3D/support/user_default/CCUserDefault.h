/* -----------------------------------------------------------------------------------
 *
 *      File            CCUserDefault.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      ForzeField Studios S.L. 
 *
 *         http://www.cocos2d-x.org      
 *         http://forzefield.com   
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

#ifndef __CCUserDefault_h__
#define __CCUserDefault_h__

#include "../../platform/CCPlatformMacros.h"
#include "../../base/CCData.h"
#include <string>

NS_CC_BEGIN

/**
 *	@addtogroup data_storage
 *	@{
 */

/**
 *	UserDefault acts as a tiny database. You can save and get base type values by it.
 *	For example, setBoolForKey("played", true) will add a bool value true into the database.
 *	Its key is "played". You can get the value of the key by getBoolForKey("played").
 * 
 *	It supports the following base types:
 *	bool, int, float, double, string
 */
class CC_DLL UserDefault
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~UserDefault ( KDvoid );

		// get value methods

		/**
		 *	@brief Get bool value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is false.
		 *	@js NA
		 */
		KDbool						getBoolForKey ( const KDchar* pKey );

		/**
		 *	@js NA
		 */
		KDbool						getBoolForKey ( const KDchar* pKey, KDbool bDefaultValue );

		/**
		 *	@brief Get integer value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is 0.
		 *	@js NA
		 */
		KDint						getIntegerForKey ( const KDchar* pKey );

		/**
		 *	@js NA
		 */
		KDint						getIntegerForKey ( const KDchar* pKey, KDint nDefaultValue );

		/**
		 *	@brief Get float value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is 0.0f.
		 *	@js NA
		 */
		KDfloat						getFloatForKey ( const KDchar* pKey );

		/**
		 * @js NA
		 */
		KDfloat						getFloatForKey ( const KDchar* pKey, KDfloat fDefaultValue );

		/**
		 *	@brief Get double value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is 0.0.
		 *	@js NA
		 */
		KDdouble					getDoubleForKey ( const KDchar* pKey );
		/**
		 *	@js NA
		 */
		KDdouble					getDoubleForKey ( const KDchar* pKey, KDdouble dDefaultValue );

		/**
		 *	@brief Get string value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is "".
		 *	@js NA
		 */
		std::string					getStringForKey ( const KDchar* pKey );

		/**
		 *	@js NA
		 */
		std::string					getStringForKey ( const KDchar* pKey, const std::string& sDefaultValue );

		/**
		 *	@brief Get binary data value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is null.
		 *	@js NA
		 *	@lua NA
		 */
		Data*						getDataForKey ( const KDchar* pKey );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Data*						getDataForKey ( const KDchar* pKey, Data* pDefaultValue );

		// set value methods

		/**
		 *	@brief Set bool value by key.
		 *	@js NA
		 */
		KDvoid						setBoolForKey ( const KDchar* pKey, KDbool bValue );

		/**
		 *	@brief Set integer value by key.
		 *	@js NA
		 */
		KDvoid						setIntegerForKey ( const KDchar* pKey, KDint nValue );

		/**
		 *	@brief Set float value by key.
		 *	@js NA
		 */
		KDvoid						setFloatForKey ( const KDchar* pKey, KDfloat fValue );

		/**
		 *	@brief Set double value by key.
		 *	@js NA
		 */
		KDvoid						setDoubleForKey ( const KDchar* pKey, KDdouble dValue );
		
		/**
		 *	@brief Set string value by key.
		 *	@js NA
		 */
		KDvoid						setStringForKey ( const KDchar* pKey, const std::string& sValue );

		/**
		 *	@brief Set binary data value by key.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						setDataForKey ( const KDchar* pKey, const Data& tValue );

		/**
		 *	@brief Save content to xml file
		 *	@js NA
		 */
		void    flush();

		/** 
		 *	returns the singleton 
		 *	@js NA
		 *	@lua NA
		 */
		static UserDefault*			getInstance ( KDvoid );
		/**
		 *	@js NA
		 */
		static KDvoid				destroyInstance ( KDvoid );

		/**
		 *	@js NA
		 */
		const static std::string&	getXMLFilePath ( KDvoid );

		/**
		 *	@js NA
		 */
		static KDbool				isXMLFileExist ( KDvoid );

	private :

		UserDefault ( KDvoid );
		static KDbool				createXMLFile ( KDvoid );
		static KDvoid				initXMLFilePath ( KDvoid );
    
		static UserDefault*			m_pUserDefault;
		static std::string			m_sFilePath;
		static KDbool				m_bIsFilePathInitialized;
};

//	end of data_storage group
/// @}

NS_CC_END

#endif	// __CCUserDefault_h__
