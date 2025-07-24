#ifndef EDITPATH_H
#define EDITPATH_H
#include <QDialog>


namespace Ui {
class editpath;
}

class editpath : public QDialog
{
    Q_OBJECT

public:
    explicit editpath(QWidget *parent = nullptr);
    ~editpath();
signals:
    void updatePath();
public slots:
    void onTableDoubleClicked(int row, int column);
    void cancelButton();
    void saveButton();
private:
    Ui::editpath *ui;
    QString filePath = "F:/basedata.json";
    int currentEditRowIndex=0;
};

#endif // EDITPATH_H
