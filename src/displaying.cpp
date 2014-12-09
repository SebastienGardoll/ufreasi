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

#include "displaying.h"
#include <iostream>
#include <qwt_scale_engine.h>

using namespace std;

/**
  * This constructor insantciate the different Curves and attach them to the

  * appropriate Plot space
  * \param ui Pointer to the application interface definition that contains all Widgets of the application
  */

Displaying::Displaying(Ui::Application * ui){
    this->ui = ui;

    //Create differents plots

    //Internal standard plot
    curveIS = new QwtPlotCurve();
    curveIS->attach(ui->graphIS);

    //Evolution plot
    curveEvol = new QwtPlotCurve();
    curveEvol->attach(ui->graphEvol);

    //Calibration points plot
    curveElement = new QwtPlotCurve();
    curveElement->attach(ui->graphElement);

    curveElementQC = new QwtPlotCurve();
    curveElementQC->attach(ui->graphElement);
    
    elementZoomer = new QwtPlotZoomer (ui->graphElement->canvas()) ;

    //Calibration line plot
    lineElement = new QwtPlotCurve();
    lineElement->attach(ui->graphElement);

    //Error bars plot
    //Vertical Bars
    barErreurV = new QwtPlotIntervalCurve();
    barErreurV->attach(ui->graphElement);
    //Horizontal Bars
    barErreurH = new QwtPlotIntervalCurve();
    barErreurH->attach(ui->graphElement);
    barErreurH->setOrientation( Qt::Horizontal );
}

Displaying::~Displaying() {
    delete curveElement;
    delete lineElement;
    delete curveElementQC;
    delete curveIS;
    delete curveEvol;
    delete barErreurV;
    delete barErreurH;
    delete elementZoomer;
}

/**
* \param modele Pointer to table model to fill with titles
*/

void Displaying::initModeleTable(QStandardItemModel * modele){
    modele->setHeaderData(0, Qt::Horizontal, "Element");
    modele->setHeaderData(1, Qt::Horizontal, "Intensity (cps)");
    modele->setHeaderData(2, Qt::Horizontal, "Intensity RSD (%)");
    modele->setHeaderData(3, Qt::Horizontal, "Concentration (ppb)");
    modele->setHeaderData(4, Qt::Horizontal, "Concentration RSD (%)");
}

/**
* Input table reset
*/

void Displaying::dispTableInput(){
    QStandardItemModel * modele = new QStandardItemModel(1, 5);

    initModeleTable(modele);

    ui->tableViewInput->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewInput->setModel(modele);
}

/**
* display the solution itentified by given Id
* \param data Input data container
* \param Id Identifier of the solution to display
*/

void Displaying::dispTableInput(Data *data, int Id){

    QStandardItemModel * modele = new QStandardItemModel(1, 5);

    initModeleTable(modele);

    ui->tableViewInput->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewInput->setModel(modele);

    //Set items of the Data table
    for(int i=0;i<data->isoSize();i++){
        modele->setItem(i, 0,new QStandardItem(data->getIso(i).getName()));
        modele->setItem(i, 1,new QStandardItem(QString::number(data->getSolution(Id).getCps(i),'f',2)));
        modele->setItem(i, 2,new QStandardItem(QString::number(data->getSolution(Id).getCpsRSD(i),'f',2)));

        if(data->getSolution(Id).getConcent(i) == 0){
            modele->setItem(i, 3,new QStandardItem(""));
            modele->setItem(i, 4,new QStandardItem(""));
        }else{
            modele->setItem(i, 3,new QStandardItem(QString::number(data->getSolution(Id).getConcent(i))));
            modele->setItem(i, 4,new QStandardItem(QString::number(data->getSolution(Id).getConcentRSD(i),'f',2)));
        }
    }
}

/**
* Output table reset
*/

void Displaying::dispTableOutput(){
    QStandardItemModel * modele = new QStandardItemModel(1, 5);

    initModeleTable(modele);

    ui->tableViewOutput->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewOutput->setModel(modele);
}

/**
* display the solution itentified by given Id
* \param data Output data container
* \param Id Identifier of the solution to display
* \param process Contains some data processing values - Calibration line, R^2, LOD, LOQ
*/

