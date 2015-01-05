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
#include <QImage>
#include <QDebug>
#include <cv.h>

#include "threshold.h"
#include "openCVToQt.hpp"

Threshold::Threshold(QObject *parent) : BaseFilter(parent)
{
    widget = new ThresholdWidget();
    filterWidget = widget;

    connect(widget, SIGNAL(parameterChanged()),
            this, SLOT(widgetParameterChanged()));

    if (parent) {
        /* Connect slots to the filtercontainer */
        connect(parent, SIGNAL(backgroundColorChanged(QColor)),
                widget, SLOT(setBackgroundColor(QColor)));
    }

    // Connect seems only to work when applied to the inherited classes
    // I would have love to connect one for all in Basefilter...
    connect(widget, SIGNAL(enableFilterToggled(bool)),
            this, SLOT(enableFilterToggled(bool)));
    connect(widget, SIGNAL(previewChecked()),
            this, SLOT(previewChecked()));

}

QString Threshold::getIdentifier()
{
    return QString("threshold");
}

QString Threshold::getName()
{
    return tr("Threshold Binarization");
}

QMap<QString, QVariant> Threshold::getSettings()
{
    QMap<QString, QVariant> settings;

    settings["enabled"] = filterEnabled;

    return settings;
}

void Threshold::setSettings(QMap<QString, QVariant> settings)
{
    loadingSettings = true;

    QColor color;

    if (settings.contains("enabled"))
        enableFilter(settings["enabled"].toBool());
    else
        enableFilter("true");

    mustRecalculate = true;
    loadingSettings = false;
}

void Threshold::settings2Dom(QDomDocument &doc, QDomElement &parent, QMap<QString, QVariant> settings)
{
    QDomElement filter = doc.createElement(getIdentifier());
    parent.appendChild(filter);

    if (settings.contains("enabled"))
        filter.setAttribute("enabled", settings["enabled"].toBool());
    else
        filter.setAttribute("enabled", true);
}

QMap<QString, QVariant> Threshold::dom2Settings(QDomElement &filterElement)
{
    QMap<QString, QVariant> settings;

    settings["whitepoint"] = filterElement.attribute("whitepoint", "#FFFFFF");
    settings["blackpoint"] = filterElement.attribute("blackpoint", "#000000");

    return settings;
}


/* Scales every pixel value of the image so that it matches the choosen White and Black points.
 *  For every color (here red) whe have:
 *  - an intensity "red"
 *  - the red component of the white point "redWhite"
 *  - the red component of the white point "redBlack"
 *  - the new intensity "redNew"
 * So we just need to scale the range redBlack..redWhite to 0..255, points under redBlack are set to 0, points over redWhite to 255:
 *   redNew = red * 255 / (redWhite - redBlack) - redBlack) (plus min and max)
 *      NOTE: to optimise, we use redDelta = (redWhite - redBlack); dividing by 255 may be contra-productive
 *            as it needs the use of real values.
 *
 * NOTE: performance improvements might be possible (use of scanline() or preview a scaled image)
 * NOTE: move this code to an ImageManipulation class?
 */
// FIXME: catch opencv problems
QImage Threshold::filter(QImage inputImage)
{
    if (!filterEnabled)
        return inputImage;

    if (inputImage.isNull())
        return inputImage;

    cv::Mat src = qimage_to_mat_cpy(inputImage);
    cv::Mat dst;

    // Grey the image
    cv::cvtColor(src, dst, CV_BGR2GRAY);
    // For programming simplicity, after each filter, we replace source with the new calculated image.
    // The dst matrix is released so we are sure to work with a clean matrix
    src = dst;
    dst.release();


    if (widget->preprocessNoise()) {
        // 9 is a good value for the bilateral filter, so we do not offer to configure it.
        cv::bilateralFilter(src, dst, 9, 9, 9);
        src = dst;
        dst.release();
    }


    if (widget->rbThreshold()) {
        cv::threshold(src, dst, widget->threshold(), 255,
                        cv::THRESH_BINARY);
        src=dst;
        dst.release();
    }

    if (widget->rbAdaptativeThreshold()) {
        cv::adaptiveThreshold(src, dst, 255, cv:: ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY,
                              widget->blockSize(), widget->cvalue());
        src=dst;
        dst.release();
    }

    if (widget->rbOtsuThreshold()) {
        double otsuThreshold = cv::threshold(src, dst, widget->threshold(), 255,
                               cv::THRESH_BINARY + cv::THRESH_OTSU);
        widget->setOtsuThreshold(otsuThreshold);
        src=dst;
        dst.release();
    }

    // For programatical purposes, we do let src=dst on the last step: it is simpler to add new options.
    // => our output is src, as dst has been re
    QImage outputImage = mat_to_qimage_cpy(src);

    return outputImage;
}


