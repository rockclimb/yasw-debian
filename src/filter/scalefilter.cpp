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

#include "scalefilter.h"

#include <QDebug>
#include <QPainter>

ScaleFilter::ScaleFilter(QObject * parent) : BaseFilter(parent)
{
    widget = new ScaleWidget();
    filterWidget = widget;

    connect(widget, SIGNAL(parameterChanged()),
            this, SLOT(widgetParameterChanged()));

    if (parent) {
        /* Connect slots to the filtercontainer */
        connect(parent, SIGNAL(backgroundColorChanged(QColor)),
                widget, SLOT(setBackgroundColor(QColor)));
        connect(parent, SIGNAL(displayUnitChanged(QString)),
                this, SLOT(setDisplayUnit(QString)));
        connect(parent, SIGNAL(dpiChanged(int)),
                widget, SLOT(setDPI(int)));
    }
}

QString ScaleFilter::getIdentifier()
{
    return QString("ScaleFilter");
}

QString ScaleFilter::getName()
{
    return tr("Scale");
}

/** \brief Gets the settings of the filter.

  As all the settings are maintained in the widget, this function just gets the setting from
  the widget and gives them back.
*/
QMap<QString, QVariant> ScaleFilter::getSettings()
{
    return widget->getSettings();
}

/** \brief Sets the settings for the filter.

  Gives the settings to the widget (were they are stored) and recalculate the produced Image.
 */

void ScaleFilter::setSettings(QMap<QString, QVariant> settings)
{
    loadingSettings = true;

    widget->setSettings(settings);

    mustRecalculate = true;
    loadingSettings = false;
}

void ScaleFilter::settings2Dom(QDomDocument &doc, QDomElement &parent, QMap<QString, QVariant> settings)
{
    QDomElement filter = doc.createElement(getIdentifier());
    parent.appendChild(filter);

    QString attribute;
    QStringList attributeNames;
    int i;

    // Iterate through attributeNames to save all double
    attributeNames << "pxImageWidth" << "pxImageHeight";

    for (i = 0; i < attributeNames.size(); i++) {
        attribute = attributeNames.at(i);
        if (settings.contains(attribute)) {
            filter.setAttribute(attribute, settings[attribute].toDouble());
        }
    }

}

QMap<QString, QVariant> ScaleFilter::dom2Settings(QDomElement &filterElement)
{
    QMap<QString, QVariant> settings;
    QString attribute;
    QStringList attributeNames;
    int i;
    QDomElement cornerElement;

    // Iterate through attributeNames to save all double
    attributeNames << "pxImageWidth" << "pxImageHeight";

    for (i = 0; i < attributeNames.size(); i++) {
        attribute = attributeNames.at(i);
        if (filterElement.hasAttribute(attribute)) {
            settings[attribute] = filterElement.attribute(attribute).toDouble();
        }
    }

    return settings;
}

void ScaleFilter::setDisplayUnit(QString unit)
{
    displayUnit = unit;
    widget->setDisplayUnit(unit);
}

QImage ScaleFilter::filter(QImage inputImage)
{
    qreal imageWidth = widget->imagePixelWidth();
    qreal imageHeight = widget->imagePixelHeight();

    if (imageWidth == 0 || imageHeight == 0 || inputPixmap.isNull()) {
        return QImage();
    }

    QSize outputImageSize = QSize(imageWidth, imageHeight);
    QImage scaledImage = inputImage.scaled(outputImageSize);
    return scaledImage;
}