void Displaying::dispTableOutput(Data *data, int Id,Processing *process){

    QStandardItemModel * modele = new QStandardItemModel(1, 5);

    initModeleTable(modele);

    ui->tableViewOutput->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewOutput->setModel(modele);

    //Set items of the Data table
    for(int i=0;i<data->isoSize();i++){

        modele->setItem(i, 0,new QStandardItem(data->getIso(i).getName()));
        modele->setItem(i, 1,new QStandardItem(QString::number(data->getSolution(Id).getCps(i),'f',2)));
        modele->setItem(i, 2,new QStandardItem(QString::number(data->getSolution(Id).getCpsRSD(i),'f',2)));

        if(data->getSolution(Id).getConcent(i) == 0){
            modele->setItem(i, 3,new QStandardItem(""));
            modele->setItem(i, 4,new QStandardItem(""));
        }else if(data->getSolution(Id).getConcent(i) < process->getLOQ(i)){
            modele->setItem(i, 3,new QStandardItem("<LOQ"));
            modele->setItem(i, 4,new QStandardItem(""));
        }else{
            modele->setItem(i, 3,new QStandardItem(QString::number(data->getSolution(Id).getConcent(i))));
            modele->setItem(i, 4,new QStandardItem(QString::number(data->getSolution(Id).getConcentRSD(i),'f',2)));
        }
    }
}

/**
* \param data data container used to generate lists
*/

void Displaying::displayLists(Data *data){

    // Display All Elements
    QStringList listeElements;

    for(int i=0;i<data->isoSize();i++){
        listeElements << data->getIso(i).getName();
    }

    ui->listCurves->clear();
    ui->listCurves->addItems(listeElements);
    ui->listCurves->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->listEvol->clear();
    ui->listEvol->addItems(listeElements);
    ui->listEvol->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Display BLANK Solutions
    QStringList listeBlanc;
    for(int i=0;i<data->solutionSize();i++){
        if(data->getSolution(i).getType() == Solution::BLK){
            listeBlanc << data->getSolution(i).getName();
        }
    }

    ui->blankSelection->setModel(new QStringListModel(listeBlanc));

    // Display Internal Standard LR/MR/HR
    QStringList listeISLR;
    QStringList listeISMR;
    QStringList listeISHR;
    for(int i=0;i<data->isoSize();i++){
        switch(data->getIso(i).getResol()){
            case Element::LR : listeISLR << data->getIso(i).getName(); break;
            case Element::MR : listeISMR << data->getIso(i).getName(); break;
            case Element::HR : listeISHR << data->getIso(i).getName(); break;
        }
    }

    ui->comboISLR->setModel(new QStringListModel(listeISLR));
    ui->comboISMR->setModel(new QStringListModel(listeISMR));
    ui->comboISHR->setModel(new QStringListModel(listeISHR));
}

/**
* \param dataIn data container : Input data before processing
* \param dataOut data container : Output data after processing
*/

void Displaying::displaySolutions(Data *dataIn,Data *dataOut){

    //Diplay Solutions list
    QStringList listeTestsIn;
    for(int i=0;i<dataIn->solutionSize();i++){
        listeTestsIn << dataIn->getSolution(i).getName();
    }

    ui->listViewInput->clear();
    ui->listViewInput->addItems(listeTestsIn);
    ui->listViewInput->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList listeTestsOut;
    for(int i=0;i<dataOut->solutionSize();i++){
        listeTestsOut << dataOut->getSolution(i).getName();
    }

    ui->listViewOutput->clear();
    ui->listViewOutput->addItems(listeTestsOut);
    ui->listViewOutput->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->echantIS->clear();
    ui->echantIS->addItems(listeTestsOut);

    //Display Blank Solutions list
    QStringList listeBlanc;
    for(int i=0;i<dataOut->solutionSize();i++){
        if(dataOut->getSolution(i).getType() == Solution::BLK){
            listeBlanc << dataOut->getSolution(i).getName();
        }
    }

    ui->blankSelection->setModel(new QStringListModel(listeBlanc));
}

/**
* display the evolution of the Internal standard element itentified by given Id
* \param data data container used to plot Internal standard evolution
* \param Id Idetifier of the element selected to be the Internal standard and to be plotted
* \param process Contains processed parameters - internal standard mean and standard deviation
*/

