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


#define HOUR 1080
#define DAY  (24*HOUR)
#define WEEK (7*DAY)
#define M(h,p) ((h)*HOUR+p)
#define MONTH (DAY+M(12,793))	/* Tikun for regular month */

int hdate_days_from_3744 (int hebrew_year)
{
    int years_from_3744;
    int molad_3744;
    int leap_months;
    int leap_left;
    int months;
    int parts;
    int days;
    int parts_left_in_week;
    int parts_left_in_day;
    int week_day;

    /* Start point for calculation is Molad new year 3744 (16BC) */
    years_from_3744 = hebrew_year - 3744;
    molad_3744 = M (1 + 6, 779);	/* Molad 3744 + 6 hours in parts */

    /* Time in months */
    leap_months = (years_from_3744 * 7 + 1) / 19;	/* Number of leap months */
    leap_left = (years_from_3744 * 7 + 1) % 19;	/* Months left of leap cycle */
    months = years_from_3744 * 12 + leap_months;	/* Total Number of months */

    /* Time in parts and days */
    parts = months * MONTH + molad_3744;	/* Molad This year + Molad 3744 - corections */
    days = months * 28 + parts / DAY - 2;	/* 28 days in month + corections */

    /* Time left for round date in corections */
    parts_left_in_week = parts % WEEK;	/* 28 % 7 = 0 so only corections counts */
    parts_left_in_day = parts % DAY;
    week_day = parts_left_in_week / DAY;

    /* Special cases of Molad Zaken */
    if ((leap_left < 12 && week_day == 3
         && parts_left_in_day >= M (9 + 6, 204)) ||
        (leap_left < 7 && week_day == 2
         && parts_left_in_day >= M (15 + 6, 589)))
    {
        days++, week_day++;
    }

    /* ADU */
    if (week_day == 1 || week_day == 4 || week_day == 6)
    {
        days++;
    }

    return days;
}

