/* dialogprinttables.cpp
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
#include "dialogprinttables.h"
#include "ui_dialogprinttables.h"
#include "hebD.h"
#include "functions.h"

DialogPrintTables::DialogPrintTables(int month,int year,int is, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPrintTables)
{
    is_m=is;//1 is a mikve window
    ui->setupUi(this);
    init(month,year);
    connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(typeChange(int)));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(ok()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(cancel()));
}

DialogPrintTables::~DialogPrintTables()
{
    delete ui;
}
void DialogPrintTables::init (int m,int y){

        QStringList montL;
        montL.clear();
        for(int i=1;i<15;i++){
            montL.append(hdate_get_hebrew_month_string(i,0));
        }
        ui->comboBoxFromMonth->clear();
        ui->comboBoxFromMonth->addItems(montL);

        ui->comboBoxToMonth->clear();
        ui->comboBoxToMonth->addItems(montL);

        QStringList yearL;
        yearL.clear();
        for(int i=5750;i<5800;i++){
            yearL.append(get_int_to_heb_string(i));
        }
        ui->comboBoxYear->clear();
        ui->comboBoxYear->addItems(yearL);

        /*hdate_struct today;
        hdate_set_gdate (&today, 0, 0, 0);*/

        ui->comboBoxFromMonth->setCurrentIndex(m-1);
        ui->comboBoxToMonth->setCurrentIndex( m-1);
        ui->comboBoxYear->setCurrentIndex(y-5750);
        if(is_m==0){
            ui->comboBoxFromMonth->setEnabled(true);
             ui->comboBoxToMonth->setEnabled(true);

             ui->comboBoxType->hide();

             QLabel * l= new QLabel(QString::fromUtf8("לוח לחודש(ים), בשנת"));
             ui->horizontalLayout->insertWidget(0,l);
             ui->checkBoxIncludeEvents->hide();
        }
}
 void DialogPrintTables::typeChange(int index){
     if(index){
         ui->comboBoxFromMonth->setEnabled(true);
          ui->comboBoxToMonth->setEnabled(true);
     }else{
         ui->comboBoxFromMonth->setEnabled(false);
          ui->comboBoxToMonth->setEnabled(false);
     }
 }
 void DialogPrintTables::ok(){
         print();

 }

 void DialogPrintTables::cancel(){
     done(0);
 }

 void DialogPrintTables::doHtmlHead(int type){
     html=html+("<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.01//EN'"
           "http://www.w3.org/TR/html4/strict.dtd'>"
           "<html>"
     "<head> "
                "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>"
                " <style title='Normal' type='text/css' media='all'>");


     if (type==0){
       html= html+  (".month { "
               "padding:2px;}"
         ".night{ "
               " text-align:center; "             
               " background: rgb(213, 213, 213);font-size:6pt; "

               "}"
               ".day{ "
               " text-align:center; "
               "font-size:6pt;"
               " }"
               ".number{ "               
               "}"
               ".sunset{"
               " text-align:center;"
               "}"
               "td{"
               " padding:0px;"               
               "font-size:6pt;"
               "}"
               " .head{"
                     "padding-right:25px;"
                     "font-size:7pt;"
               "}"
               "p{-qt-block-indent:0; text-indent:0px;margin:0px;padding:0px;}"
               );
     }else if (type==1){
         html=html+(".head{ font-size:15pt;}"
                     ".out_of_month{color: #dddddd;}"
                    ".sat{background: #dddddd;}"
                    ".holiday{background: #dddddd;}"
                    "p{-qt-block-indent:0; text-indent:0px;margin:0px;padding:0px;}"
                    ".half_holiday{background-color :qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #dddddd stop:1 #ffffff)}");
                     }

     html=html + "  </style></head>";
 }


 QString DialogPrintTables::getSunset(int d ,int m ,int y){
    int sunrise;
    int sunset;
    double height=0.0;
    if(setting_LocationConsiderHeight){
        height=(double)setting_LocationHeight;
    }
     hdate_get_utc_sun_time(d,m,y,latitude,longitude,/*height,*/&sunrise,&sunset);
     int tz1=tz;
     int tsunrise = sunrise + (tz * 60);
     QTime tTime =QTime( tsunrise / 60, tsunrise % 60);
     QDateTime rDate = QDateTime(QDate(y,m,d),tTime).toUTC();
     int offsetFromUtc =tTime.hour()- rDate.time().hour();
     if(summerTime==-1){
         if (tz<offsetFromUtc&&tz==(offsetFromUtc-1)){
                  tz1+=1;//it is on summer time
         }
     }else if (summerTime==1){

         tz1+=1;//it is on summer time
     }
     sunset = sunset + tz1 * 60.0;
      int int_time;
      int_time = sunset;
      QTime sunsetTime= QTime( int_time / 60, int_time % 60);
      return(sunsetTime.toString("H:mm"));
}

