/* -----------------------------------------------------------------------------------
 *
 *      File            CCMenuItem.cpp
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

#include "2d/menu_nodes/CCMenuItem.h"
#include "2d/actions/CCActionInterval.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "2d/label_nodes/CCLabelAtlas.h"
#include "2d/label_nodes/CCLabelTTF.h"
#include "2d/script_support/CCScriptSupport.h"
#include "audio/SimpleAudioEngine.h"

NS_CC_BEGIN
    
static long _globalFontSize = kItemSize;
static std::string _globalFontName = "Marker Felt";
static bool _globalFontNameRelease = false;

const unsigned int    kCurrentItem = 0xc0c05001;
const unsigned int    kZoomActionTag = 0xc0c05002;

const unsigned int    kNormalTag = 0x1;
const unsigned int    kSelectedTag = 0x2;
const unsigned int    kDisableTag = 0x3;

// Added By XMSoft
static std::string		l_sSoundActivate	= "";
static std::string		l_sSoundSelected	= "";
static std::string		l_sSoundUnselected	= "";

//
// MenuItem
//

MenuItem* MenuItem::create()
{
    return MenuItem::create((const ccMenuCallback&)nullptr);
}

MenuItem* MenuItem::create( const ccMenuCallback& callback)
{
    MenuItem *ret = new MenuItem();
    ret->initWithCallback(callback);
    ret->autorelease();
    return ret;
}

bool MenuItem::initWithCallback(const ccMenuCallback& callback)
{
    setAnchorPoint(Point(0.5f, 0.5f));
	m_pCallback = callback;
    m_bEnabled = true;
    m_bSelected = false;
    return true;
}

MenuItem::MenuItem ( KDvoid )
: m_bSelected ( false )
, m_bEnabled  ( false )            
, m_pCallback ( nullptr )
{
	// Added By XMSoft
	m_sSoundActivate	= l_sSoundActivate;
	m_sSoundSelected	= l_sSoundSelected;
	m_sSoundUnselected	= l_sSoundUnselected;	
}

MenuItem::~MenuItem ( KDvoid )
{
	
}

KDvoid MenuItem::selected ( KDvoid )
{
    m_bSelected = true;

	if ( m_sSoundSelected.size ( ) > 0 )
	{
		CocosDenshion::SimpleAudioEngine::getInstance ( )->playEffect ( m_sSoundSelected.c_str ( ) );
	}
}

KDvoid MenuItem::unselected ( KDvoid )
{
    m_bSelected = false;

	if ( m_sSoundUnselected.size ( ) > 0 )
	{
		CocosDenshion::SimpleAudioEngine::getInstance ( )->playEffect ( m_sSoundUnselected.c_str ( ) );
	}
}

KDvoid MenuItem::activate ( KDvoid )
{
    if ( m_bEnabled )
    {
        if ( m_pCallback )
        {
			m_pCallback ( this );
        }
        
        if ( kScriptTypeNone != m_eScriptType )
        {
            BasicScriptData		tData ( this );
            ScriptEvent			tScriptEvent ( kMenuClickedEvent, &tData );
            ScriptEngineManager::getInstance ( )->getScriptEngine ( )->sendEvent ( &tScriptEvent );
        }

		if ( m_sSoundActivate.size ( ) > 0 )
		{
			CocosDenshion::SimpleAudioEngine::getInstance ( )->playEffect ( m_sSoundActivate.c_str ( ) );
		}
    }
}

void MenuItem::setEnabled(bool enabled)
{
    m_bEnabled = enabled;
}

bool MenuItem::isEnabled() const
{
    return m_bEnabled;
}

Rect MenuItem::rect() const
{
    return Rect( m_tPosition.x - m_tContentSize.width * m_tAnchorPoint.x,
                      m_tPosition.y - m_tContentSize.height * m_tAnchorPoint.y,
                      m_tContentSize.width, m_tContentSize.height);
}

bool MenuItem::isSelected() const
{
    return m_bSelected;
}

void MenuItem::setCallback(const ccMenuCallback& callback)
{
	m_pCallback = callback;
}

// Added By XMSoft
KDvoid MenuItem::setSoundActivate ( const KDchar* szPath )
{
	if ( szPath )
	{
		m_sSoundActivate = szPath;
	}
	else
	{
		m_sSoundActivate.clear ( );
	}
}

KDvoid MenuItem::setSoundSelected ( const KDchar* szPath )
{
	if ( szPath )
	{
		m_sSoundSelected = szPath;
	}
	else
	{
		m_sSoundSelected.clear ( );
	}
}

KDvoid MenuItem::setSoundUnselected ( const KDchar* szPath )
{
	if ( szPath )
	{
		m_sSoundUnselected = szPath;
	}
	else
	{
		m_sSoundUnselected.clear ( );
	}
}

KDvoid MenuItem::setDefaultSoundActivate ( const KDchar* szPath )
{
	if ( szPath )
	{
		l_sSoundActivate = szPath;
	}
	else
	{
		l_sSoundActivate.clear ( );
	}
}

KDvoid MenuItem::setDefaultSoundSelected ( const KDchar* szPath )
{
	if ( szPath )
	{
		l_sSoundSelected = szPath;
	}
	else
	{
		l_sSoundSelected.clear ( );
	}
}

KDvoid MenuItem::setDefaultSoundUnselected ( const KDchar* szPath )
{
	if ( szPath )
	{
		l_sSoundUnselected = szPath;
	}
	else
	{
		l_sSoundUnselected.clear ( );
	}
}

//
//CCMenuItemLabel
//

void MenuItemLabel::setLabel(Node* var)
{
    if (var)
    {
        addChild(var);
        var->setAnchorPoint(Point(0, 0));
        setContentSize(var->getContentSize());
    }
    
    if (m_pLabel)
    {
        removeChild(m_pLabel, true);
    }
    
    m_pLabel = var;
}

MenuItemLabel * MenuItemLabel::create(Node*label, const ccMenuCallback& callback)
{
    MenuItemLabel *ret = new MenuItemLabel();
    ret->initWithLabel(label, callback);
    ret->autorelease();
    return ret;
}

MenuItemLabel* MenuItemLabel::create(Node *label)
{
    MenuItemLabel *ret = new MenuItemLabel();
    ret->initWithLabel(label, (const ccMenuCallback&) nullptr);
    ret->autorelease();
    return ret;
}

bool MenuItemLabel::initWithLabel(Node* label, const ccMenuCallback& callback)
{
    MenuItem::initWithCallback(callback);
    m_fOriginalScale = 1.0f;
    m_tColorBackup = Color3B::WHITE;
    setDisabledColor(Color3B(126,126,126));
    this->setLabel(label);

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    return true;
}


MenuItemLabel::~MenuItemLabel()
{
}

void MenuItemLabel::setString(const std::string& label)
{
    dynamic_cast<LabelProtocol*>(m_pLabel)->setString(label);
    this->setContentSize(m_pLabel->getContentSize());
}

void MenuItemLabel::activate()
{
    if(m_bEnabled)
    {
        this->stopAllActions();
        this->setScale( m_fOriginalScale );
        MenuItem::activate();
    }
}

void MenuItemLabel::selected()
{
    // subclass to change the default action
    if(m_bEnabled)
    {
        MenuItem::selected();
        
        Action *action = getActionByTag(kZoomActionTag);
        if (action)
        {
            this->stopAction(action);
        }
        else
        {
            m_fOriginalScale = this->getScale();
        }
        
        Action *zoomAction = ScaleTo::create(0.1f, m_fOriginalScale * 1.2f);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

void MenuItemLabel::unselected()
{
    // subclass to change the default action
    if(m_bEnabled)
    {
        MenuItem::unselected();
        this->stopActionByTag(kZoomActionTag);
        Action *zoomAction = ScaleTo::create(0.1f, m_fOriginalScale);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

void MenuItemLabel::setEnabled(bool enabled)
{
    if( m_bEnabled != enabled ) 
    {
        if(enabled == false)
        {
            m_tColorBackup = dynamic_cast<RGBAProtocol*>(m_pLabel)->getColor();
            dynamic_cast<RGBAProtocol*>(m_pLabel)->setColor(m_tDisabledColor);
        }
        else
        {
            dynamic_cast<RGBAProtocol*>(m_pLabel)->setColor(m_tColorBackup);
        }
    }
    MenuItem::setEnabled(enabled);
}

//
//CCMenuItemAtlasFont
//

MenuItemAtlasFont * MenuItemAtlasFont::create(const std::string& value, const std::string& charMapFile, KDint32 itemWidth, KDint32 itemHeight, char startCharMap)
{
    return MenuItemAtlasFont::create(value, charMapFile, itemWidth, itemHeight, startCharMap, (const ccMenuCallback&)nullptr);
}

MenuItemAtlasFont * MenuItemAtlasFont::create(const std::string& value, const std::string& charMapFile, KDint32 itemWidth, KDint32 itemHeight, char startCharMap, const ccMenuCallback& callback)
{
    MenuItemAtlasFont *ret = new MenuItemAtlasFont();
    ret->initWithString(value, charMapFile, itemWidth, itemHeight, startCharMap, callback);
    ret->autorelease();
    return ret;
}

bool MenuItemAtlasFont::initWithString(const std::string& value, const std::string& charMapFile, KDint32 itemWidth, KDint32 itemHeight, char startCharMap, const ccMenuCallback& callback)
{
    CCASSERT( value.size() != 0, "value length must be greater than 0");
    LabelAtlas *label = new LabelAtlas();
    label->initWithString(value, charMapFile, itemWidth, itemHeight, startCharMap);
    label->autorelease();
    if (MenuItemLabel::initWithLabel(label, callback))
    {
        // do something ?
    }
    return true;
}

//
//CCMenuItemFont
//

void MenuItemFont::setFontSize(KDint32 s)
{
    _globalFontSize = s;
}

KDint32 MenuItemFont::getFontSize()
{
    return _globalFontSize;
}

void MenuItemFont::setFontName(const std::string& name)
{
    if (_globalFontNameRelease)
    {
        _globalFontName.clear();
    }
    _globalFontName = name;
    _globalFontNameRelease = true;
}

const std::string& MenuItemFont::getFontName()
{
    return _globalFontName;
}

MenuItemFont * MenuItemFont::create(const std::string& value, const ccMenuCallback& callback)
{
    MenuItemFont *ret = new MenuItemFont();
    ret->initWithString(value, callback);
    ret->autorelease();
    return ret;
}


MenuItemFont * MenuItemFont::create(const std::string& value)
{
    MenuItemFont *ret = new MenuItemFont();
    ret->initWithString(value, (const ccMenuCallback&)nullptr);
    ret->autorelease();
    return ret;
}

MenuItemFont::MenuItemFont()
 : m_nFontSize(0), m_sFontName("")
{}

MenuItemFont::~MenuItemFont()
{
    CCLOGINFO("In the destructor of MenuItemFont (%p).", this);
}

bool MenuItemFont::initWithString(const std::string& value, const ccMenuCallback& callback)
{
    CCASSERT( value.size() >= 0, "Value length must be greater than 0");

    m_sFontName = _globalFontName;
    m_nFontSize = _globalFontSize;

    LabelTTF *label = LabelTTF::create(value, m_sFontName, (float)m_nFontSize);
    if (MenuItemLabel::initWithLabel(label, callback))
    {
        // do something ?
    }
    return true;
}

void MenuItemFont::recreateLabel()
{
    LabelTTF *label = LabelTTF::create(dynamic_cast<LabelProtocol*>(m_pLabel)->getString(), 
                                                    m_sFontName.c_str(), (float)m_nFontSize);
    this->setLabel(label);
}

void MenuItemFont::setFontSizeObj(KDint32 s)
{
    m_nFontSize = s;
    recreateLabel();
}

KDint32 MenuItemFont::getFontSizeObj() const
{
    return m_nFontSize;
}

void MenuItemFont::setFontNameObj(const std::string& name)
{
    m_sFontName = name;
    recreateLabel();
}

const std::string& MenuItemFont::getFontNameObj() const
{
    return m_sFontName;
}

//
//CCMenuItemSprite
//

void MenuItemSprite::setNormalImage(Node* pImage)
{
    if (pImage != m_pNormalImage)
    {
        if (pImage)
        {
            addChild(pImage, 0, kNormalTag);
            pImage->setAnchorPoint(Point(0, 0));
        }

        if (m_pNormalImage)
        {
            removeChild(m_pNormalImage, true);
        }

        m_pNormalImage = pImage;
        this->setContentSize(m_pNormalImage->getContentSize());
        this->updateImagesVisibility();
    }
}

void MenuItemSprite::setSelectedImage(Node* pImage)
{
    if (pImage != m_pNormalImage)
    {
        if (pImage)
        {
            addChild(pImage, 0, kSelectedTag);
            pImage->setAnchorPoint(Point(0, 0));
        }

        if (m_pSelectedImage)
        {
            removeChild(m_pSelectedImage, true);
        }

        m_pSelectedImage = pImage;
        this->updateImagesVisibility();
    }
}

void MenuItemSprite::setDisabledImage(Node* pImage)
{
    if (pImage != m_pNormalImage)
    {
        if (pImage)
        {
            addChild(pImage, 0, kDisableTag);
            pImage->setAnchorPoint(Point(0, 0));
        }

        if (m_pDisabledImage)
        {
            removeChild(m_pDisabledImage, true);
        }

        m_pDisabledImage = pImage;
        this->updateImagesVisibility();
    }
}

//
//CCMenuItemSprite
//

MenuItemSprite * MenuItemSprite::create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite)
{
    return MenuItemSprite::create(normalSprite, selectedSprite, disabledSprite, (const ccMenuCallback&)nullptr);
}

MenuItemSprite * MenuItemSprite::create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback)
{
    return MenuItemSprite::create(normalSprite, selectedSprite, NULL, callback);
}

MenuItemSprite * MenuItemSprite::create(Node *normalSprite, Node *selectedSprite, Node *disabledSprite, const ccMenuCallback& callback)
{
    MenuItemSprite *ret = new MenuItemSprite();
    ret->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
    ret->autorelease();
    return ret;
}

bool MenuItemSprite::initWithNormalSprite(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback)
{
    MenuItem::initWithCallback(callback);
    setNormalImage(normalSprite);
    setSelectedImage(selectedSprite);
    setDisabledImage(disabledSprite);

    if(m_pNormalImage)
    {
        this->setContentSize(m_pNormalImage->getContentSize());
    }

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    return true;
}

/**
 @since v0.99.5
 */
