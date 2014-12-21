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

#include "parser.h"

/**
  * \param file Input file that contains input data comming from ICP-MS : file to be read
  * \param data Container of the data set
  * \param process Container of processed data
  * \return Error code and a return Message to be displayed
  */

const QString Parser::ID_BLK("BLK_") ;
const QString Parser::ID_STD("STD_") ;
const QString Parser::ID_QC("QC_");

QPair<int, QString> ParserInAgilentCSV::parse(QFile * file,Data * data, Processing *process)
{
    // Avoid "unused parameter" warning
    process->DEBUG = process->DEBUG ;

    // Patterns

    QString namePattern("[A-Z][a-z]?");
    QString massPattern("/ \\d{1,3}");
    QString pulsePattern("\\d+.\\d+");
    QString rsdPattern(pulsePattern + "%") ;
    QString undefinedPattern("<###> ");
    QString resolutionPattern("\\[#\\d\\]");
    QString smpTag("^Sample:") ;
    QString eltPattern(namePattern + " " + massPattern + " " + resolutionPattern + ";");

    // Regular expressions

    QRegExp eltRegEx(eltPattern) ;
    QRegExp sampleRegEx(smpTag) ;

    QPair<int,QString> result ;
    QTextStream in(file);
    QStringList solutionNames ;
    QStringList solutionTokens ;
    QString line ;
    QRegExp reg;
    int nbElt = 0 ;

    // Parse solution names

    while(! in.atEnd())
    {
        line = in.readLine() ;
        if(line.contains(sampleRegEx))
        {
            solutionNames = line.split(';');
            solutionNames.removeAll("");
            solutionNames.removeAt(0); // Discard smpTag
            break ;
        }
    }

    //Count Elements Number

    while (!in.atEnd())
    {
        line = in.readLine();
        if (line.contains(eltRegEx))
        {
            nbElt++;
        }
    }

    if(solutionNames.isEmpty())
    {
        result.first=1;
        result.second="Input File Format Error : No Solution Detected";
        return  result;
    }

    //No elements detected

    if (nbElt==0){
        result.first=1;
        result.second="Input File Format Error : No Element Detected";
        return  result;
    }

    for(int i = 0 ; i < solutionNames.size() ; i++)
    {
        //Add Solutions with apropriate type
        Solution::Type type;

        if(solutionNames.at(i).contains(QRegExp(ID_BLK)))
        {
            type = Solution::BLK;
        }
        else if(solutionNames.at(i).contains(QRegExp(ID_STD)))
        {
            type = Solution::STD;
        }
        else if(solutionNames.at(i).contains(QRegExp(ID_QC)))
        {
            type = Solution::QC;
        }
        else
        {
            type = Solution::SMP;
        }

        data->addSolution(Solution(solutionNames.at(i),
                                   nbElt,
                                   type));
    }

    //Parse Solutions

    in.seek(0); // Rewind

    while(! in.atEnd())
    {
        int tokenIndex = 1 ;
        int eltId = -1 ;
        int solutionId = 0 ;
        QString eltName ;
        QString tmp ;
        int eltMass = -1 ;
        double pulseValue = -1. ;
        double rsdValue = -1. ;
        Solution* currentSolution = NULL ;

        line = in.readLine() ;

        if(line.contains(eltRegEx))
        {
            solutionTokens = line.split(';');
            solutionTokens.removeAll("");

            //Element Name
            reg.setPattern(namePattern);
            if (reg.indexIn(solutionTokens.at(0)) != -1)
            {
                eltName = reg.cap();
            }

            //Element Mass
            reg.setPattern(massPattern);
            if (reg.indexIn(solutionTokens.at(0)) != -1)
            {
                tmp = reg.cap() ;
                reg.setPattern("\\d{1,3}");
                reg.indexIn(tmp) ;
                eltMass = reg.cap().toInt();
            }

            // Create element
            eltId = data->addIso(Element(eltName, eltMass, Element::LR)) ;

            // Check values for each solution
            int nbValues =0;
            for(tokenIndex=1 ; tokenIndex<solutionTokens.size(); tokenIndex++)
            {
                reg.setPattern("(" + pulsePattern + ")|(" +
                               undefinedPattern + ")|(" +
                               rsdPattern +")") ;

                if(solutionTokens.at(tokenIndex).contains(reg))
                {
                    nbValues++;
                }
                else
                {
                    result.first=1;
                    result.second = "Input File Format Error: unrecognized pulse or rsd value for element \'" +
                                    data->getIso(eltId).getName() + "\' in solution \'" +
                                    data->getSolution(solutionId).getName() ;
                    return result;
                }
            }

            // Check if the number of values is compatible with number of Solutions
            if (nbValues != (2*solutionNames.size()))
            {
                result.first=1;
                result.second = "Input File Format Error:\n\nFor the Element " +
                                data->getIso(eltId).getName() +
                                " Values Number detected do not match with Solutions number";
                return result;
            }

            // Parse solution data
            tokenIndex = 1 ;
            while(tokenIndex < solutionTokens.size())
            {
                // Parse pulse value.
                pulseValue = solutionTokens.at(tokenIndex).toDouble() ;
                tokenIndex++;

                // Parse rsd value.

                // Remove trailing %
                solutionTokens[tokenIndex].remove(QChar('%'), Qt::CaseInsensitive);

                // Compute rsdValue
                rsdValue = solutionTokens.at(tokenIndex).toDouble() * pulseValue / 100. ;

                // Set solution's values
                currentSolution = &data->getSolution(solutionId) ;
                currentSolution->setCps(eltId, pulseValue);
                currentSolution->setCpsSD(eltId, rsdValue);

                tokenIndex++;
                solutionId++;
            }
        }
    }

    //Return Message
    result.first=0;
    result.second = "Parsed Elements "
                    + QString::number(nbElt)
                    + "\n\n"
                    + "Parsed Solutions "
                    + QString::number(solutionNames.size());
    return result ;
}

