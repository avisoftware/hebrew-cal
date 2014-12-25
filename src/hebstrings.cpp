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

QString hdate_get_int_string (int n)
{
    int length;
    //static char h_number[100];
    QString h_number;
    const char *digits[3][10] = {
        {" ", "א", "ב", "ג", "ד", "ה", "ו", "ז", "ח", "ט"},
        {"ט", "י", "כ", "ל", "מ", "נ", "ס", "ע", "פ", "צ"},
        {" ", "ק", "ר", "ש", "ת"}
    };

    /*if (!hdate_is_hebrew_locale())
    {
        // not hebrew locale return the number in decimal form
        //snprintf (h_number, 100, "%d", n);
        return h_number;
    }*/


    /* sanity checks */
    if (n < 1 || n > 10000)
    {
        return NULL;
    }

        h_number= "";

        if (n >= 1000)
        {
            h_number= h_number + QString::fromUtf8(digits[0][n / 1000]);
            n %= 1000;
        }
        while (n >= 400)
        {
             h_number= h_number +QString::fromUtf8(digits[2][4]);
            n -= 400;
        }
        if (n >= 100)
        {
            h_number= h_number+QString::fromUtf8(digits[2][n / 100]);
            n %= 100;
        }
        if (n >= 10)
        {
            if (n == 15 || n == 16)
                n -= 9;
            h_number=h_number+QString::fromUtf8(digits[1][n / 10]);
            n %= 10;
        }
        if (n > 0)
            h_number=h_number+ QString::fromUtf8(digits[0][n]);

        length = h_number.length() ;

        /* add the ' and " to hebrew numbers */
        if (length <= 1)
        {
            h_number.insert(1,"'");
            //h_number= h_number+ "'";
        }
        else
        {
         //   h_number[length + 1] = h_number[length];
          //  h_number[length] = h_number[length - 1];
          //  h_number[length - 1] = h_number[length - 2];
          //  h_number[length - 2] = '\"';
            h_number.insert(length-1,'\"');
        }

    return h_number;
}


int hdate_is_hebrew_locale()
{
    char *locale;
    char *language;

    /* Get the name of the current locale.  */
#ifdef ENABLE_NLS
    locale = setlocale (LC_MESSAGES, NULL);
    language = getenv ("LANGUAGE");
#else
    locale = NULL;
    language = NULL;
#endif

    if (!((locale && (locale[0] == 'h') && (locale[1] == 'e')) ||
          (language && (language[0] == 'h') && (language[1] == 'e'))))
    {
        /* not hebrew locale return false */
        return 0;
    }

    return -1;
}

QString hdate_get_holyday_string_eng (int holyday, int s)
{
    /* holyday strings */
    static char *holydays[2][37] = {
        {
          ("Rosh Hashana I"),	/* 1 */
          ("Rosh Hashana II"),
          ("Tzom Gedaliah"),
          ("Yom Kippur"),
          ("Sukkot"),
          ("Hol hamoed<br>Sukkot"),
          ("Hoshana raba"),
          ("Simchat Torah"),
          ("Chanukah"),
          ("Asara B'Tevet"),	/* 10 */
          ("Tu B'Shvat"),
          ("Ta'anit Esther"),
          ("Purim"),
          ("Shushan Purim"),
          ("Pesach"),
          ("Hol hamoed Pesach"),
          ("Yom HaAtzma'ut"),
          ("Lag B'Omer"),
          ("Erev Shavuot"),
          ("Shavuot"),			/* 20 */
          ("Tzom Tammuz"),
          ("Tish'a B'Av"),
          ("Tu B'Av"),
          ("Yom HaShoah"),
          ("Yom HaZikaron"),	/* 25 */
          ("Yom Yerushalayim"),
          ("Shmini Atzeret"),
          ("Pesach VII"),
          ("Pesach VIII"),
          ("Shavuot II"),   /* 30 */
          ("Sukkot II"),
          ("Pesach II"),
          ("Family Day"),
          ("Memorial day for fallen whose place of burial is unknown"),
          ("Rabin memorial day"), /* 35 */
          ("Zhabotinsky day"),
          ("Erev Yom Kippur")},
        {
          ("Rosh<br>Hashana I"),
          ("Rosh<br>Hashana II"),
          ("Tzom<br>Gedaliah"),
          ("Yom<br>Kippur"),
          ("Sukkot"),
          ("Hol hamoed <br>Sukkot"),
          ("Hoshana<br>raba"),
          ("Simchat<br>Torah"),
          ("Chanukah"),
          ("Asara<br>B'Tevet"),
          ("Tu B'Shvat"),
          ("Ta'anit<br>Esther"),
          ("Purim"),
          ("Shushan<br>Purim"),
          ("Pesach"),
          ("Hol hamoed<br>Pesach"),
          ("Yom<br>HaAtzma'ut"),
          ("Lag B'Omer"),
          ("Erev<br>Shavuot"),
          ("Shavuot"),
          ("Tzom<br>Tammuz"),
          ("Tish'a<br>B'Av"),
          ("Tu B'Av"),
          ("Yom<br>HaShoah"),
          ("Yom<br>HaZikaron"),
          ("Yom<br>Yerushalayim"),
          ("Shmini<br>Atzeret"),
          ("Pesach VII"),
          ("Pesach VIII"),
          ("Shavuot II"),
          ("Sukkot II"),
          ("Pesach II"),
          ("Family Day"),
          ("Memorial day for fallen<br>whose place of<br>burial is unknown"),
          ("Yitzhak Rabin<br>memorial day"), /* 35 */
          ("Zeev Zhabotinsky day"),
          ("Erev<br>Yom Kippur")}
    };
    /* make sure s is 0 or 1 */
    s = s ? 0 : 1;

    if (holyday >= 1 && holyday <= 37)
    {
        return  (QString::fromUtf8(holydays[s][holyday - 1]));
    }

    /* if not a valid holyday return NULL */
    return NULL;
}

