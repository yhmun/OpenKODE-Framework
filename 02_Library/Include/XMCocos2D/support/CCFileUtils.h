/* -----------------------------------------------------------------------------------
 *
 *      File            CCFileUtils.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCFileUtils_h__
#define __CCFileUtils_h__

#include "../ccTypes.h"
#include "../ccTypeInfo.h"

#include <map>
#include <vector>
#include <string>

NS_CC_BEGIN

class CCDictionary;
class CCArray;

/**
 * @addtogroup support
 * @{
 */

//! @brief  Helper class to handle file operations
class CCFileUtils : public TypeInfo
{
	friend class CCArray;
    friend class CCDictionary;

	protected :

		CCFileUtils ( KDvoid );

		/**
		 *  The destructor of CCFileUtils.
		 */
		virtual ~CCFileUtils ( KDvoid );

	public :
		
		/**
		 *  Gets the instance of CCFileUtils.
		 */
		static  CCFileUtils*		sharedFileUtils ( KDvoid );
		/**
		 *  Destroys the instance of CCFileUtils.
		 */
		static  KDvoid				purgeFileUtils ( KDvoid );

		/**
		 *  Purges the file searching cache.
		 *
		 *  @note It should be invoked after the resources were updated.
		 *        For instance, in the CocosPlayer sample, every time you run application from CocosBuilder,
		 *        All the resources will be downloaded to the writable folder, before new js app launchs,
		 *        this method should be invoked to clean the file search cache.
		 */
		KDvoid						purgeCachedEntries ( KDvoid );

	public :

		/**
		 *  Returns an unique ID for this class.
		 *  @note It's only used for JSBindings now.
		 *  @return The unique ID for this class.
		 */
		virtual KDint32				getClassTypeInfo ( KDvoid );
		/**
		 *	@brief		Get resource file data
		 *	@param[in]  szFileName The resource file name which contains the path.
		 *	@param[in]  szMode The read mode of the file.
		 *	@param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
		 *	@return		Upon success, a pointer to the data is returned, otherwise NULL.
		 *	@warning	Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
		 */
		KDubyte*					getFileData ( const KDchar* szFilePath, const KDchar* pszMode, KDsize* pSize );

		/**
		 *	@brief		Get resource file data from a zip file.
		 *	@param[in]  szFilePath The resource file name which contains the relative path of the zip file.
		 *	@param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
		 *	@return		Upon success, a pointer to the data is returned, otherwise NULL.
		 *	@warning	Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
		 */
        KDubyte*					getFileDataFromZip ( const KDchar* szZipPath, const KDchar* szFilePath, KDsize* pSize );

