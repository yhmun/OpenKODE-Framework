/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionInstant.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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

#include "2d/actions/CCActionInstant.h"
#include "2d/base_nodes/CCNode.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "2d/script_support/CCScriptSupport.h"

NS_CC_BEGIN
//
// InstantAction
//
bool ActionInstant::isDone() const
{
    return true;
}

void ActionInstant::step(float dt) {
    CC_UNUSED_PARAM(dt);
    update(1);
}

void ActionInstant::update(float time) {
    CC_UNUSED_PARAM(time);
    // nothing
}

//
// Show
//

Show* Show::create() 
{
    Show* ret = new Show();

    if (ret) {
        ret->autorelease();
    }

    return ret;
}

void Show::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(true);
}

ActionInstant* Show::reverse() const
{
    return Hide::create();
}

Show * Show::clone() const
{
	// no copy constructor
	auto a = new Show();
	a->autorelease();
	return a;
}

//
// Hide
//
Hide * Hide::create() 
{
    Hide *ret = new Hide();

    if (ret) {
        ret->autorelease();
    }

    return ret;
}

void Hide::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(false);
}

ActionInstant *Hide::reverse() const
{
    return Show::create();
}

Hide * Hide::clone() const
{
	// no copy constructor
	auto a = new Hide();
	a->autorelease();
	return a;
}

//
// ToggleVisibility
//
ToggleVisibility * ToggleVisibility::create()
{
    ToggleVisibility *ret = new ToggleVisibility();

    if (ret)
    {
        ret->autorelease();
    }

    return ret;
}

void ToggleVisibility::update(float time) 
{
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(!m_pTarget->isVisible());
}

ToggleVisibility * ToggleVisibility::reverse() const
{
	return ToggleVisibility::create();
}

ToggleVisibility * ToggleVisibility::clone() const
{
	// no copy constructor
	auto a = new ToggleVisibility();
	a->autorelease();
	return a;
}

//
// Remove Self
//
RemoveSelf * RemoveSelf::create(bool isNeedCleanUp /*= true*/) 
{
	RemoveSelf *ret = new RemoveSelf();

	if (ret && ret->init(isNeedCleanUp)) {
		ret->autorelease();
	}

	return ret;
}

bool RemoveSelf::init(bool isNeedCleanUp) {
	m_bIsNeedCleanUp = isNeedCleanUp;
	return true;
}

void RemoveSelf::update(float time) {
	CC_UNUSED_PARAM(time);
	m_pTarget->removeFromParentAndCleanup(m_bIsNeedCleanUp);
}

RemoveSelf *RemoveSelf::reverse() const
{
	return RemoveSelf::create(m_bIsNeedCleanUp);
}

RemoveSelf * RemoveSelf::clone() const
{
	// no copy constructor
	auto a = new RemoveSelf();
	a->init(m_bIsNeedCleanUp);
	a->autorelease();
	return a;
}

//
// FlipX
//

FlipX *FlipX::create(bool x)
{
    FlipX *ret = new FlipX();

    if (ret && ret->initWithFlipX(x)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FlipX::initWithFlipX(bool x) {
    m_bFlipX = x;
    return true;
}

void FlipX::update(float time) {
    CC_UNUSED_PARAM(time);
    static_cast<Sprite*>(m_pTarget)->setFlippedX(m_bFlipX);
}

FlipX* FlipX::reverse() const
{
    return FlipX::create(!m_bFlipX);
}

FlipX * FlipX::clone() const
{
	// no copy constructor
	auto a = new FlipX();
	a->initWithFlipX(m_bFlipX);
	a->autorelease();
	return a;
}
//
// FlipY
//

FlipY * FlipY::create(bool y)
{
    FlipY *ret = new FlipY();

    if (ret && ret->initWithFlipY(y)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FlipY::initWithFlipY(bool y) {
    m_bFlipY = y;
    return true;
}

void FlipY::update(float time) {
    CC_UNUSED_PARAM(time);
    static_cast<Sprite*>(m_pTarget)->setFlippedY(m_bFlipY);
}

FlipY* FlipY::reverse() const
{
    return FlipY::create(!m_bFlipY);
}

FlipY * FlipY::clone() const
{
	// no copy constructor
	auto a = new FlipY();
	a->initWithFlipY(m_bFlipY);
	a->autorelease();
	return a;
}

//
// Place
//

Place* Place::create(const Point& pos)
{
    Place *ret = new Place();

    if (ret && ret->initWithPosition(pos)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Place::initWithPosition(const Point& pos) {
    m_tPosition = pos;
    return true;
}

Place * Place::clone() const
{
	// no copy constructor
	auto a = new Place();
	a->initWithPosition(m_tPosition);
	a->autorelease();
	return a;
}

Place * Place::reverse() const
{
	// no reverse, just clone
	return this->clone();
}

void Place::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->setPosition(m_tPosition);
}

//
// CallFunc
//

CallFunc * CallFunc::create(const std::function<void()> &func)
{
    CallFunc *ret = new CallFunc();

    if (ret && ret->initWithFunction(func) ) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CallFunc::initWithFunction(const std::function<void()> &func)
{
	m_pFunction = func;
    return true;
}

CallFunc::~CallFunc()
{

}

CallFunc * CallFunc::clone() const
    {
    // no copy constructor
    auto a = new CallFunc();
    if( m_pFunction ){
        a->initWithFunction(m_pFunction);
    }

    a->autorelease();
    return a;
}

CallFunc * CallFunc::reverse() const
{
	// no reverse here, just return a clone
	return this->clone();
}

void CallFunc::update(float time) {
    CC_UNUSED_PARAM(time);
    this->execute();
}

void CallFunc::execute() {
    if( m_pFunction ){
        m_pFunction();
    }
}

//
// CallFuncN
//

CallFuncN * CallFuncN::create(const std::function<void(Node*)> &func)
{
    auto ret = new CallFuncN();

    if (ret && ret->initWithFunction(func) ) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return NULL;
}

void CallFuncN::execute() {
    if (m_pFunctionN) {
        m_pFunctionN(m_pTarget);
    }
}

bool CallFuncN::initWithFunction(const std::function<void (Node *)> &func)
{
    m_pFunctionN = func;
    return true;
}

CallFuncN * CallFuncN::clone() const
{
	// no copy constructor
	auto a = new CallFuncN();

    if( m_pFunction ){
        a->initWithFunction(m_pFunctionN);
    }

	a->autorelease();
	return a;
}

NS_CC_END
