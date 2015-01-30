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

#include "application.h"
#include <typeinfo>

/**
  * Create actions, initialize HCI, create Data - Display - Processing - Parser instances,

  * Open last application's settings
  */

Application::Application(QWidget *parent) : QMainWindow(parent), ui(new Ui::Application){
    ui->setupUi(this);

    //Set minimum Size
    // int largeur = QApplication::desktop()->width();
    // int hauteur = QApplication::desktop()->height();

    // this->adjustSize() ; configuration doesn't fit.
    this->setMinimumSize(820, 500);

    //Process actions on the HCI
    createActions();

    //Instancier
    processing = new Processing();

    myParserIN1 = new ParserInHRElementCSV(); // ParserInHRElementCSV set as default parser
    myParserOUT = new ParserOutCSV();
    myParserIN2 = new ParserInSTDQC();

    dataInput = new Data(myParserIN1);
    dataOutput = new Data(myParserOUT);

    display = new Displaying(ui);

    //Instanciate configuration
    QSettings settings("LGE", "Space");

    if(settings.value("File").toString() != ""){
        fileName = settings.value("File").toString();
    }

    //Init IHM
    initIHM();
}

Application::~Application(){
    delete ui;
}

void Application::about(){
    QMessageBox::about(this, tr("About uFREASI Application"),
                       tr("<p>uFREASI : <b>user-FRiendly Elemental dAta procesSIng v1.31</b></p>"
                          "<p>Application to process ICP-MS data.</p>"
                          "<p><b>Designed by :</b></p>"
                          "<p> KHELIFI Oualid - Telecom ParisTech - khelifi@enst.fr</p>"
                          "<p> THARAUD Mickael - Laboratoire de G&eacute;ochimie des Eaux - tharaud@ipgp.fr</p>"
                          "<p><b>Maintain by :</b></p>"
                          "<p> GARDOLL Sebastien - Telecom ParisTech - gardoll@enst.fr</p>"));
}