QString hdate_get_holyday_string (int holyday, int s)
{
    /* holyday strings */
    static char *holydays[2][37] = {
        {
          ("א ראש השנה"),
          ("ב ראש השנה"),
          ("צום גדליה"),
          ("יום כיפור"),
          ("סוכות"),
          ("חוה\"מ סוכות"),
          ("הושענה רבה"),
          ("שמחת תורה"),
          ("חנוכה"),
          ("עשרה בטבת"),
          ("ט\"ו בשבט"),
          ("תענית אסתר"),
          ("פורים"),
          ("שושן פורים"),
          ("פסח"),
          ("חוה\"מ פסח"),
          ("יום העצמאות"),
          ("ל\"ג בעומר"),
          ("ערב שבועות"),
          ("שבועות"),
          ("צום תמוז"),
          ("תשעה באב"),
          ("ט\"ו באב"),
          ("יום השואה"),
          ("יום הזכרון"),
          ("יום ירושלים"),
          ("שמיני עצרת"),
          ("שביעי של פסח"),
          ("יו\"ט שני"),
          ("יו\"ט שני"),
          ("יו\"ט שני"),
          ("יו\"ט שני"),
          ("יום המשפחה"),
          ("יום הזכרון לאלו שמקום קבורתם לא נודע"),
          ("יום רבין"),
          ("יום זבוטינסקי"),
          ("ערב יום כיפור")},
        {
          ("א רה\"ש"),
          ("ב רה\"ש"),
          ("צ. גדליה"),
          ("יו\"כ"),
          ("סוכות"),
          ("חוה\"מ"),
          ("הוש\"ר"),
            ("ש\"ת"),
          ("חנוכה"),
          ("צ.טבת"),
          ("ט\"ו בשבט"),
          ("תענית אסתר"),
          ("פורים"),
          ("ש.פורים"),
          ("פסח"),
          ("חוה\"מ"),
          ("י.העצמאות"),
          ("ל\"ג בעומר"),
          ("ע.שבועות"),
          ("שבועות"),
          ("צ. תמוז"),
          ("ט באב"),
          ("ט\"ו באב"),
          ("י. השואה"),
          ("י.הזיכרון"),
          ("י.ירושלים"),
          ("ש. עצרת"),
          ("ש.ש. פסח"),
          ("יו\"ט שני"),
          ("יו\"ט שני"),
          ("יו\"ט שני"),
          ("יו\"ט שני"),
          ("י.השמפחה"),
          ("י.ז.לחללים שלא נודע"),
          ("יום רבין"),
          ("י.זבוטינסקי"),
          ("ע.יום כיפור")}
    };
    /* make sure s is 0 or 1 */
    s = s ? 0 : 1;

    if (holyday >= 1 && holyday <= 37)
    {
        return  (QString::fromUtf8(holydays[s][holyday - 1]));
    }

    /* if not a valid holyday return NULL */
    return NULL;
}

