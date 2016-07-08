/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionManager.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2009      Valentin Milea
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

#include "2d/actions/CCActionManager.h"
#include "2d/base_nodes/CCNode.h"
#include "2d/CCScheduler.h"
#include "support/data_support/ccCArray.h"
#include "support/data_support/uthash.h"
#include "base/CCSet.h"

NS_CC_BEGIN
//
// singleton stuff
//
typedef struct _hashElement
{
    struct _ccArray*			actions;
    Object*						target;
    KDint						actionIndex;
    Action*						currentAction;
    KDbool						currentActionSalvaged;
    KDbool						paused;
    UT_hash_handle				hh;
} tHashElement;

ActionManager::ActionManager(void)
: m_pTargets(NULL), 
  m_pCurrentTarget(NULL),
  m_bCurrentTargetSalvaged(false)
{

}

ActionManager::~ActionManager(void)
{
    CCLOGINFO("deallocing ActionManager: %p", this);

    removeAllActions();
}

// private

void ActionManager::deleteHashElement(tHashElement *element)
{
    ccArrayFree(element->actions);
    HASH_DEL(m_pTargets, element);
    element->target->release();
    kdFree(element);
}

void ActionManager::actionAllocWithHashElement(tHashElement *element)
{
    // 4 actions per Node by default
    if (element->actions == NULL)
    {
        element->actions = ccArrayNew(4);
    }else 
    if (element->actions->num == element->actions->max)
    {
        ccArrayDoubleCapacity(element->actions);
    }

}

KDvoid ActionManager::removeActionAtIndex ( KDint32 nIndex, tHashElement* pElement )
{
    Action *action = (Action*)pElement->actions->arr[nIndex];

    if (action == pElement->currentAction && (! pElement->currentActionSalvaged))
    {
        pElement->currentAction->retain();
        pElement->currentActionSalvaged = true;
    }

    ccArrayRemoveObjectAtIndex(pElement->actions, nIndex, true);

    // update actionIndex in case we are in tick. looping over the actions
    if (pElement->actionIndex >= nIndex)
    {
        pElement->actionIndex--;
    }

    if (pElement->actions->num == 0)
    {
        if (m_pCurrentTarget == pElement)
        {
            m_bCurrentTargetSalvaged = true;
        }
        else
        {
            deleteHashElement(pElement);
        }
    }
}

// pause / resume

void ActionManager::pauseTarget(Object *target)
{
    tHashElement *element = NULL;
    HASH_FIND_PTR(m_pTargets, &target, element);
    if (element)
    {
        element->paused = true;
    }
}

void ActionManager::resumeTarget(Object *target)
{
    tHashElement *element = NULL;
    HASH_FIND_PTR(m_pTargets, &target, element);
    if (element)
    {
        element->paused = false;
    }
}

Set* ActionManager::pauseAllRunningActions()
{
    Set *idsWithActions = new Set();
    idsWithActions->autorelease();
    
    for (tHashElement *element=m_pTargets; element != NULL; element = (tHashElement *)element->hh.next) 
    {
        if (! element->paused) 
        {
            element->paused = true;
            idsWithActions->addObject(element->target);
        }
    }    
    
    return idsWithActions;
}

void ActionManager::resumeTargets(cocos2d::Set *targetsToResume)
{    
    SetIterator iter;
    for (iter = targetsToResume->begin(); iter != targetsToResume->end(); ++iter)
    {
        resumeTarget(*iter);
    }
}

// run

void ActionManager::addAction(Action *action, Node *target, bool paused)
{
    CCASSERT(action != NULL, "");
    CCASSERT(target != NULL, "");

    tHashElement *element = NULL;
    // we should convert it to Object*, because we save it as Object*
    Object *tmp = target;
    HASH_FIND_PTR(m_pTargets, &tmp, element);
    if (! element)
    {
        element = (tHashElement*)kdCalloc(sizeof(*element), 1);
        element->paused = paused;
        target->retain();
        element->target = target;
        HASH_ADD_PTR(m_pTargets, target, element);
    }

     actionAllocWithHashElement(element);
 
     CCASSERT(! ccArrayContainsObject(element->actions, action), "");
     ccArrayAppendObject(element->actions, action);
 
     action->startWithTarget(target);
}

// remove

void ActionManager::removeAllActions(void)
{
    for (tHashElement *element = m_pTargets; element != NULL; )
    {
        Object *target = element->target;
        element = (tHashElement*)element->hh.next;
        removeAllActionsFromTarget(target);
    }
}

void ActionManager::removeAllActionsFromTarget(Object *target)
{
    // explicit null handling
    if (target == NULL)
    {
        return;
    }

    tHashElement *element = NULL;
    HASH_FIND_PTR(m_pTargets, &target, element);
    if (element)
    {
        if (ccArrayContainsObject(element->actions, element->currentAction) && (! element->currentActionSalvaged))
        {
            element->currentAction->retain();
            element->currentActionSalvaged = true;
        }

        ccArrayRemoveAllObjects(element->actions);
        if (m_pCurrentTarget == element)
        {
            m_bCurrentTargetSalvaged = true;
        }
        else
        {
            deleteHashElement(element);
        }
    }
    else
    {
//        CCLOG("cocos2d: removeAllActionsFromTarget: Target not found");
    }
}

