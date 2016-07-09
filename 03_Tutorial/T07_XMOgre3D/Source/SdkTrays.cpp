/* ------------------------------------------------------------------------------------
 *
 *      File            SdkTrays.cpp
 *      Description     This source file is part of OGRE
 *                      (Object-oriented Graphics Rendering Engine)
 *      Author          Y.H Mun
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 *
 *      Contact Email: xmsoft77@gmail.com
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2012 Torus Knot Software Ltd.
 *
 *      For the latest info, see http://www.ogre3d.org/
 *
 * ------------------------------------------------------------------------------------
 *
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ------------------------------------------------------------------------------------ */

#include "Precompiled.h"
#include "SdkTrays.h"

//--------------------------------------------------------------------------
//
//  Widget
//
//--------------------------------------------------------------------------

Widget::Widget ( KDvoid )
{
    m_tTrayLoc = TL_NONE;
    m_pElement = 0;
    m_pListener = 0;
}

Widget::~Widget ( KDvoid )
{
    
}

KDvoid Widget::cleanup ( KDvoid )
{
    if ( m_pElement )
    {
        nukeOverlayElement ( m_pElement );
        m_pElement = 0;
    }
}

KDvoid Widget::nukeOverlayElement ( OverlayElement* pElement )
{
    OverlayContainer*  pContainer = dynamic_cast<OverlayContainer*> ( pElement );
    if ( pContainer )
    {
        std::vector<OverlayElement*>  aToDelete;
        
        OverlayContainer::ChildIterator  it = pContainer->getChildIterator ( );
        while ( it.hasMoreElements ( ) )
        {
            aToDelete.push_back ( it.getNext ( ) );
        }
        
        for ( KDuint i = 0; i < aToDelete.size ( ); i++ )
        {
            nukeOverlayElement ( aToDelete[i] );
        }
    }
    if ( pElement )
    {
        OverlayContainer*  pParent = pElement->getParent ( );
        if ( pParent )
        {
            pParent->removeChild ( pElement->getName ( ) );
        }
        
        OverlayManager::getSingleton ( ).destroyOverlayElement ( pElement );
    }
}

bool Widget::isCursorOver ( OverlayElement* pElement, const Vector2& tCursorPos, Real fVoidBorder )
{
    OverlayManager&  tOverMgr = OverlayManager::getSingleton ( );
    
    Real l = pElement->_getDerivedLeft ( ) * tOverMgr.getViewportWidth  ( );
    Real t = pElement->_getDerivedTop  ( ) * tOverMgr.getViewportHeight ( );
    Real r = l + pElement->getWidth  ( );
    Real b = t + pElement->getHeight ( );
    
    return ( tCursorPos.x >= l + fVoidBorder && tCursorPos.x <= r - fVoidBorder &&
             tCursorPos.y >= t + fVoidBorder && tCursorPos.y <= b - fVoidBorder );
}

Vector2 Widget::cursorOffset ( OverlayElement* pElement, const Vector2& tCursorPos )
{
    OverlayManager&  tOverMgr = OverlayManager::getSingleton ( );
    return Vector2 ( tCursorPos.x - ( pElement->_getDerivedLeft ( ) * tOverMgr.getViewportWidth  ( ) + pElement->getWidth  ( ) / 2 ),
                     tCursorPos.y - ( pElement->_getDerivedTop  ( ) * tOverMgr.getViewportHeight ( ) + pElement->getHeight ( ) / 2 ) );
}

Real Widget::getCaptionWidth ( const DisplayString& tCaption, TextAreaOverlayElement* pArea )
{
    Font*   pFont = (Font*) FontManager::getSingleton ( ).getByName ( pArea->getFontName ( ) ).getPointer ( );
    String  sCurrent = DISPLAY_STRING_TO_STRING ( tCaption );
    Real    fLineWidth = 0;
    
    for ( KDuint i = 0; i < sCurrent.length ( ); i++ )
    {
        // be sure to provide a line width in the text area
        if ( sCurrent [i] == ' ' )
        {
            if ( pArea->getSpaceWidth ( ) != 0 )
            {
                fLineWidth += pArea->getSpaceWidth ( );
            }
            else
            {
                fLineWidth += pFont->getGlyphAspectRatio ( ' ' ) * pArea->getCharHeight ( );
            }
        }
        else if ( sCurrent [i] == '\n' )
        {
            break;
        }
        // use glyph information to calculate line width
        else
        {
            fLineWidth += pFont->getGlyphAspectRatio ( sCurrent [i] ) * pArea->getCharHeight ( );
        }
    }
    
    return fLineWidth;
}

KDvoid Widget::fitCaptionToArea ( const DisplayString& tCaption, TextAreaOverlayElement* pArea, Real fMaxWidth )
{
    Font*   f = (Font*) FontManager::getSingleton ( ).getByName ( pArea->getFontName ( ) ).getPointer ( );
    String  s = DISPLAY_STRING_TO_STRING ( tCaption );
    KDint  nl = s.find ( '\n' );
    if ( nl != -1 )
    {
        s = s.substr ( 0, nl );
    }
        
    Real  fWidth = 0;
        
    for ( KDuint i = 0; i < s.length ( ); i++ )
    {
        if ( s[i] == ' ' && pArea->getSpaceWidth ( ) != 0 )
        {
            fWidth += pArea->getSpaceWidth ( );
        }
        else
        {
            fWidth += f->getGlyphAspectRatio ( s[i] ) * pArea->getCharHeight ( );
        }
            
        if ( fWidth > fMaxWidth )
        {
            s = s.substr(0, i);
            break;
        }
    }
    
    pArea->setCaption ( s );
}

OverlayElement* Widget::getOverlayElement ( KDvoid )
{
    return m_pElement;
}

const String& Widget::getName ( KDvoid )
{
    return m_pElement->getName ( );
}

TrayLocation Widget::getTrayLocation ( KDvoid )
{
    return m_tTrayLoc;
}

KDvoid Widget::hide ( KDvoid )
{
    m_pElement->hide ( );
}

KDvoid Widget::show ( KDvoid )
{
    m_pElement->show ( );
}

bool Widget::isVisible ( KDvoid )
{
    return m_pElement->isVisible ( );
}


KDvoid Widget::_cursorPressed  ( const Vector2& tCursorPos )
{
    
}

KDvoid Widget::_cursorReleased ( const Vector2& tCursorPos )
{
    
}

KDvoid  Widget::_cursorMoved ( const Vector2& tCursorPos )
{
    
}

KDvoid Widget::_focusLost ( KDvoid )
{
    
}

KDvoid Widget::_assignToTray ( TrayLocation tTrayLoc )
{
    m_tTrayLoc = tTrayLoc;
}

KDvoid Widget::_assignListener ( SdkTrayListener* pListener )
{
    m_pListener = pListener;
}

//--------------------------------------------------------------------------
//
//  Button
//
//--------------------------------------------------------------------------

Button::Button ( const String& sName, const DisplayString& tCaption, Real fWidth )
{
    m_pElement = OverlayManager::getSingleton ( ).createOverlayElementFromTemplate ( "SdkTrays/Button", "BorderPanel", sName );
    m_pBP      = (BorderPanelOverlayElement*) m_pElement;
    m_pTextArea = (TextAreaOverlayElement*) m_pBP->getChild ( m_pBP->getName ( ) + "/ButtonCaption" );
    
//#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
//    mTextArea->setCharHeight(mTextArea->getCharHeight() - 3);
//#endif
    m_pTextArea->setTop ( - ( m_pTextArea->getCharHeight ( ) / 2 ) );
    
    if ( fWidth > 0 )
    {
        m_pElement->setWidth ( fWidth );
        m_bFitToContents = false;
    }
    else
    {
        m_bFitToContents = true;
    }
        
    setCaption ( tCaption );
    m_eState = BS_UP;
}

Button::~Button ( KDvoid )
{
    
}

const DisplayString& Button::getCaption ( KDvoid )
{
    return m_pTextArea->getCaption ( );
}

KDvoid Button::setCaption ( const DisplayString& tCaption )
{
    m_pTextArea->setCaption ( tCaption );
    if ( m_bFitToContents )
    {
        m_pElement->setWidth ( getCaptionWidth ( tCaption, m_pTextArea ) + m_pElement->getHeight ( ) - 12 );
    }
}

const ButtonState& Button::getState ( KDvoid )
{
    return m_eState;
}

KDvoid Button::_cursorPressed ( const Vector2& tCursorPos )
{
    if ( isCursorOver ( m_pElement, tCursorPos, 4 ) )
    {
        setState ( BS_DOWN );
    }
}

KDvoid Button::_cursorReleased ( const Vector2& tCursorPos )
{
    if ( m_eState == BS_DOWN )
    {
        setState ( BS_OVER );
        if ( m_pListener )
        {
            m_pListener->buttonHit ( this );
        }
    }
}

KDvoid Button::_cursorMoved ( const Vector2& tCursorPos )
{
    if ( isCursorOver ( m_pElement, tCursorPos, 4 ) )
    {
        if ( m_eState == BS_UP )
        {
            setState ( BS_OVER );
        }
    }
    else
    {
        if ( m_eState != BS_UP )
        {
            setState ( BS_UP );
        }
    }
}

KDvoid Button::_focusLost ( KDvoid )
{
    setState ( BS_UP );   // reset button if cursor was lost
}

KDvoid Button::setState ( const ButtonState& eBS )
{
    if ( eBS == BS_OVER )
    {
        m_pBP->setBorderMaterialName ( "SdkTrays/Button/Over" );
        m_pBP->setMaterialName       ( "SdkTrays/Button/Over" );
    }
    else if ( eBS == BS_UP )
    {
        m_pBP->setBorderMaterialName ( "SdkTrays/Button/Up" );
        m_pBP->setMaterialName       ( "SdkTrays/Button/Up" );
    }
    else
    {
        m_pBP->setBorderMaterialName ( "SdkTrays/Button/Down" );
        m_pBP->setMaterialName       ( "SdkTrays/Button/Down" );
    }
    
    m_eState = eBS;
}

//--------------------------------------------------------------------------
//
//  TextBox
//
//--------------------------------------------------------------------------

TextBox::TextBox ( const String& sName, const DisplayString& sCaption, Real fWidth, Real fHeight )
{
    m_pElement = OverlayManager::getSingleton ( ).createOverlayElementFromTemplate ( "SdkTrays/TextBox", "BorderPanel", sName );
    m_pElement->setWidth  ( fWidth );
    m_pElement->setHeight ( fHeight );
    
    OverlayContainer*  pContainer = (OverlayContainer*) m_pElement;
    m_pTextArea   = (TextAreaOverlayElement*)    pContainer->getChild ( getName ( ) + "/TextBoxText" );
    m_pCaptionBar = (BorderPanelOverlayElement*) pContainer->getChild ( getName ( ) + "/TextBoxCaptionBar" );
    m_pCaptionBar->setWidth ( fWidth - 4 );
    
    m_pCaptionTextArea = (TextAreaOverlayElement*) m_pCaptionBar->getChild ( m_pCaptionBar->getName ( ) + "/TextBoxCaption" );
    setCaption ( sCaption );
    
    m_pScrollTrack  = (BorderPanelOverlayElement*) pContainer->getChild ( getName ( ) + "/TextBoxScrollTrack" );
    m_pScrollHandle = (PanelOverlayElement*)   m_pScrollTrack->getChild ( m_pScrollTrack->getName ( ) + "/TextBoxScrollHandle" );
    m_pScrollHandle->hide();
    
    m_bDragging = false;
    m_fScrollPercentage = 0;
    m_uStartingLine = 0;
    m_fPadding = 15;
    m_sText = "";
//#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
//    m_pTextArea->setCharHeight ( m_pTextArea->getCharHeight ( ) - 3 );
//    m_pCaptionTextArea->setCharHeight ( m_pCaptionTextArea->getCharHeight ( ) - 3 );
//#endif
    refitContents ( );
}

KDvoid TextBox::setPadding ( Real fPadding )
{
    m_fPadding = fPadding;
    refitContents ( );
}

Real TextBox::getPadding ( KDvoid )
{
    return m_fPadding;
}

const DisplayString& TextBox::getCaption ( KDvoid )
{
    return m_pCaptionTextArea->getCaption ( );
}

KDvoid TextBox::setCaption ( const DisplayString& sCaption )
{
    m_pCaptionTextArea->setCaption ( sCaption );
}

const DisplayString& TextBox::getText ( KDvoid )
{
    return m_sText;
}

KDvoid TextBox::setText ( const DisplayString& sText )
{
    m_sText = sText;
    m_aLines.clear ( );
    
    Font*  pFont = (Font*) FontManager::getSingleton ( ).getByName ( m_pTextArea->getFontName ( ) ).getPointer ( );
    
    String  sCurrent       = DISPLAY_STRING_TO_STRING ( sText );
    bool    bFirstWord     = true;
    KDuint  uLastSpace     = 0;
    KDuint  uLineBegin     = 0;
    Real    fLineWidth     = 0;
    Real    fRightBoundary = m_pElement->getWidth ( ) - 2 * m_fPadding + m_pScrollTrack->getLeft ( ) + 10;
    
    for ( KDuint i = 0; i < sCurrent.length ( ); i++ )
    {
        if ( sCurrent[i] == ' ' )
        {
            if ( m_pTextArea->getSpaceWidth ( ) != 0 )
            {
                fLineWidth += m_pTextArea->getSpaceWidth ( );
            }
            else
            {
                fLineWidth += pFont->getGlyphAspectRatio ( ' ' ) * m_pTextArea->getCharHeight ( );
            }
            
            bFirstWord = false;
            uLastSpace = i;
        }
        else if ( sCurrent[i] == '\n' )
        {
            bFirstWord = true;
            fLineWidth = 0;
            m_aLines.push_back ( sCurrent.substr ( uLineBegin, i - uLineBegin ) );
            uLineBegin = i + 1;
        }
        else
        {
            // use glyph information to calculate line width
            fLineWidth += pFont->getGlyphAspectRatio ( sCurrent[i] ) * m_pTextArea->getCharHeight ( );
            if ( fLineWidth > fRightBoundary )
            {
                if ( bFirstWord )
                {
                    sCurrent.insert ( i, "\n" );
                    i = i - 1;
                }
                else
                {
                    sCurrent [ uLastSpace ] = '\n';
                    i = uLastSpace - 1;
                }
            }
        }
    }
    
    m_aLines.push_back ( sCurrent.substr ( uLineBegin ) );
    
    KDuint  uMaxLines = getHeightInLines ( );
    
    if ( m_aLines.size ( ) > uMaxLines )     // if too much text, filter based on scroll percentage
    {
        m_pScrollHandle->show ( );
        filterLines ( );
    }
    else       // otherwise just show all the text
    {
        m_pTextArea->setCaption ( sCurrent );
        m_pScrollHandle->hide ( );
        m_fScrollPercentage = 0;
        m_pScrollHandle->setTop ( 0 );
    }
}

