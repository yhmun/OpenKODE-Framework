/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlButton.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2011      Yannick Loriot.
 *
 *          http://www.cocos2d-x.org
 *          http://yannickloriot.com
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "extensions/GUI/CCControlExtension/CCControlButton.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
#include "2d/label_nodes/CCLabelTTF.h"
#include "2d/label_nodes/CCLabelBMFont.h"
#include "2d/actions/CCAction.h"
#include "2d/actions/CCActionInterval.h"

using namespace std;

NS_CC_EXT_BEGIN

enum
{
    kZoomActionTag = 0xCCCB0001,
};

ControlButton::ControlButton()
: m_bIsPushed(false)
, m_bParentInited(false)
, m_bDoesAdjustBackgroundImage(false)
, m_pCurrentTitle(NULL)
, m_tCurrentTitleColor(Color3B::WHITE)
, m_pTitleLabel(NULL)
, m_pBackgroundSprite(NULL)
, m_bZoomOnTouchDown(false)
, m_pTitleDispatchTable(NULL)
, m_pTitleColorDispatchTable(NULL)
, m_pTitleLabelDispatchTable(NULL)
, m_pBackgroundSpriteDispatchTable(NULL)
, m_nMarginV(ControlButtonMarginTB)
, m_nMarginH(ControlButtonMarginLR)
{

}

ControlButton::~ControlButton()
{
    CC_SAFE_RELEASE(m_pCurrentTitle);
    CC_SAFE_RELEASE(m_pTitleLabel);
    CC_SAFE_RELEASE(m_pBackgroundSpriteDispatchTable);
    CC_SAFE_RELEASE(m_pTitleLabelDispatchTable);
    CC_SAFE_RELEASE(m_pTitleColorDispatchTable);
    CC_SAFE_RELEASE(m_pTitleDispatchTable);
    CC_SAFE_RELEASE(m_pBackgroundSprite);
}

//initialisers

bool ControlButton::init()
{
    return this->initWithLabelAndBackgroundSprite(LabelTTF::create("", "Helvetica", 12), Scale9Sprite::create());
}

bool ControlButton::initWithLabelAndBackgroundSprite(Node* node, Scale9Sprite* backgroundSprite)
{
    if (Control::init())
    {
        CCASSERT(node != NULL, "Label must not be nil.");
        LabelProtocol* label = dynamic_cast<LabelProtocol*>(node);
        RGBAProtocol* rgbaLabel = dynamic_cast<RGBAProtocol*>(node);
        CCASSERT(backgroundSprite != NULL, "Background sprite must not be nil.");
        CCASSERT(label != NULL || rgbaLabel!=NULL || backgroundSprite != NULL, "");
        
        m_bParentInited = true;

        // Initialize the button state tables
        this->setTitleDispatchTable(Dictionary::create());
        this->setTitleColorDispatchTable(Dictionary::create());
        this->setTitleLabelDispatchTable(Dictionary::create());
        this->setBackgroundSpriteDispatchTable(Dictionary::create());

        m_bIsPushed = false;
        m_bZoomOnTouchDown = true;

        m_pCurrentTitle=NULL;

        // Adjust the background image by default
        setAdjustBackgroundImage(true);
        setPreferredSize(Size::ZERO);
        // Zooming button by default
        m_bZoomOnTouchDown = true;
        
        // Set the default anchor point
        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Point(0.5f, 0.5f));
        
        // Set the nodes
        setTitleLabel(node);
        setBackgroundSprite(backgroundSprite);

        // Set the default color and opacity
        setColor(Color3B(255.0f, 255.0f, 255.0f));
        setOpacity(255.0f);
        setOpacityModifyRGB(true);
        
        // Initialize the dispatch table
        
        String* tempString = String::create(label->getString());
        //tempString->autorelease();
        setTitleForState(tempString, Control::State::NORMAL);
        setTitleColorForState(rgbaLabel->getColor(), Control::State::NORMAL);
        setTitleLabelForState(node, Control::State::NORMAL);
        setBackgroundSpriteForState(backgroundSprite, Control::State::NORMAL);
        
        setLabelAnchorPoint(Point(0.5f, 0.5f));

        // Layout update
        needsLayout();

        return true;
    }
    //couldn't init the Control
    else
    {
        return false;
    }
}

ControlButton* ControlButton::create(Node* label, Scale9Sprite* backgroundSprite)
{
    ControlButton *pRet = new ControlButton();
    pRet->initWithLabelAndBackgroundSprite(label, backgroundSprite);
    pRet->autorelease();
    return pRet;
}

