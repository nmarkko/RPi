#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
	
private slots:
    void on_timerUpdate();

private:
    Ui::Dialog *ui;

    QTimer *timer;

};

#endif // DIALOG_H