QString hdate_get_parasha_string_eng (int parasha, int s)
{
    /* FIXME: abbrev of parasha */
    static char *parashaot[2][62] = {
        {
           ("none"),
           ("Bereshit"),		/* 1 */
           ("Noach"),
           ("Lech-Lecha"),
           ("Vayera"),
           ("Chayei Sara"),
           ("Toldot"),
           ("Vayetzei"),
           ("Vayishlach"),
           ("Vayeshev"),
           ("Miketz"),			/* 10 */
           ("Vayigash"),
           ("Vayechi"),
           ("Shemot"),
           ("Vaera"),
           ("Bo"),
           ("Beshalach"),
           ("Yitro"),
           ("Mishpatim"),
           ("Terumah"),
           ("Tetzaveh"),		/* 20 */
           ("Ki Tisa"),
           ("Vayakhel"),
           ("Pekudei"),
           ("Vayikra"),
           ("Tzav"),
           ("Shmini"),
           ("Tazria"),
           ("Metzora"),
           ("Achrei Mot"),
           ("Kedoshim"),		/* 30 */
           ("Emor"),
           ("Behar"),
           ("Bechukotai"),
           ("Bamidbar"),
           ("Nasso"),
           ("Beha'alotcha"),
           ("Sh'lach"),
           ("Korach"),
           ("Chukat"),
           ("Balak"),			/* 40 */
           ("Pinchas"),
           ("Matot"),
           ("Masei"),
           ("Devarim"),
           ("Vaetchanan"),
           ("Eikev"),
           ("Re'eh"),
           ("Shoftim"),
           ("Ki Teitzei"),
           ("Ki Tavo"),			/* 50 */
           ("Nitzavim"),
           ("Vayeilech"),
           ("Ha'Azinu"),
           ("Vezot<br>Habracha"),	/* 54 */
           ("Vayakhel<br>Pekudei"),
           ("Tazria<br>Metzora"),
           ("Achrei Mot<br>Kedoshim"),
           ("Behar<br>Bechukotai"),
           ("Chukat<br>Balak"),
           ("Matot<br>Masei"),
           ("Nitzavim<br>Vayeilech")},
        {
           ("none"),
           ("Bereshit"),		/* 1 */
           ("Noach"),
           ("Lech-Lecha"),
           ("Vayera"),
           ("Chayei<br>Sara"),
           ("Toldot"),
           ("Vayetzei"),
           ("Vayishlach"),
           ("Vayeshev"),
           ("Miketz"),			/* 10 */
           ("Vayigash"),
           ("Vayechi"),
           ("Shemot"),
           ("Vaera"),
           ("Bo"),
           ("Beshalach"),
           ("Yitro"),
           ("Mishpatim"),
           ("Terumah"),
           ("Tetzaveh"),		/* 20 */
           ("Ki Tisa"),
           ("Vayakhel"),
           ("Pekudei"),
           ("Vayikra"),
           ("Tzav"),
           ("Shmini"),
           ("Tazria"),
           ("Metzora"),
           ("Achrei Mot"),
           ("Kedoshim"),		/* 30 */
           ("Emor"),
           ("Behar"),
           ("Bechukotai"),
           ("Bamidbar"),
           ("Nasso"),
           ("Beha'alotcha"),
           ("Sh'lach"),
           ("Korach"),
           ("Chukat"),
           ("Balak"),			/* 40 */
           ("Pinchas"),
           ("Matot"),
           ("Masei"),
           ("Devarim"),
           ("Vaetchanan"),
           ("Eikev"),
           ("Re'eh"),
           ("Shoftim"),
           ("Ki Teitzei"),
           ("Ki Tavo"),			/* 50 */
           ("Nitzavim"),
           ("Vayeilech"),
           ("Ha'Azinu"),
           ("Vezot<br>Habracha"),	/* 54 */
           ("Vayakhel<br>Pekudei"),
           ("Tazria<br>Metzora"),
           ("Achrei Mot<br>Kedoshim"),
           ("Behar<br>Bechukotai"),
           ("Chukat<br>Balak"),
           ("Matot<br>Masei"),
           ("Nitzavim<br>Vayeilech")}
    };
    /* make sure s is 0 or 1 */
    s = s ? 0 : 1;

    if (parasha >= 1 && parasha <= 61)
    {
        return (QString::fromUtf8(parashaot[s][parasha]));
    }

    /* if not a valid parasha return NULL */
    return NULL;
}
QString hdate_get_parasha_string (int parasha, int s)
{
    /* FIXME: abbrev of parasha */
    static char *parashaot[2][62] = {
        {
           ("אין"),
           ("בראשית"),
           ("נח"),
           ("לך לך"),
           ("וירא"),
           ("חיי שרה"),
           ("תולדות"),
           ("ויצא"),
           ("וישלח"),
           ("וישב"),
           ("מקץ"),
           ("ויגש"),
           ("ויחי"),
           ("שמות"),
           ("וארא"),
           ("בא"),
           ("בשלח"),
           ("יתרו"),
           ("משפטים"),
           ("תרומה"),
           ("תצווה"),
           ("כי תשא"),
           ("ויקהל"),
           ("פקודי"),
           ("ויקרא"),
           ("צו"),
           ("שמיני"),
           ("תזריע"),
           ("מצורע"),
           ("אחרי מות"),
           ("קדושים"),
           ("אמור"),
           ("בהר"),
           ("בחוקותי"),
           ("במדבר"),
           ("נשא"),
           ("בהעלותך"),
           ("שלח"),
           ("קורח"),
           ("חוקת"),
           ("בלק"),
           ("פנחס"),
           ("מטות"),
           ("מסעי"),
           ("דברים"),
           ("ואתחנן"),
           ("עקב"),
           ("ראה"),
           ("שופטים"),
           ("כי תצא"),
           ("כי תבא"),
           ("ניצבים"),
           ("וילך"),
           ("האזינו"),
           ("וזאת<br>הברכה"),
           ("ויקהל<br>פקודי"),
           ("תזריע<br>מצורע"),
           ("אחרי מות<br>קדושים"),
           ("בהר<br>בחוקותי"),
           ("חוקת<br>בלק"),
           ("מטות<br>מסעי"),
           ("ניצבים<br>וילך")},
        {
           ("none"),
           ("Bereshit"),		/* 1 */
           ("Noach"),
           ("Lech-Lecha"),
           ("Vayera"),
           ("Chayei<br>Sara"),
           ("Toldot"),
           ("Vayetzei"),
           ("Vayishlach"),
           ("Vayeshev"),
           ("Miketz"),			/* 10 */
           ("Vayigash"),
           ("Vayechi"),
           ("Shemot"),
           ("Vaera"),
           ("Bo"),
           ("Beshalach"),
           ("Yitro"),
           ("Mishpatim"),
           ("Terumah"),
           ("Tetzaveh"),		/* 20 */
           ("Ki Tisa"),
           ("Vayakhel"),
           ("Pekudei"),
           ("Vayikra"),
           ("Tzav"),
           ("Shmini"),
           ("Tazria"),
           ("Metzora"),
           ("Achrei Mot"),
           ("Kedoshim"),		/* 30 */
           ("Emor"),
           ("Behar"),
           ("Bechukotai"),
           ("Bamidbar"),
           ("Nasso"),
           ("Beha'alotcha"),
           ("Sh'lach"),
           ("Korach"),
           ("Chukat"),
           ("Balak"),			/* 40 */
           ("Pinchas"),
           ("Matot"),
           ("Masei"),
           ("Devarim"),
           ("Vaetchanan"),
           ("Eikev"),
           ("Re'eh"),
           ("Shoftim"),
           ("Ki Teitzei"),
           ("Ki Tavo"),			/* 50 */
           ("Nitzavim"),
           ("Vayeilech"),
           ("Ha'Azinu"),
           ("Vezot<br>Habracha"),	/* 54 */
           ("Vayakhel<br>Pekudei"),
           ("Tazria<br>Metzora"),
           ("Achrei Mot<br>Kedoshim"),
           ("Behar<br>Bechukotai"),
           ("Chukat<br>Balak"),
           ("Matot<br>Masei"),
           ("Nitzavim<br>Vayeilech")}
    };
    /* make sure s is 0 or 1 */
    s = s ? 0 : 1;

    if (parasha >= 1 && parasha <= 61)
    {
        return (QString::fromUtf8(parashaot[s][parasha]));
    }

    /* if not a valid parasha return NULL */
    return NULL;
}

