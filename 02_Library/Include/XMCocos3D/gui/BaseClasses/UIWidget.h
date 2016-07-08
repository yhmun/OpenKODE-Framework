/* -----------------------------------------------------------------------------------
 *
 *      File            UIWidget.h
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

#ifndef __UIWidget_h__
#define __UIWidget_h__

#include "../Layouts/UILayoutDefine.h"
#include "../Layouts/UILayoutParameter.h"
#include "../../ccTypes.h"
#include "../../base/CCGeometry.h"
#include "../../2d/actions/CCActionManager.h"
#include "../../2d/layers_scenes_transitions_nodes/CCLayer.h"
#include "../../2d/CCScheduler.h"

namespace gui {

typedef enum
{
    BRIGHT_NONE = -1,
    BRIGHT_NORMAL,
    BRIGHT_HIGHLIGHT
} BrightStyle;

typedef enum
{
    WidgetTypeWidget,		// control
    WidgetTypeContainer		// container
} WidgetType;

typedef enum
{
    UI_TEX_TYPE_LOCAL,
    UI_TEX_TYPE_PLIST
} TextureResType;

typedef enum
{
    TOUCH_EVENT_BEGAN,
    TOUCH_EVENT_MOVED,
    TOUCH_EVENT_ENDED,
    TOUCH_EVENT_CANCELED
} TouchEventType;

typedef enum
{
    SIZE_ABSOLUTE,
    SIZE_PERCENT
} SizeType;

typedef enum
{
    POSITION_ABSOLUTE,
    POSITION_PERCENT
} PositionType;

typedef KDvoid ( cocos2d::Object::*SEL_TouchEvent   ) ( cocos2d::Object*, TouchEventType );
#define toucheventselector(_SELECTOR)				  ( SEL_TouchEvent   ) ( &_SELECTOR )

// Added By XMSoft
typedef KDvoid ( cocos2d::Object::*SEL_PushEvent    ) ( cocos2d::Object* );
typedef KDvoid ( cocos2d::Object::*SEL_MoveEvent    ) ( cocos2d::Object* );
typedef KDvoid ( cocos2d::Object::*SEL_ReleaseEvent ) ( cocos2d::Object* );
typedef KDvoid ( cocos2d::Object::*SEL_CancelEvent  ) ( cocos2d::Object* );

#define coco_pushselector(_SELECTOR)				  ( SEL_PushEvent    ) ( &_SELECTOR )
#define coco_moveselector(_SELECTOR)				  ( SEL_MoveEvent    ) ( &_SELECTOR )
#define coco_releaseselector(_SELECTOR)				  ( SEL_ReleaseEvent ) ( &_SELECTOR )
#define coco_cancelselector(_SELECTOR)                ( SEL_CancelEvent  ) ( &_SELECTOR )

/**
 *   @js NA
 *   @lua NA
 */
class UIWidget : public cocos2d::Object
{
	public :

		/**
		 *	Default constructor
		 */
		UIWidget(void);
    
		/**
		 *	Default destructor
		 */
		virtual ~UIWidget ( KDvoid );
    
		/**
		 *	Allocates and initializes a widget.
		 */
		static UIWidget*				create ( KDvoid );
    
		/**
		 *	Sets whether the widget is enabled
		 *
		 *	Highest control of widget.
		 *	The default value is true, a widget is default to enabled
		 *
		 *	@param enabled   true if the widget is enabled, widget may be touched and visible, false if the widget is disabled, widget cannot be touched and hidden.
		 */
		virtual KDvoid					setEnabled ( KDbool bEnabled );
    
		/**
		 *	Determines if the widget is enabled
		 *
		 *	@return true if the widget is enabled, false if the widget is disabled.
		 */
		KDbool							isEnabled ( KDvoid ) const;
    
		/**
		 *	Sets whether the widget is visible
		 *
		 *	The default value is true, a widget is default to visible
		 *
		 *	@param visible   true if the widget is visible, false if the widget is hidden.
		 */
		KDvoid							setVisible ( KDbool bVisible );
    
		/**
		 *	Determines if the widget is visible
		 *
		 *	@return true if the widget is visible, false if the widget is hidden.
		 */
		KDbool							isVisible ( KDvoid ) const;
    