bool ControlButton::initWithTitleAndFontNameAndFontSize(string title, const char * fontName, float fontSize)
{
    LabelTTF *label = LabelTTF::create(title.c_str(), fontName, fontSize);
    return initWithLabelAndBackgroundSprite(label, Scale9Sprite::create());
}

ControlButton* ControlButton::create(string title, const char * fontName, float fontSize)
{
    ControlButton *pRet = new ControlButton();
    pRet->initWithTitleAndFontNameAndFontSize(title, fontName, fontSize);
    pRet->autorelease();
    return pRet;
}

bool ControlButton::initWithBackgroundSprite(Scale9Sprite* sprite)
{
    LabelTTF *label = LabelTTF::create("", "Arial", 30);//
    return initWithLabelAndBackgroundSprite(label, sprite);
}

ControlButton* ControlButton::create(Scale9Sprite* sprite)
{
    ControlButton *pRet = new ControlButton();
    pRet->initWithBackgroundSprite(sprite);
    pRet->autorelease();
    return pRet;
}


void ControlButton::setMargins(int marginH, int marginV)
{
    m_nMarginV = marginV;
    m_nMarginH = marginH;
    needsLayout();
}

void ControlButton::setEnabled(bool enabled)
{
    Control::setEnabled(enabled);
    needsLayout();
}

void ControlButton::setSelected(bool enabled)
{
    Control::setSelected(enabled);
    needsLayout();
}

void ControlButton::setHighlighted(bool enabled)
{
    if (enabled == true)
    {
        m_eState = Control::State::HIGH_LIGHTED;
    }
    else
    {
        m_eState = Control::State::NORMAL;
    }
    
    Control::setHighlighted(enabled);

    Action *action = getActionByTag(kZoomActionTag);
    if (action)
    {
        stopAction(action);        
    }
    needsLayout();
    if( m_bZoomOnTouchDown )
    {
        float scaleValue = (isHighlighted() && isEnabled() && !isSelected()) ? 1.1f : 1.0f;
        Action *zoomAction = ScaleTo::create(0.05f, scaleValue);
        zoomAction->setTag(kZoomActionTag);
        runAction(zoomAction);
    }
}

void ControlButton::setZoomOnTouchDown(bool zoomOnTouchDown)
{
    m_bZoomOnTouchDown = zoomOnTouchDown;
}

bool ControlButton::getZoomOnTouchDown()
{
    return m_bZoomOnTouchDown;
}

void ControlButton::setPreferredSize(Size size)
{
    if(size.width == 0 && size.height == 0)
    {
        m_bDoesAdjustBackgroundImage = true;
    }
    else
    {
        m_bDoesAdjustBackgroundImage = false;
        DictElement * item = NULL;
        CCDICT_FOREACH(m_pBackgroundSpriteDispatchTable, item)
        {
            Scale9Sprite* sprite = static_cast<Scale9Sprite*>(item->getObject());
            sprite->setPreferredSize(size);
        }
    }

    m_tPreferredSize = size;
    needsLayout();
}

Size ControlButton::getPreferredSize()
{
    return m_tPreferredSize;
}

void ControlButton::setAdjustBackgroundImage(bool adjustBackgroundImage)
{
    m_bDoesAdjustBackgroundImage=adjustBackgroundImage;
    needsLayout();
}

bool ControlButton::doesAdjustBackgroundImage()
{
    return m_bDoesAdjustBackgroundImage;
}

Point ControlButton::getLabelAnchorPoint()
{
    return this->m_tLabelAnchorPoint;
}

void ControlButton::setLabelAnchorPoint(Point labelAnchorPoint)
{
    this->m_tLabelAnchorPoint = labelAnchorPoint;
    if (m_pTitleLabel != NULL)
    {
        this->m_pTitleLabel->setAnchorPoint(labelAnchorPoint);
    }
}

String* ControlButton::getTitleForState(State state)
{
    if (m_pTitleDispatchTable != NULL)
    {
        String* title=(String*)m_pTitleDispatchTable->objectForKey((int)state);
        if (title)
        {
            return title;
        }
        return (String*)m_pTitleDispatchTable->objectForKey((int)Control::State::NORMAL);
    }
    return String::create("");
}

void ControlButton::setTitleForState(String* title, State state)
{
    m_pTitleDispatchTable->removeObjectForKey((int)state);

    if (title)
    {
        m_pTitleDispatchTable->setObject(title, (int)state);
    }
    
    // If the current state if equal to the given state we update the layout
    if (getState() == state)
    {
        needsLayout();
    }
}