KDvoid TextBox::setTextAlignment ( TextAreaOverlayElement::Alignment eAlign )
{
    if ( eAlign == TextAreaOverlayElement::Left )
    {
        m_pTextArea->setHorizontalAlignment ( GHA_LEFT );
    }
    else if ( eAlign == TextAreaOverlayElement::Center )
    {
        m_pTextArea->setHorizontalAlignment ( GHA_CENTER );
    }
    else
    {
        m_pTextArea->setHorizontalAlignment ( GHA_RIGHT );
    }
    
    refitContents ( );
}

KDvoid TextBox::clearText ( KDvoid )
{
    setText ( "" );
}

KDvoid TextBox::appendText ( const DisplayString& sText )
{
    setText ( getText ( ) + sText );
}

KDvoid TextBox::refitContents ( KDvoid )
{
    m_pScrollTrack->setHeight ( m_pElement->getHeight ( ) - m_pCaptionBar->getHeight ( ) - 20 );
    m_pScrollTrack->setTop ( m_pCaptionBar->getHeight ( ) + 10 );
    
    m_pTextArea->setTop ( m_pCaptionBar->getHeight ( ) + m_fPadding - 5 );
    
    if      ( m_pTextArea->getHorizontalAlignment ( ) == GHA_RIGHT )
    {
        m_pTextArea->setLeft ( -m_fPadding + m_pScrollTrack->getLeft ( ) );
    }
    else if ( m_pTextArea->getHorizontalAlignment ( ) == GHA_LEFT )
    {
        m_pTextArea->setLeft ( m_fPadding );
    }
    else
    {
        m_pTextArea->setLeft ( m_pScrollTrack->getLeft ( ) / 2);
    }
    
    setText ( getText ( ) );
}

KDvoid TextBox::setScrollPercentage ( Real fPercentage )
{
    m_fScrollPercentage = Math::Clamp<Real> ( fPercentage, 0, 1 );
    m_pScrollHandle->setTop ( (KDint) ( fPercentage * ( m_pScrollTrack->getHeight ( ) - m_pScrollHandle->getHeight ( ) ) ) );
    
    filterLines ( );
}

Real TextBox::getScrollPercentage ( KDvoid )
{
    return m_fScrollPercentage;
}

KDuint TextBox::getHeightInLines ( KDvoid )
{
    return (KDuint) ( ( m_pElement->getHeight ( ) - 2 * m_fPadding - m_pCaptionBar->getHeight ( ) + 5 ) / m_pTextArea->getCharHeight ( ) );
}

KDvoid TextBox::_cursorPressed ( const Vector2& tCursorPos )
{
    if ( !m_pScrollHandle->isVisible ( ) )
    {
        return;   // don't care about clicks if text not scrollable
    }
    
    Vector2  tCo = Widget::cursorOffset ( m_pScrollHandle, tCursorPos );
    
    if ( tCo.squaredLength() <= 81 )
    {
        m_bDragging = true;
        m_fDragOffset = tCo.y;
    }
    else if ( Widget::isCursorOver ( m_pScrollTrack, tCursorPos ) )
    {
        Real  fNewTop = m_pScrollHandle->getTop() + tCo.y;
        Real  fLowerBoundary = m_pScrollTrack->getHeight() - m_pScrollHandle->getHeight ( );
        m_pScrollHandle->setTop ( Math::Clamp<KDint> ( (KDint) fNewTop, 0, (KDint) fLowerBoundary ) );
        
        // update text area contents based on new scroll percentage
        m_fScrollPercentage = Math::Clamp<Real> ( fNewTop / fLowerBoundary, 0, 1 );
        filterLines ( );
    }
}

KDvoid TextBox::_cursorReleased ( const Vector2& tCursorPos )
{
    m_bDragging = false;
}

KDvoid TextBox::_cursorMoved ( const Vector2& tCursorPos )
{
    if ( m_bDragging )
    {
        Vector2 tCo = Widget::cursorOffset ( m_pScrollHandle, tCursorPos );
        Real    fNewTop = m_pScrollHandle->getTop() + tCo.y - m_fDragOffset;
        Real    fLowerBoundary = m_pScrollTrack->getHeight() - m_pScrollHandle->getHeight ( );
        
        m_pScrollHandle->setTop ( Math::Clamp<KDint> ( (KDint) fNewTop, 0, (KDint) fLowerBoundary ) );
        
        // update text area contents based on new scroll percentage
        m_fScrollPercentage = Math::Clamp<Real> ( fNewTop / fLowerBoundary, 0, 1 );
        filterLines ( );
    }
}

KDvoid TextBox::_focusLost ( KDvoid )
{
    m_bDragging = false;  // stop dragging if cursor was lost
}

KDvoid TextBox::filterLines ( KDvoid )
{
    String  sShown = "";
    KDuint  uMaxLines = getHeightInLines ( );
    KDuint  uNewStart = (KDuint) ( m_fScrollPercentage * ( m_aLines.size ( ) - uMaxLines ) + 0.5 );
    
    m_uStartingLine = uNewStart;
    
    for ( KDuint i = 0; i < uMaxLines; i++ )
    {
        sShown += m_aLines  [ m_uStartingLine + i ] + "\n";
    }
    
    m_pTextArea->setCaption ( sShown );    // show just the filtered lines
}

//--------------------------------------------------------------------------
//
//  SelectMenu
//
//--------------------------------------------------------------------------

SelectMenu::SelectMenu ( const String& sName, const DisplayString& sCaption, Real fWidth, Real fBoxWidth, KDuint uMaxItemsShown )
{
    m_nHighlightIndex   = 0;
    m_nDisplayIndex     = 0;
    m_fDragOffset       = 0.0f;
    m_nSelectionIndex   = -1;
    m_bFitToContents    = false;
    m_bCursorOver       = false;
    m_bExpanded         = false;
    m_bDragging         = false;
    m_uMaxItemsShown    = uMaxItemsShown;
    m_uItemsShown       = 0;
    m_pElement          = (BorderPanelOverlayElement*) OverlayManager::getSingleton ( ).createOverlayElementFromTemplate
                          ( "SdkTrays/SelectMenu", "BorderPanel", sName );
    m_pTextArea         = (TextAreaOverlayElement*)    ( (OverlayContainer*) m_pElement )->getChild ( sName + "/MenuCaption" );
    m_pSmallBox         = (BorderPanelOverlayElement*) ( (OverlayContainer*) m_pElement )->getChild ( sName + "/MenuSmallBox" );
    m_pSmallBox->setWidth ( fWidth - 10 );
    m_pSmallTextArea    = (TextAreaOverlayElement*) m_pSmallBox->getChild ( sName + "/MenuSmallBox/MenuSmallText" );
    m_pElement->setWidth ( fWidth );
//#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
//    m_pTextArea->setCharHeight ( m_pTextArea->getCharHeight ( ) - 3 );
//    m_pSmallTextArea->setCharHeight ( m_pSmallTextArea->getCharHeight ( ) - 3 );
//#endif
    
    if ( fBoxWidth > 0 )  // long style
    {
        if ( fWidth <= 0 )
        {
            m_bFitToContents = true;
        }
        
        m_pSmallBox->setWidth ( fBoxWidth );
        m_pSmallBox->setTop ( 2 );
        m_pSmallBox->setLeft ( fWidth - fBoxWidth - 5 );
        
        m_pElement->setHeight ( m_pSmallBox->getHeight ( ) + 4 );
        
        m_pTextArea->setHorizontalAlignment ( GHA_LEFT );
        m_pTextArea->setAlignment ( TextAreaOverlayElement::Left );
        m_pTextArea->setLeft ( 12 );
        m_pTextArea->setTop ( 10 );
    }
    
    m_pExpandedBox = (BorderPanelOverlayElement*) ( (OverlayContainer*) m_pElement )->getChild ( sName + "/MenuExpandedBox" );
    m_pExpandedBox->setWidth ( m_pSmallBox->getWidth ( ) + 10 );
    m_pExpandedBox->hide ( );
    
    m_pScrollTrack  = (BorderPanelOverlayElement*) m_pExpandedBox->getChild ( m_pExpandedBox->getName ( ) + "/MenuScrollTrack" );
    m_pScrollHandle = (PanelOverlayElement*)       m_pScrollTrack->getChild ( m_pScrollTrack->getName ( ) + "/MenuScrollHandle" );
    
    setCaption ( sCaption );
}

bool SelectMenu::isExpanded ( KDvoid )
{
    return m_bExpanded;
}

const DisplayString& SelectMenu::getCaption ( KDvoid )
{
    return m_pTextArea->getCaption ( );
}

KDvoid SelectMenu::setCaption ( const DisplayString& sCaption )
{
    m_pTextArea->setCaption ( sCaption );
    if ( m_bFitToContents )
    {
        m_pElement->setWidth ( getCaptionWidth ( sCaption, m_pTextArea ) + m_pSmallBox->getWidth ( ) + 23 );
        m_pSmallBox->setLeft ( m_pElement->getWidth ( ) - m_pSmallBox->getWidth() - 5);
    }
}

const StringVector& SelectMenu::getItems ( KDvoid )
{
    return m_aItems;
}

KDuint SelectMenu::getNumItems ( KDvoid )
{
    return m_aItems.size ( );
}

KDvoid SelectMenu::setItems ( const StringVector& aItems )
{
    m_aItems = aItems;
    m_nSelectionIndex = -1;
    
    for ( KDuint i = 0; i < m_aItemElements.size ( ); i++ )   // destroy all the item elements
    {
        nukeOverlayElement ( m_aItemElements[i] );
    }
    m_aItemElements.clear ( );
    
    m_uItemsShown = std::max<KDint> ( 2, std::min<KDint> ( m_uMaxItemsShown, m_aItems.size ( ) ) );
    
    for ( KDuint i = 0; i < m_uItemsShown; i++ )   // create all the item elements
    {
        BorderPanelOverlayElement*  e = (BorderPanelOverlayElement*) OverlayManager::getSingleton ( ).createOverlayElementFromTemplate
        ( "SdkTrays/SelectMenuItem", "BorderPanel", m_pExpandedBox->getName ( ) + "/Item" + StringConverter::toString ( i + 1 ) );
        
        e->setTop ( 6 + i * ( m_pSmallBox->getHeight ( ) - 8 ) );
        e->setWidth ( m_pExpandedBox->getWidth ( ) - 32 );
        
        m_pExpandedBox->addChild ( e );
        m_aItemElements.push_back ( e );
    }
    
    if ( !aItems.empty ( ) )
    {
        selectItem ( 0, false );
    }
    else
    {
        m_pSmallTextArea->setCaption ( "" );
    }
}

KDvoid SelectMenu::addItem ( const DisplayString& sItem )
{
    m_aItems.push_back ( sItem );
    setItems ( m_aItems );
}

KDvoid SelectMenu::removeItem ( const DisplayString& sItem )
{
    Ogre::StringVector::iterator it;
    
    for ( it = m_aItems.begin ( ); it != m_aItems.end ( ); it++ )
    {
        if ( sItem == *it )
        {
            break;
        }
    }
    
    if ( it != m_aItems.end ( ) )
    {
        m_aItems.erase ( it );
        if ( m_aItems.size ( ) < m_uItemsShown )
        {
            m_uItemsShown = m_aItems.size ( );
            nukeOverlayElement ( m_aItemElements.back ( ) );
            m_aItemElements.pop_back ( );
        }
    }
    else
    {
        String  sDesc = "Menu \"" + getName ( ) + "\" contains no item \"" + sItem + "\".";
        OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, sDesc, "SelectMenu::removeItem" );
    }
}

KDvoid SelectMenu::removeItem ( KDuint uIndex )
{
    StringVector::iterator it;
    KDuint  i = 0;
    
    for ( it = m_aItems.begin ( ); it != m_aItems.end ( ); it++ )
    {
        if ( i == uIndex )
        {
            break;
        }
        i++;
    }
    
    if ( it != m_aItems.end ( ) )
    {
        m_aItems.erase ( it );
        if ( m_aItems.size ( ) < m_uItemsShown )
        {
            m_uItemsShown = m_aItems.size ( );
            nukeOverlayElement ( m_aItemElements.back ( ) );
            m_aItemElements.pop_back ( );
        }
    }
    else
    {
        String  sDesc = "Menu \"" + getName ( ) + "\" contains no item at position " +
        StringConverter::toString ( uIndex ) + ".";
        OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, sDesc, "SelectMenu::removeItem" );
    }
}

KDvoid SelectMenu::clearItems ( KDvoid )
{
    m_aItems.clear ( );
    m_nSelectionIndex = -1;
    m_pSmallTextArea->setCaption ( "" );
}

KDvoid SelectMenu::selectItem ( KDuint uIndex, bool bNotifyListener )
{
    if ( uIndex >= m_aItems.size ( ) )
    {
        String  sDesc = "Menu \"" + getName ( ) + "\" contains no item at position " +
        StringConverter::toString ( uIndex ) + ".";
        OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, sDesc, "SelectMenu::selectItem" );
    }
    
    m_nSelectionIndex = uIndex;
    fitCaptionToArea ( m_aItems [ uIndex ], m_pSmallTextArea, m_pSmallBox->getWidth ( ) - m_pSmallTextArea->getLeft ( ) * 2 );
    
    if ( m_pListener && bNotifyListener )
    {
        m_pListener->itemSelected ( this );
    }
}

