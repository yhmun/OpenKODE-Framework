/* --------------------------------------------------------------------------
 *
 *      File            cpConstraintNode.cpp
 *      Description     Provide Drawing for Constraints
 *      Author          Y.H Mun
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
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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

#include "Precompiled.h"
#include "extensions/CCSpaceManager/ccExtras/cpConstraintNode.h"

#if 0

NS_CC_BEGIN

/*
static KDvoid drawCircle ( cpVect tCenter, KDfloat r, KDint segs )
{
	const float coef = 2.0f * (float)M_PI/segs;
	float *vertices = malloc(sizeof(float)*2*segs);
	
	for (int i=0;i<segs;i++)
	{
		float rads = i*coef;
		float j = r * cosf(rads) + center.x;
		float k = r * sinf(rads) + center.y;
		
		vertices[i*2] = j;
		vertices[i*2+1] =k;
	}
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);	
	glDrawArrays(GL_LINE_LOOP, 0, segs);
	
	free(vertices);
}

static void drawPinJoint(cpPinJoint* joint, cpBody* body_a, cpBody* body_b)
{	
	cpVect a = cpvmult(cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot)), CC_CONTENT_SCALE_FACTOR());
	cpVect b = cpvmult(cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot)), CC_CONTENT_SCALE_FACTOR());
	
	const float array[] = { a.x, a.y, b.x, b.y };
	
	glVertexPointer(2, GL_FLOAT, 0, array);
	//glEnableClientState(GL_VERTEX_ARRAY);
	
	glDrawArrays(GL_POINTS, 0, 2);
	glDrawArrays(GL_LINES, 0, 2);
	
	//glDisableClientState(GL_VERTEX_ARRAY);	
}

static void drawSlideJoint(cpSlideJoint* joint, cpBody* body_a, cpBody* body_b)
{	
	cpVect a = cpvmult(cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot)), CC_CONTENT_SCALE_FACTOR());
	cpVect b = cpvmult(cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot)), CC_CONTENT_SCALE_FACTOR());
	
	const float array[] = { a.x, a.y, b.x, b.y };
	
	glVertexPointer(2, GL_FLOAT, 0, array);
	//glEnableClientState(GL_VERTEX_ARRAY);
	
	glDrawArrays(GL_POINTS, 0, 2);
	glDrawArrays(GL_LINES, 0, 2);
	
	//glDisableClientState(GL_VERTEX_ARRAY);	
}

static void drawPivotJoint(cpPivotJoint* joint, cpBody* body_a, cpBody* body_b)
{	
	cpVect a = cpvmult(cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot)), CC_CONTENT_SCALE_FACTOR());
	cpVect b = cpvmult(cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot)), CC_CONTENT_SCALE_FACTOR());
	
	const float array[] = { a.x, a.y, b.x, b.y };
	
	glVertexPointer(2, GL_FLOAT, 0, array);
    
	//glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, 2);	
	glDrawArrays(GL_LINES, 0, 2);
	//glDisableClientState(GL_VERTEX_ARRAY);	
}

static void drawGrooveJoint(cpGrooveJoint* joint, cpBody* body_a, cpBody* body_b)
{
	cpVect a = cpvmult(cpvadd(body_a->p, cpvrotate(joint->grv_a, body_a->rot)), CC_CONTENT_SCALE_FACTOR());
	cpVect b = cpvmult(cpvadd(body_a->p, cpvrotate(joint->grv_b, body_a->rot)), CC_CONTENT_SCALE_FACTOR());
    
	cpVect grv = cpvmult(cpvadd(body_a->p, cpvrotate(joint->r1, body_a->rot)), CC_CONTENT_SCALE_FACTOR());
	
	float groove[6];
	groove[0] = a.x;
	groove[1] = a.y;
	groove[2] = b.x;
	groove[3] = b.y;	
	groove[4] = grv.x;
	groove[5] = grv.y;
    
	//glEnableClientState(GL_VERTEX_ARRAY);
    
	glVertexPointer(2, GL_FLOAT, 0, groove);
	glDrawArrays(GL_POINTS, 0, 3);
	glDrawArrays(GL_LINES, 0, 2);
    
	//glDisableClientState(GL_VERTEX_ARRAY);	
}

static void drawSpringJoint(cpDampedSpring* joint, cpBody* body_a, cpBody* body_b)
{
	static const GLfloat springVAR[] = {
		0.00f, 0.0f,
		0.20f, 0.0f,
		0.25f, 3.0f,
		0.30f,-6.0f,
		0.35f, 6.0f,
		0.40f,-6.0f,
		0.45f, 6.0f,
		0.50f,-6.0f,
		0.55f, 6.0f,
		0.60f,-6.0f,
		0.65f, 6.0f,
		0.70f,-3.0f,
		0.75f, 6.0f,
		0.80f, 0.0f,
		1.00f, 0.0f,
	};
	static const int springVAR_count = sizeof(springVAR)/sizeof(GLfloat)/2;
	
	cpVect a = cpvmult(cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot)), CC_CONTENT_SCALE_FACTOR());
	cpVect b = cpvmult(cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot)), CC_CONTENT_SCALE_FACTOR());
	
	const float array[] = { a.x, a.y, b.x, b.y };
	
	glVertexPointer(2, GL_FLOAT, 0, array);
	
	//glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, 2);	
	
	cpVect delta = cpvsub(b, a);
	
	glVertexPointer(2, GL_FLOAT, 0, springVAR);
	glPushMatrix(); {
		GLfloat x = a.x;
		GLfloat y = a.y;
		GLfloat cos = delta.x;
		GLfloat sin = delta.y;
		GLfloat s = 1.0f/cpvlength(delta);
		
		const GLfloat matrix[] = {
			cos,	sin,	0.0f, 0.0f,
			-sin*s, cos*s,	0.0f, 0.0f,
			0.0f,	0.0f,	1.0f, 1.0f,
			x,		y,		0.0f, 1.0f,
		};
		
		glMultMatrixf(matrix);
		glDrawArrays(GL_LINE_STRIP, 0, springVAR_count);
	} glPopMatrix();
	
	//glDisableClientState(GL_VERTEX_ARRAY);	
}

static KDvoid drawMotorJoint ( cpSimpleMotor* pJoint, cpBody* pBodyA, cpBody* pBodyB )
{
	ccDrawCircle ( pBodyA->p, 4.0f, 0, 12, KD_FALSE );
}

static void drawGearJoint(cpGearJoint* joint, cpBody* body_a, cpBody* body_b)
{
	cpFloat ratio = joint->ratio;
	
	cpFloat radius1 = fabs(5.0 / ratio) * CC_CONTENT_SCALE_FACTOR();
	cpFloat radius2 = fabs(5.0 * ratio) * CC_CONTENT_SCALE_FACTOR();
    
	cpVect a_pos = cpvmult(body_a->p, CC_CONTENT_SCALE_FACTOR());
	cpVect b_pos = cpvmult(body_b->p, CC_CONTENT_SCALE_FACTOR());
    
	drawCircle(a_pos, radius1, radius1*2+3);
	drawCircle(b_pos, radius2, radius2*2+3);
	
	cpVect a = cpv(0,radius1);
	cpVect b = cpv(0,radius2);
	cpVect c = cpv(0,-radius1);
	cpVect d = cpv(0,-radius2);
	
	float dx = a_pos.x - b_pos.x;
	float dy = a_pos.y - b_pos.y;
	cpVect rotation = cpvforangle(atan2f(dy,dx));	
	
	a = cpvadd(a_pos, cpvrotate(a,rotation));
	b = cpvadd(b_pos, cpvrotate(b,rotation));
	c = cpvadd(a_pos, cpvrotate(c,rotation));
	d = cpvadd(b_pos, cpvrotate(d,rotation));
	
	float array[8];
	array[0] = a.x;
	array[1] = a.y;
	array[4] = c.x;
	array[5] = c.y;
    
	if (ratio >= 0)
	{
		array[2] = b.x;
		array[3] = b.y;
		array[6] = d.x;
		array[7] = d.y;
	}
	else 
	{
		array[2] = d.x;
		array[3] = d.y;
		array[6] = b.x;
		array[7] = b.y;
	}
    
	glVertexPointer(2, GL_FLOAT, 0, array);
	
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glDrawArrays(GL_POINTS, 0, 4);	
	glDrawArrays(GL_LINES, 0, 4);
	//glDisableClientState(GL_VERTEX_ARRAY);	
}

static void drawPulleyJoint(cpPulleyJoint* joint, cpBody* body_a, cpBody* body_b)
{
	cpBody *body_c = joint->c;
	
	cpVect a = cpvmult(cpBodyLocal2World(body_a, joint->anchr1), CC_CONTENT_SCALE_FACTOR());
	cpVect b = cpvmult(cpBodyLocal2World(body_b, joint->anchr2), CC_CONTENT_SCALE_FACTOR());
	cpVect c = cpvmult(cpBodyLocal2World(body_c, joint->anchr3a), CC_CONTENT_SCALE_FACTOR());
	cpVect d = cpvmult(cpBodyLocal2World(body_c, joint->anchr3b), CC_CONTENT_SCALE_FACTOR());
	
	float array[] = {a.x,a.y,c.x,c.y,b.x,b.y,d.x,d.y};
	
	glVertexPointer(2, GL_FLOAT, 0, array);
	
	glDrawArrays(GL_POINTS, 0, 4);
	glDrawArrays(GL_LINES, 0, 4);
}
*/
KDvoid cpConstraintNodeEfficientDraw ( cpConstraint* pConstraint )
{
	/*
    cpBody* body_a = constraint->a;
	cpBody* body_b = constraint->b;

    const cpConstraintClass *klass = pConstraint->CP_PRIVATE ( klass );
    
	if(klass == cpPinJointGetClass())
		drawPinJoint((cpPinJoint*)constraint, body_a, body_b); 
	else if(klass == cpSlideJointGetClass())
		drawSlideJoint((cpSlideJoint*)constraint, body_a, body_b);  
	else if(klass == cpPivotJointGetClass())
		drawPivotJoint((cpPivotJoint*)constraint, body_a, body_b); 
	else if(klass == cpGrooveJointGetClass())
		drawGrooveJoint((cpGrooveJoint*)constraint, body_a, body_b); 
	else if (klass == cpSimpleMotorGetClass())
		drawMotorJoint((cpSimpleMotor*)constraint, body_a, body_b); 
	else if (klass == cpGearJointGetClass())
		drawGearJoint((cpGearJoint*)constraint, body_a, body_b); 
	else if(klass == cpDampedSpringGetClass())
		drawSpringJoint((cpDampedSpring *)constraint, body_a, body_b); 
	else if (klass == cpPulleyJointGetClass())
		drawPulleyJoint((cpPulleyJoint*)constraint, body_a, body_b);
		*/
}

