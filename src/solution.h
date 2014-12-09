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

#ifndef TEST_H
#define TEST_H

#include <QtCore>
#include <QtGui>

class Solution
{
public:
    /// Enumeration of different types of solutions
    enum Type{SMP   ///< Basic solution type (no particular type)
              ,STD  ///< Standard type
              ,BLK  ///< Blank type
              ,QC   ///< Quality Control type
             };

    Solution(QString name,int nbrElement,Type typeSolution);   ///< Solution Class Constructor

    Solution copy(QString copyName, Type copyTypeSolution) ; ///< Create a copy of this.
    
    QString getName();  ///< Gets the Name
    int getNbrElement();  ///< Gets the Number of Elements
    Type getType();  ///< Gets type of the solution

    double getCps(int Id);  ///< Gets Intensity
    double getCpsSD(int Id);  ///< Gets Intensity Standard Deviation
    double getCpsRSD(int Id); ///< Gets Intensity Relative Standard Deviation (%) 
    double getConcent(int Id);  ///< Gets Concentration
    double getConcentRSD(int Id);    ///< Gets Concentration Relative Standard Deviation (%)
    
    void setCps(int Id,double cpsVal);  ///< Sets Intensity
    void setCpsSD(int Id,double cpsSDVal);  ///< Sets Intensity Standard Deviation
    void setConcent(int Id,double concentVal);  ///< Sets Concentration
    void setConcentRSD(int Id,double concentRSDVal);  ///< Sets Concentration Relative Standard Deviation (%)
    
    static QString ZERO_STD_NAME ;

private:
    QList<double> * cps;    ///< Ordered List of the Intensities on the solution, of each element
    QList<double> * cpsSD;    ///< Ordered List of the Intensity Standard Deviations on the solution, of each element
    QList<double> * concent;        ///< Ordered List of the Concentrations on the solution, of each element
    QList<double> * concentRSD;      ///< Ordered List of the Concentration Relative Standard Deviation (%) on the solution, of each element
    QString name;       ///< Name of the solution
    int nbrElement;     ///< Number of Elements on the solution
    Type typeSolution;   ///< Type of solution : Basic, STD, BLK or QC
};

#endif // TEST_H
