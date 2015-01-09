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
#include <hdate.h>

QString get_int_to_heb_string(int n);
QString get_holyday_string (int holyday, int s);

QString get_parasha_string (int parasha, int s);
QString get_hebrew_month_string (int month, int s);
QString get_month_string (int month, int s);
QString get_day_in_week_string(int dw, int s);

int is_leap_year(int year);
QString getDafHyomi(QDate d);
QString getMasechetName(int n);
int fix_date(hdate_struct *h);
int fix_month(hdate_struct  * h);
int fix_day(hdate_struct  * h);
hdate_struct add_month(int d, int m, int y);
int between_month(int start,int end,int is_leap_year);
#endif // HEBD_H
