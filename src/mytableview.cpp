/* mytableview.cpp
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
#include "mytableview.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QtWidgets>
#include "locationeditdialog.h"

myTableView::myTableView(QWidget *parent,QSqlTableModel * pmodel) :  QTableView(parent)
{
     gpmodel = pmodel;//מצביע לטבלה המקורית
}
void myTableView::contextMenuEvent ( QContextMenuEvent * e )
{
    QMenu *menu = new QMenu(this);
    QModelIndex index = indexAt(e->pos());
    if (index.isValid()){
     QAction * deleteAct = new QAction(QString::fromUtf8("מחק"), this);
    QAction * editAct = new QAction(QString::fromUtf8("ערוך"), this);

     menu->addAction(editAct);
     menu->addAction(deleteAct);
     rowI =index.row();
     connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteRow()));
     connect(editAct, SIGNAL(triggered()), this, SLOT(editRow()));
    }
    else
      menu->addAction("No item was clicked on");
    menu->exec(QCursor::pos());
}
void myTableView::deleteRow(){


    if(model()->rowCount()==1){
        QMessageBox::information(this, QString::fromUtf8("טעות"),
                            QString::fromUtf8("אי אפשר למחוק שורה אחרונה בטבלה"));
    }else{
         model()->removeRow(rowI);
      gpmodel->database().transaction();
      if (gpmodel->submitAll()) {
          gpmodel->database().commit();
      } else {
          gpmodel->database().rollback();
           QMessageBox::warning(this, QString::fromUtf8("שגיאה"),
                               QString::fromUtf8("המסד נתקל בשגיאה מסוג: %1")
                               .arg(gpmodel->lastError().text()));
      }
    }

}
void myTableView::editRow(){
    QString hebName=model()->data( model()->index( rowI , 0 ) ).toString();
    QString engName=model()->data( model()->index( rowI , 1 ) ).toString();
    QString Region=model()->data( model()->index( rowI , 2 ) ).toString();
    QString Longitude=model()->data( model()->index( rowI , 3 ) ).toString();
    QString Latitude=model()->data( model()->index( rowI , 4 ) ).toString();
    QString TimeZone=model()->data( model()->index( rowI , 5 ) ).toString();
    QString PlaceHeight=model()->data( model()->index( rowI , 6 ) ).toString();
    QString Candle_Light=model()->data( model()->index( rowI , 7 ) ).toString();

       LocationEditDialog * a =  new LocationEditDialog (&hebName ,&engName,&Region,
                                                         &Longitude,&Latitude,&TimeZone,&PlaceHeight,
                                                         &Candle_Light,this);
       a->exec();
       if (!hebName.isEmpty()||!Region.isEmpty()||!Longitude.isEmpty()||!Latitude.isEmpty()||!TimeZone.isEmpty()||!Candle_Light.isEmpty()){
           int col =0;

            model()->setData(model()->index(rowI, col),hebName);
            model()->setData(model()->index(rowI, col+1),engName);
            model()->setData(model()->index(rowI, col+2),Region);
            model()->setData(model()->index(rowI, col+3),Longitude);
            model()->setData(model()->index(rowI, col+4),Latitude);
            model()->setData(model()->index(rowI, col+5),TimeZone);
            model()->setData(model()->index(rowI, col+6),PlaceHeight);           
            model()->setData(model()->index(rowI, col+7),Candle_Light);

            gpmodel->database().transaction();
            if (gpmodel->submitAll()) {
                gpmodel->database().commit();
            } else {
               gpmodel->database().rollback();

                QMessageBox::warning(this, QString::fromUtf8("שגיאה"),
                                     QString::fromUtf8(" המסד מדווח על שגיאה %1")
                                     .arg(gpmodel->lastError().text()));
            }
       }
}
