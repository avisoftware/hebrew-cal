/* locationeditdialog.cpp
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
#include "locationeditdialog.h"
#include "ui_locationeditdialog.h"
// להוסיף אימות קלט
LocationEditDialog::LocationEditDialog(QString *hebName,QString *engName,QString *Region,
                                       QString *Longitude,QString *Latitude,QString *TimeZone,
                                       QString *PlaceHeight,QString *Candle_Light,
                                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LocationEditDialog)
{
    ui->setupUi(this);
    phebName=hebName;
    pengName=engName;
    pRegion=Region;
    pLongitude=Longitude;
    pLatitude=Latitude;
    pTimeZone=TimeZone;
    pPlaceHeight=PlaceHeight;
        pCandle_Light=Candle_Light;
     ui->hebNameLineEdit->setText(*phebName);
     ui->engNameLineEdit->setText(*pengName);
     ui->RegionLineEdit->setText(*pRegion);
     ui->LongitudeLineEdit->setText(*pLongitude);
     ui->LatitudeLineEdit->setText(*pLatitude);
     ui->TimeZoneLineEdit->setText(*pTimeZone);
     ui->HeightLineEdit->setText(*pPlaceHeight);

     QString c = *pCandle_Light;
     ui->CandelLiteComboBox->setCurrentIndex( ui->CandelLiteComboBox->findText(c));


     connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(ok()));
     connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(cancel()));

}
/*
LocationEditDialog::LocationEditDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::LocationEditDialog){
     ui->setupUi(this);

     connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(ok()));
     connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(cancel()));

}*/

LocationEditDialog::~LocationEditDialog()
{
    delete ui;
}
void LocationEditDialog::ok(){
    *phebName=ui->hebNameLineEdit->text();
    *pengName=ui->engNameLineEdit->text();
    *pRegion=ui->RegionLineEdit->text();
    *pLongitude=ui->LongitudeLineEdit->text();
    *pLatitude=ui->LatitudeLineEdit->text();
    *pTimeZone=ui->TimeZoneLineEdit->text();
    *pPlaceHeight=ui->HeightLineEdit->text();
    *pCandle_Light=ui->CandelLiteComboBox->currentText();

    done(1);
}

void LocationEditDialog::cancel(){
    *phebName="";
    *pengName="";
    *pRegion="";
    *pLongitude="";
    *pLatitude="";
    *pTimeZone="";
    *pPlaceHeight="";
    *pCandle_Light="";

    done(0);
}
