/* mainwindow.cpp
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

#define DATA_BASE_LOCATION "/.config/avisoftware/hebrew-cal"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calWidget/calwidget.h"
#include "hebD.h"
#include "hebtimes.h"
#include "jumptodatedialog.h"
#include "settingsdialog.h"
#include "omerdialog.h"
#include "settingsfunc.h"
#include "functions.h"
#include "noteplugin.h"
#include "dialogprinttables.h"
#include "ui_about.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();
        note = new NotePlugIn;
        note->init();
        isActionEditNote =false;
        isActionAddNote =false;
        isMenuNotes=false;
        hasNote =false;
    init();
     ui->label_for_print->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

 void MainWindow::init(){
     connect(ui->prevMonthButton,SIGNAL(clicked()),this,SLOT(prevMonth()));
     connect(ui->prevYearButton,SIGNAL(clicked()),this,SLOT(prevYear()));
     connect(ui->nextMonthButton,SIGNAL(clicked()),this,SLOT(nextMonth()));
     connect(ui->nextYearButton,SIGNAL(clicked()),this,SLOT(nextYear()));

     connect(ui->sefiratHaomerB, SIGNAL(clicked()), this, SLOT(showDialogOmer()));

     selectMapper= new QSignalMapper(this);
                    connect(selectMapper, SIGNAL(mapped(int)),
                                 this, SIGNAL(selected(int)));
                    connect(this, SIGNAL(selected(int)),
                                 this, SLOT(select(int)));

    connect(ui->actionJumpToDate, SIGNAL(triggered()), this, SLOT(showDialogJumpToDate()));
    connect(ui->action_settings, SIGNAL(triggered()), this, SLOT(showDialogSettings()));
    connect(ui->action_print_screen, SIGNAL(triggered()), this, SLOT(print_screen()));
    connect(ui->action_print_tables, SIGNAL(triggered()), this, SLOT(print_tables()));
    connect(ui->aboutAction, SIGNAL(triggered()), this, SLOT(aboutA()));

    printTableHeader();
    draw(0);

 }
 void MainWindow::draw(int now){
    this->setWindowTitle(QString(APP_NAME)+" "+QString(APP_VERSION));
     setStyleSheet("QMainWindow{border-radius: 5px;background-color:'"+setting_MainBackgroundColor+"';} QLabel{color:'"+setting_MainTextColor+"';}");
     ui->frame_2->setStyleSheet("QFrame#frame_2 {border: 1px solid #555;border-radius: 5px;background-color:'"+setting_MainGridColor+"';}");

      ui->locationL->setText(QString::fromUtf8("מיקום: ")+setting_LocationHebName);

      if(setting_calby){
         if(now){
             printMonth(selectedDate.month(),selectedDate.year(),selectedDate.day());
         }else{
             printMonth(QDate::currentDate().month(),QDate::currentDate().year(),0);
         }
      }else{
         hdate_struct h;
         if(now){
             hdate_set_gdate (&h, selectedDate.day(), selectedDate.month(),selectedDate.year());
              printMonth(h.hd_mon,h.hd_year,h.hd_day);
         }else{
             hdate_set_gdate (&h, 0, 0, 0);
             printMonth(h.hd_mon,h.hd_year,0);
         }
      }
 }

int MainWindow::printMonth(int month, int year,int selection){
    widgetID =0;
    currentId=-1;
     hdate_struct h;
     hdate_struct today;
         int jd;
         int jd_today;
         int i, j;
         int count_days =0;
         if(setting_calby){
              hdate_set_gdate (&h, 1, month, year);
         }else{
             hdate_set_hdate (&h, 1, month, year);
         }

         int first_year=h.gd_year;
         int first_month=h.gd_mon;
         if(setting_note_enable){
             note->findNotesForMonth(first_year,first_month,h.gd_day);//init the note for this month//past the first day
           }
         /* get date of month start */
         hdate_set_gdate (&today, 0, 0, 0);
         jd_today = today.hd_jd;
         jd = h.hd_jd - h.hd_dw + 1;
         for (i = 0; i < 6; i++)
             {
                for (j = 0; j < 7; j++)
                 {
                     /* Get this day hebrew date */
                     hdate_set_jd (&h, jd);

                          int print=0 ;
                         if ( setting_calby==0 && h.hd_mon == month)
                         {
                             print =1;
                         }else if(setting_calby==1 && h.gd_mon == month){
                             print =1;
                         }
                         if(print){
                             if((!(first_year == h.gd_year)||!(first_month==h.gd_mon))&&!(first_year==0)&& setting_note_enable){
                                 note->findNotesForMonthSec(h.gd_year,h.gd_mon,h.gd_day);
                                 //first_year=0;
                             }
                             if(count_days==0){
                                 monthStartHeb=h.hd_mon;
                                 yearStartHeb=h.hd_year;
                                 monthStartG=h.gd_mon;
                                 yearStartG=h.gd_year;
                             }
                             count_days++;
                             monthEndHeb=h.hd_mon;
                             yearEndHeb=h.hd_year;
                             monthEndG=h.gd_mon;
                             yearEndG=h.gd_year;
                                 if (jd != jd_today)
                                    {
                                     if (setting_calby){
                                         if (selection!=h.gd_day){
                                            printDay(i,j,0,&h);
                                         }else{
                                              printDay(i,j,1,&h);
                                         }
                                     }else{
                                         if (selection!=h.hd_day){
                                            printDay(i,j,0,&h);
                                         }else{
                                             printDay(i,j,1,&h);
                                         }
                                     }
                                 }
                                 else /* It's today, lets print it in bold */
                                 {
                                     if (selection ==0||selection==h.hd_day||selection==h.gd_day){//הדגש רק את היום אם נבחר או שאין בחירה
                                         printDay(i,j,1,&h);
                                     }
                                     else{
                                         printDay(i,j,0,&h);
                                     }
                                 }
                         }else{
                             calWidgetL.append(new calWidget(date_setting,true));
                            ui->grid->addWidget(calWidgetL.last(),i+1,j);
                         }
                        jd++;
                        widgetID++;
                 }
             }
         printHeader();
         if(setting_note_enable){
              note->releseAllNoteFromList();
         }
         return 0;
}
int MainWindow::printDay(int r, int c,  bool select, hdate_struct const * h){
       int  reading = hdate_get_parasha (h, setting_diaspora);

        calWidgetL.append(new calWidget(date_setting,false));
        calWidget * cw = calWidgetL.last();

        QDate d (h->gd_year,h->gd_mon,h->gd_day);
         cw->setGDate(d);
         cw->setHebDate(h->hd_day,h->hd_mon,h->hd_year,h->hd_dw);
         int hol = hdate_get_holyday(h, setting_diaspora);

         int hol_t= hdate_get_holyday_type(hol);
         if ((hol_t==6||hol_t==8||hol_t==9)&&!setting_israelHoliday){//do not show israeli holidays
             hol =0;
         }
         cw->setHoliday(hol);
         cw->setParasha(reading);
         cw->setDayInWeek(h->hd_dw);

         int omer_day = hdate_get_omer_day(h);
         cw->setOmer(omer_day);
            if(setting_note_enable){
                cw->setNoteId(note->getNoteId(d));
            }
         cw->select(select);
         cw->finish();
         connect(cw, SIGNAL(selected()), selectMapper, SLOT(map()));
                  selectMapper->setMapping(cw, widgetID);
         connect(cw, SIGNAL(new_note_trig()), this, SLOT(addNote()));
         connect(cw, SIGNAL(edit_note_trig()), this, SLOT(editNote()));
         connect(cw, SIGNAL(delete_note_trig()), this, SLOT(deleteNote()));

    if (select){       
        this->select(widgetID);
        currentId=widgetID;
        selectedDate=d;
    }
         ui->grid->addWidget(cw,r+1,c);
 return 0;
}
int MainWindow::printTableHeader(){
    QStringList Days;
    Days.append(QString::fromUtf8("ראשון"));
    Days.append(QString::fromUtf8("שני"));
    Days.append(QString::fromUtf8("שלישי"));
    Days.append(QString::fromUtf8("רביעי"));
    Days.append(QString::fromUtf8("חמישי"));
    Days.append(QString::fromUtf8("שישי"));
    Days.append(QString::fromUtf8("שבת"));

    for (int i =0;i<7;i++){
            QLabel * l = new QLabel(Days.at(i));

            lableHeaderList.append(l);
            lableHeaderList.last()->setAlignment(Qt::AlignCenter);
            ui->grid->addWidget( lableHeaderList.last(),0,i);
    }
    setStyleHeader();
    return 0;
}
void MainWindow::setStyleHeader(){
    for (int i =0;i<7;i++){
        lableHeaderList.at(i)->setStyleSheet("background-color:'"+setting_TableHeaderBackgroundColor+"';");
    }
}
void MainWindow::createAction(int index){
    calWidget * cw = calWidgetL.at(index);
    if(setting_note_enable){
        if (!isMenuNotes){
            menu_2 = new QMenu(QString::fromUtf8("הערות"),ui->menuBar);
             ui->menuBar->addAction(menu_2->menuAction());
             isMenuNotes=true;
        }

         int ac = cw->getNoteId();
         if (ac>0){
             hasNote=true;
         }else{
             hasNote=false;
         }
         if (hasNote ){
             if(!isActionEditNote){
                 actionEditNote = new QAction(this);
                 actionRemoveNote  = new QAction(this);

                 menu_2->addAction(actionEditNote);
                 menu_2->addAction(actionRemoveNote);
                 actionEditNote->setText(QString::fromUtf8("הצג/ערוך הערה זו"));
                 actionRemoveNote->setText(QString::fromUtf8("מחק הערה זו"));
                 connect(actionEditNote, SIGNAL(triggered()), this, SLOT(editNote()));
                  connect(actionRemoveNote, SIGNAL(triggered()), this, SLOT(deleteNote()));
                 isActionEditNote =true;
                 if(isActionAddNote){
                       removeAction(actionAddNote);
                       delete actionAddNote;
                       isActionAddNote =false;
                 }
             }
         } else{
             if (!isActionAddNote){
                 actionAddNote = new  QAction(this);
                 menu_2->addAction(actionAddNote);
                 actionAddNote->setText(QString::fromUtf8("צור הערה לתאריך זה"));
                 connect(actionAddNote, SIGNAL(triggered()), this, SLOT(addNote()));
                 isActionAddNote =true;
                 if (isActionEditNote){
                         removeAction(actionEditNote);
                         removeAction(actionRemoveNote);
                         isActionEditNote=false;
                          delete actionEditNote;
                         delete actionRemoveNote;
                     }
             }
         }
 }else{
        if(isActionEditNote){
            removeAction(actionEditNote);
            removeAction(actionRemoveNote);
            isActionEditNote=false;
            delete actionEditNote;
            delete actionRemoveNote;
            delete menu_2;
            isMenuNotes=false;
        }
        if(isActionAddNote){
            removeAction(actionAddNote);
            delete actionAddNote;
            isActionAddNote =false;
            delete menu_2;
            isMenuNotes=false;
        }
    }
}

