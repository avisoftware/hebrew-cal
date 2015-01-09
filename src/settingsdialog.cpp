/* settingdialog.cpp
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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "locationsview.h"
#include "settingsfunc.h"
#include "functions.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(toClose()));
    connect(ui->resetPushButton, SIGNAL(clicked()), this, SLOT(resetall()));
    connect(ui->listWidget,
               SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
               this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
    addPages( );
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
void SettingsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui->stackedWidget->setCurrentIndex(ui->listWidget->row(current));
}

 void SettingsDialog::addPages(){
     calenderPage = new CalendarPage;
     timesPage=new TimesPage;
     omerPage=new OmerPage;
     viewPage=new ViewPage;
     viewTimesPage= new ViewTimesPage;
     notePage = new NotePage;

     ui->stackedWidget->addWidget(calenderPage );
     ui->stackedWidget->addWidget(timesPage);
     ui->stackedWidget->addWidget(omerPage);
     ui->stackedWidget->addWidget(viewPage);
     ui->stackedWidget->addWidget(viewTimesPage);
     ui->stackedWidget->addWidget(notePage);


 }
 void  SettingsDialog::resetall(){
    reset_settings();
    calenderPage->reset();
    timesPage->reset();
     omerPage->reset();
    viewPage->reset();
    viewTimesPage->reset();
    notePage->reset();

 }
   void SettingsDialog::toClose(){
       if (calenderPage->isChange()||timesPage->isChange()||omerPage->isChange()||viewPage->isChange()
               ||viewTimesPage->isChange()||notePage->isChange()){
           done(1);
       }else{
           done(0);
        }
   }
   void SettingsDialog::closeEvent(QCloseEvent *event)
    {
       toClose();
            event->accept();
    }
CalendarPage::CalendarPage(QWidget *parent)
    : QWidget(parent)
{
    change=0;
    QLabel *calbyLabel = new QLabel(QString::fromLocal8Bit("הצג לוח שנה לפי תאריך"));

   calbyCombo = new QComboBox;
    calbyCombo->addItem(QString::fromLocal8Bit("עברי"));
    calbyCombo->addItem(QString::fromLocal8Bit("לועזי"));


    QHBoxLayout *calbyLayout = new QHBoxLayout;
    calbyLayout->addWidget(calbyLabel);
    calbyLayout->addWidget(calbyCombo);

    diasporaCheckBox = new QCheckBox(QString::fromLocal8Bit("חו\"ל (יום טוב שני)"));
    israelHolidayCheckBox = new QCheckBox(QString::fromLocal8Bit("הצגת חגי וימי מדינת ישראל"));
    dafHyomiCheckBox= new QCheckBox(QString::fromLocal8Bit("דף היומי"));

    QVBoxLayout *calenderLayout = new QVBoxLayout;
    calenderLayout->addLayout(calbyLayout);
    calenderLayout->addWidget(diasporaCheckBox);
     calenderLayout->addWidget(israelHolidayCheckBox);
      calenderLayout->addWidget(dafHyomiCheckBox);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(calenderLayout);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    connect(calbyCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(calby_change(int)));
    connect(diasporaCheckBox, SIGNAL(toggled(bool)), this, SLOT(diaspora_change(bool)));
    connect(israelHolidayCheckBox, SIGNAL(toggled(bool)), this, SLOT(israelHoliday_change(bool)));
    connect(dafHyomiCheckBox, SIGNAL(toggled(bool)), this, SLOT(dafHyomiCheckBox_change(bool)));

    calbyCombo->setCurrentIndex(read_setting("cal","calby").toInt());
    diasporaCheckBox->setChecked(read_setting("cal","diaspora").toBool());
    israelHolidayCheckBox->setChecked(read_setting("cal","IsraelHoliday").toBool());
    dafHyomiCheckBox->setChecked(read_setting("cal","dafHyomi").toBool());

}
void CalendarPage::calby_change(int i){
    write_setting("cal","calby",i);
    change =1;
}
void CalendarPage::reset(){
    calbyCombo->setCurrentIndex(0);
    diasporaCheckBox->setChecked(false);
    israelHolidayCheckBox->setChecked(true);
    change=1;
}
 void CalendarPage::diaspora_change(bool b){
      write_setting("cal","diaspora",b);
      change =1;
 }
void CalendarPage::israelHoliday_change(bool b){
    write_setting("cal","IsraelHoliday",b);
    change =1;
}
  void CalendarPage::dafHyomiCheckBox_change(bool b){
      write_setting("cal","dafHyomi",b);
      change =1;
  }

TimesPage::TimesPage(QWidget *parent)
    : QWidget(parent){

    change=0;
    if(!init()){

        QLabel * labelLocation = new QLabel (QString::fromLocal8Bit("מיקום"));
        cityComboBox = new QComboBox();
        setModelLocation();
       QHBoxLayout *LocationLayout = new QHBoxLayout;
       LocationLayout->addWidget(labelLocation);
        LocationLayout->addWidget(cityComboBox);
        QPushButton * editLocationButton = new QPushButton(QString::fromLocal8Bit("ערוך מיקומים") );
        editLocationButton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
        LocationLayout->addWidget(editLocationButton);
        LocationLayout->addStretch(1);

         considerHeightCheckBox = new QCheckBox(QString::fromLocal8Bit("התחשבות בגובה מיקום - משנה את זמני השקיעה/זריחה (בפיתוח)"));
         considerHeightCheckBox->setEnabled(false);
         connect(considerHeightCheckBox, SIGNAL(toggled(bool)), this, SLOT(considerHeight_change(bool)));
        considerHeightCheckBox->setChecked( read_setting("times","location_consider_height").toBool());
        QGroupBox * summerTimeBox = new QGroupBox(QString::fromLocal8Bit("שעון קיץ"),this);
        autoRadioButton = new QRadioButton(QString::fromLocal8Bit("אוטומטי"),summerTimeBox);
       winterRadioButton = new QRadioButton(QString::fromLocal8Bit("שעון חורף"),summerTimeBox);
        summerRadioButton = new QRadioButton(QString::fromLocal8Bit("שעון קיץ"),summerTimeBox);

        QHBoxLayout *summerTimeLayout = new QHBoxLayout;
        summerTimeLayout->addWidget(autoRadioButton);
        summerTimeLayout->addWidget(winterRadioButton);
        summerTimeLayout->addWidget(summerRadioButton);
        summerTimeBox->setLayout(summerTimeLayout);
        setSummerTime();
        connect(autoRadioButton, SIGNAL(pressed()), this, SLOT(autoPressed()));
        connect(winterRadioButton, SIGNAL(pressed()), this, SLOT(winterPressed()));
        connect(summerRadioButton, SIGNAL(pressed()), this, SLOT(summerPressed()));

        QLabel * formatTimeLable = new QLabel (QString::fromLocal8Bit("פורמט הזמן (אין לסמוך על השניות במדויק)"));
        formatTimeComboBox= new QComboBox();
        QTime c = QTime::currentTime();
        //formatTimeComboBox->addItem(c.toString("h:mm:ss")+" (h:mm:ss)");
        //formatTimeComboBox->addItem(c.toString("hh:mm:ss")+" (hh:mm:ss)");
        formatTimeComboBox->addItem(c.toString("h:mm")+" (h:mm)");
        formatTimeComboBox->addItem(c.toString("hh:mm")+" (hh:mm)");
        formatTimeComboBox->addItem(c.toString("h:mm ap")+" (h:mm ap)");
        formatTimeComboBox->addItem(c.toString("hh:mm ap")+" (hh:mm ap)");
        QString f = read_setting("times","FormatTime").toString();

        /*if (f=="h:mm:ss") {
            formatTimeComboBox->setCurrentIndex(0);
           }
        else if(f=="hh:mm:ss") {
            formatTimeComboBox->setCurrentIndex(1);
        }*/
        if(f== "h:mm"){
            formatTimeComboBox->setCurrentIndex(0);
        }
        else if(f=="h:mm ap"){
            formatTimeComboBox->setCurrentIndex(2);
        }
         else if(f=="hh:mm ap"){
            formatTimeComboBox->setCurrentIndex(3);
        }else{
              formatTimeComboBox->setCurrentIndex(1);
        }
        QLabel * formatTime2Lable = new QLabel (QString::fromLocal8Bit("בהצגת הזמן בלי שניות הדקות מתעגלות כלפי השלם הקרוב"));

        QHBoxLayout *formatTimeLayout = new QHBoxLayout;
        formatTimeLayout->addWidget(formatTimeLable);
        formatTimeLayout->addWidget(formatTimeComboBox);

        QVBoxLayout *vbox = new QVBoxLayout;
            vbox->addLayout(LocationLayout);
            vbox->addWidget(considerHeightCheckBox);
            vbox->addWidget(summerTimeBox);
            vbox->addLayout(formatTimeLayout);
            vbox->addWidget(formatTime2Lable);
            vbox->addStretch(1);
            setLayout(vbox);
            connect(editLocationButton, SIGNAL(clicked()), this, SLOT(editLocation()));
             connect(cityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeLocation(QString)));
             connect(formatTimeComboBox, SIGNAL(activated(QString)),
                         this, SLOT(changeFormatTimeString(QString)));
    }

}
 int  TimesPage::init (){
     int firstrun= first_run();
     db = QSqlDatabase::addDatabase("QSQLITE");
     db.setHostName("localhost");
     QString name = QDir::homePath()+"/.config/avisoftware/"+APP_SHORT_NAME+"/location.db";
     db.setDatabaseName(name);
        if (!db.open())
        {
           return 1;
        }
        if(firstrun){
            createTable();
        }

        qDebug() << db.lastError().text();
        return 0;
 }

