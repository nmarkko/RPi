#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include <iostream>
#include <QTimer>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    void Connect();
    ~Dialog();

private slots:
    void on_timerUpdate();

private:
    Ui::Dialog *ui;
    QTimer *timer;
};

#endif // DIALOG_H