void MainWindow::setTimesForSelectin(int index){
    calWidget * cw = calWidgetL.at(index);
    QDate thedate= cw->getGDate();
    if (!thedate.isValid()){
           setZmanimLabelsToEmpty();
           return;
    }

    setTimesByLibhdate(thedate);
    ui->full_g_date->setText(thedate.toString("d.M.yyyy")+" - "+thedate.toString("d MMMM yyyy"));
    ui->full_heb_date->setText(QString::fromUtf8("יום ")+hdate_get_int_string(cw->getDay())+" "+
                               hdate_get_int_string(cw->getHDay())+" "+hdate_get_hebrew_month_string(cw->getHMont(),0)
                               +" "+ hdate_get_int_string(cw->getHYear()));
    ui->full_holyday->setText(cw->getHoliday());
    ui->full_parasha->setText(cw->getParasa());
    if(setting_dafHyomi){
        ui->dafLabel->setText(getDafHyomi(cw->getGDate()));
         ui->dafLabel->show();
    }else{
        ui->dafLabel->hide();
    }

    if(cw->getOmerDay()<=0){
        ui->sefiratHaomerB->hide();
    }else {
         ui->sefiratHaomerB->show();
    }
    createAction(index);
}

void MainWindow::setTimesByLibhdate(QDate theDate){

    setZmanimLabelsToEmpty();
        double sun_hour=0.0, first_light=0.0, talit=0.0, sunrise=0.0;
        double midday=0.0, sunset=0.0, first_stars=0.0, three_stars=0.0;
        double first_light_mga=0.0,first_stars_mga=0.0 , sun_hour_mga=0.0;
                double three_stars_czhish=0.0;
        double latitude = setting_LocationLatitude;
        double longitude = setting_LocationLongitude;
        double height=0.0;
        if(setting_LocationConsiderHeight){
            height=(double)setting_LocationHeight;
        }

        hdate_get_utc_sun_time_full (theDate.day(), theDate.month(), theDate.year(), latitude, longitude,height,
                                         &sun_hour,&sun_hour_mga,&first_light_mga, &first_light, &talit, &sunrise,
                                         &midday, &sunset, &first_stars,&first_stars_mga, &three_stars, &three_stars_czhish);

// find out if its in summer/winter time and if it is auto and set the time
        int tz = setting_LocationTimeZone;
        int tsunrise = ((int)qRound(sunrise)) + (tz * 60);
        QTime tTime =QTime( tsunrise / 60, tsunrise % 60);
        QDateTime rDate = QDateTime(theDate,tTime).toUTC();
        int offsetFromUtc =tTime.hour()- rDate.time().hour();
        if(setting_LocationSummerTime==-1){
            if (setting_LocationTimeZone<offsetFromUtc&&setting_LocationTimeZone==(offsetFromUtc-1)){
                     tz+=1;//it is on summer time
                     ui->summerTimeL->setText(QString::fromUtf8("שעון: קיץ"));
            }else{
                 ui->summerTimeL->setText(QString::fromUtf8("שעון: חורף"));
            }
        }else if (setting_LocationSummerTime==1){
             ui->summerTimeL->setText(QString::fromUtf8("שעון: קיץ"));
            tz+=1;//it is on summer time
        }else{
            ui->summerTimeL->setText(QString::fromUtf8("שעון: חורף"));

        }

        first_light_mga = first_light_mga +tz*60.0;
        first_light = first_light + tz * 60.0;
        talit = talit + tz * 60.0;
        sunrise = sunrise + tz * 60.0;
        midday = midday + tz * 60.0;
        sunset = sunset + tz * 60.0;
        first_stars = first_stars +tz * 60.0;
        three_stars = three_stars + tz * 60.0;
        three_stars_czhish = three_stars_czhish +tz *60;


        QTime first_light_mgaTime =setTimesMinuteAndSec(first_light_mga);
        QTime first_lightTime =setTimesMinuteAndSec(first_light);
        QTime talitTime =setTimesMinuteAndSec(talit);
        QTime sunriseTime =setTimesMinuteAndSec(sunrise);
        QTime middayTime =setTimesMinuteAndSec(midday);
        QTime sunsetTime =setTimesMinuteAndSec(sunset);

         if(setting_alot_mga_show){
             ui->alotMgaLabel->setText(QString::fromUtf8("עלות השחר (מג\"א)")+" - "+first_light_mgaTime.toString(setting_FormatString));
             ui->alotMgaLabel->show();
         }
       if(setting_alot_show){
           ui->alotlabel->setText(QString::fromUtf8("עלות השחר")+" - "+first_lightTime.toString(setting_FormatString));
           ui->alotlabel->show();
       }
       if(setting_talit_show){
            ui->talitlabel->setText(QString::fromUtf8("זמן ציצית ותפילין")+" - "+talitTime.toString(setting_FormatString));
             ui->talitlabel->show();
       }
       if(setting_zricha_show){
           ui->sunriseLabel->setText(QString::fromUtf8("הנץ החמה")+" - "+sunriseTime.toString(setting_FormatString));
            ui->sunriseLabel->show();
       }
       if(setting_end_shema_mga_show){
           ui->endShemaMgaLabel->setText(QString::fromUtf8("סוף זמן ק\"ש (מג\"א)")+" - "+first_light_mgaTime.addSecs(3*(sun_hour_mga*60.0)).toString(setting_FormatString));
            ui->endShemaMgaLabel->show();
       }
       if(setting_end_shema_show){
           ui->endshemalabel->setText(QString::fromUtf8("סוף זמן ק\"ש (הגר\"א)")+" - "+sunriseTime.addSecs(3*(sun_hour*60.0)).toString(setting_FormatString));
            ui->endshemalabel->show();
       }
       if(setting_end_tefila_mga_show){
           ui->endtefilamgaLabel->setText(QString::fromUtf8("סוף זמן תפילה (מג\"א)")+" - "+first_light_mgaTime.addSecs(4*(sun_hour_mga*60.0)).toString(setting_FormatString));
             ui->endtefilamgaLabel->show();
       }
       if(setting_end_tefila_show){
           ui->endtefilalabel->setText(QString::fromUtf8("סוף זמן תפילה")+" - "+sunriseTime.addSecs(4*(sun_hour*60.0)).toString(setting_FormatString));
             ui->endtefilalabel->show();
       }

       if(setting_midday_show){
           ui->middaylabel->setText(QString::fromUtf8("חצות")+" - "+middayTime.toString(setting_FormatString));
            ui->middaylabel->show();
       }
       if(setting_mincha_gdola_show){
           ui->bigminchalabel->setText(QString::fromUtf8("מנחה גדולה")+" - "+sunriseTime.addSecs(6.5*(sun_hour*60.0)).toString(setting_FormatString));
            ui->bigminchalabel->show();
       }
       if(setting_mincha_ktana_show){
           ui->littleminchalable->setText(QString::fromUtf8("מנחה קטנה")+" - "+sunriseTime.addSecs(9.5*(sun_hour*60.0)).toString(setting_FormatString));
            ui->littleminchalable->show();
       }
       if(setting_plug_hamincha_show){
           ui->plugaminchalable->setText(QString::fromUtf8("פלג המנחה")+" - "+sunriseTime.addSecs(10.75*(sun_hour*60.0)).toString(setting_FormatString));
             ui->plugaminchalable->show();
       }
       if(setting_shkiha_show){
           ui->sunsetLabel->setText(QString::fromUtf8("שקיעת החמה")+" - "+sunsetTime.toString(setting_FormatString));
            ui->sunsetLabel->show();
       }
       if (setting_shabat_show){
           hdate_struct  h ;
           hdate_set_gdate(&h,theDate.day(),theDate.month(),theDate.year());
           int hol = hdate_get_holyday(&h, setting_diaspora);
           hdate_struct  h1 ;//check if tomorow its holiday to print the holiday times
           hdate_set_gdate(&h1,theDate.day()+1,theDate.month(),theDate.year());           
            int hol_tomorow = hdate_get_holyday(&h1, setting_diaspora);

           if (h.hd_dw==6||hdate_get_holyday_type(hol_tomorow)==1){//it is friday or befor holiday
                 QTime candel_liteTime= sunsetTime.addSecs((setting_LocationCandleLight*60)-(setting_LocationCandleLight*60)*2);
                ui->candelLiteLabel->setText(QString::fromUtf8("הדלקת נרות")+" - "+candel_liteTime.toString(setting_FormatString));
                ui->shabatInFrame->show();
                if (h.hd_dw==6){
                    ui->label_3->setText(QString::fromUtf8("זמני כניסת השבת"));
                }else{
                    ui->label_3->setText(QString::fromUtf8("זמני כניסת החג"));
                }
          }else if(h.hd_dw==7||hdate_get_holyday_type(hol)==1){
               QString shabatOrHol ;
               if (h.hd_dw==7){
                   shabatOrHol =QString::fromUtf8("צאת השבת");
                   ui->label->setText(QString::fromUtf8("זמני יציאת השבת"));
               }else{
                   shabatOrHol =QString::fromUtf8("צאת החג");
                   ui->label->setText(QString::fromUtf8("זמני יציאת החג"));
               }



               QTime three_starsTime =setTimesMinuteAndSec(three_stars);
               ui->shabatOutLabel->setText(shabatOrHol+" - "+three_starsTime.toString(setting_FormatString));
               QTime three_stars_czhishTime =setTimesMinuteAndSec(three_stars_czhish);
               ui->shabatOutCzhisLabel->setText(shabatOrHol+QString::fromUtf8(" (חזו\"א)")+" - "+three_stars_czhishTime.toString(setting_FormatString));
               if(setting_shabat_out_rt_show){
                   ui->shabatOutRT->setText(shabatOrHol+QString::fromUtf8(" (ר\"ת)")+" - "+sunsetTime.addSecs(72*60).toString(setting_FormatString));
                   ui->shabatOutRT->show();
               }
                    ui->shabatOutFrame->show();
           }else if(hdate_get_holyday_type(hol)==5){
               ui->label->setText(QString::fromUtf8("זמני יציאת הצום"));
               QTime three_starsTime =setTimesMinuteAndSec(three_stars);
               ui->shabatOutLabel->setText(QString::fromUtf8("זמן יציאת הצום")+" - "+three_starsTime.toString(setting_FormatString));
                 ui->shabatOutFrame->show();
           }

        }

}
//set the time minute and secondes depends on format string
 QTime MainWindow::setTimesMinuteAndSec(double time){
     int int_time;
     int sec;
     // if format string has secondes, so dont round the time and calc the sec
     if(setting_FormatString.contains("ss", Qt::CaseInsensitive)){
        int_time = (int)floor(time);
        sec = (int)(60.0/100.0*((time-int_time)*100.0));
     }else{
         // else round the time and dont calc the sec
         int_time = (int)qRound(time);
         sec =00;
     }
     return QTime( int_time / 60, int_time % 60,sec);
 }

