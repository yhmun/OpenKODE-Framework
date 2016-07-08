/* -----------------------------------------------------------------------------------
 *
 *      File            CCSGUIReader.h
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

#ifndef __CCSGUIReader_h__
#define __CCSGUIReader_h__

#include "../../../gui/BaseClasses/UIWidget.h"
#include "../json/CSContentJsonDictionary.h"

namespace cocostudio {

#define kCCSVersion			1.0

class GUIReader : public cocos2d::Object
{
	public :

		 GUIReader ( KDvoid );
		~GUIReader ( KDvoid );

		static GUIReader*			shareReader ( KDvoid );
		static KDvoid				purgeGUIReader ( KDvoid );
    
		gui::UIWidget*				widgetFromJsonFile  ( const KDchar* pFileName );
		KDint						getVersionInteger   ( const KDchar* pStr );
		KDvoid						storeFileDesignSize ( const KDchar* pFileName, const cocos2d::Size& tSize );
		const cocos2d::Size			getFileDesignSize   ( const KDchar* pFileName ) const;

	protected :

		std::string					m_strFilePath;
		cocos2d::Dictionary*		m_pFileDesignSizes;
};

class WidgetPropertiesReader : public cocos2d::Object
{
	public :

		virtual gui::UIWidget*		createWidget ( JsonDictionary* pDic, const KDchar* pFullPath, const KDchar* pFileName ) = 0;
		virtual gui::UIWidget*		widgetFromJsonDictionary ( JsonDictionary* pDic ) = 0;

	protected :

		std::string					m_sFilePath;
};


class WidgetPropertiesReader0250 : public WidgetPropertiesReader
{        
	public :
				 WidgetPropertiesReader0250 ( KDvoid ) { };
		virtual ~WidgetPropertiesReader0250 ( KDvoid ) { };
    
		virtual gui::UIWidget*		createWidget ( JsonDictionary* pDic, const KDchar* pFullPath, const KDchar* pFileName );
		virtual gui::UIWidget*		widgetFromJsonDictionary ( JsonDictionary* pDic );

		virtual KDvoid				setPropsForWidgetFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
    
		virtual KDvoid				setColorPropsForWidgetFromJsonDictionary	( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForButtonFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForCheckBoxFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForImageViewFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForLabelFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForLabelAtlasFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForLabelBMFontFromJsonDictionary	( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForLoadingBarFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForSliderFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForTextFieldFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
    
		virtual KDvoid				setPropsForLayoutFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForScrollViewFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
};

class WidgetPropertiesReader0300 : public WidgetPropertiesReader
{        
	public :
				 WidgetPropertiesReader0300 ( KDvoid ) { };
		virtual ~WidgetPropertiesReader0300 ( KDvoid ) { };
    
		virtual gui::UIWidget*		createWidget ( JsonDictionary* pDic, const KDchar* pFullPath, const KDchar* pFileName );
		virtual gui::UIWidget*		widgetFromJsonDictionary ( JsonDictionary* pDic );

		virtual KDvoid				setPropsForWidgetFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
      
		virtual KDvoid				setColorPropsForWidgetFromJsonDictionary	( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForButtonFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForCheckBoxFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForImageViewFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForLabelFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForLabelAtlasFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForLabelBMFontFromJsonDictionary	( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForLoadingBarFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForSliderFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForTextFieldFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
    
		virtual KDvoid				setPropsForLayoutFromJsonDictionary			( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForPageViewFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForScrollViewFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
		virtual KDvoid				setPropsForListViewFromJsonDictionary		( gui::UIWidget* pWidget, JsonDictionary* pOptions );
};

}

#endif	// __CCSGUIReader_h__
