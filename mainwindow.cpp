#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editpath.h"
#include "dbdialog.h"
#include "detaildialog.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include <QCheckBox>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QJsonArray>
#include <QDebug>
#include<QSqlDatabase>
#include <QSqlQuery>
#include <QSystemTrayIcon>
#include <QIcon>
#include "tot_file.h"
#include "inja.hpp"
#include "Toast.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":imgs/fav.ico"));
    this->setFixedSize(this->size());
    setWindowFlags(
        Qt::Window |
        Qt::WindowCloseButtonHint |
        Qt::WindowMinimizeButtonHint
        );


    editPath = new editpath(this);
    editPath->setModal(true);
    this->initDataList();
    this->initConnect();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initDataList(){
    // 设置表头
    // 设置自定义的水平表头
    // |全选 | 文件名(基本) | 文件地址 | 是否覆盖| 是否生成 "

    ui->dataList->setColumnCount(4);
    QStringList headers;
    headers <<"模板名"<< "保存路径" << "是否覆盖"<<"是否生成";
    ui->dataList->setHorizontalHeaderLabels(headers);
    // 读取文件
    // 读取配置文件
    QFile file(this->filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = doc.array();
    ui->dataList->setRowCount(jsonArray.size());
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonValue value = jsonArray.at(i);
        QJsonObject obj = value.toObject();
        ui->dataList->setItem(i, 0, new QTableWidgetItem(obj.value("name").toString()));
        ui->dataList->setItem(i, 1, new QTableWidgetItem(obj.value("path").toString()));
        QTableWidgetItem *checkboxItem = new QTableWidgetItem();
        checkboxItem->setCheckState(Qt::Unchecked);
        bool overwrite = obj.value("overwrite").toBool();
        if(overwrite)checkboxItem->setCheckState(Qt::Checked);
        checkboxItem->setFlags(checkboxItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->dataList->setItem(i, 2, checkboxItem);

        QTableWidgetItem *checkboxItem2 = new QTableWidgetItem();
        checkboxItem2->setCheckState(Qt::Unchecked);
        bool needCreate = obj.value("needCreate").toBool();
        if(needCreate)checkboxItem2->setCheckState(Qt::Checked);
        checkboxItem2->setFlags(checkboxItem2->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->dataList->setItem(i, 3, checkboxItem2);
    }

    //  设置选择模式
    ui->dataList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dataList->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置表头不允许拉伸
    ui->dataList->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->dataList->setColumnWidth(0,90);
    ui->dataList->setColumnWidth(1,900-3*95);
    ui->dataList->setColumnWidth(2,90);
    ui->dataList->setColumnWidth(3,90);
    ui->dataList->horizontalHeader()->setStyleSheet(
                "QHeaderView::section {"
                "   background-color: #00c2a8;"
                "   color: white;"
                "   border: 1px solid #3c3c3c;"
                "   margin: 1px;"
                "}"
        );
    ui->dataList->show();
    connect(ui->dataList, &QTableWidget::cellDoubleClicked,editPath,&editpath::onTableDoubleClicked);
}
void MainWindow::initConnect(){
    dbdialog *d1 = new dbdialog();
    detaildialog *d2 = new detaildialog();
    connect(ui->linkButton,&QPushButton::clicked,d1,&dbdialog::showDBConfig);
    connect(ui->detailButton,&QPushButton::clicked,d2,&detaildialog::showDetailDialog);
    connect(editPath, &editpath::updatePath,this, &MainWindow::initDataList);
    connect(ui->runButton,&QPushButton::clicked,this,&MainWindow::createFile);
    connect(ui->dataList, &QTableWidget::cellChanged, this, &MainWindow::onTableCellChanged);
}

void MainWindow::createFile(){
    // 如果没内容则不处理
    QString input = ui->tableName->text();
    if(input == ""){
        return;
    }

    // 准备测试mysql
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QJsonObject  dbConfig = readJSON(dbConfigPath);
    db.setHostName(dbConfig.value("host").toString());  //连接本地主机
    db.setPort(dbConfig.value("port").toInt());
    db.setDatabaseName(dbConfig.value("dbName").toString());  //Mysql 创建的数据库名称
    db.setUserName(dbConfig.value("username").toString());
    db.setPassword(dbConfig.value("password").toString());    //安装 Mysql 设置的密码
    bool ok = db.open();
    if (!ok){
        QMessageBox::critical(this,"提示","数据库连接失败");
        return;
    }
    QSqlQuery query("select COLUMN_NAME columnName,DATA_TYPE dataType,if(COLUMN_KEY='PRI',1,0) isPrimary from information_schema.`COLUMNS`  where TABLE_NAME='"+ui->tableName->text()+"' and TABLE_SCHEMA='"+dbConfig.value("dbName").toString()+"'  ORDER BY ORDINAL_POSITION ");
    nlohmann::json data;
    data["columns"] = nlohmann::json::array();
    while (query.next()) {
        QString columnName = query.value("columnName").toString();
        QString dataType = query.value("dataType").toString();
        int isPri = query.value("isPrimary").toInt();
        data["columns"].push_back({
                                   {"columnName",columnName.toStdString()},
                                   {"dataType",dataType.toStdString()},
                                   {"isPrimary",isPri}
        });
    }
    db.close();
    inja::Environment env;
    // 获取文件的
    QJsonArray fileArray = readJSONArray(filePath);
    for (int i = 0; i < fileArray.size(); ++i) {
        QJsonValue value = fileArray.at(i);
        QJsonObject obj = value.toObject();
        bool needCreate = obj.value("needCreate").toBool();
        if(!needCreate)return;
        // 处理基本数据
        std::map<std::string,std::string> replaceMap;
        // 基本赋值
        data["tableName"] = ui->tableName->text().toStdString();
        data["className"] = tot::underscoreToUpperCamelCase(ui->tableName->text().toStdString());
        data["clazzName"] = tot::underscoreToCamelCaseFirstLower(ui->tableName->text().toStdString());
        //
        replaceMap["{{tableName}}"] = ui->tableName->text().toStdString();
        replaceMap["{{className}}"] = tot::underscoreToUpperCamelCase(ui->tableName->text().toStdString());
        replaceMap["{{clazzName}}"] = tot::underscoreToCamelCaseFirstLower(ui->tableName->text().toStdString());

        // 属性中的数据
        QJsonObject replaceJSON = readJSON(detailConfigPath);
        for(auto key:replaceJSON.keys()){
            data[key.toStdString()] = replaceJSON.value(key).toString().toStdString();
            replaceMap["{{"+key.toStdString()+"}}"] = replaceJSON.value(key).toString().toStdString();
        }
        std::string path = templateDir.toStdString()+obj.value("name").toString().toStdString();
        std::string resultContent = tot::resetTemplate(path);
        // 利用inja处理
        std::string finalString = env.render(resultContent, data);
        tot::createFile(obj.value("path").toString().toStdString(),replaceMap,obj.value("overwrite").toBool(),finalString);
        QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon("icon.png"), this);
        QString trayMessage = obj.value("name").toString()+"模板处理成功!";
        Toast::showText(i+1,this,trayMessage, 2000); // 显示2秒

    }

}

QJsonArray MainWindow::readJSONArray(QString path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = doc.array();
    return jsonArray;
}
QJsonObject MainWindow::readJSON(QString path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject obj = doc.object();
    return obj;
}
void MainWindow::onTableCellChanged(int row, int column){
    // 更新数据
    QTableWidgetItem *item = ui->dataList->item(row, column);


    Qt::CheckState state = item->checkState();
    QFile file(this->filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = doc.array();

    QJsonValue value = jsonArray.at(row);
    QJsonObject obj = value.toObject();
    // 复写
    if(column == 2)   {
        bool overwrite = obj.value("overwrite").toBool();
        bool needOver = (state == Qt::CheckState::Checked);
        if (needOver == overwrite) {
            return;
        };
        obj["overwrite"] = needOver;
    }
    // 生成
    if(column == 3)   {
        bool needCreate = obj.value("needCreate").toBool();
        bool needCreate2 = (state == Qt::CheckState::Checked);
        if (needCreate == needCreate2) {
            return;
        };
        obj["needCreate"] = needCreate2;
    }
    jsonArray.replace( row, obj);
    QJsonDocument doc2(jsonArray);
    QByteArray updatedJsonData = doc2.toJson(QJsonDocument::Indented);
    QFile file2(filePath);
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
    }
    file2.write(updatedJsonData); // 写入数据
    file2.close();

}

