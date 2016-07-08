/* -----------------------------------------------------------------------------------
 *
 *      File            CCDictionary.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCDictionary_h__
#define __CCDictionary_h__

#include "../support/data_support/uthash.h"
#include "CCObject.h"
#include "CCArray.h"
#include "CCString.h"

NS_CC_BEGIN

class Dictionary;

/**
 *	@addtogroup data_structures
 *	@{
 */


/**
 *	DictElement is used for traversing Dictionary.
 *
 *  A DictElement is one element of Dictionary, it contains two properties, key and object.
 *  Its key has two different type (integer and string).
 *
 *  @note The key type is unique, all the elements in Dictionary has the same key type(integer or string).
 *  @code
 *  DictElement* pElement;
 *  CCDICT_FOREACH(dict, pElement)
 *  {
 *      const char*key = pElement->getStrKey();
 *      // You certainly know the type of value, so we assume that it's a Sprite.
 *      Sprite* pSprite = static_cast<Sprite*>(pElement->getObject());
 *      // ......
 *  }
 *  @endcode
 *
 */
class CC_DLL DictElement
{
	private :

		/**
		 *  Constructor of DictElement. It's only for internal usage. Dictionary is its friend class.
		 *  
		 *  @param  pszKey    The string key of this element.
		 *  @param  pObject   The object of this element.
		 */
		DictElement ( const KDchar* pszKey, Object* pObject );

		/**
		 *  Constructor of DictElement. It's only for internal usage. Dictionary is its friend class.
		 *
		 *  @param  iKey    The integer key of this element.
		 *  @param  pObject   The object of this element.
		 */
		DictElement ( KDintptr iKey, Object* pObject );
    
	public :

		/**
		 *  The destructor of DictElement.
		 *	@js NA
		 *	@lua NA
		 */
		~DictElement ( KDvoid );

		// Inline functions need to be implemented in header file on Android.
    
		/**
		 *	Get the string key of this element.
		 *	@note    This method assumes you know the key type in the element. 
		 *           If the element's key type is integer, invoking this method will cause an assert.
		 *
		 *	@return  The string key of this element.
		 */
		inline const KDchar*	getStrKey ( KDvoid ) const
		{
			CCASSERT ( m_pStrKey [ 0 ] != '\0', "Should not call this function for integer dictionary" );
			return m_pStrKey;
		}

		/**
		 *	Get the integer key of this element.
		 *	@note    This method assumes you know the key type in the element.
		 *           If the element's key type is string, invoking this method will cause an assert.
		 *
		 *	@return  The integer key of this element.
		 */
		inline KDintptr			getIntKey ( KDvoid ) const
		{
			CCASSERT ( m_pStrKey [ 0 ] == '\0', "Should not call this function for string dictionary" );
			return m_nIntKey;
		}
    
		/**
		 *	Get the object of this element.
		 *
		 *	@return  The object of this element.
		 */
		inline Object*			getObject ( KDvoid ) const { return m_pObject; }

	private :

		// The max length of string key.
		#define   MAX_KEY_LEN   256

		// char array is needed for HASH_ADD_STR in UT_HASH.
		// So it's a pain that all elements will allocate 256 bytes for this array.
		KDchar					m_pStrKey [ MAX_KEY_LEN ];	// hash key of string type
		KDintptr				m_nIntKey;					// hash key of integer type
		Object*					m_pObject;					// hash value

	public :

		UT_hash_handle			hh;							// makes this class hashable
		friend class Dictionary;							// declare Dictionary as friend class
};

/** The macro for traversing dictionary
 *  
 *  @note It's faster than getting all keys and traversing keys to get objects by objectForKey.
 *        It's also safe to remove elements while traversing.
 */