KDvoid SelectMenu::selectItem ( const DisplayString& sItem, bool bNotifyListener )
{
    for ( KDuint i = 0; i < m_aItems.size ( ); i++ )
    {
        if ( sItem == m_aItems[i] )
        {
            selectItem ( i, bNotifyListener );
            return;
        }
    }
    
    String  sDesc = "Menu \"" + getName ( ) + "\" contains no item \"" + sItem + "\".";
    OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, sDesc, "SelectMenu::selectItem" );
}

DisplayString SelectMenu::getSelectedItem ( KDvoid )
{
    if ( m_nSelectionIndex == -1 )
    {
        String  sDesc = "Menu \"" + getName ( ) + "\" has no item selected.";
        OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, sDesc, "SelectMenu::getSelectedItem" );
        return "";
    }
    else
    {
        return m_aItems [ m_nSelectionIndex ];
    }
}

KDint SelectMenu::getSelectionIndex ( KDvoid )
{
    return m_nSelectionIndex;
}

KDvoid SelectMenu::_cursorPressed ( const Vector2& tCursorPos )
{
     OverlayManager&  tOverMgr = OverlayManager::getSingleton ( );
     
     if ( m_bExpanded )
     {
         if ( m_pScrollHandle->isVisible ( ) )   // check for scrolling
         {
             Vector2  tCo = Widget::cursorOffset ( m_pScrollHandle, tCursorPos );
     
             if ( tCo.squaredLength ( ) <= 81 )
             {
                 m_bDragging = true;
                 m_fDragOffset = tCo.y;
                 return;
             }
             else if ( Widget::isCursorOver ( m_pScrollTrack, tCursorPos ) )
             {
                 Real  fNewTop = m_pScrollHandle->getTop ( ) + tCo.y;
                 Real  fLowerBoundary = m_pScrollTrack->getHeight ( ) - m_pScrollHandle->getHeight ( );
                 m_pScrollHandle->setTop ( Math::Clamp<KDint> ( (KDint) fNewTop, 0, (KDint) fLowerBoundary ) );
     
                 Real  fScrollPercentage = Math::Clamp<Real> ( fNewTop / fLowerBoundary, 0, 1 );
                 setDisplayIndex ( (KDint) ( fScrollPercentage * ( m_aItems.size ( ) - m_aItemElements.size ( ) ) + 0.5 ) );
                 return;
             }
         }
         
         if ( !isCursorOver ( m_pExpandedBox, tCursorPos, 3 ) )
         {
             retract ( );
         }
         else
         {
             Real  l = m_aItemElements.front ( )->_getDerivedLeft ( ) * tOverMgr.getViewportWidth  ( ) + 5;
             Real  t = m_aItemElements.front ( )->_getDerivedTop  ( ) * tOverMgr.getViewportHeight ( ) + 5;
             Real  r = l + m_aItemElements.back ( )->getWidth ( ) - 10;
             Real  b = m_aItemElements.back  ( )->_getDerivedTop  ( ) * tOverMgr.getViewportHeight ( ) + m_aItemElements.back ( )->getHeight ( ) - 5;
     
             if ( tCursorPos.x >= l && tCursorPos.x <= r && tCursorPos.y >= t && tCursorPos.y <= b )
             {
                 if ( m_nHighlightIndex != m_nSelectionIndex )
                 {
                     selectItem ( m_nHighlightIndex );
                 }
                 
                 retract ( );
             }
         }
     }
     else
     {
         if ( m_aItems.size ( ) < 2 )
         {
             return;   // don't waste time showing a menu if there's no choice
         }
     
         if ( isCursorOver ( m_pSmallBox, tCursorPos, 4 ) )
         {
             m_pExpandedBox->show ( );
             m_pSmallBox->hide ( );
     
             // calculate how much vertical space we need
             Real  fIdealHeight = m_uItemsShown * ( m_pSmallBox->getHeight ( ) - 8 ) + 20;
             m_pExpandedBox->setHeight ( fIdealHeight );
             m_pScrollTrack->setHeight ( m_pExpandedBox->getHeight() - 20 );
     
             m_pExpandedBox->setLeft ( m_pSmallBox->getLeft ( ) - 4 );
     
             // if the expanded menu goes down off the screen, make it go up instead
             if ( m_pSmallBox->_getDerivedTop() * tOverMgr.getViewportHeight ( ) + fIdealHeight > tOverMgr.getViewportHeight ( ) )
             {
                 m_pExpandedBox->setTop ( m_pSmallBox->getTop ( ) + m_pSmallBox->getHeight ( ) - fIdealHeight + 3 );
                 // if we're in thick style, hide the caption because it will interfere with the expanded menu
                 if ( m_pTextArea->getHorizontalAlignment ( ) == GHA_CENTER )
                 {
                     m_pTextArea->hide ( );
                 }
             }
             else
             {
                 m_pExpandedBox->setTop ( m_pSmallBox->getTop ( ) + 3 );
             }
     
             m_bExpanded = true;
             m_nHighlightIndex = m_nSelectionIndex;
             setDisplayIndex ( m_nHighlightIndex );
     
             if ( m_uItemsShown < m_aItems.size ( ) )  // update scrollbar position
             {
                 m_pScrollHandle->show ( );
                 Real  fLowerBoundary = m_pScrollTrack->getHeight ( ) - m_pScrollHandle->getHeight ( );
                 m_pScrollHandle->setTop ( (KDint) ( m_nDisplayIndex * fLowerBoundary / ( m_aItems.size ( ) - m_aItemElements.size ( ) ) ) );
             }
             else
             {
                 m_pScrollHandle->hide ( );
             }
         }          
     }
}

KDvoid SelectMenu::_cursorReleased ( const Vector2& tCursorPos )
{
    m_bDragging = false;
}

KDvoid SelectMenu::_cursorMoved ( const Vector2& tCursorPos )
{
    OverlayManager&  tOverMgr = OverlayManager::getSingleton ( );
    
    if ( m_bExpanded )
    {
        if ( m_bDragging )
        {
            Vector2  tCo = Widget::cursorOffset ( m_pScrollHandle, tCursorPos );
            
            Real    fNewTop = m_pScrollHandle->getTop ( ) + tCo.y - m_fDragOffset;
            Real    fLowerBoundary = m_pScrollTrack->getHeight ( ) - m_pScrollHandle->getHeight ( );
            m_pScrollHandle->setTop ( Math::Clamp<KDint> ( (KDint) fNewTop, 0, (KDint) fLowerBoundary ) );
            
            Real    fScrollPercentage = Math::Clamp<Ogre::Real> ( fNewTop / fLowerBoundary, 0, 1 );
            KDint   nNewIndex = (KDint) ( fScrollPercentage * ( m_aItems.size ( ) - m_aItemElements.size ( ) ) + 0.5 );
            if ( nNewIndex != m_nDisplayIndex )
            {
                setDisplayIndex ( nNewIndex );
            }
            return;
        }
        
        Real  l = m_aItemElements.front ( )->_getDerivedLeft ( ) * tOverMgr.getViewportWidth  ( ) + 5;
        Real  t = m_aItemElements.front ( )->_getDerivedTop  ( ) * tOverMgr.getViewportHeight ( ) + 5;
        Real  r = l + m_aItemElements.back ( )->getWidth ( ) - 10;
        Real  b = m_aItemElements.back ( )->_getDerivedTop ( ) * tOverMgr.getViewportHeight ( ) + m_aItemElements.back ( )->getHeight ( ) - 5;
        
        if ( tCursorPos.x >= l && tCursorPos.x <= r && tCursorPos.y >= t && tCursorPos.y <= b )
        {
            KDint  nNewIndex = (KDint) ( m_nDisplayIndex + ( tCursorPos.y - t ) / ( b - t ) * m_aItemElements.size ( ) );
            if ( m_nHighlightIndex != nNewIndex )
            {
                m_nHighlightIndex = nNewIndex ;
                setDisplayIndex ( m_nDisplayIndex );
            }
        }
    }
    else
    {
        if ( isCursorOver ( m_pSmallBox, tCursorPos, 4 ) )
        {
            m_pSmallBox->setMaterialName       ( "SdkTrays/MiniTextBox/Over" );
            m_pSmallBox->setBorderMaterialName ( "SdkTrays/MiniTextBox/Over" );
            m_bCursorOver = true;
        }
        else
        {
            if ( m_bCursorOver )
            {
                m_pSmallBox->setMaterialName       ( "SdkTrays/MiniTextBox" );
                m_pSmallBox->setBorderMaterialName ( "SdkTrays/MiniTextBox" );
                m_bCursorOver = false;
            }
        }
    }
}

KDvoid SelectMenu::_focusLost ( KDvoid )
{
    if ( m_pExpandedBox->isVisible ( ) )
    {
        retract ( );
    }
}

KDvoid SelectMenu::setDisplayIndex ( KDuint uIndex )
{
    uIndex = std::min<KDint> ( uIndex, m_aItems.size ( ) - m_aItemElements.size ( ) );
    m_nDisplayIndex = uIndex;
    
    BorderPanelOverlayElement* ie;
    TextAreaOverlayElement*    ta;
    
    for ( KDuint i = 0; i < m_aItemElements.size ( ); i++ )
    {
        ie = m_aItemElements[i];
        ta = (TextAreaOverlayElement*) ie->getChild ( ie->getName ( ) + "/MenuItemText" );
        
        fitCaptionToArea ( m_aItems [ m_nDisplayIndex + i ], ta, ie->getWidth ( ) - 2 * ta->getLeft ( ) );
        
        if ( ( m_nDisplayIndex + i ) == m_nHighlightIndex )
        {
            ie->setMaterialName       ( "SdkTrays/MiniTextBox/Over" );
            ie->setBorderMaterialName ( "SdkTrays/MiniTextBox/Over" );
        }
        else
        {
            ie->setMaterialName       ( "SdkTrays/MiniTextBox" );
            ie->setBorderMaterialName ( "SdkTrays/MiniTextBox" );
        }
    }
}

KDvoid SelectMenu::retract ( KDvoid )
{
    m_bDragging = false;
    m_bExpanded = false;
    m_pExpandedBox->hide ( );
    m_pTextArea->show ( );
    m_pSmallBox->show ( );
    m_pSmallBox->setMaterialName ( "SdkTrays/MiniTextBox" );
    m_pSmallBox->setBorderMaterialName ( "SdkTrays/MiniTextBox" );
}

//--------------------------------------------------------------------------
//
//  Label
//
//--------------------------------------------------------------------------

// Do not instantiate any widgets directly. Use SdkTrayManager.
Label::Label ( const String& sName, const DisplayString& sCaption, Real fWidth )
{
    m_pElement  = OverlayManager::getSingleton ( ).createOverlayElementFromTemplate ( "SdkTrays/Label", "BorderPanel", sName );
    m_pTextArea = (TextAreaOverlayElement*) ( (OverlayContainer*) m_pElement )->getChild ( getName ( ) + "/LabelCaption" );
    //#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
    //        m_pTextArea->setCharHeight(mTextArea->getCharHeight() - 3);
    //#endif
    setCaption ( sCaption );
    if ( fWidth <= 0 )
    {
        m_bFitToTray = true;
    }
    else
    {
        m_bFitToTray = false;
        m_pElement->setWidth ( fWidth );
    }
}

const DisplayString& Label::getCaption ( KDvoid )
{
    return m_pTextArea->getCaption ( );
}

KDvoid Label::setCaption ( const DisplayString& sCaption )
{
    m_pTextArea->setCaption ( sCaption );
}

KDvoid Label::_cursorPressed ( const Vector2& tCursorPos )
{
    if ( m_pListener && isCursorOver ( m_pElement, tCursorPos, 3 ) )
    {
        m_pListener->labelHit ( this );
    }
}

bool Label::_isFitToTray ( KDvoid )
{
    return m_bFitToTray;
}

//--------------------------------------------------------------------------
//
//  Separator
//
//--------------------------------------------------------------------------

Separator::Separator ( const String& sName, Real fWidth )
{
    m_pElement = OverlayManager::getSingleton ( ).createOverlayElementFromTemplate ( "SdkTrays/Separator", "Panel", sName );
    
    if ( fWidth <= 0 )
    {
        m_bFitToTray = true;
    }
    else
    {
        m_bFitToTray = false;
        m_pElement->setWidth ( fWidth );
    }
}

bool Separator::_isFitToTray ( KDvoid )
{
    return m_bFitToTray;
}

//--------------------------------------------------------------------------
//
//  Slider
//
//--------------------------------------------------------------------------

