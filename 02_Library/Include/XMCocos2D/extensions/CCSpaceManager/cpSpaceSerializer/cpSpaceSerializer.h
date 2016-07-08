/* --------------------------------------------------------------------------
 *
 *      File            cpSpaceSerializer.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010 Robert Blackwood on 04/08/2010.
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

#ifndef __cpSpaceSerializer_h__
#define __cpSpaceSerializer_h__

//#include "tinyxml/tinyxml.h"

#include "XMChipmunk/chipmunk.h"
#include <map>

#define CPSS_DEFAULT_MAKE_ID(__PTR__)  ( reinterpret_cast<KDint> ( __PTR__ ) )

#if 0

class cpSpaceSerializerDelegate
{
public:
	virtual long	makeId(cpShape* shape) {return CPSS_DEFAULT_MAKE_ID(shape);}

    /*! 0 is reserved for the id of space->staticBody (for now at least) */
    virtual long	makeId(cpBody* body) {return CPSS_DEFAULT_MAKE_ID(body);}
    virtual long	makeId(cpConstraint* constraint) {return CPSS_DEFAULT_MAKE_ID(constraint);}
	
	virtual bool	writing(cpShape *shape, long shapeId) {return true;}
	virtual bool	writing(cpBody *body, long bodyId) {return true;}
	virtual bool	writing(cpConstraint *constraint, long constraintId) {return true;}
	
	virtual bool reading(cpShape *shape, long shapeId) {return true;}
	virtual bool reading(cpBody *body, long bodyId) {return true;}
	virtual bool reading(cpConstraint *constraint, long constraintId) {return true;}
};

class cpSpaceSerializer 
{
public:
	cpSpaceSerializer() : delegate(NULL) {}
	cpSpaceSerializer(cpSpaceSerializerDelegate *delegate) : delegate(delegate) {}
	
	cpSpace* load(const char* filename);
	cpSpace* load(cpSpace *space, const char* filename);
	bool save(cpSpace* space, const char* filename);
	
	cpSpaceSerializerDelegate *delegate;
		
	//Reading
	virtual cpShape *createShape(TiXmlElement *elm);
	cpShape *createCircle(TiXmlElement *elm);
	cpShape *createSegment(TiXmlElement *elm);
	cpShape *createPoly(TiXmlElement *elm);
	
	cpBody *createBody(TiXmlElement *elm);
	
	void createBodies(TiXmlElement *elm, cpBody **a, cpBody **b);
	virtual cpConstraint *createConstraint(TiXmlElement *elm);
	cpConstraint *createPinJoint(TiXmlElement *elm);
	cpConstraint *createSlideJoint(TiXmlElement *elm);
	cpConstraint *createPivotJoint(TiXmlElement *elm);
	cpConstraint *createGrooveJoint(TiXmlElement *elm);
	cpConstraint *createMotorJoint(TiXmlElement *elm);
	cpConstraint *createGearJoint(TiXmlElement *elm);
	cpConstraint *createSpringJoint(TiXmlElement *elm);
	cpConstraint *createRotaryLimitJoint(TiXmlElement *elm);
	cpConstraint *createRatchetJoint(TiXmlElement *elm);
	cpConstraint *createRotarySpringJoint(TiXmlElement *elm);
	
	cpVect createPoint(const char* name, TiXmlElement *elm);
	
	//Writing
	virtual TiXmlElement *createShapeElm(cpShape *shape);
	TiXmlElement *createCircleElm(cpShape *shape);
	TiXmlElement *createSegmentElm(cpShape *shape);
	TiXmlElement *createPolyElm(cpShape *shape);

	TiXmlElement *createBodyElm(cpBody *body);
	
	virtual TiXmlElement *createConstraintElm(cpConstraint *constraint);
	TiXmlElement *createPinJointElm(cpPinJoint *constraint);
	TiXmlElement *createSlideJointElm(cpSlideJoint *constraint);
	TiXmlElement *createPivotJointElm(cpPivotJoint *constraint);
	TiXmlElement *createGrooveJointElm(cpGrooveJoint *constraint);
	TiXmlElement *createMotorJointElm(cpSimpleMotor *constraint);
	TiXmlElement *createGearJointElm(cpGearJoint *constraint);
	TiXmlElement *createSpringJointElm(cpDampedSpring *constraint);
	TiXmlElement *createRotaryLimitJointElm(cpRotaryLimitJoint *constraint);
	TiXmlElement *createRatchetJointElm(cpRatchetJoint *constraint);
	TiXmlElement *createRotarySpringJointElm(cpDampedRotarySpring *constraint);
	
	TiXmlElement *createPointElm(const char* name, cpVect pt);
    
    typedef std::map<long, cpBody*> BodyMap;
    typedef std::map<long, cpShape*> ShapeMap;

    BodyMap& bodyMap() { return _bodyMap; }
    ShapeMap& shapeMap() { return _shapeMap; }

private:
    TiXmlDocument   _doc;
    cpSpace         *_space;
    
    //For Resolving on read and avoiding redundancies on write    
    BodyMap     _bodyMap;
    ShapeMap    _shapeMap;
};

#endif // 0

#endif // __cpSpaceSerializer_h__
