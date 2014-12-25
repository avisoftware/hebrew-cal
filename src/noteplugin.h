/* noteplugin.h
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
#ifndef NOTEPLUGIN_H
#define NOTEPLUGIN_H
#include <QMainWindow>
//#include "mainwindow.h"
//#include "interface.h"
#include "eventlist.h"
#include "functions.h"


 class NotePlugIn
{
public:
     int init();
     void findNotesForMonth(int y, int m, int d);
     void findNotesForMonthSec(int y, int m, int d);
     int getNoteId(QDate d);
     void releseAllNoteFromList();
     int addNote(QDate d);
     int editNote(int id);
     int deleteNote(int i);
     void setForSelection(int id);
 private:

    EventList list;
    QSqlDatabase  db;
    void createDataBase();
    void connectToDataBase();

};



#endif // NOTEPLUGIN_H
