/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXXMLParser.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCTMXXMLParser_h__
#define __CCTMXXMLParser_h__

#include "../../base/CCArray.h"
#include "../../base/CCDictionary.h"
#include "../../base/CCGeometry.h"
#include "../../platform/CCSAXParser.h"

NS_CC_BEGIN

class TMXObjectGroup;

/** 
 *	@file
 *	Internal TMX parser
 *
 *	IMPORTANT: These classed should not be documented using doxygen strings
 *	since the user should not use them.
 *
 */

/**
 *	@addtogroup tilemap_parallax_nodes
 *	@{
 */

enum 
{
    TMXLayerAttribNone		= 1 << 0,
    TMXLayerAttribBase64	= 1 << 1,
    TMXLayerAttribGzip		= 1 << 2,
    TMXLayerAttribZlib		= 1 << 3,
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
    kTMXTileHorizontalFlag	= 0x80000000,
    kTMXTileVerticalFlag	= 0x40000000,
    kTMXTileDiagonalFlag	= 0x20000000,
    kFlipedAll				= ( kTMXTileHorizontalFlag | kTMXTileVerticalFlag | kTMXTileDiagonalFlag ),
    kFlippedMask			= ~( kFlipedAll )
} ccTMXTileFlags;

// Bits on the far end of the 32-bit global tile ID (GID's) are used for tile flags

/** 
 *	@brief TMXLayerInfo contains the information about the layers like:
 *		- Layer name
 *		- Layer size
 *		- Layer opacity at creation time (it can be modified at runtime)
 *		- Whether the layer is visible (if it's not visible, then the CocosNode won't be created)
 *
 *	This information is obtained from the TMX file.
 */
class CC_DLL TMXLayerInfo : public Object
{
	public :

		/**
		 *	@js ctor
		 */
		TMXLayerInfo ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TMXLayerInfo ( KDvoid );

		KDvoid					setProperties ( Dictionary* pProperties );
		Dictionary*				getProperties ( KDvoid );

		Dictionary*				m_pProperties;
		std::string				m_sName;
		Size					m_tLayerSize;
		KDuint*					m_pTiles;
		KDbool					m_bVisible;
		KDubyte					m_cOpacity;
		KDbool					m_bOwnTiles;
		KDuint					m_uMinGID;
		KDuint					m_uMaxGID;
		Point					m_tOffset;
};

/**
 *	@brief TMXTilesetInfo contains the information about the tilesets like:
 *		- Tileset name
 *		- Tileset spacing
 *		- Tileset margin
 *		- size of the tiles
 *		- Image used for the tiles
 *		- Image size
 *
 *	This information is obtained from the TMX file. 
 */
class CC_DLL TMXTilesetInfo : public Object
{
	public :

		std::string				m_sName;
		KDuint					m_uFirstGid;
		Size					m_tTileSize;
		KDuint					m_uSpacing;
		KDuint					m_uMargin;
		//! filename containing the tiles (should be spritesheet / texture atlas)
		std::string				m_sSourceImage;
		//! size in pixels of the image
		Size					m_tImageSize;

	public :

		/**
		 *	@js ctor
		 */
		TMXTilesetInfo ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TMXTilesetInfo ( KDvoid );

		Rect					rectForGID ( KDuint uGid );
};

