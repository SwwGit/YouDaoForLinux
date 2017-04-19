#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonObject>

QString getFormatString(const QJsonObject &json){
    QString baseString = "译文 : \n";
    QJsonArray translations = json.value("translation").toArray();
    for(int i = 0;i < translations.count() ;i ++){
       baseString += ("     " + translations.at(i).toString() + "\n");
    }
    baseString += "\n";
    //basic
    if(!json.value("basic").isNull()){
        QJsonObject basic = json.value("basic").toObject();
        if(basic.value("us-phonetic").isNull()||basic.value("us-phonetic").isUndefined()){
            //中文
            baseString += ("拼音 : " + basic.value("phonetic").toString() + "\n");
        }else{
            //英文
            baseString += ("英 : [" + basic.value("uk-phonetic").toString() + "]    美 : [" + basic.value("us-phonetic").toString() + "]\n");
        }
        baseString += "\n解释说明:\n";
        QJsonArray explains =basic.value("explains").toArray();
        for(int i = 0;i < explains.count() ;i ++){
           baseString += ("     " + QString::number(i+1) + ":  " + explains.at(i).toString() + "\n");
        }
    }
    baseString += "\n网络释义:\n";
    //web
    if(!json.value("web").isNull()){
        QJsonArray webs = json.value("web").toArray();
        for(int i = 0;i < webs.count() ;i ++){
            QJsonObject obj = webs.at(i).toObject();
            QString key = obj.value("key").toString();
            baseString += "\n" + key + ":\n";
            QJsonArray values = obj.value("value").toArray();
            for(int j = 0;j < values.count() ;j ++){
                QString value = values.at(j).toString();
                   baseString += "      " + value + "\n";
            }
        }
    }

    return baseString;
}

// QString >> QJson
QJsonObject getJsonObjectFromString(const QString jsonString){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
// QJson >> QString
QString getStringFromJsonObject(const QJsonObject& jsonObject){
    return QString(QJsonDocument(jsonObject).toJson());
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   manager = new QNetworkAccessManager(this);
   QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_textEdit_textChanged()
{
    QString text = ui->textEdit->toPlainText();
    if (text.length() < 1){
        return;
    }
    if (text[text.length()-1] == "\n"){
        QTextCursor cursor;
        cursor = ui->textEdit->textCursor();
        ui->textEdit->setPlainText(text.remove(text.length()-1,1));
        ui->textEdit->setTextCursor(cursor);
        on_pushButton_clicked();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString url = "http://fanyi.youdao.com/openapi.do?keyfrom=LinuxTranslate&key=1964007130&type=data&doctype=json&version=1.1&q=";
    QString fullUrl = url + ui->textEdit->toPlainText();
    qDebug() << fullUrl;
    QNetworkRequest request;
    request.setUrl(fullUrl);

//QNetworkReply* reply =
    manager->get(request);
}
void MainWindow::replyFinished(QNetworkReply *reply){
    QByteArray data = reply->readAll();
    QString jsonStr = data;
    qDebug() << jsonStr;
    QJsonObject json = getJsonObjectFromString(jsonStr);
    ui->textBrowser->setText(getFormatString(json));

}
