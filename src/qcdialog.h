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

#ifndef QCDIALOG_H
#define QCDIALOG_H

#include <QDialog>

namespace Ui {
class QCDialog;
}

class QCDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QCDialog(QStringList * listQC, int nbrQC, QWidget *parent);    ///< Constructor of the Dialog box
    ~QCDialog();    ///< Destructor of the Dialog box
    
private:
    Ui::QCDialog *ui;   ///< Interface model pointer of the dialog box
};

#endif // QCDIALOG_H
