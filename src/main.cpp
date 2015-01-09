/* main.cpp
 * Hebrew Calendar based on libhdate http://libhdate.sourceforge.net
 * Copyright: (c) Avi Software <avisoftware123@gmail.com> 2014
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */




#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
     Q_INIT_RESOURCE(rc);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
#ifdef Q_OS_LINUX
    a.setWindowIcon(QIcon ( "/usr/share/pixmaps/hebrew-cal_128.png"));
#endif
#ifdef Q_OS_WIN32
//add in .pro file
#endif
    return a.exec();
}
