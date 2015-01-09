/* hebD.cpp
 * Hebrew Calendar based on libhdate - http://libhdate.sourceforge.net
 * Most of this code powerd by the libhdate developers:
 * (C) Boruch Baum  <boruch-baum@users.sourceforge.net> 2011-2013.
 * (C) Yaacov Zamir <kzamir@walla.co.il> 2003-2008.
 * (C) Amos Shapir	 <amos083@hotmail.com> 1984-2003.
 * and base on Zmanim Java API
 * Copyright (C)  Eliyahu Hershfeld 2011
 *
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
#include "hebD.h"
#include <QString>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int is_leap_year(int year){
    int length_of_year;
    int days_from_3744;

    days_from_3744 = hdate_days_from_3744 (year);
    length_of_year = hdate_days_from_3744 (year + 1) - days_from_3744;
    if (length_of_year>365){
        return 1;
    }
    return 0;
}

int fix_date(hdate_struct  * h){
    int c=0;
    if(fix_day(h)){
        c=1;
    }
    if( fix_month(h)){
        c=1;
    }
    return c;
}

int fix_month(hdate_struct  * h){
    int c=0;
    hdate_struct temp;
    hdate_set_gdate(&temp,h->gd_day,h->gd_mon,h->gd_year);

    if((temp.hd_mon!=13&&temp.hd_mon!=14)&&(h->hd_mon==13||h->hd_mon==14)){
        h->hd_mon=6;
        c=1;
    }else if(h->hd_mon==6&&(temp.hd_mon==13||temp.hd_mon==14)){
         h->hd_mon=13;
         c=1;
    }

    hdate_set_hdate(h,h->hd_day,h->hd_mon,h->hd_year);

    return c;
}

int fix_day(hdate_struct  * h){
    int c=0;
    hdate_struct temp;
    hdate_set_gdate(&temp,h->gd_day,h->gd_mon,h->gd_year);

    if(temp.hd_day!=h->hd_day){
        h->hd_day--;
        hdate_set_hdate(h, h->hd_day,h->hd_mon,h->hd_year);//correct the g date after fix the h day
        c=1;
    }
    return c;
}
//add month
hdate_struct add_month(int d, int m, int y){
    hdate_struct h;
    if(m==12){
        y=y+1;
        m=1;
    }else if(is_leap_year(y)&&m==5){
        m=13;
    }else if(m==13){
        m=14;
    }else if(m==14){
        m=7;
    }else{
        m=m+1;
    }
    hdate_set_hdate(&h,d,m,y);
    return(h);
}
int between_month(int start, int end, int is_leap_year){
    if(start==end){
        return 1;
    }
        if(!is_leap_year){
            return(end-start+1);
        }
        if(is_leap_year){
            if(start==13||start==14||end==13||end==14){
                if(start==13&&end==14){
                    return 2;
                }
                if(start==13){
                    return ((end-6)+2);
                }
                if(start==14){
                    return ((end-6)+1);
                }
                if(end==13){
                    return (6-start)+1;
                }
                if(end==14){
                    return (7-start)+1;
                }
            }else if(start<6&&end>7){
                 return(end-start+2);
            }else{
                 return(end-start+1);
            }
        }
}
/*Zmanim Java API
 * Copyright (C) 2011 Eliyahu Hershfeld
 * */
QString getDafHyomi(QDate d){
    QDate daf_start=QDate(1923,9,11);
    int j_daf_start= daf_start.toJulianDay();
    int j_today=d.toJulianDay();
    QDate shekalimChangeDate = QDate(1975,6, 24);
    int shekalimJulianChangeDay = shekalimChangeDate.toJulianDay();

       int dafPerMasechta[]  = { 64, 157, 105, 121, 22, 88, 56, 40, 35, 31, 32, 29, 27, 122, 112, 91, 66, 49, 90, 82,
                    119, 119, 176, 113, 24, 49, 76, 14, 120, 110, 142, 61, 34, 34, 28, 22,4, 10, 4,  73 };
    int cycleNo = 0;
    int dafNo = 0;

     if (j_today>shekalimJulianChangeDay) {
                 cycleNo = 8 + ((j_today - shekalimJulianChangeDay) / 2711);
                 dafNo = ((j_today - shekalimJulianChangeDay) % 2711);
             } else {
                 cycleNo = 1 + ((j_today - j_daf_start) / 2702);
                 dafNo = ((j_today - j_daf_start) % 2702);
             }


     int total = 0;
     int masechta = -1;
     int daf = 0;
     for (int j = 0; j <40 ; j++) {
                 masechta++;
                 total = total + dafPerMasechta[j] - 1;
                 if (dafNo < total) {
                     daf = 1 + dafPerMasechta[j] - (total - dafNo);
                     /* Fiddle with the weird ones near the end. */
                     if (masechta == 36) {
                         daf += 21;
                     } else if (masechta == 37) {
                         daf += 24;
                     } else if (masechta == 38) {
                         daf += 33;
                     }
                     break;
                 }
     }
     return (QString (getMasechetName(masechta)+" "+get_int_to_heb_string(daf)));

}
