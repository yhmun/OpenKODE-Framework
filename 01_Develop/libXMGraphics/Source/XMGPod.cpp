/*
 *	File			xmg_pod.cpp
 *	Description		XMGraphic : POD 
 *
 *	Author			Y.H Mun
 *	Contact			chris@xmsoft.co.kr				
 *	Copyright		Copyright (C) 2010 XMSoft. All rights reserved.
 */

#include "xmg_base.h"

#define IMPL			((XMGPODImpl *) m_impl)

XMGPODImpl::XMGPODImpl(void) : m_scene ( new SPODScene( ) ), m_vertices ( 0 ), m_indices ( 0 )
{
	
}

XMGPODImpl::~XMGPODImpl(void)
{
	KDuint	i;

	if ( m_vertices )
	{
		glDeleteBuffers ( m_scene->num_mesh, m_vertices );
		delete[] m_vertices;
	}

	if ( m_indices )
	{
		for ( i = 0; i < m_scene->num_mesh; i++ )
		{
			if ( m_indices[i] )
			{
				glDeleteBuffers ( 1, &m_indices[i] );
			}
		}

		delete[] m_indices;
	}

	delete m_scene;
}

template <typename XMGTemplate>
XMGBool XMGPODImpl::SafeAlloc(XMGTemplate** mem, KDuint32 cnt)
{
	if ( mem && cnt )
	{
		*mem = (XMGTemplate *) new XMGTemplate[ cnt ]; 

		if ( !*mem )
		{
			return XMG_FALSE;
		}
	}
	else
	{
		return XMG_FALSE;
	}

	return XMG_TRUE;
}

template <typename XMGTemplate>
XMGBool XMGPODImpl::ReadValue(KDFile* file, XMGTemplate* value, KDuint32 cnt)
{
	if ( !kdFread ( value, sizeof ( XMGTemplate ) * cnt, 1, file ) )
	{
		return XMG_FALSE;
	}

	return XMG_TRUE;
}

XMGBool XMGPODImpl::ReadMarker(KDFile* file, KDuint32& name, KDuint32& len)
{
	if ( !( kdFread ( &name, sizeof ( KDuint32 ), 1, file ) &&
		    kdFread ( &len , sizeof ( KDuint32 ), 1, file ) ) )
	{
		return XMG_FALSE;
	}

	return XMG_TRUE;
}

XMGBool XMGPODImpl::ReadSkip(KDFile* file, KDuint32 len)
{
	XMGBool		ret  = XMG_TRUE;
	void*		temp = 0;		

	if ( len > 0 )
	{				
		if ( ( temp = kdMalloc ( len ) ) )
		{
			if ( !kdFread ( temp, len, 1, file ) )
			{
				ret = XMG_FALSE;
			}
			
			kdFree( temp );
		}
		else
		{
			ret = XMG_FALSE;
		}				
	}

	return ret;
}