QPair<int, QString> ParserInHRElementCSV::parse(QFile * file,Data * data, Processing *process) {

    // Avoid "unused parameter" warning
    process->DEBUG = process->DEBUG ;

    //Parsing Strings **********************************************

    QString nameElt("[A-Z][a-z]?");
    QString massElt("\\d{1,3}");
    QString LR("LR");
    QString MR("MR");
    QString HR("HR");
    QString resolElt("("+LR+"|"+MR+"|"+HR+")");
    QString valElt("\\d+.\\d+");
    QString valEltInd("not def.");

    QString elementPathern(nameElt+massElt+"\\("+resolElt+"\\);");

    //**************************************************************

    //Return value
    QPair<int,QString> retour;
    QString retourStr;

    //The first line : Solutions Identification
    //The first column : Elements Identification

    QTextStream in(file);
    QStringList echant;
    QStringList isot;
    QString read;

    QRegExp elementReg(elementPathern);

    //Count Elements Number

    int counterElement=0;
    while (!in.atEnd()){
        read = in.readLine();
        if (read.contains(elementReg)){
            counterElement++;
        }
    }

    //No elements detected

    if (counterElement==0){
        retour.first=1;
        retour.second="Input File Format Error : No Elements Detected";
        return  retour;
    }

    //Parsing Solutions

    in.seek(0);
    echant = in.readLine().split(';');
    echant.removeAll("");

    int counterEchant=0;
    for(int j=0;j<echant.size();j++){

        //Add Solutions with apropriate type
        Solution::Type type;

        if(echant.at(j).contains(QRegExp(ID_BLK))){
            type = Solution::BLK;
        }else if(echant.at(j).contains(QRegExp(ID_STD))){
            type = Solution::STD;
        }else if(echant.at(j).contains(QRegExp(ID_QC))){
            type = Solution::QC;
        }else{
            type = Solution::SMP;
        }

        counterEchant++;
        data->addSolution(Solution(echant[j],counterElement,type));
    }

    //No solutions detected
    if(counterEchant==0){
        retour.first=1;
        retour.second="Input File Format Error : No Solutions Detected";
        return  retour;
    }

    //Parsing Elements identification
    //and Intensities

    int count =0;

    while (!in.atEnd()){
        //Read current row
        read = in.readLine();
        if (read.contains(elementReg)){
            isot = read.split(';');
            isot.removeAll("");

            //Detect elements informations
            QString name("");
            int masse=0;
            Element::Resolution resol=Element::LR;

            QRegExp reg;

            //Element Name
            reg.setPattern(nameElt);
            if (reg.indexIn(isot[0]) != -1){
                name = reg.cap();
            }

            //Element Mass
            reg.setPattern(massElt);
            if (reg.indexIn(isot[0]) != -1){
                masse = reg.cap().toInt();
            }

            //Element Resolution
            reg.setPattern(resolElt);
            if (reg.indexIn(isot[0]) != -1){
                if(reg.cap() == LR) resol = Element::LR;
                if(reg.cap() == MR) resol = Element::MR;
                if(reg.cap() == HR) resol = Element::HR;
            }

            //Add element
            int numIso = data->addIso(Element(name,masse,resol));

            //Count number of values for each element
            int counter =0;
            for(int j=1;j<isot.size();j++){
                if(isot.at(j).contains(QRegExp("("+valElt+")|("+valEltInd+")"))){
                    counter++;
                }
            }

            //Verifiy if the number of values is compatible with number of Solutions
            if (counter != (2*counterEchant)){
                retour.first=1;
                retourStr = "Input File Format Error :\n\nFor the Element "
                        + data->getIso(numIso).getName()
                        + " Values Number detected do not match with Solutions number";
                retour.second= retourStr;
                return  retour;
            }

            bool isConcent = true;
            count =0;

            //introduce pulses ans RSD pulses
            for(int j=1;j<isot.size();j++){
                if(!isot.at(j).contains(QRegExp("("+valElt+")|("+valEltInd+")"))){
                    continue;
                }

                if (isConcent){
                    data->getSolution(count).setCps(numIso,isot.at(j).toDouble());
                    isConcent=false;
                }else
                {
                   double sd = isot.at(j).toDouble() * data->getSolution(count).getCps(numIso)/ 100. ;
                   data->getSolution(count).setCpsSD(numIso,sd);
                    isConcent=true;
                    count++;
                }
            }
        }
    }

    //Return Message
    retour.first=0;
    retourStr = "Parsed Elements "
            + QString::number(counterElement)
            + "\n\n"
            + "Parsed Solutions "
            + QString::number(count);
    retour.second= retourStr;
    return retour;
}