int MainWindow::printHeader(){
    QString str;
    //heb
    str = hdate_get_hebrew_month_string(monthStartHeb,0);
    if(monthStartHeb!=monthEndHeb){
        str= str+ " " + hdate_get_hebrew_month_string(monthEndHeb,0);
    }
    str = str+" "+ hdate_get_int_string(yearStartHeb);
    if (yearStartHeb!=yearEndHeb){
        str = str+" "+ hdate_get_int_string(yearEndHeb);
    }
    //gre
    str = str+ " - "+hdate_get_month_string(monthStartG,0);
    if (monthStartG!=monthEndG){
        str = str+ " "+hdate_get_month_string(monthEndG,0);
    }
    str = str+ " "+QString::number(yearStartG);
    if(yearStartG!=yearEndG){
        str = str+ " "+QString::number(yearEndG);
    }
    ui->headerLable->setText(str);
    return 0;
}

void MainWindow::prevMonth(){
    int s = dayToSelect(setting_calby);
     if(setting_calby){//g date
       if(monthStartG==1){
           jumpToDate(s,12,yearStartG-1);
       }else{
           jumpToDate(s,monthStartG-1,yearStartG);
       }
   }else{//heb date
       if (monthStartHeb==1){//if tisrei
           jumpToDate(s,12,yearStartHeb-1);
       }else if (monthStartHeb==7&&is_leap_year(yearStartHeb)){
           jumpToDate(s,14,yearStartHeb);
       }else if (monthStartHeb==14){
           jumpToDate(s,13,yearStartHeb);
       }else if (monthStartHeb==13){
           jumpToDate(s,5,yearStartHeb);
       }else{
          jumpToDate(s,monthStartHeb-1,yearStartHeb);
       }
   }
}


