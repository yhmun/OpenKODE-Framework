/*
 *	File			xmg_pod_impl.h
 *	Description		XMGraphic : POD impl
 *
 *	Author			Y.H Mun
 *	Contact			chris@xmsoft.co.kr				
 *	Copyright		Copyright (C) 2010 XMSoft. All rights reserved.
 */

#ifndef __xmg_pod_impl_h__
#define __xmg_pod_impl_h__

//*****************************************************************************
//	Defines
//*****************************************************************************

#define VERTTYPE						GLfloat
#define VERTTYPEENUM					GL_FLOAT

#define PVRTMODELPOD_VERSION			("AB.POD.2.0")		// POD file version string

#define PVRTMODELPOD_TAG_MASK			(0x80000000)
#define PVRTMODELPOD_TAG_START			(0x00000000)
#define PVRTMODELPOD_TAG_END			(0x80000000)

#define CFAH							(1024)

//*****************************************************************************
//	Enumerations
//*****************************************************************************

enum EPVRTDataType
{
	EPODDataNone,
	EPODDataFloat,
	EPODDataInt,
	EPODDataUnsignedShort,
	EPODDataRGBA,
	EPODDataARGB,
	EPODDataD3DCOLOR,
	EPODDataUBYTE4,
	EPODDataDEC3N,
	EPODDataFixed16_16,
	EPODDataUnsignedByte,
	EPODDataShort,
	EPODDataShortNorm,
	EPODDataByte,
	EPODDataByteNorm
};

//*****************************************************************************
//	Struct      EPODFileName
//	Brief       Enum for the binary pod blocks
//*****************************************************************************
enum EPODFileName
{
	ePODFileVersion						= 1000,
	ePODFileScene,
	ePODFileExpOpt,
	ePODFileHistory,
	ePODFileEndiannessMisMatch			= -402456576,

	ePODFileColourBackground			= 2000,
	ePODFileColourAmbient,
	ePODFileNumCamera,
	ePODFileNumLight,
	ePODFileNumMesh,
	ePODFileNumNode,
	ePODFileNumMeshNode,
	ePODFileNumTexture,
	ePODFileNumMaterial,
	ePODFileNumFrame,
	ePODFileCamera,						// Will come multiple times
	ePODFileLight,						// Will come multiple times
	ePODFileMesh,						// Will come multiple times
	ePODFileNode,						// Will come multiple times
	ePODFileTexture,					// Will come multiple times
	ePODFileMaterial,					// Will come multiple times
	ePODFileFlags,

	ePODFileMatName						= 3000,
	ePODFileMatIdxTexDiffuse,
	ePODFileMatOpacity,
	ePODFileMatAmbient,
	ePODFileMatDiffuse,
	ePODFileMatSpecular,
	ePODFileMatShininess,
	ePODFileMatEffectFile,
	ePODFileMatEffectName,
	ePODFileMatIdxTexAmbient,
	ePODFileMatIdxTexSpecularColour,
	ePODFileMatIdxTexSpecularLevel,
	ePODFileMatIdxTexBump,
	ePODFileMatIdxTexEmissive,
	ePODFileMatIdxTexGlossiness,
	ePODFileMatIdxTexOpacity,
	ePODFileMatIdxTexReflection,
	ePODFileMatIdxTexRefraction,
	ePODFileMatBlendSrcRGB,
	ePODFileMatBlendSrcA,
	ePODFileMatBlendDstRGB,
	ePODFileMatBlendDstA,
	ePODFileMatBlendOpRGB,
	ePODFileMatBlendOpA,
	ePODFileMatBlendColour,
	ePODFileMatBlendFactor,
	ePODFileMatFlags,

	ePODFileTexName						= 4000,

	ePODFileNodeIdx						= 5000,
	ePODFileNodeName,
	ePODFileNodeIdxMat,
	ePODFileNodeIdxParent,
	ePODFileNodePos,
	ePODFileNodeRot,
	ePODFileNodeScale,
	ePODFileNodeAnimPos,
	ePODFileNodeAnimRot,
	ePODFileNodeAnimScale,
	ePODFileNodeMatrix,
	ePODFileNodeAnimMatrix,
	ePODFileNodeAnimFlags,
	ePODFileNodeAnimPosIdx,
	ePODFileNodeAnimRotIdx,
	ePODFileNodeAnimScaleIdx,
	ePODFileNodeAnimMatrixIdx,

