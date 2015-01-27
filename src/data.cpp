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

#include "data.h"

double Data::ZERO = 0.00000000000001 ;
const QString Data::ALL_AVG = "BLK_Avg_All" ;
const QString Data::FIRST_SEQ_AVG = "BLK_Avg_First_Seq" ;

/**
  * \param  parser Pointer to default parser
  */

Data::Data(Parser * parser){
    this->parser = parser;
}

/**
  * \param  parser Pointer to the parser to load
  */

void Data::loadParser(Parser * parser){
    this->parser = parser;
}

/**
  * This function fills elements list and mapping LR,MR,HR elements lists
  * \param element The element to add to tha data set
  * \return The Id given to the added element
  */

int Data::addIso(Element element){
    elementList.append(element);
    int current = elementList.size()-1;

    switch(element.getResol()){
        case Element::LR : mapIsoLR << current; break;
        case Element::MR : mapIsoMR << current; break;
        case Element::HR : mapIsoHR << current; break;
        case Element::UNDEFINED : mapIsoLR << current; break;
    }

    return(current);
}

/**
  * \param  Id Identifier of the element
  * \return The element given by the choosen Id on the ordered solutions list
  */

Element Data::getIso(int Id){
    return(elementList.at(Id));
}

/**
  * \return The number of elements on the ordered elements list
  */

int Data::isoSize(){
    return(elementList.size());
}

/**
  * This function fills solutions list and mapping Blank solutions list
  * \param  solution The solution to add to tha data set
  * \return The Id given to the added solution
  */

int Data::addSolution(Solution solution){
    solutionList.append(solution);
    int current = solutionList.size()-1;

    if(solution.getType() == Solution::BLK){
        mapBlk << current;
    }

    return(current);
}

/**
  * \param  Id Identifier of the solution
  * \return The solution given by the choosen Id on the ordered solutions list
  */

Solution& Data::getSolution(int Id){
    return(solutionList[Id]);
}

/**
  * \return The number of solutions on the ordered solutions list
  */

int Data::solutionSize(){
    return(solutionList.size());
}

/**
  * \param  Id Identifier of the solution on the Blank Solutions list
  * \return The Mapped identifier of the Blank solution on the list of all solutions
  */

int Data::mapBLK(int Id){
    if(Id == -1) return Id;
    return(mapBlk.at(Id));
}

/**
  * \param  Id Identifier of the element on the LR elements list
  * \return The Mapped identifier of the element on the list of all elements
  */

int Data::mapLR(int Id){
    if(Id == -1) return Id;
    return(mapIsoLR.at(Id));
}

/**
  * \param  Id Identifier of the element on the MR elements list
  * \return The Mapped identifier of the element on the list of all elements
  */

int Data::mapMR(int Id){
    if(Id == -1) return Id;
    return(mapIsoMR.at(Id));
}

/**
  * \param  Id Identifier of the element on the HR elements list
  * \return The Mapped identifier of the element on the list of all elements
  */

int Data::mapHR(int Id){
    if(Id == -1) return Id;
    return(mapIsoHR.at(Id));
}

/**
  * \param data A pointer to the data that have to be copied
  */

void Data::copyData(Data * data){

    clear();

    for(int i=0;i<data->isoSize();i++){
        addIso(Element(data->getIso(i).getElement(),
                       data->getIso(i).getMasse(),
                       data->getIso(i).getResol()));
    }

    for(int i=0;i<data->solutionSize();i++){
        addSolution(Solution(data->getSolution(i).getName(),
                              data->getSolution(i).getNbrElement(),
                              data->getSolution(i).getType()));


        for (int j=0;j<data->isoSize();j++){
            getSolution(i).setCps(j,data->getSolution(i).getCps(j));
            getSolution(i).setCpsSD(j,data->getSolution(i).getCpsSD(j));
            getSolution(i).setConcent(j,data->getSolution(i).getConcent(j));
            getSolution(i).setConcentRSD(j,data->getSolution(i).getConcentRSD(j));
        }
    }
}

/**
  * Clear all the contains : data lists
  */

void Data::clear(){
    elementList.clear();
    solutionList.clear();
    mapBlk.clear();
    mapIsoLR.clear();
    mapIsoMR.clear();
    mapIsoHR.clear();
}

/**
  * \param  file File used for input/output parsing
  * \param process Pointer to processing informations container
  */

QPair<int,QString> Data::executeParsing(QFile * file,Processing *process){
    return(this->parser->parse(file,this,process));
}

/**
  * \param  idBlk List of the Blank solutions taken into account to create new average Blank solution
  * \param  Name Name of the new Blank solution
  */

void Data::addBLKmoy(QList<int> idBlk,QString Name){

    //Only one BLK or not BLK
    if(idBlk.size()<2) return;

    int Id;
    Id = addSolution(Solution(Name,
                               isoSize(),
                               Solution::BLK));

    for(int j=0;j<isoSize();j++){
        double S = 0,maxSD = 0;
        int counter =0;

        for(int i=0;i<solutionSize();i++){
            if(idBlk.indexOf(i) != -1){
                S = S + getSolution(i).getCps(j);
                if(getSolution(i).getCpsSD(j) > maxSD){
                    maxSD = getSolution(i).getCpsSD(j);
                }
                counter++;
            }
        }

        getSolution(Id).setCps(j,S/counter);
        getSolution(Id).setCpsSD(j,maxSD);
    }
}

int Data::containsSolution(QString solutionName)
{
   for(int i = 0 ; i < this->solutionSize() ; i++)
   {
      if (this->getSolution(i).getName() == solutionName)
      {
         return i ;
      }
      else
      {
         continue ;
      }
   }
   
   return -1 ;
}
   
