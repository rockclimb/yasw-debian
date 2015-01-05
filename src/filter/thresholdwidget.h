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
#ifndef THRESHOLDWIDGET_H
#define THRESHOLDWIDGET_H

#include <QWidget>
#include <QIntValidator>
#include <QColor>
#include "abstractfilterwidget.h"


namespace Ui {
class ThresholdWidget;
}

class ThresholdWidget : public AbstractFilterWidget
{
    Q_OBJECT
    
public:
    explicit ThresholdWidget(QWidget *parent = 0);
    ~ThresholdWidget();
    void setPixmap(QPixmap pixmap);
    void setPreview(QPixmap pixmap);
    bool preview();
    void enableFilter(bool enable);

    // Get widget parameters for different clases
    // Noise preprocessing
    bool preprocessNoise();
    // Threshold
    bool rbThreshold();
    int threshold();
    // adaptative Threshold
    bool rbAdaptativeThreshold();
    double cvalue();
    int blockSize();
    // Otsu Threshold
    bool rbOtsuThreshold();
    void setOtsuThreshold(double threshold);

public slots:
    void on_preview_toggled(bool checked);
    void setBackgroundColor(QColor color);

private slots:
    void on_enable_toggled(bool checked);
    void paramChanged();
private:
    Ui::ThresholdWidget *ui;
    QIntValidator *intValidator = NULL;
};

#endif // THRESHOLDWIDGET_H
