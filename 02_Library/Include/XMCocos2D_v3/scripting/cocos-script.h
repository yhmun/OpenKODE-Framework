/* -----------------------------------------------------------------------------------
 *
 *      File            cocos-script.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __cocos_script_h__
#define __cocos_script_h__

#include "javascript/bindings/ScriptingCore.h"
#include "auto-generated/js-bindings/jsb_cocos2dx_auto.hpp"
#include "auto-generated/js-bindings/jsb_cocos2dx_extension_auto.hpp"
#include "auto-generated/js-bindings/jsb_cocos2dx_builder_auto.hpp"
#include "auto-generated/js-bindings/jsb_cocos2dx_studio_auto.hpp"
#include "auto-generated/js-bindings/jsb_cocos2dx_gui_auto.hpp"
#include "javascript/bindings/extension/jsb_cocos2dx_extension_manual.h"
#include "javascript/bindings/cocostudio/jsb_cocos2dx_studio_manual.h"
#include "javascript/bindings/gui/jsb_cocos2dx_gui_manual.h"
#include "javascript/bindings/cocos2d_specifics.hpp"
#include "javascript/bindings/cocosbuilder/cocosbuilder_specifics.hpp"
#include "javascript/bindings/chipmunk/js_bindings_chipmunk_registration.h"
#include "javascript/bindings/localstorage/js_bindings_system_registration.h"
#include "javascript/bindings/jsb_opengl_registration.h"
#include "javascript/bindings/network/XMLHTTPRequest.h"
#include "javascript/bindings/network/jsb_websocket.h"
#include "javascript/bindings/cocosbuilder/js_bindings_ccbreader.h"

#include "lua/bindings/CCLuaEngine.h"

#ifdef __cplusplus
extern "C" {
#endif
#include  "lua/bindings/tolua_fix.h"
#ifdef __cplusplus
}
#endif

#endif	// __cocos_script_h__