void MenuItemSprite::selected()
{
    MenuItem::selected();

    if (m_pNormalImage)
    {
        if (m_pDisabledImage)
        {
            m_pDisabledImage->setVisible(false);
        }

        if (m_pSelectedImage)
        {
            m_pNormalImage->setVisible(false);
            m_pSelectedImage->setVisible(true);
        }
        else
        {
            m_pNormalImage->setVisible(true);
        }
    }
}

void MenuItemSprite::unselected()
{
    MenuItem::unselected();
    if (m_pNormalImage)
    {
        m_pNormalImage->setVisible(true);

        if (m_pSelectedImage)
        {
            m_pSelectedImage->setVisible(false);
        }

        if (m_pDisabledImage)
        {
            m_pDisabledImage->setVisible(false);
        }
    }
}

void MenuItemSprite::setEnabled(bool bEnabled)
{
    if( m_bEnabled != bEnabled ) 
    {
        MenuItem::setEnabled(bEnabled);
        this->updateImagesVisibility();
    }
}

// Helper 
void MenuItemSprite::updateImagesVisibility()
{
    if (m_bEnabled)
    {
        if (m_pNormalImage)   m_pNormalImage->setVisible(true);
        if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
        if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
    }
    else
    {
        if (m_pDisabledImage)
        {
            if (m_pNormalImage)   m_pNormalImage->setVisible(false);
            if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
            if (m_pDisabledImage) m_pDisabledImage->setVisible(true);
        }
        else
        {
            if (m_pNormalImage)   m_pNormalImage->setVisible(true);
            if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
            if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
        }
    }
}

