/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransformHelp.cpp
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

#include "editor-support/cocostudio/armature/utils/CCTransformHelp.h"
#include "editor-support/cocostudio/armature/utils/CCUtilMath.h"

using namespace cocos2d;

namespace cocostudio {

AffineTransform TransformHelp::m_tHelpMatrix1;
AffineTransform TransformHelp::m_tHelpMatrix2;

Point TransformHelp::m_tHelpPoint1;
Point TransformHelp::m_tHelpPoint2;

BaseData helpParentNode;

TransformHelp::TransformHelp()
{
}

void TransformHelp::transformFromParent(BaseData &node, const BaseData &parentNode)
{
    nodeToMatrix(node, m_tHelpMatrix1);
    nodeToMatrix(parentNode, m_tHelpMatrix2);

    m_tHelpMatrix2 = AffineTransformInvert(m_tHelpMatrix2);
    m_tHelpMatrix1 = AffineTransformConcat(m_tHelpMatrix1, m_tHelpMatrix2);

    matrixToNode(m_tHelpMatrix1, node);
}

void TransformHelp::transformToParent(BaseData &node, const BaseData &parentNode)
{

    nodeToMatrix(node, m_tHelpMatrix1);
    nodeToMatrix(parentNode, m_tHelpMatrix2);

    m_tHelpMatrix1 = AffineTransformConcat(m_tHelpMatrix1, m_tHelpMatrix2);

    matrixToNode(m_tHelpMatrix1, node);
}

void TransformHelp::transformFromParentWithoutScale(BaseData &node, const BaseData &parentNode)
{

    helpParentNode.copy(&parentNode);
    helpParentNode.m_fScaleX = 1;
    helpParentNode.m_fScaleY = 1;

    nodeToMatrix(node, m_tHelpMatrix1);
    nodeToMatrix(helpParentNode, m_tHelpMatrix2);

    m_tHelpMatrix2 = AffineTransformInvert(m_tHelpMatrix2);
    m_tHelpMatrix1 = AffineTransformConcat(m_tHelpMatrix1, m_tHelpMatrix2);

    matrixToNode(m_tHelpMatrix1, node);
}

void TransformHelp::transformToParentWithoutScale(BaseData &node, const BaseData &parentNode)
{

    helpParentNode.copy(&parentNode);
    helpParentNode.m_fScaleX = 1;
    helpParentNode.m_fScaleY = 1;

    nodeToMatrix(node, m_tHelpMatrix1);
    nodeToMatrix(helpParentNode, m_tHelpMatrix2);

    m_tHelpMatrix1 = AffineTransformConcat(m_tHelpMatrix1, m_tHelpMatrix2);

    matrixToNode(m_tHelpMatrix1, node);
}

void TransformHelp::nodeToMatrix(const BaseData &node, AffineTransform &matrix)
{
    if (node.m_fSkewX == -node.m_fSkewY)
    {
        double sine   = kdSinf(node.m_fSkewX);
        double cosine = kdCosf(node.m_fSkewX);
        
        matrix.a = node.m_fScaleX * cosine;
        matrix.b = node.m_fScaleX * -sine;
        matrix.c = node.m_fScaleY * sine;
        matrix.d = node.m_fScaleY * cosine;
    }
    else
    {
        matrix.a = node.m_fScaleX * kdCosf(node.m_fSkewY);
        matrix.b = node.m_fScaleX * kdSinf(node.m_fSkewY);
        matrix.c = node.m_fScaleY * kdSinf(node.m_fSkewX);
        matrix.d = node.m_fScaleY * kdCosf(node.m_fSkewX);
    }

    matrix.tx = node.x;
    matrix.ty = node.y;
}

void TransformHelp::matrixToNode(const AffineTransform &matrix, BaseData &node)
{
    /*
     *  In as3 language, there is a function called "deltaTransformPoint", it calculate a point used give Transform
     *  but not used the tx, ty value. we simulate the function here
     */
    m_tHelpPoint1.x = 0;
    m_tHelpPoint1.y = 1;
    m_tHelpPoint1 = PointApplyAffineTransform(m_tHelpPoint1, matrix);
    m_tHelpPoint1.x -= matrix.tx;
    m_tHelpPoint1.y -= matrix.ty;

    m_tHelpPoint2.x = 1;
    m_tHelpPoint2.y = 0;
    m_tHelpPoint2 = PointApplyAffineTransform(m_tHelpPoint2, matrix);
    m_tHelpPoint2.x -= matrix.tx;
    m_tHelpPoint2.y -= matrix.ty;

    node.m_fSkewX = -(kdAtan2f(m_tHelpPoint1.y, m_tHelpPoint1.x) - 1.5707964f);
    node.m_fSkewY = kdAtan2f(m_tHelpPoint2.y, m_tHelpPoint2.x);
    node.m_fScaleX = kdSqrtf(matrix.a * matrix.a + matrix.b * matrix.b);
    node.m_fScaleY = kdSqrtf(matrix.c * matrix.c + matrix.d * matrix.d);
    node.x = matrix.tx;
    node.y = matrix.ty;
}

void TransformHelp::nodeConcat(BaseData &target, BaseData &source)
{
    target.x += source.x;
    target.y += source.y;
    target.m_fSkewX += source.m_fSkewX;
    target.m_fSkewY += source.m_fSkewY;
    target.m_fScaleX += source.m_fScaleX;
    target.m_fScaleY += source.m_fScaleY;
}

void TransformHelp::nodeSub(BaseData &target, BaseData &source)
{
    target.x -= source.x;
    target.y -= source.y;
    target.m_fSkewX -= source.m_fSkewX;
    target.m_fSkewY -= source.m_fSkewY;
    target.m_fScaleX -= source.m_fScaleX;
    target.m_fScaleY -= source.m_fScaleY;
}

}