	ePODFileMeshNumVtx					= 6000,
	ePODFileMeshNumFaces,
	ePODFileMeshNumUVW,
	ePODFileMeshFaces,
	ePODFileMeshStripLength,
	ePODFileMeshNumStrips,
	ePODFileMeshVtx,
	ePODFileMeshNor,
	ePODFileMeshTan,
	ePODFileMeshBin,
	ePODFileMeshUVW,					// Will come multiple times
	ePODFileMeshVtxCol,
	ePODFileMeshBoneIdx,
	ePODFileMeshBoneWeight,
	ePODFileMeshInterleaved,
	ePODFileMeshBoneBatches,
	ePODFileMeshBoneBatchBoneCnts,
	ePODFileMeshBoneBatchOffsets,
	ePODFileMeshBoneBatchBoneMax,
	ePODFileMeshBoneBatchCnt,
	ePODFileMeshUnpackMatrix,

	ePODFileLightIdxTgt					= 7000,
	ePODFileLightColour,
	ePODFileLightType,
	ePODFileLightConstantAttenuation,
	ePODFileLightLinearAttenuation,
	ePODFileLightQuadraticAttenuation,
	ePODFileLightFalloffAngle,
	ePODFileLightFalloffExponent,

	ePODFileCamIdxTgt					= 8000,
	ePODFileCamFOV,
	ePODFileCamFar,
	ePODFileCamNear,
	ePODFileCamAnimFOV,

	ePODFileDataType					= 9000,
	ePODFileN,
	ePODFileStride,
	ePODFileData
};

//*****************************************************************************
//	Struct      EPODLight
//	Brief       Enum for the POD format light types
//*****************************************************************************
enum EPODLight
{
	ePODPoint		= 0 ,						// Point light 
	ePODDirectional		,						// Directional light 
	ePODSpot									// Spot light 
};

//*****************************************************************************
//	Struct      EPODPrimitiveType
//	Brief       Enum for the POD format primitive types
//*****************************************************************************
enum EPODPrimitiveType
{
	ePODTriangles	= 0 ,						// Triangles 
	ePODLines									// Lines
};

//*****************************************************************************
//	Struct      EPODAnimationData
//	Brief       Enum for the POD format animation types
//*****************************************************************************
enum EPODAnimationData
{
	ePODHasPositionAni	= 0x01,					// Position animation 
	ePODHasRotationAni	= 0x02,					// Rotation animation 
	ePODHasScaleAni		= 0x04,					// Scale animation 
	ePODHasMatrixAni	= 0x08					// Matrix animation 
};

//*****************************************************************************
//	Struct      EPODMaterialFlags
//	Brief       Enum for the material flag options
//*****************************************************************************
enum EPODMaterialFlags
{
	ePODEnableBlending	= 0x01,					// Enable blending for this material 
};

//*****************************************************************************
//	Struct      EPODBlendFunc
//	Brief       Enum for the POD format blend functions
//*****************************************************************************
enum EPODBlendFunc
{
	ePODBlendFunc_ZERO	= 0 ,
	ePODBlendFunc_ONE,
	ePODBlendFunc_BLEND_FACTOR,
	ePODBlendFunc_ONE_MINUS_BLEND_FACTOR,

	ePODBlendFunc_SRC_COLOR = 0x0300,
	ePODBlendFunc_ONE_MINUS_SRC_COLOR,
	ePODBlendFunc_SRC_ALPHA,
	ePODBlendFunc_ONE_MINUS_SRC_ALPHA,
	ePODBlendFunc_DST_ALPHA,
	ePODBlendFunc_ONE_MINUS_DST_ALPHA,
	ePODBlendFunc_DST_COLOR,
	ePODBlendFunc_ONE_MINUS_DST_COLOR,
	ePODBlendFunc_SRC_ALPHA_SATURATE,

	ePODBlendFunc_CONSTANT_COLOR = 0x8001,
	ePODBlendFunc_ONE_MINUS_CONSTANT_COLOR,
	ePODBlendFunc_CONSTANT_ALPHA,
	ePODBlendFunc_ONE_MINUS_CONSTANT_ALPHA
};