int hdate_hdate_to_jd (int day, int month, int year, int *jd_tishrey1, int *jd_tishrey1_next_year)
{
    int length_of_year;
    int jd;
    int days_from_3744;

    /* Adjust for leap year */
    if (month == 13)
    {
        month = 6;
    }
    if (month == 14)
    {
        month = 6;
        day += 30;
    }

    /* Calculate days since 1,1,3744 */
    days_from_3744 = hdate_days_from_3744 (year);
    day = days_from_3744 + (59 * (month - 1) + 1) / 2 + day;

    /* length of year */
    length_of_year = hdate_days_from_3744 (year + 1) - days_from_3744;

    /* Special cases for this year */
    if (length_of_year % 10 > 4 && month > 2)	/* long Heshvan */
        day++;
    if (length_of_year % 10 < 4 && month > 3)	/* short Kislev */
        day--;
    if (length_of_year > 365 && month > 6)	/* leap year */
        day += 30;

    /* adjust to julian */
    jd = day + 1715118;

    /* return the 1 of tishrey julians */
    if (jd_tishrey1 && jd_tishrey1_next_year)
    {
        *jd_tishrey1 = days_from_3744 + 1715119;
        *jd_tishrey1_next_year = *jd_tishrey1 + length_of_year;
    }

    return jd;
}
void hdate_jd_to_gdate (int jd, int *d, int *m, int *y)
{
    int l, n, i, j;
    l = jd + 68569;
    n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    i = (4000 * (l + 1)) / 1461001;	/* that's 1,461,001 */
    l = l - (1461 * i) / 4 + 31;
    j = (80 * l) / 2447;
    *d = l - (2447 * j) / 80;
    l = j / 11;
    *m = j + 2 - (12 * l);
    *y = 100 * (n - 49) + i + l;	/* that's a lower-case L */

    return;
}
hdate_struct * hdate_set_hdate (hdate_struct * h, int d, int m, int y)
{
    int jd;
    int jd_tishrey1, jd_tishrey1_next_year;

    if (!h) return NULL;

    h->hd_day = d;
    h->hd_mon = m;
    h->hd_year = y;

    jd = hdate_hdate_to_jd (d, m, y, &jd_tishrey1, &jd_tishrey1_next_year);
    hdate_jd_to_gdate (jd, &(h->gd_day), &(h->gd_mon), &(h->gd_year));

    h->hd_dw = (jd + 1) % 7 + 1;
    h->hd_size_of_year = jd_tishrey1_next_year - jd_tishrey1;
    h->hd_new_year_dw = (jd_tishrey1 + 1) % 7 + 1;
    h->hd_year_type = hdate_get_year_type (h->hd_size_of_year , h->hd_new_year_dw);
    h->hd_jd = jd;
    h->hd_days = jd - jd_tishrey1 + 1;
    h->hd_weeks = ((h->hd_days - 1) + (h->hd_new_year_dw - 1)) / 7 + 1;

    return (h);
}
void hdate_jd_to_hdate (int jd, int *day, int *month, int *year, int *jd_tishrey1, int *jd_tishrey1_next_year)
{
    int days;
    int size_of_year;
    int internal_jd_tishrey1, internal_jd_tishrey1_next_year;

    /* calculate Gregorian date */
    hdate_jd_to_gdate (jd, day, month, year);

    /* Guess Hebrew year is Gregorian year + 3760 */
    *year = *year + 3760;

    internal_jd_tishrey1 = hdate_days_from_3744 (*year) + 1715119;
    internal_jd_tishrey1_next_year = hdate_days_from_3744 (*year + 1) + 1715119;

    /* Check if computed year was underestimated */
    if (internal_jd_tishrey1_next_year <= jd)
    {
        *year = *year + 1;
        internal_jd_tishrey1 = internal_jd_tishrey1_next_year;
        internal_jd_tishrey1_next_year = hdate_days_from_3744 (*year + 1) + 1715119;
    }

    size_of_year = internal_jd_tishrey1_next_year - internal_jd_tishrey1;

    /* days into this year, first month 0..29 */
    days = jd - internal_jd_tishrey1;

    /* last 8 months allways have 236 days */
    if (days >= (size_of_year - 236)) /* in last 8 months */
    {
        days = days - (size_of_year - 236);
        *month = days * 2 / 59;
        *day = days - (*month * 59 + 1) / 2 + 1;

        *month = *month + 4 + 1;

        /* if leap */
        if (size_of_year > 355 && *month <=6)
            *month = *month + 8;
    }
    else /* in 4-5 first months */
    {
        /* Special cases for this year */
        if (size_of_year % 10 > 4 && days == 59) /* long Heshvan (day 30 of Heshvan) */
            {
                *month = 1;
                *day = 30;
            }
        else if (size_of_year % 10 > 4 && days > 59) /* long Heshvan */
            {
                *month = (days - 1) * 2 / 59;
                *day = days - (*month * 59 + 1) / 2;
            }
        else if (size_of_year % 10 < 4 && days > 87) /* short kislev */
            {
                *month = (days + 1) * 2 / 59;
                *day = days - (*month * 59 + 1) / 2 + 2;
            }
        else /* regular months */
            {
                *month = days * 2 / 59;
                *day = days - (*month * 59 + 1) / 2 + 1;
            }

        *month = *month + 1;
    }

    /* return the 1 of tishrey julians */
    if (jd_tishrey1 && jd_tishrey1_next_year)
    {
        *jd_tishrey1 = internal_jd_tishrey1;
        *jd_tishrey1_next_year = internal_jd_tishrey1_next_year;
    }

    return;
}

