#include "dialog.h"
#include "ui_dialog.h"
#include <QTimer>
#include <unistd.h>
#include <stdlib.h>

#include<wiringPiI2C.h>
#include<wiringPi.h>

//za temperaturu
FILE *ft;
char tekst[100];
double broj;
//za napon
int fd, adcVal;
int napon;
const char PCF8591 = 0x48;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->lcdNumber->display((QString)"10.0");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timerUpdate()));
    this->timer->start(1000);
    if(wiringPiSetup () == -1) exit (1);
    fd = wiringPiI2CSetup(PCF8591);
}

Dialog::~Dialog()
{
    delete ui;
}


double readTemperatureSensor(){
    //PAZNJA: 7237df2 u putanji navedenoj ispod je specifican broj koji kernel dodeli
	//menjati po potrebi
    ft=fopen("/sys/bus/w1/devices/28-000007237df2/w1_slave","r");
    if(ft==NULL) return 0;
    int i=0;
    for(i=0;i<22;i++) //samo temperatura
    fscanf(ft,"%s", tekst);
    fclose(ft);
    //obrisati „t=”
    for(i=0;i<10;i++) tekst[i]=tekst[i+2];
    int temp=atoi(tekst); //prebaci u double
    double tem=(double)temp/1000;
    return tem;

}

void Dialog::on_timerUpdate() {
	//ocitaj i ispisi temperaturu
    broj=readTemperatureSensor();
    ui->lcdNumber->display(QString::number(broj));
	//napon na potenciometru
    wiringPiI2CReadReg8(fd, PCF8591 + 3);
    adcVal = wiringPiI2CReadReg8(fd, PCF8591 + 3);
    napon = (99*adcVal)/ 255;
    ui->dial->setValue(napon);
	//TODO: prikaz osvetljenja na slider-u
    //ui->verticalSlider->setValue(osvetljenje);
}