//*****************************************************************************
//	Struct      EPODBlendOp
//	Brief       Enum for the POD format blend operation
//*****************************************************************************
enum EPODBlendOp
{
	ePODBlendOp_ADD = 0x8006,
	ePODBlendOp_MIN,
	ePODBlendOp_MAX,
	ePODBlendOp_SUBTRACT = 0x800A,
	ePODBlendOp_REVERSE_SUBTRACT,
};

//*****************************************************************************
//	Structures
//*****************************************************************************

//*****************************************************************************
//	Handles a batch of bones
//*****************************************************************************
//*****************************************************************************
//	Class CPVRTBoneBatches
//	Brief A class for processing vertices into bone batches
//*****************************************************************************
class CPVRTBoneBatches
{
	public:

		KDint*		batches;					// Space for nBatchBoneMax bone indices, per batch 
		KDint*		batch_bone_count;			// Actual number of bone indices, per batch 
		KDint*		batch_offset;				// Offset into triangle array, per batch 
		KDint		batch_bone_max;				// Stored value as was passed into Create() 
		KDint		batch_count;				// Number of batches to render 

		KDint Create(
			KDint* const			vertex_number_output,	// vertex count
			KDchar** const			vertex_output,			// Output vertices (program must free() this)
			KDuint16* const			index_array,			// index array for triangle list
			const KDint				vertex_number,			// vertex count
			const KDchar* const		vertices,				// vertices
			const KDint				stride,					// Size of a vertex (in bytes)
			const KDint				offset_weight,			// Offset in bytes to the vertex bone-weights
			const EPVRTDataType		type_weight,			// Data type of the vertex bone-weights
			const KDint				offset_idx,				// Offset in bytes to the vertex bone-indices
			const EPVRTDataType		type_idx,				// Data type of the vertex bone-indices
			const KDint				tri_number,				// Number of triangles
			const KDint				batch_bone_max,			// Number of bones a batch can reference
			const KDint				vertex_bones);			// Number of bones affecting each vertex

		//*********************************************************************
		//	Function		Release
		//	Description		Destroy the bone batch structure
		//*********************************************************************
		void Release(void)
		{
			delete batches;				batches	= 0; 
			delete batch_bone_count;	batch_bone_count = 0; 
			delete batch_offset;		batch_offset = 0; 
			batch_count = 0;
		}
};

//*****************************************************************************
// Struct     CPODData
// Brief      Struct for representing POD data
//*****************************************************************************
struct SPODData
{
	EPVRTDataType		type;						// Type of data stored 
	KDuint32			number;						// Number of values per vertex 
	KDuint32			stride;						// Distance in bytes from one array entry to the next 
	KDuint8*			data;						// Actual data (array of values); if mesh is interleaved, this is an OFFSET from pInterleaved 

	SPODData(void)
	{
		kdMemset( this, 0, sizeof( SPODData ) );
		type   = EPODDataFloat;
	}

	~SPODData(void)
	{
		if ( data )
		{
			delete[] data;
		}
	}
};

//*****************************************************************************
//	Struct      SPODCamera
//	Brief       Struct for storing POD camera data
//*****************************************************************************
struct SPODCamera 
{
	KDint32				idx_target;					// Index of the target object 
	VERTTYPE			fov;						// Field of view 
	VERTTYPE			zfar;						// Far clip plane 
	VERTTYPE			znear;						// Near clip plane 
	VERTTYPE*			animate;					// 1 VERTTYPE per frame of animation. 

	SPODCamera(void)
	{
		kdMemset( this, 0, sizeof( SPODCamera ) );
	}

	~SPODCamera(void)
	{
		if ( animate ) 
		{
			delete[] animate;
		}
	}
};

//*****************************************************************************
//	Struct      SPODLight
//	Brief       Struct for storing POD light data
//*****************************************************************************
struct SPODLight 
{
	KDint32				idx_target;					// Index of the target object 
	VERTTYPE			color[3];					// Light colour (0.0f -> 1.0f for each channel) 
	EPODLight			type;						// Light type (point, directional, spot etc.) 
	KDfloat				constant_attenuation;		// Constant attenuation 
	KDfloat				linear_attenuation;			// Linear atternuation 
	KDfloat				quadratic_attenuation;		// Quadratic attenuation 
	KDfloat				falloff_angle;				// Falloff angle (in radians) 
	KDfloat				falloff_exponent;			// Falloff exponent 