		/**
		 *	Sets whether the widget is bright
		 *
		 *	The default value is true, a widget is default to bright
		 *
		 *	@param visible   true if the widget is bright, false if the widget is dark.
		 */
		KDvoid							setBright ( KDbool bBright );
    
		/**
		 *	Determines if the widget is bright
		 *
		 *	@return true if the widget is bright, false if the widget is dark.
		 */
		KDbool							isBright ( KDvoid ) const;
    
		/**
		 *	Sets whether the widget is touch enabled
		 *
		 *	The default value is false, a widget is default to touch disabled
		 *
		 *	@param visible   true if the widget is touch enabled, false if the widget is touch disabled.
		 */
		virtual KDvoid					setTouchEnabled ( KDbool bEnabled );
    
		/**
		 *	To set the bright style of widget.
		 *
		 *	@see BrightStyle
		 *
		 *	@param style   BRIGHT_NORMAL the widget is normal state, BRIGHT_HIGHLIGHT the widget is height light state.
		 */
		KDvoid							setBrightStyle ( BrightStyle eStyle );
    
		/**
		 *	Determines if the widget is touch enabled
		 *
		 *	@return true if the widget is touch enabled, false if the widget is touch disabled.
		 */
		KDbool							isTouchEnabled ( KDvoid ) const;
    
		/**
		 *	Determines if the widget is on focused
		 *
		 *	@return true if the widget is on focused, false if the widget is not on focused.
		 */
		KDbool							isFocused ( KDvoid ) const;

		/**
		 *	Sets whether the widget is on focused
		 *
		 *	The default value is false, a widget is default to not on focused
		 *
		 *	@param fucosed   true if the widget is on focused, false if the widget is not on focused.
		 */
		KDvoid							setFocused ( KDbool bFocused );
    
		/**
		 *	Sets the Z order which stands for the drawing order, and reorder this widget in its parent's children array.
		 *
		 *	The Z order of widget is relative to its "brothers": children of the same parent.
		 *	It's nothing to do with OpenGL's z vertex. This one only affects the draw order of widgets in cocos2d.
		 *	The larger number it is, the later this widget will be drawn in each message loop.
		 *	Please refer to setVertexZ(float) for the difference.
		 *
		 *	@param nZOrder   Z order of this widget.
		 */
		KDvoid							setZOrder ( KDint z );
    
		/**
		 *	Gets the Z order of this widget.
		 *
		 *	@see setZOrder(int)
		 *
		 *	@return The Z order.
		 */
		KDint							getZOrder ( KDvoid );
    
		/**
		 *	Gets the left boundary position of this widget.
		 *
		 *	@return The left boundary position of this widget.
		 */
		KDfloat							getLeftInParent ( KDvoid );
    
		/**
		 *	Gets the bottom boundary position of this widget.
		 *
		 *	@return The bottom boundary position of this widget.
		 */
		KDfloat							getBottomInParent ( KDvoid );
    
		/**
		 *	Gets the right boundary position of this widget.
		 *
		 *	@return The right boundary position of this widget.
		 */
		KDfloat							getRightInParent ( KDvoid );
    
		/**
		 *	Gets the top boundary position of this widget.
		 *
		 *	@return The top boundary position of this widget.
		 */
		KDfloat							getTopInParent ( KDvoid );

		/**
		 *	Adds a child to the container.
		 *
		 *	@param child A child widget
		 */
		virtual KDbool					addChild ( UIWidget* pChild );
    
		/**
		 *	Removes a child from the container with a cleanup
		 *
		 *	@param child     The child widget which will be removed.
		 *
		 *	@return the result of removing, succeeded or failed.
		 */
		virtual KDbool					removeChild ( UIWidget* pChild );

		/**
		 *	Removes this widget itself from its parent widget.
		 *	If the widget orphan, then it will destroy itself.
		 */
		virtual KDvoid					removeFromParent ( KDvoid );
    
		/**
		 *	Removes all children from the container, and do a cleanup to all running actions depending on the cleanup parameter.
		 */
		virtual KDvoid					removeAllChildren ( KDvoid );
        