Slider::Slider ( const String& sName, const DisplayString& sCaption, Real fWidth, Real fTrackWidth,
                 Real fValueBoxWidth, Real fMinValue, Real fMaxValue, KDuint uSnaps )
{
 
    m_fDragOffset    = 0.0f;
    m_fValue         = 0.0f;
    m_fMinValue      = 0.0f;
    m_fMaxValue      = 0.0f;
    m_fInterval      = 0.0f;
    m_bDragging      = false;
    m_bFitToContents = false;
    m_pElement       = OverlayManager::getSingleton ( ).createOverlayElementFromTemplate ( "SdkTrays/Slider", "BorderPanel", sName );
    m_pElement->setWidth ( fWidth );
    
    OverlayContainer*  pContainer = (OverlayContainer*) m_pElement;
    m_pTextArea = (Ogre::TextAreaOverlayElement*) pContainer->getChild(getName() + "/SliderCaption");
    
    OverlayContainer*  pValueBox = (Ogre::OverlayContainer*) pContainer->getChild ( getName ( ) + "/SliderValueBox" );
    pValueBox->setWidth ( fValueBoxWidth );
    pValueBox->setLeft  ( -( fValueBoxWidth + 5 ) );
    
    m_pValueTextArea = (TextAreaOverlayElement*) pValueBox->getChild ( pValueBox->getName ( ) + "/SliderValueText" );
    m_pTrack  = (BorderPanelOverlayElement*) pContainer->getChild ( getName ( ) + "/SliderTrack" );
    m_pHandle = (PanelOverlayElement*) m_pTrack->getChild ( m_pTrack->getName() + "/SliderHandle" );

//#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
//    m_pTextArea     ->setCharHeight ( m_pTextArea     ->getCharHeight ( ) - 3 );
//    m_pValueTextArea->setCharHeight ( m_pValueTextArea->getCharHeight ( ) - 3 );
//#endif
    
    if ( fTrackWidth <= 0)  // tall style
    {
        m_pTrack->setWidth ( fWidth - 16 );
    }
    else  // long style
    {
        if ( fWidth <= 0 )
        {
            m_bFitToContents = true;
        }
        
        m_pElement->setHeight ( 34 );
        m_pTextArea->setTop ( 10 );
        pValueBox->setTop ( 2 );
        m_pTrack->setTop ( -23 );
        m_pTrack->setWidth ( fTrackWidth );
        m_pTrack->setHorizontalAlignment ( GHA_RIGHT );
        m_pTrack->setLeft ( - ( fTrackWidth + fValueBoxWidth + 5 ) );
    }
    
    setCaption ( sCaption );
    setRange ( fMinValue, fMaxValue, uSnaps, false );
}

KDvoid Slider::setRange ( Real fMinValue, Real fMaxValue, KDuint uSnaps, bool bNotifyListener )
{
    m_fMinValue = fMinValue;
    m_fMaxValue = fMaxValue;
    
    if ( uSnaps <= 1 || m_fMinValue >= m_fMaxValue )
    {
        m_fInterval = 0;
        m_pHandle->hide ( );
        m_fValue = fMinValue;
        if ( uSnaps == 1 )
        {
            m_pValueTextArea->setCaption ( StringConverter::toString ( m_fMinValue ) );
        }
        else
        {
            m_pValueTextArea->setCaption ( "" );
        }
    }
    else
    {
        m_pHandle->show ( );
        m_fInterval = ( fMaxValue - fMinValue ) / ( uSnaps - 1 );
        setValue ( fMinValue, bNotifyListener );
    }
}

const DisplayString& Slider::getValueCaption ( KDvoid )
{
    return m_pValueTextArea->getCaption ( );
}

KDvoid Slider::setValueCaption ( const DisplayString& sCaption )
{
    m_pValueTextArea->setCaption ( sCaption );
}

KDvoid Slider::setValue ( Real fValue, bool bNotifyListener )
{
    if ( m_fInterval == 0 )
    {
        return;
    }
    
    m_fValue = Math::Clamp<Real> ( fValue, m_fMinValue, m_fMaxValue );
    
    setValueCaption ( StringConverter::toString ( m_fValue ) );
    
    if ( m_pListener && bNotifyListener )
    {
        m_pListener->sliderMoved ( this );
    }
    
    if ( !m_bDragging )
    {
        m_pHandle->setLeft ( (KDint) ( ( m_fValue - m_fMinValue ) / ( m_fMaxValue - m_fMinValue ) * ( m_pTrack->getWidth ( ) - m_pHandle->getWidth ( ) ) ) );
    }
}

Real Slider::getValue ( KDvoid )
{
    return m_fValue;
}

const DisplayString& Slider::getCaption ( KDvoid )
{
    return m_pTextArea->getCaption ( );
}

KDvoid Slider::setCaption ( const DisplayString& sCaption )
{
    m_pTextArea->setCaption ( sCaption );
    
    if ( m_bFitToContents )
    {
        m_pElement->setWidth ( getCaptionWidth ( sCaption, m_pTextArea ) + m_pValueTextArea->getParent ( )->getWidth ( ) + m_pTrack->getWidth ( ) + 26 );
    }
}

KDvoid Slider::_cursorPressed ( const Vector2& tCursorPos )
{
    if ( !m_pHandle->isVisible ( ) )
    {
        return;
    }
    
    Vector2  tCo = Widget::cursorOffset ( m_pHandle, tCursorPos );
    
    if ( tCo.squaredLength ( ) <= 81 )
    {
        m_bDragging   = true;
        m_fDragOffset = tCo.x;
    }
    else if ( Widget::isCursorOver ( m_pTrack, tCursorPos ) )
    {
        Real  fNewLeft = m_pHandle->getLeft ( ) + tCo.x;
        Real  fRightBoundary = m_pTrack->getWidth ( ) - m_pHandle->getWidth ( );
        
        m_pHandle->setLeft ( Math::Clamp<KDint> ( (KDint) fNewLeft, 0, (KDint) fRightBoundary ) );
        setValue ( getSnappedValue ( fNewLeft / fRightBoundary ) );
    }     
}

KDvoid Slider::_cursorReleased ( const Vector2& tCursorPos )
{
    if ( m_bDragging )
    {
        m_bDragging = false;
        m_pHandle->setLeft ( (KDint) ( ( m_fValue - m_fMinValue ) / ( m_fMaxValue - m_fMinValue ) * ( m_pTrack->getWidth ( ) - m_pHandle->getWidth ( ) ) ) );
    }
}

KDvoid Slider::_cursorMoved ( const Vector2& tCursorPos )
{
    if ( m_bDragging )
    {
        Vector2  tCo = Widget::cursorOffset ( m_pHandle, tCursorPos );
        Real  fNewLeft = m_pHandle->getLeft() + tCo.x - m_fDragOffset;
        Real  fRightBoundary = m_pTrack->getWidth ( ) - m_pHandle->getWidth ( );
        
        m_pHandle->setLeft ( Math::Clamp<KDint> ( (KDint) fNewLeft, 0, (KDint) fRightBoundary ) );
        setValue ( getSnappedValue ( fNewLeft / fRightBoundary ) );
    }
}

KDvoid Slider::_focusLost ( KDvoid )
{
    m_bDragging = false;
}

Real Slider::getSnappedValue ( Real fPercentage )
{
    fPercentage = Math::Clamp<Real> ( fPercentage, 0, 1 );
    KDuint uWhichMarker = (KDuint) ( fPercentage * ( m_fMaxValue - m_fMinValue ) / m_fInterval + 0.5 );
    return uWhichMarker * m_fInterval + m_fMinValue;
}

//--------------------------------------------------------------------------
//
//  ParamsPanel
//
//--------------------------------------------------------------------------

ParamsPanel::ParamsPanel ( const String& sName, Real fWidth, KDuint uLines )
{
    m_pElement = OverlayManager::getSingleton ( ).createOverlayElementFromTemplate
    (
        "SdkTrays/ParamsPanel", "BorderPanel", sName
    );
    
    Ogre::OverlayContainer*  pContainer = (OverlayContainer*) m_pElement;
    m_pNamesArea  = (TextAreaOverlayElement*) pContainer->getChild ( getName ( ) + "/ParamsPanelNames"  );
    m_pValuesArea = (TextAreaOverlayElement*) pContainer->getChild ( getName ( ) + "/ParamsPanelValues" );
//#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
//    m_pNamesArea ->setCharHeight (m_pNamesArea ->getCharHeight ( ) - 3 );
//    m_pValuesArea->setCharHeight (m_pValuesArea->getCharHeight ( ) - 3 );
//#endif
    m_pElement->setWidth  ( fWidth );
    m_pElement->setHeight ( m_pNamesArea->getTop ( ) * 2 + uLines * m_pNamesArea->getCharHeight ( ) );
}

KDvoid ParamsPanel::setAllParamNames ( const StringVector& aParamNames )
{
    m_aNames = aParamNames;
    m_aValues.clear ( );
    m_aValues.resize ( m_aNames.size ( ), "" );
    m_pElement->setHeight ( m_pNamesArea->getTop ( ) * 2 + m_aNames.size ( ) * m_pNamesArea->getCharHeight ( ) );
    updateText ( );
}

const StringVector& ParamsPanel::getAllParamNames ( KDvoid )
{
    return m_aNames;
}

KDvoid ParamsPanel::setAllParamValues ( const StringVector& aParamValues )
{
    m_aValues = aParamValues;
    m_aValues.resize ( m_aNames.size ( ), "" );
    updateText ( );
}

KDvoid ParamsPanel::setParamValue ( const DisplayString& sParamName, const DisplayString& sParamValue )
{
    for ( KDuint i = 0; i < m_aNames.size ( ); i++ )
    {
        if ( m_aNames[i] == DISPLAY_STRING_TO_STRING ( sParamName ) )
        {
            m_aValues[i] = DISPLAY_STRING_TO_STRING ( sParamValue );
            updateText ( );
            return;
        }
    }
    
    String  sDesc = "ParamsPanel \"" + getName ( ) + "\" has no parameter \"" + DISPLAY_STRING_TO_STRING ( sParamName ) + "\".";
    OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, sDesc, "ParamsPanel::setParamValue" );
}

KDvoid ParamsPanel::setParamValue ( KDuint uIndex, const DisplayString& sParamValue )
{
    if ( uIndex >= m_aNames.size ( ) )
    {
        String  sDesc = "ParamsPanel \"" + getName ( ) + "\" has no parameter at position " +
        StringConverter::toString ( uIndex ) + ".";
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, sDesc, "ParamsPanel::setParamValue" );
    }
    
    m_aValues [ uIndex ] = DISPLAY_STRING_TO_STRING ( sParamValue );
    updateText ( );
}

DisplayString ParamsPanel::getParamValue ( const DisplayString& sParamName )
{
    for ( KDuint i = 0; i < m_aNames.size ( ); i++ )
    {
        if ( m_aNames[i] == DISPLAY_STRING_TO_STRING ( sParamName ) )
        {
            return m_aValues[i];
        }
    }
    
    String  sDesc = "ParamsPanel \"" + getName ( ) + "\" has no parameter \"" + DISPLAY_STRING_TO_STRING ( sParamName ) + "\".";
    OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, sDesc, "ParamsPanel::getParamValue" );
     
    return "";
}

DisplayString ParamsPanel::getParamValue ( KDuint uIndex )
{
    if ( uIndex >= m_aNames.size ( ) )
    {
        String  sDesc = "ParamsPanel \"" + getName ( ) + "\" has no parameter at position " + StringConverter::toString ( uIndex ) + ".";
        OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, sDesc, "ParamsPanel::getParamValue" );
    }
    
    return m_aValues [ uIndex ];
}

const StringVector& ParamsPanel::getAllParamValues ( KDvoid )
{
    return m_aValues;
}

KDvoid ParamsPanel::updateText ( KDvoid )
{
    DisplayString   sNamesDS;
    DisplayString  sValuesDS;
    
    for ( KDuint i = 0; i < m_aNames.size ( ); i++ )
    {
         sNamesDS.append ( m_aNames [ i ] + ":\n" );
        sValuesDS.append ( m_aValues[ i ] + "\n"  );
    }
    
    m_pNamesArea ->setCaption ( sNamesDS  );
    m_pValuesArea->setCaption ( sValuesDS );
}

//--------------------------------------------------------------------------
//
//  CheckBox
//
//--------------------------------------------------------------------------

CheckBox::CheckBox ( const String& sName, const DisplayString& sCaption, Real fWidth )
{
    m_bCursorOver    = false;
    m_bFitToContents = fWidth <= 0;
    m_pElement = OverlayManager::getSingleton ( ).createOverlayElementFromTemplate
    (
        "SdkTrays/CheckBox", "BorderPanel", sName
    );
    OverlayContainer*  pContainer = (OverlayContainer*) m_pElement;
    m_pTextArea = (TextAreaOverlayElement*)    pContainer->getChild ( getName ( ) + "/CheckBoxCaption" );
    m_pSquare   = (BorderPanelOverlayElement*) pContainer->getChild ( getName ( ) + "/CheckBoxSquare"  );
    m_pX        = m_pSquare->getChild ( m_pSquare->getName ( ) + "/CheckBoxX" );
    m_pX->hide ( );
    m_pElement->setWidth ( fWidth );
    
//#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
//    m_pTextArea->setCharHeight ( m_pTextArea->getCharHeight ( ) - 3 );
//#endif
    setCaption ( sCaption );
}

const DisplayString& CheckBox::getCaption ( KDvoid )
{
    return m_pTextArea->getCaption ( );
}

KDvoid CheckBox::setCaption ( const DisplayString& sCaption )
{
    m_pTextArea->setCaption ( sCaption );
    if ( m_bFitToContents )
    {
        m_pElement->setWidth ( getCaptionWidth ( sCaption, m_pTextArea ) + m_pSquare->getWidth ( ) + 23 );
    }
}

bool CheckBox::isChecked ( KDvoid )
{
    return m_pX->isVisible ( );
}

KDvoid CheckBox::setChecked ( bool bChecked, bool bNotifyListener )
{
    if ( bChecked )
    {
        m_pX->show ( );
    }
    else
    {
        m_pX->hide ( );
    }
    if ( m_pListener && bNotifyListener )
    {
        m_pListener->checkBoxToggled ( this );
    }
}

KDvoid CheckBox::toggle ( bool bNotifyListener )
{
    setChecked ( !isChecked ( ), bNotifyListener );
}

KDvoid CheckBox::_cursorPressed ( const Vector2& tCursorPos )
{
    if ( m_bCursorOver && m_pListener )
    {
        toggle ( );
    }
}

KDvoid CheckBox::_cursorMoved ( const Vector2& tCursorPos )
{
     if ( isCursorOver ( m_pSquare, tCursorPos, 5 ) )
     {
         if ( !m_bCursorOver )
        {
            m_bCursorOver = true;
            m_pSquare->setMaterialName       ( "SdkTrays/MiniTextBox/Over" );
            m_pSquare->setBorderMaterialName ( "SdkTrays/MiniTextBox/Over" );
        }
     }
     else
     {
         if ( m_bCursorOver )
         {
             m_bCursorOver = false;
             m_pSquare->setMaterialName       ( "SdkTrays/MiniTextBox" );
             m_pSquare->setBorderMaterialName ( "SdkTrays/MiniTextBox" );
         }
     }
}

