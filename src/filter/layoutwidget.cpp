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

#include "layoutwidget.h"
#include "ui_layoutwidget.h"
#include "constants.h"

#include <QDebug>
#include <QString>
#include <QtCore/qmath.h>


LayoutWidget::LayoutWidget(QWidget *parent) :
    AbstractFilterWidget(parent),
    ui(new Ui::LayoutWidget)
{
    ui->setupUi(this);


    // default value
    displayUnit = "pixel";

    doubleValidator = new QDoubleValidator;
    doubleValidator->setBottom(0.001);
    doubleValidator->setDecimals(3);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->pageHeight->setValidator(doubleValidator);
    ui->pageWidth->setValidator(doubleValidator);

    ui->horizontalAlignment->insertItems(0, Constants::horizontalAlignment);
    ui->horizontalAlignment->setCurrentIndex(Constants::horizontalAlignment.indexOf("Center"));
    ui->verticalAlignment->insertItems(0, Constants::verticalAlignment);
    ui->verticalAlignment->setCurrentIndex(Constants::verticalAlignment.indexOf("Center"));

    setDPI(Constants::DEFAULT_DPI);
}

LayoutWidget::~LayoutWidget()
{
    delete ui;
    delete doubleValidator;
}

void LayoutWidget::setPixmap(QPixmap pixmap)
{
    inputPixmap = pixmap;
    if (!pixmap.isNull()
            && pxPageWidth == 0
            && pxPageHeight == 0) {
        pxPageWidth = pixmap.width();
        pxPageHeight = pixmap.height();
        updateFormSizes();
    }
    if (!preview()) {
        ui->view->setPixmap(pixmap);
    }
}

void LayoutWidget::setPreview(QPixmap pixmap)
{
    previewPixmap = pixmap;
    if (preview())
        ui->view->setPixmap(pixmap);
}

bool LayoutWidget::preview()
{
    return ui->preview->isChecked();
}

double LayoutWidget::pagePixelHeight()
{
    return pxPageHeight;
}

double LayoutWidget::pagePixelWidth()
{
    return pxPageWidth;
}

QString LayoutWidget::verticalAlignement()
{
    return ui->verticalAlignment->currentText();
}

QString LayoutWidget::horizontalAlignement()
{
    return ui->horizontalAlignment->currentText();
}

QMap<QString, QVariant> LayoutWidget::getSettings()
{
    QMap<QString, QVariant> settings;

    settings["pxPageWidth"] = pxPageWidth;
    settings["pxPageHeight"] = pxPageHeight;
    settings["verticalAlignement"] = verticalAlignement();
    settings["horizontalAlignement"] = horizontalAlignement();
    return settings;
}

void LayoutWidget::setSettings(QMap<QString, QVariant> settings)
{
    pxPageWidth = settings["pxPageWidth"].toDouble();
    pxPageHeight = settings["pxPageHeight"].toDouble();

    if (Constants::verticalAlignment.contains(settings["verticalAlignement"].toString())) {
        ui->verticalAlignment->setCurrentIndex(
                    Constants::verticalAlignment.indexOf(settings["verticalAlignement"].toString()));
    } else {
        ui->verticalAlignment->setCurrentIndex(
                    Constants::verticalAlignment.indexOf("Center"));
    }

    if (Constants::horizontalAlignment.contains(settings["horizontalAlignement"].toString())) {
        ui->horizontalAlignment->setCurrentIndex(
                    Constants::horizontalAlignment.indexOf(settings["horizontalAlignement"].toString()));
    } else {
        ui->horizontalAlignment->setCurrentIndex(
                    Constants::horizontalAlignment.indexOf("Center"));
    }

    // Update the Form
    updateFormSizes();

}

void LayoutWidget::setDisplayUnit(QString unit)
{
    switch (Constants::displayUnits.indexOf(unit)) {
        case 0:     // pixel
            factorPixeltoDisplayUnit = 1;
            break;
        case 1:     // millimeter
            factorPixeltoDisplayUnit = Constants::milimeterPerInch / dpi;
            break;
        case 2:     // inches
            factorPixeltoDisplayUnit = 1 / dpi;
            break;
        default:
        // The unit provided is not valid. Do nothing.
        return;
    }
    displayUnit = unit;
    ui->lbUnitHeight->setText(unit);
    ui->lbUnitWidth->setText(unit);

    updateFormSizes();
}

