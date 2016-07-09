#include "3d/Base.h"
#include "3d/C3DAnimation.h"
#include "3d/C3DAnimationChannel.h"

#include "3d/C3DAnimationCurve.h"
#include "3d/C3DBone.h"

namespace cocos3d
{

C3DAnimationChannel::C3DAnimationChannel(C3DAnimation* animation, C3DBone* bone, C3DAnimationCurve* curve, unsigned long duration)
    : _animation(animation), _bone(bone),  _curve(curve), _duration(duration)
{
	_curve->retain();
    _bone->addChannel(this);
    _animation->retain();
}


C3DAnimationChannel::~C3DAnimationChannel()
{
    SAFE_RELEASE(_curve);
    SAFE_RELEASE(_animation);
}

C3DAnimationCurve* C3DAnimationChannel::getCurve() const
{
    return _curve;
}


}