void MainWindow::prevYear(){
    int s = dayToSelect(setting_calby);
      if(setting_calby){//g date
        jumpToDate(s,monthStartG,yearStartG-1);
    }else{//heb date
        if(monthStartHeb==13||monthStartHeb==14){
           jumpToDate(s,6,yearStartHeb-1);
        }else if(monthStartHeb==6&&is_leap_year(yearStartHeb-1)){
            jumpToDate(s,13,yearStartHeb-1);
        }else{
             jumpToDate(s,monthStartHeb,yearStartHeb-1);
        }
    }
}


void MainWindow::nextMonth(){
    int s = dayToSelect(setting_calby);

    if(setting_calby){//g date
        if(monthStartG==12){
            jumpToDate(s,1,yearStartG+1);
           }else{
            jumpToDate(s,monthStartG+1,yearStartG);
             }

    }else{//heb date
        if (monthStartHeb==12){//if elol
             jumpToDate(s,1,yearStartHeb+1);
             }else if(monthStartHeb==5&&is_leap_year(yearStartHeb)){//if shvat in shana meoberet
           jumpToDate(s,13,yearStartHeb);
        }else if(monthStartHeb==13){//if adar 1
             jumpToDate(s,14,yearStartHeb);
           }else if(monthStartHeb==14){//if adar 2
             jumpToDate(s,7,yearStartHeb);
            }else{
             jumpToDate(s,monthStartHeb+1,yearStartHeb);
              }
    }
}