///
/// MenuItemImage
///

MenuItemImage* MenuItemImage::create()
{
    MenuItemImage *ret = new MenuItemImage();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool MenuItemImage::init(void)
{
    return initWithNormalImage("", "", "", (const ccMenuCallback&)nullptr);
}

MenuItemImage * MenuItemImage::create(const std::string& normalImage, const std::string& selectedImage)
{
    return MenuItemImage::create(normalImage, selectedImage, "", (const ccMenuCallback&)nullptr);
}

MenuItemImage * MenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const ccMenuCallback& callback)
{
    return MenuItemImage::create(normalImage, selectedImage, "", callback);
}

MenuItemImage * MenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
    MenuItemImage *ret = new MenuItemImage();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, callback))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

MenuItemImage * MenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage)
{
    MenuItemImage *ret = new MenuItemImage();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, (const ccMenuCallback&)nullptr))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool MenuItemImage::initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
    Node *normalSprite = NULL;
    Node *selectedSprite = NULL;
    Node *disabledSprite = NULL;

    if (normalImage.size() >0)
    {
        normalSprite = Sprite::create(normalImage);
    }

    if (selectedImage.size() >0)
    {
        selectedSprite = Sprite::create(selectedImage);
    }

    if(disabledImage.size() >0)
    {
        disabledSprite = Sprite::create(disabledImage);
    }
    return initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
}

