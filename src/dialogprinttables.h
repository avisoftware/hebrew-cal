/* dialogprinttables.h
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
#ifndef DIALOGPRINTTABLES_H
#define DIALOGPRINTTABLES_H
#include <QDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtWidgets>
#include <QMessageBox>
#include "settingsfunc.h"

QT_BEGIN_NAMESPACE

    class QPrinter;
    class QPrintDialog;
QT_END_NAMESPACE

namespace Ui {
class DialogPrintTables;
}

class DialogPrintTables : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPrintTables(int month, int year, int is, QWidget *parent = 0);
    ~DialogPrintTables();
    void setSettingsDateAndTimes(bool settingDiaspora, bool settingIsraelHoliday,
                                 double settingLocationLatitude ,double settingLocationLongitude,
                                 int settingLocationTimeZone, int settingLocationSummerTime,bool settingLocationConsiderHeight,
    int settingLocationHeight){setting_diaspora=settingDiaspora;
                                                                                             setting_israelHoliday=settingIsraelHoliday;
                                                                                              latitude = settingLocationLatitude;
                                                                                              longitude = settingLocationLongitude;
                                                                                              tz = settingLocationTimeZone;
                                                                                             summerTime = settingLocationSummerTime;
                                                                                              setting_LocationConsiderHeight=settingLocationConsiderHeight;
                                                                                                                     setting_LocationHeight=settingLocationHeight;}
    void init (int m, int y);
private:
    int is_m;
    void doHtmlHead(int type);

    void printing_process_year(QPrinter *printer);
    void printing_process_month(QPrinter *printer);
    void print();
    int check_info();


    QString getSunset(int d, int m, int y);

    QString html;
    Ui::DialogPrintTables *ui;

    bool setting_diaspora;
    bool setting_israelHoliday;
    double latitude ;
    double longitude ;
    bool setting_LocationConsiderHeight;
    int setting_LocationHeight;
    int tz ;
   int summerTime ;
private slots:
    void typeChange(int index);
    void ok();
    void cancel();
};

#endif // DIALOGPRINTTABLES_H
