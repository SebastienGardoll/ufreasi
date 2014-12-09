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

#ifndef REGLINE_H
#define REGLINE_H

//#include <QtCore>
//#include <QtGui>
//#include <iostream>
//#include <cmath>

using namespace std;

class Regline {

public :

   Regline () ; ///< Class constructor

   double slope ; ///< line' slope
   double intercept ; ///< line's intercept
   double slopeSdSquare ; ///< square of slope's standard deviation 
   double interceptSdSquare ; ///< square of intercept's standard deviation
};

#endif // REGLINE_H
