/* ------------------------------------------------------------------------------------
 *
 *      File            SdkTrays.h
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

#ifndef __SdkTrays_h__
#define __SdkTrays_h__

#if OGRE_UNICODE_SUPPORT
#define DISPLAY_STRING_TO_STRING(DS) (DS.asUTF8())
#else
#define DISPLAY_STRING_TO_STRING(DS) (DS)
#endif

enum TrayLocation   // enumerator values for widget tray anchoring locations
{
    TL_TOPLEFT      ,
    TL_TOP          ,
    TL_TOPRIGHT     ,
    TL_LEFT         ,
    TL_CENTER       ,
    TL_RIGHT        ,
    TL_BOTTOMLEFT   ,
    TL_BOTTOM       ,
    TL_BOTTOMRIGHT  ,
    TL_NONE         ,
};

enum ButtonState   // enumerator values for button states
{
    BS_UP           ,
    BS_OVER         ,
    BS_DOWN         ,
};

// forward widget class declarations
class Widget;
class Button;
class SelectMenu;
class Label;
class Slider;
class CheckBox;
class TextBox;

/**
 * Listener class for responding to tray events.
 */
class SdkTrayListener
{
    public :
    
        virtual ~SdkTrayListener ( KDvoid ) { }
    
        virtual KDvoid  buttonHit ( Button* pButton ) { }
    
        virtual KDvoid  itemSelected ( SelectMenu* pMenu ) { }
    
        virtual KDvoid  labelHit ( Label* pLabel ) { }
    
        virtual KDvoid  sliderMoved ( Slider* pSlider ) { }
    
        virtual KDvoid  checkBoxToggled ( CheckBox* pBox ) { }
    
        virtual KDvoid  okDialogClosed ( const DisplayString& message ) { }
    
        virtual KDvoid  yesNoDialogClosed ( const DisplayString& tQuestion, KDbool bYesHit ) { }
};

/**
 * Abstract base class for all widgets.
 */
class Widget
{
    public :
    
        Widget ( KDvoid );
    
        virtual ~Widget ( KDvoid );
    
        KDvoid  cleanup ( KDvoid );
    
        /**
         *  Static utility method to recursively delete an overlay element plus
         *  all of its children from the system.
         */
        static KDvoid  nukeOverlayElement ( OverlayElement* pElement );
    
        /**
         *  Static utility method to check if the cursor is over an overlay element.
         */
        static bool  isCursorOver ( OverlayElement* pElement, const Vector2& tCursorPos, Real fVoidBorder = 0 );
    
        /**
         *  Static utility method used to get the cursor's offset from the center
         *  of an overlay element in pixels.
         */
        static Vector2  cursorOffset ( OverlayElement* pElement, const Vector2& tCursorPos );
    
        /**
         *  Static utility method used to get the width of a caption in a text area.
         */
        static Real  getCaptionWidth ( const DisplayString& tCaption, TextAreaOverlayElement* pArea );

        /**
         *  Static utility method to cut off a string to fit in a text area.
         */
        static KDvoid  fitCaptionToArea ( const DisplayString& tCaption, TextAreaOverlayElement* pArea, Real fMaxWidth );
    
        OverlayElement*  getOverlayElement ( KDvoid );
    
        const String&  getName ( KDvoid );
    
        TrayLocation  getTrayLocation ( KDvoid );
    
        KDvoid  hide ( KDvoid );
        KDvoid  show ( KDvoid );
    
        bool    isVisible ( KDvoid );
    
        // callbacks
        virtual KDvoid  _cursorPressed  ( const Vector2& tCursorPos ); 
        virtual KDvoid  _cursorReleased ( const Vector2& tCursorPos ); 
        virtual KDvoid  _cursorMoved    ( const Vector2& tCursorPos ); 
        virtual KDvoid  _focusLost      ( KDvoid ); 
    
        // internal methods used by SdkTrayManager. do not call directly.    
        KDvoid  _assignToTray ( TrayLocation tTrayLoc );
        KDvoid  _assignListener ( SdkTrayListener* pListener );
    
    protected :
    
        OverlayElement*     m_pElement;
        TrayLocation        m_tTrayLoc;
        SdkTrayListener*    m_pListener;
};

typedef std::vector<Widget*> WidgetList;
typedef Ogre::VectorIterator<WidgetList> WidgetIterator;

/**
 *  Basic button class.
 */

