/* calwidget.cpp
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
#include "calwidget.h"
#include "ui_calwidget.h"
#include "../hebD.h"
#include <QMenu>


calWidget::calWidget(DateWidgetSettings *setting, bool empty, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calWidget)
{
    settings = new DateWidgetSettings;//date setting struct to send the setting beetwin class's
    settings->setting_MainSelectorColor=setting->setting_MainSelectorColor;
    settings->setting_DateBackgroundColor =setting->setting_DateBackgroundColor;
    settings->setting_DateBackgroundHolydayColor =setting->setting_DateBackgroundHolydayColor;
    settings->setting_DateParashaColor =setting->setting_DateParashaColor ;
    settings->setting_DateHolydayColor =setting->setting_DateHolydayColor;
    settings->setting_DateTextColor = setting->setting_DateTextColor;
    settings->setting_DateTextHolydayColor =setting->setting_DateTextHolydayColor;
    settings->setting_sefirat_homer_nosach =setting->setting_sefirat_homer_nosach;
    settings->setting_note_enable=setting->setting_note_enable;
    ui->setupUi(this);

    dontDrewFrames = empty;
    BG = "QWidget#calWidget{background-color :'"+settings->setting_DateBackgroundColor+"';}";

     setStyleSheet(BG);
     selection=0;
     OmerDay=0;
     id_note=0;
     hasNote=false;
     toDeleteNote = false;
     holiday = "";
     init();
}

calWidget::~calWidget()
{
    delete ui;
}
 void calWidget::init(){
     //to_delete_frames=0;
      setAllEmpty();
      dataFormat= "h:mm"; //the date format    
 }
  void  calWidget::finish(){
      // createActions();
  }

int calWidget::setHebDate(int hD, int hM, int hY, int D){
   QString hebD = get_int_to_heb_string(hD);
    ui->hDateL->setText(hebD);
    ui->hDateL->setStyleSheet("color:'"+settings->setting_DateTextColor+"';");
    hDay = hD;
     hMont=hM;
     hYear=hY;
     Day=D;
    return 0;
}

int calWidget::setGDate(QDate gD){
    gDate=gD;
    ui->gDateL->setText(QString::number(gD.day()));
    ui->gDateL->setStyleSheet("color:'"+settings->setting_DateTextColor+"';");
    return 0;
}


int calWidget::setHoliday(int hol){
     holiday = hdate_get_holyday_string(hol,1);
     if(hol){
         ui->holidayL->setText(holiday);
        ui->holidayL->setStyleSheet("color:'"+settings->setting_DateHolydayColor+"';");
        ui->hDateL->setStyleSheet("color:'"+settings->setting_DateTextHolydayColor+"';");
        ui->gDateL->setStyleSheet("color:'"+settings->setting_DateTextHolydayColor+"';");
        if(!selection){
            if (hdate_get_holyday_type(hol)==1){
                BG =" QWidget#calWidget{ background-color :'"+settings->setting_DateBackgroundHolydayColor+"';}";
                setStyleSheet(BG);
             //  BG=settings->setting_DateBackgroundHolydayColor;
            }else{
                QString back = "QWidget#calWidget{background-color :qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 '"+
                        settings->setting_DateBackgroundHolydayColor+"', stop:1 '"+settings->setting_DateBackgroundColor+"');}";
                setStyleSheet(back);
                BG= back;
            }
        }

     }
    return 0;
}
 int calWidget::setDayInWeek(int dw){

         if (dw==7){
             BG = "QWidget#calWidget{background-color :'"+settings->setting_DateBackgroundHolydayColor+"';}";
             if(!selection){
             setStyleSheet(BG);
            }
          }
     return 0;
 }

int calWidget::setAllEmpty(){
     ui->gDateL ->setText("");
     ui->hDateL ->setText("");
     ui->holidayL ->setText("");
     ui->parashaL->setText("");
     ui->omerL->setText("");
     return 0;
}
QString myStringFromHeb(const char * str){
    return (QString::fromUtf8(str));
}

int calWidget::setParasha(int p){

         if (p)
         {
              parasha=hdate_get_parasha_string (p, 1);
              ui->hDateL->setStyleSheet("color:'"+settings->setting_DateTextHolydayColor+"';");
              ui->gDateL->setStyleSheet("color:'"+settings->setting_DateTextHolydayColor+"';");
              if(!selection){
                  BG = "QWidget#calWidget{background-color :'"+settings->setting_DateBackgroundHolydayColor+"';}";
                  setStyleSheet(BG);
              }
              //BG=settings->setting_DateBackgroundHolydayColor;
         }else{
             parasha ="";
         }
    ui->parashaL->setText(parasha);
    ui->parashaL->setStyleSheet("color:'"+settings->setting_DateParashaColor+"';");

    return 0;
}
void calWidget::paintEvent(QPaintEvent *)
 {
    QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
        QPainter  apainter (this);
        if(id_note>0){            
            QColor color(note_color);

            apainter.setRenderHint(QPainter::Antialiasing);
            apainter.setPen(color);
            apainter.setBrush(QBrush(color));

             QPointF points[3] = {
                 QPointF((double) (this->size().width()/3)*2, 0),
                 QPointF((double)(this->size().width()),0),
                 QPointF((double)(this->size().width()), (double) (this->size().width()/3))
             };

             apainter.drawPolygon(points, 3);
        }
        if (toDeleteNote){
            QRectF rect(0, (this->size().width()/3)*2, 0,  (this->size().width()/3));
            apainter.eraseRect(rect);
            toDeleteNote= false;
        }
}

void calWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        event->accept();
            select();
        //repaint();
    }
}
void calWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setLayoutDirection(Qt::RightToLeft);

     if(settings->setting_note_enable){
         menu.addSeparator();

         menu.addAction(note_Act);
         menu.addSeparator();
         if(id_note==0){
             menu.addAction(new_note_Act);
         }else if(id_note>0){
             menu.addAction(edit_note_Act);
             menu.addAction(delete_note_Act);
         }
     }
    menu.exec(event->globalPos());
}


void  calWidget::select(bool toSelect)
{
    if(toSelect){
        if (!dontDrewFrames){
           selection=true;
           setStyleSheet("QWidget#calWidget {background-color :'"+settings->setting_MainSelectorColor+"';}");
           emit selected();
        }
    }
}
void calWidget::unselect(){
    selection = false;
    setStyleSheet(BG);
}

int calWidget::setOmer(int n){
    if(n){
        QString str = get_int_to_heb_string(n);
        if (settings->setting_sefirat_homer_nosach){
            str +=" "+ myStringFromHeb("לעומר");
        }else{
            str +=" " +myStringFromHeb("בעומר");
        }
         ui->omerL->setText(str);
         OmerDay =n;
    }else{

    }
     return 0;
}
 //NOTE PLUG
void calWidget::setNoteId(int id){
    note_color = QColor(read_setting("notes","Color").toString());
    id_note=id;
    if(id>0){

    }
    createNoteActions();
    //drew note!

}
  void  calWidget::changeNoteId(int id){
    hasNote=true;
      setNoteId( id);
  }



void calWidget::deleteNote()
{
    //drew note!
    if(id_note>0){
        toDeleteNote= true;
        update();
    }
    id_note=0;
    hasNote=true;
    createNoteActions();
}
void calWidget::createNoteActions(){
    if(settings->setting_note_enable){
        note_Act = new QAction(myStringFromHeb("הערות:"), this);

        note_Act->setEnabled(false);

        if(id_note==0){
            if(hasNote){
                delete edit_note_Act;
                delete delete_note_Act;
            }
             new_note_Act= new QAction(myStringFromHeb("הוסף הערה"), this);
             connect(new_note_Act, SIGNAL(triggered()), this, SLOT(new_note()));

             hasNote=false;
        }else if(id_note>0){
            if(hasNote){
               delete  new_note_Act;
            }
            edit_note_Act= new QAction(myStringFromHeb("הצג/ערוך הערה"), this);
            delete_note_Act= new QAction(myStringFromHeb("מחק הערה"), this);
            connect(edit_note_Act, SIGNAL(triggered()), this, SLOT(edit_note()));
            connect(delete_note_Act, SIGNAL(triggered()), this, SLOT(delete_note()));
            hasNote=true;
        }
    }
}


void calWidget::new_note(){
    emit new_note_trig();
}

void calWidget::edit_note(){
     emit edit_note_trig();
}

void calWidget::delete_note(){
    emit delete_note_trig();
}
void calWidget::resizeEvent(QResizeEvent *event)
{
        update();
    QWidget::resizeEvent(event);
}

