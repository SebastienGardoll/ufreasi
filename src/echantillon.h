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

class Echantillon
{
public:
    enum type{ECHANT,STD,BLANC,QC};
    Echantillon(QString name,int nbrElement,type typeEchantillon);

    QString getName();
    int getNbrElement();
    type getType();

    double getCoups(int Id);
    double getCoupsSD(int Id);
    double getConcent(int Id);
    double getConcentSD(int Id);

    void setCoups(int Id,double coupsVal);
    void setCoupsSD(int Id,double coupsSDVal);
    void setConcent(int Id,double concentrationVal);
    void setConcentSD(int Id,double concentrationSDVal);

private:
    QList<double> * coups;
    QList<double> * coupsSD;
    QList<double> * concentration;
    QList<double> * concentrationSD;

    QString name;
    int nbrElement;
    type typeEchantillon;
};

#endif // TEST_H
