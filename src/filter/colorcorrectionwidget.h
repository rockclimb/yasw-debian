/*
 * Copyright (C) 2012-2015 Robert Chéramy (robert@cheramy.net)
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
#ifndef COLORCORRECTIONWIDGET_H
#define COLORCORRECTIONWIDGET_H

#include <QWidget>
#include <QIntValidator>
#include <QColor>
#include "abstractfilterwidget.h"


namespace Ui {
class ColorCorrectionWidget;
}

class ColorCorrectionWidget : public AbstractFilterWidget
{
    Q_OBJECT
    
public:
    explicit ColorCorrectionWidget(QWidget *parent = 0);
    ~ColorCorrectionWidget();
    void setPixmap(QPixmap pixmap);
    void setPreview(QPixmap pixmap);
    bool preview();
    QColor whitePoint();
    QColor blackPoint();
    void setWhitePoint(QColor white);
    void setBlackPoint(QColor black);
    void enableFilter(bool enable);

public slots:
    void on_preview_toggled(bool checked);
    void imageClicked(QColor color);
    void setBackgroundColor(QColor color);

private slots:
    void on_enable_toggled(bool checked);
    void on_whitepoint_clicked();
    void on_blackpoint_clicked();

    void on_pickWhitepoint_clicked();

    void on_pickBlackpoint_clicked();

private:
    Ui::ColorCorrectionWidget *ui;
    QIntValidator *intValidator = NULL;
    bool pickingWhiteColor = false;
    bool pickingBlackColor = false;
    QColor whitepoint;
    QColor blackpoint;
};

#endif // COLORCORRECTIONWIDGET_H
