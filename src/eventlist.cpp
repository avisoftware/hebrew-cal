/* eventlist.cpp
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
#include "eventlist.h"

void EventList::append(QDate d, int id){
    id_L.append(id);
    Date_L.append(d);

}
void EventList::remove(int index){
    id_L.removeAt(index);
    Date_L.removeAt(index);

}
 void EventList::removeAll(){
     id_L.clear();
     Date_L.clear();

 }

QDate EventList::getFirstDate(){
    if (Date_L.isEmpty ()){
         return QDate();
    }
    return (Date_L.first());
}


