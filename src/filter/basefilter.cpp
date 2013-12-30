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
#include "basefilter.h"
#include <QDebug>

/*! \class BaseFilter

  \brief The BaseFilter class is the model for all Filter applied on pages.

  It should be inherited by every Filter. The PageFilter class is functional but does nothing, and
  returns the pixmap unmodified.

  Some functions are definied as virtual in order to allow the calling class use a unique interface
  (all Filter can be stored as a "BaseFilter" Class).
*/

/*! \brief  Constructs a Filter
*/
BaseFilter::BaseFilter(QObject * parent) : QObject(parent)
{
    widget = new BaseFilterWidget();
    filterWidget = widget;
}

BaseFilter::~BaseFilter()
{
    delete widget;
}


/*! \brief Set input Page

  This function is called by the calling class to set the Pixmap to be "filtered".
*/
void BaseFilter::setImage(QPixmap pixmap)
{
    inputPixmap = pixmap;
    emit parameterChanged();
    filterWidget->setPixmap(pixmap);
    mustRecalculate = true;
}

/*! \brief Returns the transformed image

  @returns The transformed page, or a null Pixmap if no page is available
*/
QPixmap BaseFilter::getOutputImage()
{
    refresh();
    return outputPixmap;
}

/*! \brief Gets the widget to display the filter

    The returned widget must not be freed, it is handled by the class destructor.
    @returns The Widget for this Filter.
  */
AbstractFilterWidget* BaseFilter::getWidget()
{
    return filterWidget;
}

/** \brief Returns a universal name for this filter.

 This identifier is unique for the filter. It can be used to identify the
 filter used (for example in configuration files)
 */
QString BaseFilter::getIdentifier()
{
    return QString("BaseFilter");
}

/*! \brief Retruns the name of the filter */
QString BaseFilter::getName()
{
    return tr("Base Filer");
}



void BaseFilter::inputImageChanged()
{
    reloadInputImage = true;
    // Tell folowing filter that my parameter changed.
    emit parameterChanged();
}

void BaseFilter::widgetParameterChanged()
{
    emit parameterChanged();
    mustRecalculate = true;
    // Only refresh the output image if preview is active
    if (filterWidget->preview()) {
        refresh();
    }
}

/*! \brief virtual function to get the Filter settings

    @returns A QMap of keys (QString) to settings (QVariant).
*/
QMap<QString, QVariant> BaseFilter::getSettings()
{
    /* As BaseFilter does nothing, there is nothing to save */
    return QMap<QString, QVariant>();
}

/** \brief virtual function do set the Filter settings.

  If a needed setting is not available in settings, a default has to be set.
*/
void BaseFilter::setSettings(QMap<QString, QVariant> /* settings */)
{
//    loadingSettings = true;
    /* Ignore settings, as there is nothing to set */
//    loadingSettings = false;
}

void BaseFilter::setPreviousFilter(BaseFilter *filter)
{
    previousFilter = filter;
    /* Change of previous Filter = change of external Parameter*/
    inputImageChanged();
}

void BaseFilter::refresh()
{
    qDebug() << "refresh";
    if (loadingSettings)
        return;

    if (reloadInputImage && previousFilter) {
        setImage(previousFilter->getOutputImage());
        reloadInputImage = false;
        mustRecalculate = true;
    }
    if (mustRecalculate) {
        compute();
        mustRecalculate = false;
        filterWidget->setPreview(outputPixmap);
    }
}


//// Checks if the image is to be recalculated and if so call compute()
//void BaseFilter::recompute()
//{
//    if (reloadInputImage && previousFilter) {
//        setImage(previousFilter->getOutputImage());
//        reloadInputImage = false;
//        mustRecalculate = true;
//    }
//    if (mustRecalculate) {
//        compute();
//        mustRecalculate = false;
//    }
//}

// Do compute the outputPixmap with the help of all available parameters.
void BaseFilter::compute()
{
    outputPixmap = inputPixmap;
}

