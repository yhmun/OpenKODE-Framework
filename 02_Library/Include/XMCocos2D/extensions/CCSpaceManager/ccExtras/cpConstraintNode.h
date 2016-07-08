/* --------------------------------------------------------------------------
 *
 *      File            cpConstraintNode.h
 *      Description     Provide Drawing for Constraints
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2009 Robert Blackwood 02/22/2009
 *
 *         Mobile Bros. All rights reserved.
 *
 *         http://www.mobile-bros.com  
 *
 * --------------------------------------------------------------------------
 *      
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

#ifndef __cpConstraintNode_h__
#define __cpConstraintNode_h__

#include "../../../base_nodes/CCNode.h"
#include "../../../CCProtocols.h"
#include "XMChipmunk/chipmunk.h"

NS_CC_BEGIN

class CCSpaceManager;

/** GL pre draw state for cpConstraintNode */
KDvoid		cpConstraintNodePreDrawState ( KDvoid );

/** GL post draw state for cpConstraintNode */
KDvoid		cpConstraintNodePostDrawState ( KDvoid );

/** Draw a constraint with the correct pre/post states */
KDvoid		cpConstraintNodeDraw ( cpConstraint* pConstraint );

/** Draw a constraint without the pre/post states. Use the pre/post calls above to draw many constraints at once */
KDvoid		cpConstraintNodeEfficientDraw ( cpConstraint* pConstraint );

class cpConstraintNode : public CCNode, public CCRGBAProtocol
{
	public :

		virtual ~cpConstraintNode ( KDvoid );

		static cpConstraintNode*	create ( cpConstraint* pConstraint );

	public :

		virtual KDbool		initWithConstraint ( cpConstraint* pConstraint );

		virtual KDvoid		draw ( KDvoid );

	public :

		CC_SYNTHESIZE ( cpConstraint*, m_pConstraint, Constraint );

		CC_SYNTHESIZE_BOOL ( m_bAutoFreeConstraint, AutoFreeConstraint );

		CC_SYNTHESIZE ( CCSpaceManager*, m_pSpaceManager, CCSpaceManager );

		CC_SYNTHESIZE ( cpFloat, m_fPointSize, PointSize );

		CC_SYNTHESIZE ( cpFloat, m_fLineWidth, LineWidth );

		CC_SYNTHESIZE_BOOL ( m_bSmoothDraw, SmoothDraw );

		virtual KDbool containsPoint ( cpVect tPoint, cpFloat fPadding );

		CC_SYNTHESIZE_BOOL ( m_bOpacityModifyRGB, OpacityModifyRGB );

	private :

		virtual KDbool		containsPoint ( cpVect tPoint, cpFloat fPadding, cpConstraint* pConstraint );
};

NS_CC_END

#endif	// __cpConstraintNode_h__