KDvoid CheckBox::_focusLost ( KDvoid )
{
     m_pSquare->setMaterialName       ( "SdkTrays/MiniTextBox" );
     m_pSquare->setBorderMaterialName ( "SdkTrays/MiniTextBox" );
     m_bCursorOver = false;
}

//--------------------------------------------------------------------------
//
//  DecorWidget
//
//--------------------------------------------------------------------------

DecorWidget::DecorWidget ( const String& sName, const String& sTemplateName )
{
    m_pElement = OverlayManager::getSingleton ( ).createOverlayElementFromTemplate ( sTemplateName, "", sName );
}

//--------------------------------------------------------------------------
//
//  ProgressBar
//
//--------------------------------------------------------------------------

ProgressBar::ProgressBar ( const String& sName, const DisplayString& sCaption, Real fWidth, Real fCommentBoxWidth )
{
    m_fProgress = 0.0f;
    m_pElement = OverlayManager::getSingleton().createOverlayElementFromTemplate ( "SdkTrays/ProgressBar", "BorderPanel", sName );
    m_pElement->setWidth ( fWidth );
    
    OverlayContainer*  pContainer = (OverlayContainer*) m_pElement;
    m_pTextArea = (TextAreaOverlayElement*) pContainer->getChild ( getName ( ) + "/ProgressCaption" );
    OverlayContainer*  pCommentBox = (OverlayContainer*) pContainer->getChild ( getName ( ) + "/ProgressCommentBox" );
    pCommentBox->setWidth ( fCommentBoxWidth );
    pCommentBox->setLeft  ( -( fCommentBoxWidth + 5 ) );
    m_pCommentTextArea = (TextAreaOverlayElement*) pCommentBox->getChild ( pCommentBox->getName ( ) + "/ProgressCommentText" );
    m_pMeter = pContainer->getChild ( getName ( ) + "/ProgressMeter" );
    m_pMeter->setWidth ( fWidth - 10 );
    m_pFill = ( (OverlayContainer*) m_pMeter )->getChild ( m_pMeter->getName ( ) + "/ProgressFill" );
    setCaption ( sCaption );
}

KDvoid ProgressBar::setProgress ( Real fProgress )
{
    m_fProgress = Math::Clamp<Real> ( fProgress, 0, 1 );
    m_pFill->setWidth(std::max<KDint> ( (KDint) m_pFill->getHeight ( ), (KDint) ( m_fProgress * ( m_pMeter->getWidth ( ) - 2 * m_pFill->getLeft ( ) ) ) ) );
}

Real ProgressBar::getProgress ( KDvoid )
{
    return m_fProgress;
}

const DisplayString& ProgressBar::getCaption ( KDvoid )
{
    return m_pTextArea->getCaption ( );
}

KDvoid ProgressBar::setCaption ( const DisplayString& sCaption )
{
    m_pTextArea->setCaption ( sCaption );
}

const DisplayString& ProgressBar::getComment ( KDvoid )
{
    return m_pCommentTextArea->getCaption ( );
}

KDvoid ProgressBar::setComment ( const DisplayString& sComment )
{
    m_pCommentTextArea->setCaption ( sComment );
}

//--------------------------------------------------------------------------
//
//  SdkTrayManager
//
//--------------------------------------------------------------------------

SdkTrayManager::SdkTrayManager ( const String& sName, RenderWindow* pWindow, SdkTrayListener* pListener )
{
    m_sName                 = sName;
    m_pWindow               = pWindow;
    m_pListener             = pListener;
    m_fWidgetPadding        = 8;
    m_fWidgetSpacing        = 2;
    m_fTrayPadding          = 0;
    m_bTrayDrag             = false;
    m_pExpandedMenu         = 0;
    m_pDialog               = 0;
    m_pOk                   = 0;
    m_pYes                  = 0;
    m_pNo                   = 0;
    m_bCursorWasVisible     = false;
    m_pFpsLabel             = 0;
    m_pStatsPanel           = 0;
    m_pLogo                 = 0;
    m_pLoadBar              = 0;
    m_fGroupInitProportion  = 0.0f;
    m_fGroupLoadProportion  = 0.0f;
    m_fLoadInc              = 0.0f;
    
    m_pTimer                = Ogre::Root::getSingleton ( ).getTimer ( );
    m_uLastStatUpdateTime   = 0;
        
    OverlayManager&  tOverMgr = Ogre::OverlayManager::getSingleton ( );
        
    String  sNameBase = m_sName + "/";
    std::replace ( sNameBase.begin ( ), sNameBase.end ( ), ' ', '_' );
   
    // create overlay layers for everything
    m_pBackdropLayer = tOverMgr.create ( sNameBase + "BackdropLayer" );
    m_pTraysLayer    = tOverMgr.create ( sNameBase + "WidgetsLayer"  );
    m_pPriorityLayer = tOverMgr.create ( sNameBase + "PriorityLayer" );
    m_pCursorLayer   = tOverMgr.create ( sNameBase + "CursorLayer"   );
    
    m_pBackdropLayer->setZOrder ( 100 );
    m_pTraysLayer   ->setZOrder ( 200 );
    m_pPriorityLayer->setZOrder ( 300 );
    m_pCursorLayer  ->setZOrder ( 400 );

    // make backdrop and cursor overlay containers
    m_pCursor = (OverlayContainer*) tOverMgr.createOverlayElementFromTemplate ( "SdkTrays/Cursor", "Panel", sNameBase + "Cursor" );
    m_pCursorLayer->add2D ( m_pCursor );
    
    m_pBackdrop = (OverlayContainer*) tOverMgr.createOverlayElement ( "Panel", sNameBase + "Backdrop" );
    m_pBackdropLayer->add2D ( m_pBackdrop );
    
    m_pDialogShade = (OverlayContainer*) tOverMgr.createOverlayElement ( "Panel", sNameBase + "DialogShade" );
    m_pDialogShade->setMaterialName ( "SdkTrays/Shade" );
    m_pDialogShade->hide ( );
    
    m_pPriorityLayer->add2D ( m_pDialogShade );

    String  sTrayNames [ ] = { "TopLeft", "Top", "TopRight", "Left", "Center", "Right", "BottomLeft", "Bottom", "BottomRight" };
        
    for ( KDuint i = 0; i < 9; i++ )    // make the real trays
    {
        m_pTrays [ i ] = (OverlayContainer*) tOverMgr.createOverlayElementFromTemplate ( "SdkTrays/Tray", "BorderPanel", sNameBase + sTrayNames[i] + "Tray" );
        m_pTraysLayer->add2D ( m_pTrays[i] );
            
        m_eTrayWidgetAlign[i] = Ogre::GHA_CENTER;
            
        // align trays based on location
        if ( i == TL_TOP        || i == TL_CENTER || i == TL_BOTTOM      ) m_pTrays [ i ]->setHorizontalAlignment ( GHA_CENTER );
        if ( i == TL_LEFT       || i == TL_CENTER || i == TL_RIGHT       ) m_pTrays [ i ]->setVerticalAlignment   ( GVA_CENTER );
        if ( i == TL_TOPRIGHT   || i == TL_RIGHT  || i == TL_BOTTOMRIGHT ) m_pTrays [ i ]->setHorizontalAlignment ( GHA_RIGHT  );
        if ( i == TL_BOTTOMLEFT || i == TL_BOTTOM || i == TL_BOTTOMRIGHT ) m_pTrays [ i ]->setVerticalAlignment   ( GVA_BOTTOM );
    }
    
    // create the null tray for free-floating widgets
    m_pTrays[9] = (OverlayContainer*) tOverMgr.createOverlayElement ( "Panel", sNameBase + "NullTray" );
    m_eTrayWidgetAlign[9] = Ogre::GHA_LEFT;
    m_pTraysLayer->add2D ( m_pTrays[9] );
    adjustTrays ( );
    
    showTrays  ( );
    showCursor ( );
}

SdkTrayManager::~SdkTrayManager ( KDvoid )
{
    Ogre::OverlayManager&  tOverMgr = Ogre::OverlayManager::getSingleton ( );
    
    destroyAllWidgets ( );
    
    for ( KDuint i = 0; i < m_aWidgetDeathRow.size ( ); i++ )   // delete widgets queued for destruction
    {
        delete m_aWidgetDeathRow [ i ];
    }
    m_aWidgetDeathRow.clear ( );
    
    tOverMgr.destroy ( m_pBackdropLayer );
    tOverMgr.destroy ( m_pTraysLayer );
    tOverMgr.destroy ( m_pPriorityLayer );
    tOverMgr.destroy ( m_pCursorLayer );
    
    closeDialog ( );
    hideLoadingBar ( );
    
    Widget::nukeOverlayElement ( m_pBackdrop );
    Widget::nukeOverlayElement ( m_pCursor );
    Widget::nukeOverlayElement ( m_pDialogShade );
    
    for ( KDuint i = 0; i < 10; i++ )
    {
        Widget::nukeOverlayElement ( m_pTrays[i] );
    }
}

Ray SdkTrayManager::screenToScene ( Camera* pCamera, const Vector2& tPt )
{
    return pCamera->getCameraToViewportRay ( tPt.x, tPt.y );
}

Vector2 SdkTrayManager::sceneToScreen ( Camera* pCamera, const Vector3& tPt )
{
    Vector3  tResult = pCamera->getProjectionMatrix ( ) * pCamera->getViewMatrix ( ) * tPt;
    return Vector2 ( ( tResult.x + 1 ) / 2, ( -tResult.y + 1 ) / 2 );
}

OverlayContainer* SdkTrayManager::getTrayContainer ( TrayLocation tTrayLoc )
{
    return m_pTrays [ tTrayLoc ];
}

Overlay* SdkTrayManager::getBackdropLayer ( KDvoid )
{
    return m_pBackdropLayer;
}

Overlay* SdkTrayManager::getTraysLayer ( KDvoid )
{
    return m_pTraysLayer;
}

Overlay* SdkTrayManager::getCursorLayer ( KDvoid )
{
    return m_pCursorLayer;
}

OverlayContainer* SdkTrayManager::getBackdropContainer ( KDvoid )
{
    return m_pBackdrop;
}

OverlayContainer* SdkTrayManager::getCursorContainer ( KDvoid )
{
    return m_pCursor;
}

OverlayElement* SdkTrayManager::getCursorImage ( KDvoid )
{
    return m_pCursor->getChild ( m_pCursor->getName ( ) + "/CursorImage" );
}

KDvoid SdkTrayManager::setListener ( SdkTrayListener* pListener )
{
    m_pListener = pListener;
}

SdkTrayListener* SdkTrayManager::getListener ( KDvoid )
{
    return m_pListener;
}

KDvoid SdkTrayManager::showAll ( KDvoid )
{
    showBackdrop ( );
    showTrays    ( );
    showCursor   ( );
}

KDvoid SdkTrayManager::hideAll ( KDvoid )
{
    hideBackdrop ( );
    hideTrays    ( );
    hideCursor   ( );
}

KDvoid SdkTrayManager::showBackdrop ( const String& sMaterialName )
{
    if ( sMaterialName != StringUtil::BLANK )
    {
        m_pBackdrop->setMaterialName ( sMaterialName );
    }
    
    m_pBackdropLayer->show ( );
}

KDvoid SdkTrayManager::hideBackdrop ( KDvoid )
{
    m_pBackdropLayer->hide ( );
}

KDvoid SdkTrayManager::showCursor ( const String& sMaterialName )
{
    if ( sMaterialName != StringUtil::BLANK )
    {
        getCursorImage ( )->setMaterialName ( sMaterialName );
    }
        
    if ( !m_pCursorLayer->isVisible ( ) )
    {
        m_pCursorLayer->show ( );
        refreshCursor ( );
    }
}

KDvoid SdkTrayManager::hideCursor ( KDvoid )
{
    m_pCursorLayer->hide ( );
    
    // give widgets a chance to reset in case they're in the middle of something
    for ( KDuint i = 0; i < 10; i++)
    {
        for ( KDuint j = 0; j < m_aWidgets [ i ].size ( ); j++ )
        {
            m_aWidgets [ i ][ j ]->_focusLost ( );
        }
    }
    
    setExpandedMenu ( 0 );
}

KDvoid SdkTrayManager::refreshCursor ( KDvoid )
{
/*
#if (OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0) || (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS)
    // TODO:
    // the position should be based on the orientation, for now simply return
    return;
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
    std::vector<OIS::MultiTouchState> states = mMouse->getMultiTouchStates();
    if(states.size() > 0)
        mCursor->setPosition(states[0].X.abs, states[0].Y.abs);
#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    // TODO: handle cursor positioning
#else
    mCursor->setPosition(mMouse->getMouseState().X.abs, mMouse->getMouseState().Y.abs);
#endif
*/
}

KDvoid SdkTrayManager::showTrays ( KDvoid )
{
    m_pTraysLayer   ->show ( );
    m_pPriorityLayer->show ( );
}

KDvoid SdkTrayManager::hideTrays ( KDvoid )
{
    m_pTraysLayer   ->hide ( );
    m_pPriorityLayer->hide ( );
    
    // give widgets a chance to reset in case they're in the middle of something
    for ( KDuint i = 0; i < 10; i++ )
    {
        for ( KDuint j = 0; j < m_aWidgets[i].size ( ); j++)
        {
            m_aWidgets[i][j]->_focusLost ( );
        }
    }
    
    setExpandedMenu ( 0 );
}

bool SdkTrayManager::isCursorVisible ( KDvoid )
{
    return m_pCursorLayer->isVisible ( );
}

bool SdkTrayManager::isBackdropVisible ( KDvoid )
{
    return m_pBackdropLayer->isVisible ( );
}

bool SdkTrayManager::areTraysVisible ( KDvoid )
{
    return m_pTraysLayer->isVisible ( );
}

