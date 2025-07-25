#include "dbdialog.h"
#include "ui_dbdialog.h"
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QIntValidator>
dbdialog::dbdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dbdialog)
{
    ui->setupUi(this);
    setFixedSize(this->size());
    setModal(true);
    connect(ui->saveButton,&QPushButton::clicked,this,&dbdialog::saveDBConfig);
    connect(ui->cancelButton,&QPushButton::clicked,this,&dbdialog::hide);
}

dbdialog::~dbdialog()
{
    delete ui;
}

void dbdialog::showDBConfig(){
    // 读取配置文件
    QFile file(this->filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "错误：无法打开文件" << filePath;
        qDebug() << "具体错误：" << file.errorString();
    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject obj = doc.object();
    // 写入
    QString host("host");
    ui->host->setText(obj.value(host).toString());
    QString port("port");
    ui->port->setText(obj.value(port).toString());
    QString dbName("dbName");
    ui->dbName->setText(obj.value(dbName).toString());
    QString username("username");
    ui->username->setText(obj.value(username).toString());
    QString password("password");
    ui->password->setText(obj.value(password).toString());

     QIntValidator *validatorInt = new QIntValidator(1, 65535, ui->port);
     ui->port->setValidator(validatorInt);
    this->show();
}

void dbdialog::saveDBConfig(){
    // 读取配置文件
    QJsonObject dbConfig;
    dbConfig["host"] = ui->host->text();
    dbConfig["port"] = ui->port->text();
    dbConfig["dbName"] = ui->dbName->text();
    dbConfig["username"] = ui->username->text();
    dbConfig["password"] = ui->password->text();

    QJsonDocument doc(dbConfig);
    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "错误：无法打开文件" << filePath;
        qDebug() << "具体错误：" << file.errorString();
    }
    file.write(jsonData); // 写入数据
    file.close();
    this->hide();
}

void dbdialog::hideDBConfig(){
    this->hide();
}
