/* locationeditdialog.h
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
#ifndef LOCATIONEDITDIALOG_H
#define LOCATIONEDITDIALOG_H

#include <QDialog>

namespace Ui {
class LocationEditDialog;
}

class LocationEditDialog : public QDialog
{
    Q_OBJECT
    
public:

    explicit LocationEditDialog(QString *hebName, QString *engName, QString *Region,
                                QString *Longitude, QString *Latitude, QString *TimeZone,
                                QString *PlaceHeight, QString *Candle_Light, QWidget *parent = 0);
     //LocationEditDialog(QWidget *parent = 0);
    ~LocationEditDialog();
    
private:
    Ui::LocationEditDialog *ui;
    QString *phebName;
    QString *pengName;
    QString *pRegion;
    QString *pLongitude;
    QString *pLatitude;
    QString *pTimeZone;
    QString *pPlaceHeight;  
    QString *pCandle_Light;
private slots:
    void ok();
    void cancel();
};

#endif // LOCATIONEDITDIALOG_H