QString hdate_get_month_string_eng (int month, int s)
{

    static char *months[2][12] = {
        { ("Jan"),  ("Feb"),  ("Mar"),  ("Apr"),  ("May"),
          ("Jun"),  ("Jul"),  ("Aug"),  ("Sep"),  ("Oct"),
          ("Nov"),  ("Dec")},
        { ("January"),  ("February"),  ("March"),
          ("April"),  ("May"),  ("June"),
          ("July"),  ("August"),  ("September"),
          ("October"),  ("November"),  ("December")}
    };

#ifdef ENABLE_NLS
    bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

    /* make sure s is 0 or 1 */
    s = s ? 0 : 1;

    if (month >= 1 && month <= 12)
    {
        return QString::fromUtf8(months[s][month - 1]);
    }

    /* if not a valid month return NULL */
    return NULL;
}
QString hdate_get_month_string (int month, int s)
{

    static char *months[2][12] = {
        { ("Jan"),  ("Feb"),  ("Mar"),  ("Apr"),  ("May"),
          ("Jun"),  ("Jul"),  ("Aug"),  ("Sep"),  ("Oct"),
          ("Nov"),  ("Dec")},
        { ("ינואר"),
          ("פברואר"),
          ("מרץ"),
          ("אפריל"),
          ("מאי"),
          ("יוני"),
          ("יולי"),
          ("אוגוסט"),
          ("ספטמבר"),
           ("אוקטובר"),
          ("נובמבר"),
          ("דצמבר")}
    };

#ifdef ENABLE_NLS
    bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

    /* make sure s is 0 or 1 */
    s = s ? 0 : 1;

    if (month >= 1 && month <= 12)
    {
        return QString::fromUtf8(months[s][month - 1]);
    }

    /* if not a valid month return NULL */
    return NULL;
}

