#ifndef _PARALLAX_TEST_H_
#define _PARALLAX_TEST_H_

#include "../testBasic.h"
#include "../BaseTest.h"
#include "extensions/cocos-ext.h"

USING_NS_CC_EXT;

class ParallaxDemo : public BaseTest
{
protected:
    TextureAtlas* _atlas;

public:
    ParallaxDemo(void);
    ~ParallaxDemo(void);

    virtual std::string title();
    virtual void onEnter();

    void restartCallback(Object* sender);
    void nextCallback(Object* sender);
    void backCallback(Object* sender);
};

class Parallax1 : public ParallaxDemo
{
protected:
    Node*        _root;
    Node*        _target;
    MotionStreak*        _streak;

public:
    Parallax1();
    virtual std::string title();
};

class Parallax2 : public ParallaxDemo
{
protected:
    Node*        _root;
    Node*        _target;
    MotionStreak*        _streak;

public:
    Parallax2();
    
    void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);

    virtual std::string title();
};

class ParallaxScroll : public BaseTest
{
	public :

		ParallaxScroll ( KDvoid );

		KDvoid		update ( KDfloat fDelta );

	private :

		ParallaxScrollNode*		m_pParallax;
};

class ParallaxTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

#endif
