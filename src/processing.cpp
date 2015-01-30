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

#include "processing.h"
#include <iostream>
#include <iomanip>

bool Processing::DEBUG = true ;

Processing::Processing(){
}

/**
    * The constructor creates the lists of values corresponding to isoSize
    (Calibration lines,Calibration errors,LOQ,LOD)
    * and intializes them to 0
  */

void Processing::init(int nb_element){
    lines.clear();
    squareR.clear();
    LOQ.clear();
    LOD.clear();
    for (int i = 0 ; i < nb_element ; i++)
    {
       Regline rl ;
       rl.slope = 0. ;
       rl.slopeSdSquare = 0. ;
       rl.intercept = 0. ;
       rl.interceptSdSquare = 0. ;
       lines << rl ;
       squareR << 0;
       LOQ << 0;
       LOD << 0;
    }
}

/**
  * Substract Choosen Blank to other solutions contained in data
  * - The possibiliy of Substract/Not Substract Blank to standards
  * - Not Substract the Blank for the internal standards elements for example
  * \param data data containter where the operation of substration is applied
  * \param IdBlk Identifier of the Blank (on the solutions list) used for the substraction
  * \param subSTD Boolean to substract or not standard solutions
  * \param isoNotSub The element excluded from the substraction operation
  */

void Processing::subtructBlank(Data *data,int IdBlk,bool subSTD,QList<int> isoNotSub){

    double blancVal;
    double val;

    if (IdBlk == -1) return;

    for(int j=0;j<data->isoSize();j++){

        // Not Substract indicated elements
        if(isoNotSub.indexOf(j) != -1) continue;

        blancVal = data->getSolution(IdBlk).getCps(j);
        for(int i=0;i<data->solutionSize();i++){
            // Not Apply Susbtraction to STD
            if(subSTD == false){
                if (data->getSolution(i).getType() == Solution::STD) continue;
            }

            val = data->getSolution(i).getCps(j);

            if(val==0) continue;

            //Substract Blank
            data->getSolution(i).setCps(j,val-blancVal);
        }
    }
}

/**
  * Correction with Choosen internal standard element to be applied to other elements contained in data
  * - The correction value is taken on a choosen solution
  * - The intrnal standard resolution is tested to apply correction only to the same resolution elements
  * \param data data containter where the operation of substration is applied
  * \param IdEchant Identifier of the solution where internal standard value is taken
  * \param Id Identifier of the internal standard element
  * \param resol The elements resolution impacted by the correction
  */

void Processing::correctionIS(Data *data,int IdEchant,int Id,Element::Resolution resol){

    double alpha=1;
    double val;
    
    if(IdEchant == -1 or Id == -1) return;

    for(int i=0;i<data->solutionSize();i++){

        //Avoid division by 0
        if(data->getSolution(i).getCps(Id)==0) continue;
        
        //Correction Coefficient
        alpha = data->getSolution(IdEchant).getCps(Id)/data->getSolution(i).getCps(Id);

        for(int j=0;j<data->isoSize();j++){
            if(data->getIso(j).getResol() == resol){

                val = data->getSolution(i).getCps(j);

                if(val==0) continue;

                //Correct with the coefficient alpha
                data->getSolution(i).setCps(j,val*alpha);
            }
        }
    }
}

/**
  * \param data data containter
  * \param notEtal Elements for which there isn't calibration compute (typically internal standards)
  */

void Processing::calibration(Data *data,QList<int> notEtal){
    for(int i=0;i<data->isoSize();i++){
        if(notEtal.indexOf(i) != -1) continue;
        computeLine(data,i);
    }
}

/**
  * \param data data containter target of the computation
  */