	SPODLight(void)
	{
		kdMemset( this, 0, sizeof( SPODLight ) );
	}

	~SPODLight(void)
	{
		
	}
};

//*****************************************************************************
//	Struct      SPODMesh
//	Brief       Struct for storing POD mesh data
//*****************************************************************************
struct SPODMesh 
{
	KDuint32			num_vertex;					// Number of vertices in the mesh 
	KDuint32			num_faces;					// Number of triangles in the mesh 
	KDuint32			num_UVW;					// Number of texture coordinate channels per vertex 
	SPODData			faces;						// List of triangle indices 
	KDuint32*			strip_length;				// If mesh is stripped: number of tris per strip. 
	KDuint32			num_strips;					// If mesh is stripped: number of strips, length of pnStripLength array. 
	SPODData			vertex;						// List of vertices (x0, y0, z0, x1, y1, z1, x2, etc...) 
	SPODData			normals;					// List of vertex normals (Nx0, Ny0, Nz0, Nx1, Ny1, Nz1, Nx2, etc...) 
	SPODData			tangents;					// List of vertex tangents (Tx0, Ty0, Tz0, Tx1, Ty1, Tz1, Tx2, etc...) 
	SPODData			binormals;					// List of vertex binormals (Bx0, By0, Bz0, Bx1, By1, Bz1, Bx2, etc...) 
	SPODData*			UVW;						// List of UVW coordinate sets; size of array given by 'nNumUVW' 
	SPODData			vertex_colors;				// A colour per vertex 
	SPODData			bone_idx;					// nNumBones*nNumVertex ints (Vtx0Idx0, Vtx0Idx1, ... Vtx1Idx0, Vtx1Idx1, ...) 
	SPODData			bone_weight;				// nNumBones*nNumVertex floats (Vtx0Wt0, Vtx0Wt1, ... Vtx1Wt0, Vtx1Wt1, ...) 

	KDuint8*			interleaved;				// Interleaved vertex data 

	CPVRTBoneBatches	bone_batches;				// Bone tables 

	EPODPrimitiveType	primitive_type;				// Primitive type used by this mesh 

	XMGMatrix4F			unpack_matrix;				// A matrix used for unscaling scaled vertex data created with PVRTModelPODScaleAndConvertVtxData

	SPODMesh(void)
	{
		kdMemset( this, 0, sizeof( SPODMesh ) );

		primitive_type = ePODTriangles;
		//unpack_matrix.Identity();
	}

	~SPODMesh(void)
	{
		if ( strip_length )
		{
			delete[] strip_length;
		}

		if ( UVW )
		{
			delete[] UVW;
		}

		if ( interleaved )
		{
			delete[] interleaved;
		}
	}

};

//*****************************************************************************
//	Struct      SPODNode
//	Brief       Struct for storing POD node data
//*****************************************************************************
struct SPODNode
{
	KDint32				index;						// Index into mesh, light or camera array, depending on which object list contains this Node 
	KDchar*				name;						// Name of object 
	KDint32				idx_material;				// Index of material used on this mesh 

	KDint32				idx_parent;					// Index into MeshInstance array; recursively apply ancestor's transforms after this instance's. 

	KDuint32			ani_flags;					// Stores which animation arrays the POD Node contains 

	KDuint32*			ani_position_idx;
	VERTTYPE*			ani_position;				// 3 floats per frame of animation. 

	KDuint32*			ani_rotation_idx;
	VERTTYPE*			ani_rotation;				// 4 floats per frame of animation. 

	KDuint32*			ani_scale_idx;
	VERTTYPE*			ani_scale;					// 7 floats per frame of animation. 

	KDuint32*			ani_matrix_idx;
	VERTTYPE*			ani_matrix;					// 16 floats per frame of animation. 

	SPODNode(void)
	{
		kdMemset( this, 0, sizeof( SPODNode ) );
	}

