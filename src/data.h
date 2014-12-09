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

#ifndef DATA_H
#define DATA_H

#include <QtGui>
#include <QtCore>
#include "element.h"
#include "solution.h"
#include "displaying.h"
#include "parser.h"

class Parser;

class Data
{
public:
    Data(Parser *parser); ///< Constructor of Data class
    void loadParser(Parser * parser); ///< Loads a specific parser used to proceed the Data

    int addIso(Element element); ///< Adds an element to the data set
    Element getIso(int); ///< Gets an element identified by its Id
    int isoSize(); ///< Gets the Number of elements

    int addSolution(Solution solution); ///< Add a solution to the data set
    Solution& getSolution(int); ///< Gets a solution identified by its Id
    int containsSolution(QString solutionName) ; ///< Return the id of first occurence of a solution with the given name or -1.
    int solutionSize(); ///< Gets the Number of Solutions

    int mapBLK(int Id);   ///< Gets the mapping from BLK list to Solutions list
    int mapLR(int Id);    ///< Gets the mapping from LR elements list to all elements list
    int mapMR(int Id);    ///< Gets the mapping from MR elements list to all elements list
    int mapHR(int Id);    ///< Gets the mapping from HR elements list to all elements list

    void addBLKmoy(QList<int> idBlk,QString Name); ///< Adds a calculated Blank Solution

    void copyData(Data * data); ///< Copies data contain from the given parameter
    void clear();   ///< Clears data contain

    QPair<int, QString> executeParsing(QFile *file,class Processing *process);
    
    static double ZERO ;

private:
    QList<Element> elementList;   ///< List of elements
    QList<Solution> solutionList;   ///< List of solutions

    QList<int> mapBlk;     ///< Mapping list from BLK list to Solutions list
    QList<int> mapIsoLR;    ///< Mapping list from LR elements list to all elements list
    QList<int> mapIsoMR;    ///< Mapping list from MR elements list to all elements list
    QList<int> mapIsoHR;    ///< Mapping list from HR elements list to all elements list

    Parser *parser; ///< Loaded parser used to process the instance of Data

};
#endif // DATA_H
