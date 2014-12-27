/*
 * Copyright (C) 2012-2014 Robert Chéramy (robert@cheramy.net)
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
#ifndef CROPPINGGRAPHICSVIEW_H
#define CROPPINGGRAPHICSVIEW_H
#include <QGraphicsRectItem>
#include "basefiltergraphicsview.h"
#include "croppingcorner.h"

class CroppingGraphicsView : public BaseFilterGraphicsView
{
    Q_OBJECT
public:
    CroppingGraphicsView(QWidget *parent);
    ~CroppingGraphicsView();
    QRect getRectangle();
    bool rectangleMoved();
    void resetRectangleMoved();
    QMap<QString, QVariant> getSettings();
    void setSettings(QMap <QString, QVariant> settings);
    void setSelectionColor(QColor color);

public slots:
    void moveRectangle();
    void setPreview(bool preview);
    void setEnabled(bool enabled);

private:
    void hideRectangle(bool hide);
    // Defaults defined in croppingwidget.ui
    bool preview = false;
    bool enabled = true;

    CroppingCorner *topLeftCorner;
    CroppingCorner *bottomRightCorner;
    QGraphicsRectItem *rectangle;
    const QPoint defaultTopLeft = QPoint(100, 100);
    const QPoint defaultBottomRight = QPoint(500, 500);
signals:
    void parameterChanged();
};

#endif // CROPPINGGRAPHICSVIEW_H
