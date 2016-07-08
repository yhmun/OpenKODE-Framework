/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXXMLParser.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011		¬®¬Ñ¬Ü¬ã¬Ú¬Þ ¬¡¬Ü¬ã¬Ö¬ß¬à¬Ó
 *      Copyright (c) 2009-2010 Ricardo Quesada
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCTMXXMLParser_h__
#define __CCTMXXMLParser_h__

#include "../cocoa/CCDictionary.h"
#include "../support/CCSAXParser.h"

NS_CC_BEGIN
    
class CCTMXObjectGroup;

/**
 * @addtogroup tilemap_parallax_nodes
 * @{
 */

enum
{
	TMXLayerAttribNone   = 1 << 0,
	TMXLayerAttribBase64 = 1 << 1,
	TMXLayerAttribGzip   = 1 << 2,
	TMXLayerAttribZlib   = 1 << 3,
};

enum
{
	TMXPropertyNone,
	TMXPropertyMap,
	TMXPropertyLayer,
	TMXPropertyObjectGroup,
	TMXPropertyObject,
	TMXPropertyTile
};

typedef enum ccTMXTileFlags_
{
    kCCTMXTileHorizontalFlag = 0x80000000,
    kCCTMXTileVerticalFlag	 = 0x40000000,
    kCCTMXTileDiagonalFlag	 = 0x20000000,
    kCCFlipedAll			 =  ( kCCTMXTileHorizontalFlag | kCCTMXTileVerticalFlag | kCCTMXTileDiagonalFlag ),
    kCCFlippedMask			 = ~( kCCFlipedAll )
} ccTMXTileFlags;

// Bits on the far end of the 32-bit global tile ID (GID's) are used for tile flags

/** 
 *	@brief	CCTMXLayerInfo contains the information about the layers like:
 *		- Layer name
 *		- Layer size
 *		- Layer opacity at creation time (it can be modified at runtime)
 *		- Whether the layer is visible (if it's not visible, then the CocosNode won't be created)
 *
 *	This information is obtained from the TMX file.
 */
class CCTMXLayerInfo : public CCObject
{
	public :
		         CCTMXLayerInfo ( KDvoid );
		virtual ~CCTMXLayerInfo ( KDvoid );

	public :

		CC_PROPERTY ( CCDictionary*, m_pProperties, Properties );

	public :

		std::string		m_sName;
		CCSize			m_tLayerSize;
		KDuint*			m_pTiles;
		KDbool			m_bVisible;
		KDubyte			m_cOpacity;
		KDbool			m_bOwnTiles;
		KDuint			m_uMinGID;
		KDuint			m_uMaxGID;
		CCPoint			m_tOffset;
};

/** 
 *	@brief	CCTMXTilesetInfo contains the information about the tilesets like:
 *		- Tileset name
 *		- Tileset spacing
 *		- Tileset margin
 *		- size of the tiles
 *		- Image used for the tiles
 *		- Image size
 *
 *	This information is obtained from the TMX file. 
 */
class CCTMXTilesetInfo : public CCObject
{
	public :
		         CCTMXTilesetInfo ( KDvoid );
		virtual ~CCTMXTilesetInfo ( KDvoid );

	public :

		CCRect			rectForGID ( KDuint uGID );

	public :

		std::string		m_sName;
		KDuint			m_uFirstGid;
		CCSize			m_tTileSize;
		KDuint			m_uSpacing;
		KDuint			m_uMargin;

		//! filename containing the tiles (should be spritesheet / texture atlas)
		std::string		m_sSourceImage;

		//! size in pixels of the image
		CCSize			m_tImageSize;
};

/** 
 *	@brief	CCTMXMapInfo contains the information about the map like:
 *		- Map orientation (hexagonal, isometric or orthogonal)
 *		- Tile size
 *		- Map size
 *
 *	And it also contains:
 *		- Layers (an array of TMXLayerInfo objects)
 *		- Tilesets (an array of TMXTilesetInfo objects)
 *		- ObjectGroups (an array of TMXObjectGroupInfo objects)
 *
 *	This information is obtained from the TMX file.
 *
 */