class Button : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        Button ( const String& sName, const DisplayString& tCaption, Real fWidth );
    
        virtual ~Button ( KDvoid );
    
        const DisplayString&  getCaption ( KDvoid );
    
        KDvoid  setCaption ( const DisplayString& tCaption );
    
        const ButtonState&  getState ( KDvoid );
    
        KDvoid  _cursorPressed  ( const Vector2& tCursorPos );
        KDvoid  _cursorReleased ( const Vector2& tCursorPos );
        KDvoid  _cursorMoved    ( const Vector2& tCursorPos );
    
        KDvoid  _focusLost ( KDvoid );
    
    protected :
    
        KDvoid  setState ( const ButtonState& eBS );
    
        ButtonState                     m_eState;
        BorderPanelOverlayElement*      m_pBP;
        TextAreaOverlayElement*         m_pTextArea;
        bool                            m_bFitToContents;
};

/**
 *  Scrollable text box widget.
 */
class TextBox : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        TextBox ( const String& sName, const DisplayString& sCaption, Real fWidth, Real fHeight );
    
        KDvoid  setPadding ( Real fPadding );    
        Real    getPadding ( KDvoid );
    
        const DisplayString&  getCaption ( KDvoid );
    
        KDvoid  setCaption ( const DisplayString& sCaption );
    
        const DisplayString&  getText ( KDvoid );
    
        /**
         *  Sets text box content. Most of this method is for wordwrap.
         */
        KDvoid  setText ( const DisplayString& sText );

        /**
         *  Sets text box content horizontal alignment.
         */
        KDvoid  setTextAlignment ( TextAreaOverlayElement::Alignment eAlign );
    
        KDvoid  clearText ( KDvoid );
    
        KDvoid  appendText ( const DisplayString& sText );
    
        /**
         *  Makes adjustments based on new padding, size, or alignment info.
         */
        KDvoid  refitContents ( KDvoid );
    
        /**
         *  Sets how far scrolled down the text is as a percentage.
         */
        KDvoid  setScrollPercentage ( Real fPercentage );
    
        /**
         *  Sets how far scrolled down the text is as a percentage.
         */
        Real    getScrollPercentage ( KDvoid );
    
        /**
         *  Gets how many lines of text can fit in this window.
         */
        KDuint  getHeightInLines ( KDvoid );

        KDvoid  _cursorPressed  ( const Vector2& tCursorPos );
        KDvoid  _cursorReleased ( const Vector2& tCursorPos );
        KDvoid  _cursorMoved    ( const Vector2& tCursorPos );
    
        KDvoid  _focusLost ( KDvoid );
    
    protected :
    
        /**
         *  Decides which lines to show.
         */
        KDvoid  filterLines ( KDvoid );
    
    protected :
    
        TextAreaOverlayElement*     m_pTextArea;
        BorderPanelOverlayElement*  m_pCaptionBar;
        TextAreaOverlayElement*     m_pCaptionTextArea;
        BorderPanelOverlayElement*  m_pScrollTrack;
        PanelOverlayElement*        m_pScrollHandle;
        DisplayString               m_sText;
        StringVector                m_aLines;
        Real                        m_fPadding;
        bool                        m_bDragging;
        Real                        m_fScrollPercentage;
        Real                        m_fDragOffset;
        KDuint                      m_uStartingLine;
};

/**
 *  Basic selection menu widget.
 */