//
// Setter of sprite frames
//
void MenuItemImage::setNormalSpriteFrame(SpriteFrame * frame)
{
    setNormalImage(Sprite::createWithSpriteFrame(frame));
}

void MenuItemImage::setSelectedSpriteFrame(SpriteFrame * frame)
{
    setSelectedImage(Sprite::createWithSpriteFrame(frame));
}

void MenuItemImage::setDisabledSpriteFrame(SpriteFrame * frame)
{
    setDisabledImage(Sprite::createWithSpriteFrame(frame));
}

//
// MenuItemToggle
//
MenuItemToggle * MenuItemToggle::createWithCallback(const ccMenuCallback &callback, Array* menuItems)
{
    MenuItemToggle *ret = new MenuItemToggle();
    ret->MenuItem::initWithCallback(callback);
    ret->m_pSubItems = Array::create();
    ret->m_pSubItems->retain();

    for (int z=0; z < menuItems->count(); z++)
    {
        MenuItem* menuItem = (MenuItem*)menuItems->getObjectAtIndex(z);
        ret->m_pSubItems->addObject(menuItem);
    }

    ret->m_uSelectedIndex = UINT_MAX;
    ret->setSelectedIndex(0);
    return ret;
}

MenuItemToggle * MenuItemToggle::createWithCallback(const ccMenuCallback &callback, MenuItem* item, ...)
{
    va_list args;
    va_start(args, item);
    MenuItemToggle *ret = new MenuItemToggle();
    ret->initWithCallback(callback, item, args);
    ret->autorelease();
    va_end(args);
    return ret;
}

