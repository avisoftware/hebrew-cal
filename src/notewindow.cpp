/* notewindow.cpp
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
#include "notewindow.h"
#include "ui_notewindow.h"
#include <QtSql>
NoteWindow::NoteWindow(QString text ,QString * rNote,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoteWindow)
{
    ui->setupUi(this);
    p_note = rNote;    
    ui->note_board->setPlainText(text);
    connect(ui->close_button,SIGNAL(clicked()),this,SLOT(close()));
}

NoteWindow::~NoteWindow()
{
    delete ui;
}
void NoteWindow::closeEvent(QCloseEvent *event)
 {
        *p_note = ui->note_board->toPlainText();
         event->accept();

 }
