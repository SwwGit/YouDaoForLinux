#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QtNetwork>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
  void on_textEdit_textChanged();

  void on_pushButton_clicked();
   void replyFinished(QNetworkReply* reply);
private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *manager;
};

#endif // MAINWINDOW_H