#define CCDICT_FOREACH(__dict__, __el__)									\
    DictElement* pTmp##__dict__##__el__ = nullptr;							\
    if (__dict__)															\
    HASH_ITER(hh, (__dict__)->m_pElements, __el__, pTmp##__dict__##__el__)



/**
 *  Dictionary is a class like NSDictionary in Obj-C .
 *
 *  @note Only the pointer of Object or its subclass can be inserted to Dictionary.
 *  @code
 *  // Create a dictionary, return an autorelease object.
 *  Dictionary* pDict = Dictionary::create();
 *
 *  // Insert objects to dictionary
 *  String* pValue1 = String::create("100");
 *  String* pValue2 = String::create("120");
 *  Integer* pValue3 = Integer::create(200);
 *  pDict->setObject(pValue1, "key1");
 *  pDict->setObject(pValue2, "key2");
 *  pDict->setObject(pValue3, "key3");
 *
 *  // Get the object for key
 *  String* pStr1 = (String*)pDict->objectForKey("key1");
 *  log("{ key1: %s }", pStr1->getCString());
 *  Integer* pInteger = (Integer*)pDict->objectForKey("key3");
 *  log("{ key3: %d }", pInteger->getValue());
 *  @endcode
 *
 */

class CC_DLL Dictionary : public Object, public Clonable
{
	public :

		/**
		 *	The constructor of Dictionary.
		 *	@js NA
		 *	@lua NA
		 */
		Dictionary ( KDvoid );

		/**
		 *	The destructor of Dictionary
		 *	@js NA
		 *	@lua NA
		 */
		~Dictionary ( KDvoid );

		/** 
		 *	Initializes the dictionary. It returns true if the initializations was successful. 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool					init ( KDvoid );

		/**
		 *  Get the count of elements in Dictionary.
		 *
		 *  @return  The count of elements.
		 *	@js NA
		 */
		KDuint					count ( KDvoid );

		/**
		 *  Return all keys of elements.
		 *
		 *  @return  The array contains all keys of elements. It's an autorelease object yet.
		 *	@js NA
		 */
		Array*					allKeys ( KDvoid );

		/** 
		 *  Get all keys according to the specified object.
		 *  @warning  We use '==' to compare two objects
		 *  @return   The array contains all keys for the specified object. It's an autorelease object yet.
		 *	@js NA
		 */
		Array*					allKeysForObject ( Object* pObject );

		/**
		 *  Get the object according to the specified string key.
		 *
		 *  @note The dictionary needs to use string as key. If integer is passed, an assert will appear.
		 *  @param key  The string key for searching.
		 *  @return The object matches the key. You need to force convert it to the type you know.
		 *  @code
		 *     // Assume that the elements are String* pointers. Convert it by following code.
		 *     String* pStr = (String*)pDict->objectForKey("key1");
		 *     // Do something about pStr.
		 *     // If you don't know the object type, properly you need to use dynamic_cast<SomeType*> to check it.
		 *     String* pStr2 = dynamic_cast<String*>(pDict->objectForKey("key1"));
		 *     if (pStr2 != NULL) {
		 *          // Do something about pStr2
		 *     }
		 *  @endcode
		 *  @see objectForKey(KDintptr)
		 *	@js NA
		 */
		Object*					objectForKey ( const std::string& sKey );
    
		/**
		 *  Get the object according to the specified integer key.
		 *
		 *  @note The dictionary needs to use integer as key. If string is passed, an assert will appear.
		 *  @param key  The integer key for searching.
		 *  @return The object matches the key.
		 *  @see objectForKey(const std::string&)
		 *	@js NA
		 */
		Object*					objectForKey ( KDintptr nKey );
    
		/** 
		 *	Get the value according to the specified string key.
		 *
		 *  @note Be careful to use this function since it assumes the objects in the dictionary are String pointer.
		 *  @param key  The string key for searching
		 *  @return An instance of String.
		 *          It will return an empty string if the objects aren't String pointer or the key wasn't found.
		 *  @see valueForKey(KDintptr)
		 *  @js NA
		 */
		const String*			valueForKey ( const std::string& sKey );
    
		/** 
		 *	Get the value according to the specified integer key.
		 *
		 *  @note Be careful to use this function since it assumes the objects in the dictionary are String pointer.
		 *  @param key  The string key for searching.
		 *  @return An instance of String.
		 *          It will return an empty string if the objects aren't String pointer or the key wasn't found.
		 *  @see valueForKey(KDintptr)
		 *  @js NA
		 */
		const String*			valueForKey ( KDintptr nKey );

		/**
		 *	Insert an object to dictionary, and match it with the specified string key.
		 *
		 *  @note Whe the first time this method is invoked, the key type will be set to string.
		 *        After that you can't setObject with an integer key.
		 *        If the dictionary contains the key you passed, the object matching the key will be released and removed from dictionary.
		 *        Then the new object will be inserted after that.
		 *
		 *  @param pObject  The Object to be inserted.
		 *  @param key      The string key for searching.
		 *  @see setObject(Object*, KDintptr)
		 *	@js NA
		 */
		KDvoid					setObject ( Object* pObject, const std::string& sKey );
    
		/** 
		 *	Insert an object to dictionary, and match it with the specified string key.
		 *
		 *  @note Then the first time this method is invoked, the key type will be set to string.
		 *        After that you can't setObject with an integer key.
		 *        If the dictionary contains the key you passed, the object matching the key will be released and removed from dictionary.
		 *        Then the new object will be inserted after that.
		 *  @param pObject  The Object to be inserted.
		 *  @param key      The string key for searching.
		 *  @see setObject(Object*, const std::string&)
		 *  @js NA
		 */
		KDvoid					setObject ( Object* pObject, KDintptr nKey );

		/** 
		 *  Remove an object by the specified string key.
		 *
		 *  @param key  The string key for searching.
		 *  @see removeObjectForKey(KDintptr), removeObjectsForKeys(Array*),
		 *       removeObjectForElememt(DictElement*), removeAllObjects().
		 *  @js NA
		 */
		KDvoid					removeObjectForKey ( const std::string& sKey );
    
		/**
		 *  Remove an object by the specified integer key.
		 *
		 *  @param key  The integer key for searching.
		 *  @see removeObjectForKey(const std::string&), removeObjectsForKeys(Array*),
		 *       removeObjectForElememt(DictElement*), removeAllObjects().
		 *  @js NA
		 */
		KDvoid					removeObjectForKey ( KDintptr nKey );
    
		/**
		 *  Remove objects by an array of keys.
		 *
		 *  @param pKeyArray  The array contains keys to be removed.
		 *  @see removeObjectForKey(const std::string&), removeObjectForKey(KDintptr),
		 *       removeObjectForElememt(DictElement*), removeAllObjects().
		 *  @js NA
		 */
		KDvoid					removeObjectsForKeys ( Array* pKeyArray );
    
		/**
		 *  Remove an object by an element.
		 *
		 *  @param pElement  The element need to be removed.
		 *  @see removeObjectForKey(const std::string&), removeObjectForKey(KDintptr),
		 *       removeObjectsForKeys(Array*), removeAllObjects().
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					removeObjectForElememt ( DictElement* pElement );
    
		/**
		 *  Remove all objects in the dictionary.
		 *
		 *  @see removeObjectForKey(const std::string&), removeObjectForKey(KDintptr),
		 *       removeObjectsForKeys(Array*), removeObjectForElememt(DictElement*).
		 *	@js NA
		 */
		KDvoid					removeAllObjects ( KDvoid );
    
		/**
		 *  Return a random object in the dictionary.
		 *
		 *  @return The random object. 
		 *  @see objectForKey(KDintptr), objectForKey(const std::string&)
		 *  @js NA
		 *  @lua NA
		 */
		Object*					randomObject ( KDvoid );
    
		/**
		 *  Create a dictionary.
		 *  @return A dictionary which is an autorelease object.
		 *  @see createWithDictionary(Dictionary*), createWithContentsOfFile(const char*), createWithContentsOfFileThreadSafe(const char*).
		 *  @js NA
		 */
		static Dictionary*		create ( KDvoid );

		/**
		 *  Create a dictionary with an existing dictionary.
		 *
		 *  @param srcDict The exist dictionary.
		 *  @return A dictionary which is an autorelease object.
		 *  @see create(), createWithContentsOfFile(const char*), createWithContentsOfFileThreadSafe(const char*).
		 *  @js NA
		 */
		static Dictionary*		createWithDictionary ( Dictionary* pSrcDict );
    
		/**
		 *  Create a dictionary with a plist file.
		 *  @param  pFileName  The name of the plist file.
		 *  @return A dictionary which is an autorelease object.
		 *  @see create(), createWithDictionary(Dictionary*), createWithContentsOfFileThreadSafe(const char*).
		 *  @js NA
		 */
		static Dictionary*		createWithContentsOfFile ( const KDchar* pFileName );
    
		/**
		 *  Write a dictionary to a plist file.
		 *  @param fullPath The full path of the plist file. You can get writeable path by getWritablePath()
		 *  @return true if successed, false if failed
		 *  @js NA
		 *  @lua NA
		 */
		KDbool					writeToFile ( const KDchar* pFullPath );
     
		/**
		 *  Create a dictionary with a plist file.
		 *  
		 *  @note the return object isn't an autorelease object.
		 *        This can make sure not using autorelease pool in a new thread.
		 *        Therefore, you need to manage the lifecycle of the return object.
		 *        It means that when you don't need it, CC_SAFE_RELEASE needs to be invoked.
		 *
		 *  @param  pFileName  The name of the plist file.
		 *  @return A dictionary which isn't an autorelease object.
		 *  @js NA
		 *  @lua NA
		 */
		static Dictionary*		createWithContentsOfFileThreadSafe ( const KDchar* pFileName );

		/**
		 *	override functions 
		 *  @js NA
		 *  @lua NA
		 */
		virtual KDvoid			acceptVisitor ( DataVisitor& tVisitor );

		/**
		 *  @js NA
		 *  @lua NA
		 */
		virtual Dictionary*		clone ( KDvoid ) const;
    
	private :

		/** 
		 *  For internal usage, invoked by setObject.
		 */
		KDvoid					setObjectUnSafe ( Object* pObject, const std::string& sKey );
		KDvoid					setObjectUnSafe ( Object* pObject, const KDintptr nKey );
    
	public :

		/**
		 *  All the elements in dictionary.
		 * 
		 *  @note For internal usage, we need to declare this member variable as public since it's used in UT_HASH.
		 */
		DictElement*			m_pElements;

	private :
    
		/** The support type of dictionary, it's confirmed when setObject is invoked. */
		enum DictType
		{
			kDictUnknown = 0,
			kDictStr,
			kDictInt
		};
    
		/** 
		 *  The type of dictionary, it's assigned to kDictUnknown by default.
		 */
		DictType				m_eDictType;
};

// end of data_structure group
/// @}

NS_CC_END

#endif	// __CCDictionary_h__
