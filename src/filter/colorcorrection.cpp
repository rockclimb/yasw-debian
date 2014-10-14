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
#include "colorcorrection.h"
#include <QImage>
#include <QDebug>

ColorCorrection::ColorCorrection(QObject *parent) : BaseFilter(parent)
{
    widget = new ColorCorrectionWidget();
    filterWidget = widget;

    connect(widget, SIGNAL(parameterChanged()),
            this, SLOT(widgetParameterChanged()));

    if (parent) {
        /* Connect slots to the filtercontainer */
        connect(parent, SIGNAL(backgroundColorChanged(QColor)),
                widget, SLOT(setBackgroundColor(QColor)));
    }

}

QString ColorCorrection::getIdentifier()
{
    return QString("colorcorrection");
}

QString ColorCorrection::getName()
{
    return tr("Color Correction");
}

QMap<QString, QVariant> ColorCorrection::getSettings()
{
    QMap<QString, QVariant> settings;

    QColor color;

    color = widget->whitePoint();
    settings["whiteRedValue"] = color.red();
    settings["whiteGreenValue"] = color.green();
    settings["whiteBlueValue"] = color.blue();

    color = widget->blackPoint();
    settings["blackRedValue"] = color.red();
    settings["blackGreenValue"] = color.green();
    settings["blackBlueValue"] = color.blue();

    return settings;
}

void ColorCorrection::setSettings(QMap<QString, QVariant> settings)
{
    loadingSettings = true;

    QColor color;

    //FIXME: should default to 255,255,255 when no settings present.
    color.setRed(settings["whiteRedValue"].toInt());
    color.setGreen(settings["whiteGreenValue"].toInt());
    color.setBlue(settings["whiteBlueValue"].toInt());
    widget->setWhitePoint(color);

    color.setRed(settings["blackRedValue"].toInt());
    color.setGreen(settings["blackGreenValue"].toInt());
    color.setBlue(settings["blackBlueValue"].toInt());
    widget->setBlackPoint(color);

    mustRecalculate = true;
    loadingSettings = false;
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
QImage ColorCorrection::filter(QImage inputImage)
{
    QImage outputImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32_Premultiplied);

    int x, y; // coordinates in the image for the for() loops
    QRgb pixelColor;
    int redNew, redWhite, redBlack, redDelta;
    int greenNew, greenWhite, greenBlack, greenDelta;
    int blueNew, blueWhite, blueBlack, blueDelta;

    QColor whitePoint = widget->whitePoint();
    QColor blackPoint = widget->blackPoint();

    // Optimisation: Storing everything static in seperate values to avoid needless calls while computing.
    redWhite = whitePoint.red();
    greenWhite = whitePoint.green();
    blueWhite = whitePoint.blue();
    redBlack = blackPoint.red();
    greenBlack = blackPoint.green();
    blueBlack = blackPoint.blue();
    // as we divide through xxxDelta, it must at least be 1.
    redDelta = qMax(1, redWhite - redBlack);
    greenDelta = qMax(1, greenWhite - greenBlack);
    blueDelta = qMax(1, blueWhite - blueBlack);
    int imageWidth = inputImage.width();
    int imageHeight = inputImage.height();

    for (x = 0; x < imageWidth; x++) {
        for (y = 0; y < imageHeight; y++) {
            pixelColor = inputImage.pixel(x,y);
            redNew =   qMax(0, qMin(255, qRed(pixelColor)   * 255 / redDelta   - redBlack));
            greenNew = qMax(0, qMin(255, qGreen(pixelColor) * 255 / greenDelta - greenBlack));
            blueNew =  qMax(0, qMin(255, qBlue(pixelColor)  * 255 / blueDelta  - blueBlack));
            outputImage.setPixel(x, y, qRgb(redNew, greenNew, blueNew));
        }
    }
    return outputImage;
}