Color3B ControlButton::getTitleColorForState(State state) const
{
    Color3B returnColor = Color3B::WHITE;
    do 
    {
        CC_BREAK_IF(NULL == m_pTitleColorDispatchTable);
        Color3bObject* colorObject=(Color3bObject*)m_pTitleColorDispatchTable->objectForKey((int)state);
        if (colorObject)
        {
            returnColor = colorObject->m_tValue;
            break;
        }

        colorObject = (Color3bObject*)m_pTitleColorDispatchTable->objectForKey((int)Control::State::NORMAL);
        if (colorObject)
        {
            returnColor = colorObject->m_tValue;
        }
    } while (0);

    return returnColor;
}

void ControlButton::setTitleColorForState(Color3B color, State state)
{
    //Color3B* colorValue=&color;
    m_pTitleColorDispatchTable->removeObjectForKey((int)state);
    Color3bObject* pColor3bObject = new Color3bObject(color);
    pColor3bObject->autorelease();
    m_pTitleColorDispatchTable->setObject(pColor3bObject, (int)state);
      
    // If the current state if equal to the given state we update the layout
    if (getState() == state)
    {
        needsLayout();
    }
}

Node* ControlButton::getTitleLabelForState(State state)
{
    Node* titleLabel = (Node*)m_pTitleLabelDispatchTable->objectForKey((int)state);
    if (titleLabel)
    {
        return titleLabel;
    }
    return (Node*)m_pTitleLabelDispatchTable->objectForKey((int)Control::State::NORMAL);
}

void ControlButton::setTitleLabelForState(Node* titleLabel, State state)
{
    Node* previousLabel = (Node*)m_pTitleLabelDispatchTable->objectForKey((int)state);
    if (previousLabel)
    {
        removeChild(previousLabel, true);
        m_pTitleLabelDispatchTable->removeObjectForKey((int)state);
    }

    m_pTitleLabelDispatchTable->setObject(titleLabel, (int)state);
    titleLabel->setVisible(false);
    titleLabel->setAnchorPoint(Point(0.5f, 0.5f));
    addChild(titleLabel, 1);

    // If the current state if equal to the given state we update the layout
    if (getState() == state)
    {
        needsLayout();
    }
}

void ControlButton::setTitleTTFForState(const char * fntFile, State state)
{
    String * title = this->getTitleForState(state);
    if (!title)
    {
        title = String::create("");
    }
    this->setTitleLabelForState(LabelTTF::create(title->getCString(), fntFile, 12), state);
}

const char * ControlButton::getTitleTTFForState(State state)
{
    LabelProtocol* label = dynamic_cast<LabelProtocol*>(this->getTitleLabelForState(state));
    LabelTTF* labelTTF = dynamic_cast<LabelTTF*>(label);
    if(labelTTF != 0)
    {
        return labelTTF->getFontName().c_str();
    }
    else
    {
        return "";
    }
}

void ControlButton::setTitleTTFSizeForState(float size, State state)
{
    LabelProtocol* label = dynamic_cast<LabelProtocol*>(this->getTitleLabelForState(state));
    if(label)
    {
        LabelTTF* labelTTF = dynamic_cast<LabelTTF*>(label);
        if(labelTTF != 0)
        {
            return labelTTF->setFontSize(size);
        }
    }
}

float ControlButton::getTitleTTFSizeForState(State state)
{
    LabelProtocol* label = dynamic_cast<LabelProtocol*>(this->getTitleLabelForState(state));
    LabelTTF* labelTTF = dynamic_cast<LabelTTF*>(label);
    if(labelTTF != 0)
    {
        return labelTTF->getFontSize();
    }
    else
    {
        return 0;
    }
}

void ControlButton::setTitleBMFontForState(const char * fntFile, State state)
{
    String * title = this->getTitleForState(state);
    if (!title)
    {
        title = String::create("");
    }
    this->setTitleLabelForState(LabelBMFont::create(title->getCString(), fntFile), state);
}

const char * ControlButton::getTitleBMFontForState(State state)
{
    LabelProtocol* label = dynamic_cast<LabelProtocol*>(this->getTitleLabelForState(state));
    LabelBMFont* labelBMFont = dynamic_cast<LabelBMFont*>(label);
    if(labelBMFont != 0)
    {
        return labelBMFont->getFntFile();
    }
    else
    {
        return "";
    }
}


