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

#include "element.h"

/**
* The constructor initializes the name of the element, the mass and the resolution
* \param element Name of the element
* \param mass The mass of the element
* \param resol The resolution of the element (High, Medium or Low resoltion - HR, MR, LR)
*/

Element::Element(QString element,int mass,Resolution resol){
    this->element = element;
    this->mass = mass;
    this->resol = resol;
}

/**
* \return The name of the element
*/

QString Element::getElement(){
    return element;
}

/**
* \return The mass of the element
*/

int Element::getMasse(){
    return mass;
}

/**
* \return The resolution of the element with Resolution enumeration value (HR,MR,LR)
*/

Element::Resolution Element::getResol(){
    return resol;
}

/**
* \return The resolution of the element with resolution's String definition ("HR","MR","LR")
*/

QString Element::getResolName(){
    switch(resol){
        case Element::LR : return("LR"); break;
        case Element::MR : return("MR"); break;
        case Element::HR : return("HR"); break;
        default : return(""); break;
    }
}

/**
* \return The complete name of the istope : "mass name resolution"; example : "7Li LR"
*/

QString Element::getName(){
    return(QString::number(mass) + element + " " + getResolName());
}
