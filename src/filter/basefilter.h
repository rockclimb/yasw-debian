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
#ifndef BASEFILTER_H
#define BASEFILTER_H

#include <QPixmap>
#include <QWidget>
#include <QObject>
#include <QMap>
#include <QVariant>
#include <QString>
#include "basefilterwidget.h"
#include <QImage>


class BaseFilter : public QObject
{
    Q_OBJECT
public:
    BaseFilter(QObject * parent = 0);
    ~BaseFilter();
    void setImage(const QPixmap pixmap);
    virtual QPixmap getOutputImage();

    AbstractFilterWidget* getWidget();
    virtual QString getIdentifier();
    virtual QString getName();

    virtual QMap<QString, QVariant> getSettings();
    virtual void setSettings(QMap <QString, QVariant> settings);

    void setPreviousFilter(BaseFilter *filter);
    void refresh();

public slots:
    /* get the information from external classes that an external parameter changed.
     * Next time getFilteredImage() is called, must reload the inputPixmap. */
    void inputImageChanged();
    /* Parameter for the Filter changed through user intercaction */
    void widgetParameterChanged();
signals:
    /* Yell that my parameter (this includes input image) changed and that one need to reload my FilteredImage */
    void parameterChanged();

protected:
    QPixmap inputPixmap;
    QPixmap outputPixmap;
    AbstractFilterWidget *filterWidget = NULL;
    /* Store the information that the input image has to be reloaded before producing the output image */
    bool reloadInputImage = false;
    /* Only recalculate when mustRecalculate == true */
    bool mustRecalculate = false;
    /* Link to previous Filter which can delivery a new inputImage */
    BaseFilter *previousFilter = NULL;
    // when true, do not recalculate (this is not an user interaction)
    bool loadingSettings = false;
    virtual void compute();
    virtual QImage filter(QImage inputImage);

private:
    BaseFilterWidget* widget;


};

#endif // BASEFILTER_H