KDvoid SdkTrayManager::adjustTrays ( KDvoid )
{
    for ( KDuint i = 0; i < 9; i++ )        // resizes and hides trays if necessary
    {
        Real  fTrayWidth  = 0;
        Real  fTrayHeight = m_fWidgetPadding;
        std::vector<OverlayElement*>  aLabelsAndSeps;

        if ( m_aWidgets[i].empty ( ) )      // hide tray if empty
        {
            m_pTrays[i]->hide ( );
            continue;
        }
        else
        {
            m_pTrays[i]->show ( );
        }

        // arrange widgets and calculate final tray size and position
        for ( KDuint j = 0; j < m_aWidgets[i].size ( ); j++ )
        {
            OverlayElement*  e = m_aWidgets[i][j]->getOverlayElement ( );
                                      
            if ( j != 0 )
            {
                fTrayHeight += m_fWidgetSpacing;   // don't space first widget
            }
            
            e->setVerticalAlignment ( GVA_TOP );
            e->setTop ( fTrayHeight );
            
            switch ( e->getHorizontalAlignment ( ) )
            {
                case GHA_LEFT  : e->setLeft ( m_fWidgetPadding );                           break;
                case GHA_RIGHT : e->setLeft ( -( e->getWidth ( ) + m_fWidgetPadding ) );    break;                    
                default        : e->setLeft ( -( e->getWidth ( ) / 2 ) );
            }
            
            // prevents some weird texture filtering problems (just some)
            e->setPosition   ( (KDint) e->getLeft  ( ), (KDint) e->getTop    ( ) );
            e->setDimensions ( (KDint) e->getWidth ( ), (KDint) e->getHeight ( ) );
            
            fTrayHeight += e->getHeight ( );
        
            Label*  l = dynamic_cast<Label*> ( m_aWidgets[i][j]);
            if ( l && l->_isFitToTray ( ) )
            {
                aLabelsAndSeps.push_back(e);
                continue;
            }
            Separator*  s = dynamic_cast<Separator*> ( m_aWidgets[i][j]);
            if ( s && s->_isFitToTray ( ) )
            {
                aLabelsAndSeps.push_back(e);
                continue;
            }
                
            if ( e->getWidth ( ) > fTrayWidth )
            {
                fTrayWidth = e->getWidth ( );
            }
                           
        }

        // add paddings and resize trays
        m_pTrays[i]->setWidth  ( fTrayWidth  + 2 * m_fWidgetPadding );
        m_pTrays[i]->setHeight ( fTrayHeight + m_fWidgetPadding );
        
        for ( KDuint j = 0; j < aLabelsAndSeps.size ( ); j++ )
        {
            aLabelsAndSeps[j]->setWidth (  (KDint)  fTrayWidth );
            aLabelsAndSeps[j]->setLeft  ( -(KDint)( fTrayWidth / 2 ) );
        }
    }
    
    for ( KDuint i = 0; i < 9; i++ )    // snap trays to anchors
    {
        if ( i == TL_TOPLEFT    || i == TL_LEFT     || i == TL_BOTTOMLEFT   ) m_pTrays[i]->setLeft ( m_fTrayPadding);
        if ( i == TL_TOP        || i == TL_CENTER   || i == TL_BOTTOM       ) m_pTrays[i]->setLeft ( -m_pTrays[i]->getWidth ( ) / 2);
        if ( i == TL_TOPRIGHT   || i == TL_RIGHT    || i == TL_BOTTOMRIGHT  ) m_pTrays[i]->setLeft ( -( m_pTrays[i]->getWidth ( ) + m_fTrayPadding ) );
        
        if ( i == TL_TOPLEFT    || i == TL_TOP      || i == TL_TOPRIGHT     ) m_pTrays[i]->setTop ( m_fTrayPadding );
        if ( i == TL_LEFT       || i == TL_CENTER   || i == TL_RIGHT        ) m_pTrays[i]->setTop ( -m_pTrays[i]->getHeight ( ) / 2 );
        if ( i == TL_BOTTOMLEFT || i == TL_BOTTOM   || i == TL_BOTTOMRIGHT  ) m_pTrays[i]->setTop ( -m_pTrays[i]->getHeight ( ) - m_fTrayPadding );
        
        // prevents some weird texture filtering problems (just some)
        m_pTrays[i]->setPosition   ( (KDint) m_pTrays[i]->getLeft  ( ), (KDint) m_pTrays[i]->getTop    ( ) );
        m_pTrays[i]->setDimensions ( (KDint) m_pTrays[i]->getWidth ( ), (KDint) m_pTrays[i]->getHeight ( ) );
    }
}

Ray SdkTrayManager::getCursorRay ( Camera* pCamera )
{
    return screenToScene ( pCamera, Vector2 ( m_pCursor->_getLeft ( ), m_pCursor->_getTop ( ) ) );
}

Button* SdkTrayManager::createButton ( TrayLocation tTrayLoc, const String& sName, const String& sCaption, Real fWidth )
{
    Button*  pRet = new Button ( sName, sCaption, fWidth );
    moveWidgetToTray ( pRet, tTrayLoc );
    pRet->_assignListener ( m_pListener );
    return pRet;
}

TextBox* SdkTrayManager::createTextBox ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption, Real fWidth, Real fHeight )
{
    TextBox*  pRet = new TextBox ( sName, sCaption, fWidth, fHeight );
    moveWidgetToTray ( pRet, tTrayLoc );
    pRet->_assignListener ( m_pListener );
    return pRet;
}

SelectMenu* SdkTrayManager::createThickSelectMenu ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                                    Real fWidth, KDuint uMaxItemsShown, const StringVector& aItems )
{
    SelectMenu*  pRet = new SelectMenu ( sName, sCaption, fWidth, 0, uMaxItemsShown );
    moveWidgetToTray ( pRet, tTrayLoc );
    pRet->_assignListener ( m_pListener );
    if ( !aItems.empty ( ) )
    {
        pRet->setItems ( aItems );
    }
    return pRet;
}

SelectMenu* SdkTrayManager::createLongSelectMenu  ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                                    Real fWidth, Real fBoxWidth, KDuint uMaxItemsShown, const StringVector& aItems )
{
    SelectMenu*  pRet = new SelectMenu ( sName, sCaption, fWidth, fBoxWidth, uMaxItemsShown );
    moveWidgetToTray ( pRet, tTrayLoc );
    pRet->_assignListener ( m_pListener );
    if ( !aItems.empty ( ) )
    {
        pRet->setItems ( aItems );
    }
    return pRet;
}

SelectMenu* SdkTrayManager::createLongSelectMenu  ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                                    Real fBoxWidth, KDuint uMaxItemsShown, const StringVector& aItems )
{
    return createLongSelectMenu ( tTrayLoc, sName, sCaption, 0, fBoxWidth, uMaxItemsShown, aItems );
}

Label* SdkTrayManager::createLabel ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption, Real fWidth )
{
    Label*  pRet = new Label ( sName, sCaption, fWidth );
    moveWidgetToTray ( pRet, tTrayLoc );
    pRet->_assignListener ( m_pListener );
    return pRet;
}

Separator* SdkTrayManager::createSeparator ( TrayLocation tTrayLoc, const String& sName, Real fWidth )
{
    Separator*  pRet = new Separator ( sName, fWidth );
    moveWidgetToTray ( pRet, tTrayLoc );
    return pRet;
}

Slider* SdkTrayManager::createThickSlider ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                            Real fWidth, Real fValueBoxWidth, Real fMinValue, Real fMaxValue, KDuint uSnaps )
{
    Slider*  pRet = new Slider ( sName, sCaption, fWidth, 0, fValueBoxWidth, fMinValue, fMaxValue, uSnaps );
    moveWidgetToTray ( pRet, tTrayLoc );
    pRet->_assignListener ( m_pListener );
    return pRet;
}

Slider* SdkTrayManager::createLongSlider ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                           Real fWidth, Real fTrackWidth, Real fValueBoxWidth, Real fMinValue, Real fMaxValue, KDuint uSnaps )
{
    if ( fTrackWidth <= 0 )
    {
        fTrackWidth = 1;
    }
    
    Slider*  pRet = new Slider ( sName, sCaption, fWidth, fTrackWidth, fValueBoxWidth, fMinValue, fMaxValue, uSnaps );
    moveWidgetToTray ( pRet, tTrayLoc );
    pRet->_assignListener ( m_pListener );
    return pRet;
}

Slider* SdkTrayManager::createLongSlider ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                           Real fTrackWidth, Real fValueBoxWidth, Real fMinValue, Real fMaxValue, KDuint uSnaps )
{
    return createLongSlider ( tTrayLoc, sName, sCaption, 0, fTrackWidth, fValueBoxWidth, fMinValue, fMaxValue, uSnaps );
}

ParamsPanel* SdkTrayManager::createParamsPanel ( TrayLocation tTrayLoc, const String& sName, Real fWidth, KDuint uLines )
{
    ParamsPanel*  pRet = new ParamsPanel ( sName, fWidth, uLines );
    moveWidgetToTray ( pRet, tTrayLoc );
    return pRet;
}

ParamsPanel* SdkTrayManager::createParamsPanel ( TrayLocation tTrayLoc, const String& sName, Real fWidth, const StringVector& aParamNames )
{
    ParamsPanel*  pRet = new ParamsPanel ( sName, fWidth, aParamNames.size ( ) );
    pRet->setAllParamNames ( aParamNames );
    moveWidgetToTray ( pRet, tTrayLoc );
    return pRet;
}

CheckBox* SdkTrayManager::createCheckBox ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption, Real fWidth )
{
    CheckBox*  pRet = new CheckBox ( sName, sCaption, fWidth );
    moveWidgetToTray ( pRet, tTrayLoc );
    pRet->_assignListener ( m_pListener );
    return pRet;
}

DecorWidget* SdkTrayManager::createDecorWidget ( TrayLocation tTrayLoc, const String& sName, const String& sTemplateName )
{
    DecorWidget*  pRet = new DecorWidget ( sName, sTemplateName );
    moveWidgetToTray ( pRet, tTrayLoc );
    return pRet;
}

ProgressBar* SdkTrayManager::createProgressBar ( TrayLocation tTrayLoc, const Ogre::String& sName, const Ogre::DisplayString& sCaption, Real fWidth, Real fCommentBoxWidth )
{
    ProgressBar*  pRet = new ProgressBar ( sName, sCaption, fWidth, fCommentBoxWidth );
    moveWidgetToTray ( pRet, tTrayLoc );
    return pRet;
}

KDvoid SdkTrayManager::showFrameStats ( TrayLocation tTrayLoc, KDint nPlace )
{
    if ( !areFrameStatsVisible ( ) )
    {
        StringVector  aStats;
        aStats.push_back("Average FPS");
        aStats.push_back("Best FPS");
        aStats.push_back("Worst FPS");
        aStats.push_back("Triangles");
        aStats.push_back("Batches");
        
        m_pFpsLabel = createLabel ( TL_NONE, m_sName + "/FpsLabel", "FPS:", 180 );
        m_pFpsLabel->_assignListener ( this );
        m_pStatsPanel = createParamsPanel ( TL_NONE, m_sName + "/StatsPanel", 180, aStats );
    }
    
    moveWidgetToTray ( m_pFpsLabel  , tTrayLoc, nPlace );
    moveWidgetToTray ( m_pStatsPanel, tTrayLoc, locateWidgetInTray ( m_pFpsLabel ) + 1 );
}

KDvoid SdkTrayManager::hideFrameStats ( KDvoid )
{
    if ( areFrameStatsVisible ( ) )
    {
        destroyWidget ( m_pFpsLabel );
        destroyWidget ( m_pStatsPanel );
        m_pFpsLabel   = 0;
        m_pStatsPanel = 0;
    }
}

bool SdkTrayManager::areFrameStatsVisible ( KDvoid )
{
    return m_pFpsLabel != 0;
}

KDvoid SdkTrayManager::toggleAdvancedFrameStats ( KDvoid )
{
    if ( m_pFpsLabel )
    {
        labelHit ( m_pFpsLabel );
    }
}

KDvoid SdkTrayManager::showLogo ( TrayLocation tTrayLoc, KDint nPlace )
{
    if ( !isLogoVisible ( ) )
    {
        m_pLogo = createDecorWidget ( TL_NONE, m_sName + "/Logo", "SdkTrays/Logo" );
    }
    
    moveWidgetToTray ( m_pLogo, tTrayLoc, nPlace );
}

KDvoid SdkTrayManager::hideLogo ( KDvoid )
{
    if ( isLogoVisible ( ) )
    {
        destroyWidget ( m_pLogo );
        m_pLogo = 0;
    }
}

bool SdkTrayManager::isLogoVisible ( KDvoid )
{
    return m_pLogo != 0;
}

KDvoid SdkTrayManager::showLoadingBar ( KDuint uNumGroupsInit, KDuint uNumGroupsLoad, Real fInitProportion )
{
    if ( m_pDialog )
    {
        closeDialog ( );
    }
    if ( m_pLoadBar )
    {
        hideLoadingBar ( );
    }
            
    m_pLoadBar = new ProgressBar ( m_sName + "/LoadingBar", "Loading...", 400, 308 );
    OverlayElement*  e = m_pLoadBar->getOverlayElement ( );
    m_pDialogShade->addChild ( e );
    e->setVerticalAlignment ( GVA_CENTER );
    e->setLeft ( - ( e->getWidth  ( ) / 2 ) );
    e->setTop  ( - ( e->getHeight ( ) / 2 ) );
            
    ResourceGroupManager::getSingleton ( ).addResourceGroupListener ( this );
    m_bCursorWasVisible = isCursorVisible ( );
    hideCursor ( );
    m_pDialogShade->show ( );
            
    // calculate the proportion of job required to init/load one group            
    if ( uNumGroupsInit == 0 && uNumGroupsLoad != 0)
    {
        m_fGroupInitProportion = 0;
        m_fGroupLoadProportion = 1;
    }
    else if ( uNumGroupsLoad == 0 && uNumGroupsInit != 0 )
    {
        m_fGroupLoadProportion = 0;
        if ( uNumGroupsInit != 0)
        {
            m_fGroupInitProportion = 1;
        }
    }
    else if ( uNumGroupsInit == 0 && uNumGroupsLoad == 0)
    {
        m_fGroupInitProportion = 0;
        m_fGroupLoadProportion = 0;
    }
    else
    {
        m_fGroupInitProportion = fInitProportion / uNumGroupsInit;
        m_fGroupLoadProportion = ( 1 - fInitProportion ) / uNumGroupsLoad;
    }
}