void TimesPage::createTable (){
     QSqlQuery query;
     query.exec("CREATE TABLE IF NOT EXISTS location ("
                "NameHeb TEXT,NameEng TEXT,Region TEXT, Longitude TEXT, Latitude TEXT,TimeZone TEXT,PlaceHeight TEXT,Candle_Light TEXT)");
     qDebug()<< query.lastError().text();
     }

  int TimesPage::first_run(){
      if (QFile::exists("location.db")){
          return 0;
      }
      return 1;
  }
 void TimesPage::editLocation(){
     locationsView * lv = new locationsView(this);
     lv->exec();
     setModelLocation();
 }
 void TimesPage::setModelLocation(){
     QSqlQueryModel * model = new QSqlQueryModel();
        model->setQuery("SELECT  mainName FROM location");
        qDebug()<< model->lastError().text();
        cityComboBox->setModel(model);
        cityComboBox->setCurrentIndex(cityComboBox->findText(read_setting("times","location_heb_name").toString()));
 }
   void TimesPage::considerHeight_change(bool c){
       write_setting("times","location_consider_height",c);
       change =1;
   }

 void TimesPage::changeLocation(QString loc){
     QSqlQuery q ;
     q.exec("SELECT * FROM location WHERE mainName='"+loc+"'");
     q.first();
     write_setting("times","location_heb_name",loc);
     write_setting("times","location_eng_name",q.value(1).toString());
     write_setting("times","location_region",q.value(2).toString());
     write_setting("times","location_longitude",q.value(3).toString());
     write_setting("times","location_latitude",q.value(4).toString());
     write_setting("times","location_time_zone",q.value(5).toString());
      write_setting("times","location_height",q.value(6).toString());     
     write_setting("times","location_candle_light",q.value(7).toString());
     change =1;
 }
  void TimesPage::setSummerTime(){
      switch (read_setting("times","SummerTime").toInt()){
          case -1:
          {
            autoRadioButton->setChecked(true);
             break;
          }
          case 0:
          {
            winterRadioButton->setChecked(true);
              break;
          }
          case 1:
          {
            summerRadioButton->setChecked(true);
              break;
          }
      }
  }

 void TimesPage::winterPressed(){
     write_setting("times","SummerTime",0);
     change =1;
 }

 void TimesPage::summerPressed(){
     write_setting("times","SummerTime",1);
     change =1;
 }

 void TimesPage::autoPressed(){
      write_setting("times","SummerTime",-1);
      change =1;
 }
  void TimesPage::changeFormatTimeString(const QString &formatString){
      int f = formatString.indexOf("(")+1;
      QString s =formatString.mid(f,formatString.count()-f-1);
      write_setting("times","FormatTime",s);
    change =1;
  }

 void TimesPage::reset(){
     cityComboBox->setCurrentIndex(0);
     autoRadioButton->setChecked(true);
     formatTimeComboBox->setCurrentIndex(2);
     change=1;
 }
 OmerPage::OmerPage(QWidget *parent)
     : QWidget(parent)
 {
     change=0;
     QLabel * typeLabel = new QLabel (QString::fromLocal8Bit("נוסח"));
        typeLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Ignored);
        typeComboBox = new QComboBox();
        typeComboBox->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
        typeComboBox->addItem(QString::fromLocal8Bit("אשכנז"));
        typeComboBox->addItem(QString::fromLocal8Bit("ספרד"));
        typeComboBox->addItem(QString::fromLocal8Bit("עדות המזרח"));


        switch (read_setting("sefirat_homer","nosach").toInt()){
            case 0:
            {
              typeComboBox->setCurrentIndex ( 0 );
               break;
            }
            case 1:
            {
             typeComboBox->setCurrentIndex ( 1 );
                break;
            }
            case 2:
            {
              typeComboBox->setCurrentIndex ( 2 );
                break;
            }
        default:
            typeComboBox->setCurrentIndex ( 0 );
        }

        typeLabel->setBuddy(typeComboBox);

        QHBoxLayout *nosachLayout = new QHBoxLayout;
        nosachLayout->addWidget(typeLabel);
        nosachLayout->addWidget(typeComboBox);
        nosachLayout->addStretch(1);
        QVBoxLayout *vbox = new QVBoxLayout;
            vbox->addLayout(nosachLayout);
            vbox->addStretch(1);
            setLayout(vbox);
        connect(typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeNosach(int)));
 }
 void OmerPage::changeNosach(int n){
     switch (n){
     case 0:
         {
            write_setting("sefirat_homer","nosach",0);
            break;
         }
         case 1:
         {
            write_setting("sefirat_homer","nosach",1);
             break;
         }
         case 2:
         {
             write_setting("sefirat_homer","nosach",2);
             break;
         }
     }
     change =1;
 }
 void OmerPage::reset(){
     typeComboBox->setCurrentIndex(0);
     change=1;
 }
 ViewPage::ViewPage(QWidget *parent)
     : QWidget(parent)
 {
    change=0;
    QFormLayout * form= new QFormLayout;

    MainBackgroundColorButton = new QPushButton();
    MainTextColorButton = new QPushButton();
    MainSelectorColorButton = new QPushButton();
    MainGridColorButton = new QPushButton();
    DateBackgroundColorButton = new QPushButton();
    DateBackgroundHolydayColorButton = new QPushButton();
    DateTextColorButton = new QPushButton();
    DateTextHolydayColorButton = new QPushButton();
    DateParashaColorButton = new QPushButton();
    DateHolydayColorButton = new QPushButton();
    TableHeaderBackgroundColor= new QPushButton();
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע רקע ראשי")),MainBackgroundColorButton);
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע טקסט ראשי")),MainTextColorButton);
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע רקע כותרת ימים ")),TableHeaderBackgroundColor);
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע רשת")),MainGridColorButton);

    form->addRow(new QLabel(QString::fromLocal8Bit("צבע רקע תאריך נבחר")),MainSelectorColorButton);
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע רקע תאריך")),DateBackgroundColorButton);
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע רקע שבת וחג")),DateBackgroundHolydayColorButton);
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע טקסט תאריך")),DateTextColorButton);
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע טקסט תאריך בשבת וחג")),DateTextHolydayColorButton);
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע טקסט פרשת שבוע")),DateParashaColorButton);
    form->addRow(new QLabel(QString::fromLocal8Bit("צבע טקסט חג")),DateHolydayColorButton);
    form->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);


       setLayout(form);
         connect(MainBackgroundColorButton, SIGNAL(clicked()), this, SLOT(MainBackgroundColorChange()));
         connect(MainTextColorButton, SIGNAL(clicked()), this, SLOT(MainTextColorChange()));
         connect(TableHeaderBackgroundColor, SIGNAL(clicked()), this, SLOT(TableHeaderBackgroundColorChange()));
         connect(MainGridColorButton, SIGNAL(clicked()), this, SLOT(MainGridColorChange()));
         connect(MainSelectorColorButton, SIGNAL(clicked()), this, SLOT(MainSelectorColorChange()));
         connect(DateBackgroundColorButton, SIGNAL(clicked()), this, SLOT(DateBackgroundColorChange()));
         connect(DateBackgroundHolydayColorButton, SIGNAL(clicked()), this, SLOT(DateBackgroundHolydayColorChange()));
         connect(DateParashaColorButton, SIGNAL(clicked()), this, SLOT(DateParashaColorChange()));
         connect(DateHolydayColorButton, SIGNAL(clicked()), this, SLOT(DateHolydayColorChange()));
         connect(DateTextColorButton, SIGNAL(clicked()), this, SLOT(DateTextColorChange()));
         connect(DateTextHolydayColorButton, SIGNAL(clicked()), this, SLOT(DateTextHolydayColorChange()));
    setColors();

 }
  void ViewPage::setColors(){
      MainBackgroundColorButton->setStyleSheet("background-color:'"+read_setting("view","MainBackgroundColor").toString()+"';");
      MainTextColorButton->setStyleSheet("background-color:'"+read_setting("view","MainTextColor").toString()+"';");
      TableHeaderBackgroundColor->setStyleSheet("background-color:'"+read_setting("view","TableHeaderBackgroundColor").toString()+"';");
      MainGridColorButton->setStyleSheet("background-color: '"+read_setting("view","MainGridColor").toString()+"';");
      MainSelectorColorButton->setStyleSheet("background-color:'"+read_setting("view","MainSelectorColor").toString()+"';");
      DateBackgroundColorButton->setStyleSheet("background-color:'"+read_setting("view","DateBackgroundColor").toString()+"';");
      DateBackgroundHolydayColorButton->setStyleSheet("background-color:'"+read_setting("view","DateBackgroundHolydayColor").toString()+"';");
      DateParashaColorButton->setStyleSheet("background-color:'"+read_setting("view","DateParashaColor").toString()+"';");
      DateHolydayColorButton->setStyleSheet("background-color:'"+read_setting("view","DateHolydayColor").toString()+"';");
      DateTextColorButton->setStyleSheet("background-color:'"+read_setting("view","DateTextColor").toString()+"';");
      DateTextHolydayColorButton->setStyleSheet("background-color:'"+read_setting("view","DateTextHolydayColor").toString()+"';");



  }

 void ViewPage::MainBackgroundColorChange(){
     QColor color;
           color = QColorDialog::getColor(QColor(read_setting("view","MainBackgroundColor").toString()));
       if (color.isValid()) {
           MainBackgroundColorButton->setStyleSheet("background-color:'"+color.name()+"';");
           write_setting("view","MainBackgroundColor",color.name());
           change =1;
       }
 }

 void ViewPage::MainTextColorChange(){
     QColor color;
           color = QColorDialog::getColor(QColor(read_setting("view","MainTextColor").toString()));
       if (color.isValid()) {
           MainTextColorButton->setStyleSheet("background-color:'"+color.name()+"';");
           write_setting("view","MainTextColor",color.name());
           change =1;
       }
 }