int hdate_gdate_to_jd (int day, int month, int year)
{
    int a;
    int y;
    int m;
    int jdn;

    a = (14 - month) / 12;
    y = year + 4800 - a;
    m = month + 12 * a - 3;

    jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;

    return jdn;
}
int hdate_get_year_type (int size_of_year, int new_year_dw)
{
    /* Only 14 combinations of size and week day are posible */
    static int year_types[24] =
        {1, 0, 0, 2, 0, 3, 4, 0, 5, 0, 6, 7,
        8, 0, 9, 10, 0, 11, 0, 0, 12, 0, 13, 14};

    int offset;

    /* convert size and first day to 1..24 number */
    /* 2,3,5,7 -> 1,2,3,4 */
    /* 353, 354, 355, 383, 384, 385 -> 0, 1, 2, 3, 4, 5 */
    offset = (new_year_dw + 1) / 2;
    offset = offset + 4 * ((size_of_year % 10 - 3) + (size_of_year / 10 - 35));

    /* some combinations are imposible */
    return year_types[offset - 1];
}
hdate_struct * hdate_set_gdate (hdate_struct * h, int d, int m, int y)
{
    int jd;
    int jd_tishrey1, jd_tishrey1_next_year;

    if (!h) return NULL;

    /* check for null dates (kobi) */
    if ((d == 0) || (m == 0))
    {
        struct tm *tm;
        //long t;
        /* FIXME: day start at 6:00 or 12:00 like in Gregorian cal. ? */
        const time_t  t = time (0);//// שיניתי
       // t = time (0);
         tm = localtime (&t);
        d = tm->tm_mday;
        m = tm->tm_mon + 1;
        y = tm->tm_year + 1900;
    }

    h->gd_day = d;
    h->gd_mon = m;
    h->gd_year = y;

    jd = hdate_gdate_to_jd (d, m, y);
    hdate_jd_to_hdate (jd, &(h->hd_day), &(h->hd_mon), &(h->hd_year), &jd_tishrey1, &jd_tishrey1_next_year);

    h->hd_dw = (jd + 1) % 7 + 1;
    h->hd_size_of_year = jd_tishrey1_next_year - jd_tishrey1;
    h->hd_new_year_dw = (jd_tishrey1 + 1) % 7 + 1;
    h->hd_year_type = hdate_get_year_type (h->hd_size_of_year , h->hd_new_year_dw);
    h->hd_jd = jd;
    h->hd_days = jd - jd_tishrey1 + 1;
    h->hd_weeks = ((h->hd_days - 1) + (h->hd_new_year_dw - 1)) / 7 + 1;

    return (h);
}
hdate_struct * hdate_set_jd (hdate_struct * h, int jd)
{
    int jd_tishrey1, jd_tishrey1_next_year;

    if (!h) return NULL;

    hdate_jd_to_gdate (jd, &(h->gd_day), &(h->gd_mon), &(h->gd_year));
    hdate_jd_to_hdate (jd, &(h->hd_day), &(h->hd_mon), &(h->hd_year), &jd_tishrey1, &jd_tishrey1_next_year);

    h->hd_dw = (jd + 1) % 7 + 1;
    h->hd_size_of_year = jd_tishrey1_next_year - jd_tishrey1;
    h->hd_new_year_dw = (jd_tishrey1 + 1) % 7 + 1;
    h->hd_year_type = hdate_get_year_type (h->hd_size_of_year , h->hd_new_year_dw);
    h->hd_jd = jd;
    h->hd_days = jd - jd_tishrey1 + 1;
    h->hd_weeks = ((h->hd_days - 1) + (h->hd_new_year_dw - 1)) / 7 + 1;

    return (h);
}


