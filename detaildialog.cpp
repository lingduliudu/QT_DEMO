#include "detaildialog.h"
#include "ui_detaildialog.h"
#include <QJsonObject>
#include <QFile>
#include <QDir>
detaildialog::detaildialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::detaildialog)
{
    ui->setupUi(this);
    connect(ui->cancelButton,&QPushButton::clicked,this,&detaildialog::hide);
    connect(ui->saveButton,&QPushButton::clicked,this,&detaildialog::saveDetailDialog);
    // 添加映射
    lineMap["entityPackage"]=ui->entityPackage;
    lineMap["requestPackage"]=ui->requestPackage;
    lineMap["iservicePackage"]=ui->iservicePackage;
    lineMap["servicePackage"]=ui->servicePackage;
    lineMap["ifeignPackage"] = ui->ifeignPackage;
    lineMap["feignPackage"] = ui->feignPackage;
    lineMap["imapperPackage"] = ui->imapperPackage;
    lineMap["controllerPackage"] = ui->controllerPackage;
    lineMap["ext1Package"] = ui->ext1Package;
    lineMap["ext2Package"] = ui->ext2Package;
    lineMap["var1"] = ui->var1;
    lineMap["var2"] = ui->var2;
    lineMap["var3"] = ui->var3;
    lineMap["var4"] = ui->var4;
    lineMap["var5"] = ui->var5;
    lineMap["var6"] = ui->var6;

}

detaildialog::~detaildialog()
{
    delete ui;
}

void detaildialog::showDetailDialog(){
    // 读取配置文件
    QFile file(this->filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject obj = doc.object();
    // 写入
    for(auto key:lineMap.keys()){
        QLineEdit *v = lineMap[key];
        v->setText(obj.value(key).toString());
    }
    show();
}
void detaildialog::saveDetailDialog(){
    // 读取配置文件
    QJsonObject dbConfig;
    for(auto key:lineMap.keys()){
        QLineEdit *v = lineMap[key];
        dbConfig[key] = v->text();
    }
    QJsonDocument doc(dbConfig);
    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    }
    file.write(jsonData); // 写入数据
    file.close();
    hide();
}
