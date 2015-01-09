/* settingsdialog.h
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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QtWidgets>
#include <QWidget>
#include <QtSql>
QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE
namespace Ui {
class SettingsDialog;
}
class CalendarPage : public QWidget
{
     Q_OBJECT
public:
    CalendarPage(QWidget *parent = 0);
    int isChange(){return change;}
    void reset();
private:
    int change;
    QComboBox *calbyCombo ;
    QCheckBox *diasporaCheckBox;
    QCheckBox *israelHolidayCheckBox;
    QCheckBox *dafHyomiCheckBox;

private slots:
    void calby_change(int i);
    void diaspora_change(bool b);
     void israelHoliday_change(bool b);
     void dafHyomiCheckBox_change(bool b);
};
class TimesPage : public QWidget
{
     Q_OBJECT
public:
    TimesPage(QWidget *parent = 0);
     int isChange(){return change;}
     void reset();
private:
      QCheckBox *considerHeightCheckBox;
       QComboBox * cityComboBox;
       QComboBox * formatTimeComboBox;
       QSqlDatabase db;
       void createTable ();
       int first_run();
       void setModelLocation();
       int init ();
       void setSummerTime();

       QRadioButton * autoRadioButton ;
       QRadioButton * winterRadioButton ;
       QRadioButton * summerRadioButton ;


       int change;
private slots:
       void editLocation();
       void changeLocation(QString loc);

       void winterPressed();
       void summerPressed();
       void autoPressed();
       void considerHeight_change(bool c);
       void changeFormatTimeString(const QString &formatString);
};

class OmerPage : public QWidget
{
     Q_OBJECT
public:
    OmerPage(QWidget *parent = 0);
    int isChange(){return change;}
    void reset();
private:
    int change;

       QComboBox * typeComboBox;
private slots:
       void changeNosach(int n);
};

class ViewPage : public QWidget
{
     Q_OBJECT
public:
    ViewPage(QWidget *parent = 0);
    int isChange(){return change;}
    void reset();
private:
    int change;

    QPushButton * MainBackgroundColorButton ;
    QPushButton * MainTextColorButton;
    QPushButton * TableHeaderBackgroundColor;
    QPushButton * MainGridColorButton ;
    QPushButton * MainSelectorColorButton;
    QPushButton * DateBackgroundColorButton ;
    QPushButton * DateBackgroundHolydayColorButton ;
    QPushButton * DateTextColorButton ;

    QPushButton * DateTextHolydayColorButton ;
    QPushButton * DateParashaColorButton;
    QPushButton * DateHolydayColorButton;
    void setColors();

private slots:
    void MainBackgroundColorChange();
    void MainTextColorChange();
    void TableHeaderBackgroundColorChange();
   void MainGridColorChange();
   void MainSelectorColorChange();
   void DateBackgroundColorChange();
   void DateBackgroundHolydayColorChange();
   void DateParashaColorChange();
   void DateHolydayColorChange();
   void DateTextColorChange();
   void DateTextHolydayColorChange();
};
class ViewTimesPage : public QWidget
{
     Q_OBJECT
public:
    ViewTimesPage(QWidget *parent = 0);
    int isChange(){return change;}
    void reset();

private:
    QScrollArea * scroll;
    QCheckBox * alot_mgaCheckBox;
    QCheckBox * alotCheckBox;
    QCheckBox * talitCheckBox;
    QCheckBox * zrichaCheckBox;
    QCheckBox * end_shemaCheckBox;
    QCheckBox * end_shema_mgaCheckBox;
    QCheckBox * end_tefila_mgaCheckBox;
    QCheckBox * end_tefilaCheckBox;
    QCheckBox * middayCheckBox;
    QCheckBox * mincha_gdolaCheckBox;
    QCheckBox * mincha_ktanaCheckBox;
    QCheckBox * plug_haminchaCheckBox;
    QCheckBox * shkihaCheckBox;
    QCheckBox * shabatCheckBox;
    QCheckBox * shabatOutRTCheckBox;
    int change;
    void setAll();

private slots:
   void alot_mga_change(bool b);
   void alot_change(bool b);
   void talit_change(bool b);
   void zricha_change(bool b);
   void end_shema_mga_change(bool b);
   void end_shema_change(bool b);
   void end_tefila_mga_change(bool b);
   void end_tefila_change(bool b);
   void midday_change(bool b);
   void mincha_gdola_change(bool b);
   void mincha_ktana_change(bool b);
   void plug_hamincha_change(bool b);
   void shkiha_change(bool b);
   void shabat_change(bool b);
   void shabat_out_rt_change(bool b);
};
class NotePage : public QWidget
{
     Q_OBJECT
public:
    NotePage(QWidget *parent = 0);
    int isChange(){return change;}
    void reset();
private:
    int change;
    //QComboBox *enableCheckBox ;
     QGroupBox *MainGroupBox ;
    QPushButton * ColorButton ;
    QPushButton * clearAllButton ;
    void setColor();
private slots:
    void enable_change(bool b);
    void ColorChange();
    void clearAll();
};

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog( QWidget *parent = 0);
    ~SettingsDialog();
public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
private:
    Ui::SettingsDialog *ui;
    void addPages();
    void  closeEvent ( QCloseEvent * event );
    CalendarPage * calenderPage;
    TimesPage * timesPage;
    OmerPage * omerPage;
    ViewPage * viewPage;
    ViewTimesPage * viewTimesPage;
    NotePage * notePage;
private slots:
    void resetall();
    void toClose();
};


#endif // SETTINGSDIALOG_H
