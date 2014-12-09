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

#ifndef ISOTOPE_H
#define ISOTOPE_H

#include <QString>

class Element{
public:
    /// Enumeration of different possible resolutions
    enum Resolution {HR,    ///< High resolution
                     MR,    ///< Medium resolution
                     LR     ///< Low resolution
                    };
    Element(QString,int mass,Resolution);   ///< Element Class Constructor

    QString getElement();   ///< Gets the Name of the element
    int getMasse(); ///< Gets the Mass
    Resolution getResol();  ///< Gets the resolution (value of Resolution enumeration)
    QString getResolName(); ///< Gets the resolution defined into a String
    QString getName();  ///< Gets the complete Name of the istope, used to display the element to HCI

private:
    QString element;    ///< The name of the istope
    int mass;   ///< The mass of the istope
    Resolution resol;   ///< The resolution of the istope
};

#endif // ISOTOPE_H