		/**
		 *	Returns the fullpath for a given filename.
	     *
		 *	First it will try to get a new filename from the "filenameLookup" dictionary.
		 *	If a new filename can't be found on the dictionary, it will use the original filename.
		 *	Then it will try to obtain the full path of the filename using the CCFileUtils search rules: resolutions, and search paths.
		 *	The file search is based on the array element order of search paths and resolution directories.
	     *
		 *	For instance:
		 *
     	 *	We set two elements("/mnt/sdcard/", "internal_dir/") to search paths vector by setSearchPaths,
     	 *	and set three elements("resources-ipadhd/", "resources-ipad/", "resources-iphonehd")
     	 *	to resolutions vector by setSearchResolutionsOrder. The "internal_dir" is relative to "Resources/".
		 *
		 *	If we have a file named 'sprite.png', the mapping in fileLookup dictionary contains `key: sprite.png -> value: sprite.pvr.gz`.
     	 *	Firstly, it will replace 'sprite.png' with 'sprite.pvr.gz', then searching the file sprite.pvr.gz as follows:
		 *
     	 *		/mnt/sdcard/resources-ipadhd/sprite.pvr.gz      (if not found, search next)
     	 *		/mnt/sdcard/resources-ipad/sprite.pvr.gz        (if not found, search next)
     	 *		/mnt/sdcard/resources-iphonehd/sprite.pvr.gz    (if not found, search next)
     	 *		/mnt/sdcard/sprite.pvr.gz                       (if not found, search next)
     	 *		internal_dir/resources-ipadhd/sprite.pvr.gz     (if not found, search next)
     	 *		internal_dir/resources-ipad/sprite.pvr.gz       (if not found, search next)
     	 *		internal_dir/resources-iphonehd/sprite.pvr.gz   (if not found, search next)
     	 *		internal_dir/sprite.pvr.gz                      (if not found, return "sprite.png")
		 *
		 *	If the filename contains relative path like "gamescene/uilayer/sprite.png",
		 *	and the mapping in fileLookup dictionary contains `key: gamescene/uilayer/sprite.png -> value: gamescene/uilayer/sprite.pvr.gz`.
		 *	The file search order will be:
		 *
     	 *		/mnt/sdcard/gamescene/uilayer/resources-ipadhd/sprite.pvr.gz      (if not found, search next)
     	 *		/mnt/sdcard/gamescene/uilayer/resources-ipad/sprite.pvr.gz        (if not found, search next)
     	 *		/mnt/sdcard/gamescene/uilayer/resources-iphonehd/sprite.pvr.gz    (if not found, search next)
     	 *		/mnt/sdcard/gamescene/uilayer/sprite.pvr.gz                       (if not found, search next)
     	 *		internal_dir/gamescene/uilayer/resources-ipadhd/sprite.pvr.gz     (if not found, search next)
     	 *		internal_dir/gamescene/uilayer/resources-ipad/sprite.pvr.gz       (if not found, search next)
     	 *		internal_dir/gamescene/uilayer/resources-iphonehd/sprite.pvr.gz   (if not found, search next)
     	 *		internal_dir/gamescene/uilayer/sprite.pvr.gz                      (if not found, return "gamescene/uilayer/sprite.png")
		 *
		 *	If the new file can't be found on the file system, it will return the parameter pszFileName directly.
	     *
		 *	This method was added to simplify multiplatform support. Whether you are using cocos2d-js or any cross-compilation toolchain like StellaSDK or Apportable,
		 *	you might need to load different resources for a given file in the different platforms.
		 *
		 *	@since v2.1
		 */
		virtual std::string			fullPathForFilename ( const KDchar* szFileName );
		
		/**
		 *	Loads the filenameLookup dictionary from the contents of a filename.
		 * 
		 *	@note The plist file name should follow the format below:
		 * 
		 *	@code
		 *	<?xml version="1.0" encoding="UTF-8"?>
		 *	<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
		 *	<plist version="1.0">
		 *	<dict>
		 *     <key>filenames</key>
		 *     <dict>
		 *         <key>sounds/click.wav</key>
		 *         <string>sounds/click.caf</string>
		 *         <key>sounds/endgame.wav</key>
		 *         <string>sounds/endgame.caf</string>
		 *         <key>sounds/gem-0.wav</key>
		 *         <string>sounds/gem-0.caf</string>
		 *     </dict>
		 *     <key>metadata</key>
		 *     <dict>
		 *         <key>version</key>
		 *         <integer>1</integer>
		 *     </dict>
		 *	</dict>
		 *	</plist>
		 *	@endcode
		 *	@param filename The plist file name.
		 *
		 *	@since v2.1
		 */
		virtual KDvoid				loadFilenameLookupDictionaryFromFile ( const KDchar* szFilename );
	    
		/** 
		 *  Sets the filenameLookup dictionary.
		 *
		 *  @param pFilenameLookupDict The dictionary for replacing filename.
		 *  @since v2.1
		 */
		virtual KDvoid				setFilenameLookupDictionary ( CCDictionary* pFilenameLookupDict );

		/**
		 *  Gets full path from a file name and the path of the reletive file.
		 *  @param pszFilename The file name.
		 *  @param pszRelativeFile The path of the relative file.
		 *  @return The full path.
		 *          e.g. pszFilename: hello.png, pszRelativeFile: /User/path1/path2/hello.plist
		 *               Return: /User/path1/path2/hello.pvr (If there a a key(hello.png)-value(hello.pvr) in FilenameLookup dictionary. )
		 *
		 */
		const KDchar*				fullPathFromRelativeFile ( const KDchar* szFileName, const KDchar* szRelativeFile );
    