		/**
		 *	Reorders a child according to a new z value.
		 *
		 *	@param child     An already added child node. It MUST be already added.
		 *	@param zOrder    Z order for drawing priority and touched priority. Please refer to setZOrder(int)
		 */
		virtual KDvoid					reorderChild ( UIWidget* pChild );
    
		/**
		 *	Gets a child from the container with its name
		 *
		 *	@param name   An key to find the child widget.
		 *
		 *	@return a UIWidget object whose name equals to the input parameter
		 */
		UIWidget*						getChildByName ( const KDchar* pName );
    
		/**
		 *	Gets a child from the container with its tag
		 *
		 *	@param tag   An identifier to find the child widget.
		 *
		 *	@return a UIWidget object whose tag equals to the input parameter
		 */
		UIWidget*						getChildByTag ( KDint nTag );
    
		/**
		 *	Return an array of children
		 *
		 *	Composing a "tree" structure is a very important feature of UIWidget
		 *
		 *	@return An array of children
		 */
		virtual cocos2d::Array*			getChildren ( KDvoid );
    
		/**
		 *	Gets the renderer of widget
		 *
		 *	renderer is a Node, it's for drawing
		 *
		 *	@return a Node object
		 */
		cocos2d::Node*					getRenderer ( KDvoid );
    
		/**
		 *	Add a Node for rendering.
		 *
		 *	renderer is a Node, it's for drawing
		 *
		 *	@param renderer     A render node
		 *
		 *	@param zOrder    Z order for drawing priority. Please refer to Node::setZOrder(int)
		 */
		KDvoid							addRenderer ( cocos2d::Node* pRenderer, KDint zOrder );
    
		/**
		 *	Remove a Node from widget.
		 *
		 *	renderer is a Node, it's for drawing
		 *
		 *	@param renderer  A render node which needs to be removed
		 *
		 *	@param cleanup   true if all running actions and callbacks on the render node will be cleanup, false otherwise.
		 */
		KDvoid							removeRenderer ( cocos2d::Node* pRenderer, KDbool bCleanup );
    
		/**
		 *	Sets the parent widget
		 *
		 *	@param parent    A pointer to the parnet widget
		 */
		KDvoid							setParent ( UIWidget* pParent );
    
		/**
		 *	Returns a pointer to the parent widget
		 *
		 *	@see setParent(UIWidget*)
		 *
		 *	@returns A pointer to the parnet widget
		 */
		UIWidget*						getParent ( KDvoid );
    
		/**
		 *	Sets the touch event target/selector of the menu item
		 */
		KDvoid							addTouchEventListener ( cocos2d::Object* pTarget, SEL_TouchEvent pSelector );
        
		// Added By XMSoft
		KDvoid							addPushDownEvent	( cocos2d::Object* pTarget, SEL_PushEvent    pSelector );
        KDvoid							addMoveEvent		( cocos2d::Object* pTarget, SEL_MoveEvent    pSelector );
        KDvoid							addReleaseEvent		( cocos2d::Object* pTarget, SEL_ReleaseEvent pSelector );
        KDvoid							addCancelEvent		( cocos2d::Object* pTarget, SEL_CancelEvent  pSelector );

		// cocos2d property
    
		/**
		 *	Changes the position (x,y) of the widget in OpenGL coordinates
		 *
		 *	Usually we use p(x,y) to compose Point object.
		 *	The original point (0,0) is at the left-bottom corner of screen.
		 *
		 *	@param position  The position (x,y) of the widget in OpenGL coordinates
		 */
		KDvoid							setPosition ( const cocos2d::Point& tPos );
    
		/**
		 *	Changes the position (x,y) of the widget in OpenGL coordinates
		 *
		 *	Usually we use p(x,y) to compose Point object.
		 *	The original point (0,0) is at the left-bottom corner of screen.
		 *
		 *	@param percent  The percent (x,y) of the widget in OpenGL coordinates
		 */
		KDvoid							setPositionPercent ( const cocos2d::Point& tPercent );
    
		/**
		 *	Gets the position (x,y) of the widget in OpenGL coordinates
		 *
		 *	@see setPosition(const Point&)
		 *
		 *	@return The position (x,y) of the widget in OpenGL coordinates
		 */
		const cocos2d::Point&			getPosition ( KDvoid );
    
