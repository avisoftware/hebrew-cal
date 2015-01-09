/* jumptodatedialog.cpp
 * Hebrew Calendar based on libhdate http://libhdate.sourceforge.net
 *
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
#include "jumptodatedialog.h"
#include "ui_dialogjumptodate.h"
#include "hebD.h"
JumpToDateDialog::JumpToDateDialog(int *day, int *month, int *year, int * type, QWidget *parent) :
    QDialog(parent),  ui(new Ui::JumpToDateDialog)
{    
    ui->setupUi(this);
     pDay=day;
     pMonth=month;
     pYear=year;
     pType=type;
    init();
}
void JumpToDateDialog::init(){

    ui->gDateRadioButton->setChecked(true);
    ui->gDateDateEdit->setEnabled(true);

    ui->hDayComboBox->setEnabled(false);
    ui->hMontComboBox->setEnabled(false);
    ui->hYearComboBox->setEnabled(false);
    connect(ui->gDateRadioButton,SIGNAL(pressed()),this,SLOT(gPress()));
    connect(ui->hebDateRadioButton,SIGNAL(pressed()),this,SLOT(hPress()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(ok()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(cancel()));
    setDates();
}
void JumpToDateDialog::gPress(){
    ui->gDateDateEdit->setEnabled(true);
    ui->hDayComboBox->setEnabled(false);
    ui->hMontComboBox->setEnabled(false);
    ui->hYearComboBox->setEnabled(false);
}

void JumpToDateDialog::hPress(){
    ui->gDateDateEdit->setEnabled(false);
    ui->hDayComboBox->setEnabled(true);
    ui->hMontComboBox->setEnabled(true);
    ui->hYearComboBox->setEnabled(true);
}
void JumpToDateDialog::setDates(){
    QDate today = QDate::currentDate();
    ui->gDateDateEdit->setDate(today);
    setComboDate();

}
 void JumpToDateDialog::setComboDate(){
     QStringList dayL;
     dayL.clear();
     for(int i=1;i<31;i++){
         dayL.append(get_int_to_heb_string(i));
     }
     ui->hDayComboBox->clear();
     ui->hDayComboBox->addItems(dayL);

     QStringList montL;
     montL.clear();
     for(int i=1;i<15;i++){
         montL.append(hdate_get_hebrew_month_string(i,0));
     }
     ui->hMontComboBox->clear();
     ui->hMontComboBox->addItems(montL);

     QStringList yearL;
     yearL.clear();
     for(int i=5750;i<5800;i++){
         yearL.append(get_int_to_heb_string(i));
     }
     ui->hYearComboBox->clear();
     ui->hYearComboBox->addItems(yearL);

     hdate_struct today;
     hdate_set_gdate (&today, 0, 0, 0);

     ui->hDayComboBox->setCurrentIndex( today.hd_day-1);
     ui->hMontComboBox->setCurrentIndex( today.hd_mon-1);
     ui->hYearComboBox->setCurrentIndex( today.hd_year-5750);
 }
 void JumpToDateDialog::ok(){
     if(ui->gDateRadioButton->isChecked()){
         *pDay= ui->gDateDateEdit->date().day();
         *pMonth=ui->gDateDateEdit->date().month();
         *pYear=ui->gDateDateEdit->date().year();
         *pType =1;
     }else{
         *pDay= (ui->hDayComboBox->currentIndex()+1);
         *pMonth=(ui->hMontComboBox->currentIndex()+1);
         *pYear=(ui->hYearComboBox->currentIndex()+5750);
           *pType =2;
     }
     done(1);
 }

 void JumpToDateDialog::cancel(){
     *pDay=0;
     *pMonth=0;
     *pYear=0;
     *pType=0;
     done(0);
 }