// id == -1 means disable option.
void Processing::computeConcent(Data *outputData, Data* inputData, int blank_id, int is_ref_id, int* is_elements)
{
   qreal im, di, conc, rsdconc, slope_rsd_s ;
   qreal is_sol_rsd_s [3] ;
   qreal is_ref_rsd_s [3] ;
   qreal alpha [3] ;
   
   Element::Resolution resol ;
   
   for(int i=0;i<inputData->solutionSize();i++)
   {
      Solution& solInput = inputData->getSolution(i) ;
      Solution& solOutput = outputData->getSolution(i) ;

      if(solOutput.getType() != Solution::STD)
      {
         if(is_ref_id != -1) // Internal standard correction factors are computed.
         {
            Solution& is = inputData->getSolution(is_ref_id) ;
            
            // can be optimized.
            int is_element_id ;
            for(int i = 0 ; i < 3 ; i++)
            {
               is_element_id = is_elements[i] ;
               
               if(is_element_id != -1)
               {
                  is_sol_rsd_s[i] = computeIsRSD_s(solInput.getCpsSD(is_element_id),
                                                   solInput.getCps(is_element_id)) ;
                  is_ref_rsd_s[i] = computeIsRSD_s(is.getCpsSD(is_element_id),
                                                   is.getCps(is_element_id)) ;
                  alpha[i] = computeAlpha(is.getCps(is_element_id), solInput.getCps(is_element_id)) ;
               }
               else // not necessary but just in case.
               {
                  is_sol_rsd_s[i] = 0. ;
                  is_ref_rsd_s[i] = 0. ;
                  alpha[i] = 1. ;
               }             
            }
         }
         
         for(int j=0;j<inputData->isoSize();j++)
         {
            if (lines.at(j).slope <= Data::ZERO) continue ;
          
            resol = outputData->getIso(j).getResol() ;
            
            // im and di computation.
            im = solInput.getCps(j) ;
            di = solInput.getCpsSD(j) ;
          
            // compute concentration.
            conc = (solOutput.getCps(j) - lines.at(j).intercept)/lines.at(j).slope ;
            solOutput.setConcent(j, conc) ;
            
            slope_rsd_s = computeSlopeRsdS(lines[j]) ;
            
            // compute concentration error.
            if(blank_id != -1) // blank cases.
            {
               // b and db computation.
               qreal b, db ;
               Solution& blk = inputData->getSolution(blank_id) ;
     
               b = blk.getCps(j) ;
               db = blk.getCpsSD(j) ;
     
               if(is_ref_id != -1 && is_elements[resol] != -1)
               {
                  rsdconc = concentRSD_blank_IS_s(im, di, lines[j], b, db, is_sol_rsd_s[resol],
                                                  is_ref_rsd_s[resol], alpha[resol]) ;
               }
               else
               {
                  rsdconc = concentRSD_blank_s (im, di, lines[j], b, db) ;
               }
            }
            else // y-intercept or no option cases 
            {
               if(is_ref_id != -1 && is_elements[resol] != -1)
               {
                  rsdconc = concentRSD_y_intercept_IS_s(im, di, lines[j], is_sol_rsd_s[resol],
                                                      is_ref_rsd_s[resol], alpha[resol]) ;
               }
               else
               {
                  rsdconc = concentRSD_y_intercept_s(im, di,lines[j]) ;
               }
            }
            
            rsdconc = rsdconc + slope_rsd_s ;
  
            rsdconc = qSqrt(rsdconc) * 100. ;
            solOutput.setConcentRSD(j, rsdconc);
          }
       }
    }
}

qreal Processing::computeSlopeRsdS(const Regline& rl)
{
   qreal result ;
   
   result = rl.slopeSdSquare / (rl.slope * rl.slope) ;
   
   return result ;
}

qreal Processing::computeAlpha(double is_ref, double is_sol)
{
   qreal result ;
   
   result = is_ref / is_sol ;
   
   return result ;
}

qreal Processing::computeIsRSD_s(double dis, double is)
{
   qreal result ;
   
   result = dis/is ;
   
   result = result * result ;
   
   return result ;
}

qreal Processing::concentRSD_y_intercept_s(double im, double di, const Regline& rl)
{
  qreal result ;
  
  result = (di * di + rl.interceptSdSquare)
            /
            ((im - rl.intercept) * (im - rl.intercept)) ;
            
  return result ;
}

qreal Processing::concentRSD_blank_s (double im, double di, const Regline& rl,
                                      double b, double db)
{
   qreal result ;
   
   result = ((di * di) + (db * db) + rl.interceptSdSquare)
            /
            ((im - b - rl.intercept) * (im - b - rl.intercept)) ;
            
   return result ;
}

