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

#include "solution.h"


QString Solution::ZERO_STD_NAME = "STD_ZERO_STD" ;

/**
* The constructor creates the lists of values corresponding to nbrElement (cps,cpsSD,concent,concentSD)
* and intializes them to 0

* It initializes the name, the number of Elements and the solution's type
* \param name Name of the solution
* \param nbrElement Number of Elements on the solution
* \param typeSolution Type of the solution (Basic, STD, BLK, QC)
*/

Solution::Solution(QString name,int nbrElement,Type typeSolution){
    this->name = name;
    this->nbrElement = nbrElement;
    this->typeSolution = typeSolution;

    cps = new QList<double>;
    cpsSD = new QList<double>;
    concent = new QList<double>;
    concentRSD = new QList<double>;

    for (int i=0;i<nbrElement;i++){
        cps->append(0.);
        cpsSD->append(0.);
        concent->append(0.);
        concentRSD->append(0.);
    }
}

/**
* \return The name of the solution
*/

QString Solution::getName(){
    return(name);
}

/**
* \return The Number of Elements on the solution
*/

int Solution::getNbrElement(){
    return(nbrElement);
}

/**
* \return The type of the solution
*/

Solution::Type Solution::getType(){
    return typeSolution;
}

/**
* \param Id Identifier of the Element on the ordered list of the Elements
* \param cpsVal Set Intensity of the Element on the present solution
*/

void Solution::setCps(int Id,double cpsVal){
    cps->replace(Id,cpsVal);
}

/**
* \param Id Identifier of the Element on the ordered list of the Elements
* \param cpsSDVal Set Intensity Standard Deviation value of the Element on the present solution
*/

void Solution::setCpsSD(int Id,double cpsSDVal){
    cpsSD->replace(Id,cpsSDVal);
}

/**
* \param Id Identifier of the Element on the ordered list of the Elements
* \param concentVal Set the concentration of the Element on the present solution
*/

void Solution::setConcent(int Id,double concentVal){
    concent->replace(Id,concentVal);
}

/**
* \param Id Identifier of the Element on the ordered list of the Elements
* \param concentRSDVal Set the Concentration Relative Standard Deviation (%) value of the Element on the present solution
*/

void Solution::setConcentRSD(int Id,double concentRSDVal){
    concentRSD->replace(Id,concentRSDVal);
}

/**
* \param Id Identifier of the Element on the ordered list of the Elements
* \return The Intensity of the Element identified by Id parameter
*/

double Solution::getCps(int Id){
    return cps->at(Id);
}

/**
* \param Id Identifier of the Element on the ordered list of the Elements
* \return The Intensity Standard Deviation of the Element identified by Id parameter
*/

double Solution::getCpsSD(int Id){
    return cpsSD->at(Id);
}

/**
* \param Id Identifier of the Element on the ordered list of the Elements
* \return The Relative Intensity Standard Deviation (%) of the Element identified by Id parameter
*/
double Solution::getCpsRSD(int Id)
{
   return (this->getCpsSD(Id) * 100. / this->getCps(Id)) ;
}

/**
* \param Id Identifier of the Element on the ordered list of the Elements
* \return The Concentration of the Element identified by Id parameter
*/

double Solution::getConcent(int Id){
    return concent->at(Id);
}

/**
* \param Id Identifier of the Element on the ordered list of the Elements
* \return The Concentration Relative Standard Deviation of the Element identified by Id parameter
*/

double Solution::getConcentRSD(int Id){
    return concentRSD->at(Id);
}

/**
 * \param copyName the copy's name
 * \param copyTypeSolution the copy's type
 * \return a copy of this
 */
Solution Solution::copy(QString copyName, Type copyTypeSolution)
{
   Solution result (*this) ;
   
   result.name = copyName ;
   result.typeSolution = copyTypeSolution ;
   
   return result ;
}