	~SPODNode(void)
	{
		if ( name )
		{
			delete[] name;
		}

		if ( ani_position_idx )
		{
			delete[] ani_position_idx;
		}

		if ( ani_position )
		{
			delete[] ani_position;
		}

		if ( ani_rotation_idx )
		{
			delete[] ani_rotation_idx;
		}

		if ( ani_rotation )
		{
			delete[] ani_rotation;
		}

		if ( ani_scale_idx )
		{
			delete[] ani_scale_idx;
		}

		if ( ani_scale )
		{
			delete[] ani_scale;
		}

		if ( ani_matrix_idx )
		{
			delete[] ani_matrix_idx;
		}

		if ( ani_matrix )
		{
			delete[] ani_matrix;
		}
	}
};

//*****************************************************************************
//	Struct      SPODTexture
//	Brief       Struct for storing POD texture data
//*****************************************************************************
struct SPODTexture 
{
	KDchar*				name;						// File-name of texture 

	SPODTexture(void)
	{
		name = 0;
	}

	~SPODTexture(void)
	{
		if ( name )
		{
			delete[] name;
		}
	}
};

//*****************************************************************************
//	Struct      SPODMaterial
//	Brief       Struct for storing POD material data
//*****************************************************************************
struct SPODMaterial 
{
	KDchar*			name;							// Name of material 
	KDint32			idx_tex_diffuse;				// Idx into pTexture for the diffuse texture 
	KDint32			idx_tex_ambient;				// Idx into pTexture for the ambient texture 
	KDint32			idx_tex_specular_color;			// Idx into pTexture for the specular colour texture 
	KDint32			idx_tex_specular_level;			// Idx into pTexture for the specular level texture 
	KDint32			idx_tex_bump;					// Idx into pTexture for the bump map 
	KDint32			idx_tex_emissive;				// Idx into pTexture for the emissive texture 
	KDint32			idx_tex_glossiness;				// Idx into pTexture for the glossiness texture 
	KDint32			idx_tex_opacity;				// Idx into pTexture for the opacity texture 
	KDint32			idx_tex_reflection;				// Idx into pTexture for the reflection texture 
	KDint32			idx_tex_refraction;				// Idx into pTexture for the refraction texture 
	VERTTYPE		mat_opacity;					// Material opacity (used with vertex alpha ?) 
	VERTTYPE		mat_ambient[3];					// Ambient RGB value 
	VERTTYPE		mat_diffuse[3];					// Diffuse RGB value 
	VERTTYPE		mat_specular[3];				// Specular RGB value 
	VERTTYPE		mat_shininess;					// Material shininess 
	KDchar*			effect_file;					// Name of effect file 
	KDchar*			effect_name;					// Name of effect in the effect file 
	EPODBlendFunc	blend_src_RGB;					// Blending RGB source value 
	EPODBlendFunc	blend_src_A;					// Blending alpha source value 
	EPODBlendFunc	blend_dst_RGB;					// Blending RGB destination value 
	EPODBlendFunc	blend_dst_A;					// Blending alpha destination value 
	EPODBlendOp		blend_op_RGB;					// Blending RGB operation 
	EPODBlendOp		blend_op_A;						// Blending alpha operation 
	VERTTYPE		blend_color[4];					// A RGBA colour to be used in blending 
	VERTTYPE		blend_factor[4];				// An array of blend factors, one for each RGBA component 
	KDuint32		flags;							// Stores information about the material e.g. Enable blending 

	SPODMaterial(void)
	{
		kdMemset( this, 0, sizeof( SPODMaterial ) );

		idx_tex_diffuse		   = -1;
		idx_tex_ambient		   = -1;
		idx_tex_specular_color = -1;
		idx_tex_specular_level = -1;
		idx_tex_bump		   = -1;
		idx_tex_emissive	   = -1;
		idx_tex_glossiness	   = -1;
		idx_tex_opacity		   = -1;
		idx_tex_reflection	   = -1;
		idx_tex_refraction	   = -1;

		blend_src_RGB = blend_src_A = ePODBlendFunc_ONE;
		blend_dst_RGB = blend_dst_A = ePODBlendFunc_ZERO;
		blend_op_RGB  = blend_op_A  = ePODBlendOp_ADD;
	}

