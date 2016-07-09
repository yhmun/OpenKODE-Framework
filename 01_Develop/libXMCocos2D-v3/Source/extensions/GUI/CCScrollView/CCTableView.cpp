/* -----------------------------------------------------------------------------------
 *
 *      File            CCTableView.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2010      Sangwoo Im.
 *
 *          http://www.cocos2d-x.org
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

#include "extensions/GUI/CCScrollView/CCTableView.h"
#include "extensions/GUI/CCScrollView/CCTableViewCell.h"
#include "2d/menu_nodes/CCMenu.h"
#include "extensions/GUI/CCScrollView/CCSorting.h"
#include "2d/layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_EXT_BEGIN

TableView* TableView::create(TableViewDataSource* dataSource, Size size)
{
    return TableView::create(dataSource, size, NULL);
}

TableView* TableView::create(TableViewDataSource* dataSource, Size size, Node *container)
{
    TableView *table = new TableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();

    return table;
}

bool TableView::initWithViewSize(Size size, Node* container/* = NULL*/)
{
    if (ScrollView::initWithViewSize(size,container))
    {
        m_pCellsUsed      = new ArrayForObjectSorting();
        m_pCellsUsed->init();
        m_pCellsFreed     = new ArrayForObjectSorting();
        m_pCellsFreed->init();
        m_pIndices        = new std::set<KDint32>();
        m_eVordering      = VerticalFillOrder::BOTTOM_UP;
        this->setDirection(Direction::VERTICAL);

        ScrollView::setDelegate(this);
        return true;
    }
    return false;
}

TableView::TableView()
: m_pTouchedCell(nullptr)
, m_pIndices(nullptr)
, m_pCellsUsed(nullptr)
, m_pCellsFreed(nullptr)
, m_pDataSource(nullptr)
, m_pTableViewDelegate(nullptr)
, m_eOldDirection(Direction::NONE)
{

}

TableView::~TableView()
{
    CC_SAFE_DELETE(m_pIndices);
    CC_SAFE_RELEASE(m_pCellsUsed);
    CC_SAFE_RELEASE(m_pCellsFreed);
}

void TableView::setVerticalFillOrder(VerticalFillOrder fillOrder)
{
    if (m_eVordering != fillOrder) {
        m_eVordering = fillOrder;
        if (m_pCellsUsed->count() > 0) {
            this->reloadData();
        }
    }
}

TableView::VerticalFillOrder TableView::getVerticalFillOrder()
{
    return m_eVordering;
}

void TableView::reloadData()
{
    m_eOldDirection = Direction::NONE;
    Object* pObj = NULL;
    CCARRAY_FOREACH(m_pCellsUsed, pObj)
    {
        TableViewCell* cell = static_cast<TableViewCell*>(pObj);

        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellWillRecycle(this, cell);
        }

        m_pCellsFreed->addObject(cell);
        cell->reset();
        if (cell->getParent() == this->getContainer())
        {
            this->getContainer()->removeChild(cell, true);
        }
    }

    m_pIndices->clear();
    m_pCellsUsed->release();
    m_pCellsUsed = new ArrayForObjectSorting();
    m_pCellsUsed->init();

    this->_updateCellPositions();
    this->_updateContentSize();
    if (m_pDataSource->numberOfCellsInTableView(this) > 0)
    {
        this->scrollViewDidScroll(this);
    }
}

TableViewCell *TableView::cellAtIndex(KDint32 idx)
{
    TableViewCell *found = NULL;

    if (m_pIndices->find(idx) != m_pIndices->end())
    {
        found = (TableViewCell *)m_pCellsUsed->objectWithObjectID(idx);
    }

    return found;
}

void TableView::updateCellAtIndex(KDint32 idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    long countOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }

    TableViewCell* cell = this->cellAtIndex(idx);
    if (cell)
    {
        this->_moveCellOutOfSight(cell);
    }
    cell = m_pDataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);
}

void TableView::insertCellAtIndex(KDint32 idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }

    long countOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }

    TableViewCell* cell = NULL;
    long newIdx = 0;

    cell = static_cast<TableViewCell*>(m_pCellsUsed->objectWithObjectID(idx));
    if (cell)
    {
        newIdx = m_pCellsUsed->indexOfSortedObject(cell);
        for (long i = newIdx; i<m_pCellsUsed->count(); i++)
        {
            cell = static_cast<TableViewCell*>(m_pCellsUsed->getObjectAtIndex(i));
            this->_setIndexForCell(cell->getIdx()+1, cell);
        }
    }

 //   [_indices shiftIndexesStartingAtIndex:idx by:1];

    //insert a new cell
    cell = m_pDataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);

    this->_updateCellPositions();
    this->_updateContentSize();
}

