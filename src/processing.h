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

#ifndef PROCESSING_H
#define PROCESSING_H

#include <QtCore>
#include <QtGui>
#include <iostream>
#include <cmath>
#include "data.h"
#include "regline.h"

using namespace std;

class Data;

class Processing
{
public:
    Processing();   ///< Constructor of Processing class
    ~Processing(); ///< Class destructor
    
    void init(int nb_element); ///< Initializes Processing contain

    void computeConcent(Data *outputData, Data* inputData, int blank_id, int is_ref_id, int* is_elements); ///< Computes concentrations on data container
    void calibration(Data *data, QList<int> notEtal); ///< Computes calibration lines
    void computeLimits(Data *data,QList<int> IdBlanc); ///< Computes limits - of detection (LOD), of Quantification (LOQ)

    
    
    
  
    Regline getLine(int Id); ///< Gets calibration line corresponding to the Id element given
    double getSquareR(int Id); ///< Gets calibration line Error corresponding to the by Id element given
    double getLOD(int Id); ///< Gets LOD corresponding to the Id element given
    double getLOQ(int Id); ///< Gets LOQ corresponding to the Id element given
    
    void subtructBlank(Data *data,int IdBlk, bool subSTD,QList<int> isoNotSub); ///< Applies Blank Substraction
    void correctionIS(Data *data,int IdEchant,int Id,Element::Resolution resol); ///< Applies Internal Standard Correction

    QPair<int, QStringList> passQC(Data *input, Data *output);  ///<  Quality control after data processing

    static double max(QVector<double> vector); ///< Computes the max of a vector
    static double min(QVector<double> vector); ///< Computes the min of a vector
    static double avr(QVector<double> vector); ///< Computes the average of a vector
    static QPair<double,double> stat(QVector<double> vector); ///< Computes the average (first) and the standard deviation (second) of a vector

    static bool isValid(double value); ///< Tests the validit of a number
    
    static bool DEBUG ; ///< Enable debug traces printing
    static bool INFO ; ///< Enable info traces printing

private:
    QList<Regline> lines ; ///< List of calibration lines
    QList<double> squareR;    ///< List of calibration lines erros - Coefficient of determination
    QList<double> LOQ;      ///< List of LOQ for each element
    QList<double> LOD;      ///< List of LOD for each element
    
    void computeLine(Data *data, int Id);      ///< Computes a calibration line.
    
    qreal concentRSD_y_intercept_s(double i_m, double i_sd, const Regline& rl) ;
    
    qreal concentRSD_blank_s (double i_m, double i_sd, const Regline& rl,
                              double blank_i, double blank_sd) ;
    
    qreal concentRSD_y_intercept_IS_s(double i_m, double i_sd, const Regline& rl,
                                      double is_sol_rsd_s, double is_ref_rsd_s,
                                      double alpha) ;
    qreal concentRSD_blank_IS_s(double im, double di, const Regline& rl,
                                double b, double db,
                                double is_sol_rsd_s, double is_ref_rsd_s,
                                double alpha) ;
    qreal computeSlopeRsdS(const Regline& rl) ;
    qreal computeAlpha(double is_ref, double is_sol) ;
    qreal computeIsRSD_s(double dis, double is) ;
};

#endif // PROCESSING_H
