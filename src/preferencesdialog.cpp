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
#include <QColorDialog>
#include <QColor>
#include <QPalette>
#include <QDebug>

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "constants.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    setSelectionColor(Qt::red);
    setBackgroundColor(Qt::lightGray);

    ui->unit->insertItems(0, Constants::displayUnits);
    setDisplayUnit("pixel");
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::setSettings(QSettings *newSettings)
{
    QColor color;
    settings = newSettings;

    color = QColor(settings->value("selectionColor").toString());
    if (!color.isValid()) {
        color = selectionColor;
    }
    setSelectionColor(color);

    color = QColor(settings->value("backgroundColor").toString());
    if (!color.isValid()) {
        color = backgroundColor;
    }
    setBackgroundColor(color);

    QString unit = settings->value("displayUnit").toString();
    setDisplayUnit(unit);
}

void PreferencesDialog::on_selectionColorButton_clicked()
{
    QColor color;

    color = QColorDialog::getColor(selectionColor, this, tr("Choose a color for visual selection"));
    setSelectionColor(color);
}

/** \brief Sets the Color for grafical selection feedback (example deykeystoning quadrilateral)

  This function emits a selectionColorChanged signal, so that the filtercontainer can
  transmit the information to the plugings, who should change the color used.
*/
void PreferencesDialog::setSelectionColor(QColor color)
{
    QPalette palette;

    if (!color.isValid()) {
        return;
    }

    selectionColor = color;
    palette.setColor(QPalette::Button, color);
    ui->selectionColorButton->setPalette(palette);

    if (settings) {
        settings->setValue("selectionColor", color.name());
    }

    emit(selectionColorChanged(color));
}

void PreferencesDialog::setBackgroundColor(QColor color)
{
    QPalette palette;

    if (!color.isValid()) {
        return;
    }

    backgroundColor = color;
    palette.setColor(QPalette::Button, color);
    ui->backgroundColorButton->setPalette(palette);

    if (settings) {
        settings->setValue("backgroundColor", color.name());
    }

    emit(backgroundColorChanged(color));
}

void PreferencesDialog::setDisplayUnit(QString unit)
{
    if (!Constants::displayUnits.contains(unit)) // unit unknown? Do nothing.
        return;

    ui->unit->setCurrentIndex(Constants::displayUnits.indexOf(unit));

    if (settings) {
        settings->setValue("displayUnit", unit);
    }
}

void PreferencesDialog::on_backgroundColorButton_clicked()
{
    QColor color;

    color = QColorDialog::getColor(backgroundColor, this, tr("Choose a background color"));
    setBackgroundColor(color);
}

void PreferencesDialog::on_unit_currentIndexChanged(const QString &defaultUnit)
{
    if (settings) {
        settings->setValue("displayUnit", defaultUnit);
    }
}