class SelectMenu : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        SelectMenu ( const String& sName, const DisplayString& sCaption, Real fWidth, Real fBoxWidth, KDuint uMaxItemsShown );
 
        bool    isExpanded ( KDvoid );
    
        const DisplayString&  getCaption ( KDvoid );
    
        KDvoid  setCaption ( const DisplayString& sCaption );
    
        const StringVector&  getItems ( KDvoid );
    
        KDuint  getNumItems ( KDvoid );
    
        KDvoid  setItems   ( const StringVector& aItems );
    
        KDvoid  addItem    ( const DisplayString& sItem );
    
        KDvoid  removeItem ( const DisplayString& sItem );
        
        KDvoid  removeItem ( KDuint uIndex );
    
        KDvoid  clearItems ( KDvoid );
    
        KDvoid  selectItem ( KDuint uIndex, bool bNotifyListener = true );
    
        KDvoid  selectItem ( const DisplayString& sItem, bool bNotifyListener = true );
    
        DisplayString  getSelectedItem ( KDvoid );
    
        KDint   getSelectionIndex ( KDvoid );
    
        KDvoid  _cursorPressed  ( const Vector2& tCursorPos );
        KDvoid  _cursorReleased ( const Vector2& tCursorPos );
        KDvoid  _cursorMoved    ( const Vector2& tCursorPos );
    
        KDvoid  _focusLost ( KDvoid );
    
    protected :
    
        /**
         *  Internal method - sets which item goes at the top of the expanded menu.
         */
        KDvoid  setDisplayIndex ( KDuint uIndex );
    
        /**
         *  Internal method - cleans up an expanded menu.
         */
        KDvoid  retract ( KDvoid );
    
    protected :
    
        BorderPanelOverlayElement*              m_pSmallBox;
        BorderPanelOverlayElement*              m_pExpandedBox;
        TextAreaOverlayElement*                 m_pTextArea;
        TextAreaOverlayElement*                 m_pSmallTextArea;
        BorderPanelOverlayElement*              m_pScrollTrack;
        PanelOverlayElement*                    m_pScrollHandle;
        std::vector<BorderPanelOverlayElement*> m_aItemElements;
        KDuint                                  m_uMaxItemsShown;
        KDuint                                  m_uItemsShown;
        bool                                    m_bCursorOver;
        bool                                    m_bExpanded;
        bool                                    m_bFitToContents;
        bool                                    m_bDragging;
        StringVector                            m_aItems;
        KDint                                   m_nSelectionIndex;
        KDint                                   m_nHighlightIndex;
        KDint                                   m_nDisplayIndex;
        Real                                    m_fDragOffset;
};

/**
 *  Basic label widget.
 */
class Label : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        Label ( const String& sName, const DisplayString& sCaption, Real fWidth );

        const DisplayString&  getCaption ( KDvoid );
    
        KDvoid  setCaption ( const DisplayString& sCaption );
       
        KDvoid  _cursorPressed ( const Vector2& tCursorPos );
    
        bool    _isFitToTray ( KDvoid );
 
    protected :
    
        TextAreaOverlayElement*     m_pTextArea;
        bool                        m_bFitToTray;
};


/**
 *  Basic separator widget.
 */
class Separator : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        Separator ( const String& sName, Real fWidth );
    
        bool  _isFitToTray ( KDvoid );
    
    protected :
    
        bool    m_bFitToTray;
};

/**
 *  Basic slider widget.
 */
class Slider : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        Slider ( const String& sName, const DisplayString& sCaption, Real fWidth, Real fTrackWidth,
                 Real fValueBoxWidth, Real fMinValue, Real fMaxValue, KDuint uSnaps );
    
        /**
         *  Sets the minimum value, maximum value, and the number of snapping points.
         */
        KDvoid  setRange ( Real fMinValue, Real fMaxValue, KDuint uSnaps, bool bNotifyListener = true );
    
        const DisplayString&  getValueCaption ( KDvoid );

        /**
         *  You can use this method to manually format how the value is displayed.
         */
        KDvoid  setValueCaption ( const DisplayString& sCaption );
    
        KDvoid  setValue ( Real fValue, bool bNotifyListener = true );
        Real    getValue ( KDvoid );
    
        const DisplayString&  getCaption ( KDvoid );
    
        KDvoid  setCaption ( const DisplayString& sCaption );
    
        KDvoid  _cursorPressed  ( const Vector2& tCursorPos );
        KDvoid  _cursorReleased ( const Vector2& tCursorPos );
        KDvoid  _cursorMoved    ( const Vector2& tCursorPos );
    
        KDvoid  _focusLost ( KDvoid );
    
    protected : 
    
        /**
         *  Internal method - given a percentage (from left to right), gets the
         *  value of the nearest marker.
         */
        Real  getSnappedValue ( Real fPercentage );
    
    protected :
    
        TextAreaOverlayElement*     m_pTextArea;
        TextAreaOverlayElement*     m_pValueTextArea;
        BorderPanelOverlayElement*  m_pTrack;
        PanelOverlayElement*        m_pHandle;
        bool                        m_bDragging;
        bool                        m_bFitToContents;
        Real                        m_fDragOffset;
        Real                        m_fValue;
        Real                        m_fMinValue;
        Real                        m_fMaxValue;
        Real                        m_fInterval;
};

/**
 *  Basic parameters panel widget.
 */