void DialogPrintTables::printing_process_year(QPrinter * printer){
    QPainter painter;

    painter.begin(&*printer);
    painter.setLayoutDirection(Qt::RightToLeft);

    html.clear();
    doHtmlHead(0);
   hdate_struct h;
   int year=ui->comboBoxYear->currentIndex()+5750;
   hdate_set_hdate (&h, 1, 1, year);
   QString note_about_marks ="";




   QString copyright=note_about_marks+"<p><span style='font-size:10pt;'>"+QString(APP_NAME)+" "+QString(APP_VERSION)+" "+QString::fromUtf8("הודפס בתוכנת ")+"</span></p>";
   QString table_head= "<table dir='rtl' align='right' width='100%' border='0' cellspacing='0' cellpadding ='1' style='border-color:black;'><tr style='vertical-align: top;'>";
   QString body= "<body align='right'  font-style:normal;' >";// style='font-weight:400;
   html= html +body+table_head;
   int months=13;//if leap year
   if(is_leap_year(year)){
       months=14;
   }
   int m=0;
   //FIXME: this part is for hebrew print, in qt4 i have to print it in reverse.

   QList<int>LMonth;
   for(int i =1;i<months;i++){
       if(months ==14){
           if (i==6&&m==5){
               m=13;
           }else if(m==13){
               m=14;
           }else if (m==14){
               m=7;
           }else{
              m++; //m=i;
           }
       }else{
           m=i;
       }
      LMonth.append(m);
   }
   int step=3;
   int p=0;
   int l=0;
   for(int i=step;i>l;i--){
       if(p<LMonth.count()){
           if(html==""){
               doHtmlHead(0);
               html= html +body+table_head;
           }
           hdate_set_hdate(&h,1,LMonth.at(i-1),year);
           //fix_date(&h);
           hdate_struct h_ent_m;
           hdate_set_hdate (&h_ent_m, 30, h.hd_mon, year);
           fix_date(&h_ent_m);
           QString year_g;
           if (h.gd_year==h_ent_m.gd_year){
               year_g=QString::number(h.gd_year);
           }else{
               year_g=QString::number(h.gd_year)+"-"+QString::number(h_ent_m.gd_year);
           }


           QString new_table="";
           if((p+1)%3==1&&p>0){
               new_table="</tr></table>"+table_head;
           }else{
               new_table="";
           }


           QString width="";
           if(i==13){
               width="width=33%";
           }
           html= html + (new_table+"<td "+width+" class='month'><div dir='rtl'> "
                         " <table border='1' cellspacing='0' width=100% style='border-color:black;'>"
                         " <tr  align=center>"
                         " <td colspan='8' align='right' class='head'> " +get_hebrew_month_string(h.hd_mon,0)+" "+get_int_to_heb_string(year)+ "</td>	"
                         "</tr>"
                          "<tr>"
                         "<td align=center class='sunset'>" +QString::fromUtf8("שקיעה")+ "</td>"
                         "<td class='number'>" + "</td>	"
                         " <td align=center class='day'>" +QString::fromUtf8("יום")+ "</td>"
                         "<td  align=center class='night'> " +QString::fromUtf8("לילה")+ "</td>"
                         "<td colspan='4'  width: 40% align=center>" +
                         hdate_get_month_string(h.gd_mon,0)+" "+hdate_get_month_string(h_ent_m.gd_mon,0)
                         + " "+year_g+ "</td> "
                             "</tr>");
           for(int j= 1;j<31;j++){
               hdate_set_hdate(&h,j,LMonth.at(i-1),year);
               if(j==30){
                   if( fix_date(&h)){
                       continue;
                   }
               }
               QString night_s="";
               QString day_s="";


               int hol = hdate_get_holyday(&h, setting_diaspora);
               int hol_t= hdate_get_holyday_type(hol);
               if ((hol_t==6||hol_t==8||hol_t==9)&&!setting_israelHoliday){//do not show israeli holidays
                   hol =0;
               }
               QString holyday = hdate_get_holyday_string(hol,0);

               html= html +" <tr class='day_detail'>"
                       "<td align=center> " +getSunset(h.gd_day,h.gd_mon,h.gd_year)+ " </td>"
                       "<td> " +"&nbsp;"+ " </td>"
                       "<td align=center> " +day_s+ " </td>"
                        "<td class='night' align=center> " +night_s+ " </td>"
                       " <td align=center class='g_day_t'>" +QString::number(h.gd_day)+ "</td>"
                       " <td align=center class='h_day_t'> " +get_int_to_heb_string(h.hd_day)+ " </td>"
                       "<td align=center class='day_in_w' > " +get_day_in_week_string(h.hd_dw,0)+ "</td>"
                       " <td align=center class='holyday'>" +holyday+ "</td>"
               " </tr>";
           }
           html= html+"</table>"
                   "</div> </td>";

           if(p==5||p==11||p==12){              
               html= html +"</tr></table>";
                html= html +copyright;
                       html= html +"</body></html>";
                       qDebug()<<html;
               QTextDocument *document = new QTextDocument();
                   document->setHtml(html);
                   document->setPageSize(QSizeF(printer->pageRect().width(),printer->pageRect().height()));
                   document->drawContents(&painter);
                   if(p==11&&months==14){
                       printer->newPage();
                   }else if (p==5){
                       printer->newPage();
                   }
               html="";
           }
           p++;
           if(i==(l+1)){
               i=p+step+1;
               l=l+step;
               if(i==16){
                   i=14;
               }
           }
       }
   }
   painter.end();
}


