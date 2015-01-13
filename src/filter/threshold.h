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
#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "cv.h"
#include "basefilter.h"
#include "thresholdwidget.h"

class Threshold : public BaseFilter
{
public:
    Threshold(QObject * parent = 0);
    QString getIdentifier();
    QString getName();
    QMap<QString, QVariant> getSettings();
    void setSettings(QMap <QString, QVariant> settings);
    void settings2Dom(QDomDocument &doc, QDomElement &parent, QMap<QString, QVariant> settings);
    QMap<QString, QVariant> dom2Settings(QDomElement &filterElement);

protected:
    virtual QImage filter(QImage inputImage);

private:
    ThresholdWidget *widget;
};

#endif // THRESHOLD_H