KDvoid SdkTrayManager::hideLoadingBar ( KDvoid )
{
    if ( m_pLoadBar )
    {
        m_pLoadBar->cleanup ( );
        delete m_pLoadBar;
        m_pLoadBar = 0;
        
        ResourceGroupManager::getSingleton ( ).removeResourceGroupListener ( this );
        if ( m_bCursorWasVisible )
        {
            showCursor ( );
        }
        m_pDialogShade->hide ( );
    }
}

bool SdkTrayManager::isLoadingBarVisible ( KDvoid )
{
    return m_pLoadBar != 0;
}

KDvoid SdkTrayManager::showOkDialog ( const DisplayString& sCaption, const DisplayString& sMessage )
{
    if ( m_pLoadBar )
    {
        hideLoadingBar ( );
    }
        
    OverlayElement* e;
    
    if ( m_pDialog )
    {
        m_pDialog->setCaption ( sCaption );
        m_pDialog->setText    ( sMessage );
        
        if ( m_pOk )
        {
            return;
        }
        else
        {
            m_pYes->cleanup ( );
            m_pNo ->cleanup ( );
            delete m_pYes;
            delete m_pNo;
            m_pYes = 0;
            m_pNo  = 0;
        }
    }
    else
    {
        // give widgets a chance to reset in case they're in the middle of something
        for ( KDuint i = 0; i < 10; i++ )
        {
            for ( KDuint j = 0; j < m_aWidgets[i].size ( ); j++ )
            {
                m_aWidgets[i][j]->_focusLost ( );
            }
        }
        
        m_pDialogShade->show ( );
        
        m_pDialog = new TextBox ( m_sName + "/DialogBox", sCaption, 300, 208 );
        m_pDialog->setText ( sMessage );
        e = m_pDialog->getOverlayElement ( );
        m_pDialogShade->addChild ( e );
        e->setVerticalAlignment ( GVA_CENTER );
        e->setLeft ( - ( e->getWidth  ( ) / 2 ) );
        e->setTop  ( - ( e->getHeight ( ) / 2 ) );
        
        m_bCursorWasVisible = isCursorVisible ( );
        showCursor ( );
    }
    
    m_pOk = new Button ( m_sName + "/OkButton", "OK", 60 );
    m_pOk->_assignListener ( this );
    e = m_pOk->getOverlayElement ( );
    m_pDialogShade->addChild ( e );
    e->setVerticalAlignment ( GVA_CENTER );
    e->setLeft ( -( e->getWidth ( ) / 2 ) );
    e->setTop  ( m_pDialog->getOverlayElement ( )->getTop ( ) + m_pDialog->getOverlayElement ( )->getHeight ( ) + 5 );
}

KDvoid SdkTrayManager::showYesNoDialog ( const DisplayString& sCaption, const DisplayString& sQuestion )
{
    if ( m_pLoadBar )
    {
        hideLoadingBar ( );
    }
        
    OverlayElement*  e;
    
    if ( m_pDialog )
    {
        m_pDialog->setCaption ( sCaption );
        m_pDialog->setText    ( sQuestion );
        
        if ( m_pOk )
        {
            m_pOk->cleanup ( );
            delete m_pOk;
            m_pOk = 0;
        }
        else
        {
            return;
        }
    }
    else
    {
        // give widgets a chance to reset in case they're in the middle of something
        for ( KDuint i = 0; i < 10; i++ )
        {
            for ( KDuint j = 0; j < m_aWidgets[i].size ( ); j++ )
            {
                m_aWidgets[i][j]->_focusLost ( );
            }
        }
        
        m_pDialogShade->show ( );
        
        m_pDialog = new TextBox ( m_sName + "/DialogBox", sCaption, 300, 208 );
        m_pDialog->setText ( sQuestion );
        e = m_pDialog->getOverlayElement ( );
        m_pDialogShade->addChild ( e );
        e->setVerticalAlignment ( GVA_CENTER );
        e->setLeft ( - ( e->getWidth  ( ) / 2 ) );
        e->setTop  ( - ( e->getHeight ( ) / 2 ) );
        
        m_bCursorWasVisible = isCursorVisible ( );
        showCursor ( );
    }
    
    m_pYes = new Button ( m_sName + "/YesButton", "Yes", 58 );
    m_pYes->_assignListener ( this );
    e = m_pYes->getOverlayElement ( );
    m_pDialogShade->addChild ( e );
    e->setVerticalAlignment ( GVA_CENTER );
    e->setLeft ( -( e->getWidth ( ) + 2 ) );
    e->setTop  ( m_pDialog->getOverlayElement ( )->getTop ( ) + m_pDialog->getOverlayElement ( )->getHeight ( ) + 5 );
    
    m_pNo = new Button ( m_sName + "/NoButton", "No", 50 );
    m_pNo->_assignListener ( this );
    e = m_pNo->getOverlayElement ( );
    m_pDialogShade->addChild ( e );
    e->setVerticalAlignment ( GVA_CENTER );
    e->setLeft ( 3 );
    e->setTop ( m_pDialog->getOverlayElement ( )->getTop ( ) + m_pDialog->getOverlayElement ( )->getHeight ( ) + 5 );
}

KDvoid SdkTrayManager::closeDialog ( KDvoid )
{
    if ( m_pDialog )
    {
        if ( m_pOk )
        {
            m_pOk->cleanup ( );
            delete m_pOk;
            m_pOk = 0;
        }
        else
        {
            m_pYes->cleanup ( );
            m_pNo ->cleanup ( );
            delete m_pYes;
            delete m_pNo;
            m_pYes = 0;
            m_pNo  = 0;
        }
        
        m_pDialogShade->hide ( );
        m_pDialog->cleanup ( );
        delete m_pDialog;
        m_pDialog = 0;
        
        if ( !m_bCursorWasVisible )
        {
            hideCursor ( );
        }
    }
}

bool SdkTrayManager::isDialogVisible ( KDvoid )
{
    return m_pDialog != 0;
}

Widget* SdkTrayManager::getWidget ( TrayLocation tTrayLoc, KDuint uPlace )
{
    if ( uPlace < m_aWidgets [ tTrayLoc ].size ( ) )
    {
        return m_aWidgets [ tTrayLoc ][ uPlace ];
    }
    
    return 0;
}

Widget* SdkTrayManager::getWidget ( TrayLocation tTrayLoc, const String& sName )
{
    for ( KDuint i = 0; i < m_aWidgets [ tTrayLoc ].size ( ); i++ )
    {
        if ( m_aWidgets [ tTrayLoc ][ i ]->getName ( ) == sName )
        {
            return m_aWidgets [ tTrayLoc ][ i ];
        }
    }
    
    return 0;
}

Widget* SdkTrayManager::getWidget ( const String& sName )
{
    for ( KDuint i = 0; i < 10; i++ )
    {
        for ( KDuint j = 0; j < m_aWidgets[i].size ( ); j++ )
        {
            if ( m_aWidgets[i][j]->getName ( ) == sName )
            {
                return m_aWidgets[i][j];
            }
        }
    }
    return 0;
}

KDuint SdkTrayManager::getNumWidgets ( KDvoid )
{
    KDuint uTotal = 0;
    
    for ( KDuint i = 0; i < 10; i++ )
    {
        uTotal += m_aWidgets [ i ].size ( );
    }
    
    return uTotal;
}

KDuint SdkTrayManager::getNumWidgets ( TrayLocation tTrayLoc )
{
    return m_aWidgets [ tTrayLoc ].size ( );
}

WidgetIterator SdkTrayManager::getWidgetIterator ( TrayLocation tTrayLoc )
{
    return WidgetIterator ( m_aWidgets [ tTrayLoc ].begin ( ), m_aWidgets [ tTrayLoc ].end ( ) );
}

KDint SdkTrayManager::locateWidgetInTray ( Widget* pWidget )
{
    for ( KDuint i = 0; i < m_aWidgets [ pWidget->getTrayLocation ( ) ].size ( ); i++ )
    {
        if ( m_aWidgets [ pWidget->getTrayLocation ( ) ][ i ] == pWidget )
        {
            return i;
        }
    }
    
    return -1;
}

KDvoid SdkTrayManager::destroyWidget ( Widget* pWidget )
{
    if ( !pWidget )
    {
        OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, "Widget does not exist.", "TrayManager::destroyWidget" );
    }

    // in case special widgets are destroyed manually, set them to 0
    if ( pWidget == m_pLogo )
    {
        m_pLogo = 0;
    }
    else if ( pWidget == m_pStatsPanel )
    {
        m_pStatsPanel = 0;
    }
    else if ( pWidget == m_pFpsLabel )
    {
        m_pFpsLabel = 0;
    }
                    
    m_pTrays [ pWidget->getTrayLocation ( ) ]->removeChild ( pWidget->getName ( ) );
                    
    WidgetList&  aWList = m_aWidgets [ pWidget->getTrayLocation ( ) ];
    aWList.erase ( std::find ( aWList.begin ( ), aWList.end ( ), pWidget ) );
    if ( pWidget == m_pExpandedMenu )
    {
        setExpandedMenu ( 0 );
    }
                        
    pWidget->cleanup ( );
                        
    m_aWidgetDeathRow.push_back ( pWidget );
                        
    adjustTrays ( );
}

KDvoid SdkTrayManager::destroyWidget ( TrayLocation tTrayLoc, KDuint uPlace )
{
    destroyWidget ( getWidget ( tTrayLoc, uPlace ) );
}

KDvoid SdkTrayManager::destroyWidget ( TrayLocation tTrayLoc, const String& sName )
{
    destroyWidget ( getWidget ( tTrayLoc, sName ) );
}

KDvoid SdkTrayManager::destroyWidget ( const String& sName )
{
    destroyWidget ( getWidget ( sName ) );
}

KDvoid SdkTrayManager::destroyAllWidgetsInTray ( TrayLocation tTrayLoc )
{
    while ( !m_aWidgets [ tTrayLoc ].empty ( ) )
    {
        destroyWidget ( m_aWidgets [ tTrayLoc ][ 0 ] );
    }
}

KDvoid SdkTrayManager::destroyAllWidgets ( KDvoid )
{
    for ( KDuint i = 0; i < 10; i++ )  // destroy every widget in every tray (including null tray)
    {
        destroyAllWidgetsInTray ( (TrayLocation) i );
    }
}

KDvoid SdkTrayManager::moveWidgetToTray ( Widget* pWidget, TrayLocation tTrayLoc, KDint nPlace )
{
    if ( !pWidget )
    {
        OGRE_EXCEPT ( Exception::ERR_ITEM_NOT_FOUND, "Widget does not exist.", "TrayManager::moveWidgetToTray" );
    }
        
    // remove widget from old tray
    WidgetList&  tWList = m_aWidgets [ pWidget->getTrayLocation ( ) ];
    WidgetList::iterator  it = std::find ( tWList.begin ( ), tWList.end ( ), pWidget );
    if ( it != tWList.end ( ) )
    {
        tWList.erase ( it );
        m_pTrays [ pWidget->getTrayLocation ( ) ]->removeChild ( pWidget->getName ( ) );
    }
    
    // insert widget into new tray at given position, or at the end if unspecified or invalid
    if ( nPlace == -1 || nPlace > (KDint) m_aWidgets [ tTrayLoc ].size ( ) )
    {
        nPlace = m_aWidgets [ tTrayLoc ].size ( );
    }
    
    m_aWidgets [ tTrayLoc ].insert ( m_aWidgets [ tTrayLoc ].begin ( ) + nPlace, pWidget );
    m_pTrays [ tTrayLoc ]->addChild ( pWidget->getOverlayElement ( ) );
        
    pWidget->getOverlayElement ( )->setHorizontalAlignment ( m_eTrayWidgetAlign [ tTrayLoc ] );
        
    // adjust trays if necessary
    if ( pWidget->getTrayLocation ( ) != TL_NONE || tTrayLoc != TL_NONE )
    {
        adjustTrays ( );
    }
    
    pWidget->_assignToTray ( tTrayLoc );
}

KDvoid SdkTrayManager::moveWidgetToTray ( const String& sName, TrayLocation tTrayLoc, KDuint uPlace )
{
    moveWidgetToTray ( getWidget ( sName ), tTrayLoc, uPlace );
}

KDvoid SdkTrayManager::moveWidgetToTray ( TrayLocation tCurrentTrayLoc, const String& sName, TrayLocation tTargetTrayLoc, KDint nPlace )
{
    moveWidgetToTray ( getWidget ( tCurrentTrayLoc, sName ), tTargetTrayLoc, nPlace );
}

KDvoid SdkTrayManager::moveWidgetToTray ( TrayLocation tCurrentTrayLoc, KDuint uCurrentPlace, TrayLocation tTargetTrayLoc, KDint nTargetPlace )
{
    moveWidgetToTray ( getWidget ( tCurrentTrayLoc, uCurrentPlace ), tTargetTrayLoc, nTargetPlace );
}

KDvoid SdkTrayManager::removeWidgetFromTray ( Widget* pWidget )
{
    moveWidgetToTray ( pWidget, TL_NONE );
}

KDvoid SdkTrayManager::removeWidgetFromTray ( const String& sName )
{
    removeWidgetFromTray ( getWidget ( sName ) );
}

KDvoid SdkTrayManager::removeWidgetFromTray ( TrayLocation tTrayLoc, const String& sName )
{
    removeWidgetFromTray ( getWidget ( tTrayLoc, sName ) );
}

KDvoid SdkTrayManager::removeWidgetFromTray ( TrayLocation tTrayLoc, KDint nPlace )
{
    removeWidgetFromTray ( getWidget ( tTrayLoc, nPlace ) );
}

