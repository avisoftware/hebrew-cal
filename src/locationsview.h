/* locationsview.h
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
#ifndef LOCATIONSVIEW_H
#define LOCATIONSVIEW_H

#include <QDialog>
#include <QtSql>
#include "mytableview.h"
namespace Ui {
class locationsView;
}

class locationsView : public QDialog
{
    Q_OBJECT
    
public:
    explicit locationsView(QWidget *parent = 0);
    ~locationsView();

private:
    Ui::locationsView *ui;

    QSqlTableModel * main_model;
     myTableView * myTable;


     int init();
private slots:
       void addNew();
};

#endif // LOCATIONSVIEW_H
