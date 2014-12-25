/* hebD.h
 * Hebrew Calendar based on libhdate http://libhdate.sourceforge.net
 * Most of this code powerd by the libhdate developers:
 * (C) Boruch Baum  <boruch-baum@users.sourceforge.net> 2011-2013.
 * (C) Yaacov Zamir <kzamir@walla.co.il> 2003-2008.
 * (C) Amos Shapir	 <amos083@hotmail.com> 1984-2003.
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
#ifndef HEBD_H
#define HEBD_H

#include <QString>
#include <QDate>
/** @struct hdate_struct
  @brief libhdate Hebrew date struct
*/
typedef struct
{
    /** The number of day in the hebrew month (1..31). */
    int hd_day;
    /** The number of the hebrew month 1..14 (1 - tishre, 13 - adar 1, 14 - adar 2). */
    int hd_mon;
    /** The number of the hebrew year. */
    int hd_year;
    /** The number of the day in the month. (1..31) */
    int gd_day;
    /** The number of the month 1..12 (1 - jan). */
    int gd_mon;
    /** The number of the year. */
    int gd_year;
    /** The day of the week 1..7 (1 - sunday). */
    int hd_dw;
    /** The length of the year in days. */
    int hd_size_of_year;
    /** The week day of Hebrew new year. */
    int hd_new_year_dw;
    /** The number type of year. */
    int hd_year_type;
    /** The Julian day number */
    int hd_jd;
    /** The number of days passed since 1 tishrey */
    int hd_days;
    /** The number of weeks passed since 1 tishrey */
    int hd_weeks;
} hdate_struct;

hdate_struct *
hdate_set_hdate (hdate_struct *h, int d, int m, int y);

int
hdate_hdate_to_jd (int day, int month, int year, int *jd_tishrey1, int *jd_tishrey1_next_year);

int
hdate_days_from_3744 (int hebrew_year);
void
hdate_jd_to_gdate (int jd, int *day, int *month, int *year);
hdate_struct *
hdate_set_gdate (hdate_struct *h, int d, int m, int y);
int
hdate_gdate_to_jd (int day, int month, int year);
void
hdate_jd_to_hdate (int jd, int *day, int *month, int *year, int *jd_tishrey1, int *jd_tishrey1_next_year);
int
hdate_get_year_type (int size_of_year, int new_year_dw);
hdate_struct *
hdate_set_jd (hdate_struct *h, int jd);
int
hdate_get_holyday_type (int holyday);
int hdate_get_omer_day(hdate_struct const * h);
int
hdate_get_holyday (hdate_struct const * h, int diaspora);
QString hdate_get_int_string(int n);
int
hdate_is_hebrew_locale();
QString
hdate_get_holyday_string (int holyday, int s);
int
hdate_get_parasha (hdate_struct const * h, int diaspora);

QString hdate_get_parasha_string (int parasha, int s);
QString hdate_get_hebrew_month_string (int month, int s);
QString hdate_get_month_string (int month, int s);
QString hdate_get_day_in_week_string(int dw, int s);

int is_leap_year(int year);
QString getDafHyomi(QDate d);
QString getMasechetName(int n);
int fix_date(hdate_struct *h);
int fix_month(hdate_struct  * h);
int fix_day(hdate_struct  * h);
hdate_struct add_month(int d, int m, int y);
int between_month(int start,int end,int is_leap_year);
#endif // HEBD_H
