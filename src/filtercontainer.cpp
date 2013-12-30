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
#include "filtercontainer.h"
#include <QDebug>
#include "rotation.h"
#include "dekeystoning.h"
#include "cropping.h"
#include "scaling.h"
#include <QPrinter>

/** \class FilterContainer
    \brief A customised QTabWidget to display the different filters.

    FilterContainer initialises each Filter.

    As QTabWidget::currentWidget only returns a QWidget and we need a FilterWidget and its virtual functions,
    we keep a Map between tabs indexes and FilterWidgets in tabToWidget.
    */


FilterContainer::FilterContainer( QWidget * parent)
    : QTabWidget(parent)
{
    // initialise the filters
    Rotation *rotationFilter = new Rotation(this);
    tabToFilter.append(rotationFilter);
    addTab(rotationFilter->getWidget(), rotationFilter->getName());

    Dekeystoning *dekeystoningFilter = new Dekeystoning(this);
    tabToFilter.append(dekeystoningFilter);
    addTab(dekeystoningFilter->getWidget(), dekeystoningFilter->getName());
    /* connect the filter to previous filter so it gets changes automaticaly */
    connect(rotationFilter, SIGNAL(parameterChanged()),
            dekeystoningFilter, SLOT(inputImageChanged()));
    dekeystoningFilter->setPreviousFilter(rotationFilter);

    Cropping *croppingFilter = new Cropping(this);
    tabToFilter.append(croppingFilter);
    addTab(croppingFilter->getWidget(), croppingFilter->getName());
    /* connect the filter to previous filter so it gets changes automaticaly */
    connect(dekeystoningFilter, SIGNAL(parameterChanged()),
            croppingFilter, SLOT(inputImageChanged()));
    croppingFilter->setPreviousFilter(dekeystoningFilter);

    scalingFilter = new Scaling(this);
    tabToFilter.append(scalingFilter);
    addTab(scalingFilter->getWidget(), scalingFilter->getName());
    /* connect the filter to previous filter so it gets changes automaticaly */
    scalingFilter->setPreviousFilter(croppingFilter);
    connect(croppingFilter, SIGNAL(parameterChanged()),
            scalingFilter, SLOT(inputImageChanged()));
    connect(this, SIGNAL(currentChanged(int)),
            this, SLOT(tabChanged(int)));
}

FilterContainer::~FilterContainer()
{
    int index;
    for (index = 0; index < tabToFilter.size(); index++) {
        delete tabToFilter[index];
    }
    tabToFilter.clear();
}

/* Sets the image to be worked on. */
void FilterContainer::setImage(QPixmap pixmap)
{
    tabToFilter[0]->setImage(pixmap);

    int currentTab = std::min (tabToFilter.size(), currentIndex());
    tabToFilter[currentTab]->refresh();
}

void FilterContainer::setSelectionColor(QColor color)
{
    emit(selectionColorChanged(color));
}

void FilterContainer::setBackgroundColor(QColor color)
{
    emit(backgroundColorChanged(color));
}

void FilterContainer::tabChanged(int index)
{
    int currentTab = std::min (tabToFilter.size(), currentIndex());
    tabToFilter[currentTab]->refresh();
    emit filterChanged(tabToFilter[oldIndex]->getIdentifier());
    oldIndex = index;
}

/*! \brief Get settings from the filters.

  Each Filter.getSettings will return its settings in a QMap<QString, QVariant>, which will be packed in another
  QMap (its type is QMap<QString, QMap<QString, QVariant> > ). The String Key is the value of Filter.getName().
  */
QMap<QString, QVariant> FilterContainer::getSettings()
{
    /* QVariant here is a QMap<QString, QVariant> */
    QMap<QString, QVariant> allSettings;
    BaseFilter *filter;


    foreach (filter, tabToFilter) {
        allSettings[filter->getIdentifier()] = filter->getSettings();
    }

    return allSettings;
}

/*! \brief Set settings to the filters.

    See also FilterContainer::getSettings
  */
void FilterContainer::setSettings(QMap<QString, QVariant> settings)
{
    QString filterName;
    BaseFilter *filter;
    bool filterSet;

    foreach (filter, tabToFilter) {
        filterSet = false;
        foreach (filterName, settings.keys()) {
            if (filterName == filter->getIdentifier()) {
                filter->setSettings(settings[filterName].toMap());
                filterSet = true;
            }
        }
        if (filterSet == false) {
            /* we did not found settings for this filter, reset settings */
            filter->setSettings(QMap<QString, QVariant>());
        }
    }
}

/** \brief Compute and return the resulting image above all filter
 */
QPixmap FilterContainer::getResultImage()
{
    int maxTab = tabToFilter.size() - 1;

    return tabToFilter[maxTab]->getOutputImage();
}

/** \brief returns the Size of the current image.

    \returns QMap<QString, QVariant>; keys are size (QSize), and
        unit (enum QPrinter::Unit)
*/
QMap<QString, QVariant> FilterContainer::getPageSize()
{
    QMap<QString, QVariant> imageSize;
    qreal width, height;

    width = scalingFilter->pageMilimeterWidth();
    height = scalingFilter->pageMilimeterHeight();

    imageSize["unit"] = QPrinter::Millimeter;
    imageSize["size"] = QSize(width, height);

    return imageSize;
}

/** \brief Returns the identifiert of the current filter */
QString FilterContainer::currentFilter()
{
    return tabToFilter[currentIndex()]->getIdentifier();
}