/** 
 *	@brief TMXMapInfo contains the information about the map like:
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
class CC_DLL TMXMapInfo : public Object, public SAXDelegator
{    
	public :

		/** creates a TMX Format with a tmx file */
		static TMXMapInfo*			create ( const std::string& sTmxFile );
		
		/** creates a TMX Format with an XML string and a TMX resource path */
		static TMXMapInfo*			createWithXML ( const std::string& sTmxString, const std::string& sResourcePath );
    
		/**
		 *	@js ctor
		 */
		TMXMapInfo ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TMXMapInfo ( KDvoid );
    
		/** initializes a TMX format with a  tmx file */
		KDbool						initWithTMXFile ( const std::string& sTmxFile );
		
		/** initializes a TMX format with an XML string and a TMX resource path */
		KDbool						initWithXML  ( const std::string& sTmxString, const std::string& sResourcePath );
		
		/** initializes parsing of an XML file, either a tmx (Map) file or tsx (Tileset) file */
		KDbool						parseXMLFile ( const std::string& sXmlFilename );
		
		/* initializes parsing of an XML string, either a tmx (Map) string or tsx (Tileset) string */
		KDbool						parseXMLString ( const std::string& sXmlString );

		Dictionary*					getTileProperties ( KDvoid ) { return m_pTileProperties; }
		KDvoid						setTileProperties ( Dictionary* pTileProperties )
		{
			CC_SAFE_RETAIN ( pTileProperties );
			CC_SAFE_RELEASE ( m_pTileProperties );
			m_pTileProperties = pTileProperties;
		}

		/// map orientation
		inline KDint				getOrientation() const { return m_nOrientation; };
		inline KDvoid				setOrientation(int orientation) { m_nOrientation = orientation; };

		/// map width & height
		inline const Size&			getMapSize() const { return m_tMapSize; };
		inline KDvoid				setMapSize(const Size& mapSize) { m_tMapSize = mapSize; };

		/// tiles width & height
		inline const Size&			getTileSize() const { return m_tTileSize; };
		inline KDvoid				setTileSize(const Size& tileSize) { m_tTileSize = tileSize; };
    
		/// Layers
		inline Array*				getLayers ( KDvoid ) const { return m_pLayers; };
		inline KDvoid				setLayers ( Array* pLayers )
		{
			CC_SAFE_RETAIN ( pLayers );
			CC_SAFE_RELEASE ( m_pLayers );
			m_pLayers = pLayers;
		}

		/// tilesets
		inline Array*				getTilesets ( KDvoid ) const { return m_pTilesets; }
		inline KDvoid				setTilesets ( Array* pTilesets )
		{
			CC_SAFE_RETAIN ( pTilesets );
			CC_SAFE_RELEASE ( m_pTilesets );
			m_pTilesets = pTilesets;
		}

		/// ObjectGroups
		inline Array*				getObjectGroups ( KDvoid ) const { return m_pObjectGroups; }
		inline KDvoid				setObjectGroups ( Array* pGroups )
		{
			CC_SAFE_RETAIN ( pGroups );
			CC_SAFE_RELEASE ( m_pObjectGroups );
			m_pObjectGroups = pGroups;
		}

		/// parent element
		inline KDint				getParentElement ( KDvoid ) const { return m_nParentElement; }
		inline KDvoid				setParentElement ( KDint nElement ) { m_nParentElement = nElement; }

		/// parent GID
		inline KDuint				getParentGID ( KDvoid ) const { return m_uParentGID; }
		inline KDvoid				setParentGID ( KDuint uGid ) { m_uParentGID = uGid; }

		/// layer attribs
		inline KDint				getLayerAttribs ( KDvoid ) const { return m_nLayerAttribs; }
		inline KDvoid				setLayerAttribs ( KDint nLayerAttribs ) { m_nLayerAttribs = nLayerAttribs; }

		/// is storing characters?
		inline KDbool				 isStoringCharacters ( KDvoid ) const { return m_bStoringCharacters; }
		inline KDvoid				setStoringCharacters ( KDbool bStoringCharacters ) { m_bStoringCharacters = bStoringCharacters; }

		/// properties
		inline Dictionary*			getProperties ( KDvoid ) const { return m_pProperties; };
		inline KDvoid				setProperties ( Dictionary* pProperties )
		{
			CC_SAFE_RETAIN ( pProperties );
			CC_SAFE_RELEASE ( m_pProperties );
			m_pProperties = pProperties;
		}
    
		// implement pure virtual methods of SAXDelegator
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						startElement ( KDvoid* pCtx, const KDchar* pName, const KDchar** pAtts );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						endElement ( KDvoid* pCtx, const KDchar* pName );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						textHandler ( KDvoid* pCtx, const KDchar* pCh, KDint nLen );

		inline const std::string&	getCurrentString ( KDvoid ) const { return m_sCurrentString; }
		inline KDvoid				setCurrentString ( const std::string& sCurrentString ) { m_sCurrentString = sCurrentString; }
		inline const std::string&	getTMXFileName ( KDvoid ) const { return m_sTMXFileName; }
		inline KDvoid				setTMXFileName ( const std::string& fileName){ m_sTMXFileName = fileName; }

	protected :

		KDvoid						internalInit ( const std::string& sTmxFileName, const std::string& sResourcePath );

		/// map orientation
		KDint						m_nOrientation;
		/// map width & height
		Size						m_tMapSize;
		/// tiles width & height
		Size						m_tTileSize;
		/// Layers
		Array*						m_pLayers;
		/// tilesets
		Array*						m_pTilesets;
		/// ObjectGroups
		Array*						m_pObjectGroups;
		/// parent element
		KDint						m_nParentElement;
		/// parent GID
		KDuint						m_uParentGID;
		/// layer attribs
		KDint						m_nLayerAttribs;
		/// is storing characters?
		KDbool						m_bStoringCharacters;
		/// properties
		Dictionary*					m_pProperties;
    
		//! tmx filename
		std::string					m_sTMXFileName;
		// tmx resource path
		std::string					m_sResources;
		//! current string
		std::string					m_sCurrentString;
		//! tile properties
		Dictionary*					m_pTileProperties;
		KDuint						m_uCurrentFirstGID;
};

// end of tilemap_parallax_nodes group
/// @}

NS_CC_END

#endif

