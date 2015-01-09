/* hebstring.cpp
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
#include "hebD.h"

QString get_int_to_heb_string (int n){
    return QString::fromUtf8(hdate_get_int_string(n));
}

QString get_holyday_string (int holyday, int s){
    return QString::fromUtf8(hdate_get_holyday_string(holyday,s));
}
QString get_parasha_string (int parasha, int s){
    return QString::fromUtf8(hdate_get_parasha_string(parasha,s));
}
QString get_month_string (int month, int s){
    return QString::fromUtf8(hdate_get_month_string(month,s));
}
QString get_hebrew_month_string (int month, int s){
    return QString::fromUtf8(hdate_get_hebrew_month_string(month,s));
}
QString get_day_in_week_string(int dw,int s){
     return QString::fromUtf8(hdate_get_day_string(dw,s));
}

QString getMasechetName(int n){
    static QString masechtosBavli [40]= { "ברכות",
                                          "שבת",
                                          "עירובין",
                                          "פסחים",
                                          "שקלים",
                                          "יומא",
                                          "סוכה",
                                          "ביצה",
                                          "ראש השנה",
                                          "תענית",
                                          "מגילה",
                                          "מועד קטן",
                                          "חגיגה",
                                          "יבמות",
                "כתובות",
                                          "נדרים",
                                          "נזיר",
                                          "סוטה",
                                          "גיטין",
                                          "קידושין",
                                          "בבא קמא",
                                          "בבא מציעא",
                                          "בבא בתרא",
                "סנהדרין",
                                          "מכות",
                                          "שבועות",
                                          "עבודה זרה",
                                          "הוריות",
                                          "זבחים",
                                          "מנחות",
                                          "חולין",
                                          "בכורות",
                "ערכין",
                                          "תמורה",
                                          "כריתות",
                                          "מעילה",
                                          "קינים",
                                          "תמיד",
                                          "מידות",
                                          "נדה"};
    if(n>=0&&n<=40){
         return(masechtosBavli[n]);
    }

    return NULL;
}