/**
  * \param file Output file that contains processed data : file to be written
  * \param data Container of the data set
  * \param process Container of processed data
  * \return Error code and a return Message to be displayed
  */

QPair<int, QString> ParserOutCSV::parse(QFile * file,Data * data,Processing *process) {
    QTextStream out(file);

    //1st Line : Elements Name
    out << ";;";

    for(int i=0;i<data->isoSize();i++){
        out << data->getIso(i).getName() << ";;;";
    }

    out << "\n\n";
    out << ";LOD (ppb) = ;";

    for(int i=0;i<data->isoSize();i++){
        out << process->getLOD(i) << ";;;";
    }

    out << "\n";
    out << ";LOQ (ppb) = ;";

    for(int i=0;i<data->isoSize();i++){
        out << process->getLOQ(i) << ";;;";
    }

    out << "\n\n";
    out << ";;";

    for(int i=0;i<data->isoSize();i++){
        out <<  "Concentration (ppb);SD (ppb);RSD (%);";
    }

    out << "\n\n";

    //Echant lines with concentrations
    for(int j=0;j<data->solutionSize();j++){
        out << data->getSolution(j).getName() << ";;";
        for(int i=0;i<data->isoSize();i++){
            if(data->getSolution(j).getConcent(i)==0){
                out << ";;;";
            }else if(data->getSolution(j).getConcent(i) < process->getLOQ(i)){
                out << "<LOQ";
                out << ";;;";
            }else{
                out << data->getSolution(j).getConcent(i) << ";";
                out << data->getSolution(j).getConcentRSD(i)*
                        data->getSolution(j).getConcent(i)/100 << ";";
                out << data->getSolution(j).getConcentRSD(i) << ";";
            }
        }
        out << "\n";
    }

    //Return Message
    QPair<int,QString> retour;
    retour.first=0;
    retour.second="Done";

    return retour;
}

/**
  * \param file Input file that contains STD and QC concentrations : file to be read
  * \param data Container of the data set
  * \param process Container of processed data
  * \return Error code and a return Message to be displayed
  */

QPair<int, QString> ParserInSTDQC::parse(QFile * file,Data * data,Processing *process) {

    // Avoid "unused parameter" warning
    process->DEBUG = process->DEBUG ;

    // Parsing Strings **********************************************

    QString elementReg("[A-Z][a-z]?;");

    // **************************************************************

    QTextStream in(file);
    QStringList echant;

    echant = in.readLine().split(';');

    QList<QStringList> isot;
    QString read;

    //Read file contain and split it
    while (!in.atEnd()){
        read = in.readLine();
        if (read.contains(QRegExp(elementReg))){
            isot << read.split(";");
        }
    }

    int column;
    int line;

    //For each solution
    for(int i=0;i<data->solutionSize();i++){
        //Introduce concentration only for STD and QC Solutions
        if(data->getSolution(i).getType() != Solution::STD and
                data->getSolution(i).getType() != Solution::QC){
            continue;
        }

        column = -1;

        //Search the appropriate STD or QC
        for(int k=0;k<echant.size();k++){
            if(data->getSolution(i).getName().contains(QRegExp("^"+echant.at(k)+"(_?\\d+)?$"))){
                column = k;
            }
        }

        //If not found : return message
        if(column == -1){
            QPair<int,QString> retour;
            retour.first=1;
            retour.second="Solution "+data->getSolution(i).getName()+ " Not found";
            return retour;
        }

        //For all elements
        for(int j=0;j<data->isoSize();j++){
            line = -1;
            //Detect the row of the element
            for(int p=0;p<isot.size();p++){
                if(isot.at(p).at(0) == data->getIso(j).getElement()){
                    line = p;
                    break;
                }
            }

            //If not detected

            if (line == -1) continue;


            //Introduce concetration
            QString value = isot.at(line).value(column);
            data->getSolution(i).setConcent(j,value.toDouble());

            QString valueSD = isot.at(line).value(column+1);
            data->getSolution(i).setConcentRSD(j,valueSD.toDouble());
        }
    }

    //Return message
    QPair<int,QString> retour;
    retour.first=0;
    retour.second="Concentrations Introduced";

    return retour;
}
