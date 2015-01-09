/* omerdialog.cpp
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
#include "omerdialog.h"
#include "ui_omerdialog.h"
#include <QtWidgets>

omerDialog::omerDialog(int d,int no, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::omerDialog)
{
    day_in_omer=d;
    nosach =no;
    ui->setupUi(this);
    init();
}

omerDialog::~omerDialog()
{
    delete ui;
}
void omerDialog::init(){
    QString FirstBracha = QString::fromUtf8("ברוך אתה יהוה אלהינו מלך העולם,\n אשר קדשנו במצותיו וצונו על ספירת העמר:");
    QString LastBracha = QString::fromUtf8("הרחמן הוא יחזיר לנו עבודת בית המקדש \nלמקומה במהרה בימינו, אמן סלה");
    ui->LfirstBracha->setText(FirstBracha);
    ui->LlastBracha->setText(LastBracha);

    if(nosach==1||nosach==2){
      ui->BbeforeBracha->setText(QString::fromUtf8("...לשם יחוד"));
      ui->BbeforeBracha->setToolTip(QString::fromUtf8("לחץ על מנת להרחיב"));
    }else{
        ui->BbeforeBracha->hide();
    }

    ui->BafterBracha->setText(QString::fromUtf8("...למנצח"));
    ui->BafterBracha->setToolTip(QString::fromUtf8("לחץ על מנת להרחיב"));

    ui->omerString->setText(getString());
    connect(ui->BbeforeBracha, SIGNAL(clicked()), this, SLOT(show_brachot_dialog_before()));
    connect(ui->BafterBracha, SIGNAL(clicked()), this, SLOT(show_brachot_dialog_after()));
}
 QString omerDialog::getString (){
     QString firstString = getFirstString();
     if(day_in_omer<7){
         return (firstString);
     }
     QString secondString = getSecondString();
     return (firstString+"\n"+secondString);

 }
 QString omerDialog::getFirstString (){
     if (day_in_omer == 0){
         return ( QString::fromUtf8("--------"));
     }else if(day_in_omer<7){
         QString today = QString::fromUtf8("היום");
         QString num = convertNumOfOmerToString();
         QString days = dayORdays();
         QString last_word;
         if(nosach==0){
          last_word = QString::fromUtf8("בעומר");
         }
         else if (nosach==1 ){
             last_word = QString::fromUtf8("לעומר");
         }
         else if(nosach==3){
             last_word = QString::fromUtf8("לעומר");
         }
         if (day_in_omer==0){
             return (today+" "+days+" "+num+" "+last_word);
         }
         return (today+" "+num+" "+days+" "+last_word);
     }
  QString today = QString::fromUtf8("היום");
  QString num = convertNumOfOmerToString();
  QString days = dayORdays();
  if (nosach==2){
      return (today+" "+num+" "+days+" "+QString::fromUtf8("לעומר"));
  }
  return (today+" "+num+" "+days);
 }

 QString omerDialog::getSecondString (){
     QString she = QString::fromUtf8("שהם");
     QString pirut = convertNumToDays();
     QString last_word;
     if(nosach==0){
      last_word = QString::fromUtf8("בעומר");
     }
     else if (nosach==1  ){
         last_word = QString::fromUtf8("לעומר");
     }
     else if(nosach==2){
         last_word = QString::fromUtf8("");
     }


     return (she+" "+pirut+" "+last_word);
 }
 QString omerDialog::convertNumOfOmerToString()
 {
     if (day_in_omer == 0){
         return ( QString("null"));
     }
    QString num_asarot []= {QString::fromUtf8 ("עשר"),
                           QString::fromUtf8 ("עשרים"),
                           QString::fromUtf8 ("שלשים"),
                           QString::fromUtf8 ("ארבעים")};

     QString num_unit []= { QString::fromUtf8 ("אחד"),
                 QString::fromUtf8 ("שנים"),
                 QString::fromUtf8 ("שלשה"),
                 QString::fromUtf8 ("ארבעה"),
                 QString::fromUtf8 ("חמשה"),
                 QString::fromUtf8 ("ששה"),
                 QString::fromUtf8 ("שבעה"),
                 QString::fromUtf8 ("שמונה"),
                 QString::fromUtf8 ("תשעה")};
     int unit = day_in_omer % 10 ;
     int asarot = day_in_omer/ 10 ;
     if (asarot <= 0)
     {
         if (unit == 2)
         {
             return (QString::fromUtf8("שני"));
         }
         return (num_unit[day_in_omer-1]);
     }
     else{
         if (unit==0)
         {
             if (asarot==1){
             return (QString::fromUtf8("עשרה"));
             }
              return ( num_asarot[asarot-1]);
         }
         QString andS = QString::fromUtf8("ו");

         if (day_in_omer < 20){
             return (num_unit[unit-1] +" "+num_asarot[asarot-1]);
         }
         else{
         return ( num_unit[unit-1]+" "+andS+num_asarot[asarot-1]);
         }
     }

 }
 QString omerDialog::dayORdays(){
     if (day_in_omer>1 && day_in_omer<11 )
     {
         return (QString::fromUtf8("ימים"));
     }
     else{
         return (QString::fromUtf8("יום"));
     }

 }
 QString omerDialog::convertNumToDays(){
     QString weeks []= {QString::fromUtf8 ("שבוע אחד"),
                            QString::fromUtf8 ("שני שבועות"),
                            QString::fromUtf8 ("שלשה שבועות"),
                            QString::fromUtf8 ("ארבעה שבועות"),
                            QString::fromUtf8 ("חמשה שבועות"),
                            QString::fromUtf8 ("ששה שבועות"),
                             QString::fromUtf8 ("שבעה שבועות")};

      QString days []= { QString::fromUtf8 ("אחד"),
                  QString::fromUtf8 ("שני"),
                  QString::fromUtf8 ("שלשה"),
                  QString::fromUtf8 ("ארבעה"),
                  QString::fromUtf8 ("חמשה"),
                  QString::fromUtf8 ("ששה")};
      int num_days = day_in_omer % 7 ;
      int num_weeks = day_in_omer/ 7 ;
      QString andS = QString::fromUtf8("ו");
      QString yamim = QString::fromUtf8("ימים");
      if (num_days==0){
          return (weeks [num_weeks-1]);
      }
      else if (num_days==1){
          return (weeks [num_weeks-1]+"\n"+andS+(QString::fromUtf8("יום"))+" "+days [num_days-1]);
      }
      return (weeks [num_weeks-1]+"\n"+andS+days [num_days-1]+" "+yamim);

 }
 void omerDialog::show_brachot_dialog_before(){
     show_brachot_dialog(1);
 }
 void omerDialog::show_brachot_dialog_after(){
     show_brachot_dialog(2);
 }
 void omerDialog::show_brachot_dialog(int type){
     brachot b(type,nosach,day_in_omer);
     b.exec();
 }
 brachot::brachot(int show , int type, int dayInOmer)
 {

     QTextBrowser * viwer = new  QTextBrowser();
     QString text;
     QString path;
     viwer->setFrameShape(QFrame::NoFrame);
     viwer->setTextInteractionFlags(Qt::NoTextInteraction);
     switch(show){
     case 1://before bracha
         if (type==0){
             path=":/html/first_bracha_a.html";
         }
         else if (type==1){
             path=":/html/first_bracha_s.html";
         }
         else if (type==2){
             path=":/html/first_bracha_m.html";
         }
         else{
             path=":/html/first_bracha_a.html";
         }
         break;
     case 2: // after bracha
         if (type==0){
             path=":/html/last_bracha_a.html";
         }
         else if (type==1){
             path=":/html/last_bracha_s.html";
         }
         else if (type==2){
             path=":/html/last_bracha_m.html";
         }
         else{
             path=":/html/last_bracha_a.html";
         }
         break;
     }

     QString s =sfirot(dayInOmer);
     QFile doc(path);
     doc.open(QIODevice::ReadOnly);
     QTextStream in(&doc);
     in.setCodec("UTF-8");

     QString file;
     file = in.readAll();
     text= file.replace(QString("()"),s);
     viwer->setHtml(text);


     QHBoxLayout *mainLayout = new QHBoxLayout;
     mainLayout->addWidget(viwer);
     setLayout(mainLayout);
     setWindowTitle(QString::fromUtf8("ספירת העומר"));
     resize(500, 250);
 }
  QString brachot::sfirot(int DayInOmer){

      int seven = getSeven(DayInOmer)-1;
      int unit = getUnit(DayInOmer)-1;

      QString s_sefirot[7]={QString::fromUtf8("חסד")
                            ,QString::fromUtf8("גבורה")
                            ,QString::fromUtf8("תפארת")
                            ,QString::fromUtf8("נצח")
                            ,QString::fromUtf8("הוד")
                            ,QString::fromUtf8("יסוד")
                            ,QString::fromUtf8("מלכות")};
      QString sb =QString::fromUtf8("שב");
      return(s_sefirot[unit]+" "+sb+s_sefirot[seven]);
  }
  int brachot::getSeven(int DayInOmer){
      return ((int)ceil(DayInOmer/7.0));
  }
  int brachot::getUnit(int DayInOmer){
    int  tmp = DayInOmer%7;
    if (tmp==0){
        tmp=7;
    }
    return tmp;
  }