int hdate_get_holyday_type (int holyday)
{
    int holyday_type;

    switch (holyday)
    {
    case 0: /* regular day */
        holyday_type = 0;
        break;

    case 1:
    case 2:
    case 4: /* FIXME: yom kipur is a yom tov ? */
    case 5:
    case 8:
    case 15:
    case 20:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32: /* Yom tov, To find erev yom tov, check if tomorrow returns 1 */
        holyday_type = 1;
        break;

    case 37: /* Erev yom kippur */
        holyday_type = 2;
        break;

    case 6:
    case 7:
    case 16: /* Hol hamoed */
        holyday_type = 3;
        break;

    case 9:
    case 13:
    case 14: /* Hanuka and purim */
        holyday_type = 4;
        break;

    case 3:
    case 10:
    case 12:
    case 21:
    case 22: /* tzom */
        holyday_type = 5;
        break;

    case 17:
    case 26: /* Independance day and Yom yerushalaim */
        holyday_type = 6;
        break;

    case 18:
    case 23:
    case 11: /* Lag baomer ,Tu beav, Tu beshvat */
        holyday_type = 7;
        break;

    case 24:
    case 25: /* Tzahal and Holocaust memorial days */
        holyday_type = 8;
        break;


    default: /* National days */
        holyday_type = 9;
        break;
    }

    return holyday_type;
}
int hdate_get_holyday (hdate_struct const * h, int diaspora)
{
    int holyday;

    /* holydays table */
    static int holydays_table[14][30] =
    {
        {	/* Tishrey */
            1, 2, 3, 3, 0, 0, 0, 0, 37, 4,
            0, 0, 0, 0, 5, 31, 6, 6, 6, 6,
            7, 27, 8, 0, 0, 0, 0, 0, 0, 0},
        {	/* Heshvan */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 35,
            35, 35, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {	/* Kislev */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 9, 9, 9, 9, 9, 9},
        {	/* Tevet */
            9, 9, 9, 0, 0, 0, 0, 0, 0, 10,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {	/* Shvat */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 11, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 33},
        {	/* Adar */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            12, 0, 12, 13, 14, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {	/* Nisan */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 15, 32, 16, 16, 16, 16,
            28, 29, 0, 0, 0, 24, 24, 24, 0, 0},
        {	/* Iyar */
            0, 17, 17, 17, 17, 17, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 18, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 26, 0, 0},
        {	/* Sivan */
            0, 0, 0, 0, 19, 20, 30, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {	/* Tamuz */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 21, 21, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 36, 36},
        {	/* Av */
            0, 0, 0, 0, 0, 0, 0, 0, 22, 22,
            0, 0, 0, 0, 23, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {	/* Elul */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {	/* Adar 1 */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {	/* Adar 2 */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            12, 0, 12, 13, 14, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    /* sanity check */
    if (h->hd_mon < 1 || h->hd_mon > 14 || h->hd_day < 1 || h->hd_day > 31)
        return 0;

    holyday = holydays_table[h->hd_mon - 1][h->hd_day - 1];

    /* if tzom on sat delay one day */
    /* tzom gdalyaho on sat */
    if ((holyday == 3) && (h->hd_dw == 7 || (h->hd_day == 4 && h->hd_dw !=1)))
        holyday = 0;
    /* 17 of Tamuz on sat */
    if ((holyday == 21) && ((h->hd_dw == 7) || (h->hd_day == 18 && h->hd_dw != 1)))
        holyday = 0;
    /* 9 of Av on sat */
    if ((holyday == 22) && ((h->hd_dw == 7) || (h->hd_day == 10 && h->hd_dw != 1)))
        holyday = 0;

    /* Hanukah in a long year */
    if ((holyday == 9) && (h->hd_size_of_year % 10 != 3) && (h->hd_day == 3))
        holyday = 0;

    /* if tanit ester on sat mov to Thu */
    if ((holyday == 12) && ((h->hd_dw == 7) || (h->hd_day == 11 && h->hd_dw != 5)))
        holyday = 0;

    /* yom yerushalym after 68 */
    if (holyday == 26)
    {
        if (h->gd_year < 1968)
            holyday = 0;
    }

    /* yom ha azmaot and yom ha zicaron */
    if (holyday == 17)
    {
        if (h->gd_year < 1948)
            holyday = 0;
        else if (h->gd_year < 2004)
        {
            if ((h->hd_day == 3) && (h->hd_dw == 5))
                holyday = 17;
            else if ((h->hd_day == 4) && (h->hd_dw == 5))
                holyday = 17;
            else if ((h->hd_day == 5) && (h->hd_dw != 6 && h->hd_dw != 7))
                holyday = 17;
            else if ((h->hd_day == 2) && (h->hd_dw == 4))
                holyday = 25;
            else if ((h->hd_day == 3) && (h->hd_dw == 4))
                holyday = 25;
            else if ((h->hd_day == 4) && (h->hd_dw != 5 && h->hd_dw != 6))
                holyday = 25;
            else
                holyday = 0;
        }
        else
        {
            if ((h->hd_day == 3) && (h->hd_dw == 5))
                holyday = 17;
            else if ((h->hd_day == 4) && (h->hd_dw == 5))
                holyday = 17;
            else if ((h->hd_day == 6) && (h->hd_dw == 3))
                holyday = 17;
            else if ((h->hd_day == 5) && (h->hd_dw != 6 && h->hd_dw != 7 && h->hd_dw != 2))
                holyday = 17;
            else if ((h->hd_day == 2) && (h->hd_dw == 4))
                holyday = 25;
            else if ((h->hd_day == 3) && (h->hd_dw == 4))
                holyday = 25;
            else if ((h->hd_day == 5) && (h->hd_dw == 2))
                holyday = 25;
            else if ((h->hd_day == 4) && (h->hd_dw != 5 && h->hd_dw != 6 && h->hd_dw != 1))
                holyday = 25;
            else
                holyday = 0;
        }
    }

    /* yom ha shoaa, on years after 1958 */
    if (holyday == 24)
    {
        if (h->gd_year < 1958)
            holyday = 0;
        else
        {
            if ((h->hd_day == 26) && (h->hd_dw != 5))
                holyday = 0;
            if ((h->hd_day == 28) && (h->hd_dw != 2))
                holyday = 0;
            if ((h->hd_day == 27) && (h->hd_dw == 6 || h->hd_dw == 1))
                holyday = 0;
        }
    }

    /* Rabin day, on years after 1997 */
    if (holyday == 35)
    {
        if (h->gd_year < 1997)
            holyday = 0;
        else
        {
            if ((h->hd_day == 10 || h->hd_day == 11) && (h->hd_dw != 5))
                holyday = 0;
            if ((h->hd_day == 12) && (h->hd_dw == 6 || h->hd_dw == 7))
                holyday = 0;
        }
    }

    /* Zhabotinsky day, on years after 2005 */
    if (holyday == 36)
    {
        if (h->gd_year < 2005)
            holyday = 0;
        else
        {
            if ((h->hd_day == 30) && (h->hd_dw != 1))
                holyday = 0;
            if ((h->hd_day == 29) && (h->hd_dw == 7))
                holyday = 0;
        }
    }

    /* diaspora holidays */

    /* simchat tora only in diaspora in israel just one day shmini+simchat tora */
    if (holyday == 8 && !diaspora)
        holyday = 0;

    /* sukkot II holiday only in diaspora */
        if (holyday == 31 && !diaspora)
                holyday = 6;

    /* pesach II holiday only in diaspora */
        if (holyday == 32 && !diaspora)
                holyday = 16;

    /* shavot II holiday only in diaspora */
    if (holyday == 30 && !diaspora)
        holyday = 0;

    /* pesach VIII holiday only in diaspora */
    if (holyday == 29 && !diaspora)
        holyday = 0;

    return holyday;
}


int hdate_get_parasha (const hdate_struct *h, int diaspora)
{
    static int join_flags[2][14][7] =
    {
        {
            {1, 1, 1, 1, 0, 1, 1}, /* 1 be erez israel */
            {1, 1, 1, 1, 0, 1, 0}, /* 2 */
            {1, 1, 1, 1, 0, 1, 1}, /* 3 */
            {1, 1, 1, 0, 0, 1, 0}, /* 4 */
            {1, 1, 1, 1, 0, 1, 1}, /* 5 */
            {0, 1, 1, 1, 0, 1, 0}, /* 6 */
            {1, 1, 1, 1, 0, 1, 1}, /* 7 */
            {0, 0, 0, 0, 0, 1, 1}, /* 8 */
            {0, 0, 0, 0, 0, 0, 0}, /* 9 */
            {0, 0, 0, 0, 0, 1, 1}, /* 10 */
            {0, 0, 0, 0, 0, 0, 0}, /* 11 */
            {0, 0, 0, 0, 0, 0, 0}, /* 12 */
            {0, 0, 0, 0, 0, 0, 1}, /* 13 */
            {0, 0, 0, 0, 0, 1, 1}  /* 14 */
        },
        {
            {1, 1, 1, 1, 0, 1, 1}, /* 1 in diaspora */
            {1, 1, 1, 1, 0, 1, 0}, /* 2 */
            {1, 1, 1, 1, 1, 1, 1}, /* 3 */
            {1, 1, 1, 1, 0, 1, 0}, /* 4 */
            {1, 1, 1, 1, 1, 1, 1}, /* 5 */
            {0, 1, 1, 1, 0, 1, 0}, /* 6 */
            {1, 1, 1, 1, 0, 1, 1}, /* 7 */
            {0, 0, 0, 0, 1, 1, 1}, /* 8 */
            {0, 0, 0, 0, 0, 0, 0}, /* 9 */
            {0, 0, 0, 0, 0, 1, 1}, /* 10 */
            {0, 0, 0, 0, 0, 1, 0}, /* 11 */
            {0, 0, 0, 0, 0, 1, 0}, /* 12 */
            {0, 0, 0, 0, 0, 0, 1}, /* 13 */
            {0, 0, 0, 0, 1, 1, 1}  /* 14 */
        }
    };

    int reading;

    /* if simhat tora return vezot habracha */
    if (h->hd_mon == 1)
    {
        /* simhat tora is a day after shmini atzeret outsite israel */
        if (h->hd_day == 22 && !diaspora) return 54;
        if (h->hd_day == 23 && diaspora) return 54;
    }


    if (h->hd_mon == 1 && h->hd_day == 22)
    {
        return 54;
    }

    /* if not shabat return none */
    if (h->hd_dw != 7)
    {
        return 0;
    }

    /* check for diaspora readings */
    diaspora = diaspora?1:0;

    switch (h->hd_weeks)
    {
    case  1:
        if (h->hd_new_year_dw == 7)
        {
            /* Rosh hashana */
            return 0;
        }
        else if ((h->hd_new_year_dw == 2) || (h->hd_new_year_dw == 3))
        {
            return 52;
        }
        else /* if (h->hd_new_year_dw == 5) */
        {
            return 53;
        }
        break;
    case  2:
        if (h->hd_new_year_dw == 5)
        {
            /* Yom kippur */
            return 0;
        }
        else
        {
            return 53;
        }
        break;
    case  3:
        /* Succot */
        return 0;
        break;
    case  4:
        if (h->hd_new_year_dw == 7)
        {
            /* Simhat tora in israel */
            if (!diaspora) return 54;
            /* Not simhat tora in diaspora */
            else return 0;
        }
        else
        {
            return 1;
        }
        break;
    default:
        /* simhat tora on week 4 bereshit too */
        reading = h->hd_weeks - 3;

        /* was simhat tora on shabat ? */
        if (h->hd_new_year_dw == 7)
            reading = reading - 1;

        /* no joining */
        if (reading < 22)
        {
            return reading;
        }

        /* pesach */
        if ((h->hd_mon == 7) && (h->hd_day > 14))
        {
            /* Shmini of pesach in diaspora is on the 22 of the month*/
            if (diaspora && (h->hd_day <= 22))
                return 0;
            if (!diaspora && (h->hd_day < 22))
                return 0;
        }

        /* Pesach allways removes one */
        if (((h->hd_mon == 7) && (h->hd_day > 21)) || (h->hd_mon > 7 && h->hd_mon < 13))
        {
            reading--;

            /* on diaspora, shmini of pesach may fall on shabat if next new year is on shabat */
            if (diaspora &&
                (((h->hd_new_year_dw + h->hd_size_of_year) % 7) == 2))
            {
                reading--;
            }
        }

        /* on diaspora, shavot may fall on shabat if next new year is on shabat */
        if (diaspora &&
            (h->hd_mon < 13) &&
            ((h->hd_mon > 9) || (h->hd_mon == 9 && h->hd_day >= 7)) &&
            ((h->hd_new_year_dw + h->hd_size_of_year) % 7) == 0)
        {
            if (h->hd_mon == 9 && h->hd_day == 7)
            {
                return 0;
            }
            else
            {
                reading--;
            }
        }

        /* joining */
        if (join_flags[diaspora][h->hd_year_type - 1][0] && (reading >= 22))
        {
            if (reading == 22)
            {
                return 55;
            }
            else
            {
                reading++;
            }
        }
        if (join_flags[diaspora][h->hd_year_type - 1][1] && (reading >= 27))
        {
            if (reading == 27)
            {
                return 56;
            }
            else
            {
                reading++;
            }
        }
        if (join_flags[diaspora][h->hd_year_type - 1][2] && (reading >= 29))
        {
            if (reading == 29)
            {
                return 57;
            }
            else
            {
                reading++;
            }
        }
        if (join_flags[diaspora][h->hd_year_type - 1][3] && (reading >= 32))
        {
            if (reading == 32)
            {
                return 58;
            }
            else
            {
                reading++;
            }
        }

        if (join_flags[diaspora][h->hd_year_type - 1][4] && (reading >= 39))
        {
            if (reading == 39)
            {
                return 59;
            }
            else
            {
                reading++;
            }
        }
        if (join_flags[diaspora][h->hd_year_type - 1][5] && (reading >= 42))
        {
            if (reading == 42)
            {
                return 60;
            }
            else
            {
                reading++;
            }
        }
        if (join_flags[diaspora][h->hd_year_type - 1][6] && (reading >= 51))
        {
            if (reading == 51)
            {
                return 61;
            }
            else
            {
                reading++;
            }
        }
        break;
    }

    return reading;
}
int hdate_get_omer_day(hdate_struct const * h)
{
    int omer_day;
    hdate_struct sixteen_nissan;

    hdate_set_hdate(&sixteen_nissan, 16, 7, h->hd_year);
    omer_day = h->hd_jd - sixteen_nissan.hd_jd + 1;

    if ((omer_day > 49) || (omer_day < 0))
        omer_day = 0;

    return omer_day;
}
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
     return (QString (getMasechetName(masechta)+" "+hdate_get_int_string(daf)));

}