QString hdate_get_hebrew_month_string_eng (int month, int s)
{
    /* FIXME: abbrev of Hebrew months */
    static char *months[2][14] = {
        { ("Tishrei"),  ("Cheshvan"),  ("Kislev"),  ("Tevet"),
          ("Sh'vat"),  ("Adar"),  ("Nisan"),  ("Iyyar"),
          ("Sivan"),  ("Tamuz"),  ("Av"),  ("Elul"),  ("Adar I"),
          ("Adar II")},
        { ("Tishrei"),  ("Cheshvan"),  ("Kislev"),  ("Tevet"),
          ("Sh'vat"),  ("Adar"),  ("Nisan"),  ("Iyyar"),
          ("Sivan"),  ("Tamuz"),  ("Av"),  ("Elul"),  ("Adar I"),
          ("Adar II")}
    };

#ifdef ENABLE_NLS
    bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

    /* make sure s is 0 or 1 */
    s = s ? 0 : 1;

    if (month >= 1 && month <= 14)
    {
        return QString::fromUtf8(months[s][month - 1]);
    }

    /* if not a valid month return NULL */
    return NULL;
}
QString hdate_get_hebrew_month_string (int month, int s)
{
    /* FIXME: abbrev of Hebrew months */
    static char *months[2][14] = {
        { ("Tishrei"),  ("Cheshvan"),  ("Kislev"),  ("Tevet"),
          ("Sh'vat"),  ("Adar"),  ("Nisan"),  ("Iyyar"),
          ("Sivan"),  ("Tamuz"),  ("Av"),  ("Elul"),  ("Adar I"),
          ("Adar II")},
        { ("תשרי"),
          ("חשוון"),
          ("כסלו"),
          ("טבת"),
          ("שבט"),
          ("אדר"),
          ("ניסן"),
          ("אייר"),
          ("סיוון"),
          ("תמוז"),
          ("אב"),
          ("אלול"),
          ("אדר א"),
          ("אדר ב")}
    };

#ifdef ENABLE_NLS
    bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

    /* make sure s is 0 or 1 */
    s = s ? 0 : 1;

    if (month >= 1 && month <= 14)
    {
        return QString::fromUtf8(months[s][month - 1]);
    }

    /* if not a valid month return NULL */
    return NULL;
}
QString hdate_get_day_in_week_string(int dw,int s){
    /* FIXME: abbrev of Hebrew months */
    static char *days[2][14] = {
        { ("Sunday"),  ("Monday"),  ("Tuesday"),  ("Wednesday"),
          ("Thursday"),  ("Friday"),  ("Saturday")},
        { ("ראשון"),
          ("שני"),
          ("שלישי"),
          ("רביעי"),
          ("חמישי"),
          ("שישי"),
          ("שבת")}
    };

#ifdef ENABLE_NLS
    bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

    /* make sure s is 0 or 1 */
    s = s ? 0 : 1;

    if (dw >= 1 && dw <= 14)
    {
        return QString::fromUtf8(days[s][dw - 1]);
    }

    /* if not a valid month return NULL */
    return NULL;
}
QString getMasechetName(int n){
    static char  * masechtosBavli [40]= { "ברכות",
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
         return(QString::fromUtf8(masechtosBavli[n]));
    }

    return NULL;
}