	~SPODMaterial(void)
	{
		if ( name )
		{
			delete[] name;
		}

		if ( effect_file )
		{
			delete[] effect_file;
		}

		if ( effect_name )
		{
			delete[] effect_name;
		}
	}
};

//*****************************************************************************
//	Struct      SPODScene
//	Brief       Struct for storing POD scene data
//*****************************************************************************
struct SPODScene 
{
	VERTTYPE			color_background[3];		// Background colour 
	VERTTYPE			color_ambient[3];			// Ambient colour 

	KDuint32			num_camera;					// The length of the array pCamera 
	SPODCamera*			camera;						// Camera nodes array 

	KDuint32			num_light;					// The length of the array pLight 
	SPODLight*			light;						// Light nodes array 

	KDuint32			num_mesh;					// The length of the array pMesh 
	SPODMesh*			mesh;						// Mesh array. Meshes may be instanced several times in a scene; i.e. multiple Nodes may reference any given mesh. 

	KDuint32			num_node;					// Number of items in the array pNode 
	KDuint32			num_mesh_node;				// Number of items in the array pNode which are objects 
	SPODNode*			node;						// Node array. Sorted as such: objects, lights, cameras, Everything Else (bones, helpers etc) 

	KDuint32			num_texture;				// Number of textures in the array pTexture 
	SPODTexture*		texture;					// Texture array 

	KDuint32			num_material;				// Number of materials in the array pMaterial 
	SPODMaterial*		material;					// Material array 

	KDuint32			num_frame;					// Number of frames of animation 
	KDuint32			flags;						// PVRTMODELPODSF_* bit-flags 

	SPODScene(void)
	{
		kdMemset( this, 0, sizeof( SPODScene ) );
	}

	~SPODScene(void)
	{
		if ( camera )
		{
			delete[] camera;
		}

		if ( light )
		{
			delete[] light;
		}

		if ( mesh )
		{
			delete[] mesh;
		}

		if ( node )
		{
			delete[] node;
		}

		if ( texture )
		{
			delete[] texture;
		}

		if ( material )
		{
			delete[] material;
		}
	}
};

class XMGPODImpl
{
	friend class XMGPOD;	

	private :

		XMGPODImpl(void);
		virtual ~XMGPODImpl(void);

		template <typename XMGTemplate>
		XMGBool		SafeAlloc	(XMGTemplate** mem, KDuint32 cnt);

		template <typename XMGTemplate>
		XMGBool		ReadValue	(KDFile* file, XMGTemplate*  value, KDuint32  cnt);
		XMGBool		ReadMarker	(KDFile* file, KDuint32& name , KDuint32& len);		
		XMGBool		ReadSkip	(KDFile* file, KDuint32  len);	

		XMGBool		ReadScene	(KDFile* file, SPODScene&	 scene	 );
		XMGBool		ReadCamera	(KDFile* file, SPODCamera&	 camera  );
		XMGBool		ReadLight	(KDFile* file, SPODLight&	 light	 );
		XMGBool		ReadMaterial(KDFile* file, SPODMaterial& material);
		XMGBool		ReadMesh	(KDFile* file, SPODMesh&	 mesh	 );
		XMGBool		ReadNode	(KDFile* file, SPODNode&	 node	 );
		XMGBool		ReadTexture (KDFile* file, SPODTexture&  texture );
		XMGBool		ReadData	(KDFile* file, SPODData&	 data	 , KDuint32 spec, KDint valid_data);

		void		FixInterleavedEndianness(SPODMesh& mesh);

		KDuint32	PODCountIndices(const SPODMesh& mesh);
		KDuint32	PODDataTypeSize(const EPVRTDataType type);

		VERTTYPE	GetCameraPos(XMGVector3F& from, XMGVector3F& to, const KDuint32 idx) const;
		void		GetWorldMatrix(XMGMatrix4F& out, const SPODNode& node) const;

	private :

		void		SetVertices(void);

		SPODScene*		m_scene;
		GLuint*			m_vertices;
		GLuint*			m_indices;
};

#define IMPL_POD		((XMGPODImpl *) pod->m_impl)

#endif // __xmg_pod_impl_h__ 