void MainWindow::nextYear(){
    int s = dayToSelect(setting_calby);
   if(setting_calby){//g date
        jumpToDate(s,monthStartG,yearStartG+1);
        }else{//heb date
        if(monthStartHeb==13||monthStartHeb==14){
           jumpToDate(s,6,yearStartHeb+1);
        }else if(monthStartHeb==6&&is_leap_year(yearStartHeb+1)){
           jumpToDate(s,13,yearStartHeb+1);
        }else{
            jumpToDate(s,monthStartHeb,yearStartHeb+1);
        }
    }
}
void MainWindow::deleteCalWidgets(){  
    while (!calWidgetL.isEmpty()){
        calWidgetL.at(0)->disconnect();
       delete calWidgetL.takeFirst();
    }
    calWidgetL.clear();
    setZmanimLabelsToEmpty();

}

  void MainWindow::setZmanimLabelsToEmpty(){

      ui->full_g_date->setText("");       
      ui->full_heb_date->setText("");
      ui->full_holyday->setText("");
      ui->full_parasha->setText("");
      ui->alotMgaLabel->setText("");
      ui->alotlabel->setText("");
      ui->talitlabel->setText("");
      ui->sunriseLabel->setText("");
      ui->endShemaMgaLabel->setText("");
      ui->endshemalabel->setText("");
      ui->endtefilalabel->setText("");
      ui->middaylabel->setText("");
      ui->bigminchalabel->setText("");
      ui->littleminchalable->setText("");
      ui->plugaminchalable->setText("");
      ui->sunsetLabel->setText("");
      ui->candelLiteLabel->setText("");     
      ui->shabatOutLabel->setText("");
      ui->shabatOutCzhisLabel->setText("");
      ui->shabatOutRT->setText("");
      ui->endtefilamgaLabel->setText("");

      ui->alotMgaLabel->hide();
      ui->alotlabel->hide();
      ui->talitlabel->hide();
      ui->sunriseLabel->hide();
       ui->endShemaMgaLabel->hide();
      ui->endshemalabel->hide();
      ui->endtefilalabel->hide();
      ui->middaylabel->hide();
      ui->bigminchalabel->hide();
      ui->littleminchalable->hide();
      ui->plugaminchalable->hide();
      ui->sunsetLabel->hide();
     ui->endtefilamgaLabel->hide();
     ui->shabatOutRT->hide();
      ui->shabatOutFrame->hide();
    ui->shabatInFrame->hide();
  }

 void MainWindow::select(int id){
      setTimesForSelectin(id);
      if(currentId>=0&&currentId!=id){
        calWidgetL.at(currentId)->unselect();
      }
      currentId=id;
      selectedDate= calWidgetL.at(currentId)->getGDate();
  }
 void MainWindow::showDialogJumpToDate(){
     int day=0;
     int month =0;
     int year=0;
     int type=0;//1=g 2=h
     JumpToDateDialog * jtdd = new JumpToDateDialog(&day,&month,&year,&type);
      jtdd->exec();
      if (day>0 && month >0 && year>0&&type>0){
          jumpToDate(day,month,year,type);
      }
 }
