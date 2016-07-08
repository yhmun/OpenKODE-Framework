/* --------------------------------------------------------------------------
 *
 *      File            M3GLoader.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010 UEDA.Takashi
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 *
 * --------------------------------------------------------------------------
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __M3GLoader_h__
#define __M3GLoader_h__

#include <XMM3G/M3GObject3D.h>
#include <string>

class M3GTransformable;
class M3GGroup;
class M3GMesh;

/**
 * @~English	Download and deserializes scene graph nodes and node components, as well as entire scene graphs.
 */
class M3GLoader 
{
	protected :

		M3GLoader ( KDFile* pFile );

		~M3GLoader ( KDvoid );

	public :

		/**
		 * @brief		Deserializes Object3D instances from the given byte array, starting at the given offset.
		 * @param[in]	pFile
		 * @param[in]	nOffset
		 * @return
		 */
		static M3GObject3DArray  load ( KDFile* pFile, KDoff nOffset );

		/**
		 * @brief		Deserializes Object3D instances from the given byte array, starting at the given offset.
		 * @param[in]	uSize
		 * @param[in]	pData
		 * @param[in]	nOffset
		 * @return
		 */
		static M3GObject3DArray  load ( KDsize uSize, const KDbyte* pData, KDoff nOffset );

		/**
		 * @brief		Deserializes Object3D instances from the named resource.
		 * @param[in]	szPath
		 * @return
		 */
		static M3GObject3DArray  load ( const KDchar* szPath );

	private :

        KDbool		loadM3G ( KDvoid );
        KDbool		loadIMG ( KDvoid );

		KDoff       getOffset ( KDvoid );

		KDbool		readSection ( KDvoid );
		KDbool		readObject  ( KDvoid );

		M3GObject3D*  readHeader				( KDvoid );
		M3GObject3D*  readAnimationController	( KDvoid );
		M3GObject3D*  readAnimationTrack		( KDvoid );
		M3GObject3D*  readAppearance			( KDvoid );
		M3GObject3D*  readBackground			( KDvoid );
		M3GObject3D*  readCamera				( KDvoid );
		M3GObject3D*  readCompositingMode		( KDvoid );
		M3GObject3D*  readFog					( KDvoid );
		M3GObject3D*  readPolygonMode			( KDvoid );
		M3GObject3D*  readGroup					( KDvoid );
		M3GObject3D*  readImage2D				( KDvoid );
		M3GObject3D*  readTriangleStripArray	( KDvoid );
		M3GObject3D*  readLight					( KDvoid );
		M3GObject3D*  readMaterial				( KDvoid );
		M3GObject3D*  readMesh					( KDvoid );
		M3GObject3D*  readMorphingMesh			( KDvoid );
		M3GObject3D*  readSkinnedMesh			( KDvoid );
		M3GObject3D*  readTexture2D				( KDvoid );
		M3GObject3D*  readSprite3D				( KDvoid );
		M3GObject3D*  readKeyframeSequence		( KDvoid );
		M3GObject3D*  readVertexArray			( KDvoid );
		M3GObject3D*  readVertexBuffer			( KDvoid );
		M3GObject3D*  readWorld					( KDvoid );
		M3GObject3D*  readExternalReference		( KDvoid );
		
		KDbool		readSubObject3D		 ( M3GObject3D*		 pObject );
		KDbool		readSubTransformable ( M3GTransformable* pObject );
		KDbool		readSubNode			 ( M3GNode*			 pObject );
		KDbool		readSubGroup		 ( M3GGroup*		 pObject );
		KDbool		readSubMesh			 ( M3GMesh*			 pObject );

		KDbool		readBool	( KDvoid );
		KDubyte		readUbyte   ( KDvoid );
		KDushort	readUshort  ( KDvoid );
		KDuint		readUint	( KDvoid );
		KDbyte		readByte	( KDvoid );
		KDshort		readShort	( KDvoid );
		KDint		readInt 	( KDvoid );
		KDfloat		readFloat	( KDvoid );
		KDuint		readRGB		( KDvoid );
		KDuint		readRGBA	( KDvoid );
		std::string	readString  ( KDvoid );

		KDvoid		readUbytes  ( KDubyte*	pValues, KDsize uSize );
		KDvoid		readBytes	( KDbyte*	pValues, KDsize uSize );
		KDvoid		readUshorts	( KDushort*	pValues, KDsize uSize );
		KDvoid		readFloats	( KDfloat*	pValues, KDsize uSize );
		
	private :

		M3GObject3DArray		m_aObjects;
		KDFile*					m_pFile;
};

#endif
