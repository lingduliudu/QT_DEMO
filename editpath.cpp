#include "editpath.h"
#include "ui_editpath.h"
#include "mainwindow.h"
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

editpath::editpath(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editpath)
{
    ui->setupUi(this);
    connect(ui->cancelButton,&QPushButton::clicked,this,&editpath::cancelButton);
    connect(ui->saveButton,&QPushButton::clicked,this,&editpath::saveButton);
}

editpath::~editpath()
{
    delete ui;
}

void editpath::onTableDoubleClicked(int row, int column){
    currentEditRowIndex = row;
    // 从数据中解析行数并赋值
    QFile file(this->filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = doc.array();
    QJsonValue value = jsonArray.at(row);
    QJsonObject obj = value.toObject();
    QString currentPos = QString(obj.value("path").toString());
    ui->lineEdit->setText(currentPos);
    this->show();
}
void editpath::cancelButton(){
    this->hide();
}
void editpath::saveButton(){
    // 从数据中解析行数并赋值
    QFile file(this->filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = doc.array();

    QJsonValue value = jsonArray.at(currentEditRowIndex);
    QJsonObject obj = value.toObject();
    obj["path"] = ui->lineEdit->text();
    jsonArray.replace(currentEditRowIndex, obj);
    QJsonDocument doc2(jsonArray);
    QByteArray updatedJsonData = doc2.toJson(QJsonDocument::Indented);
    QFile file2(filePath);
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
    }
    file2.write(updatedJsonData); // 写入数据
    file2.close();
    // 更新列表
    emit updatePath();
    this->hide();
}
