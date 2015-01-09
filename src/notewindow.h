/* notewindow.h
 * Hebrew Calendar based on libhdate http://libhdate.sourceforge.net
 * Copyright: (c) Avi Software <avisoftware123@gmail.com> 2014
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


#ifndef NOTEWINDOW_H
#define NOTEWINDOW_H

#include <QDialog>

namespace Ui {
class NoteWindow;
}

class NoteWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit NoteWindow(QString text, QString *rNote, QWidget *parent = 0);
    ~NoteWindow();
   void  closeEvent(QCloseEvent *event);
private:
    Ui::NoteWindow *ui;
    QString * p_note;
};

#endif // NOTEWINDOW_H
