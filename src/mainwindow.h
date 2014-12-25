/* mainwindow.h
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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtSql>
#include <QtWidgets>

#include "calWidget/calwidget.h"
#include "hebD.h"

#include "settingsfunc.h"
#include "noteplugin.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     void  init();
     Ui::MainWindow *ui;
private:


    QSignalMapper *selectMapper;
    QSignalMapper *delMapper;
    QMenu *menu_2;
    QAction *actionEditNote;
    QAction *actionAddNote;
    QAction *actionRemoveNote;
    bool isActionEditNote;
    bool isActionAddNote;
    bool isMenuNotes;
    bool hasNote;
    NotePlugIn * note;
   int  widgetID;
   int currentId;
   QDate selectedDate;
    QList <calWidget*> calWidgetL;

    void draw(int now=0);
    int printMonth(int month, int year, int selection);
    int printTableHeader();
     int printDay(int r, int c,  bool select, hdate_struct const * h);
    int jumpToDate(int day,int month,int year,int type =0);


    int printHeader();
    int monthStartHeb;
    int monthEndHeb;
    int yearStartHeb;
    int yearEndHeb;

    int monthStartG;
    int monthEndG;
    int yearStartG;
    int yearEndG;
    void deleteCalWidgets();
    void setZmanimLabelsToEmpty();
    int dayToSelect(int by);
    void loadSettings();
    void createAction(int index);
    int connectToDataBase();
    void createDataBase();

    QSqlDatabase  db;
    int setting_calby;//0 = h , 1=g
    bool setting_diaspora;
    bool setting_israelHoliday;
    bool setting_dafHyomi;
    QString setting_LocationHebName;
    QString setting_LocationEngName;
    QString setting_LocationRegion;
    double setting_LocationLongitude;
    double setting_LocationLatitude;
    int setting_LocationTimeZone;
    int setting_LocationHeight;
    bool setting_LocationConsiderHeight;
    int setting_LocationCandleLight;
    int setting_LocationSummerTime;
    QString setting_FormatString;
    int setting_sefirat_homer_nosach;
    QString setting_MainBackgroundColor;
    QString setting_MainTextColor;
    QString setting_TableHeaderBackgroundColor;
    QString setting_MainGridColor;

    bool setting_alot_mga_show;
    bool setting_alot_show;
    bool setting_talit_show;
    bool setting_zricha_show;
    bool setting_end_shema_mga_show;
    bool setting_end_shema_show;
    bool setting_end_tefila_show;
   bool setting_end_tefila_mga_show;
    bool setting_midday_show;
    bool setting_mincha_gdola_show;
    bool setting_mincha_ktana_show;
    bool setting_plug_hamincha_show;
    bool setting_shkiha_show;
    bool setting_shabat_show;
     bool setting_shabat_out_rt_show;

    bool setting_note_enable;
    DateWidgetSettings * date_setting;

    QList <QLabel*> lableHeaderList;
    void setStyleHeader();

    void setTimesByLibhdate(QDate theDate);
    QTime setTimesMinuteAndSec(double time);

private slots:
    void setTimesForSelectin(int index );
    void prevMonth();
    void prevYear();
    void nextMonth();
    void nextYear();
    void select(int id);
    void showDialogJumpToDate();
    void showDialogSettings();
    void print_screen();
    void showDialogOmer();
    void print_tables();
    void aboutA();

    void addNote();
    void editNote();
    void deleteNote();

 signals:
    void selected(int id);
};



//PLUGIN

#endif // MAINWINDOW_H