		/**
		 *	Gets the percent (x,y) of the widget in OpenGL coordinates
		 *
		 *	@see setPosition(const Point&)
		 *
		 *	@return The percent (x,y) of the widget in OpenGL coordinates
		 */
		const cocos2d::Point&			getPositionPercent ( KDvoid );
    
		/**
		 *	Changes the position type of the widget
		 *
		 *	@see PositionType
		 *
		 *	@param type  the position type of widget
		 */
		KDvoid							setPositionType ( PositionType eType );

		/**
		 *	Gets the position type of the widget
		 *
		 *	@see PositionType
		 *
		 *	@return type  the position type of widget
		 */
		PositionType					getPositionType ( KDvoid ) const;
    
		/**
		 *	Sets the anchor point in percent.
		 *
		 *	anchorPoint is the point around which all transformations and positioning manipulations take place.
		 *	It's like a pin in the widget where it is "attached" to its parent.
		 *	The anchorPoint is normalized, like a percentage. (0,0) means the bottom-left corner and (1,1) means the top-right corner.
		 *	But you can use values higher than (1,1) and lower than (0,0) too.
		 *	The default anchorPoint is (0.5,0.5), so it starts in the center of the widget.
		 *
		 *	@param anchorPoint   The anchor point of widget.
		 */
		virtual KDvoid					setAnchorPoint ( const cocos2d::Point& pt );
    
		/**
		 *	Returns the anchor point in percent.
		 *
		 *	@see setAnchorPoint(const Point&)
		 *
		 *	@return The anchor point of widget.
		 */
		const cocos2d::Point&			getAnchorPoint ( KDvoid );
    
		/**
		 *	Changes both X and Y scale factor of the widget.
		 *
		 *	1.0 is the default scale factor. It modifies the X and Y scale at the same time.
		 *
		 *	@param scale     The scale factor for both X and Y axis.
		 */
		virtual KDvoid					setScale ( KDfloat fScale );
    
		/**
		 *	Gets the scale factor of the widget,  when X and Y have the same scale factor.
		 *
		 *	@warning Assert when m_fScaleX != m_fScaleY.
		 *	@see setScale(float)
		 *
		 *	@return The scale factor of the widget.
		 */
		KDfloat							getScale ( KDvoid );
    
		/**
		 *	Changes the scale factor on X axis of this widget
		 *
		 *	The deafult value is 1.0 if you haven't changed it before
		 *
		 *	@param fScaleX   The scale factor on X axis.
		 */
		virtual KDvoid					setScaleX ( KDfloat fScaleX );
    
		/**
		 *	Returns the scale factor on X axis of this widget
		 *
		 *	@see setScaleX(float)
		 *
		 *	@return The scale factor on X axis.
		 */
		KDfloat							getScaleX ( KDvoid );
    
		/**
		 *	Changes the scale factor on Y axis of this widget
		 *
		 *	The Default value is 1.0 if you haven't changed it before.
		 *
		 *	@param fScaleY   The scale factor on Y axis.
		 */
		virtual KDvoid					setScaleY ( KDfloat fScaleY );
    
		/**
		 *	Returns the scale factor on Y axis of this widget
		 *
		 *	@see setScaleY(float)
		 *
		 *	@return The scale factor on Y axis.
		 */
		KDfloat							getScaleY ( KDvoid );
    
		/**
		 *	Sets the rotation (angle) of the widget in degrees.
		 *
		 *	0 is the default rotation angle.
		 *	Positive values rotate widget clockwise, and negative values for anti-clockwise.
		 *
		 *	@param fRotation     The roration of the widget in degrees.
		 */
		KDvoid							setRotation ( KDfloat fRotation );
    
		/**
		 *	Returns the rotation of the widget in degrees.
		 *
		 *	@see setRotation(float)
		 *
		 *	@return The rotation of the widget in degrees.
		 */
		KDfloat							getRotation ( KDvoid );
    
		/**
		 *	Sets the X rotation (angle) of the widget in degrees which performs a horizontal rotational skew.
		 *
		 *	0 is the default rotation angle.
		 *	Positive values rotate widget clockwise, and negative values for anti-clockwise.
		 *
		 *	@param fRotationX    The X rotation in degrees which performs a horizontal rotational skew.
		 */
		KDvoid							setRotationX ( KDfloat fRotationX );
    