class ParamsPanel : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        ParamsPanel ( const String& sName, Real fWidth, KDuint uLines );

        KDvoid  setAllParamNames ( const StringVector& aParamNames );
    
        const StringVector& getAllParamNames ( KDvoid );
    
        KDvoid  setAllParamValues ( const StringVector& aParamValues );
    
        KDvoid  setParamValue ( const DisplayString& sParamName, const DisplayString& sParamValue );
    
        KDvoid  setParamValue ( KDuint uIndex, const DisplayString& sParamValue );
    
        DisplayString  getParamValue ( const DisplayString& sParamName );
    
        DisplayString  getParamValue ( KDuint uIndex );
    
        const StringVector&  getAllParamValues ( KDvoid );
    
    protected :
    
        /**
         *  Internal method - updates text areas based on name and value lists.
         */
        KDvoid  updateText ( KDvoid );
    
    protected :
    
        TextAreaOverlayElement*     m_pNamesArea;
        TextAreaOverlayElement*     m_pValuesArea;
        StringVector                m_aNames;
        StringVector                m_aValues;
};

/**
 *  Basic check box widget.
 */
class CheckBox : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        CheckBox ( const String& sName, const DisplayString& sCaption, Real fWidth );
    
        const DisplayString&  getCaption ( KDvoid );
    
        KDvoid  setCaption ( const DisplayString& sCaption );
    
        bool     isChecked ( KDvoid );    
        KDvoid  setChecked ( bool bChecked, bool bNotifyListener = true );
    
        KDvoid  toggle ( bool bNotifyListener = true );
 
    
        KDvoid  _cursorPressed ( const Vector2& tCursorPos );
        KDvoid  _cursorMoved   ( const Vector2& tCursorPos );
    
        KDvoid  _focusLost ( KDvoid );
    
    protected :
    
        TextAreaOverlayElement*     m_pTextArea;
        BorderPanelOverlayElement*  m_pSquare;
        OverlayElement*             m_pX;
        bool                        m_bFitToContents;
        bool                        m_bCursorOver;
};

/**
 *  Custom, decorative widget created from a template.
 */
class DecorWidget : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        DecorWidget ( const String& sName, const String& sTemplateName );
};

/**
 *  Basic progress bar widget.
 */
class ProgressBar : public Widget
{
    public :
    
        // Do not instantiate any widgets directly. Use SdkTrayManager.
        ProgressBar ( const String& sName, const DisplayString& sCaption, Real fWidth, Real fCommentBoxWidth );
    
        /**
         *  Sets the progress as a percentage.
         */
        KDvoid  setProgress ( Real fProgress );
    
        /**
         *  Gets the progress as a percentage.
         */
        Real  getProgress ( KDvoid );
    
        const DisplayString&  getCaption ( KDvoid );
    
        KDvoid  setCaption ( const DisplayString& sCaption );
    
        const DisplayString&  getComment ( KDvoid );
    
        KDvoid  setComment ( const DisplayString& sComment );
    
    protected :
    
        TextAreaOverlayElement*     m_pTextArea;
        TextAreaOverlayElement*     m_pCommentTextArea;
        OverlayElement*             m_pMeter;
        OverlayElement*             m_pFill;
        Real                        m_fProgress;
};

/**
 *  Main class to manage a cursor, backdrop, trays and widgets.
 */
class SdkTrayManager : public SdkTrayListener, public ResourceGroupListener
{
    public :

        SdkTrayManager ( const String& sName, RenderWindow* pWindow, SdkTrayListener* pListener = 0 );
    
        /**
         *  Destroys background, cursor, widgets, and trays.
         */
        virtual  ~SdkTrayManager ( KDvoid );
    
        /**
         *  Converts a 2D screen coordinate (in pixels) to a 3D ray into the scene.
         */
        static Ray  screenToScene ( Camera* pCamera, const Vector2& tPt );
    
        /**
         *  Converts a 3D scene position to a 2D screen position (in relative screen size, 0.0-1.0).
         */
        static Vector2  sceneToScreen ( Camera* pCamera, const Vector3& tPt );
    
        // these methods get the underlying overlays and overlay elements    
        OverlayContainer*  getTrayContainer     ( TrayLocation tTrayLoc );
        Overlay*           getBackdropLayer     ( KDvoid );
        Overlay*           getTraysLayer        ( KDvoid );
        Overlay*           getCursorLayer       ( KDvoid );
        OverlayContainer*  getBackdropContainer ( KDvoid );
        OverlayContainer*  getCursorContainer   ( KDvoid );
        OverlayElement*    getCursorImage       ( KDvoid );
    
