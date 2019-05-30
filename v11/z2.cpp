#include "dialog.h"
#include "ui_dialog.h"
#include <bcm2835.h>
#include <QTimer>
#include <QString>
#include <unistd.h>
#include <stdlib.h>
#define changeHexToInt(hex)  ((((hex)>>4) *10 ) + ((hex)%16) )

#define SEK 0x02
#define MINu 0x03
#define SAT 0x04

/*unsigned*/ char WriteBuf[2];
/*unsigned*/ char ReadBuf;
/*unsigned*/ char g8563_Store[3];
/*unsigned*/ char init8563_Store[3]={0x00,0x59,0x08};

void P8563_settime()
{
    WriteBuf[0] = SEK;
    WriteBuf[1] = g8563_Store[0];
    bcm2835_i2c_write(WriteBuf,2);
    WriteBuf[0] = MINu;
    WriteBuf[1] = g8563_Store[1];
    bcm2835_i2c_write(WriteBuf,2);
    WriteBuf[0] = SAT;
    WriteBuf[1] = g8563_Store[2];
    bcm2835_i2c_write(WriteBuf,2);
}
void P8563_init()
{
    unsigned char i;
    for(i=0;i<=3;i++)
        g8563_Store[i]=init8563_Store[i];
    P8563_settime();
    printf("Postavi početno tekuće vreme\n");
    //inicijalizacija RTC-a
    WriteBuf[0] = 0x0;
    WriteBuf[1] = 0x00; //normalni rezim rada
    bcm2835_i2c_write(WriteBuf,2);
}



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timerUpdate()));
    this->timer->start(1000);
    if (!bcm2835_init())
        printf("nope");
    bcm2835_i2c_begin();
    // adresa PCF8563 na I2C magistrali
    bcm2835_i2c_setSlaveAddress(0x51);
    bcm2835_i2c_set_baudrate(10000);
    P8563_init() ;
}

Dialog::~Dialog()
{
    bcm2835_i2c_end();
    bcm2835_close();
    delete ui;
}



void P8563_Readtime()
{
    /*unsigned*/ char time[7];
    WriteBuf[0] = SEK;
    bcm2835_i2c_write_read_rs(WriteBuf, 1, time, 7);
    g8563_Store[0] = time[0] & 0x7f;
    g8563_Store[1] = time[1] & 0x7f;
    g8563_Store[2] = time[2] & 0x3f;
    g8563_Store[0] = changeHexToInt(g8563_Store[0]);
    g8563_Store[1] = changeHexToInt(g8563_Store[1]);
    g8563_Store[2] = changeHexToInt(g8563_Store[2]);
}

void Dialog::on_pushButton_clicked()
{


}

void Dialog::on_timerUpdate() {
    P8563_Readtime();
    ui->label_3->setText(QString::fromStdString("jkhfshajk"));


}
