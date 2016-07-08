/* -----------------------------------------------------------------------------------
 *
 *      File            CCDataVisitor.cpp
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

#include "base/CCObject.h"
#include "base/CCBool.h"
#include "base/CCInteger.h"
#include "base/CCFloat.h"
#include "base/CCDouble.h"
#include "base/CCString.h"
#include "base/CCArray.h"
#include "base/CCDictionary.h"
#include "base/CCSet.h"
#include "base/CCData.h"

NS_CC_BEGIN

void DataVisitor::visit(const Bool *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Integer *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Float *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Double *value)
{
    visitObject(value);
}

void DataVisitor::visit(const String *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Array *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Dictionary *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Set *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Data *value)
{
    visitObject(value);
}

// PrettyPrinter
PrettyPrinter::PrettyPrinter(int indentLevel/* = 0 */)
{
    setIndentLevel(indentLevel);
}

void PrettyPrinter::clear()
{
    m_sResult.clear();
}

std::string PrettyPrinter::getResult()
{
    return m_sResult;
}

void PrettyPrinter::visitObject(const Object *p)
{
    char buf[50] = {0};
    kdSprintf(buf, "%p", p);
    m_sResult += buf;
}

void PrettyPrinter::visit(const Bool * p)
{
	char buf[50] = {0};
	kdSprintf(buf, "%s", p->getValue() ? "true" : "false");
	m_sResult += buf;
}

void PrettyPrinter::visit(const Integer *p)
{
    char buf[50] = {0};
    kdSprintf(buf, "%d", p->getValue());
    m_sResult += buf;
}

void PrettyPrinter::visit(const Float *p)
{
    char buf[50] = {0};
    kdSprintf(buf, "%f", p->getValue());
    m_sResult += buf;
}

void PrettyPrinter::visit(const Double *p)
{
    char buf[50] = {0};
    kdSprintf(buf, "%lf", p->getValue());
    m_sResult += buf;
}

void PrettyPrinter::visit(const String *p)
{
    m_sResult += p->getCString();
}

void PrettyPrinter::visit(const Array *p)
{
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "<array>\n";

    setIndentLevel(m_nIndentLevel+1);
    Object* obj;
    int i = 0;
    char buf[50] = {0};
    CCARRAY_FOREACH(p, obj)
    {
        if (i > 0) {
            m_sResult += "\n";
        }
        kdSprintf(buf, "%s%02d: ", m_sIndentStr.c_str(), i);
        m_sResult += buf;
        PrettyPrinter v(m_nIndentLevel);
        obj->acceptVisitor(v);
        m_sResult += v.getResult();
        i++;
    }
    setIndentLevel(m_nIndentLevel-1);
    
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "</array>";
}

void PrettyPrinter::visit(const Dictionary *p)
{
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "<dict>\n";
    
    setIndentLevel(m_nIndentLevel+1);
    DictElement* element;
    bool bFirstElement = true;
    char buf[1000] = {0};
    CCDICT_FOREACH(p, element)
    {
        if (!bFirstElement) {
            m_sResult += "\n";
        }
        kdSprintf(buf, "%s%s: ", m_sIndentStr.c_str(),element->getStrKey());
        m_sResult += buf;
        PrettyPrinter v(m_nIndentLevel);
        element->getObject()->acceptVisitor(v);
        m_sResult += v.getResult();
        bFirstElement = false;
    }
    setIndentLevel(m_nIndentLevel-1);
    
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "</dict>";
}

void PrettyPrinter::visit(const Set *p)
{
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "<set>\n";
    
    setIndentLevel(m_nIndentLevel+1);

    int i = 0;
    Set* tmp = const_cast<Set*>(p);
    SetIterator it = tmp->begin();

    for (; it != tmp->end(); ++it, ++i) {
        if (i > 0) {
            m_sResult += "\n";
        }
        m_sResult += m_sIndentStr.c_str();
        PrettyPrinter v(m_nIndentLevel);
        (*it)->acceptVisitor(v);
        m_sResult += v.getResult();
    }
    setIndentLevel(m_nIndentLevel-1);
    
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "</set>\n";
}

void PrettyPrinter::visit(const Data *p)
{
	//TODO Implement
	DataVisitor::visit(p);
}

void PrettyPrinter::setIndentLevel(int indentLevel)
{
    m_nIndentLevel = indentLevel;
    m_sIndentStr.clear();
    for (int i = 0; i < m_nIndentLevel; ++i) {
        m_sIndentStr += "\t";
    }
}

NS_CC_END
