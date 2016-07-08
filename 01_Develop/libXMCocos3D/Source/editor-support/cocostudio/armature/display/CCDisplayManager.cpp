/* -----------------------------------------------------------------------------------
 *
 *      File            CCDisplayManager.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#include "editor-support/cocostudio/armature/display/CCDisplayManager.h"
#include "editor-support/cocostudio/armature/CCBone.h"
#include "editor-support/cocostudio/armature/CCArmature.h"
#include "editor-support/cocostudio/armature/utils/CCUtilMath.h"
#include "editor-support/cocostudio/armature/display/CCSkin.h"
#include "2d/particle_nodes/CCParticleSystemQuad.h"

using namespace cocos2d;

namespace cocostudio {

DisplayManager *DisplayManager::create(Bone *bone)
{
    DisplayManager *pDisplayManager = new DisplayManager();
    if (pDisplayManager && pDisplayManager->init(bone))
    {
        pDisplayManager->autorelease();
        return pDisplayManager;
    }
    CC_SAFE_DELETE(pDisplayManager);
    return nullptr;
}


DisplayManager::DisplayManager()
    : m_pDecoDisplayList(nullptr)
    , m_pDisplayRenderNode(nullptr)
    , m_eDisplayType(CS_DISPLAY_MAX)
    , m_pCurrentDecoDisplay(nullptr)
    , m_nDisplayIndex(-1)
    , m_bForceChangeDisplay(false)
    , m_bVisible(true)
    , m_pBone(nullptr)
{
}

DisplayManager::~DisplayManager()
{
    CC_SAFE_DELETE(m_pDecoDisplayList);

    if( m_pDisplayRenderNode )
    {
        m_pDisplayRenderNode->removeFromParentAndCleanup(true);
        if(m_pDisplayRenderNode->retainCount() > 0)
            CC_SAFE_RELEASE(m_pDisplayRenderNode);
    }

}

bool DisplayManager::init(Bone *bone)
{
    bool ret = false;

    do
    {

        m_pBone = bone;

        initDisplayList(bone->getBoneData());

        ret = true;
    }
    while (0);

    return ret;
}


void DisplayManager::addDisplay(DisplayData *displayData, int index)
{
    DecorativeDisplay *decoDisplay = nullptr;

    if( (index >= 0) && (index < m_pDecoDisplayList->count()) )
    {
        decoDisplay = (DecorativeDisplay *)m_pDecoDisplayList->getObjectAtIndex(index);
    }
    else
    {
        decoDisplay = DecorativeDisplay::create();
        m_pDecoDisplayList->addObject(decoDisplay);
    }

    DisplayFactory::addDisplay(m_pBone, decoDisplay, displayData);

    //! if changed display index is current display index, then change current display to the new display
    if(index == m_nDisplayIndex)
    {
        m_nDisplayIndex = -1;
        changeDisplayByIndex(index, false);
    }
}

void DisplayManager::addDisplay(Node *display, int index)
{
    DecorativeDisplay *decoDisplay = nullptr;

    if( (index >= 0) && (index < m_pDecoDisplayList->count()) )
    {
        decoDisplay = (DecorativeDisplay *)m_pDecoDisplayList->getObjectAtIndex(index);
    }
    else
    {
        decoDisplay = DecorativeDisplay::create();
        m_pDecoDisplayList->addObject(decoDisplay);
    }

    DisplayData *displayData = nullptr;
    if (Skin *skin = dynamic_cast<Skin *>(display))
    {
        skin->setBone(m_pBone);
        displayData = SpriteDisplayData::create();

        DisplayFactory::initSpriteDisplay(m_pBone, decoDisplay, skin->getDisplayName().c_str(), skin);

        if (SpriteDisplayData *spriteDisplayData = (SpriteDisplayData *)decoDisplay->getDisplayData())
        {
            skin->setSkinData(spriteDisplayData->m_tSkinData);
            ((SpriteDisplayData *)displayData)->m_tSkinData = spriteDisplayData->m_tSkinData;
        }
        else
        {
            bool find = false;

            for (int i = m_pDecoDisplayList->count()-2; i>=0; i--)
            {
                DecorativeDisplay *dd = static_cast<DecorativeDisplay*>(m_pDecoDisplayList->getObjectAtIndex(i));
                SpriteDisplayData *sdd = static_cast<SpriteDisplayData*>(dd->getDisplayData());
                if (sdd)
                {
                    find = true;
                    skin->setSkinData(sdd->m_tSkinData);
                    static_cast<SpriteDisplayData*>(displayData)->m_tSkinData = sdd->m_tSkinData;
                    break;
                }
            }

            if (!find)
            {
                BaseData baseData;
                skin->setSkinData(baseData);
            }
        }
    }
    else if (dynamic_cast<ParticleSystemQuad *>(display))
    {
        displayData = ParticleDisplayData::create();

        display->removeFromParent();
        
        Armature *armature = m_pBone->getArmature();
        if (armature)
        {
            display->setParent(armature);
        }
    }
    else if(Armature *armature = dynamic_cast<Armature *>(display))
    {
        displayData = ArmatureDisplayData::create();
        armature->setParentBone(m_pBone);
    }
    else
    {
        displayData = DisplayData::create();
    }

    decoDisplay->setDisplay(display);
    decoDisplay->setDisplayData(displayData);

    //! if changed display index is current display index, then change current display to the new display
    if(index == m_nDisplayIndex)
    {
        m_nDisplayIndex = -1;
        changeDisplayByIndex(index, false);
    }
}

void DisplayManager::removeDisplay(int index)
{
    if(index == m_nDisplayIndex)
    {
        setCurrentDecorativeDisplay(nullptr);
        m_nDisplayIndex = -1;
    }

    m_pDecoDisplayList->removeObjectAtIndex(index);
}

Array *DisplayManager::getDecorativeDisplayList() const
{
    return m_pDecoDisplayList;
}

void DisplayManager::changeDisplayByIndex(int index, bool force)
{
    CCASSERT( (m_pDecoDisplayList ? index < (int)m_pDecoDisplayList->count() : true), "the _index value is out of range");

    m_bForceChangeDisplay = force;

    //! If index is equal to current display index,then do nothing
    if ( m_nDisplayIndex == index)
        return;


    m_nDisplayIndex = index;

    //! If displayIndex < 0, it means you want to hide you display
    if (m_nDisplayIndex < 0)
    {
        if(m_pDisplayRenderNode)
        {
            m_pDisplayRenderNode->removeFromParentAndCleanup(true);
            setCurrentDecorativeDisplay(nullptr);
        }
        return;
    }


    DecorativeDisplay *decoDisplay = (DecorativeDisplay *)m_pDecoDisplayList->getObjectAtIndex(m_nDisplayIndex);

    setCurrentDecorativeDisplay(decoDisplay);
}

void DisplayManager::setCurrentDecorativeDisplay(DecorativeDisplay *decoDisplay)
{
#if ENABLE_PHYSICS_BOX2D_DETECT || ENABLE_PHYSICS_CHIPMUNK_DETECT
    if (m_pCurrentDecoDisplay && m_pCurrentDecoDisplay->getColliderDetector())
    {
        m_pCurrentDecoDisplay->getColliderDetector()->setActive(false);
    }
#endif

    m_pCurrentDecoDisplay = decoDisplay;

#if ENABLE_PHYSICS_BOX2D_DETECT || ENABLE_PHYSICS_CHIPMUNK_DETECT
    if (m_pCurrentDecoDisplay && m_pCurrentDecoDisplay->getColliderDetector())
    {
        m_pCurrentDecoDisplay->getColliderDetector()->setActive(true);
    }
#endif

    Node *displayRenderNode = m_pCurrentDecoDisplay == nullptr ? nullptr : m_pCurrentDecoDisplay->getDisplay();
    if (m_pDisplayRenderNode)
    {
        if (dynamic_cast<Armature *>(m_pDisplayRenderNode) != nullptr)
        {
            m_pBone->setChildArmature(nullptr);
        }
        m_pDisplayRenderNode->removeFromParentAndCleanup(true);
        m_pDisplayRenderNode->release();
    }

    m_pDisplayRenderNode = displayRenderNode;

    if(m_pDisplayRenderNode)
    {
        if (Armature *armature = dynamic_cast<Armature *>(m_pDisplayRenderNode))
        {
            m_pBone->setChildArmature(armature);
        }
        else if (ParticleSystemQuad *particle = dynamic_cast<ParticleSystemQuad *>(m_pDisplayRenderNode))
        {
            particle->resetSystem();
        }

        if (RGBAProtocol *rgbaProtocaol = dynamic_cast<RGBAProtocol *>(m_pDisplayRenderNode))
        {
			rgbaProtocaol->setColor(m_pBone->getDisplayedColor());
			rgbaProtocaol->setOpacity(m_pBone->getDisplayedOpacity());
        }

        m_pDisplayRenderNode->retain();
        m_pDisplayRenderNode->setVisible(m_bVisible);

        m_eDisplayType = m_pCurrentDecoDisplay->getDisplayData()->m_eDisplayType;
    }
    else
    {
        m_eDisplayType =  CS_DISPLAY_MAX;
    }
}

Node *DisplayManager::getDisplayRenderNode() const
{
    return m_pDisplayRenderNode;
}


DisplayType DisplayManager::getDisplayRenderNodeType() const
{
    return m_eDisplayType;
}

int DisplayManager::getCurrentDisplayIndex() const
{
    return m_nDisplayIndex;
}

DecorativeDisplay *DisplayManager::getCurrentDecorativeDisplay() const
{
    return m_pCurrentDecoDisplay;
}

DecorativeDisplay *DisplayManager::getDecorativeDisplayByIndex( int index) const
{
    return (DecorativeDisplay *)m_pDecoDisplayList->getObjectAtIndex(index);
}

void DisplayManager::initDisplayList(BoneData *boneData)
{
    CC_SAFE_DELETE(m_pDecoDisplayList);
    m_pDecoDisplayList = Array::create();
    m_pDecoDisplayList->retain();

    CS_RETURN_IF(!boneData);

    for(auto object : boneData->m_tDisplayDataList)
    {
        DisplayData *displayData = static_cast<DisplayData *>(object);

        DecorativeDisplay *decoDisplay = DecorativeDisplay::create();
        decoDisplay->setDisplayData(displayData);

        DisplayFactory::createDisplay(m_pBone, decoDisplay);

        m_pDecoDisplayList->addObject(decoDisplay);
    }
}


bool DisplayManager::containPoint(Point &point)
{
    if(!m_bVisible || m_nDisplayIndex < 0)
    {
        return false;
    }

    bool ret = false;

    switch (m_pCurrentDecoDisplay->getDisplayData()->m_eDisplayType)
    {
    case CS_DISPLAY_SPRITE:
    {
        /*
         *  First we first check if the point is in the sprite content rect. If false, then we continue to check
         *  the contour point. If this step is also false, then we can say the bone not contain this point.
         *
         */

        Point outPoint = Point(0, 0);

        Sprite *sprite = (Sprite *)m_pCurrentDecoDisplay->getDisplay();
        sprite = (Sprite *)sprite->getChildByTag(0);

        ret = CC_SPRITE_CONTAIN_POINT_WITH_RETURN(sprite, point, outPoint);

    }
    break;

    default:
        break;
    }
    return ret;
}

bool DisplayManager::containPoint(float x, float y)
{
    Point p = Point(x, y);
    return containPoint(p);
}


void DisplayManager::setVisible(bool visible)
{
    if(!m_pDisplayRenderNode)
        return;

    m_bVisible = visible;
    m_pDisplayRenderNode->setVisible(visible);
}

bool DisplayManager::isVisible() const
{
    return m_bVisible;
}


Size DisplayManager::getContentSize() const
{
    CS_RETURN_IF(!m_pDisplayRenderNode) Size(0, 0);
    return m_pDisplayRenderNode->getContentSize();
}

Rect DisplayManager::getBoundingBox() const
{
    CS_RETURN_IF(!m_pDisplayRenderNode) Rect(0, 0, 0, 0);
    return m_pDisplayRenderNode->getBoundingBox();
}


Point DisplayManager::getAnchorPoint() const
{
    CS_RETURN_IF(!m_pDisplayRenderNode) Point(0, 0);
    return m_pDisplayRenderNode->getAnchorPoint();
}

Point DisplayManager::getAnchorPointInPoints() const
{
    CS_RETURN_IF(!m_pDisplayRenderNode) Point(0, 0);
    return m_pDisplayRenderNode->getAnchorPointInPoints();
}


}