void TableView::removeCellAtIndex(KDint32 idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }

    long uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }

    unsigned int newIdx = 0;

    TableViewCell* cell = this->cellAtIndex(idx);
    if (!cell)
    {
        return;
    }

    newIdx = m_pCellsUsed->indexOfSortedObject(cell);

    //remove first
    this->_moveCellOutOfSight(cell);

    m_pIndices->erase(idx);
    this->_updateCellPositions();
//    [_indices shiftIndexesStartingAtIndex:idx+1 by:-1];
    for (unsigned int i=m_pCellsUsed->count()-1; i > newIdx; i--)
    {
        cell = (TableViewCell*)m_pCellsUsed->getObjectAtIndex(i);
        this->_setIndexForCell(cell->getIdx()-1, cell);
    }
}

TableViewCell *TableView::dequeueCell()
{
    TableViewCell *cell;

    if (m_pCellsFreed->count() == 0) {
        cell = NULL;
    } else {
        cell = (TableViewCell*)m_pCellsFreed->getObjectAtIndex(0);
        cell->retain();
        m_pCellsFreed->removeObjectAtIndex(0);
        cell->autorelease();
    }
    return cell;
}

void TableView::_addCellIfNecessary(TableViewCell * cell)
{
    if (cell->getParent() != this->getContainer())
    {
        this->getContainer()->addChild(cell);
    }
    m_pCellsUsed->insertSortedObject(cell);
    m_pIndices->insert(cell->getIdx());
    // [_indices addIndex:cell.idx];
}

void TableView::_updateContentSize()
{
    Size size = Size::ZERO;
    unsigned int cellsCount = m_pDataSource->numberOfCellsInTableView(this);

    if (cellsCount > 0)
    {
        float maxPosition = m_aCellsPositions[cellsCount];

        switch (this->getDirection())
        {
            case Direction::HORIZONTAL:
                size = Size(maxPosition, m_tViewSize.height);
                break;
            default:
                size = Size(m_tViewSize.width, maxPosition);
                break;
        }
    }

    this->setContentSize(size);

	if (m_eOldDirection != m_eDirection)
	{
		if (m_eDirection == Direction::HORIZONTAL)
		{
			this->setContentOffset(Point(0,0));
		}
		else
		{
			this->setContentOffset(Point(0,this->minContainerOffset().y));
		}
		m_eOldDirection = m_eDirection;
	}

}

Point TableView::_offsetFromIndex(KDint32 index)
{
    Point offset = this->__offsetFromIndex(index);

    const Size cellSize = m_pDataSource->tableCellSizeForIndex(this, index);
    if (m_eVordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y - cellSize.height;
    }
    return offset;
}

Point TableView::__offsetFromIndex(KDint32 index)
{
    Point offset;
    Size  cellSize;

    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            offset = Point(m_aCellsPositions[index], 0.0f);
            break;
        default:
            offset = Point(0.0f, m_aCellsPositions[index]);
            break;
    }

    return offset;
}

KDint32 TableView::_indexFromOffset(Point offset)
{
    KDint32 index = 0;
    const KDint32 maxIdx = m_pDataSource->numberOfCellsInTableView(this) - 1;

    if (m_eVordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y;
    }
    index = this->__indexFromOffset(offset);
    if (index != -1)
    {
        index = KD_MAX(0, index);
        if (index > maxIdx)
        {
            index = CC_INVALID_INDEX;
        }
    }

    return index;
}

KDint32 TableView::__indexFromOffset(Point offset)
{
    KDint32 low = 0;
    KDint32 high = m_pDataSource->numberOfCellsInTableView(this) - 1;
    float search;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            search = offset.x;
            break;
        default:
            search = offset.y;
            break;
    }

    while (high >= low)
    {
        KDint32 index = low + (high - low) / 2;
        float cellStart = m_aCellsPositions[index];
        float cellEnd = m_aCellsPositions[index + 1];

        if (search >= cellStart && search <= cellEnd)
        {
            return index;
        }
        else if (search < cellStart)
        {
            high = index - 1;
        }
        else
        {
            low = index + 1;
        }
    }

    if (low <= 0) {
        return 0;
    }

    return -1;
}

void TableView::_moveCellOutOfSight(TableViewCell *cell)
{
    if(m_pTableViewDelegate != NULL) {
        m_pTableViewDelegate->tableCellWillRecycle(this, cell);
    }

    m_pCellsFreed->addObject(cell);
    m_pCellsUsed->removeSortedObject(cell);
    m_pIndices->erase(cell->getIdx());
    // [_indices removeIndex:cell.idx];
    cell->reset();
    if (cell->getParent() == this->getContainer()) {
        this->getContainer()->removeChild(cell, true);;
    }
}

