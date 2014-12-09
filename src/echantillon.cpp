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

#include "echantillon.h"

Echantillon::Echantillon(QString name,int nbrElement,type typeEchantillon){
    this->name = name;
    this->nbrElement = nbrElement;
    this->typeEchantillon = typeEchantillon;

    coups = new QList<double>;
    coupsSD = new QList<double>;
    concentration = new QList<double>;
    concentrationSD = new QList<double>;

    for (int i=0;i<nbrElement;i++){
        coups->append(0);
        coupsSD->append(0);
        concentration->append(0);
        concentrationSD->append(0);
    }
}

/*******************************
** Get Solution Name
*******************************/

QString Echantillon::getName(){
    return(name);
}

/*******************************
** Get Element Number
*******************************/

int Echantillon::getNbrElement(){
    return(nbrElement);
}

/***********************************************************
** Get the Type of the Solution : Echant / QC / STD / BLANC
************************************************************/

Echantillon::type Echantillon::getType(){
    return typeEchantillon;
}

/***********************************************************
** Get Intensity of Element idetified by Id on the Solution
************************************************************/

double Echantillon::getCoups(int Id){
    return coups->at(Id);
}

/****************************************************************
** Get Intensity Error of Element idetified by Id on the Solution
****************************************************************/

double Echantillon::getCoupsSD(int Id){
    return coupsSD->at(Id);
}

/***********************************************************
** Set Intensity of Element idetified by Id on the Solution
************************************************************/

void Echantillon::setCoups(int Id,double coupsVal){
    coups->replace(Id,coupsVal);
}

/****************************************************************
** Set Intensity Error of Element idetified by Id on the Solution
****************************************************************/

void Echantillon::setCoupsSD(int Id,double coupsSDVal){
    coupsSD->replace(Id,coupsSDVal);
}

/****************************************************************
** Set Concetration of Element idetified by Id on the Solution
****************************************************************/

void Echantillon::setConcent(int Id,double concentrationVal){
    concentration->replace(Id,concentrationVal);
}

/******************************************************************
** Set Concetration Error of Element idetified by Id on the Solution
******************************************************************/

void Echantillon::setConcentSD(int Id,double concentrationSDVal){
    concentrationSD->replace(Id,concentrationSDVal);
}

/****************************************************************
** Get Concetration of Element idetified by Id on the Solution
****************************************************************/

double Echantillon::getConcent(int Id){
    return concentration->at(Id);
}

/******************************************************************
** Get Concetration Error of Element idetified by Id on the Solution
******************************************************************/

double Echantillon::getConcentSD(int Id){
    return concentrationSD->at(Id);
}

