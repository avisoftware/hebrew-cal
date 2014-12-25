/* hebtimes.cpp
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
#ifndef HEBTIMES_H
#define HEBTIMES_H
#include <qdatetime.h>
#include <cassert>


// libhdate
int hdate_get_day_of_year (int day, int month, int year);
void hdate_get_utc_sun_time_deg (int day, int month, int year, double latitude, double longitude, double deg, double *sunrise, double *sunset);
void hdate_get_utc_sun_time(int day, int month, int year, double latitude, double longitude,double high, double *sunrise, double *sunset);
void hdate_get_utc_sun_time_full (int day, int month, int year, double latitude, double longitude,double high,
    double *sun_hour,double * sun_hour_mga,double *first_light_mga, double *first_light, double *talit, double *sunrise,
    double *midday,double *sunset, double *first_stars, double * first_stars_mga, double *three_stars,double *three_stars_czhish);

#endif // HEBTIMES_H