MenuItemToggle * MenuItemToggle::create()
{
    MenuItemToggle *ret = new MenuItemToggle();
    ret->initWithItem(NULL);
    ret->autorelease();
    return ret;
}

bool MenuItemToggle::initWithCallback(const ccMenuCallback &callback, MenuItem *item, va_list args)
{
    MenuItem::initWithCallback(callback);
    this->m_pSubItems = Array::create();
    this->m_pSubItems->retain();
    int z = 0;
    MenuItem *i = item;
    while(i)
    {
        z++;
        m_pSubItems->addObject(i);
        i = va_arg(args, MenuItem*);
    }
    m_uSelectedIndex = UINT_MAX;
    this->setSelectedIndex(0);
    return true;
}

MenuItemToggle* MenuItemToggle::create(MenuItem *item)
{
    MenuItemToggle *ret = new MenuItemToggle();
    ret->initWithItem(item);
    ret->autorelease();
    return ret;
}

bool MenuItemToggle::initWithItem(MenuItem *item)
{
    MenuItem::initWithCallback((const ccMenuCallback&)nullptr);
    setSubItems(Array::create());

    if (item)
    {
        m_pSubItems->addObject(item);
    }
    m_uSelectedIndex = UINT_MAX;
    this->setSelectedIndex(0);
    
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    
    return true;
}

void MenuItemToggle::addSubItem(MenuItem *item)
{
    m_pSubItems->addObject(item);
}

MenuItemToggle::~MenuItemToggle()
{
    if (m_pSubItems)
    {
        for (auto& item : *m_pSubItems)
        {
            static_cast<MenuItem*>(item)->cleanup();
        }
        m_pSubItems->release();
    }
}

void MenuItemToggle::setSelectedIndex(unsigned int index)
{
    if( index != m_uSelectedIndex && m_pSubItems->count() > 0 )
    {
        m_uSelectedIndex = index;
        MenuItem *currentItem = (MenuItem*)getChildByTag(kCurrentItem);
        if( currentItem )
        {
            currentItem->removeFromParentAndCleanup(false);
        }

        MenuItem* item = (MenuItem*)m_pSubItems->getObjectAtIndex(m_uSelectedIndex);
        this->addChild(item, 0, kCurrentItem);
        Size s = item->getContentSize();
        this->setContentSize(s);
        item->setPosition( Point( s.width/2, s.height/2 ) );
    }
}

void MenuItemToggle::selected()
{
    MenuItem::selected();
    static_cast<MenuItem*>(m_pSubItems->getObjectAtIndex(m_uSelectedIndex))->selected();
}

void MenuItemToggle::unselected()
{
    MenuItem::unselected();
    static_cast<MenuItem*>(m_pSubItems->getObjectAtIndex(m_uSelectedIndex))->unselected();
}

void MenuItemToggle::activate()
{
    // update index
    if( m_bEnabled ) 
    {
        unsigned int newIndex = (m_uSelectedIndex + 1) % m_pSubItems->count();
        this->setSelectedIndex(newIndex);
    }
    MenuItem::activate();
}
void MenuItemToggle::setEnabled(bool enabled)
{
    if (m_bEnabled != enabled)
    {
        MenuItem::setEnabled(enabled);

        if(m_pSubItems && m_pSubItems->count() > 0)
        {
            Object* pObj = NULL;
            CCARRAY_FOREACH(m_pSubItems, pObj)
            {
                MenuItem* pItem = static_cast<MenuItem*>(pObj);
                pItem->setEnabled(enabled);
            }
        }
    }
}

MenuItem* MenuItemToggle::getSelectedItem()
{
    return static_cast<MenuItem*>(m_pSubItems->getObjectAtIndex(m_uSelectedIndex));
}

NS_CC_END
