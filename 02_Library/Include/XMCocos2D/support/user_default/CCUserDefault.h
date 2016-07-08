/* -----------------------------------------------------------------------------------
 *
 *      File            CCUserDefault.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCUserDefault_h__
#define __CCUserDefault_h__

#include "../../ccTypes.h"

#include <string>

NS_CC_BEGIN
    
/**
 * @addtogroup data_storage
 * @{
 */

/**
 *	CCUserDefault acts as a tiny database. You can save and get base type values by it.
 *	For example, setBoolForKey("played", true) will add a bool value true into the database.
 *	Its key is "played". You can get the value of the key by getBoolForKey("played").
 * 
 *	It supports the following base types:
 *	bool, int, float, double, string
 */
class CCUserDefault
{
	protected :
				 CCUserDefault ( KDvoid );
		virtual ~CCUserDefault ( KDvoid );

	public :

		static CCUserDefault*		 sharedUserDefault ( KDvoid );
		static KDvoid			purgeSharedUserDefault ( KDvoid );

		const static std::string&	getXMLFilePath ( KDvoid );

	public :

		// get value methods

		/**
		 *	@brief	Get bool value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is false.
		 */
		KDbool			getBoolForKey ( const KDchar* szKey );
		KDbool			getBoolForKey ( const KDchar* szKey, KDbool bDefault );

		/**
		 *	@brief	Get integer value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is 0.
		 */
		KDint			getIntegerForKey ( const KDchar* szKey );
		KDint			getIntegerForKey ( const KDchar* szKey, KDint nDefault );

		/**
		 *	@brief	Get float value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is 0.0f.
		 */
		KDfloat			getFloatForKey ( const KDchar* szKey );
		KDfloat			getFloatForKey ( const KDchar* szKey, KDfloat fDefault );

		/**
		 *	@brief	Get double value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is 0.0.
		 */
		KDdouble		getDoubleForKey ( const KDchar* szKey );
		KDdouble		getDoubleForKey ( const KDchar* szKey, KDdouble dDefault );

		/**
		 *	@brief	Get string value by key, if the key doesn't exist, a default value will return.
		 *	You can set the default value, or it is "".
		 */
		std::string		getStringForKey ( const KDchar* szKey );
		std::string		getStringForKey ( const KDchar* szKey, const std::string sDefault );

		// set value methods

		/**
		 *	@brief	Set bool value by key.
		 */
		KDvoid			setBoolForKey ( const KDchar* szKey, KDbool bValue );

		/**
		 *	@brief	Set integer value by key.
		 */
		KDvoid			setIntegerForKey ( const KDchar* szKey, KDint nValue );

		/**
		 *	@brief	Set float value by key.
		 */
		KDvoid			setFloatForKey ( const KDchar* szKey, KDfloat fValue );

		/**
		 *	@brief	Set double value by key.
		 */
		KDvoid			setDoubleForKey ( const KDchar* szKey, KDdouble dValue );

		/**
		 *	@brief	Set string value by key.
		 */
		KDvoid			setStringForKey ( const KDchar* szKey, std::string sValue );

		/**
		 *	@brief Save content to xml file
		 */
		KDvoid			flush ( KDvoid );		

	private :

		static KDbool  createXMLFile ( KDvoid );
		static KDbool  isXMLFileExist ( KDvoid );
		static KDvoid  initXMLFilePath ( KDvoid );
};

// end of data_storage group
/// @}

NS_CC_END

#endif // __CCUserDefault_h__
