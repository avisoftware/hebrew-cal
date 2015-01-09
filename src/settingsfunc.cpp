/* settingsfunc.cpp
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
#include "settingsfunc.h"
#include "functions.h"
void write_setting(QString g,QString s , QVariant value){
    QSettings settings("avisoftware", "hebrew-cal");
         settings.beginGroup(g);
         settings.setValue(s, value);
         settings.endGroup();
}
QVariant read_setting(QString g,QString s ){
    QSettings settings("avisoftware", "hebrew-cal");
         settings.beginGroup(g);
         QVariant a =settings.value(s);
         settings.endGroup();
      return   a;
}
void reset_settings( QSqlDatabase  db){

    write_setting("","version",APP_VERSION);
    write_setting("cal","calby",0);
    write_setting("cal","diaspora",false);
    write_setting("cal","IsraelHoliday",true);
    write_setting("cal","dafHyomi",true);

    QSqlQuery q(db) ;
    q.exec("SELECT * FROM location");
    qDebug()<< q.lastError().text()<<" reset_settings line: 26";
    q.first();
    write_setting("times","location_heb_name",q.value(0).toString());
    write_setting("times","location_eng_name",q.value(1).toString());
    write_setting("times","location_region",q.value(2).toString());
    write_setting("times","location_longitude",q.value(3).toString());
    write_setting("times","location_latitude",q.value(4).toString());
    write_setting("times","location_time_zone",q.value(5).toString());
     write_setting("times","location_height",q.value(6).toString());    
    write_setting("times","location_candle_light",q.value(7).toString());
    write_setting("times","SummerTime",-1);
    write_setting("times","FormatTime","h:mm");
    write_setting("times","location_consider_height",false);

    write_setting("sefirat_homer","nosach",0);

    write_setting("view","MainBackgroundColor","#f3f3f3");
    write_setting("view","MainTextColor","#5a5a5a");
    write_setting("view","TableHeaderBackgroundColor","#7fb468");
    write_setting("view","MainGridColor","#e6aaab");
     write_setting("view","MainSelectorColor","#ffffca");
     write_setting("view","DateBackgroundColor","#b8e8e4");
     write_setting("view","DateBackgroundHolydayColor","#cac7c7");
     write_setting("view","DateParashaColor","#8a8c01");
     write_setting("view","DateHolydayColor","#880000");
     write_setting("view","DateTextColor","#022ff7");
     write_setting("view","DateTextHolydayColor","#002d81");


      write_setting("view_times","alot_mga",true);
     write_setting("view_times","alot",true);
     write_setting("view_times","talit",true);
     write_setting("view_times","zricha",true);
      write_setting("view_times","end_shema_mga",true);
     write_setting("view_times","end_shema",true);
     write_setting("view_times","end_tefila_mga",true);
     write_setting("view_times","end_tefila",true);
     write_setting("view_times","midday",true);
     write_setting("view_times","mincha_gdola",true);
     write_setting("view_times","mincha_ktana",true);
     write_setting("view_times","plug_hamincha",true);
     write_setting("view_times","shkiha",true);
     write_setting("view_times","shabat",true);
     write_setting("view_times","shabatOutRT",true);
         write_setting("notes","enable",true);
          write_setting("notes","Color","#e6aaab");


}
