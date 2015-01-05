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
#include "thresholdwidget.h"
#include "ui_thresholdwidget.h"
#include "constants.h"


ThresholdWidget::ThresholdWidget(QWidget *parent) :
    AbstractFilterWidget(parent),
    ui(new Ui::ThresholdWidget)
{
    ui->setupUi(this);
}

ThresholdWidget::~ThresholdWidget()
{
    delete ui;
    delete intValidator;
}


void ThresholdWidget::setPixmap(QPixmap pixmap)
{
    inputPixmap = pixmap;
    ui->view->setPixmap(pixmap);
}

void ThresholdWidget::setPreview(QPixmap pixmap)
{
    previewPixmap = pixmap;
    if (preview())
        ui->view->setPixmap(pixmap);
}

bool ThresholdWidget::preview()
{
    return ui->preview->isChecked();
}

void ThresholdWidget::enableFilter(bool enable)
{
    ui->enable->setChecked(enable);
}

bool ThresholdWidget::preprocessNoise()
{
    return ui->preprocessNoise->isChecked();
}

bool ThresholdWidget::rbThreshold()
{
    return ui->rbThreshold->isChecked();
}

int ThresholdWidget::threshold()
{
    return ui->threshold->text().toInt();
}

bool ThresholdWidget::rbAdaptativeThreshold()
{
    return ui->rbAdaptativeThreshold->isChecked();
}

double ThresholdWidget::cvalue()
{
    return ui->cval->text().toDouble();
}

int ThresholdWidget::blockSize()
{
    int blocksize = ui->blocksize->text().toInt();
    blocksize = qMax(blocksize, 1);
    blocksize = blocksize - (blocksize % 2) + 1;
    return blocksize;
}

bool ThresholdWidget::rbOtsuThreshold()
{
    return ui->rbOtsuThreshold->isChecked();
}

void ThresholdWidget::setOtsuThreshold(double threshold)
{
    ui->otsuThreshold->setText(Constants::float2String(threshold));
}

void ThresholdWidget::on_preview_toggled(bool checked)
{
    if (checked) {
        // This does recalculate the output image if necessary and sets the preview Image.
        emit previewChecked();
    } else {
        ui->view->setPixmap(inputPixmap);
    }
}

void ThresholdWidget::setBackgroundColor(QColor color)
{
    ui->view->setBackgroundBrush(QBrush(color));
}

void ThresholdWidget::on_enable_toggled(bool checked)
{
    emit enableFilterToggled(checked);
}

void ThresholdWidget::paramChanged()
{
    emit parameterChanged();
}
