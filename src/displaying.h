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

#ifndef DISPLAYING_H
#define DISPLAYING_H

#include <QtGui>
#include <QtCore>
#include "ui_application.h"
#include "data.h"
#include "processing.h"

#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend_item.h>
#include <qwt_plot_picker.h>
#include <qwt_interval_symbol.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_curve_fitter.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>

class Data;
class Processing;

class Displaying{
public:
    Displaying(Ui::Application *);   ///< Constructor of Display Class
    ~Displaying() ; ///< Destructor of Display Class
     
    void initModeleTable(QStandardItemModel * modele); ///< Fills the models attached to tables with titles
    void dispTableInput(Data *data,int Id); ///< displays input data on the input Table
    void dispTableInput();  ///< Erases display on the input Table
    void dispTableOutput(Data *data,int Id,Processing *process);    ///< display output data on the output Table
    void dispTableOutput(); ///< Erases display on the output Table

    void displaySolutions(Data *dataIn,Data *dataOut);    ///< Generates list of solutions
    void displayLists(Data *data);  ///< Generates lists : of Elements, HR-MR-LR Elements and Blanks

    void plotIS(Data *data, int Id, Processing *process);    ///< Plots internal standard curve
    void plotElement(Data *output,Data *input,int Id, Processing *process); ///< Plots Calibration curves
    void plotEvol(Data * data,int Id, QString evolName); ///< Plots Evolution curve

private:
    Ui::Application * ui;   ///< Interface model pointer of the application

    QwtPlotCurve * curveElement; ///< Calibaration points curve - STD values
    QwtPlotCurve * lineElement; ///< Calibaration line curve
    QwtPlotCurve * curveElementQC; ///< Quality Control points curve - QC values

    QwtPlotCurve * curveIS; ///< Internal standard curve
    QwtPlotCurve * curveEvol; ///< Evolution tab curve

    QwtPlotIntervalCurve * barErreurV; ///< Vertical error bars - STD and QC values
    QwtPlotIntervalCurve * barErreurH; ///< Horizontal error bars - STD and QC values
    
    QwtPlotZoomer* elementZoomer ; ///< Zoomer for calibration plot
};

#endif // DISPLAY_H