        KDvoid  setListener ( SdkTrayListener* pListener );
    
        SdkTrayListener*  getListener ( KDvoid );

        KDvoid  showAll ( KDvoid );
        KDvoid  hideAll ( KDvoid );

        /**
         *  Displays specified material on backdrop, or the last material used if
         *  none specified. Good for pause menus like in the browser.
         */
        KDvoid  showBackdrop ( const String& sMaterialName = StringUtil::BLANK );
        KDvoid  hideBackdrop ( KDvoid );

        /**
         *  Displays specified material on cursor, or the last material used if
         *  none specified. Used to change cursor type.
         */
        KDvoid  showCursor ( const String& sMaterialName = Ogre::StringUtil::BLANK );
        KDvoid  hideCursor ( KDvoid );

        /**
         *  Updates cursor position based on unbuffered mouse state. This is necessary
         *  because if the tray manager has been cut off from mouse events for a time,
         *  the cursor position will be out of date.
         */
        KDvoid  refreshCursor ( KDvoid );

        KDvoid  showTrays ( KDvoid );
        KDvoid  hideTrays ( KDvoid );
    
        bool    isCursorVisible   ( KDvoid );
        bool    isBackdropVisible ( KDvoid );
        bool    areTraysVisible   ( KDvoid );
    
        /**
         *  Fits trays to their contents and snaps them to their anchor locations.
         */     
        virtual KDvoid  adjustTrays ( KDvoid );
    
        /**
         *  Returns a 3D ray into the scene that is directly underneath the cursor.
         */
        Ray  getCursorRay ( Camera* pCamera );

        Button*      createButton          ( TrayLocation tTrayLoc, const String& sName, const String& sCaption, Real fWidth = 0 );

