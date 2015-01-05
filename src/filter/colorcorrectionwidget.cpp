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
#include "colorcorrectionwidget.h"
#include "ui_colorcorrectionwidget.h"

#include <QDebug>
#include <QColorDialog>

ColorCorrectionWidget::ColorCorrectionWidget(QWidget *parent) :
    AbstractFilterWidget(parent),
    ui(new Ui::ColorCorrectionWidget)
{
    ui->setupUi(this);

    connect(ui->view, SIGNAL(pixmapClicked(QColor)),
            this, SLOT(imageClicked(QColor)));
}

ColorCorrectionWidget::~ColorCorrectionWidget()
{
    delete ui;
    delete intValidator;
}


void ColorCorrectionWidget::setPixmap(QPixmap pixmap)
{
    inputPixmap = pixmap;
    ui->view->setPixmap(pixmap);
}

void ColorCorrectionWidget::setPreview(QPixmap pixmap)
{
    previewPixmap = pixmap;
    ui->previewImage->setPixmap(pixmap.scaledToWidth(ui->previewImage->width()));
}

bool ColorCorrectionWidget::preview()
{
    return ui->preview->isChecked();
}

QColor ColorCorrectionWidget::whitePoint()
{
    return whitepoint;
}

QColor ColorCorrectionWidget::blackPoint()
{
    return blackpoint;
}

void ColorCorrectionWidget::setWhitePoint(QColor white)
{
    whitepoint = white;
    QString whiteName = whitepoint.name();
    ui->whitepoint->setText(whiteName);
    ui->whitepoint->setStyleSheet("background-color: " + whiteName + ";");
    emit parameterChanged();
}

void ColorCorrectionWidget::setBlackPoint(QColor black)
{
    blackpoint = black;
    QString blackName = blackpoint.name();
    ui->blackpoint->setText(blackName);
    ui->blackpoint->setStyleSheet("background-color: " + blackName + "; color: rgb(255, 255, 255);");
    emit parameterChanged();
}

void ColorCorrectionWidget::enableFilter(bool enable)
{
    ui->enable->setChecked(enable);
}

void ColorCorrectionWidget::on_preview_toggled(bool checked)
{
    if (checked) {
        // This does recalculate the output image if necessary and sets the preview Image.
        emit previewChecked();
    } else {
        ui->view->setPixmap(inputPixmap);
    }
}

// This slot is called every time the image in ui-view is clicked. React only when curently picking a color.
void ColorCorrectionWidget::imageClicked(QColor color)
{
    if (pickingWhiteColor) {
        setWhitePoint(color);
        // Resets state of picking Color. This call to the slot is to avoid duplicate code
        on_pickWhitepoint_clicked();
    }

    if (pickingBlackColor) {
        setBlackPoint(color);
        // Resets state of picking Color. This call to the slot is to avoid duplicate code
        on_pickBlackpoint_clicked();
    }
}

void ColorCorrectionWidget::setBackgroundColor(QColor color)
{
    ui->view->setBackgroundBrush(QBrush(color));
}

void ColorCorrectionWidget::on_enable_toggled(bool checked)
{
    emit enableFilterToggled(checked);
}

void ColorCorrectionWidget::on_whitepoint_clicked()
{
    QColor color = QColorDialog::getColor(whitepoint, this, tr("Choose whitepoint"));
    if (color.isValid()) {
        setWhitePoint(color);
    }
}

void ColorCorrectionWidget::on_blackpoint_clicked()
{
    QColor color = QColorDialog::getColor(blackpoint, this, tr("Choose blackpoint"));
    if (color.isValid()) {
        setBlackPoint(color);
    }
}

void ColorCorrectionWidget::on_pickWhitepoint_clicked()
{
    pickingWhiteColor = ! pickingWhiteColor;

    // Disable any action with black point.
    ui->pickBlackpoint->setText(tr("Pick"));
    pickingBlackColor= false;

    // Set a CrossCursor over the view when picking.
    ui->view->pickingColor(pickingWhiteColor);
    if (pickingWhiteColor) {
        ui->pickWhitepoint->setText(tr("Picking..."));
    } else {
        ui->pickWhitepoint->setText(tr("Pick"));
    }
}

void ColorCorrectionWidget::on_pickBlackpoint_clicked()
{
    pickingBlackColor = ! pickingBlackColor;

    // Disable any action with black point.
    ui->pickWhitepoint->setText(tr("Pick"));
    pickingWhiteColor= false;

    ui->view->pickingColor(pickingBlackColor);
    if (pickingBlackColor) {
        ui->pickBlackpoint->setText(tr("Picking..."));
    } else {
        ui->pickBlackpoint->setText(tr("Pick"));
    }
}