qreal Processing::concentRSD_y_intercept_IS_s(double im, double di, const Regline& rl,
                                              double is_sol_rsd_s, double is_ref_rsd_s,
                                              double alpha)
{
   qreal numerator, im_alpha, denumerator ;
   
   im_alpha = (im * alpha) ;
   
   numerator = di / im ;
   numerator = numerator * numerator ;
   numerator = (numerator + is_sol_rsd_s + is_ref_rsd_s) * im_alpha * im_alpha + rl.interceptSdSquare ;
   
   denumerator = (im_alpha - rl.intercept) * (im_alpha - rl.intercept) ;
   
   return (numerator/denumerator)  ;
}


qreal Processing::concentRSD_blank_IS_s(double im, double di, const Regline& rl,
                                        double b, double db,
                                        double is_sol_rsd_s, double is_ref_rsd_s,
                                        double alpha)
{
   qreal numerator, i_m_alpha, denumerator ;
   
   i_m_alpha = (im - b) * alpha ;
   
   numerator = (di * di + db *db)
               /
               ((im - b) * (im - b));
   
   numerator = (numerator + is_sol_rsd_s + is_ref_rsd_s) * i_m_alpha * i_m_alpha + rl.interceptSdSquare ;
   
   denumerator = (i_m_alpha - rl.intercept) * (i_m_alpha - rl.intercept) ;
   
   return (numerator/denumerator) ;
}

/**
  * \param input data containter before processing
  * \param output data containter after processing
  * \return Error code and a list of all the quality controls that didn't pass
  */

QPair<int,QStringList> Processing::passQC(Data *input,Data *output){
    QPair<int,QStringList> retour;
    retour.first = 0;
    retour.second.clear();

    for(int i=0;i<input->solutionSize();i++){
        if(input->getSolution(i).getType() == Solution::QC){
            for(int j=0;j<input->isoSize();j++){

                double val1 = input->getSolution(i).getConcent(j);
                double err1 = input->getSolution(i).getConcentRSD(j)*val1/100;

                double val2 = output->getSolution(i).getConcent(j);
                double err2 = output->getSolution(i).getConcentRSD(j)*val2/100;

                if(val1==0 || val2==0) continue;

                double val11 = val1-err1;
                double val12 = val1+err1;
                double val21 = val2-err2;
                double val22 = val2+err2;

                bool QCcondition = (val11 >= val21 && val11 <= val22) ||
                        (val12 >= val21  && val12 <= val22) ||
                         (val22 <= val12 && val21 >= val11);

                if(! QCcondition){
                    retour.first = 1;
                    retour.second <<  QString("%1 : %2").arg(input->getSolution(i).getName(),
                                                                    input->getIso(j).getName());
                }
            }
        }
    }

    return(retour);
}

/**
  * \param data data containter used to compute calibration line for the given element
  * \param Id Identifier of the element
  */

void Processing::computeLine(Data *data,int Id){
    QVector<double> x; // Standard concentrations.
    QVector<double> y; // Standard intensities.
    
    for(int i=0;i<data->solutionSize();i++){
       
       if(data->getSolution(i).getType()==Solution::STD)
       {
            if(  (data->getSolution(i).getConcent(Id) ==0. ||
                  data->getSolution(i).getCps(Id)==0. 
                 )
               &&
                 data->getSolution(i).getName() != Solution::ZERO_STD_NAME
              )
            {
               continue ;
            }

            x.push_back(data->getSolution(i).getConcent(Id));
            y.push_back(data->getSolution(i).getCps(Id));
        }
    }

    //Linear regression approximation
    double moyX,moyY,S1,S2, SD2 ;

    moyX = avr(x);
    moyY = avr(y);

    S1=0.;
    S2=0.;

    for(int i=0;i<x.size();i++){
        S1=S1+(x[i]-moyX)*(y[i]-moyY);
        S2=S2+(x[i]-moyX)*(x[i]-moyX);
    }

    if(!isValid(S1/S2)) return;

    lines[Id].slope = S1/S2;
    lines[Id].intercept = moyY-(lines[Id].slope)*moyX;
    
    // Compute R² of the Linear regression
    S1=0.;
    SD2 = S2 ;
    S2=0.;

    for(int i=0;i<x.size();i++)
    {
        S1=S1+(y[i]-moyY)*(y[i]-moyY);
        double val = lines[Id].slope*x[i]+lines[Id].intercept;
        S2=S2+(y[i]-val)*(y[i]-val);
    }

    if(!isValid(S2/S1)) return;

    squareR[Id]=(1.-S2/S1);

    // square of slope and y intercept standard deviation calculations:
    
    double facto = (S1 - lines[Id].slope * lines[Id].slope * SD2)
                    /
                    (x.size() - 2.)  ;
    
    lines[Id].slopeSdSquare = facto / SD2 ;
    
    lines[Id].interceptSdSquare = facto * (1./x.size() + moyX * moyX / SD2) ;
    
    if(Processing::DEBUG)
    {
      Element el = data->getIso(Id) ;
      cout << setiosflags(ios::fixed) << std::setprecision(8) << el.getName().toStdString() << ": "
           << "slope = " << lines[Id].slope << ", y-intercept =  " << lines[Id].intercept
           << ", r2 = " << squareR[Id] << endl ;
    }
}