Scale9Sprite* ControlButton::getBackgroundSpriteForState(State state)
{
    Scale9Sprite* backgroundSprite = (Scale9Sprite*)m_pBackgroundSpriteDispatchTable->objectForKey((int)state);
    if (backgroundSprite)
    {
        return backgroundSprite;
    }
    return (Scale9Sprite*)m_pBackgroundSpriteDispatchTable->objectForKey((int)Control::State::NORMAL);
}


void ControlButton::setBackgroundSpriteForState(Scale9Sprite* sprite, State state)
{
    Size oldPreferredSize = m_tPreferredSize;

    Scale9Sprite* previousBackgroundSprite = (Scale9Sprite*)m_pBackgroundSpriteDispatchTable->objectForKey((int)state);
    if (previousBackgroundSprite)
    {
        removeChild(previousBackgroundSprite, true);
        m_pBackgroundSpriteDispatchTable->removeObjectForKey((int)state);
    }

    m_pBackgroundSpriteDispatchTable->setObject(sprite, (int)state);
    sprite->setVisible(false);
    sprite->setAnchorPoint(Point(0.5f, 0.5f));
    addChild(sprite);

    if (this->m_tPreferredSize.width != 0 || this->m_tPreferredSize.height != 0)
    {
        if (oldPreferredSize.equals(m_tPreferredSize))
        {
            // Force update of preferred size
            sprite->setPreferredSize(Size(oldPreferredSize.width+1, oldPreferredSize.height+1));
        }
        
        sprite->setPreferredSize(this->m_tPreferredSize);
    }

    // If the current state if equal to the given state we update the layout
    if (getState() == state)
    {
        needsLayout();
    }
}

void ControlButton::setBackgroundSpriteFrameForState(SpriteFrame * spriteFrame, State state)
{
    Scale9Sprite * sprite = Scale9Sprite::createWithSpriteFrame(spriteFrame);
    this->setBackgroundSpriteForState(sprite, state);
}


void ControlButton::needsLayout()
{
    if (!m_bParentInited) {
        return;
    }
    // Hide the background and the label
    if (m_pTitleLabel != NULL) {
        m_pTitleLabel->setVisible(false);
    }
    if (m_pBackgroundSprite) {
        m_pBackgroundSprite->setVisible(false);
    }
    // Update anchor of all labels
    this->setLabelAnchorPoint(this->m_tLabelAnchorPoint);
    
    // Update the label to match with the current state
    CC_SAFE_RELEASE(m_pCurrentTitle);
    m_pCurrentTitle = getTitleForState(m_eState);
    CC_SAFE_RETAIN(m_pCurrentTitle);

    m_tCurrentTitleColor = getTitleColorForState(m_eState);

    this->setTitleLabel(getTitleLabelForState(m_eState));

    LabelProtocol* label = dynamic_cast<LabelProtocol*>(m_pTitleLabel);
    if (label && m_pCurrentTitle)
    {
        label->setString(m_pCurrentTitle->getCString());
    }

    RGBAProtocol* rgbaLabel = dynamic_cast<RGBAProtocol*>(m_pTitleLabel);
    if (rgbaLabel)
    {
        rgbaLabel->setColor(m_tCurrentTitleColor);
    }
    if (m_pTitleLabel != NULL)
    {
        m_pTitleLabel->setPosition(Point (getContentSize().width / 2, getContentSize().height / 2));
    }
    
    // Update the background sprite
    this->setBackgroundSprite(this->getBackgroundSpriteForState(m_eState));
    if (m_pBackgroundSprite != NULL)
    {
        m_pBackgroundSprite->setPosition(Point (getContentSize().width / 2, getContentSize().height / 2));
    }
   
    // Get the title label size
    Size titleLabelSize;
    if (m_pTitleLabel != NULL)
    {
        titleLabelSize = m_pTitleLabel->getBoundingBox().size;
    }
    
    // Adjust the background image if necessary
    if (m_bDoesAdjustBackgroundImage)
    {
        // Add the margins
        if (m_pBackgroundSprite != NULL)
        {
            m_pBackgroundSprite->setContentSize(Size(titleLabelSize.width + m_nMarginH * 2, titleLabelSize.height + m_nMarginV * 2));
        }
    } 
    else
    {        
        //TODO: should this also have margins if one of the preferred sizes is relaxed?
        if (m_pBackgroundSprite != NULL)
        {
            Size preferredSize = m_pBackgroundSprite->getPreferredSize();
            if (preferredSize.width <= 0)
            {
                preferredSize.width = titleLabelSize.width;
            }
            if (preferredSize.height <= 0)
            {
                preferredSize.height = titleLabelSize.height;
            }

            m_pBackgroundSprite->setContentSize(preferredSize);
        }
    }
    
    // Set the content size
    Rect rectTitle;
    if (m_pTitleLabel != NULL)
    {
        rectTitle = m_pTitleLabel->getBoundingBox();
    }
    Rect rectBackground;
    if (m_pBackgroundSprite != NULL)
    {
        rectBackground = m_pBackgroundSprite->getBoundingBox();
    }

    Rect maxRect = ControlUtils::RectUnion(rectTitle, rectBackground);
    setContentSize(Size(maxRect.size.width, maxRect.size.height));        
    
    if (m_pTitleLabel != NULL)
    {
        m_pTitleLabel->setPosition(Point(getContentSize().width/2, getContentSize().height/2));
        // Make visible the background and the label
        m_pTitleLabel->setVisible(true);
    }
  
    if (m_pBackgroundSprite != NULL)
    {
        m_pBackgroundSprite->setPosition(Point(getContentSize().width/2, getContentSize().height/2));
        m_pBackgroundSprite->setVisible(true);   
    }   
}