void LayoutWidget::enableFilter(bool enable)
{
    ui->enable->setChecked(enable);
}

void LayoutWidget::setBackgroundColor(QColor color)
{
    ui->view->setBackgroundBrush(QBrush(color));
}

void LayoutWidget::on_preview_toggled(bool checked)
{
    if (checked)
        ui->view->setPixmap(previewPixmap);
    else
        ui->view->setPixmap(inputPixmap);
}

// When a parameter is changed, the input and resulting Image Sizes are recalculated with this function.
void LayoutWidget::updateFormSizes()
{
    QString newPageWidth, newPageHeight;
    newPageWidth = QString::number(pxPageWidth * factorPixeltoDisplayUnit, 'f', 3);
    newPageHeight = QString::number(pxPageHeight * factorPixeltoDisplayUnit, 'f', 3);

    ui->pageWidth->setText(newPageWidth);
    ui->pageHeight->setText(newPageHeight);
    ui->pixelPageWidth->setText(QString::number(pxPageWidth, 'f', 3));
    ui->pixelPageHeight->setText(QString::number(pxPageHeight, 'f', 3));
    ui->inchPageWidth->setText(QString::number(pxPageWidth / dpi, 'f', 3));
    ui->inchPageHeight->setText(QString::number(pxPageHeight / dpi, 'f', 3));
    ui->millimeterPageWidth->setText(
            QString::number(pxPageWidth / dpi * Constants::milimeterPerInch, 'f', 3));
    ui->millimeterPageHeight->setText(
                QString::number(pxPageHeight / dpi * Constants::milimeterPerInch, 'f', 3));
}

void LayoutWidget::setDPI(int newDpi)
{
    if (newDpi < Constants::MIN_DPI) // we only accept positive DPI
        return;

    if (dpi == newDpi) // do nothing when nothing changes.
        return;

    dpi = newDpi;

    switch (Constants::displayUnits.indexOf(displayUnit)) {
        case 0:     // pixel
            factorPixeltoDisplayUnit = 1;
            break;
        case 1:     // millimeter
            factorPixeltoDisplayUnit = Constants::milimeterPerInch / dpi;
            break;
        case 2:     // inches
            factorPixeltoDisplayUnit = 1 / dpi;
            break;
        // default:
        // Default will not happen, we check the unit before setting displayUnit
    }

    updateFormSizes();
    emit parameterChanged();
}

void LayoutWidget::on_pageWidth_editingFinished()
{
    pxPageWidth = ui->pageWidth->text().toDouble() / factorPixeltoDisplayUnit;

    updateFormSizes();
    emit parameterChanged();
}

void LayoutWidget::on_pageHeight_editingFinished()
{
    pxPageHeight = ui->pageHeight->text().toDouble() / factorPixeltoDisplayUnit;

    updateFormSizes();
    emit parameterChanged();
}

// TODO: minimal width / height ist max(0.001, width of input image)
void LayoutWidget::on_pageWidth_textEdited(const QString &strValue)
{
    qreal width = strValue.toDouble();

    if (width <= 0.001) {
        ui->pageWidth->setStyleSheet("background-color: rgb(255, 0, 0);");
    } else {
        ui->pageWidth->setStyleSheet("");
    }
}

void LayoutWidget::on_pageHeight_textEdited(const QString &strValue)
{
    qreal height = strValue.toDouble();

    if (height <= 0.001) {
        ui->pageHeight->setStyleSheet("background-color: rgb(255, 0, 0);");
    } else {
        ui->pageHeight->setStyleSheet("");
    }}

void LayoutWidget::on_horizontalAlignment_currentIndexChanged(int /*index*/)
{
    emit parameterChanged();
}

void LayoutWidget::on_verticalAlignment_currentIndexChanged(int /*index*/)
{
    emit parameterChanged();
}

void LayoutWidget::on_enable_toggled(bool checked)
{
    emit enableFilterToggled(checked);
}
