#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editpath.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initDataList();
    void initConnect();
    void createFile();
    QJsonArray readJSONArray(QString path);
    QJsonObject readJSON(QString path);
public slots:
    void onTableCellChanged(int row,int column);
private:
    Ui::MainWindow *ui;
    editpath * editPath;
    QString templateDir = "template/";
    QString filePath = "basedata.json";
    QString dbConfigPath = "dbconfig.json";
    QString detailConfigPath = "detailconfig.json";
    QString typeMapPath = "typemap.json";

};
#endif // MAINWINDOW_H