		/**
		 *	Gets the X rotation (angle) of the widget in degrees which performs a horizontal rotation skew.
		 *
		 *	@see setRotationX(float)
		 *
		 *	@return The X rotation in degrees.
		 */
		KDfloat							getRotationX ( KDvoid );
    
		/**
		 *	Sets the Y rotation (angle) of the widget in degrees which performs a vertical rotational skew.
		 *
		 *	0 is the default rotation angle.
		 *	Positive values rotate widget clockwise, and negative values for anti-clockwise.
		 *
		 *	@param fRotationY    The Y rotation in degrees.
		 */
		KDvoid							setRotationY ( KDfloat fRotationY );
    
		/**
		 *	Gets the Y rotation (angle) of the widget in degrees which performs a vertical rotational skew.
		 *
		 *	@see setRotationY(float)
		 *
		 *	@return The Y rotation in degrees.
		 */
		KDfloat							getRotationY ( KDvoid );
    
		/**
		 *	Sets whether the widget should be flipped horizontally or not.
		 *
		 *	@param bFlipX true if the widget should be flipped horizaontally, false otherwise.
		 */
		virtual KDvoid					setFlipX ( KDbool bFlipX ) { }
    
		/**
		 *	Returns the flag which indicates whether the widget is flipped horizontally or not.
		 *
		 *	It only flips the texture of the widget, and not the texture of the widget's children.
		 *	Also, flipping the texture doesn't alter the anchorPoint.
		 *	If you want to flip the anchorPoint too, and/or to flip the children too use:
		 *	widget->setScaleX(sprite->getScaleX() * -1);
		 *
		 *	@return true if the widget is flipped horizaontally, false otherwise.
		 */
		virtual KDbool					isFlipX ( KDvoid ) { return false; }
    
		/**
		 *	Sets whether the widget should be flipped vertically or not.
		 *
		 *	@param bFlipY true if the widget should be flipped vertically, flase otherwise.
		 */
		virtual KDvoid					setFlipY ( KDbool bFlipY ) { }
    
		/**
		 *	Return the flag which indicates whether the widget is flipped vertically or not.
		 *
		 *	It only flips the texture of the widget, and not the texture of the widget's children.
		 *	Also, flipping the texture doesn't alter the anchorPoint.
		 *	If you want to flip the anchorPoint too, and/or to flip the children too use:
		 *	widget->setScaleY(widget->getScaleY() * -1);
		 *
		 *	@return true if the widget is flipped vertically, flase otherwise.
		 */
		virtual KDbool					isFlipY ( KDvoid ) { return false; }
    
		/**
		 *	Sets color to widget
		 *
		 *	It default change the color of widget's children.
		 *
		 *	@param color
		 */
		virtual KDvoid					setColor ( const cocos2d::Color3B& tColor );
    
		/**
		 *	Gets color of widget
		 *
		 *	@return color
		 */
		virtual const cocos2d::Color3B&	getColor ( KDvoid );
    
		/**
		 *	Sets opacity to widget
		 *
		 *	It default change the opacity of widget's children.
		 *
		 *	@param color
		 */
		virtual KDvoid					setOpacity ( KDint nOpacity );
    
