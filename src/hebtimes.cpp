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

#include "hebtimes.h"
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#define M_PI       3.14159265358979323846

// libhdate algorithm
int hdate_get_day_of_year (int day, int month, int year)
{
    int jd;

    /* get todays julian day number */
    jd = (1461 * (year + 4800 + (month - 14) / 12)) / 4 +
        (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12 -
        (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4 + day;

    /* substruct the julian day of 1/1/year and add one */
    jd = jd - ((1461 * (year + 4799)) / 4 +
        367 * 11 / 12 - (3 * ((year + 4899) / 100)) / 4);

    return jd;
}

void hdate_get_utc_sun_time_deg (int day, int month, int year, double latitude, double longitude, double deg, double *sunrise, double *sunset){
        double gama; /* location of sun in yearly cycle in radians */
        double eqtime; /* diffference betwen sun noon and clock noon */
        double decl; /* sun declanation */
        double ha; /* solar hour engle */
        double sunrise_angle = M_PI * deg / 180.0; /* sun angle at sunrise/set */

        int day_of_year;

        /* get the day of year */
        day_of_year = hdate_get_day_of_year (day, month, year);

        /* get radians of sun orbit around erth =) */
        gama = 2.0 * M_PI * ((double)(day_of_year - 1) / 365.0);


        /* get the diff betwen suns clock and wall clock in minutes */
        eqtime = 229.18 * (0.000075 + 0.001868 * cos (gama)
            - 0.032077 * sin (gama) - 0.014615 * cos (2.0 * gama)
            - 0.040849 * sin (2.0 * gama));



        /* calculate suns declanation at the equater in radians */
        decl = 0.006918 - 0.399912 * cos (gama) + 0.070257 * sin (gama)
            - 0.006758 * cos (2.0 * gama) + 0.000907 * sin (2.0 * gama)
            - 0.002697 * cos (3.0 * gama) + 0.00148 * sin (3.0 * gama);

        /* we use radians, ratio is 2pi/360 */
        latitude = M_PI * latitude / 180.0;

        /* the sun real time diff from noon at sunset/rise in radians */

        ha = acos (cos (sunrise_angle) / (cos (latitude) * cos (decl)) - tan (latitude) * tan (decl));

        /* check for too high altitudes and return negative values */


        /* we use minutes, ratio is 1440min/2pi */
        ha = 720.0 * ha / M_PI;

        /* get sunset/rise times in utc wall clock in minutes from 00:00 time */
            //we rounded the value (it close to other calenders time in one minute sometime)

                *sunrise = (720.0 - 4.0 * longitude - ha - eqtime);
                *sunset = (720.0 - 4.0 * longitude + ha - eqtime);

        /* *sunrise = (int)(720.0 - 4.0 * longitude - ha - eqtime);
        *sunset = (int)(720.0 - 4.0 * longitude + ha - eqtime);*/

        return;

}
void hdate_get_utc_sun_time(int day, int month, int year, double latitude, double longitude, double high,double *sunrise, double *sunset){

    double fix_high=0.0;
    double d =0.0;
    double deg ;
    if(high>1.0){
        fix_high=  -2.076 * sqrt(high)/60.0;
        d = 0.833-fix_high;
        deg = 90.000+d;
    }else{
        deg = 90.833;
    }
    hdate_get_utc_sun_time_deg (day, month, year, latitude, longitude, deg, sunrise, sunset);

        return;

}
void hdate_get_utc_sun_time_full (int day, int month, int year, double latitude, double longitude,double high,
    double *sun_hour,double * sun_hour_mga,double *first_light_mga, double *first_light, double *talit, double *sunrise,
    double *midday,double *sunset, double *first_stars, double * first_stars_mga, double *three_stars,double *three_stars_czhish)
{
    double place_holder;
    double fix_high=0.0;
    double d =0.0;
    double deg ;
    if(high>1.0){
        fix_high=  -2.076 * sqrt(high)/60.0;
        d = 0.833-fix_high;
        deg = 90.000+d;
    }else{
        deg = 90.833;
    }

    /* sunset and rise time */
    hdate_get_utc_sun_time_deg (day, month, year, latitude, longitude, deg, sunrise, sunset);

    /* shaa zmanit by gara, 1/12 of light time */
    *sun_hour = (*sunset - *sunrise) / 12;
    *midday = (*sunset + *sunrise) / 2;

    /* get times of the different sun angles */
    hdate_get_utc_sun_time_deg (day, month, year, latitude, longitude, 106.01, first_light, &place_holder);
    hdate_get_utc_sun_time_deg (day, month, year, latitude, longitude, 101.0, talit, &place_holder);
    hdate_get_utc_sun_time_deg (day, month, year, latitude, longitude, 96.0, &place_holder, first_stars);
    hdate_get_utc_sun_time_deg (day, month, year, latitude, longitude, 98.5, &place_holder, three_stars);
    hdate_get_utc_sun_time_deg (day, month, year, latitude, longitude, 99.28, &place_holder, three_stars_czhish);
    /* first light and first stars by the magen avraham*/
    hdate_get_utc_sun_time_deg (day, month, year, latitude, longitude,109.75, first_light_mga, first_stars_mga);
    /* shaa zmanit by magen avraham, 1/12 of light time */
    *sun_hour_mga = (*first_stars_mga  - *first_light_mga) / 12;

    return;
}