void ViewPage::MainGridColorChange(){
    QColor color;
          color = QColorDialog::getColor(QColor(read_setting("view","MainGridColor").toString()));
      if (color.isValid()) {
          MainGridColorButton->setStyleSheet("background-color:'"+color.name()+"';");
          write_setting("view","MainGridColor",color.name());
          change =1;
      }
}

void ViewPage::MainSelectorColorChange(){
    QColor color;
          color = QColorDialog::getColor(QColor(read_setting("view","MainSelectorColor").toString()));
      if (color.isValid()) {
          MainSelectorColorButton->setStyleSheet("background-color:'"+color.name()+"';");
          write_setting("view","MainSelectorColor",color.name());
          change =1;
      }
}

void ViewPage::DateBackgroundColorChange(){
    QColor color;
          color = QColorDialog::getColor(QColor(read_setting("view","DateBackgroundColor").toString()));
      if (color.isValid()) {
          DateBackgroundColorButton->setStyleSheet("background-color:'"+color.name()+"';");
          write_setting("view","DateBackgroundColor",color.name());
          change =1;
      }
}

void ViewPage::DateBackgroundHolydayColorChange(){
    QColor color;
          color = QColorDialog::getColor(QColor(read_setting("view","DateBackgroundHolydayColor").toString()));
      if (color.isValid()) {
          DateBackgroundHolydayColorButton->setStyleSheet("background-color:'"+color.name()+"';");
          write_setting("view","DateBackgroundHolydayColor",color.name());
          change =1;
      }
}

