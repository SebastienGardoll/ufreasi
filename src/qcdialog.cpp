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

#include "qcdialog.h"
#include "ui_qcdialog.h"

/**
* the constructor displays a list of the QC given in listQC

* and the total number of QC
* \param listQC Pointer to the list of QC that not passed
* \param nbrQC Total number of QC
* \param parent Parent widget that called this dialog  box
*/

QCDialog::QCDialog(QStringList *listQC,int nbrQC, QWidget *parent) : QDialog(parent), ui(new Ui::QCDialog){
    ui->setupUi(this);

    ui->qcList->addItems(*listQC);
    ui->qcList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->labelQC->setText(QString("Quality Control <b>Not valid</b> : "
                                 + QString::number(listQC->size()) + " / " + QString::number(nbrQC)));
}

QCDialog::~QCDialog(){
    delete ui;
}
