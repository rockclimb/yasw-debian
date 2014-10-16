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
#ifndef SCALING_H
#define SCALING_H

#include "basefilter.h"
#include "scalingwidget.h"

class Scaling : public BaseFilter
{
public:
    Scaling(QObject * parent = 0);
    QString getIdentifier();
    QString getName();
    QMap<QString, QVariant> getSettings();
    void setSettings(QMap <QString, QVariant> settings);
    void settings2Dom(QDomDocument &doc, QDomElement &imageElement, QMap<QString, QVariant> settings);
    QMap<QString, QVariant> dom2Settings(QDomElement &filterElement);
    qreal pageMilimeterHeight();
    qreal pageMilimeterWidth();
protected:
    virtual QImage filter(QImage inputImage);

private:
    ScalingWidget *widget;
    QTransform scalingMatrix;
};

#endif // SCALING_H