int MainWindow::jumpToDate(int day,int month,int year,int type ){    
     deleteCalWidgets();
    if(type==1){
        hdate_struct h;
        hdate_set_gdate (&h, day, month, year);
        printMonth(h.hd_mon,h.hd_year,h.hd_day);
    }else{
        hdate_struct h;
        hdate_set_hdate (&h, day, month, year);
        fix_date(&h);
        printMonth(h.hd_mon,h.hd_year,h.hd_day);
    }
    return 0;
}


 int  MainWindow::dayToSelect(int by){
    if(by){
        return calWidgetL.at(currentId)->getGDate().day();
    }else{
        return calWidgetL.at(currentId)->getHDay();
    }
}
 void MainWindow::showDialogOmer(){
     int d =calWidgetL.at(currentId)->getOmerDay();
     omerDialog *od = new  omerDialog(d,setting_sefirat_homer_nosach,this);
     od->exec();
 }

void MainWindow::showDialogSettings(){
    SettingsDialog *sd = new SettingsDialog();
    if(sd->exec()){
        deleteCalWidgets();
        loadSettings();
     draw(1);
     setStyleHeader();
    }
}

void MainWindow::print_screen(){
    QPrinter printer;
     QPrintDialog *dialog = new QPrintDialog(&printer, this);
     if (dialog->exec() != QDialog::Accepted)
         return ;

     printer.setOrientation(QPrinter::Landscape);
     ui->nextMonthButton->hide();
     ui->nextYearButton->hide();
     ui->prevMonthButton->hide();
     ui->prevYearButton->hide();
      ui->label_for_print->show();
     ui->label_for_print->setText(QString::fromUtf8("הודפס בתוכנת Hebrew Calendar Pluse "));

     ui->centralWidget->resize(printer.width(),printer.height());
     ui->centralWidget->render(&printer);
     ui->nextMonthButton->show();
     ui->nextYearButton->show();
     ui->prevMonthButton->show();
     ui->prevYearButton->show();
     ui->label_for_print->setText("");
     ui->label_for_print->hide();
}

