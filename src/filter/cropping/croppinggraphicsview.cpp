/*
 * Copyright (C) 2012 Robert Chéramy (robert@cheramy.net)
 *
 * This file is part of YASW (Yet Another Scan Wizard).
 *
 * YASW is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * YASW is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with YASW.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "croppinggraphicsview.h"

CroppingGraphicsView::CroppingGraphicsView(QWidget *parent) :
    BaseFilterGraphicsView(parent)
{
    topLeftCorner = new CroppingCorner(100, 100, TopLeftCorner, &rectangle);
    scene->addItem(topLeftCorner);

    bottomRightCorner = new CroppingCorner(500, 500, BottomRightCorner, &rectangle);
    scene->addItem(bottomRightCorner);

    scene->addRect(rectangle);
}

QRect CroppingGraphicsView::getRectangle()
{
    return QRect(topLeftCorner->scenePos().toPoint(),
                 bottomRightCorner->scenePos().toPoint());
}

void CroppingGraphicsView::resetRectangleMoved()
{
    topLeftCorner->resetCornerMoved();
    bottomRightCorner->resetCornerMoved();
}

bool CroppingGraphicsView::rectangleMoved()
{
    return topLeftCorner->getCornerMoved()
            || bottomRightCorner->getCornerMoved();
}

void CroppingGraphicsView::hideRectangle(bool hide)
{
    bool showRectangle = !hide;

    topLeftCorner->setVisible(showRectangle);
    bottomRightCorner->setVisible(showRectangle);
    //! \todo to hide Rectangle we need a QGraphicsRectItem
}