class CCTMXMapInfo : public CCObject, public CCSAXDelegator
{	
	public :	
		         CCTMXMapInfo ( KDvoid );
		virtual ~CCTMXMapInfo ( KDvoid );

		/** creates a TMX Format with a tmx file */
		static   CCTMXMapInfo*		createWithTMXFile ( const KDchar* szTMXPath );

		/** creates a TMX Format with an XML string and a TMX resource path */
		static   CCTMXMapInfo*		createWithXML ( const KDchar* szTMXString, const KDchar* szResPath );
		
	public :	

		/** initializes a TMX format with a tmx file */
		virtual KDbool			initWithTMXFile ( const KDchar* szTMXPath );

		/** initializes a TMX format with an XML string and a TMX resource path */
		virtual KDbool			initWithXML ( const KDchar* szTMXString, const KDchar* szResPath );

		/** initializes parsing of an XML file, either a tmx (Map) file or tsx (Tileset) file */
		KDbool					parseXMLFile ( const KDchar* szXMLPath );
	
		/* initializes parsing of an XML string, either a tmx (Map) string or tsx (Tileset) string */
		KDbool					parseXMLString ( const KDchar* szXMLString );

		CCDictionary*			getTileProperties ( KDvoid );
		KDvoid					setTileProperties ( CCDictionary* pTileProperties );

		// implement pure virtual methods of CCSAXDelegator
		KDvoid					startElement ( KDvoid* pCtx, const KDchar* szFilePath, const KDchar** pAtts );
		KDvoid					endElement   ( KDvoid* pCtx, const KDchar* szFilePath );
		KDvoid					textHandler  ( KDvoid* pCtx, const KDchar* szText, KDint nLen );

		inline const KDchar*	getCurrentString ( KDvoid ) { return m_sCurrentString.c_str ( ); }
		inline KDvoid			setCurrentString ( const KDchar* szCurrentString ) { m_sCurrentString = szCurrentString; }

		inline const KDchar*	getTMXFileName ( KDvoid ) { return m_sTMXFileName.c_str ( ); }
		inline KDvoid			setTMXFileName ( const KDchar* szTMXPath ) { m_sTMXFileName = szTMXPath; }

		/// map orientation
		CC_SYNTHESIZE ( KDint,	m_nOrientation, Orientation );
	
		/// map width & height
		CC_SYNTHESIZE_PASS_BY_REF ( CCSize, m_tMapSize, MapSize );

		/// tiles width & height
		CC_SYNTHESIZE_PASS_BY_REF ( CCSize, m_tTileSize, TileSize );

		/// Layers
		CC_PROPERTY ( CCArray*, m_pLayers, Layers );

		/// tilesets
		CC_PROPERTY ( CCArray*, m_pTilesets, Tilesets );

		/// ObjectGroups
		CC_PROPERTY ( CCArray*, m_pObjectGroups, ObjectGroups );

		/// parent element
		CC_SYNTHESIZE ( KDint, m_nParentElement, ParentElement );

		/// parent GID
		CC_SYNTHESIZE ( KDuint, m_uParentGID, ParentGID );

		/// layer attribs
		CC_SYNTHESIZE ( KDint, m_nLayerAttribs, LayerAttribs );

		/// is storing characters?
		CC_SYNTHESIZE ( KDbool, m_bStoringCharacters, StoringCharacters );

		/// properties
		CC_PROPERTY ( CCDictionary*, m_pProperties, Properties );

	private :

		KDvoid				internalInit ( const KDchar* szTMXFilename, const KDchar* szResPath );

	protected :

		//! tmx filename
		std::string			m_sTMXFileName;

		// tmx resource path
		std::string			m_sResources;

		//! current string
		std::string			m_sCurrentString;

		//! tile properties
		CCDictionary*		m_pTileProperties;

		KDuint				m_uCurrentFirstGID;
};

// end of tilemap_parallax_nodes group
/// @}

NS_CC_END

#endif // __CCTMXXMLParser_h__



