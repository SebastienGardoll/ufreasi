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

#include <QtGui/QApplication>
#include "application.h"

/*! \mainpage uFREASI Documentation
 *
 * uFREASI is an Application developed to process HR-ICP-MS Element output data
 *
 * Designed for a studies project at Telecom ParisTech in collaboration with Laboratoire de Geochimie des Eaux
 *
 * Designed by :
 * - KHELIFI Oualid - Telecom ParisTech - khelifi@enst.fr : As the initial analyst, conceptor and developer.
 * - THARAUD Mickael - Laboratoire de Geochimie des Eaux - tharaud@ipgp.fr : as the ICP-MS expert and one of the stakeholders.
 * - GARDOLL Sébastien - Telecom ParisTech - sebastien.gardoll@telecom-paristech.fr : As the current analyst, conceptor and developer.
 * - SIVRY Yann - Laboratoire de Geochimie des Eaux - sivry@ipgp.fr : as stakeholder delegate.
 */

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Application myApplication;
    myApplication.show();

    return app.exec();
}