		const KDchar*				fullPathFromWritablePath ( const KDchar* szFileName );

		/** 
		 *  Sets the array that contains the search order of the resources.
		 *
		 *  @param searchResolutionsOrder The source array that contains the search order of the resources.
		 *  @see getSearchResolutionsOrder(void), fullPathForFilename(const char*).
		 *  @since v2.1
		 */
		virtual KDvoid				setSearchResolutionsOrder ( const std::vector<std::string>& aSearchResolutionsOrder );

		/**
		  * Append search order of the resources.
		  *
		  * @see setSearchResolutionsOrder(), fullPathForFilename().
		  * @since v2.1
		  */
		virtual KDvoid				addSearchResolutionsOrder ( const KDchar* szOrder );
	    
		/**
		 *  Gets the array that contains the search order of the resources.
		 *
		 *  @see setSearchResolutionsOrder(const std::vector<std::string>&), fullPathForFilename(const char*).
		 *  @since v2.1
		 */
		virtual const std::vector<std::string>&		getSearchResolutionsOrder ( KDvoid );
	    
		/** 
		 *  Sets the array of search paths.
		 * 
		 *  You can use this array to modify the search path of the resources.
		 *  If you want to use "themes" or search resources in the "cache", you can do it easily by adding new entries in this array.
		 *
		 *  @note This method could access relative path and absolute path.
		 *        If the relative path was passed to the vector, CCFileUtils will add the default resource directory before the relative path.
		 *        For instance:
		 *        	On Android, the default resource root path is "assets/".
		 *        	If "/mnt/sdcard/" and "resources-large" were set to the search paths vector,
		 *        	"resources-large" will be converted to "assets/resources-large" since it was a relative path.
		 *
		 *  @param searchPaths The array contains search paths.
		 *  @see fullPathForFilename(const char*)
		 *  @since v2.1
		 */
		virtual KDvoid				setSearchPaths ( const std::vector<std::string>& aRearchPaths );

		/**
		 * Add search path.
		 *
		 * @since v2.1
		 */
		KDvoid						addSearchPath ( const KDchar* szPath );

		/**
		  * Removes a path from search paths.
		  *
		  * @since v2.2
		  * @lua NA
		  */
		virtual KDvoid				removeSearchPath ( const KDchar* szPath );

		/**
		  * Removes all paths.
		  *
		  * @since v2.2
		  * @lua NA
		  */
		KDvoid						removeAllPaths ( KDvoid );

		/**
		 *  Gets the array of search paths.
		 *  
		 *  @return The array of search paths.
		 *  @see fullPathForFilename(const char*).
		 */
		virtual const std::vector<std::string>&		getSearchPaths ( KDvoid );

		/**
		 *  Gets the writable path.
		 *  @return  The path that can be write/read a file in
		 */
		virtual std::string			getWritablePath ( KDvoid );

		/**
		 *  Checks whether a file exists.
		 *
		 *  @note If a relative path was passed in, it will be inserted a default root path at the beginning.
		 *  @param strFilePath The path of the file, it could be a relative or absolute path.
		 *  @return true if the file exists, otherwise it will return false.
		 */
		virtual KDbool				isFileExist ( const std::string& sFilePath );

		/**
		 *  Checks whether the path is an absolute path.
		 *
		 *  @note On Android, if the parameter passed in is relative to "assets/", this method will treat it as an absolute path.
		 *        Also on Blackberry, path starts with "app/native/Resources/" is treated as an absolute path.
		 *
		 *  @param strPath The path that needs to be checked.
		 *  @return true if it's an absolute path, otherwise it will return false.
		 */
		virtual KDbool				isAbsolutePath ( const std::string& sPath );

		/**
		 *	@brief	Set/Get whether pop-up a message box when the image load failed
		 */
		KDvoid						setPopupNotify ( KDbool bNotify );
		KDbool						 isPopupNotify ( KDvoid );