KDvoid cpConstraintNodePreDrawState ( KDvoid )
{
    glDisableClientState ( GL_COLOR_ARRAY );
    glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
    glDisable ( GL_TEXTURE_2D );
}

KDvoid cpConstraintNodePostDrawState ( KDvoid )
{
    glEnableClientState ( GL_COLOR_ARRAY );
	glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
	glEnable ( GL_TEXTURE_2D );
}

KDvoid cpConstraintNodeDraw ( cpConstraint* pConstraint )
{
    cpConstraintNodePreDrawState  ( );
    cpConstraintNodeEfficientDraw ( pConstraint );
    cpConstraintNodePostDrawState ( );
}

cpConstraintNode* cpConstraintNode::create ( cpConstraint* pConstraint )
{
	cpConstraintNode*  pRet = new cpConstraintNode ( );
	
	if ( pRet && pRet->initWithConstraint ( pConstraint ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool cpConstraintNode::initWithConstraint ( cpConstraint* pConstraint )
{	
	m_pConstraint	= pConstraint;
	m_tColor		= ccBLACK;
	m_cOpacity		= 255;

	m_fPointSize	= 3;
	m_fLineWidth	= 1;
	m_bSmoothDraw	= KD_TRUE;

	if ( pConstraint )
	{
		pConstraint->data = this;
	}
	
	return KD_TRUE;
}

cpConstraintNode::~cpConstraintNode ( KDvoid )
{
	if ( m_bAutoFreeConstraint )
	{
		m_pSpaceManager->removeAndFreeConstraint ( m_pConstraint );
	}
}

KDbool cpConstraintNode::containsPoint ( cpVect tPoint, cpFloat fPadding )
{
	return this->containsPoint ( tPoint, fPadding, m_pConstraint );
}

KDbool cpConstraintNode::containsPoint ( cpVect tPoint, cpFloat fPadding, cpConstraint* pConstraint )
{
	KDbool  bContains = KD_FALSE;
	/*
	cpBody *body_a = constraint->a;
	cpBody *body_b = constraint->b;
	
	cpVect apt, bpt;
	
	const cpConstraintClass *klass = constraint->CP_PRIVATE(klass);
	
	if(klass == cpPinJointGetClass())
	{
		cpPinJoint *joint = (cpPinJoint*)constraint;
		
		apt = cpvmult(cpBodyLocal2World(body_a, joint->anchr1), CC_CONTENT_SCALE_FACTOR());
		bpt = cpvmult(cpBodyLocal2World(body_b, joint->anchr2), CC_CONTENT_SCALE_FACTOR());
	}
	else if(klass == cpSlideJointGetClass())
	{
		cpSlideJoint *joint = (cpSlideJoint*)constraint;
		
		apt = cpvmult(cpBodyLocal2World(body_a, joint->anchr1), CC_CONTENT_SCALE_FACTOR());
		bpt = cpvmult(cpBodyLocal2World(body_b, joint->anchr2), CC_CONTENT_SCALE_FACTOR());
	}
	else if(klass == cpPivotJointGetClass())
	{
		cpPivotJoint* joint = (cpPivotJoint*)constraint;
		
		apt = cpvmult(cpBodyLocal2World(body_a, joint->anchr1), CC_CONTENT_SCALE_FACTOR());
		bpt = cpvmult(cpBodyLocal2World(body_b, joint->anchr2), CC_CONTENT_SCALE_FACTOR());
	}
	else if(klass == cpGrooveJointGetClass())
	{
		cpGrooveJoint *joint = (cpGrooveJoint*)constraint;
		
		apt = cpvmult(cpBodyLocal2World(body_a, joint->grv_a), CC_CONTENT_SCALE_FACTOR());
		bpt = cpvmult(cpBodyLocal2World(body_a, joint->grv_b), CC_CONTENT_SCALE_FACTOR());
	}
	//else if(klass == cpBreakableJointGetClass())
	//{
		//This works... but it uses the assumption that data is a cpConstraintNode
		//cpBreakableJoint *joint = (cpBreakableJoint*)constraint;
		//return [(cpConstraintNode*)joint->delegate->data containsPoint:pt padding:padding constraint:joint->delegate];
	//}
	else if (klass == cpSimpleMotorGetClass())
	{
		apt = bpt = cpvmult(body_a->p, CC_CONTENT_SCALE_FACTOR());
	}
	else if (klass == cpGearJointGetClass())
	{
		cpGearJoint *joint = (cpGearJoint*)constraint;
		
		cpFloat ratio = joint->ratio;
		
		cpFloat radius1 = 5.0 / ratio * CC_CONTENT_SCALE_FACTOR();
		cpFloat radius2 = 5.0 * ratio * CC_CONTENT_SCALE_FACTOR();

		// pad it out by radius (half diameter as the biggest gear...)
		padding += (radius1 > radius2) ? radius1 : radius2;
		
		apt = cpvmult(body_a->p, CC_CONTENT_SCALE_FACTOR());
		bpt = cpvmult(body_b->p, CC_CONTENT_SCALE_FACTOR());
	}
	else if(klass == cpDampedSpringGetClass())
	{
		padding += 20; // 20 is width of spring
		cpDampedSpring *joint = (cpDampedSpring*)constraint;
		
		apt = cpvmult(cpBodyLocal2World(body_a, joint->anchr1), CC_CONTENT_SCALE_FACTOR());
		bpt = cpvmult(cpBodyLocal2World(body_b, joint->anchr2), CC_CONTENT_SCALE_FACTOR());
	}
	else
		return NO;
		
	cpFloat width = (_pointSize > _lineWidth) ? _pointSize : _lineWidth;
	width += padding*2*CC_CONTENT_SCALE_FACTOR();
	
	cpFloat length = cpvlength(cpvsub(bpt, apt)) + padding*2*CC_CONTENT_SCALE_FACTOR();
	cpVect halfpt = cpvadd(apt, cpvmult(cpvsub(bpt, apt), 0.5f));
	
	// Algorithm Explained
	// 
	//	At this point we have 2 pts (in some cases they are the same point).
	//	From these two points we construct a flat (rotation zero) rectangle, we
	//	then rotate the point in question by the same rotation it would take to make 
	//	the actual bounding rect "flat". Rotation is always around the center (halfpt)
	//	of the rect.
	//
	
	float dx = apt.x - bpt.x;
	float dy = apt.y - bpt.y;
	float rotation = atan2f(dy,dx);	
		
	//recalc pt (non-rotated)
	pt = cpvadd(halfpt, cpvrotate(cpvsub(pt,halfpt), cpvforangle(-rotation)));
	
	contains = CGRectContainsPoint(CGRectMake(halfpt.x-length/2.0, halfpt.y-width/2.0, length, width), CGPointMake(pt.x,pt.y));
	*/

	return bContains;
}

KDvoid cpConstraintNode::draw ( KDvoid )
{
	ccPointSize ( m_fPointSize );
	ccLineWidth ( m_fLineWidth );
	
	ccDrawColor4B ( m_tColor.r, m_tColor.g, m_tColor.b, m_cOpacity );
				
    cpConstraintNodeDraw ( m_pConstraint );
} 

NS_CC_END

#endif