/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXLayer.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCTMXLayer_h__
#define __CCTMXLayer_h__

#include "CCTMXObjectGroup.h"
#include "CCTMXXMLParser.h"
#include "../base_nodes/CCAtlasNode.h"
#include "../sprite_nodes/CCSpriteBatchNode.h"
#include "../../support/data_support/ccCArray.h"

NS_CC_BEGIN

class TMXMapInfo;
class TMXLayerInfo;
class TMXTilesetInfo;
struct _ccCArray;

/**
 *	@addtogroup tilemap_parallax_nodes
 *	@{
 */

/**
 *	@brief TMXLayer represents the TMX layer.
 *
 *	It is a subclass of SpriteBatchNode. By default the tiles are rendered using a TextureAtlas.
 *	If you modify a tile on runtime, then, that tile will become a Sprite, otherwise no Sprite objects are created.
 *	The benefits of using Sprite objects as tiles are:
 *		- tiles (Sprite) can be rotated/scaled/moved with a nice API
 *
 *	If the layer contains a property named "cc_vertexz" with an integer (in can be positive or negative),
 *	then all the tiles belonging to the layer will use that value as their OpenGL vertex Z for depth.
 *
 *	On the other hand, if the "cc_vertexz" property has the "automatic" value, then the tiles will use an automatic vertex Z value.
 *	Also before drawing the tiles, GL_ALPHA_TEST will be enabled, and disabled after drawing them. The used alpha func will be:
 *
 *	glAlphaFunc( GL_GREATER, value )
 *
 *	"value" by default is 0, but you can change it from Tiled by adding the "cc_alpha_func" property to the layer.
 *	The value 0 should work for most cases, but if you have tiles that are semi-transparent, then you might want to use a different
 *	value, like 0.5.
 *
 *	For further information, please see the programming guide:
 *
 *	http://www.cocos2d-iphone.org/wiki/doku.php/prog_guide:tiled_maps
 *
 *	@since v0.8.1
 *	Tiles can have tile flags for additional properties. At the moment only flip horizontal and flip vertical are used. These bit flags are defined in TMXXMLParser.h.
 *
 *	@since 1.1
 */

class CC_DLL TMXLayer : public SpriteBatchNode
{
	public :

		/** creates a TMXLayer with an tileset info, a layer info and a map info */
		static TMXLayer*		create ( TMXTilesetInfo* pTilesetInfo, TMXLayerInfo* pLayerInfo, TMXMapInfo* pMapInfo );

		/**
		 *	@js ctor
		 */
		TMXLayer ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TMXLayer ( KDvoid );

		/** initializes a TMXLayer with a tileset info, a layer info and a map info */
		KDbool					initWithTilesetInfo ( TMXTilesetInfo* pTilesetInfo, TMXLayerInfo* pLayerInfo, TMXMapInfo* pMapInfo );

		/** 
		 *	dealloc the map that contains the tile position from memory.
		 *	Unless you want to know at runtime the tiles positions, you can safely call this method.
		 *	If you are going to call layer->tileGIDAt() then, don't release the map
		 */
		KDvoid					releaseMap ( KDvoid );

		/** 
		 *	returns the tile (Sprite) at a given a tile coordinate.
		 *	The returned Sprite will be already added to the TMXLayer. Don't add it again.
		 *	The Sprite can be treated like any other Sprite: rotated, scaled, translated, opacity, color, etc.
		 *	You can remove either by calling:
		 *		- layer->removeChild(sprite, cleanup);
		 *		- or layer->removeTileAt(Point(x,y));
		 */
		Sprite*					getTileAt ( const Point& tTileCoordinate );

		/** 
		 *	returns the tile gid at a given tile coordinate. It also returns the tile flags.
		 *	This method requires the the tile map has not been previously released (eg. don't call [layer releaseMap])
		 */
		KDuint					getTileGIDAt ( const Point& tTileCoordinate, ccTMXTileFlags* pFlags = nullptr );

		/** 
		 *	sets the tile gid (gid = tile global id) at a given tile coordinate.
		 *	The Tile GID can be obtained by using the method "tileGIDAt" or by using the TMX editor -> Tileset Mgr +1.
		 *	If a tile is already placed at that position, then it will be removed.
		 */
		KDvoid					setTileGID ( KDuint uGid, const Point& tTileCoordinate );

		/** 
		 *	sets the tile gid (gid = tile global id) at a given tile coordinate.
		 *	The Tile GID can be obtained by using the method "tileGIDAt" or by using the TMX editor -> Tileset Mgr +1.
		 *	If a tile is already placed at that position, then it will be removed.
		 *
		 *	Use withFlags if the tile flags need to be changed as well
		 */
		KDvoid					setTileGID ( KDuint uGid, const Point& tTileCoordinate, ccTMXTileFlags eFlags );

		/** removes a tile at given tile coordinate */
		KDvoid					removeTileAt ( const Point& tTileCoordinate );

		/** returns the position in points of a given tile coordinate */
		Point					getPositionAt ( const Point& tTileCoordinate );

