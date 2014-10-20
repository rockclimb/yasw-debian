/*
 * Copyright (C) 2014 Robert Chéramy (robert@cheramy.net)
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
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QStringList>


static const char VERSION[] = "git master for 0.6";


class Constants
{
public:
    static QStringList displayUnits;
    static QStringList dpiList;
    static qreal const milimeterPerInch ; //= 25.4;

    static int const MIN_DPI = 10;
    static int const DEFAULT_DPI = 300;
};



#endif // CONSTANTS_H