void MainWindow::createDataBase(){
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS location ("
               "mainName TEXT,secName TEXT,Region TEXT, Longitude TEXT, Latitude TEXT,TimeZone TEXT,PlaceHeight TEXT,Candle_Light TEXT)");
    qDebug()<< query.lastError().text() << " createDataBase line: 992";
    QString city ;

    city = QString::fromUtf8("ירושלים");
    query.exec("INSERT INTO  location (mainName,secName,Region,Longitude,Latitude,TimeZone,"
               "PlaceHeight,Candle_Light) VALUES ('"+city+"','jerusalem','Israel','35.22','31.78'"
               ",'2',800,'40')");
    city = QString::fromUtf8("תל אביב");
    query.exec("INSERT INTO  location (mainName,secName,Region,Longitude,Latitude,TimeZone,"
               "PlaceHeight,Candle_Light) VALUES ('"+city+"','tel aviv','Israel','34.77','32.06'"
               ",'2',0,'22')");
    city = QString::fromUtf8("באר שבע");
    query.exec("INSERT INTO  location (mainName,secName,Region,Longitude,Latitude,TimeZone,"
               "PlaceHeight,Candle_Light) VALUES ('"+city+"','beher Sheva','Israel','34.79','31.24'"
               ",'2',0,'20')");
    city = QString::fromUtf8("חיפה");
    query.exec("INSERT INTO  location (mainName,secName,Region,Longitude,Latitude,TimeZone,"
               "PlaceHeight,Candle_Light) VALUES ('"+city+"','haifa','Israel','34.991','32.8'"
               ",'2',0,'30')");
    city = QString::fromUtf8("בני ברק");
    query.exec("INSERT INTO  location (mainName,secName,Region,Longitude,Latitude,TimeZone,"
               "PlaceHeight,Candle_Light) VALUES ('"+city+"','bnei brack','Israel','34.82','32.08'"
               ",'2',0,'22')");
    //FIXME: in linux its negative value??
    qDebug()<< query.lastError().text()<<" createDataBase line: 998";
     city = QString::fromUtf8("נתיבות");
    query.exec("INSERT INTO  location (mainName,secName,Region,Longitude,Latitude,TimeZone,"
               "PlaceHeight,Candle_Light) VALUES ('"+city+"', 'netivot','Israel','34.59','31.42'"
               ",'2','142','30')");
    city = QString::fromUtf8("חיפה");
    query.exec("INSERT INTO  location (mainName,secName,Region,Longitude,Latitude,TimeZone,"
               "PlaceHeight,Candle_Light) VALUES ('"+city+"','haifa','Israel','34.991','32.8'"
               ",'2',0,'30')");
     qDebug()<< query.lastError().text()<<" createDataBase line: 1003";
}
 int MainWindow::connectToDataBase(){
     db = QSqlDatabase::addDatabase("QSQLITE","location");
     db.setHostName("localhost");
      QString name = QDir::homePath()+"/.config/avisoftware/"+APP_SHORT_NAME+"/location.db";
     db.setDatabaseName(name);

        if (!db.open())
        {
           return 1;
        }
        qDebug() << db.lastError().text()<<" connectToDataBase  line 1014";
        return 0;
 }


