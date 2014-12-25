/* noteplugin.cpp
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
#include "noteplugin.h"
#include "mainwindow.h"
#include "notewindow.h"
int NotePlugIn::init(){
    QString name = QDir::homePath()+"/.config/avisoftware/"+APP_SHORT_NAME+"/notes.db";
    if(!checkDataBase(name)){
        connectToDataBase();
        createDataBase();

    }else{
        connectToDataBase();
    }
    return 0;
}
 void  NotePlugIn::createDataBase(){
     QSqlQuery query(db);
     query.exec("CREATE TABLE IF NOT EXISTS notes ("
                "id INT,text TEXT,year INT,month INT,day INT)");
     qDebug()<< query.lastError().text();

 }
void NotePlugIn::connectToDataBase(){
    db =  QSqlDatabase::addDatabase("QSQLITE","notes");
    db.setHostName("localhost");
    QString name = QDir::homePath()+"/.config/avisoftware/"+APP_SHORT_NAME+"/notes.db";
    db.setDatabaseName(name);

       if (!db.open())
       {
          return ;
       }
       qDebug() << db.lastError().text();

}
 void NotePlugIn::findNotesForMonth(int y, int m,int d){
     QSqlQueryModel query;
     query.setQuery("SELECT * FROM notes "
                    "WHERE year ='"+QString::number(y)+"' AND month ='"+QString::number(m)+"'AND day >= "+QString::number(d)+" ORDER BY day",db);
     qDebug()<< query.lastError().text();
     int i =query.rowCount();
     for (int j=0;i>j;j++){
         QString s= query.record(j).value(4).toString()+"-"+query.record(j).value(3).toString()+"-"+query.record(j).value(2).toString();

         list.append(QDate::fromString(s,"d-M-yyyy"),query.record(j).value(0).toInt());

     }
 }
 void NotePlugIn::findNotesForMonthSec(int y, int m, int d){
     QSqlQueryModel query;
     query.setQuery("SELECT * FROM notes "
                    "WHERE year ='"+QString::number(y)+"' AND month ='"+QString::number(m)+"'AND day >= "+QString::number(d)+" ORDER BY day",db);
     qDebug()<< query.lastError().text();
     int i =query.rowCount();
     for (int j=0;i>j;j++){
         QString s= query.record(j).value(4).toString()+"-"+query.record(j).value(3).toString()+"-"+query.record(j).value(2).toString();

         list.append(QDate::fromString(s,"d-M-yyyy"),query.record(j).value(0).toInt());

     }
 }
 int  NotePlugIn::getNoteId(QDate d){
     if(list.getFirstDate()==d){
         list.remove(0);
         QSqlQueryModel query;
         query.setQuery("SELECT id FROM notes "
                        "WHERE year ='"+QString::number(d.year())+"' AND month ='"+QString::number(d.month())+"' AND day ='"+QString::number(d.day())+"'",db);
         qDebug()<< query.lastError().text();
         if (query.record(0).value(0).isValid()){
              return (query.record(0).value(0).toInt());
         }
     }

     return 0;
 }
   void NotePlugIn::releseAllNoteFromList(){
        list.removeAll();
   }

int NotePlugIn::addNote(QDate d){

    QString  noteT;
    NoteWindow * nw = new NoteWindow(0,&noteT,0);
    nw->exec();
    if(noteT.isEmpty()){
        return 0;
    }
    noteT= QRegExp::escape(noteT);
    noteT.replace("'","''");
    QSqlQueryModel query;
    query.setQuery("SELECT MAX(id)  FROM notes",db);
    int i = query.record(0).value(0).toInt()+1;
 qDebug()<< query.lastError().text();
    QString q ="INSERT INTO notes (id,text,year,month,day)VALUES(%1,'%2',%3,%4,%5)";
    q=q.arg(i).arg(noteT).arg(d.year()).arg(d.month()).arg(d.day());
    query.setQuery(q,db);
    qDebug()<< query.lastError().text();
    return i;

}
 int NotePlugIn::editNote(int id){//return 0 if deleted!!
     QString  noteT;
     QString s ="";
     if (id >0){
           QSqlQueryModel query;
         query.setQuery("SELECT text FROM notes WHERE id="+QString::number(id),db);
          s = query.record(0).value(0).toString();
     }
     NoteWindow * nw = new NoteWindow(s,&noteT,0);
     nw->exec();
      QSqlQueryModel query;
     if(noteT.isEmpty()){
         query.setQuery("DELETE  FROM notes WHERE id= "+QString::number(id),db);
        return 0;
     }else{
         noteT= QRegExp::escape(noteT);
          noteT.replace("'","''");
         QString q ="UPDATE  notes SET text='"+noteT+"' WHERE id ="+QString::number(id);

         query.setQuery(q,db);
         qDebug()<< query.lastError().text();
         }
     return 1;
 }
int  NotePlugIn::deleteNote(int i){
    QSqlQueryModel query;
    query.setQuery("DELETE  FROM notes WHERE id= "+QString::number(i),db);
    qDebug()<< query.lastError().text();
    return 0;
}

void NotePlugIn::setForSelection(int id){



}






