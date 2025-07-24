#ifndef DBDIALOG_H
#define DBDIALOG_H

#include <QDialog>

namespace Ui {
class dbdialog;
}

class dbdialog : public QDialog
{
    Q_OBJECT

public:
    explicit dbdialog(QWidget *parent = nullptr);
    ~dbdialog();

public slots:
    void showDBConfig();
    void saveDBConfig();
    void hideDBConfig();
private:
    Ui::dbdialog *ui;
    QString filePath = "dbconfig.json";
};

#endif // DBDIALOG_H