KDvoid SdkTrayManager::clearTray ( TrayLocation tTrayLoc )
{
    if ( tTrayLoc == TL_NONE )
    {
        return;      // can't clear the null tray
    }
    
    while ( !m_aWidgets [ tTrayLoc ].empty ( ) )   // remove every widget from given tray
    {
        removeWidgetFromTray ( m_aWidgets [ tTrayLoc ][ 0 ] );
    }
}

KDvoid SdkTrayManager::clearAllTrays ( KDvoid )
{
    for ( KDuint i = 0; i < 9; i++ )
    {
        clearTray ( (TrayLocation) i );
    }
}

bool SdkTrayManager::frameRenderingQueued ( const FrameEvent& tEvent )
{
    for ( KDuint i = 0; i < m_aWidgetDeathRow.size ( ); i++ )
    {
        delete m_aWidgetDeathRow[i];
    }
    m_aWidgetDeathRow.clear();
    
    KDuint uCurrentTime = m_pTimer->getMilliseconds ( );
    if ( areFrameStatsVisible ( ) && uCurrentTime - m_uLastStatUpdateTime > 250)
    {
        RenderTarget::FrameStats  tStats = m_pWindow->getStatistics ( );
        
        m_uLastStatUpdateTime = uCurrentTime;
        
        String  sS ( "FPS: " );
        sS += StringConverter::toString ( (KDint) tStats.lastFPS );
        
        for ( KDint i = sS.length ( ) - 5; i > 5; i -= 3 )
        {
            sS.insert ( i, 1, ',' );
        }
        m_pFpsLabel->setCaption ( sS );
        
        if ( m_pStatsPanel->getOverlayElement ( )->isVisible ( ) )
        {
            StringVector        aValues;
            std::ostringstream  oStr;
            
            oStr.str ( "" );
            oStr << std::fixed << std::setprecision ( 1 ) << tStats.avgFPS;
            String  sStr = oStr.str ( );
            for ( KDint i = sStr.length ( ) - 5; i > 0; i -= 3 )
            {
                sStr.insert ( i, 1, ',' );
            }
            aValues.push_back ( sStr );
            
            oStr.str ( "" );
            oStr << std::fixed << std::setprecision(1) << tStats.bestFPS;
            sStr = oStr.str ( );
            for ( KDint i = sStr.length ( ) - 5; i > 0; i -= 3 )
            {
                sStr.insert ( i, 1, ',' );
            }
            aValues.push_back ( sStr );
            
            oStr.str ( "" );
            oStr << std::fixed << std::setprecision(1) << tStats.worstFPS;
            sStr = oStr.str ( );
            for ( KDint i = sStr.length ( ) - 5; i > 0; i -= 3 )
            {
                sStr.insert ( i, 1, ',' );
            }
            aValues.push_back ( sStr );
            
            sStr = StringConverter::toString ( tStats.triangleCount );
            for ( KDint i = sStr.length ( ) - 3; i > 0; i -= 3 )
            {
                sStr.insert ( i, 1, ',' );
            }
            aValues.push_back ( sStr );
            
            sStr = StringConverter::toString ( tStats.batchCount );
            for ( KDint i = sStr.length ( ) - 3; i > 0; i -= 3 )
            {
                sStr.insert ( i, 1, ',' );
            }
            aValues.push_back ( sStr );
            
            m_pStatsPanel->setAllParamValues ( aValues );
        }
    }
    
    return true;
}

KDvoid SdkTrayManager::windowUpdate ( KDvoid )
{
    m_pWindow->update ( );
} 

KDvoid SdkTrayManager::resourceGroupScriptingStarted ( const String& sGroupName, size_t uScriptCount )
{
    m_fLoadInc = m_fGroupInitProportion / uScriptCount;
    m_pLoadBar->setCaption ( "Parsing..." );
    windowUpdate ( );
}

KDvoid SdkTrayManager::scriptParseStarted ( const String& sScriptName, bool& bSkipThisScript )
{
    m_pLoadBar->setComment ( sScriptName );
    windowUpdate ( );
}

KDvoid SdkTrayManager::scriptParseEnded ( const String& sScriptName, bool bSkipped )
{
    m_pLoadBar->setProgress ( m_pLoadBar->getProgress ( ) + m_fLoadInc );
    windowUpdate ( );
}

KDvoid SdkTrayManager::resourceGroupScriptingEnded ( const String& sGroupName )
{
    
}

KDvoid SdkTrayManager::resourceGroupLoadStarted ( const String& sGroupName, size_t uResourceCount )
{
    m_fLoadInc = m_fGroupLoadProportion / uResourceCount;
    m_pLoadBar->setCaption ( "Loading..." );
    windowUpdate ( );
}

KDvoid SdkTrayManager::resourceLoadStarted ( const ResourcePtr& pResource )
{
    m_pLoadBar->setComment ( pResource->getName ( ) );
    windowUpdate ( );
}

KDvoid SdkTrayManager::resourceLoadEnded ( KDvoid )
{
    m_pLoadBar->setProgress ( m_pLoadBar->getProgress ( ) + m_fLoadInc );
    windowUpdate ( );
}

KDvoid SdkTrayManager::worldGeometryStageStarted ( const String& sDescription )
{
    m_pLoadBar->setComment ( sDescription );
    windowUpdate ( );
}

KDvoid SdkTrayManager::worldGeometryStageEnded ( KDvoid )
{
    m_pLoadBar->setProgress ( m_pLoadBar->getProgress ( ) + m_fLoadInc );
    windowUpdate ( );
}

KDvoid SdkTrayManager::resourceGroupLoadEnded ( const String& sGroupName )
{
    
}

KDvoid SdkTrayManager::labelHit ( Label* pLabel )
{
    if ( m_pStatsPanel->getOverlayElement ( )->isVisible ( ) )
    {
        m_pStatsPanel->getOverlayElement ( )->hide ( );
        m_pFpsLabel  ->getOverlayElement ( )->setWidth ( 150 );
        removeWidgetFromTray ( m_pStatsPanel );
    }
    else
    {
        m_pStatsPanel->getOverlayElement ( )->show ( );
        m_pFpsLabel  ->getOverlayElement ( )->setWidth ( 180 );
        moveWidgetToTray ( m_pStatsPanel, m_pFpsLabel->getTrayLocation ( ), locateWidgetInTray ( m_pFpsLabel ) + 1 );
    }
}

/**
 *  Destroys dialog widgets, notifies listener, and ends high priority session.
 */
KDvoid SdkTrayManager::buttonHit ( Button* pButton )
{
    if ( m_pListener )
    {
        if ( pButton == m_pOk )
        {
            m_pListener->okDialogClosed ( m_pDialog->getText ( ) );
        }
        else
        {
            m_pListener->yesNoDialogClosed ( m_pDialog->getText ( ), pButton == m_pYes );
        }
    }
    closeDialog ( );
}

bool SdkTrayManager::injectTouchMoved ( const KDEventTouches& tTouches )
{
    Vector2  tCursorPos ( tTouches.touches [ 0 ].x, m_pWindow->getHeight ( ) - tTouches.touches [ 0 ].y );
    
    m_pCursor->setPosition ( tCursorPos.x, tCursorPos.y );
    
    if ( m_pExpandedMenu)   // only check top priority widget until it passes on
    {
        m_pExpandedMenu->_cursorMoved ( tCursorPos );
        return true;
    }
    
    if ( m_pDialog )   // only check top priority widget until it passes on
    {
        m_pDialog->_cursorMoved ( tCursorPos );
        if ( m_pOk )
        {
            m_pOk->_cursorMoved ( tCursorPos );
        }
        else
        {
            m_pYes->_cursorMoved ( tCursorPos );
            m_pNo ->_cursorMoved ( tCursorPos );
        }
        return true;
    }
    
    for ( KDuint i = 0; i < 10; i++ )
    {
        if ( !m_pTrays[i]->isVisible ( ) )
        {
            continue;
        }
        
        for ( KDuint j = 0; j < m_aWidgets[i].size ( ); j++ )
        {
            Widget*  pWidget = m_aWidgets[i][j];
            if ( !pWidget->getOverlayElement ( )->isVisible ( ) )
            {
                continue;
            }
            pWidget->_cursorMoved ( tCursorPos );    // send event to widget
        }
    }
    
    if ( m_bTrayDrag )
    {
        return true;  // don't pass this event on if we're in the middle of a drag
    }
    
    return false;
}

bool SdkTrayManager::injectTouchPressed ( const KDEventTouches& tTouches )
{
    Vector2  tCursorPos ( tTouches.touches [ 0 ].x, m_pWindow->getHeight ( ) - tTouches.touches [ 0 ].y );
    
    m_pCursor->setPosition ( tCursorPos.x, tCursorPos.y );
    
    m_bTrayDrag = false;
    
    if ( m_pExpandedMenu )   // only check top priority widget until it passes on
    {
        m_pExpandedMenu->_cursorPressed ( tCursorPos );
        if ( !m_pExpandedMenu->isExpanded ( ) )
        {
            setExpandedMenu ( 0 );
        }
        return true;
    }
    
    if ( m_pDialog )   // only check top priority widget until it passes on
    {
        m_pDialog->_cursorPressed ( tCursorPos );
        if ( m_pOk )
        {
            m_pOk->_cursorPressed ( tCursorPos );
        }
        else
        {
            m_pYes->_cursorPressed ( tCursorPos );
            m_pNo ->_cursorPressed ( tCursorPos );
        }
        return true;
    }
    
    for ( KDint i = 0; i < 9; i++)   // check if mouse is over a non-null tray
    {
        if ( m_pTrays[i]->isVisible ( ) && Widget::isCursorOver ( m_pTrays[i], tCursorPos, 2 ) )
        {
            m_bTrayDrag = true;   // initiate a drag that originates in a tray
            break;
        }
    }
    
    for ( KDuint i = 0; i < m_aWidgets[9].size(); i++)  // check if mouse is over a non-null tray's widgets
    {
        if ( m_aWidgets[9][i]->getOverlayElement ( )->isVisible ( ) &&
            Widget::isCursorOver ( m_aWidgets[9][i]->getOverlayElement ( ), tCursorPos ) )
        {
            m_bTrayDrag = true;   // initiate a drag that originates in a tray
            break;
        }
    }
    
    if ( !m_bTrayDrag )
    {
        return false;   // don't process if mouse press is not in tray
    }
    
    for ( KDuint i = 0; i < 10; i++ )
    {
        if ( !m_pTrays[i]->isVisible ( ) )
        {
            continue;
        }
        
        for ( KDuint j = 0; j < m_aWidgets[i].size ( ); j++ )
        {
            Widget*  pWidget = m_aWidgets[i][j];
            if ( !pWidget->getOverlayElement ( )->isVisible ( ) )
            {
                continue;
            }
            pWidget->_cursorPressed ( tCursorPos );    // send event to widget
            
            SelectMenu*  pMenu = dynamic_cast<SelectMenu*> ( pWidget );
            if ( pMenu && pMenu->isExpanded ( ) )       // a menu has begun a top priority session
            {
                setExpandedMenu ( pMenu );
                return true;
            }
        }
    }
    
    return true;   // a tray click is not to be handled by another party
}

bool SdkTrayManager::injectTouchReleased ( const KDEventTouches& tTouches )
{
    Vector2  tCursorPos ( tTouches.touches [ 0 ].x, m_pWindow->getHeight ( ) - tTouches.touches [ 0 ].y );
    
    m_pCursor->setPosition ( tCursorPos.x, tCursorPos.y );
    
    if ( m_pExpandedMenu )   // only check top priority widget until it passes on
    {
        m_pExpandedMenu->_cursorReleased ( tCursorPos );
        return true;
    }
    
    if ( m_pDialog )   // only check top priority widget until it passes on
    {
        m_pDialog->_cursorReleased ( tCursorPos );
        if ( m_pOk )
        {
            m_pOk->_cursorReleased ( tCursorPos );
        }
        else
        {
            m_pYes->_cursorReleased ( tCursorPos );
            // very important to check if second button still exists, because first button could've closed the popup
            if ( m_pNo )
            {
                m_pNo->_cursorReleased ( tCursorPos );
            }
        }
        return true;
    }
    
    if ( !m_bTrayDrag )
    {
        return false;    // this click did not originate in a tray, so don't process
    }
    
    for ( KDuint i = 0; i < 10; i++)
    {
        if ( !m_pTrays [ i ]->isVisible ( ) )
        {
            continue;
        }
        
        for ( KDuint j = 0; j < m_aWidgets[i].size ( ); j++ )
        {
            Widget*  pWidget = m_aWidgets[i][j];
            if ( !pWidget->getOverlayElement ( )->isVisible ( ) )
            {
                continue;
            }
            pWidget->_cursorReleased ( tCursorPos );    // send event to widget
        }
    }
    
    m_bTrayDrag = false;    // stop this drag
    return true;            // this click did originate in this tray, so don't pass it on
}

KDvoid SdkTrayManager::setExpandedMenu ( SelectMenu* pMenu )
{
    if ( !m_pExpandedMenu && pMenu )
    {
        OverlayContainer*   c = (OverlayContainer*) pMenu->getOverlayElement ( );
        OverlayContainer*  eb = (OverlayContainer*) c->getChild ( pMenu->getName ( ) + "/MenuExpandedBox" );
        eb->_update ( );
        
        eb->setPosition
        (
            (KDuint) ( eb->_getDerivedLeft ( ) * OverlayManager::getSingleton ( ).getViewportWidth  ( ) ),
            (KDuint) ( eb->_getDerivedTop  ( ) * OverlayManager::getSingleton ( ).getViewportHeight ( ) )
        );
        c->removeChild ( eb->getName ( ) );
        m_pPriorityLayer->add2D ( eb );
    }
    else if ( m_pExpandedMenu && !pMenu )
    {
        OverlayContainer*  eb = m_pPriorityLayer->getChild ( m_pExpandedMenu->getName ( ) + "/MenuExpandedBox" );
        m_pPriorityLayer->remove2D ( eb );
        ( (OverlayContainer*) m_pExpandedMenu->getOverlayElement ( ) )->addChild ( eb );
    }
    
    m_pExpandedMenu = pMenu;
}