		/**
		 *	Gets opacity of widget
		 *
		 *	@return opacity
		 */
		virtual KDint					getOpacity ( KDvoid );
		virtual KDbool					 isCascadeOpacityEnabled ( KDvoid );
		virtual KDvoid					setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled );
		virtual KDbool					 isCascadeColorEnabled ( KDvoid );
		virtual KDvoid					setCascadeColorEnabled ( KDbool bCascadeColorEnabled );
		KDvoid							setBlendFunc ( cocos2d::BlendFunc tBlendFunc );
    
		// cocos action

		virtual KDvoid					setActionManager ( cocos2d::ActionManager* pActionManager );
		virtual cocos2d::ActionManager* getActionManager ( KDvoid );
		cocos2d::Action*				runAction ( cocos2d::Action* pAction );
		KDvoid							stopAllActions ( KDvoid );
		KDvoid							stopAction ( cocos2d::Action* pAction );
		KDvoid							stopActionByTag ( KDint nTag );
		cocos2d::Action*				getActionByTag ( KDint nTag );
    
		/**
		 *	A call back function when widget lost of focus.
		 */
		KDvoid							didNotSelectSelf ( KDvoid );
    
		/**
		 *	Checks a point if in parent's area.
		 *
		 *	@param point
		 *
		 *	@return true if the point is in parent's area, flase otherwise.
		 */
		KDbool							clippingParentAreaContainPoint ( const cocos2d::Point& pt );
    
		/**
		 *	Sends the touch event to widget's parent
		 */
		virtual KDvoid					checkChildInfo ( KDint nHandleState, UIWidget* pSender, const cocos2d::Point& tTouchPoint );
    
		/**
		 *	Gets the touch began point of widget when widget is selected.
		 *
		 *	@return the touch began point.
		 */
		const cocos2d::Point&			getTouchStartPos ( KDvoid );
    
		/**
		 *	Gets the touch move point of widget when widget is selected.
		 *
		 *	@return the touch move point.
		 */
		const cocos2d::Point&			getTouchMovePos ( KDvoid );
    
		/**
		 *	Gets the touch end point of widget when widget is selected.
		 *
		 *	@return the touch end point.
		 */
		const cocos2d::Point&			getTouchEndPos ( KDvoid );
    
		/**
		 *	Changes the tag that is used to identify the widget easily.
		 *
		 *	@param A interger that indentifies the widget.
		 */
		KDvoid							setTag ( KDint nTag );
    
		/**
		 *	Returns a tag that is used to identify the widget easily.
		 *
		 *	You can set tags to widget then identify them easily.
		 *
		 *	@return A interger that identifies the widget.
		 */
		KDint							getTag ( KDvoid ) const;
    
		/**
		 *	Changes the name that is used to identify the widget easily.
		 *
		 *	@param A const char* that indentifies the widget.
		 */
		KDvoid							setName ( const KDchar* pName );
    
		/**
		 *	Returns a name that is used to identify the widget easily.
		 *
		 *	You can set tags to widget then identify them easily.
		 *
		 *	@return A const char* that identifies the widget.
		 */
		const KDchar*					getName ( KDvoid ) const;
    
		/**
		 *	Returns a type that is widget's type
		 *
		 *	@see WidgetType
		 *
		 *	@return A WidgetType
		 */
		WidgetType						getWidgetType ( KDvoid ) const;
    
		/**
		 *	Changes the size that is widget's size
		 *
		 *	@param size that is widget's size
		 */
		virtual KDvoid					setSize ( const cocos2d::Size& tSize );
    
		/**
		 *	Changes the percent that is widget's percent size
		 *
		 *	@param percent that is widget's percent size
		 */
		virtual KDvoid					setSizePercent ( const cocos2d::Point& tPercent );
    
		/**
		 *	Changes the size type of widget.
		 *
		 *	@see SizeType
		 *
		 *	@param type that is widget's size type
		 */
		KDvoid							setSizeType ( SizeType eType );

		/**
		 *	Gets the size type of widget.
		 *
		 *	@see SizeType
		 *
		 *	@param type that is widget's size type
		 */
		SizeType						getSizeType ( KDvoid ) const;
    
		/**
		 *	Returns size of widget
		 *
		 *	@return size
		 */
		const cocos2d::Size&			getSize ( KDvoid ) const;
    
		/**
		 *	Returns size percent of widget
		 *
		 *	@return size percent
		 */
		const cocos2d::Point&			getSizePercent ( KDvoid ) const;
    
		/**
		 *	Checks a point if is in widget's space
		 *
		 *	@param point
		 *
		 *	@return true if the point is in widget's space, flase otherwise.
		 */
		virtual KDbool					hitTest ( const cocos2d::Point& pt );
    
		/**
		 *	A call back function called when widget is selected, and on touch began.
		 *
		 *	@param touch point
		 *
		 *	@return true if the event should be pass to parent, flase otherwise.
		 */
		virtual KDbool					onTouchBegan ( const cocos2d::Point& tTouchPoint );
    
		/**
		 *	A call back function called when widget is selected, and on touch moved.
		 *
		 *	@param touch point
		 */
		virtual KDvoid					onTouchMoved ( const cocos2d::Point& tTouchPoint );
    
		/**
		 *	A call back function called when widget is selected, and on touch ended.
		 *
		 *	@param touch point
		 */
		virtual KDvoid					onTouchEnded ( const cocos2d::Point& tTouchPoint );
    
		/**
		 *	A call back function called when widget is selected, and on touch canceled.
		 *
		 *	@param touch point
		 */
		virtual KDvoid					onTouchCancelled ( const cocos2d::Point& tTouchPoint );
    
		/**
		 *	A call back function called when widget is selected, and on touch long clicked.
		 *
		 *	@param touch point
		 */
		virtual KDvoid					onTouchLongClicked( const cocos2d::Point& tTouchPoint );
    
		/**
		 *	Sets a LayoutParameter to widget. 
		 *
		 *	@see LayoutParameter
		 *
		 *	@param LayoutParameter pointer
		 *
		 *	@param type  Relative or Linear
		 */
		KDvoid							setLayoutParameter ( UILayoutParameter* pParameter );
    
		/**
		 *	Gets LayoutParameter of widget.
		 *
		 *	@see LayoutParameter
		 *
		 *	@param type  Relative or Linear
		 *
		 *	@return LayoutParameter
		 */
		UILayoutParameter*				getLayoutParameter ( LayoutParameterType eType );
    
		/**
		 *	Ignore the widget size
		 *
		 *	@param ignore, true that widget will ignore it's size, use texture size, false otherwise. Default value is true.
		 */
		virtual KDvoid					ignoreContentAdaptWithSize ( KDbool bIgnore );
    
		/**
		 *	Gets the widget if is ignore it's size.
		 *
		 *	@param ignore, true that widget will ignore it's size, use texture size, false otherwise. Default value is true.
		 */
		KDbool							isIgnoreContentAdaptWithSize ( KDvoid ) const;
    
		/**
		 *	Gets world position of widget.
		 *
		 *	@return world position of widget.
		 */
		cocos2d::Point					getWorldPosition ( KDvoid );
    
		/**
		 *	Converts a Point to world space coordinates. The result is in Points.
		 */
		cocos2d::Point					convertToWorldSpace ( const cocos2d::Point& pt );
    
		/**
		 *	Gets the Virtual Renderer of widget.
		 *
		 *	For example, a button's Virtual Renderer is it's texture renderer.
		 *
		 *	@return Node pointer.
		 */
		virtual cocos2d::Node*			getVirtualRenderer ( KDvoid );
    
		/**
		 *	Schedules the "update" method.
		 */
		KDvoid							setUpdateEnabled ( KDbool bEnable );
    
		/**
		 *	is the "update" method scheduled.
		 */
		KDbool							isUpdateEnabled ( KDvoid );
    
		/**
		 *	Gets the content size of widget.
		 *
		 *	Content size is widget's texture size.
		 */
		virtual const cocos2d::Size&	getContentSize ( KDvoid ) const;
    
		/**
		 *	Returns the "class name" of widget.
		 */
		virtual const KDchar*			getDescription ( KDvoid ) const;
    
		UIWidget*						clone ( KDvoid );

		virtual KDvoid					onEnter ( KDvoid );
		virtual KDvoid					onExit ( KDvoid );
    
		KDvoid							updateSizeAndPosition ( KDvoid );
    
		virtual Object*					getUserObject ( KDvoid ) { return m_pUserObject; }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual const Object*			getUserObject ( KDvoid ) const { return m_pUserObject; }
    
		/**
		 *	Returns a user assigned Object
		 *
		 *	Similar to UserData, but instead of holding a void* it holds an object.
		 *	The UserObject will be retained once in this method,
		 *	and the previous UserObject (if existed) will be relese.
		 *	The UserObject will be released in Node's destructure.
		 *
		 *	@param userObject    A user assigned Object
		 */
		virtual KDvoid					setUserObject ( Object* pUserObject );

		/* temp action */
		KDvoid							setActionTag ( KDint nTag );
		KDint							getActionTag ( KDvoid );

		// Added By XMSoft 
		KDvoid							setActive ( KDbool bActive );
		KDbool							 isActive ( KDvoid );

	protected :

		// call back function called when size changed.
		virtual KDvoid					onSizeChanged ( KDvoid );
    
		// initializes state of widget.
		virtual KDbool					init ( KDvoid );
    
		// initializes renderer of widget.
		virtual KDvoid					initRenderer ( KDvoid );
    
		// call back function called widget's state changed to normal.
		virtual KDvoid					onPressStateChangedToNormal ( KDvoid );
    
		// call back function called widget's state changed to selected.
		virtual KDvoid					onPressStateChangedToPressed ( KDvoid );
    
		// call back function called widget's state changed to dark.
		virtual KDvoid					onPressStateChangedToDisabled ( KDvoid );
		KDvoid							pushDownEvent ( KDvoid );
		KDvoid							moveEvent ( KDvoid );
		KDvoid							releaseUpEvent ( KDvoid );
		KDvoid							cancelUpEvent ( KDvoid );
		KDvoid							longClickEvent ( KDvoid );
		KDvoid							updateAnchorPoint ( KDvoid );
		KDvoid							copyProperties ( UIWidget* pModel );
		virtual UIWidget*				createCloneInstance ( KDvoid );
		virtual KDvoid					copySpecialProperties ( UIWidget* pModel );
		virtual KDvoid					copyClonedWidgetChildren ( UIWidget* pModel );

	protected :

		KDbool							m_bEnabled;				///< Highest control of widget
		KDbool							m_bVisible;				///< is this widget visible
		KDbool							m_bBright;				///< is this widget bright
		KDbool							m_bTouchEnabled;		///< is this widget touch endabled
		KDbool							m_bTouchPassedEnabled;	///< is the touch event should be passed
		KDbool							m_bFocus;				///< is the widget on focus
		KDint							m_nWidgetZOrder;		///< z-order value that affects the draw order and touch order
		cocos2d::Point					m_tAnchorPoint;			///< anchor point normalized
		UIWidget*						m_pWidgetParent;		///< parent of widget
		BrightStyle						m_eBrightStyle;			///< bright style
		KDbool							m_bUpdateEnabled;		///< is "update" method scheduled
		cocos2d::Node*					m_pRenderer;			///< base renderer
		cocos2d::Point					m_tTouchStartPos;		///< touch began point
		cocos2d::Point					m_tTouchMovePos;		///< touch moved point
		cocos2d::Point					m_tTouchEndPos;			///< touch ended point
    
		Object*							m_pTouchEventListener;
		SEL_TouchEvent					m_pTouchEventSelector;
        
		// Added By XMSoft
		cocos2d::Object*				m_pPushListener;
        SEL_PushEvent					m_pPushSelector;
        cocos2d::Object*				m_pMoveListener;
        SEL_MoveEvent					m_pMoveSelector;
        cocos2d::Object*				m_pReleaseListener;
        SEL_ReleaseEvent				m_pReleaseSelector;
        cocos2d::Object*				m_pCancelListener;
        SEL_ReleaseEvent				m_pCancelSelector;

		KDint							m_nWidgetTag;
		std::string						m_sName;
		WidgetType						m_eWidgetType;
		KDint							m_nActionTag;
		cocos2d::Size					m_tSize;
		cocos2d::Size					m_tCustomSize;
		cocos2d::Dictionary*			m_pLayoutParameterDictionary;
		KDbool							m_bIgnoreSize;
		cocos2d::Array*					m_pChildren;
		KDbool							m_bAffectByClipping;
    
		cocos2d::Scheduler*				m_pScheduler;
    
		SizeType						m_eSizeType;
		cocos2d::Point					m_tSizePercent;
		PositionType					m_ePositionType;
		cocos2d::Point					m_tPositionPercent;
		KDbool							m_bIsRunning;
		cocos2d::Object*				m_pUserObject;
};

/**
 *   @js NA
 *   @lua NA
 */
class GUIRenderer : public cocos2d::NodeRGBA
{
	public :

		GUIRenderer ( KDvoid );
		virtual ~GUIRenderer ( KDvoid );

		virtual KDvoid					visit ( KDvoid );

		static GUIRenderer*				create ( KDvoid );

		KDvoid							setEnabled ( KDbool bEnabled );
		KDbool							 isEnabled ( KDvoid ) const;
	
	protected :

		KDbool							m_bEnabled;
};

}
#endif	// __UIWidget_h__
