#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timerUpdate()));
    this->timer->start(1000);
}

QByteArray temp;
int vrednost;

void Dialog::Connect()
{
    QTcpSocket *socket = new QTcpSocket();
    socket -> connectToHost("10.1.220.234",1234);
    if(socket->waitForConnected(500))
    //uspesno uspostavljena veza
    {
        socket->write("osvetljaj");
        socket->waitForBytesWritten(500);
        if(socket->waitForReadyRead(500))
            temp=socket->readAll();
        else
            std::cout<<"Fejlovanje pri citanju"<<std::endl;
        vrednost=temp.toInt();
        ui->verticalSlider->setValue(vrednost);

        socket->write("potenciometar");
        socket->waitForBytesWritten(500);
        if(socket->waitForReadyRead(500))
            temp=socket->readAll();
        else
            std::cout<<"Fejlovanje pri citanju"<<std::endl;
        vrednost=temp.toInt();
        ui->dial->setValue(vrednost);

        socket->write("temperatura");
        socket->waitForBytesWritten(500);
        if(socket->waitForReadyRead(500))
            temp=socket->readAll();
        else
            std::cout<<"Fejlovanje pri citanju"<<std::endl;
        vrednost=temp.toInt();
        ui->lcdNumber->display(QString::number(vrednost));

        socket->close();
    }
    //else
    //problem pri uspostavljanju veze
}

void Dialog::on_timerUpdate() {
    Connect();
}

Dialog::~Dialog()
{
    delete ui;
}
