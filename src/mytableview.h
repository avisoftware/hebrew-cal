#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H


#include <QTableView>
#include <QtSql>
QT_BEGIN_NAMESPACE
class QTableView;
class QSqlTableModel;
QT_END_NAMESPACE
class myTableView : public QTableView
{
    Q_OBJECT
public:
     myTableView(QWidget *parent , QSqlTableModel *pmodel);
     void contextMenuEvent ( QContextMenuEvent * e );
        int rowI;
private:
        QSqlTableModel * gpmodel;
     private slots:
        void deleteRow();
        void editRow();
signals:
    
public slots:
    
};

#endif // MYTABLEVIEW_H