void DialogPrintTables::printing_process_month(QPrinter *printer){
    QPainter painter;

    painter.begin(&*printer);
    painter.setLayoutDirection(Qt::LeftToRight);

    html.clear();
    doHtmlHead(1);
    int year=ui->comboBoxYear->currentIndex()+5750;
     int month_start=ui->comboBoxFromMonth->currentIndex()+1;
     int month_end=ui->comboBoxToMonth->currentIndex()+1;

     //hdate_struct h;
     //hdate_set_hdate (&h, 1, 1, year);
     int months=13;//if leap year
     int  b= between_month(month_start,month_end,is_leap_year(year));
     if(is_leap_year(year)){
         months=14;
     }
     int m=month_start;
     QList<int>LMonth;
     for(int i =0;i<b;i++){
          LMonth.append(m);
         if(months ==14){
             if (m==5){
                 m=13;
             }else if(m==13){
                 m=14;
             }else if (m==14){
                 m=7;
             }else{
                m++; //m=i;
             }
         }else{
             m++;
         }
     }
     QString copyright="<br><p><span style='font-size:10pt;'>"+QString(APP_NAME)+" "+QString(APP_VERSION)+" "+QString::fromUtf8("הודפס בתוכנת ")+"</span></p><br>";
     QString table_head= "<table dir='rtl' align='right' width='100%' border='1' cellspacing='0' cellpadding ='1' style='border-color:black;'>";
     QString body= "<body align='right'  style='font-weight:400; font-style:normal;'>";
     html= html +body;
     html= html +table_head;

     for(int i=0;i<LMonth.count();i++){
             if(html==""){
                 doHtmlHead(1);
                 html= html +body+table_head;
             }
             hdate_struct h;
             hdate_set_hdate(&h,1,LMonth.at(i),year);
             hdate_struct h_ent_m;
             hdate_set_hdate (&h_ent_m, 30, h.hd_mon, year);
             fix_date(&h_ent_m);
             QString year_g;
             if (h.gd_year==h_ent_m.gd_year){
                 year_g=QString::number(h.gd_year);
             }else{
                 year_g=QString::number(h.gd_year)+"-"+QString::number(h_ent_m.gd_year);
             }
                hdate_struct h_first;
                 int jd1  = h.hd_jd - h.hd_dw + 1;
                hdate_set_jd (&h_first, jd1);

                html= html + (" <tr  align=right>"
                           " <td colspan='7' align='center' class='head'> "+get_hebrew_month_string(h.hd_mon,0)+" "+get_int_to_heb_string(year)+"</td>	"
                           "</tr>"
                            "<tr><td colspan='7' align='center' class='head'>"+ get_month_string(h.gd_mon,0)+" "+get_month_string(h_ent_m.gd_mon,0)
                           + " "+year_g+"</td> "
                               "</tr>");
            int jd  = h.hd_jd - h.hd_dw + 1;

            QStringList Days;
            Days.append(QString::fromUtf8("ראשון"));
            Days.append(QString::fromUtf8("שני"));
            Days.append(QString::fromUtf8("שלישי"));
            Days.append(QString::fromUtf8("רביעי"));
            Days.append(QString::fromUtf8("חמישי"));
            Days.append(QString::fromUtf8("שישי"));
            Days.append(QString::fromUtf8("שבת"));
            html =html+"<tr>";
            for(int l=7;l>0;l--){
                html =html+"<td align='center' style='background:#aaaaaa; font-size:9pt;'>"+Days.at(l-1)+"</td>";
            }
             html =html+"</tr>";

             for(int j= 0;j<6;j++){
                 html= html + "<tr>";
                 for (int k=7;k>0;k--){

                      hdate_set_jd (&h, jd+(k-1));
                      int hol = hdate_get_holyday(&h, setting_diaspora);
                      int hol_t= hdate_get_holyday_type(hol);
                      if ((hol_t==6||hol_t==8||hol_t==9)&&!setting_israelHoliday){//do not show israeli holidays
                          hol =0;
                      }
                        int out=0;
                      int reading = hdate_get_parasha (&h, setting_diaspora);
                      if (h.hd_mon != LMonth.at(i)){
                          html= html + ("<td align='center' width='14%' class='out_of_month'>");
                          out=1;
                         }
                         else if (k == 7){
                           html= html + ("<td  align='center' width='14%' class='sat'>");
                         }
                         else if (hol){
                              if (hdate_get_holyday_type(hol)==1){
                                  html= html + ("<td align='center' width='14%' class='holiday'>");
                              }else{
                                  html= html + ("<td align='center' width='14%' class='half_holiday'>");
                              }
                          }
                           else{
                          html= html + ("<td align='center' width='14%' class='regular'>");
                          }
                      html= html+"<table height='100%' width='100%' border='0' cellspacing='0' cellpadding ='2'><tr>";
                      html= html + "<td height='33%' align='left'>"+QString::number(h.gd_day)+"</td>";
                            html= html +"<td align='right'>"+get_int_to_heb_string(h.hd_day)+"</td></tr>";

                                 html= html +"<tr><td>&nbsp;</td></tr>";

                            html= html +"<tr><td height='33%' valign='bottom' align='center' colspan='2'>";
                            if(hol){
                                 html= html +"<span style='font-size:7pt;'>"+get_holyday_string(hol,0)+"</span>";
                                 if(reading){
                                     html= html + "<span style='font-size:7pt;'> "+get_parasha_string (reading, 1).replace("<br>"," ")+"</span>";
                                 }
                             }else{
                                 if(reading){
                                     html= html + "<span style='font-size:7pt;'>"+get_parasha_string (reading, 1).replace("<br>"," ")+"</span>";
                                 }else{
                                     html= html +"&nbsp;";
                                 }
                             }

                            html= html + " </td></tr></table></td>";
                     // jd++;
                 }
                 jd+=7;
             }
                 html= html +"</tr></table>";
                   html= html +copyright;
                   html= html +"</body></html>";

                 QTextDocument *document = new QTextDocument();
                    qDebug()<<html;
                    document->setHtml(html);
                     document->setPageSize(QSizeF(printer->pageRect().width(),printer->pageRect().height()));
                     document->drawContents(&painter);
                     if((LMonth.count()-1)>i){
                         printer->newPage();
                     }
                 html="";
        }
     painter.end();
}
int DialogPrintTables::check_info(){
    if(ui->comboBoxType->currentIndex()==1){
        int month_start=ui->comboBoxFromMonth->currentIndex()+1;
        int month_end=ui->comboBoxToMonth->currentIndex()+1;
        int year=ui->comboBoxYear->currentIndex()+5750;

             if(is_leap_year(year)){
                 if(month_start==13||month_start==14||month_end==13||month_end==14){
                     if(month_start==13||month_start==14){
                         if(month_end<7){
                             return 1;
                         }
                     }
                         if (month_end==13||month_end==14){
                             if(month_start>5&&(month_start>month_end||month_start<13)){
                                 return 1;
                             }
                         }
                 }else{
                     if(month_end<month_start){
                         return 1;
                     }
                 }
             }else{
                 if(month_start==13||month_start==14||month_end==13||month_end==14){
                      return 1;
                 }
                 if(month_end<month_start){
                     return 1;
                 }
             }
        }
    return 0;
}

void DialogPrintTables::print(){
     if(!check_info()){
#ifndef QT_NO_PRINTER
  QPrinter printer;
   QPrintDialog *dialog = new QPrintDialog(&printer, this);
   if (dialog->exec() != QDialog::Accepted)
       return ;

   if(is_m==1){

    }else{
        printing_process_month(&printer);
   }
     done(1);
#endif
     }else{
         QMessageBox::warning(this,QString::fromUtf8("שגיאה"),QString::fromUtf8("החודשים שבחרת אינם מתאימים"));
     }


}
