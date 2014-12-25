/* omerdialog.h
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

#ifndef OMERDIALOG_H
#define OMERDIALOG_H

#include <QDialog>

namespace Ui {
class omerDialog;
}

class omerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit omerDialog(int d, int no, QWidget *parent = 0);
    ~omerDialog();
private:
    Ui::omerDialog *ui;

    int day_in_omer;
    int nosach;
    void init();


    QString getString ();
    QString getFirstString ();
    QString getSecondString ();
    QString convertNumOfOmerToString();

    QString dayORdays();
    QString convertNumToDays();

    void show_brachot_dialog(int type);
    private slots:
      void show_brachot_dialog_before();
      void show_brachot_dialog_after();
};

class brachot : public QDialog
{
    Q_OBJECT
public:
    brachot(int show, int type, int dayInOmer);
    QString typeS;
private:
    QString sfirot(int DayInOmer);
    int getSeven(int DayInOmer);
    int getUnit(int DayInOmer);
signals:

public slots:

};

#endif // OMERDIALOG_H