	protected :

		/**
		 *  Initializes the instance of CCFileUtils. It will set m_searchPathArray and m_searchResolutionsOrderArray to default values.
		 *
		 *  @note When you are porting Cocos2d-x to a new platform, you may need to take care of this method.
		 *        You could assign a default value to m_strDefaultResRootPath in the subclass of CCFileUtils(e.g. CCFileUtilsAndroid). Then invoke the CCFileUtils::init().
		 *  @return true if successed, otherwise it returns false.
		 *
		 */
		virtual KDbool				init ( KDvoid );

		/**
		 *  Gets the new filename from the filename lookup dictionary.
		 *  @param pszFileName The original filename.
		 *  @return The new filename after searching in the filename lookup dictionary.
		 *          If the original filename wasn't in the dictionary, it will return the original filename.
		 */
		virtual std::string			getNewFilename ( const KDchar* szFileName );
	    
		/**
		 *  Gets full path for filename, resolution directory and search path.
		 *
		 *  @param filename The file name.
		 *  @param resolutionDirectory The resolution directory.
		 *  @param searchPath The search path.
		 *  @return The full path of the file. It will return an empty string if the full path of the file doesn't exist.
		 */
		virtual std::string			getPathForFilename ( const std::string& sFilename, const std::string& sResolutionDirectory, const std::string& sSearchPath );
	    
		/**
		 *  Gets full path for the directory and the filename.
		 *
		 *  @note Only iOS and Mac need to override this method since they are using
		 *        `[[NSBundle mainBundle] pathForResource: ofType: inDirectory:]` to make a full path.
		 *        Other platforms will use the default implementation of this method.
		 *  @param strDirectory The directory contains the file we are looking for.
		 *  @param strFilename  The name of the file.
		 *  @return The full path of the file, if the file can't be found, it will return an empty string.
		 */
		virtual std::string			getFullPathForDirectoryAndFilename ( const std::string& sDirectory, const std::string& sFilename );
	    
		/**
		 *  Creates a dictionary by the contents of a file.
		 *  @note This method is used internally.
		 */
		virtual CCDictionary*		createCCDictionaryWithContentsOfFile ( const std::string& sFilename );
	    
		/**
		 *  Write a dictionary to a plist file.
		 *  @note This method is used internally.
		 */
		virtual KDbool				writeToFile ( CCDictionary* pDict, const std::string& sFullPath );

		/**
		 *  Creates an array by the contents of a file.
		 *  @note This method is used internally.
		 */
		virtual CCArray*			createCCArrayWithContentsOfFile ( const std::string& sFilename );

	protected :

		/** 
		 *	Dictionary used to lookup filenames based on a key.
		 *  It is used internally by the following methods:
		 *
		 *  std::string fullPathForFilename(const char*);
		 *
		 *  @since v2.1
		 */
		CCDictionary*						m_pFilenameLookupDict;
	    
		/** 
		 *  The vector contains resolution folders.
		 *  The lower index of the element in this vector, the higher priority for this resolution directory.
		 */
		std::vector<std::string>			m_aSearchResolutionsOrderArray;
	    
		/**
		 * The vector contains search paths.
		 * The lower index of the element in this vector, the higher priority for this search path.
		 */
		std::vector<std::string>			m_aSearchPathArray;
	    
		/**
		 *  The default root path of resources.
		 *  If the default root path of resources needs to be changed, do it in the `init` method of CCFileUtils's subclass.
		 *  For instance:
		 *  On Android, the default root path of resources will be assigned with "assets/" in CCFileUtilsAndroid::init().
		 *  Similarly on Blackberry, we assign "app/native/Resources/" to this variable in CCFileUtilsBlackberry::init().
		 */
		std::string							m_sDefaultResRootPath;
	    
		/**
		 *  The full path cache. When a file is found, it will be added into this cache. 
		 *  This variable is used for improving the performance of file search.
		 */
		std::map<std::string, std::string>	m_aFullPathCache;
};

// end of support group
/// @}

NS_CC_END

#endif // __CCFileUtils_h__