void MainWindow::loadSettings(){
    QString name = QDir::homePath()+"/.config/avisoftware/"+APP_SHORT_NAME+"/location.db";
    if(!checkDataBase(name)){
        QDir dir = QDir::home();                 // "/home/user"
        dir.cd(".config");
        if(!dir.cd("avisoftware")){
            dir.mkpath("avisoftware");
            dir.cd("avisoftware");
        }
        dir.mkpath(APP_SHORT_NAME);

        connectToDataBase();
        createDataBase();
        reset_settings(db);
    }else{
        connectToDataBase();
    }
    if(!checkDataBase(QDir::homePath()+"/.config/avisoftware/"+APP_SHORT_NAME+".conf")){
        reset_settings(db);
    }
    setting_calby =read_setting("cal","calby").toInt();
    setting_diaspora=read_setting("cal","diaspora").toBool();
    setting_israelHoliday = read_setting("cal","IsraelHoliday").toBool();
    setting_dafHyomi = read_setting("cal","dafHyomi").toBool();

    setting_LocationHebName =read_setting("times","location_heb_name").toString();
    setting_LocationEngName =read_setting("times","location_eng_name").toString();
    setting_LocationRegion = read_setting("times","location_region").toString();
    setting_LocationLongitude = read_setting("times","location_longitude").toDouble();
    setting_LocationLatitude = read_setting("times","location_latitude").toDouble();
    setting_LocationTimeZone=  read_setting("times","location_time_zone").toInt();
    setting_LocationHeight = read_setting("times","location_height").toInt();
    setting_LocationConsiderHeight = read_setting("times","location_consider_height").toBool();
    setting_LocationCandleLight = read_setting("times","location_candle_light").toInt();
    setting_LocationSummerTime =  read_setting("times","SummerTime").toInt();
    setting_FormatString =  read_setting("times","FormatTime").toString();


     setting_MainBackgroundColor = read_setting("view","MainBackgroundColor").toString();
     setting_MainTextColor =read_setting("view","MainTextColor").toString();
     setting_TableHeaderBackgroundColor=read_setting("view","TableHeaderBackgroundColor").toString();
     setting_MainGridColor =read_setting("view","MainGridColor").toString();

     date_setting = new DateWidgetSettings;
     date_setting->setting_MainSelectorColor =read_setting("view","MainSelectorColor").toString();
     date_setting->setting_DateBackgroundColor= read_setting("view","DateBackgroundColor").toString();
     date_setting->setting_DateBackgroundHolydayColor= read_setting("view","DateBackgroundHolydayColor").toString();
     date_setting->setting_DateParashaColor= read_setting("view","DateParashaColor").toString();
     date_setting->setting_DateHolydayColor= read_setting("view","DateHolydayColor").toString();
     date_setting->setting_DateTextColor= read_setting("view","DateTextColor").toString();
     date_setting->setting_DateTextHolydayColor= read_setting("view","DateTextHolydayColor").toString();
     date_setting->setting_sefirat_homer_nosach= read_setting("sefirat_homer","nosach").toInt();
           setting_sefirat_homer_nosach= date_setting->setting_sefirat_homer_nosach;


      setting_alot_mga_show = read_setting("view_times","alot_mga").toBool();
      setting_alot_show =read_setting("view_times","alot").toBool();
      setting_talit_show =read_setting("view_times","talit").toBool();
      setting_zricha_show =read_setting("view_times","zricha").toBool();
      setting_end_shema_mga_show =read_setting("view_times","end_shema_mga").toBool();
      setting_end_shema_show =read_setting("view_times","end_shema").toBool();
      setting_end_tefila_mga_show= read_setting("view_times","end_tefila_mga").toBool();
      setting_end_tefila_show =read_setting("view_times","end_tefila").toBool();
      setting_midday_show =read_setting("view_times","midday").toBool();
      setting_mincha_gdola_show =read_setting("view_times","mincha_gdola").toBool();
      setting_mincha_ktana_show =read_setting("view_times","mincha_ktana").toBool();
      setting_plug_hamincha_show =read_setting("view_times","plug_hamincha").toBool();
      setting_shkiha_show =read_setting("view_times","shkiha").toBool();
      setting_shabat_show =read_setting("view_times","shabat").toBool();
      setting_shabat_out_rt_show = read_setting("view_times","shabatOutRT").toBool();
      setting_note_enable=read_setting("notes","enable").toBool();
             date_setting->setting_note_enable=setting_note_enable;

      db.close();
}
  void MainWindow::addNote(){
      int i=note->addNote(calWidgetL.at(currentId)->getGDate());
      if (i>0){
          calWidgetL.at(currentId)->changeNoteId(i);
          setTimesForSelectin(currentId);
      }
  }
  void MainWindow::editNote(){
      int d = note->editNote(calWidgetL.at(currentId)->getNoteId());
      if (d==0){
          // delete mark
          calWidgetL.at(currentId)->deleteNote();
      }
      setTimesForSelectin(currentId);
  }
  void MainWindow::deleteNote(){
      int d = note->deleteNote(calWidgetL.at(currentId)->getNoteId());
      if (d==0){
          // delete mark
          calWidgetL.at(currentId)->deleteNote();
      }
      setTimesForSelectin(currentId);
  }
void MainWindow::print_tables(){
    int y=calWidgetL.at(currentId)->getHYear();
    int m=calWidgetL.at(currentId)->getHMont();
    DialogPrintTables * pt = new DialogPrintTables(m,y,0,this);
   pt->setSettingsDateAndTimes(setting_diaspora,setting_israelHoliday,setting_LocationLatitude,
                               setting_LocationLongitude,setting_LocationTimeZone,
                               setting_LocationSummerTime,setting_LocationConsiderHeight,setting_LocationHeight);
    pt->exec();
}
 void MainWindow::aboutA(){
     QDialog * d =new QDialog(this);
    Ui::Dialog a;
    a.setupUi(d);
    d->exec();
 }