void Displaying::plotIS(Data * data,int Id, Processing *process){

    if(Id == -1) return;

    //Display plot title
    ui->graphIS->setTitle(QwtText("Internal Standard "+
                                  data->getIso(Id).getName()));

    ui->graphIS->setAxisScale(QwtPlot::xBottom,1,data->solutionSize(),1);

    //Configure Internal Standards Plot
    curveIS->setRenderHint(QwtPlotItem::RenderAntialiased);
    curveIS->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::black),QPen(Qt::black,1),QSize(5,5)));
    curveIS->setPen(QPen(Qt::blue,1));

    QVector<double> x;
    QVector<double> y;
    
    //Plot Internal Standard Values
    for(int i=0;i<data->solutionSize();i++){
        if(Processing::isValid(data->getSolution(i).getCps(Id))){
            x.push_back(i+1);
            y.push_back(data->getSolution(i).getCps(Id));
        }
    }

    curveIS->setSamples(x, y);
    ui->graphIS->replot();
    
    QPair<double, double> isStat = process->stat(y) ;
    
    double rsd = 100. * isStat.second / isStat.first ;
    
    //Setting the Legend
    ui->graphIS->insertLegend(new QwtLegend, QwtPlot::BottomLegend);

    curveIS->setTitle(QwtText("Average intensity (cps) =  " + QString::number(isStat.first, 'f', 1) +
                              " | SD (cps) = " + QString::number(isStat.second, 'f', 1) +
                              " | RSD (%) = " + QString::number(rsd, 'f', 1)));
}

/**
* display the evolution of the element itentified by given Id
* \param data data container used to plot the evolution of the selectec element
* \param Id Idetifier of the element selected to be plotted
* \param evolName Solutions used to plot element evolution, have to contain evolName
* in their Name to be taken into account
*/

void Displaying::plotEvol(Data * data,int Id,QString evolName){

    if(evolName == "" or Id == -1) return;

    //Display plot title
    ui->graphEvol->setTitle(QwtText("Evolution "+evolName+" - "+data->getIso(Id).getName()));

    //Configure Evolution Plot
    curveEvol->setRenderHint(QwtPlotItem::RenderAntialiased);
    curveEvol->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::black),QPen(Qt::black,1),QSize(5,5)));
    curveEvol->setPen(QPen(Qt::blue,1));

    ui->graphEvol->setAxisScale(QwtPlot::xBottom,1,data->solutionSize(),1);

    QVector<double> x;
    QVector<double> y;

    //Plot Evolution
    for(int i=0;i<data->solutionSize();i++){
        if(data->getSolution(i).getName().contains(QRegExp(evolName))){
            x.push_back(i+1);
            y.push_back(data->getSolution(i).getCps(Id));
        }
    }

    curveEvol->setSamples(x, y);

    ui->graphEvol->replot();
}

/**
* display the calibration plot of the element itentified by given Id
* \param output data container after processing
* \param input data container before processing
* \param Id Idetifier of the element selected to be plotted
* \param process Contains processed parameters - Calibration line, R^2, LOQ, LOD
*/