void ViewPage::DateParashaColorChange(){
    QColor color;
          color = QColorDialog::getColor(QColor(read_setting("view","DateParashaColor").toString()));
      if (color.isValid()) {
          DateParashaColorButton->setStyleSheet("background-color:'"+color.name()+"';");
          write_setting("view","DateParashaColor",color.name());
          change =1;
      }
}

void ViewPage::DateHolydayColorChange(){
    QColor color;
          color = QColorDialog::getColor(QColor(read_setting("view","DateHolydayColor").toString()));
      if (color.isValid()) {
          DateHolydayColorButton->setStyleSheet("background-color:'"+color.name()+"';");
          write_setting("view","DateHolydayColor",color.name());
          change =1;
      }
}

void ViewPage::DateTextColorChange(){
    QColor color;
          color = QColorDialog::getColor(QColor(read_setting("view","DateTextColor").toString()));
      if (color.isValid()) {
          DateTextColorButton->setStyleSheet("background-color:'"+color.name()+"';");
          write_setting("view","DateTextColor",color.name());
          change =1;
      }
}

void ViewPage::DateTextHolydayColorChange(){
    QColor color;
          color = QColorDialog::getColor(QColor(read_setting("view","DateTextHolydayColor").toString()));
      if (color.isValid()) {
          DateTextHolydayColorButton->setStyleSheet("background-color:'"+color.name()+"';");
          write_setting("view","DateTextHolydayColor",color.name());

          change =1;
      }
}
 void  ViewPage::TableHeaderBackgroundColorChange(){
     QColor color;
           color = QColorDialog::getColor(QColor(read_setting("view","TableHeaderBackgroundColor").toString()));
       if (color.isValid()) {
           TableHeaderBackgroundColor->setStyleSheet("background-color:'"+color.name()+"';");
           write_setting("view","TableHeaderBackgroundColor",color.name());

           change =1;
       }
 }