void Application::createActions(){

    // Tool Bar
    quitAct = new QAction(QIcon(":/res/close.png"),tr("&Quit"), this);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(QIcon(":/res/ufreasi.png"),tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(QIcon(":/res/qt.png"),tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    openQcStdAct = new QAction(QIcon(":/res/config.png"),tr("&STD-QC File"), this);
    openQcStdAct->setStatusTip(tr("Open STD-QC file"));
    connect(openQcStdAct, SIGNAL(triggered()), this, SLOT(openSTDQC()));

    openAct = new QAction(QIcon(":/res/open.png"),tr("&Data File"), this);
    openAct->setStatusTip(tr("Open input Data file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    plotAct = new QAction(QIcon(":/res/plot.png"),tr("&Save Plot"), this);
    plotAct->setStatusTip(tr("Save the plotted Graph"));
    connect(plotAct, SIGNAL(triggered()), this, SLOT(plotSave()));
    plotAct->setEnabled(false);

    saveAct = new QAction(QIcon(":/res/save.png"),tr("&Save Output"), this);
    saveAct->setStatusTip(tr("Save Output Data"));
    connect(saveAct,SIGNAL(triggered()), this, SLOT(saveFile()));
    saveAct->setEnabled(false);

    runAct = new QAction(QIcon(":/res/run.png"),tr("&Process"), this);
    runAct->setStatusTip(tr("Run the processing"));
    connect(runAct,SIGNAL(triggered()), this, SLOT(run()));
    runAct->setEnabled(false);

    //Add to toolbar
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolBar->setMovable(false);

    ui->toolBar->addAction(openAct);
    ui->toolBar->addAction(openQcStdAct);

    ui->toolBar->addSeparator();
    ui->toolBar->addAction(plotAct);
    ui->toolBar->addAction(saveAct);

    ui->toolBar->addSeparator();
    ui->toolBar->addAction(aboutAct);
    ui->toolBar->addAction(aboutQtAct);

    ui->toolBar->addSeparator();
    ui->toolBar->addAction(quitAct);
    ui->toolBar->addSeparator();

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->addWidget(spacer);

    ui->toolBar->addSeparator();
    ui->toolBar->addAction(runAct);

    //Lists selection
    connect(ui->listViewInput,SIGNAL(currentRowChanged(int)),
            this, SLOT(onListViewInput(int)));

    connect(ui->listViewOutput,SIGNAL(currentRowChanged(int)),
            this, SLOT(onListViewOutput(int)));

    connect(ui->listCurves,SIGNAL(currentRowChanged(int)),
            this, SLOT(onListCurves(int)));

    connect(ui->listEvol,SIGNAL(currentRowChanged(int)),
            this, SLOT(onListEvol(int)));

    connect(ui->plotISchoice,SIGNAL(currentIndexChanged(int)),this,SLOT(onISChoice()));

    connect(ui->comboISLR,SIGNAL(currentIndexChanged(int)),this,SLOT(listISChange()));
    connect(ui->comboISMR,SIGNAL(currentIndexChanged(int)),this,SLOT(listISChange()));
    connect(ui->comboISHR,SIGNAL(currentIndexChanged(int)),this,SLOT(listISChange()));

    connect(ui->evolChoice,SIGNAL(currentIndexChanged(int)),this,SLOT(listEvolChoice()));

    //Tab Changement
    connect(ui->tabs,SIGNAL(currentChanged(int)),this,SLOT(tab(int)));
}

void Application::initIHM(){

    ui->tabs->setCurrentIndex(0);

    //Graphs
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(Qt::gray, 0.0, Qt::DotLine));
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(true);
    grid->setRenderHint(QwtPlotItem::RenderAntialiased);
    grid->attach(ui->graphElement);

    QwtPlotGrid *grid1 = new QwtPlotGrid();
    grid1->setPen(QPen(Qt::gray, 0.0, Qt::DotLine));
    grid1->enableX(true);
    grid1->enableXMin(true);
    grid1->enableY(true);
    grid1->enableYMin(true);
    grid1->setRenderHint(QwtPlotItem::RenderAntialiased);
    grid1->attach(ui->graphIS);

    QwtPlotGrid *grid2 = new QwtPlotGrid();
    grid2->setPen(QPen(Qt::gray, 0.0, Qt::DotLine));
    grid2->enableX(true);
    grid2->enableXMin(true);
    grid2->enableY(true);
    grid2->enableYMin(true);
    grid2->setRenderHint(QwtPlotItem::RenderAntialiased);
    grid2->attach(ui->graphEvol);

    ui->graphElement->setTitle(QwtText("Calibration"));
    ui->graphElement->setAxisTitle( QwtPlot::xBottom, "Concentration (ppb)" );
    ui->graphElement->setAxisTitle( QwtPlot::yLeft, "Intensity (cps)" );
    ui->graphElement->replot();

    ui->graphIS->setTitle(QwtText("Internal Standard"));
    ui->graphIS->setAxisTitle(QwtPlot::xBottom, "Samples running order" );
    ui->graphIS->setAxisLabelRotation(QwtPlot::xBottom,270);
    // ui->graphIS->setAxisFont(QwtPlot::xBottom,QFont("times",12,QFont::Bold));
    ui->graphIS->setAxisTitle( QwtPlot::yLeft, "Intensity (cps)" );
    ui->graphIS->setAxisMaxMinor(QwtPlot::xBottom, 0);
    ui->graphIS->replot();

    ui->graphEvol->setTitle(QwtText("Evolution"));
    ui->graphEvol->setAxisTitle(QwtPlot::xBottom, "Samples running order" );
    ui->graphEvol->setAxisLabelRotation(QwtPlot::xBottom,270);
    // ui->graphEvol->setAxisFont(QwtPlot::xBottom,QFont("times",12,QFont::Bold));
    ui->graphEvol->setAxisTitle( QwtPlot::yLeft, "Intensity (cps)" );
    ui->graphEvol->setAxisMaxMinor(QwtPlot::xBottom, 0);
    ui->graphEvol->replot();

    // Set ICP-MS model available
    ui->icpmsModel->addItem(ParserInHRElementCSV::ICP_MS_NAME);
    ui->icpmsModel->addItem(ParserInAgilentCSV::ICP_MS_NAME);
    ui->icpmsModel->setCurrentIndex(0);
}

/**

  * \param index Current index on the solutions list on Input tab
  */

void Application::onListViewInput(int index){
    //Display Input Data table
    if(index == -1) return;
    display->dispTableInput(dataInput,index);
}

/**
  * \param index Current index on the solutions list on Output tab
  */

void Application::onListViewOutput(int index){
    //Display Output Data table
    if(index == -1) return;
    display->dispTableOutput(dataOutput,index,processing);
}

/**
  * \param index Current index on the elements list on Calibration plot tab
  */

void Application::onListCurves(int index){
    //Diplay Elements plot
    if(index == -1) return;
    display->plotElement(dataOutput,dataInput,index,processing);
}

/**
  * \param index Current index on the elements list on Evolution plot tab
  */

void Application::onListEvol(int index){
    //Diplay Elements plot
    if(index == -1) return;
    display->plotEvol(dataInput,index,ui->evolChoice->currentText());
}

void Application::onISChoice(){
    //Plot selected Internal Standard
    int current;
    switch(ui->plotISchoice->currentIndex()){
        case 0 : current = dataInput->mapLR(ui->comboISLR->currentIndex()); break;
        case 1 : current = dataInput->mapMR(ui->comboISMR->currentIndex()); break;
        case 2 : current = dataInput->mapHR(ui->comboISHR->currentIndex()); break;
    default : current = -1;
    }
    display->plotIS(dataInput,current, processing);
}

void Application::listEvolChoice(){
    display->plotEvol(dataInput,ui->listEvol->currentIndex().row(),ui->evolChoice->currentText());
}

void Application::listISChange(){
    //Internal Standard choice list
    QStringList listISChoice;
    listISChoice << "LR Internal Standard : " + ui->comboISLR->currentText();
    listISChoice << "MR Internal Standard : " + ui->comboISMR->currentText();
    listISChoice << "HR Internal Standard : " + ui->comboISHR->currentText();
    ui->plotISchoice->setModel(new QStringListModel(listISChoice));
}

void Application::on_icpmsModel_currentIndexChanged(const QString &selection)
{
    if(ParserInHRElementCSV::ICP_MS_NAME.compare(selection) == 0)
    {
        if(typeid(myParserIN1) != typeid(ParserInHRElementCSV))
        {
            delete myParserIN1;
            myParserIN1 = new ParserInHRElementCSV() ;
            dataInput->loadParser(myParserIN1);
        }
    }
    else if(ParserInAgilentCSV::ICP_MS_NAME.compare(selection) == 0)
    {
        if(typeid(myParserIN1) != typeid(ParserInAgilentCSV))
        {
            delete myParserIN1;
            myParserIN1 = new ParserInAgilentCSV();
            dataInput->loadParser(myParserIN1);
        }
    }
}

void Application::run(){

    if(Processing::DEBUG)
    {
        cout << "*** begin processing ***" << endl ;
        cout << "# uFREASI configuration:" << endl ;
        cout << "  Selected ICP instrument: " << ui->icpmsModel->currentText().toStdString() << endl ;

        if(ui->correctionIS->isChecked())
        {
            cout << "  Internal standard correction is set with:" << endl ;
            cout << "    - " << ui->echantIS->currentText().toStdString() << " as the IS reference solution" << endl ;
            cout << "    - IS check boxes' status are: " << ui->correctISLR->isChecked() << " ; "
                    << ui->correctISMR->isChecked() << " ; " << ui->correctISHR->isChecked() << endl ;
            cout << "    - " << ui->comboISLR->currentText().toStdString() << " as the IS-LR isotope reference" << endl ;
            cout << "    - " << ui->comboISMR->currentText().toStdString() << " as the IS-MR isotope reference" << endl ;
            cout << "    - " << ui->comboISHR->currentText().toStdString() << " as the IS-HR isotope reference" << endl ;
        }
        else
        {
            cout << "  No internal standard correction is applied" << endl ;
        }

        bool displayBlk = true ;

        if(ui->yIntercept->isChecked())
        {
            cout << "  Y-intercept is set" ;
        }
        else
        {
            if(ui->subBlank->isChecked())
            {
                cout << "  Blank subtraction is set" ;
            }
            else
            {
                cout << "  No blank correction is applied" << endl ;
                displayBlk = false ;
            }
        }

        if(displayBlk)
        {
            cout << " with " << ui->blankSelection->currentText().toStdString() << " as the blank reference solution" << endl;
        }

        if(ui->controlQC->isChecked())
        {
            cout << "  Quality control validation is set" << endl ;
        }
    }

    if(Processing::DEBUG)
        cout << "# open QC-STD file" << endl ;

    QSettings settings("LGE", "uFREASI");

    if(settings.value("File").toString() != ""){
        fileName = settings.value("File").toString();
    }else{
        QMessageBox::warning(this, tr("Read QC-STD File"),
                             tr("You have to load a QC-STD Concentrations File"));
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Read File"),
                             tr("Cannot read STD QC Concentration file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    if(Processing::DEBUG)
        cout << "# parse QC-STD file" << endl ;

    //Get concentrations from input file
    dataInput->loadParser(myParserIN2);
    QPair<int,QString> retour = dataInput->executeParsing(&file,processing);

    file.close();

    if(retour.first == 1){
        QMessageBox::information(this,tr("Parsing CSV"),retour.second);
        return;
    }

    if(Processing::DEBUG)
        cout << "# init processing variables" << endl ;

    int blank_id = -1 ;
    int is_ref_id = -1 ;
    int is_elements[] = {-1 , -1 , -1} ; // IS for respectively HR, MR and LR resolution.
                                         // Yes it is almost redundant with isoIS.
    //Internal Standard elements
    QList<int> isoIS;
    if(ui->correctionIS->isChecked() == true){
        if(ui->correctISLR->isChecked() == true) isoIS << dataInput->mapLR(ui->comboISLR->currentIndex());
        if(ui->correctISMR->isChecked() == true) isoIS << dataInput->mapMR(ui->comboISMR->currentIndex());
        if(ui->correctISHR->isChecked() == true) isoIS << dataInput->mapHR(ui->comboISHR->currentIndex());
    }

    //Copy data for reprocessing
    dataOutput->copyData(dataInput);

    //Correction with internal standards
    //Manage multiple internal stanadrds according to resolution
    // ALWAYS do first as average blank calulation is wrong if the internal standard reference is taken from a blank.
    if (ui->correctionIS->isChecked() == true)
    {
        if(Processing::DEBUG)
            cout << "# internal standard correction" << endl ;

        is_ref_id = ui->echantIS->currentIndex() ;
        
        is_elements[Element::LR] = dataInput->mapLR(ui->comboISLR->currentIndex()) ;
        is_elements[Element::MR] = dataInput->mapMR(ui->comboISMR->currentIndex()) ;
        is_elements[Element::HR] = dataInput->mapHR(ui->comboISHR->currentIndex()) ;
        
        if(ui->correctISLR->isChecked() == true){
            processing->correctionIS(dataOutput,
                                     is_ref_id,
                                     is_elements[Element::LR],
                                     Element::LR);
        }
        if(ui->correctISMR->isChecked() == true){
            processing->correctionIS(dataOutput,
                                     is_ref_id,
                                     is_elements[Element::MR],
                                     Element::MR);
        }
        if(ui->correctISHR->isChecked() == true){
            processing->correctionIS(dataOutput,
                                     is_ref_id,
                                     is_elements[Element::HR],
                                     Element::HR);
        }
    }

    //Substract Blank or not (not substract Internal standards)
    if (ui->subBlank->isChecked() == true)
    {
        if(Processing::DEBUG)
            cout << "# substract blank" << endl ;

        blank_id = dataOutput->mapBLK(ui->blankSelection->currentIndex()) ;
        processing->subtructBlank(dataOutput,
                                  blank_id,
                                  true,
                                  isoIS);
    }
    
    if(Processing::DEBUG)
        cout << "# add blk solutions" << endl ;

    //Add BLK Solutions
    QList<int> idBlkMoy;
    for(int i=0;i<dataInput->solutionSize();i++){
        if(dataInput->getSolution(i).getType() == Solution::BLK) idBlkMoy << i;
    }

    dataOutput->addBLKmoy(idBlkMoy, Data::ALL_AVG);

    idBlkMoy.clear();
    int i=0;
    while(dataInput->getSolution(i).getType() != Solution::BLK and i<dataInput->solutionSize()){
        i++;
    }

    while(dataInput->getSolution(i).getType() == Solution::BLK and i<dataInput->solutionSize()){
       idBlkMoy << i;
        i++;
    }
    
    dataOutput->addBLKmoy(idBlkMoy,Data::FIRST_SEQ_AVG);
    
    if(ui->yIntercept->isChecked())
    {
        if(Processing::DEBUG)
            cout << "# y intercept" << endl ;

       // Blank solution are part of the linear regression.
       // Add blank as a zero concentration standard.
       
       // If the user doesn't provide a zero standard named Solution::ZERO_STD_NAME then create it.
       if(dataOutput->containsSolution(Solution::ZERO_STD_NAME) == -1)
       {
          Solution blkRef = dataOutput->getSolution(dataOutput->mapBLK(ui->blankSelection->currentIndex())) ;
          dataOutput->addSolution(blkRef.copy(Solution::ZERO_STD_NAME, Solution::STD)) ;
       
          // Add an item in the output solution list if not already done.
          if(ui->listViewOutput->item(ui->listViewOutput->count() -1 )->text() != Solution::ZERO_STD_NAME)
             ui->listViewOutput->addItem(Solution::ZERO_STD_NAME) ;
       }
    }
    
    if(Processing::DEBUG)
        cout << "# process calibration" << endl ;

    //Process calibration
    processing->calibration(dataOutput,isoIS);
    
    //DEBUG
    if(Processing::DEBUG)
    {
       cout << "blank_id = " << blank_id << ", is_ref_id = " << is_ref_id ;
       for(int i = 0 ; i < 3 ; i++)
          cout << ", is_elements[" << i << "] = " << is_elements[i] ;
       cout << endl ;
    }
    
    if(Processing::DEBUG)
        cout << "*** process concentration ***" << endl ;

    //Process concentrations
    processing->computeConcent(dataOutput, dataInput, blank_id, is_ref_id, is_elements) ;
    
    if(Processing::DEBUG)
        cout << "*** process limits ***" << endl ;

    processing->computeLimits(dataOutput,idBlkMoy);

    //Apply Quality control
    if (ui->controlQC->isChecked() == true)
    {
        if(Processing::DEBUG)
            cout << "*** process QC control ***" << endl ;

        QPair<int,QStringList> retour = processing->passQC(dataInput,dataOutput);

        int nbrQC = 0;

        for(int i=0;i<dataInput->solutionSize();i++){
            if(dataInput->getSolution(i).getType() == Solution::QC){
                for(int j=0;j<dataInput->isoSize();j++){
                    if(dataInput->getSolution(i).getConcent(j) != 0) nbrQC++;
                }
            }
        }
        
        // if(retour.first == 1)
        {
            QString debugTrace (QString::number(retour.second.size()) + " / " + QString::number(nbrQC)) ;
            cout << "# QC not valid: " << debugTrace.toStdString() << endl ;
            QCDialog * dialog =  new QCDialog(&retour.second,nbrQC,this);
            dialog->exec();
        }
    }

    if(Processing::DEBUG)
        cout << "*** refreshing plots and tables ***" << endl ;

    //Refresh plot of Internal Standard
    onISChoice();

    int row;

    //Refresh elements plot
    row = ui->listCurves->currentIndex().row();
    if(row != -1){
        display->plotElement(dataOutput,dataInput,row,processing);
    }

    //Refrech output data table
    row = ui->listViewOutput->currentIndex().row();
    if(row != -1){
        display->dispTableOutput(dataOutput,row,processing);
    }

    //Refrech input data table
    row = ui->listViewInput->currentIndex().row();
    if(row != -1){
         display->dispTableInput(dataInput,row);
    }

    if(Processing::DEBUG)
        cout << "*** end of processing ***" << endl ;
}

void Application::openFile()
{
    runAct->setEnabled(false);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose a Data file"), ".",
                                                    tr("CSV File (*.csv)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Read File"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    dataInput->clear();

    //Get Input data from file
    dataInput->loadParser(myParserIN1);

    //Display return message
    QPair<int,QString> retour = dataInput->executeParsing(&file,processing);

    file.close();

    QMessageBox::information(this,tr("Parsing CSV"),retour.second);

    if (retour.first !=0){

        dataOutput->clear();
        dataOutput->copyData(dataInput);

        //Initialiaze processing interface
        processing->init(dataOutput->isoSize());

        //Initialize table view
        display->dispTableInput();
        display->dispTableOutput();

        //Display Elements/Solutions parsed
        display->displayLists(dataOutput);
        display->displaySolutions(dataInput,dataOutput);

        ui->plotISchoice->clear();
        ui->evolChoice->clear();
        return;
    }

    runAct->setEnabled(true);

    //Clear Output Data
    //Copy Input Data on Output Data
    dataOutput->clear();
    dataOutput->copyData(dataInput);

    //Add BLK Solutions
    QList<int> idBlkMoy;
    for(int i=0;i<dataInput->solutionSize();i++){
        if(dataInput->getSolution(i).getType() == Solution::BLK) idBlkMoy << i;
    }

    dataOutput->addBLKmoy(idBlkMoy,Data::ALL_AVG);

    idBlkMoy.clear();
    int i=0;
    while(dataInput->getSolution(i).getType() != Solution::BLK and i<dataInput->solutionSize()){
        i++;
    }

    while(dataInput->getSolution(i).getType() == Solution::BLK and i<dataInput->solutionSize()){
        idBlkMoy << i;
        i++;
    }
    dataOutput->addBLKmoy(idBlkMoy,Data::FIRST_SEQ_AVG);

    //Initialiaze processing interface
    processing->init(dataOutput->isoSize());

    //Initialize table view
    display->dispTableInput();
    display->dispTableOutput();

    //Display Elements/Solutions parsed
    display->displayLists(dataOutput);
    display->displaySolutions(dataInput,dataOutput);

    QSettings settings("LGE", "uFREASI");

    if(settings.value("LR").toString() != ""){
        int value = ui->comboISLR->findText(settings.value("LR").toString());
        if(value != -1) ui->comboISLR->setCurrentIndex(value);
    }
    if(settings.value("MR").toString() != ""){
        int value = ui->comboISMR->findText(settings.value("MR").toString());
        if(value != -1) ui->comboISMR->setCurrentIndex(value);
    }
    if(settings.value("HR").toString() != ""){
        int value = ui->comboISHR->findText(settings.value("HR").toString());
        if(value != -1) ui->comboISHR->setCurrentIndex(value);
    }

    //Refresh Choosen Internal Standards
    listISChange();

    //Display Internal Standards plot
    onISChoice();

    QStringList listeEvol;
    listeEvol << "BLK";
    listeEvol << "VS";
    ui->evolChoice->clear();
    ui->evolChoice->addItems(listeEvol);
}

void Application::openSTDQC(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose a file - QC & STD"), ".",
                                                    tr("CSV File (*.csv)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Read File"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    // Instancier la conf
    QSettings settings("LGE", "uFREASI");
    settings.setValue("File",fileName);
}

void Application::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Choose a file name"), ".",
                                                    tr("CSV File (*.csv)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Save File"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    //Save Output data
    dataOutput->executeParsing(&file,processing);
    file.close();
}

void Application::plotSave()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save Current Plot",  QString(),
                                                    "JPG File (*.jpg)");

     //ui->tabs->indexOf(ui->tab_graphs)
    if ( !fileName.isEmpty() )
    {
        QPixmap picture;
        //Save current displayed plot
        if(ui->tabs->currentIndex()==ui->tabs->indexOf(ui->tabIS)){
            //Current displayed plot = graphIS
            picture = QPixmap::grabWidget(ui->graphIS);
            picture.save(fileName);
        }else if (ui->tabs->currentIndex()==ui->tabs->indexOf(ui->tabElmnt)){
            //Current displayed plot = graphElement
            picture = QPixmap::grabWidget(ui->graphElement);
            picture.save(fileName);
        }else if (ui->tabs->currentIndex()==ui->tabs->indexOf(ui->tabEvol)){
            //Current displayed plot = graphEvolution
            picture = QPixmap::grabWidget(ui->graphEvol);
            picture.save(fileName);
        }
    }
}

/**
  * \param event Close event that contains informations about the event
  */

void Application::closeEvent(QCloseEvent *event){
    int ret = QMessageBox::question(this, tr("Close"),
                                    tr("Do you really want to close this application ?"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::Yes);
    switch (ret) {
    case QMessageBox::No:{
        event->ignore();
        break;
    }
    case QMessageBox::Yes:{
        QSettings settings("LGE", "uFREASI");
        if(ui->comboISLR->currentText() != "" and
                ui->comboISLR->currentText() != settings.value("LR").toString()){
            settings.setValue("LR",ui->comboISLR->currentText());
        }
        if(ui->comboISMR->currentText() != "" and
                ui->comboISMR->currentText() != settings.value("MR").toString()){
            settings.setValue("MR",ui->comboISMR->currentText());
        }
        if(ui->comboISHR->currentText() != "" and
                ui->comboISHR->currentText() != settings.value("HR").toString()){
            settings.setValue("HR",ui->comboISHR->currentText());
        }
        event->accept();
        break;
    }
    default:{
        event->ignore();
        break;
    }
    }
}

/**
  * \param currentTab Tab currently selected
  */

void Application::tab(int currentTab){

    //Disable/Enable Save plot Button
    if (currentTab == ui->tabs->indexOf(ui->tabElmnt)
            or currentTab == ui->tabs->indexOf(ui->tabIS)
            or currentTab == ui->tabs->indexOf(ui->tabEvol)){
        plotAct->setEnabled(true);
    }else{
        plotAct->setEnabled(false);
    }

    //Disable/Enable Save output Button
    if (currentTab == ui->tabs->indexOf(ui->tab_output)){
        saveAct->setEnabled(true);
    }else{
        saveAct->setEnabled(false);
    }
}

void Application::on_yIntercept_clicked()
{
    ui->subBlank->setChecked(false) ;
    enableBlankSelection() ;
}

void Application::on_subBlank_clicked()
{
    ui->yIntercept->setChecked(false) ;
    enableBlankSelection() ;
}

void Application::enableBlankSelection()
{
    bool enable = true ;

    if(ui->yIntercept->isChecked() || ui->subBlank->isChecked())
    {
        enable = true ;
    }
    else
    {
        enable = false ;
    }

    ui->blankSelection->setEnabled(enable) ;
}

void Application::on_correctionIS_clicked()
{
    bool enable = true ;

    if(ui->correctionIS->isChecked())
    {
        enable = true ;
    }
    else
    {
        enable = false ;
    }

    ui->groupBox->setEnabled(enable) ;
}
