/* jumptodatedialog.h
 * Hebrew Calendar based on libhdate http://libhdate.sourceforge.net
 *
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
#ifndef JUMPTODATEDIALOG_H
#define JUMPTODATEDIALOG_H

#include <QDialog>
#include <QDate>
namespace Ui {
  class JumpToDateDialog;
}
class JumpToDateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit JumpToDateDialog(int * day, int *month ,int * year,int *type,QWidget *parent = 0);
private:
    int * pDay;
    int * pMonth;
    int * pYear;
    int *pType;
    Ui::JumpToDateDialog *ui;
    void init ();
    void setDates();
    void setComboDate();
private slots:
    void gPress();
    void hPress();

    void ok();
    void cancel();
signals:
    
public slots:
    
};

#endif // JUMPTODATEDIALOG_H