XMGBool XMGPODImpl::ReadScene(KDFile* file, SPODScene& scene)
{
	KDuint32	name  = 0;
	KDuint32	len   = 0;

	KDuint32	num_camera   = 0;
	KDuint32	num_light	 = 0;
	KDuint32	num_material = 0;
	KDuint32	num_mesh     = 0;
	KDuint32	num_texture  = 0;
	KDuint32	num_node	 = 0;

	while ( ReadMarker ( file, name, len ) == XMG_TRUE )
	{
		switch ( name )
		{
			case ePODFileScene | PVRTMODELPOD_TAG_END :

				if ( num_camera   != scene.num_camera   )	return XMG_FALSE;
				if ( num_light	  != scene.num_light	)	return XMG_FALSE;
				if ( num_material != scene.num_material )	return XMG_FALSE;
				if ( num_mesh	  != scene.num_mesh     )	return XMG_FALSE;
				if ( num_texture  != scene.num_texture  )	return XMG_FALSE;
				if ( num_node	  != scene.num_node     )	return XMG_FALSE;
				
				return XMG_TRUE;

			case ePODFileColourBackground	:	if ( ReadValue	 ( file, (KDuint32 *) scene.color_background , 3 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileColourAmbient		:	if ( ReadValue	 ( file, (KDuint32 *) scene.color_ambient	 , 3 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileNumCamera			:	if ( ReadValue	 ( file, &scene.num_camera					 , 1 ) == XMG_FALSE ) return XMG_FALSE;
												if ( SafeAlloc	 (		  &scene.camera,		scene.num_camera ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileNumLight			:	if ( ReadValue	 ( file, &scene.num_light					 , 1 ) == XMG_FALSE ) return XMG_FALSE;	
												if ( SafeAlloc	 (		  &scene.light,			scene.num_light  ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileNumMesh			:	if ( ReadValue	 ( file, &scene.num_mesh					 , 1 ) == XMG_FALSE ) return XMG_FALSE;
												if ( SafeAlloc	 (		  &scene.mesh,			scene.num_mesh   ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileNumNode			:	if ( ReadValue	 ( file, &scene.num_node					 , 1 ) == XMG_FALSE ) return XMG_FALSE;
												if ( SafeAlloc	 (		  &scene.node,			scene.num_node   ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileNumMeshNode		:	if ( ReadValue	 ( file, &scene.num_mesh_node				 , 1 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileNumTexture			:	if ( ReadValue	 ( file, &scene.num_texture					 , 1 ) == XMG_FALSE ) return XMG_FALSE;
												if ( SafeAlloc	 (		  &scene.texture,	   scene.num_texture ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileNumMaterial		:	if ( ReadValue	 ( file, &scene.num_material				 , 1 ) == XMG_FALSE ) return XMG_FALSE;
												if ( SafeAlloc	 (		  &scene.material,	  scene.num_material ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileNumFrame			:	if ( ReadValue	 ( file, &scene.num_frame					 , 1 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileFlags				: 	if ( ReadValue	 ( file, &scene.flags						 , 1 ) == XMG_FALSE ) return XMG_FALSE;	break;

			case ePODFileCamera				:	if ( ReadCamera  ( file, scene.camera[ num_camera++ ]			 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileLight				:	if ( ReadLight   ( file, scene.light[ num_light++ ]				 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileMaterial			:	if ( ReadMaterial( file, scene.material[ num_material++ ]		 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileMesh				:	if ( ReadMesh    ( file, scene.mesh[ num_mesh++ ]				 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileNode				:	if ( ReadNode    ( file, scene.node[ num_node++ ]				 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileTexture			:	if ( ReadTexture ( file, scene.texture[ num_texture++ ]			 ) == XMG_FALSE ) return XMG_FALSE;	break;

			default							:	if ( ReadSkip	 ( file, len									 ) == XMG_FALSE ) return XMG_FALSE;		}

	}

	return XMG_TRUE;
}

XMGBool XMGPODImpl::ReadCamera(KDFile* file, SPODCamera& camera)
{
	KDuint32	name  = 0;
	KDuint32	len   = 0;

	while ( ReadMarker ( file, name, len ) == XMG_TRUE )
	{
		switch ( name )
		{
			case ePODFileCamera | PVRTMODELPOD_TAG_END :	return XMG_TRUE;

			case ePODFileCamIdxTgt	:	if ( ReadValue ( file, &camera.idx_target		  , 1   ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileCamFOV		:	if ( ReadValue ( file, &camera.fov				  , 1   ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileCamFar		:	if ( ReadValue ( file, &camera.zfar				  , 1   ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileCamNear	:	if ( ReadValue ( file, &camera.znear			  , 1   ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileCamAnimFOV :	if ( SafeAlloc (	   &camera.animate			  , len ) == XMG_FALSE ) return XMG_FALSE;
										if ( ReadValue ( file, (KDuint32 *) camera.animate, len ) == XMG_FALSE ) return XMG_FALSE;	break;

			default					:	if ( ReadSkip  ( file,							    len ) == XMG_FALSE ) return XMG_FALSE;
		}
	}

	return XMG_TRUE;	
}

XMGBool XMGPODImpl::ReadLight(KDFile* file, SPODLight&	light)
{
	KDuint32	name  = 0;
	KDuint32	len   = 0;

	while ( ReadMarker ( file, name, len ) == XMG_TRUE )
	{
		switch ( name )
		{
			case ePODFileLight | PVRTMODELPOD_TAG_END :		return XMG_TRUE;	

			case ePODFileLightIdxTgt				:	if ( ReadValue ( file, &light.idx_target			, 1 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileLightColour				:	if ( ReadValue ( file, (KDuint32 *) light.color		, 3 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileLightType					:	if ( ReadValue ( file, (KDuint32 *) &light.type		, 1 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileLightConstantAttenuation	:	if ( ReadValue ( file, &light.constant_attenuation	, 1 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileLightLinearAttenuation		:	if ( ReadValue ( file, &light.linear_attenuation	, 1 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileLightQuadraticAttenuation	:	if ( ReadValue ( file, &light.quadratic_attenuation	, 1 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileLightFalloffAngle			:	if ( ReadValue ( file, &light.falloff_angle			, 1 ) == XMG_FALSE ) return XMG_FALSE;	break;
			case ePODFileLightFalloffExponent		:	if ( ReadValue ( file, &light.falloff_exponent		, 1 ) == XMG_FALSE ) return XMG_FALSE;	break;

			default									:	if ( ReadSkip  ( file,								len ) == XMG_FALSE ) return XMG_FALSE;
		}
	}

	return XMG_TRUE;	
}

XMGBool XMGPODImpl::ReadMaterial(KDFile* file, SPODMaterial& material)
{	
	KDuint32	name  = 0;
	KDuint32	len   = 0;

	while ( ReadMarker ( file, name, len ) == XMG_TRUE )
	{
		switch ( name )
		{
			case ePODFileMaterial | PVRTMODELPOD_TAG_END :	return XMG_TRUE;	

			case ePODFileMatFlags				 :	if ( ReadValue ( file, &material.flags						, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatName				 :	if ( SafeAlloc (	   &material.name						, len ) == XMG_FALSE ) return XMG_FALSE;
													if ( ReadValue ( file, material.name						, len ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexDiffuse		 :	if ( ReadValue ( file, &material.idx_tex_diffuse			, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexAmbient		 :	if ( ReadValue ( file, &material.idx_tex_ambient			, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexSpecularColour :	if ( ReadValue ( file, &material.idx_tex_specular_color		, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexSpecularLevel  :	if ( ReadValue ( file, &material.idx_tex_specular_level		, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexBump			 :	if ( ReadValue ( file, &material.idx_tex_bump				, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexEmissive		 :	if ( ReadValue ( file, &material.idx_tex_emissive			, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexGlossiness	 :	if ( ReadValue ( file, &material.idx_tex_glossiness			, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexOpacity		 :	if ( ReadValue ( file, &material.idx_tex_opacity			, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexReflection	 :	if ( ReadValue ( file, &material.idx_tex_reflection			, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatIdxTexRefraction	 :	if ( ReadValue ( file, &material.idx_tex_refraction			, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatOpacity				 :	if ( ReadValue ( file, &material.mat_opacity				, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatAmbient				 :	if ( ReadValue ( file, (KDuint32 *) material.mat_ambient	, 3   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatDiffuse				 :	if ( ReadValue ( file, (KDuint32 *) material.mat_diffuse	, 3   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatSpecular			 :	if ( ReadValue ( file, (KDuint32 *) material.mat_specular	, 3   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatShininess			 :	if ( ReadValue ( file, &material.mat_shininess				, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatEffectFile			 :	if ( SafeAlloc (	   &material.effect_file				, len ) == XMG_FALSE ) return XMG_FALSE;
													if ( ReadValue ( file, material.effect_file					, len ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatEffectName			 :	if ( SafeAlloc (	   &material.effect_name				, len ) == XMG_FALSE ) return XMG_FALSE;
													if ( ReadValue ( file, material.effect_name					, len ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatBlendSrcRGB			 :	if ( ReadValue ( file, (KDuint32 *) &material.blend_src_RGB	, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatBlendSrcA			 :	if ( ReadValue ( file, (KDuint32 *) &material.blend_src_A	, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatBlendDstRGB			 :	if ( ReadValue ( file, (KDuint32 *) &material.blend_dst_RGB	, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatBlendDstA			 :	if ( ReadValue ( file, (KDuint32 *) &material.blend_dst_A	, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatBlendOpRGB			 :	if ( ReadValue ( file, (KDuint32 *) &material.blend_op_RGB	, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatBlendOpA			 :	if ( ReadValue ( file, (KDuint32 *) &material.blend_op_A	, 1   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatBlendColour			 :	if ( ReadValue ( file, (KDuint32 *) material.blend_color	, 4   ) == XMG_FALSE ) return XMG_FALSE; break;
			case ePODFileMatBlendFactor			 :	if ( ReadValue ( file, (KDuint32 *) material.blend_factor	, 4   ) == XMG_FALSE ) return XMG_FALSE; break;

			default								 :	if ( ReadSkip  ( file, len ) == XMG_FALSE ) return XMG_FALSE;	
		}
	}

	return XMG_TRUE;	
}

XMGBool XMGPODImpl::ReadMesh(KDFile* file, SPODMesh& mesh)
{
	KDuint32	name    = 0;
	KDuint32	len     = 0;
	KDuint32	num_UVW = 0;

	while ( ReadMarker ( file, name, len ) == XMG_TRUE )
	{
		switch ( name )
		{
			case ePODFileMesh | PVRTMODELPOD_TAG_END :	
				
				if ( num_UVW != mesh.num_UVW )
				{
					return XMG_FALSE;
				}

				FixInterleavedEndianness( mesh );
				return XMG_TRUE;

			case ePODFileMeshNumVtx						:			if ( ReadValue( file, &mesh.num_vertex, 1 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshNumFaces :		
				if ( ReadValue( file, &mesh.num_faces, 1 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshNumUVW :			
				if ( ReadValue( file, &mesh.num_UVW, 1 ) == XMG_FALSE ) return XMG_FALSE;
				if ( SafeAlloc( &mesh.UVW, mesh.num_UVW ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshStripLength :	
				if ( SafeAlloc( &mesh.strip_length, len ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, &mesh.strip_length, len / 4 ) == XMG_FALSE ) return XMG_FALSE;
				break;
			case ePODFileMeshNumStrips :		
				if ( ReadValue( file, &mesh.num_strips, 1 ) == XMG_FALSE ) return XMG_FALSE;
				break;
			case ePODFileMeshInterleaved :	
				if ( SafeAlloc( &mesh.interleaved, len ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, mesh.interleaved, len ) == XMG_FALSE ) return XMG_FALSE;
				break;
			case ePODFileMeshBoneBatches :	
				if ( SafeAlloc( &mesh.bone_batches.batches, len / 4 ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, &mesh.bone_batches.batches, len / 4 ) == XMG_FALSE ) return XMG_FALSE;
				break;
			case ePODFileMeshBoneBatchBoneCnts :	
				if ( SafeAlloc( &mesh.bone_batches.batch_bone_count, len / 4 ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, &mesh.bone_batches.batch_bone_count, len / 4 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshBoneBatchOffsets :
				if ( SafeAlloc( &mesh.bone_batches.batch_offset, len / 4 ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, &mesh.bone_batches.batch_offset, len / 4 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshBoneBatchBoneMax :
				if ( ReadValue( file, &mesh.bone_batches.batch_bone_max, 1 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshBoneBatchCnt :	
				if ( ReadValue( file, &mesh.bone_batches.batch_count, 1 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshUnpackMatrix :		
				if ( ReadValue( file, (KDuint32 *) mesh.unpack_matrix.Get(), 16 ) == XMG_FALSE ) return XMG_FALSE;
				break;
			
			case ePODFileMeshFaces :	
				if ( ReadData( file, mesh.faces, ePODFileMeshFaces, 1 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshVtx :			
				if ( ReadData( file, mesh.vertex, ePODFileMeshVtx, mesh.interleaved == 0 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshNor:			
				if ( ReadData( file, mesh.normals, ePODFileMeshNor, mesh.interleaved == 0 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshTan:			
				if ( ReadData( file, mesh.tangents, ePODFileMeshTan, mesh.interleaved == 0 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshBin:			
				if ( ReadData( file, mesh.binormals, ePODFileMeshBin, mesh.interleaved == 0 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshUVW:			
				if ( ReadData( file, mesh.UVW[ num_UVW++ ], ePODFileMeshUVW, mesh.interleaved == 0 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshVtxCol:		
				if ( ReadData( file, mesh.vertex_colors, ePODFileMeshVtxCol, mesh.interleaved == 0 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshBoneIdx:	
				if ( ReadData( file, mesh.bone_idx, ePODFileMeshBoneIdx, mesh.interleaved == 0 ) == XMG_FALSE ) return XMG_FALSE;
				break;

			case ePODFileMeshBoneWeight :
				if ( ReadData( file, mesh.bone_weight, ePODFileMeshBoneWeight, mesh.interleaved == 0 ) == XMG_FALSE ) return XMG_FALSE;
				break;
				
			default :

				if ( ReadSkip( file, len ) == XMG_FALSE ) return XMG_FALSE;
		}
	}

	return XMG_TRUE;
}

XMGBool XMGPODImpl::ReadNode(KDFile* file, SPODNode& node)
{
	KDuint32	name  = 0;
	KDuint32	len   = 0;

	KDint		old_format = 0;
	VERTTYPE	pos[3]     = { 0, 0, 0 };
	VERTTYPE	quat[4]    = { 0, 0, 1 };
	VERTTYPE	scale[7]   = { 1, 1, 1, 0, 0, 0, 0 };

	while ( ReadMarker( file, name, len ) == XMG_TRUE )
	{
		switch ( name )
		{
			case ePODFileNode | PVRTMODELPOD_TAG_END :

				if ( old_format )
				{
					if ( node.ani_position )
					{
						node.ani_flags |= ePODHasPositionAni;
					}
					else
					{
						if ( SafeAlloc( &node.ani_position, 3 ) == XMG_FALSE ) return XMG_FALSE;
						kdMemcpy( node.ani_position, pos, sizeof( pos ) );
					}

					if ( node.ani_rotation )
					{
						node.ani_flags |= ePODHasRotationAni;
					}
					else
					{
						if ( SafeAlloc( &node.ani_rotation, 4 ) == XMG_FALSE ) return XMG_FALSE;
						kdMemcpy( node.ani_rotation, quat, sizeof( quat ) );
					}

					if ( node.ani_scale )
					{
						node.ani_flags |= ePODHasScaleAni;
					}
					else
					{
						if ( SafeAlloc( &node.ani_scale, 7 ) == XMG_FALSE ) return XMG_FALSE;
						kdMemcpy( node.ani_scale, scale, sizeof( scale ) );
					}
				}

				return XMG_TRUE;

			case ePODFileNodeIdx :	
				
				if ( ReadValue( file, &node.index, 1 ) == XMG_FALSE ) return XMG_FALSE;
				
				break;

			case ePODFileNodeName :	
				
				if ( SafeAlloc( &node.name, len ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, node.name, len ) == XMG_FALSE ) return XMG_FALSE;
					
				break;
			case ePODFileNodeIdxMat :
				
				if ( ReadValue( file, &node.idx_material, 1 ) == XMG_FALSE ) return XMG_FALSE;
		
				break;

			case ePODFileNodeIdxParent :
				
				if ( ReadValue( file, &node.idx_parent, 1 ) == XMG_FALSE ) return XMG_FALSE;
			
				break;

			case ePODFileNodeAnimFlags :
				
				if ( ReadValue( file, &node.ani_flags, 1 ) == XMG_FALSE ) return XMG_FALSE;

				break;

			case ePODFileNodeAnimPosIdx :	
				
				if ( SafeAlloc( &node.ani_position_idx, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, node.ani_position_idx, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;

				break;

			case ePODFileNodeAnimPos :	
				
				if ( SafeAlloc( &node.ani_position, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, (KDuint32 *) node.ani_position, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;

				break;

			case ePODFileNodeAnimRotIdx :
				
				if ( SafeAlloc( &node.ani_rotation_idx, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, node.ani_rotation_idx, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;

				break;

			case ePODFileNodeAnimRot :	
				
				if ( SafeAlloc( &node.ani_rotation, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, (KDuint32 *) node.ani_rotation, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;

				break;

			case ePODFileNodeAnimScaleIdx :	
				
				if ( SafeAlloc( &node.ani_scale_idx, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, node.ani_scale_idx, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;

				break;

			case ePODFileNodeAnimScale :	
				
				if ( SafeAlloc( &node.ani_scale, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, (KDuint32 *) node.ani_scale, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;

				break;

			case ePODFileNodeAnimMatrixIdx :
				
				if ( SafeAlloc( &node.ani_matrix_idx, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, node.ani_matrix_idx, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;

				break;

			case ePODFileNodeAnimMatrix :
				
				if ( SafeAlloc( &node.ani_matrix, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, (KDuint32 *) node.ani_matrix, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;

				break;

			// Parameters from the older pod format
			case ePODFileNodePos :	

				if ( ReadValue( file, (KDuint32 *) pos, 3 ) == XMG_FALSE ) return XMG_FALSE;
				old_format = 1;

				break;

			case ePODFileNodeRot :	
				
				if ( ReadValue( file, (KDuint32 *) quat, 4 ) == XMG_FALSE ) return XMG_FALSE;
				old_format = 1;	

				break;

			case ePODFileNodeScale :	
				
				if ( ReadValue( file, (KDuint32 *) scale, 3 ) == XMG_FALSE ) return XMG_FALSE;
				old_format = 1;		

				break;

			default :

				if ( ReadSkip( file, len ) == XMG_FALSE ) return XMG_FALSE;
		}
	}

	return XMG_TRUE;
}

XMGBool XMGPODImpl::ReadTexture(KDFile* file, SPODTexture& texture)
{	
	KDuint32	name  = 0;
	KDuint32	len   = 0;

	while ( ReadMarker( file, name, len ) == XMG_TRUE )
	{
		switch ( name )
		{
			case ePODFileTexture | PVRTMODELPOD_TAG_END :		
				
				return XMG_TRUE;

			case ePODFileTexName :	
				
				if ( SafeAlloc( &texture.name, len ) == XMG_FALSE ) return XMG_FALSE;
				if ( ReadValue( file, texture.name, len ) == XMG_FALSE ) return XMG_FALSE;
		
				break;

			default :

				if ( ReadSkip( file, len ) == XMG_FALSE ) return XMG_FALSE;
		}
	}

	return XMG_TRUE;
}

XMGBool XMGPODImpl::ReadData(KDFile* file, SPODData& data, KDuint32 spec, KDint valid_data)
{	
	KDuint32	name   = 0;
	KDuint32	len    = 0;

	KDuint32	buffer = 0;
	KDuint32*	ptr32  = 0;
	KDuint16*	ptr16  = 0;

	while ( ReadMarker( file, name, len ) == XMG_TRUE )
	{
		if ( name == ( spec | PVRTMODELPOD_TAG_END ) )
		{
			break;
		}

		switch ( name )
		{
			case ePODFileDataType :
				
				if ( ReadValue( file, (KDuint32 *) &data.type, 1 ) == XMG_FALSE ) return XMG_FALSE;
	
				break;

			case ePODFileN :		
				
				if ( ReadValue( file, &data.number, 1 ) == XMG_FALSE ) return XMG_FALSE;

				break;

			case ePODFileStride :

				if ( ReadValue( file, &data.stride, 1 ) == XMG_FALSE ) return XMG_FALSE;

				break;
			
			case ePODFileData :

				if ( valid_data )
				{					
					switch ( PODDataTypeSize( data.type ) )
					{
						case 1 : 
							
							if ( SafeAlloc( &data.data, len ) == XMG_FALSE ) return XMG_FALSE;
							if ( ReadValue( file, &data.data, len ) == XMG_FALSE ) return XMG_FALSE;

							break;

						case 2 :
							
							if ( SafeAlloc( &ptr16, len / sizeof( KDuint16 ) ) == XMG_FALSE ) return XMG_FALSE;
							if ( ReadValue( file, ptr16, len / sizeof( KDuint16 ) ) == XMG_FALSE ) return XMG_FALSE;
							data.data = (KDuint8 *) ptr16;									

							break;

						case 4 :

							if ( SafeAlloc( &ptr32, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;
							if ( ReadValue( file, ptr32, len / sizeof( KDuint32 ) ) == XMG_FALSE ) return XMG_FALSE;		
							data.data = (KDuint8 *) ptr32;

							break;

						default :

							return XMG_FALSE;
					}
					
				}
				else
				{
					if ( ReadValue( file, &buffer, 1 ) == XMG_TRUE )
					{
						data.data = (KDuint8 *) buffer;
					}
					else
					{
						return XMG_FALSE;
					}
				}

				break;		 

			default :

				if ( ReadSkip( file, len ) == XMG_FALSE ) return XMG_FALSE;
		}
	}

	return XMG_TRUE;
}

void XMGPODImpl::FixInterleavedEndianness(SPODMesh& mesh)
{
	// Not yet
}

KDuint32 XMGPODImpl::PODCountIndices(const SPODMesh& mesh)
{
	KDuint	i;
	KDuint	count = 0;

	if ( mesh.num_strips )
	{
		for ( i = 0; i < mesh.num_strips; ++i )
		{
			count += mesh.strip_length[i] + 2;
		}

		return count;
	}

	return mesh.num_faces * 3;
}

KDuint32 XMGPODImpl::PODDataTypeSize(const EPVRTDataType type)
{
	switch ( type )
	{
		default						:	return 0;

		case EPODDataFloat			:	return sizeof( KDfloat32 );
		case EPODDataInt			:	return sizeof( KDint32 );
		case EPODDataShort			:
		case EPODDataShortNorm		:
		case EPODDataUnsignedShort	:	return sizeof( KDuint16 );
		case EPODDataRGBA			:	return sizeof( KDuint32 );
		case EPODDataARGB			:	return sizeof( KDuint32 );
		case EPODDataD3DCOLOR		:	return sizeof( KDuint32 );
		case EPODDataUBYTE4			:	return sizeof( KDuint32 );
		case EPODDataDEC3N			:	return sizeof( KDuint32 );
		case EPODDataFixed16_16		:	return sizeof( KDuint32 );
		case EPODDataUnsignedByte	:
		case EPODDataByte			:
		case EPODDataByteNorm		:	return sizeof( KDuint8 );
	}
}

void XMGPODImpl::SetVertices(void)
{
	KDuint		i;
	GLsizeiptr	size;
	SPODMesh*	mesh;

	if ( m_scene->num_mesh == 0 )
	{
		return;
	}

	if ( m_scene->mesh[0].interleaved == 0 )
	{
		kdLogMessagefKHR( "Please re-export with the interleaved option enabled." );
	}

	m_vertices = new GLuint[ m_scene->num_mesh ];	
	if ( !m_vertices )
	{
		return;
	}

	m_indices = new GLuint[ m_scene->num_mesh ];
	if ( !m_indices )
	{
		return;
	}

	glGenBuffers( m_scene->num_mesh, m_vertices );

	for ( i = 0; i < m_scene->num_mesh; ++i )
	{		
		mesh = &m_scene->mesh[i];
		size = mesh->num_vertex * mesh->vertex.stride;

		glBindBuffer( GL_ARRAY_BUFFER, m_vertices[i] );
		glBufferData( GL_ARRAY_BUFFER, size, mesh->interleaved, GL_STATIC_DRAW );

		m_indices[i] = 0;

		if ( mesh->faces.data )
		{
			glGenBuffers ( 1, &m_indices[i] );
			size = PODCountIndices( *mesh ) * sizeof( GLshort );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_indices[i] );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, mesh->faces.data, GL_STATIC_DRAW );
		}
	}

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

XMGPOD::XMGPOD(void) : m_impl( new XMGPODImpl() )
{

}

XMGPOD::~XMGPOD(void)
{
	delete (XMGPODImpl *) m_impl;
}

XMGPOD* XMGPOD::Create(KDchar* path)
{
	XMGPOD*		pod  = 0;	
	KDFile*		file = 0;
	KDuint32	name = 0;
	KDuint32	len  = 0;
	
	KDchar*		ver	 = 0;
	KDboolean	done = KD_FALSE;
	KDboolean	need_options = KD_FALSE;
	KDboolean	need_history = KD_FALSE;
	KDboolean	load_options_or_history = need_options | need_history;

	pod = new XMGPOD();
	if ( !pod )
	{		
		goto failed;
	}

	file = kdFopen( path, "rb+" );
	if ( !file )
	{
		goto failed;
	}

	while ( IMPL_POD->ReadMarker( file, name, len ) == XMG_TRUE )
	{
		switch ( name )
		{
			case ePODFileVersion :

				if ( len != kdStrlen( PVRTMODELPOD_VERSION ) + 1 )
				{
					goto failed;
				}

				if ( !( ver = (KDchar *) kdMalloc( len ) ) )
				{
					goto failed;
				}

				if ( !kdFread( ver, len, 1, file ) )
				{
					goto failed;
				}

				if ( kdStrcmp( ver, PVRTMODELPOD_VERSION ) != 0 )
				{
					goto failed;
				}			

				kdFree( ver );
				ver = 0;

				continue;

			case ePODFileScene :

				if ( IMPL_POD->ReadScene( file, *IMPL_POD->m_scene ) == XMG_FALSE )
				{
					goto failed;
				}

				done = KD_TRUE;

				continue;

			case ePODFileExpOpt :

				if ( need_options )
				{
					need_options = KD_FALSE;
				}

				break;

			case ePODFileHistory :

				break;

			case ePODFileScene | PVRTMODELPOD_TAG_END :

				if ( done == KD_TRUE )
				{
					goto success;
				}

				break;

			case ePODFileEndiannessMisMatch :

				goto failed;
		}

		if ( load_options_or_history && !need_options && !need_history )
		{
			break;
		}
		
		if ( IMPL_POD->ReadSkip( file, len ) == XMG_FALSE )		
		{
			goto failed;
		}
	}

	if ( load_options_or_history )
	{
		goto success;
	}

success :

	IMPL_POD->SetVertices();
	
	return pod;

failed :

	if ( ver )
	{
		kdFree( ver );
	}

	if ( pod )
	{
		delete pod;
	}

	return 0;
}

void XMGPOD::Free(XMGPOD *pod)
{
	delete pod;
}

void XMGPOD::Render(void)
{
	SPODScene*	scene  = 0;
	//SPODMesh*	mesh   = 0;
	//SPODNode*	node   = 0;
	KDint32		cam_id = 0;

	XMGVector3F	from;
	XMGVector3F to( 1.0f );
	XMGVector3F	up( 0.0f, 1.0f, 0.0f );

	//VERTTYPE	fov = 0;
	//KDuint32	i;

	( (XMGRenderImpl *) XMGRender::m_impl )->Begin();

	scene = IMPL->m_scene;

	cam_id = scene->node[ scene->num_mesh_node + scene->num_light ].index;

	if ( scene->camera[ cam_id ].idx_target != -1 )
	{
		IMPL->GetCameraPos( from, to, 0 );
	}
	else
	{

	}
	/*
	glEnableClientState(GL_VERTEX_ARRAY);

	for ( i = 0; i < scene->num_mesh_node; i++ )
	{
		node = &scene->node[ i ];
		mesh = &scene->mesh[ node->index ];

		glBindBuffer( GL_ARRAY_BUFFER, IMPL->m_vertices[ node->index ] );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IMPL->m_indices[ node->index ] );

		glVertexPointer( mesh->vertex.number, VERTTYPEENUM, mesh->vertex.stride, mesh->vertex.data );

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

		if ( mesh->num_strips == 0 )
		{
			if ( IMPL->m_indices[ node->index ] )
			{
				glDrawElements( GL_TRIANGLES, mesh->num_faces * 3, GL_UNSIGNED_SHORT, 0 );
			}
			else
			{
				glDrawArrays( GL_TRIANGLES, 0, mesh->num_faces * 3 );
			}
		}

	}

	*/

	( (XMGRenderImpl *) XMGRender::m_impl )->End();
}

VERTTYPE XMGPODImpl::GetCameraPos(XMGVector3F& from, XMGVector3F& to, const KDuint32 idx) const
{
	XMGMatrix4F		matrix;
	//VERTTYPE*		data;
	//SPODCamera*		camera;
	SPODNode*		node;

	node = &m_scene->node[ m_scene->num_mesh_node + m_scene->num_light + idx ];

	//GetWorldMatrix( matrix, node );

	return 0;
}

void XMGPODImpl::GetWorldMatrix(XMGMatrix4F& out, const SPODNode& node) const
{
	KDuint32	index;

	index = (KDuint32) ( &node - m_scene->node );



}
