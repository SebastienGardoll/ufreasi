/** 
 *  user-FRiendly Elemental dAta procesSIng (uFREASI)
 *  
 *  Copyright © 2012 Oualid Khelefi.
 *
 *  Authors : see AUTHORS.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PARSER_H
#define PARSER_H

#include "data.h"
#include <QtGui>
#include <QtCore>
#include <iostream>

using namespace std;

class Data;

class Parser{
public:
    virtual QPair<int,QString> parse(QFile *file,class Data *data,class Processing *process) = 0; ///< Parsing Function
};

/**
  * Parser for the input CSV data file, raw data comming from the ICP-MS
  */

class ParserInCSV: public Parser{
public:
    QPair<int,QString> parse(QFile *file,Data *data,class Processing *process); ///< Parsing Function
};

/**
  * Parser for the output CSV data file, processed results data comming from the application and stored on a file
  */

class ParserOutCSV: public Parser{
public:
    QPair<int, QString> parse(QFile *file,Data *data,class Processing *process); ///< Parsing Function
};

/**
  * Parser for the input CSV concentrations file, it contains known concentrations of standards and quality controls
  */

class ParserInSTDQC: public Parser{
public:
    QPair<int, QString> parse(QFile *file,Data *data,class Processing *process); ///< Parsing Function
};

#endif // PARSER_H