void TableView::_setIndexForCell(KDint32 index, TableViewCell *cell)
{
    cell->setAnchorPoint(Point(0.0f, 0.0f));
    cell->setPosition(this->_offsetFromIndex(index));
    cell->setIdx(index);
}

void TableView::_updateCellPositions() {
    int cellsCount = m_pDataSource->numberOfCellsInTableView(this);
    m_aCellsPositions.resize(cellsCount + 1, 0.0);

    if (cellsCount > 0)
    {
        float currentPos = 0;
        Size cellSize;
        for (int i=0; i < cellsCount; i++)
        {
            m_aCellsPositions[i] = currentPos;
            cellSize = m_pDataSource->tableCellSizeForIndex(this, i);
            switch (this->getDirection())
            {
                case Direction::HORIZONTAL:
                    currentPos += cellSize.width;
                    break;
                default:
                    currentPos += cellSize.height;
                    break;
            }
        }
        m_aCellsPositions[cellsCount] = currentPos;//1 extra value allows us to get right/bottom of the last cell
    }

}

void TableView::scrollViewDidScroll(ScrollView* view)
{
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems)
    {
        return;
    }

    if(m_pTableViewDelegate != NULL) {
        m_pTableViewDelegate->scrollViewDidScroll(this);
    }

    long startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    Point offset = this->getContentOffset() * -1;
    maxIdx = KD_MAX(uCountOfItems-1, 0);

    if (m_eVordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = offset.y + m_tViewSize.height/this->getContainer()->getScaleY();
    }
    startIdx = this->_indexFromOffset(offset);
	if (startIdx == CC_INVALID_INDEX)
	{
		startIdx = uCountOfItems - 1;
	}

    if (m_eVordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y -= m_tViewSize.height/this->getContainer()->getScaleY();
    }
    else
    {
        offset.y += m_tViewSize.height/this->getContainer()->getScaleY();
    }
    offset.x += m_tViewSize.width/this->getContainer()->getScaleX();

    endIdx   = this->_indexFromOffset(offset);
    if (endIdx == CC_INVALID_INDEX)
	{
		endIdx = uCountOfItems - 1;
	}

#if 0 // For Testing.
    Object* pObj;
    int i = 0;
    CCARRAY_FOREACH(_cellsUsed, pObj)
    {
        TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        log("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(_cellsFreed, pObj)
    {
        TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        log("cells freed index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

    if (m_pCellsUsed->count() > 0)
    {
        TableViewCell* cell = (TableViewCell*)m_pCellsUsed->getObjectAtIndex(0);

        idx = cell->getIdx();
        while(idx <startIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (TableViewCell*)m_pCellsUsed->getObjectAtIndex(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    if (m_pCellsUsed->count() > 0)
    {
        TableViewCell *cell = (TableViewCell*)m_pCellsUsed->getLastObject();
        idx = cell->getIdx();

        while(idx <= maxIdx && idx > endIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (TableViewCell*)m_pCellsUsed->getLastObject();
                idx = cell->getIdx();

            }
            else
            {
                break;
            }
        }
    }

    for (long i = startIdx; i <= endIdx; i++)
    {
        //if ([_indices containsIndex:i])
        if (m_pIndices->find(i) != m_pIndices->end())
        {
            continue;
        }
        this->updateCellAtIndex(i);
    }
}

void TableView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible()) {
        return;
    }

    if (m_pTouchedCell){
		Rect bb = this->getBoundingBox();
		bb.origin = m_pParent->convertToWorldSpace(bb.origin);

		if (bb.containsPoint(pTouch->getLocation()) && m_pTableViewDelegate != NULL)
        {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
            m_pTableViewDelegate->tableCellTouched(this, m_pTouchedCell);
        }

        m_pTouchedCell = NULL;
    }

    ScrollView::onTouchEnded(pTouch, pEvent);
}

bool TableView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible()) {
        return false;
    }

    bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);

    if(m_aTouches.size() == 1) {
        unsigned int        index;
        Point           point;

        point = this->getContainer()->convertTouchToNodeSpace(pTouch);

        index = this->_indexFromOffset(point);
		if (index == CC_INVALID_INDEX)
		{
			m_pTouchedCell = NULL;
		}
        else
		{
			m_pTouchedCell  = this->cellAtIndex(index);
		}

        if (m_pTouchedCell && m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellHighlight(this, m_pTouchedCell);
        }
    }
    else if(m_pTouchedCell) {
        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }

        m_pTouchedCell = NULL;
    }

    return touchResult;
}

void TableView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchMoved(pTouch, pEvent);

    if (m_pTouchedCell && isTouchMoved()) {
        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }

        m_pTouchedCell = NULL;
    }
}

void TableView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchCancelled(pTouch, pEvent);

    if (m_pTouchedCell) {
        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }

        m_pTouchedCell = NULL;
    }
}

NS_CC_EXT_END