void Displaying::plotElement(Data *output,Data *input,int Id,Processing *process){

    if(Id == -1) return;

    ui->graphElement->setAutoDelete(true);
    ui->graphElement->setAutoReplot(true);
    ui->graphElement->setAxisAutoScale(QwtPlot::xBottom);
    ui->graphElement->setAxisAutoScale(QwtPlot::yLeft);

    //Display plot title
    ui->graphElement->setTitle(QwtText("Calibration "
                                       +output->getIso(Id).getName()));

    //Configure Elements Plot
    curveElement->setRenderHint(QwtPlotItem::RenderAntialiased);
    curveElement->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::black),QPen(Qt::black,1),QSize(2,2)));
    curveElement->setStyle(QwtPlotCurve::NoCurve);

    QVector<double> x;
    QVector<double> y;
    QVector<QwtIntervalSample> barresV;
    QVector<QwtIntervalSample> barresH;

    barErreurV->setSymbol(new QwtIntervalSymbol(QwtIntervalSymbol::Bar));
    barErreurV->setStyle(QwtPlotIntervalCurve::NoCurve);
    barErreurV->setRenderHint(QwtPlotItem::RenderAntialiased);

    barErreurH->setSymbol(new QwtIntervalSymbol(QwtIntervalSymbol::Bar));
    barErreurH->setStyle(QwtPlotIntervalCurve::NoCurve);
    barErreurH->setRenderHint(QwtPlotItem::RenderAntialiased);

    //Plot Element Values (take into account STD values only)
    for(int i=0;i<output->solutionSize();i++){
        if(output->getSolution(i).getType()==Solution::STD){
            double cps = output->getSolution(i).getCps(Id);
            double concent = output->getSolution(i).getConcent(Id);
            double cpsErr = output->getSolution(i).getCpsRSD(Id);
            double concentErr = output->getSolution(i).getConcentRSD(Id);

            if((concent ==0 || cps==0) && output->getSolution(i).getName() != Solution::ZERO_STD_NAME)
               continue;

            x.push_back(concent);
            y.push_back(cps);
            barresV.push_back(QwtIntervalSample(concent,
                                               cps+cps*cpsErr/100,
                                               cps-cps*cpsErr/100));
            barresH.push_back(QwtIntervalSample(cps,
                                                  concent+concent*concentErr/100,
                                                  concent-concent*concentErr/100));
        }
    }

    //Diplay Values
    curveElement->setSamples(x, y);

    //Configure Elements Plot QC
    curveElementQC->setRenderHint(QwtPlotItem::RenderAntialiased);
    curveElementQC->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::red),QPen(Qt::red,1),QSize(3,3)));
    curveElementQC->setStyle(QwtPlotCurve::NoCurve);

    QVector<double> xQC;
    QVector<double> yQC;

    //Plot QC Values
    for(int i=0;i<output->solutionSize();i++){
        if(output->getSolution(i).getType()==Solution::QC){
            double cps = output->getSolution(i).getCps(Id);
            double concent = input->getSolution(i).getConcent(Id);
            double cpsErr = output->getSolution(i).getCpsRSD(Id);
            double concentErr = input->getSolution(i).getConcentRSD(Id);

            if(concent ==0 or cps==0) continue;

            xQC.push_back(concent);
            yQC.push_back(cps);
            barresV.push_back(QwtIntervalSample(concent,
                                                  cps+cps*cpsErr/100,
                                                  cps-cps*cpsErr/100));
            barresH.push_back(QwtIntervalSample(cps,
                                                  concent+concent*concentErr/100,
                                                  concent-concent*concentErr/100));
        }
    }

    //Diplay Values
    curveElementQC->setSamples(xQC, yQC);

    //Error Bars
    barErreurV->setSamples(barresV);
    barErreurH->setSamples(barresH);

    QVector<double> xD;
    QVector<double> yD;

    //Configure Elements Calibration line Plot
    lineElement->setRenderHint(QwtPlotItem::RenderAntialiased);
    lineElement->setPen(QPen(Qt::blue,1));

    //Plot Calibration line plot
    double valMaxX = Processing::max(x+xD+xQC);
    double valMinX = Processing::min(x+xD+xQC);

    double valMaxY = Processing::max(y+yD+yQC);
    double valMinY = Processing::min(y+yD+yQC);

    valMinX = 0.9*valMinX;
    valMaxX = 1.1*valMaxX;

    valMinY = 0.9*valMinY;
    valMaxY = 1.1*valMaxY;

    xD.push_back(valMinX);
    yD.push_back(process->getLine(Id).slope*valMinX+process->getLine(Id).intercept);
    xD.push_back(valMaxX);
    yD.push_back(process->getLine(Id).slope*valMaxX+process->getLine(Id).intercept);

    lineElement->setSamples(xD,yD);

    //Setting the Legend
    ui->graphElement->insertLegend(new QwtLegend, QwtPlot::BottomLegend);

    lineElement->setTitle(QwtText("y = " + QString::number(process->getLine(Id).slope, 'f', 1) +
                                    " x + (" + QString::number(process->getLine(Id).intercept, 'f', 1) + ")" +
                                  " | " + QString("R%1").arg(QChar(0x00B2)) +  " = " + QString::number(process->getSquareR(Id), 'f', 5) +
                                    " | LOD = " + QString::number(process->getLOD(Id), 'f', 6) + " ppb" +
                                    " | LOQ = " + QString::number(process->getLOQ(Id), 'f', 6) + " ppb"));
    curveElementQC->setTitle("QC");
    curveElement->setTitle(QwtText("STD "+output->getIso(Id).getName()));
    barErreurH->setTitle("Error Bars");

    if(valMinY != valMaxY and valMinX != valMaxX){
        ui->graphElement->setAxisScale(QwtPlot::yLeft, valMinY, valMaxY);
        ui->graphElement->setAxisScale(QwtPlot::xBottom, valMinX,valMaxX);
    }
    ui->graphElement->replot();
    
    elementZoomer->setZoomBase() ;
}