        TextBox*     createTextBox         ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption, Real fWidth, Real fHeight );
 
        SelectMenu*  createThickSelectMenu ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                             Real fWidth, KDuint uMaxItemsShown, const StringVector& aItems = Ogre::StringVector ( ) );
    
        SelectMenu*  createLongSelectMenu  ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                             Real fWidth, Real fBoxWidth, KDuint uMaxItemsShown, const StringVector& aOtems = Ogre::StringVector ( ) );
    
        SelectMenu*  createLongSelectMenu  ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                             Real fBoxWidth, KDuint uMaxItemsShown, const StringVector& aItems = Ogre::StringVector ( ) );
    
        Label*       createLabel           ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption, Real fWidth = 0 );
    
        Separator*   createSeparator       ( TrayLocation tTrayLoc, const String& sName, Real fWidth = 0 );
    
        Slider*      createThickSlider     ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                             Real fWidth, Real fValueBoxWidth, Real fMinValue, Real fMaxValue, KDuint uSnaps );
    
        Slider*      createLongSlider      ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                             Real fWidth, Real fTrackWidth, Real fValueBoxWidth, Real fMinValue, Real fMaxValue, KDuint uSnaps );
    
        Slider*      createLongSlider      ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption,
                                             Real fTrackWidth, Real fValueBoxWidth, Real fMinValue, Real fMaxValue, KDuint uSnaps );
    
        ParamsPanel* createParamsPanel     ( TrayLocation tTrayLoc, const String& sName, Real fWidth, KDuint uLines );
    
        ParamsPanel* createParamsPanel     ( TrayLocation tTrayLoc, const String& sName, Real fWidth, const StringVector& aParamNames );
    
        CheckBox*    createCheckBox        ( TrayLocation tTrayLoc, const String& sName, const DisplayString& sCaption, Real fWidth = 0 );

        DecorWidget* createDecorWidget     ( TrayLocation tTrayLoc, const String& sName, const String& sTemplateName );
    
        ProgressBar* createProgressBar     ( TrayLocation tTrayLoc, const Ogre::String& sName, const Ogre::DisplayString& sCaption,
                                             Real fWidth, Real fCommentBoxWidth );
    
        /**
         *  Shows frame statistics widget set in the specified location.
         */
        KDvoid  showFrameStats ( TrayLocation tTrayLoc, KDint nPlace = -1 );
    
        /**
         *  Hides frame statistics widget set.
         */
        KDvoid  hideFrameStats ( KDvoid );
    
        bool    areFrameStatsVisible ( KDvoid );
    
        /**
         *  Toggles visibility of advanced statistics.
         */
        KDvoid  toggleAdvancedFrameStats ( KDvoid );

        /**
         *  Shows logo in the specified location.
         */
        KDvoid  showLogo ( TrayLocation tTrayLoc, KDint nPlace = -1 );
    
        KDvoid  hideLogo ( KDvoid );
    
        bool    isLogoVisible ( KDvoid );
    
        /**
         *  Shows loading bar. Also takes job settings: the number of resource groups
         *  to be initialised, the number of resource groups to be loaded, and the
         *  proportion of the job that will be taken up by initialisation. Usually,
         *  script parsing takes up most time, so the default value is 0.7.
         */ 
        KDvoid  showLoadingBar ( KDuint uNumGroupsInit = 1, KDuint uNumGroupsLoad = 1, Real fInitProportion = 0.7f );
        KDvoid  hideLoadingBar ( KDvoid );
    
        bool    isLoadingBarVisible ( KDvoid );
    
        /**
         *  Pops up a message dialog with an OK button.
         */ 
        KDvoid  showOkDialog ( const DisplayString& sCaption, const DisplayString& sMessage );
     
        /**
         *  Pops up a question dialog with Yes and No buttons.
         */ 
        KDvoid  showYesNoDialog ( const DisplayString& sCaption, const DisplayString& sQuestion );
    
        /**
         *  Hides whatever dialog is currently showing.
         */
        KDvoid  closeDialog ( KDvoid );
    
        /**
         *  Determines if any dialog is currently visible.
         */
        bool    isDialogVisible ( KDvoid );
    
        /**
         *  Gets a widget from a tray by place.
         */
        Widget*  getWidget ( TrayLocation tTrayLoc, KDuint uPlace );
    
        /**
         *  Gets a widget from a tray by name.
         */
        Widget*  getWidget ( TrayLocation tTrayLoc, const String& sName );
    
        /**
         *  Gets a widget by name.
         */
        Widget*  getWidget ( const String& sName );
    
        /**
         *  Gets the number of widgets in total.
         */
        KDuint   getNumWidgets ( KDvoid );
    
        /**
         *  Gets the number of widgets in a tray.
         */
        KDuint   getNumWidgets ( TrayLocation tTrayLoc );
    
        /**
         *  Gets all the widgets of a specific tray.
         */
        WidgetIterator  getWidgetIterator ( TrayLocation tTrayLoc );
    
        /**
         *  Gets a widget's position in its tray.
         */
        KDint   locateWidgetInTray ( Widget* pWidget );
    
        /**
         *  Destroys a widget.
         */
        KDvoid  destroyWidget ( Widget* pWidget );

        KDvoid  destroyWidget ( TrayLocation tTrayLoc, KDuint uPlace );
    
        KDvoid  destroyWidget ( TrayLocation tTrayLoc, const String& sName );
    
        KDvoid  destroyWidget ( const String& sName );
    
        /**
         *  Destroys all widgets in a tray.
         */    
        KDvoid  destroyAllWidgetsInTray ( TrayLocation tTrayLoc );
    
        /**
         *  Destroys all widgets.
         */
        KDvoid  destroyAllWidgets ( KDvoid );
    
        /**
         *  Adds a widget to a specified tray.
         */
        KDvoid  moveWidgetToTray ( Widget* pWidget, TrayLocation tTrayLoc, KDint nPlace = -1 );
    
        KDvoid  moveWidgetToTray ( const String& sName, TrayLocation tTrayLoc, KDuint uPlace = -1 );
    
        KDvoid  moveWidgetToTray ( TrayLocation tCurrentTrayLoc, const String& sName, TrayLocation tTargetTrayLoc, KDint nPlace = -1 );
    
        KDvoid  moveWidgetToTray ( TrayLocation tCurrentTrayLoc, KDuint uCurrentPlace, TrayLocation tTargetTrayLoc, KDint nTargetPlace = -1 );
    
        /**
         *  Removes a widget from its tray. Same as moving it to the null tray.
         */
        KDvoid  removeWidgetFromTray ( Widget* pWidget );

        KDvoid  removeWidgetFromTray ( const String& sName );

        KDvoid  removeWidgetFromTray ( TrayLocation tTrayLoc, const String& sName );
    
        KDvoid  removeWidgetFromTray ( TrayLocation tTrayLoc, KDint nPlace );
    
        /**
         *  Removes all widgets from a widget tray.
         */
        KDvoid  clearTray ( TrayLocation tTrayLoc );
    
        /**
         *  Removes all widgets from all widget trays.
         */
        KDvoid  clearAllTrays ( KDvoid );
    
        /**
         *  Process frame events. Updates frame statistics widget set and deletes
         *  all widgets queued for destruction.         
         */
        bool    frameRenderingQueued ( const FrameEvent& tEvent );
    
        KDvoid  windowUpdate ( KDvoid );
        
        KDvoid  resourceGroupScriptingStarted ( const String& sGroupName, size_t uScriptCount );
    
        KDvoid  scriptParseStarted ( const String& sScriptName, bool& bSkipThisScript );
    
        KDvoid  scriptParseEnded ( const String& sScriptName, bool bSkipped );
    
        KDvoid  resourceGroupScriptingEnded ( const String& sGroupName );
    
        KDvoid  resourceGroupLoadStarted ( const String& sGroupName, size_t uResourceCount );
    
        KDvoid  resourceLoadStarted ( const ResourcePtr& pResource );
    
        KDvoid  resourceLoadEnded ( KDvoid );
    
        KDvoid  worldGeometryStageStarted ( const String& sDescription );
    
        KDvoid  worldGeometryStageEnded ( KDvoid );
    
        KDvoid  resourceGroupLoadEnded ( const String& sGroupName );
    
        /**
         *  Toggles visibility of advanced statistics.
         */
        KDvoid  labelHit ( Label* pLabel );
    
        /**
         *  Destroys dialog widgets, notifies listener, and ends high priority session.
         */
        KDvoid  buttonHit ( Button* pButton );
    
        /**
         *  Processes touch down events. Returns true if the event was
         *  consumed and should not be passed on to other handlers.
         */
        bool    injectTouchMoved    ( const KDEventTouches& tTouches );
        bool    injectTouchPressed  ( const KDEventTouches& tTouches );
        bool    injectTouchReleased ( const KDEventTouches& tTouches );
    
    protected :
    
        /**
         *  Internal method to prioritise / deprioritise expanded menus.
         */
        KDvoid  setExpandedMenu ( SelectMenu* pMenu );
    
    protected :
    
        String                  m_sName;                    // name of this tray system
        RenderWindow*           m_pWindow;                  // render window
        Overlay*                m_pBackdropLayer;           // backdrop layer
        Overlay*                m_pTraysLayer;              // widget layer
        Overlay*                m_pPriorityLayer;           // top priority layer
        Overlay*                m_pCursorLayer;             // cursor layer
        OverlayContainer*       m_pBackdrop;                // backdrop
        OverlayContainer*       m_pTrays[10];               // widget trays
        WidgetList              m_aWidgets[10];             // widgets
        WidgetList              m_aWidgetDeathRow;          // widget queue for deletion
        OverlayContainer*       m_pCursor;                  // cursor
        SdkTrayListener*        m_pListener;                // tray listener
        Real                    m_fWidgetPadding;           // widget padding
        Real                    m_fWidgetSpacing;           // widget spacing
        Real                    m_fTrayPadding;             // tray padding
        bool                    m_bTrayDrag;                // a mouse press was initiated on a tray
        SelectMenu*             m_pExpandedMenu;            // top priority expanded menu widget
        TextBox*                m_pDialog;                  // top priority dialog widget
        OverlayContainer*       m_pDialogShade;             // top priority dialog shade
        Button*                 m_pOk;                      // top priority OK button
        Button*                 m_pYes;                     // top priority Yes button
        Button*                 m_pNo;                      // top priority No button
        bool                    m_bCursorWasVisible;        // cursor state before showing dialog
        Label*                  m_pFpsLabel;                // FPS label
        ParamsPanel*            m_pStatsPanel;              // frame stats panel
        DecorWidget*            m_pLogo;                    // logo
        ProgressBar*            m_pLoadBar;                 // loading bar
        Real                    m_fGroupInitProportion;     // proportion of load job assigned to initialising one resource group
        Real                    m_fGroupLoadProportion;     // proportion of load job assigned to loading one resource group
        Real                    m_fLoadInc;                 // loading increment
        GuiHorizontalAlignment  m_eTrayWidgetAlign[10];     // tray widget alignments
        Timer*                  m_pTimer;                   // Root::getSingleton().getTimer()
        KDuint                  m_uLastStatUpdateTime;      // The last time the stat text were updated
    
};

#endif // __SdkTrays_h__