		/** return the value for the specific property name */
		String*					getProperty ( const KDchar* pPropertyName ) const;

		/** Creates the tiles */
		KDvoid					setupTiles ( KDvoid );

		inline const KDchar*	getLayerName ( KDvoid ) { return m_sLayerName.c_str(  ); }
		inline KDvoid			setLayerName ( const KDchar* pLayerName ) { m_sLayerName = pLayerName; }

		/** size of the layer in tiles */
		inline const Size&		getLayerSize ( KDvoid ) const { return m_tLayerSize; }
		inline KDvoid			setLayerSize ( const Size& tSize ) { m_tLayerSize = tSize; }
    
		/** size of the map's tile (could be different from the tile's size) */
		inline const Size&		getMapTileSize ( KDvoid ) const { return m_tMapTileSize; }
		inline KDvoid			setMapTileSize ( const Size& tSize ) { m_tMapTileSize = tSize; }
    
		/** 
		 *	pointer to the map of tiles 
		 *	@js NA
		 *	@lua NA
		 */
		inline KDuint*			getTiles ( KDvoid ) const { return m_pTiles; }
		inline KDvoid			setTiles ( KDuint* pTiles ) { m_pTiles = pTiles; }
    
		/** Tileset information for the layer */
		inline TMXTilesetInfo*	getTileSet ( KDvoid ) const { return m_pTileSet; }
		inline KDvoid			setTileSet ( TMXTilesetInfo* pInfo )
		{
			CC_SAFE_RETAIN ( pInfo );
			CC_SAFE_RELEASE ( m_pTileSet );
			m_pTileSet = pInfo;
		}
    
		/** Layer orientation, which is the same as the map orientation */
		inline KDuint			getLayerOrientation ( KDvoid ) const { return m_uLayerOrientation; }
		inline KDvoid			setLayerOrientation ( KDuint uOrientation ) { m_uLayerOrientation = uOrientation; }
    
		/** properties from the layer. They can be added using Tiled */
		inline Dictionary*		getProperties ( KDvoid ) const { return m_pProperties; }
		inline KDvoid			setProperties ( Dictionary* pProperties )
		{
			CC_SAFE_RETAIN ( pProperties );
			CC_SAFE_RELEASE ( m_pProperties );
			m_pProperties = pProperties;
		}

		//
		// Override
		//
		/** 
		 *	TMXLayer doesn't support adding a Sprite manually.
		 *	@warning addchild(z, tag); is not supported on TMXLayer. Instead of setTileGID.
		 */
		virtual KDvoid			addChild ( Node* pChild, KDint zOrder, KDint nTag ) override;
		
		// super method
		KDvoid					removeChild ( Node* pChild, KDbool bCleanup ) override;

	private :

		Point					getPositionForIsoAt   ( const Point& tPos );
		Point					getPositionForOrthoAt ( const Point& tPos );
		Point					getPositionForHexAt   ( const Point& tPos );

		Point					calculateLayerOffset ( const Point& tOffset );

		/* optimization methods */
		Sprite*					appendTileForGID ( KDuint uGid, const Point& tPos );
		Sprite*					insertTileForGID ( KDuint uGid, const Point& tPos );
		Sprite*					updateTileForGID ( KDuint uGid, const Point& tPos );

		/* The layer recognizes some special properties, like cc_vertez */
		KDvoid					parseInternalProperties ( KDvoid );
		KDvoid					setupTileSprite ( Sprite* pSprite, Point tPos, KDuint uGid );
		Sprite*					reusedTileWithRect ( Rect tRect );
		KDint					getVertexZForPos ( const Point& tPos );

		// index
		KDuint					atlasIndexForExistantZ ( KDuint z );
		KDuint					atlasIndexForNewZ ( KDint z );
    
	protected :

		//! name of the layer
		std::string				m_sLayerName;
		//! TMX Layer supports opacity
		KDubyte					m_cOpacity;

		KDuint			        m_uMinGID;
		KDuint			        m_uMaxGID;

		//! Only used when vertexZ is used
		KDint					m_nVertexZvalue;
		KDbool					m_bUseAutomaticVertexZ;

		//! used for optimization
		Sprite*					m_pReusedTile;
		ccCArray*				m_pAtlasIndexArray;
    
		// used for retina display
		KDfloat					m_fContentScaleFactor;
    
		/** size of the layer in tiles */
		Size					m_tLayerSize;
		/** size of the map's tile (could be different from the tile's size) */
		Size					m_tMapTileSize;
		/** pointer to the map of tiles */
		KDuint*					m_pTiles;
		/** Tileset information for the layer */
		TMXTilesetInfo*			m_pTileSet;
		/** Layer orientation, which is the same as the map orientation */
		KDuint					m_uLayerOrientation;
		/** properties from the layer. They can be added using Tiled */
		Dictionary*				m_pProperties;
};

//	end of tilemap_parallax_nodes group
/// @}

NS_CC_END

#endif	// __CCTMXLayer_h__