void ActionManager::removeAction(Action *action)
{
    // explicit null handling
    if (action == NULL)
    {
        return;
    }

    tHashElement *element = NULL;
    Object *target = action->getOriginalTarget();
    HASH_FIND_PTR(m_pTargets, &target, element);
    if (element)
    {
        long i = ccArrayGetIndexOfObject(element->actions, action);
        if (i != CC_INVALID_INDEX)
        {
            removeActionAtIndex(i, element);
        }
    }
    else
    {
        CCLOG("cocos2d: removeAction: Target not found");
    }
}

void ActionManager::removeActionByTag(int tag, Object *target)
{
    CCASSERT(tag != Action::INVALID_TAG, "");
    CCASSERT(target != NULL, "");

    tHashElement *element = NULL;
    HASH_FIND_PTR(m_pTargets, &target, element);

    if (element)
    {
        long limit = element->actions->num;
        for (long i = 0; i < limit; ++i)
        {
            Action *action = (Action*)element->actions->arr[i];

            if (action->getTag() == (int)tag && action->getOriginalTarget() == target)
            {
                removeActionAtIndex(i, element);
                break;
            }
        }
    }
}

// get

// XXX: Passing "const O *" instead of "const O&" because HASH_FIND_IT requries the address of a pointer
// and, it is not possible to get the address of a reference
Action* ActionManager::getActionByTag(int tag, const Object *target) const
{
    CCASSERT(tag != Action::INVALID_TAG, "");

    tHashElement *element = NULL;
    HASH_FIND_PTR(m_pTargets, &target, element);

    if (element)
    {
        if (element->actions != NULL)
        {
            long limit = element->actions->num;
            for (long i = 0; i < limit; ++i)
            {
                Action *action = (Action*)element->actions->arr[i];

                if (action->getTag() == (int)tag)
                {
                    return action;
                }
            }
        }
        CCLOG("cocos2d : getActionByTag(tag = %d): Action not found", tag);
    }
    else
    {
        // CCLOG("cocos2d : getActionByTag: Target not found");
    }

    return NULL;
}

// XXX: Passing "const O *" instead of "const O&" because HASH_FIND_IT requries the address of a pointer
// and, it is not possible to get the address of a reference
KDint32 ActionManager::getNumberOfRunningActionsInTarget ( const Object* pTarget ) const
{
    tHashElement *element = NULL;
    HASH_FIND_PTR(m_pTargets, &pTarget, element);
    if (element)
    {
        return element->actions ? element->actions->num : 0;
    }

    return 0;
}

// main loop
void ActionManager::update(float dt)
{
    for (tHashElement *elt = m_pTargets; elt != NULL; )
    {
        m_pCurrentTarget = elt;
        m_bCurrentTargetSalvaged = false;

        if (! m_pCurrentTarget->paused)
        {
            // The 'actions' MutableArray may change while inside this loop.
            for (m_pCurrentTarget->actionIndex = 0; m_pCurrentTarget->actionIndex < m_pCurrentTarget->actions->num;
                m_pCurrentTarget->actionIndex++)
            {
                m_pCurrentTarget->currentAction = (Action*)m_pCurrentTarget->actions->arr[m_pCurrentTarget->actionIndex];
                if (m_pCurrentTarget->currentAction == NULL)
                {
                    continue;
                }

                m_pCurrentTarget->currentActionSalvaged = false;

                m_pCurrentTarget->currentAction->step(dt);

                if (m_pCurrentTarget->currentActionSalvaged)
                {
                    // The currentAction told the node to remove it. To prevent the action from
                    // accidentally deallocating itself before finishing its step, we retained
                    // it. Now that step is done, it's safe to release it.
                    m_pCurrentTarget->currentAction->release();
                } else
                if (m_pCurrentTarget->currentAction->isDone())
                {
                    m_pCurrentTarget->currentAction->stop();

                    Action *action = m_pCurrentTarget->currentAction;
                    // Make currentAction nil to prevent removeAction from salvaging it.
                    m_pCurrentTarget->currentAction = NULL;
                    removeAction(action);
                }

                m_pCurrentTarget->currentAction = NULL;
            }
        }

        // elt, at this moment, is still valid
        // so it is safe to ask this here (issue #490)
        elt = (tHashElement*)(elt->hh.next);

        // only delete currentTarget if no actions were scheduled during the cycle (issue #481)
        if (m_bCurrentTargetSalvaged && m_pCurrentTarget->actions->num == 0)
        {
            deleteHashElement(m_pCurrentTarget);
        }
    }

    // issue #635
    m_pCurrentTarget = NULL;
}

NS_CC_END
