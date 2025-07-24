#ifndef DETAILDIALOG_H
#define DETAILDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class detaildialog;
}

class detaildialog : public QDialog
{
    Q_OBJECT

public:
    explicit detaildialog(QWidget *parent = nullptr);
    ~detaildialog();
public slots:
    void showDetailDialog();
    void saveDetailDialog();
private:
    Ui::detaildialog *ui;
    QString filePath = "detailconfig.json";
    QMap<QString,QLineEdit*> lineMap;
};

#endif // DETAILDIALOG_H
