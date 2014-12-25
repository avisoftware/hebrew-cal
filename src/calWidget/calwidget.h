/* calwidget.h
 * Hebrew Calendar based on libhdate http://libhdate.sourceforge.net
 *Copyright: (c) Avi Software <avisoftware123@gmail.com> 2014
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

#ifndef CALWIDGET_H
#define CALWIDGET_H

#include <QWidget>
#include <QDate>
#include <QTime>
#include <QList>
#include <QPainter>
#include <QMouseEvent>
#include <QList>
#include "../settingsfunc.h"

QT_BEGIN_NAMESPACE
class QTime;
class QPrinter;
QT_END_NAMESPACE

namespace Ui {
class calWidget;
}

class calWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit calWidget(DateWidgetSettings * setting, bool empty=false,  QWidget *parent = 0);
    ~calWidget();

    int setHebDate(int hD, int hM, int hY, int D);
    int setGDate(QDate gD);
    int setHoliday(int hol);
    int setParasha(int p);
    int setOmer(int n);
    int setDayInWeek(int dw);
    int setAllEmpty();

    void select(bool toSelect=true);
    void unselect();
    bool isSelect(){return selection;}
    QDate getGDate(){return gDate;}
    int getHDay(){return hDay;}
    int getHMont(){return hMont;}
    int getHYear(){return hYear;}
    int getDay(){return Day;}
    int getOmerDay(){return OmerDay;}
    QString getParasa(){return parasha;}
    QString getHoliday(){return holiday;}
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);
    void drewFrames();
    void deleteFrames();

    //NOTE PLUG
    void setNoteId(int id);
     void changeNoteId(int id);
    int getNoteId(){return id_note;}
    void deleteNote();

     void finish();
protected:

    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QString BG;
     QString parasha;
     QString holiday;
    bool dontDrewFrames;
    //int to_delete_frames;
    bool selection;
    void init();
    QDate gDate;
    int hDay;
    int hMont;
    int hYear;
    int Day;
    int OmerDay;
    QString dataFormat;
    Ui::calWidget *ui;
    DateWidgetSettings * settings;
    //NOTE PLUG
    int id_note;
    bool toDeleteNote;
    QColor note_color;
    bool hasNote;
    QAction *note_Act;
    QAction *new_note_Act;
    QAction *edit_note_Act;
    QAction *delete_note_Act;

    void createNoteActions();
  signals:
      void selected();
       //NOTE PLUG
      void new_note_trig();
      void edit_note_trig();
      void delete_note_trig();
private slots:
    //NOTE PLUG
    void new_note();
    void edit_note();
    void delete_note();
};


#endif // CALWIDGET_H
