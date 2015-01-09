/* locationsview.cpp
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
#include "locationsview.h"
#include "ui_locationsview.h"
#include "locationeditdialog.h"
#include <QMessageBox>


locationsView::locationsView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::locationsView)
{
    ui->setupUi(this);
    init();
}
int locationsView::init(){

       main_model = new  QSqlTableModel(this);
           main_model->setTable("location");
           main_model->setEditStrategy(QSqlTableModel::OnFieldChange);

           main_model->setHeaderData(0, Qt::Horizontal,QVariant(QString::fromUtf8("שם עברית")));
           main_model->setHeaderData(1, Qt::Horizontal,QVariant(QString::fromUtf8("שם אנגלית")));
           main_model->setHeaderData(2, Qt::Horizontal,QVariant(QString::fromUtf8("איזור")));
           main_model->setHeaderData(3, Qt::Horizontal,QVariant(QString::fromUtf8("אורך")));
           main_model->setHeaderData(4, Qt::Horizontal,QVariant(QString::fromUtf8("רוחב")));
           main_model->setHeaderData(5, Qt::Horizontal,QVariant(QString::fromUtf8("הפרש שעות")));
           main_model->setHeaderData(6, Qt::Horizontal,QVariant(QString::fromUtf8("גובה")));
           main_model->setHeaderData(7, Qt::Horizontal,QVariant(QString::fromUtf8("זמן הדלקת נרות")));
           main_model->select();

        myTable = new myTableView(this,main_model);
        myTable->setEditTriggers(QAbstractItemView::DoubleClicked);
        myTable->setSortingEnabled(true);
        myTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        myTable->setSelectionMode(QAbstractItemView::SingleSelection);
        myTable->setShowGrid(false);
        myTable->verticalHeader()->hide();
        myTable->setAlternatingRowColors(true);
       // myTable->resizeColumnsToContents();
         myTable->setModel(main_model);
    ui->verticalLayout->addWidget(myTable);

    connect(ui->AddPushButton, SIGNAL(clicked()), this, SLOT(addNew()));
    return 0;
}

locationsView::~locationsView()
{
    delete ui;
}

  void locationsView::addNew(){
      QString hebName="" ;
      QString engName="";
      QString Region="";
      QString Longitude="";
      QString Latitude="";
      QString TimeZone="";
      QString PlaceHeight="";     
      QString Candle_Light="";

         LocationEditDialog * a =  new LocationEditDialog (&hebName ,&engName,&Region,
                                                           &Longitude,&Latitude,&TimeZone,&PlaceHeight,
                                                           &Candle_Light,this);
         a->exec();
         if (!hebName.isEmpty()||!Region.isEmpty()||!Longitude.isEmpty()||!Latitude.isEmpty()||!TimeZone.isEmpty()||!Candle_Light.isEmpty()){
             int col =0;
             int row = main_model->rowCount();
              main_model->insertRow(row);
              main_model->setData(main_model->index(row, col),hebName);
              main_model->setData(main_model->index(row, col+1),engName);
              main_model->setData(main_model->index(row, col+2),Region);
              main_model->setData(main_model->index(row, col+3),Longitude);
              main_model->setData(main_model->index(row, col+4),Latitude);
              main_model->setData(main_model->index(row, col+5),TimeZone);
              main_model->setData(main_model->index(row, col+6),PlaceHeight);              
              main_model->setData(main_model->index(row, col+7),Candle_Light);

              main_model->database().transaction();
              if (main_model->submitAll()) {
                  main_model->database().commit();
              } else {
                  main_model->database().rollback();

                  QMessageBox::warning(this, QString::fromUtf8("שגיאה"),
                                       QString::fromUtf8(" המסד מדווח על שגיאה %1")
                                       .arg(main_model->lastError().text()));
              }
         }
  }
