/* --------------------------------------------------------------------------
 *
 *      File            M3GInternal.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 UEDA.Takashi
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __M3GInternal_h__
#define __M3GInternal_h__

#include <kode.h>
#include <vector>

#include "M3GVector.h"
#include "M3GMatrix.h"
#include "M3GQuaternion.h"
#include "M3GKeyframe.h"

class M3GCamera;
class M3GNode;

class M3GRenderState
{
	public :

		M3GRenderState ( KDvoid )
		{
			m_nPass						= 0;
			m_nLightIndex				= 0;
			m_nLayer					= -999;
			m_fAlpha					= 1;
			m_bVertexColorBufferReady	= KD_FALSE;
			m_nDefaultVertexColor		= 0xffffffff;
		}

		KDint					m_nPass;
		KDint					m_nLightIndex;
		KDint					m_nLayer;
		KDfloat					m_fAlpha;
		KDbool					m_bVertexColorBufferReady;
		KDint					m_nDefaultVertexColor;
		std::vector<KDint>		m_vLayers;
		const M3GCamera*		m_pCamera;
};

class M3GBoneWeight 
{
	public :

		M3GBoneWeight ( KDint index, KDint weight ) 
		{
			m_nIndex  = index;
			m_nWeight = weight;
		};

		KDint					m_nIndex;
		KDint					m_nWeight;
};

class M3GBindPose 
{
	public :

		M3GBindPose ( M3GNode* node, const M3GMatrix& matrix )
		{
			m_pBone = node;
			m_tInverse = matrix;
		};
		
		M3GNode*				m_pBone;
		M3GMatrix				m_tInverse;
};

KDvoid M3GException ( const KDchar* szName, const KDchar* szFunc, const KDchar* szFormat, ... );

KDbool M3GTriangleIntersect ( const M3GVector& org, const M3GVector& dir, 
                              const M3GVector& v0, const M3GVector& v1, const M3GVector& v2,
                              KDfloat* u_, KDfloat* v_, KDfloat* t_ );

#endif