bool ControlButton::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!isTouchInside(pTouch) || !isEnabled() || !isVisible() || !hasVisibleParents() )
    {
        return false;
    }
    
    for (Node *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    m_bIsPushed = true;
    this->setHighlighted(true);
    sendActionsForControlEvents(Control::EventType::TOUCH_DOWN);
    return true;
}

void ControlButton::onTouchMoved(Touch *pTouch, Event *pEvent)
{    
    if (!isEnabled() || !isPushed() || isSelected())
    {
        if (isHighlighted())
        {
            setHighlighted(false);
        }
        return;
    }
    
    bool isTouchMoveInside = isTouchInside(pTouch);
    if (isTouchMoveInside && !isHighlighted())
    {
        setHighlighted(true);
        sendActionsForControlEvents(Control::EventType::DRAG_ENTER);
    }
    else if (isTouchMoveInside && isHighlighted())
    {
        sendActionsForControlEvents(Control::EventType::DRAG_INSIDE);
    }
    else if (!isTouchMoveInside && isHighlighted())
    {
        setHighlighted(false);
        
        sendActionsForControlEvents(Control::EventType::DRAG_EXIT);        
    }
    else if (!isTouchMoveInside && !isHighlighted())
    {
        sendActionsForControlEvents(Control::EventType::DRAG_OUTSIDE);        
    }
}
void ControlButton::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    m_bIsPushed = false;
    setHighlighted(false);
    
    
    if (isTouchInside(pTouch))
    {
        sendActionsForControlEvents(Control::EventType::TOUCH_UP_INSIDE);        
    }
    else
    {
        sendActionsForControlEvents(Control::EventType::TOUCH_UP_OUTSIDE);        
    }
}

void ControlButton::setOpacity(GLubyte opacity)
{
    // XXX fixed me if not correct
    Control::setOpacity(opacity);
//    _opacity = opacity;
//    
//    Object* child;
//    Array* children=getChildren();
//    CCARRAY_FOREACH(children, child)
//    {
//        RGBAProtocol* pNode = dynamic_cast<RGBAProtocol*>(child);        
//        if (pNode)
//        {
//            pNode->setOpacity(opacity);
//        }
//    }
    DictElement * item = NULL;
    CCDICT_FOREACH(m_pBackgroundSpriteDispatchTable, item)
    {
        Scale9Sprite* sprite = static_cast<Scale9Sprite*>(item->getObject());
        sprite->setOpacity(opacity);
    }
}

GLubyte ControlButton::getOpacity() const
{
    return m_cRealOpacity;
}

void ControlButton::setColor(const Color3B & color)
{
	Control::setColor(color);
	
	DictElement * item = NULL;
    CCDICT_FOREACH(m_pBackgroundSpriteDispatchTable, item)
    {
        Scale9Sprite* sprite = static_cast<Scale9Sprite*>(item->getObject());
        sprite->setColor(color);
    }
}

const Color3B& ControlButton::getColor() const
{
	return m_tRealColor;
}

void ControlButton::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    m_bIsPushed = false;
    setHighlighted(false);
    sendActionsForControlEvents(Control::EventType::TOUCH_CANCEL);
}

ControlButton* ControlButton::create()
{
    ControlButton *pControlButton = new ControlButton();
    if (pControlButton && pControlButton->init())
    {
        pControlButton->autorelease();
        return pControlButton;
    }
    CC_SAFE_DELETE(pControlButton);
    return NULL;
}

NS_CC_EXT_END