/**
  * \param data data containter used for the computation
  * \param IdBlanc Blanks list used to comput LOD and LOQ, here
  * - LOD = 3 * standard deviation of the blanks list
  * - LOQ = 10 * standard deviation of the blanks list
  */

void Processing::computeLimits(Data *data,QList<int> IdBlanc){

    if (IdBlanc.size() == 0) return;

    for(int j=0;j<data->isoSize();j++){
        QVector<double> x;
        for(int i=0;i<IdBlanc.size();i++){
            x.push_back(data->getSolution(IdBlanc.at(i)).getConcent(j));
        }

        double S=0,moyX=0;
        moyX=avr(x);
        for(int i=0;i<IdBlanc.size();i++){
            S=S+(x[i]-moyX)*(x[i]-moyX);
        }
        S=sqrt(S/IdBlanc.size());

        LOD[j]=3*S;
        LOQ[j]=10*S;
    }
}

/**
  * \param Id Identifier of the line corresponding to the id of the element
  * \return the calibration line
  */

Regline Processing::getLine(int Id){
    return lines[Id];
}

/**
  * \param Id Identifier of the line corresponding to the id of the element
  * \return Coefficient of determination of the calibration line
  */

double Processing::getSquareR(int Id){
    return squareR[Id];
}

/**
  * \param Id Identifier of the line corresponding to the id of the element
  * \return Limit of detection for the element
  */

double Processing::getLOD(int Id){
    return LOD[Id];
}

/**
  * \param Id Identifier of the line corresponding to the id of the element
  * \return Limit of quantification for the element
  */

double Processing::getLOQ(int Id){
    return LOQ[Id];
}

QPair<double,double> Processing::stat(QVector<double> vector) {
   QPair<double,double> result ;
   
   // Average
   result.first = avr(vector) ;
   result.second = .0 ;
   
   for(int i=0;i<vector.size();i++){
            result.second = result.second + (vector[i] - result.first)*(vector[i] - result.first);
        }
        
   // Standard deviation
   result.second=sqrt(result.second/vector.size());
   
   return result ;
}

/**
  * \param vector Input vector
  * \return Average of vector values
  */

double Processing::avr(QVector<double> vector){
    double S=.0;

    for(int i=0;i<vector.size();i++){
        S=S+vector[i];
    }

    return S/vector.size();
}

/**
  * \param vector Input vector
  * \return Max of vector values
  */

double Processing::max(QVector<double> vector){
    double val=0.;

    for(int i=0;i<vector.size();i++){
        if (vector[i]>val){
            val = vector[i];
        }
    }
    return val;
}

/**
  * \param vector Input vector
  * \return Min of vector values
  */

double Processing::min(QVector<double> vector){
    if(vector.size() == 0) return 0;

    double val=vector[0];

    for(int i=0;i<vector.size();i++){
        if (vector[i]<val){
            val = vector[i];
        }
    }
    return val;
}

/**
  * \param value Input value
  * \return Boolean taking false if the value is not a number (nan) or infinite (inf)
  */

bool Processing::isValid(double value){
    return ((!isnan(value)) and (!isinf(value)));
}
