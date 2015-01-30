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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtGui>
#include <QtCore>
#include "displaying.h"
#include "data.h"
#include "parser.h"
#include "element.h"
#include "solution.h"
#include "processing.h"
#include "ui_application.h"
#include "qcdialog.h"

#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_zoomer.h>

#include <iostream>
using namespace std;

namespace Ui {
class Application;
}

class Application : public QMainWindow
{    
    Q_OBJECT
public:
    explicit Application(QWidget *parent = 0);      ///< Constructor of Application class
    ~Application();      ///< Destructor of Application class

    void closeEvent(QCloseEvent *event);    ///< Lunched when the application is closed
    
private slots:

    void onListViewInput(int index);        ///< Slot of clicking on Solutions list in input data view
    void onListViewOutput(int index);        ///< Slot of clicking on Solutions list in output data view
    void onListCurves(int index);        ///< Slot of clicking on Elements list in Calibration curve view
    void onListEvol(int index);        ///< Slot of clicking on Elements list in Evolution curve view

    void tab(int currentTab);           ///< Slot of tab change

    void onISChoice();  ///< Slot of choosing Internal standard plot plot to diplay
    void listISChange();  ///< Slot to change Internal standards selection
    void listEvolChoice();  ///< Slot of choosing the evolution plot to diplay

    void saveFile();      ///< Slot of clicking on save output button
    void openFile();      ///< Slot of clicking on open data file button
    void run();      ///< Slot of clicking on proceed button
    void openSTDQC();      ///< Slot of clicking on open QC-STD file button
    void plotSave();      ///< Slot of clicking on save plots button
    void about();  ///< Slot of clicking on about button

    void on_yIntercept_clicked();
    void on_subBlank_clicked();
    void on_correctionIS_clicked();

    void on_icpmsModel_currentIndexChanged(const QString &selection);

private:
    Ui::Application *ui;    ///< Contains the definition of the widgets and HCI elements

    void createActions();   ///< Creation of HCI actions
    void initIHM();   ///< Initialize HCI configuration

    void enableBlankSelection() ; //enable or disable blank selection for linear regression option.

    Data *dataInput;    ///< Input data container - before processing
    Data *dataOutput;    ///< Output data container - after processing
    Displaying *display;    ///< display facade - Gathers all plotting, tables and lists display functions
    Processing *processing;    ///< processing facade - Gathers all processing functions applied to data, contains some processing results

    QAction *aboutAct;  ///< About button action
    QAction *aboutQtAct;  ///< About Qt button action
    QAction *quitAct;  ///< Close button action
    QAction *openQcStdAct;  ///< Open STD-QC file button action
    QAction *openAct;  ///< Open data file button action
    QAction *plotAct;  ///< Save plot button action
    QAction *saveAct;  ///< Save output file button action
    QAction *runAct;  ///< Proceed button action

    Parser * myParserIN1;     ///< Parser Instance 1 - For data input file
    Parser * myParserIN2;     ///< Parser Instance 2 - For STD-QC file
    Parser * myParserOUT;     ///< Parser Instance 3 - For data output file

    QString fileName;     ///< File path and name currently opened
};

#endif // APPLICATION_H
