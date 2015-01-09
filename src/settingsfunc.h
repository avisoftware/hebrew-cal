/* settingsfunc.h
 * Hebrew Calendar based on libhdate http://libhdate.sourceforge.net
 *Copyright: (c) Avi Software <avisoftware123@gmail.com> 2014
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
#ifndef SETTINGSFUNC_H
#define SETTINGSFUNC_H
#include <QtCore>
#include <QtSql>

void write_setting(QString g,QString s , QVariant value);
QVariant read_setting(QString g,QString s );
void reset_settings(QSqlDatabase db = QSqlDatabase());


typedef struct
{
    public:
    QString setting_MainSelectorColor;
    QString setting_DateBackgroundColor;
    QString setting_DateBackgroundHolydayColor;
    QString setting_DateParashaColor;
    QString setting_DateHolydayColor;
    QString setting_DateTextColor;
    QString setting_DateTextHolydayColor;
    int setting_sefirat_homer_nosach;
    bool setting_note_enable;

}DateWidgetSettings;
#endif // SETTINGSFUNC_H