void ViewPage::reset(){
    setColors();
    change=1;
}
ViewTimesPage::ViewTimesPage(QWidget *parent)
    : QWidget(parent)
{
     scroll= new QScrollArea();
    scroll->setBackgroundRole(QPalette::Light);
     alot_mgaCheckBox = new QCheckBox(QString::fromLocal8Bit("עלות השחר (מג\"א)"));
     alotCheckBox = new QCheckBox(QString::fromLocal8Bit("עלות השחר"));
     talitCheckBox = new QCheckBox(QString::fromLocal8Bit("זמן ציצית ותפילין"));
     zrichaCheckBox = new QCheckBox(QString::fromLocal8Bit("הנץ החמה"));
     end_shema_mgaCheckBox =  new QCheckBox(QString::fromLocal8Bit("סוף זמן קריאת שמע (מג\"א)"));
     end_shemaCheckBox = new QCheckBox(QString::fromLocal8Bit("סוף זמן קריאת שמע (הגר\"א)"));
     end_tefila_mgaCheckBox = new QCheckBox(QString::fromLocal8Bit("סוף זמן תפילה (מג\"א)"));
     end_tefilaCheckBox = new QCheckBox(QString::fromLocal8Bit("סוף זמן תפילה"));
     middayCheckBox = new QCheckBox(QString::fromLocal8Bit("חצות"));
     mincha_gdolaCheckBox= new QCheckBox(QString::fromLocal8Bit("מנחה גדולה"));
     mincha_ktanaCheckBox= new QCheckBox(QString::fromLocal8Bit("מנחה קטנה"));
     plug_haminchaCheckBox= new QCheckBox(QString::fromLocal8Bit("פלג המנחה"));
     shkihaCheckBox= new QCheckBox(QString::fromLocal8Bit("שקיעה"));
     shabatCheckBox = new QCheckBox(QString::fromLocal8Bit("זמני שבת/ חג / צומות"));
    shabatOutRTCheckBox = new QCheckBox(QString::fromLocal8Bit("צאת השבת ר\"ת"));

    QVBoxLayout *list = new QVBoxLayout;
     list->addWidget(alot_mgaCheckBox);
    list->addWidget(alotCheckBox);
    list->addWidget(talitCheckBox);
    list->addWidget(zrichaCheckBox);
    list->addWidget(end_shema_mgaCheckBox);
    list->addWidget(end_shemaCheckBox);
    list->addWidget(end_tefila_mgaCheckBox);
    list->addWidget(end_tefilaCheckBox);
    list->addWidget(middayCheckBox);
    list->addWidget(mincha_gdolaCheckBox);
    list->addWidget(mincha_ktanaCheckBox);
    list->addWidget(plug_haminchaCheckBox);
    list->addWidget(shkihaCheckBox);
     list->addWidget(shabatCheckBox);
     list->addWidget(shabatOutRTCheckBox);
    list->addStretch(1);

    scroll->setLayout(list);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(scroll);
    setLayout(mainLayout);

    setAll();

    connect(alot_mgaCheckBox, SIGNAL(toggled(bool)), this, SLOT(alot_mga_change(bool)));
    connect(alotCheckBox, SIGNAL(toggled(bool)), this, SLOT(alot_change(bool)));
    connect(talitCheckBox, SIGNAL(toggled(bool)), this, SLOT(talit_change(bool)));
    connect(zrichaCheckBox, SIGNAL(toggled(bool)), this, SLOT(zricha_change(bool)));
    connect(end_shema_mgaCheckBox, SIGNAL(toggled(bool)), this, SLOT(end_shema_mga_change(bool)));
    connect(end_shemaCheckBox, SIGNAL(toggled(bool)), this, SLOT(end_shema_change(bool)));
    connect(end_tefila_mgaCheckBox, SIGNAL(toggled(bool)), this, SLOT(end_tefila_mga_change(bool)));
    connect(end_tefilaCheckBox, SIGNAL(toggled(bool)), this, SLOT(end_tefila_change(bool)));
    connect(middayCheckBox, SIGNAL(toggled(bool)), this, SLOT(midday_change(bool)));
    connect(mincha_gdolaCheckBox, SIGNAL(toggled(bool)), this, SLOT(mincha_gdola_change(bool)));
    connect(mincha_ktanaCheckBox, SIGNAL(toggled(bool)), this, SLOT(mincha_ktana_change(bool)));
    connect(plug_haminchaCheckBox, SIGNAL(toggled(bool)), this, SLOT(plug_hamincha_change(bool)));
    connect(shkihaCheckBox, SIGNAL(toggled(bool)), this, SLOT(shkiha_change(bool)));
    connect(shabatCheckBox, SIGNAL(toggled(bool)), this, SLOT(shabat_change(bool)));
    connect(shabatOutRTCheckBox, SIGNAL(toggled(bool)), this, SLOT(shabat_out_rt_change(bool)));

}
 void ViewTimesPage::reset(){
     alot_mgaCheckBox->setChecked(true);
     alotCheckBox->setChecked(true);
     talitCheckBox->setChecked(true);
     zrichaCheckBox->setChecked(true);
     end_shemaCheckBox->setChecked(true);
     end_shema_mgaCheckBox->setChecked(true);
     end_tefila_mgaCheckBox->setChecked(true);
     end_tefilaCheckBox->setChecked(true);
     middayCheckBox->setChecked(true);
     mincha_gdolaCheckBox->setChecked(true);
     mincha_ktanaCheckBox->setChecked(true);
     plug_haminchaCheckBox->setChecked(true);
     shkihaCheckBox->setChecked(true);
     shabatCheckBox->setChecked(true);
    shabatOutRTCheckBox->setChecked(true);
      change=1;
 }
  void ViewTimesPage::setAll(){
      alot_mgaCheckBox->setChecked(read_setting("view_times","alot_mga").toBool());
      alotCheckBox->setChecked(read_setting("view_times","alot").toBool());
      talitCheckBox->setChecked(read_setting("view_times","talit").toBool());
      zrichaCheckBox->setChecked(read_setting("view_times","zricha").toBool());
      end_shema_mgaCheckBox->setChecked(read_setting("view_times","end_shema_mga").toBool());
      end_shemaCheckBox->setChecked(read_setting("view_times","end_shema").toBool());
      end_tefila_mgaCheckBox->setChecked(read_setting("view_times","end_tefila_mga").toBool());
      end_tefilaCheckBox->setChecked(read_setting("view_times","end_tefila").toBool());
      middayCheckBox->setChecked(read_setting("view_times","midday").toBool());
      mincha_gdolaCheckBox->setChecked(read_setting("view_times","mincha_gdola").toBool());
      mincha_ktanaCheckBox->setChecked(read_setting("view_times","mincha_ktana").toBool());
      plug_haminchaCheckBox->setChecked(read_setting("view_times","plug_hamincha").toBool());
      shkihaCheckBox->setChecked(read_setting("view_times","shkiha").toBool());
      shabatCheckBox->setChecked(read_setting("view_times","shabat").toBool());
      shabatOutRTCheckBox->setChecked(read_setting("view_times","shabatOutRT").toBool());

  }


  void ViewTimesPage::alot_mga_change(bool b) {
      write_setting("view_times","alot_mga",b);
      change =1;
  }
   void ViewTimesPage::alot_change(bool b) {
       write_setting("view_times","alot",b);
       change =1;
   }
   void ViewTimesPage::talit_change(bool b) {
        write_setting("view_times","talit",b);
       change =1;
   }
   void ViewTimesPage::zricha_change(bool b) {
       write_setting("view_times","zricha",b);

       change =1;
   }
   void ViewTimesPage::end_shema_mga_change(bool b){
       write_setting("view_times","end_shema_mga",b);
       change =1;
   }
   void ViewTimesPage::end_shema_change(bool b) {
       write_setting("view_times","end_shema",b);

       change =1;
   }
   void ViewTimesPage::end_tefila_mga_change(bool b){
       write_setting("view_times","end_tefila_mga",b);
       change =1;
   }

   void ViewTimesPage::end_tefila_change(bool b) {
       write_setting("view_times","end_tefila",b);
       change =1;
   }
   void ViewTimesPage::midday_change(bool b) {
       write_setting("view_times","midday",b);
       change =1;
   }
   void ViewTimesPage::mincha_gdola_change(bool b) {
       write_setting("view_times","mincha_gdola",b);
       change =1;
   }
   void ViewTimesPage::mincha_ktana_change(bool b) {
       write_setting("view_times","mincha_ktana",b);
       change =1;
   }
   void ViewTimesPage::plug_hamincha_change(bool b) {
       write_setting("view_times","plug_hamincha",b);
       change =1;
   }
   void ViewTimesPage::shkiha_change(bool b) {
       write_setting("view_times","shkiha",b);
       change =1;
   }
   void  ViewTimesPage::shabat_change(bool b){
       write_setting("view_times","shabat",b);
       change =1;
   }
   void ViewTimesPage::shabat_out_rt_change(bool b){
       write_setting("view_times","shabatOutRT",b);
       change =1;
   }

   NotePage::NotePage(QWidget *parent)
       : QWidget(parent)
   {
       change=0;

        //enableCheckBox = new QCheckBox(QString::fromLocal8Bit("אפשר הערות"));
       MainGroupBox = new QGroupBox(QString::fromLocal8Bit("אפשר הערות"),this);
       MainGroupBox->setCheckable(true);

       ColorButton = new QPushButton();
       clearAllButton = new QPushButton(QString::fromLocal8Bit("מחק את כל ההערות"));

       QFormLayout * form= new QFormLayout;
       form->addRow(new QLabel(QString::fromLocal8Bit("צבע הערות")),ColorButton);
       form->addRow(clearAllButton,new QWidget());
       form->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);

        QVBoxLayout *gLayout = new QVBoxLayout;
            gLayout->addLayout(form);

        MainGroupBox->setLayout(gLayout);


       QVBoxLayout *mainLayout = new QVBoxLayout;
       mainLayout->addWidget(MainGroupBox);
       mainLayout->addStretch(1);
       setLayout(mainLayout);

        MainGroupBox->setChecked(read_setting("notes","enable").toBool());

         QSqlQueryModel query;
        query.setQuery("SELECT COUNT (id)FROM notes",QSqlDatabase::database("notes"));
        qDebug()<< query.lastError().text();
        QString s= query.record(0).value(0).toString();
        clearAllButton->setText(clearAllButton->text()+" -("+s+QString::fromLocal8Bit(") הערות"));
        connect(MainGroupBox, SIGNAL(toggled(bool)), this, SLOT(enable_change(bool)));
        connect(ColorButton, SIGNAL(clicked()), this, SLOT(ColorChange()));
        connect(clearAllButton, SIGNAL(clicked()), this, SLOT(clearAll()));
        setColor();

   }
   void NotePage::enable_change(bool b){
       write_setting("notes","enable",b);
       change =1;
   }
   void NotePage::reset(){
       MainGroupBox->setChecked(true);
        setColor();

       change=1;
   }
   void NotePage::ColorChange(){
        QColor color;
              color = QColorDialog::getColor(QColor(read_setting("notes","Color").toString()));
          if (color.isValid()) {
              ColorButton->setStyleSheet("background-color:'"+color.name()+"';");
              write_setting("notes","Color",color.name());
              change =1;
          }
    }
   void NotePage::clearAll(){
       int ret = QMessageBox::warning(this, QString::fromLocal8Bit("אזהרה"),
                                      QString::fromLocal8Bit("האם אתה בטוח שברצונך\n"
                                         "למחוק את כל ההערות?"),QMessageBox::Yes|QMessageBox::No);
       if (ret==QMessageBox::Yes){
           QSqlQuery query(QSqlDatabase::database("notes"));
           query.exec("DELETE FROM notes");
           qDebug()<< query.lastError().text();

          change =1;
       }

   }
   void NotePage::setColor(){
     ColorButton->setStyleSheet("background-color:'"+read_setting("notes","Color").toString()+"';");

 }

