/* eventlist.h
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
#ifndef EVENTLIST_H
#define EVENTLIST_H
#include <QDate>

class EventList
{
public:
    void append(QDate d, int id);
    void remove(int index);
    void removeAll();
    QDate getFirstDate();
    void sort();
private:
    QList <int> id_L;
    QList <QDate> Date_L;

    QList <int> day_L;
    QList <int> night_L;
};

#endif // EVENTLIST_H
