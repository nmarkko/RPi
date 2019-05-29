#include "dialog.h"
#include "ui_dialog.h"
#include <QTimer>
#include <unistd.h>
#include <stdlib.h>

//NAPOMENA1!!!! Na početak imeProjekta.pro fajla dodati: LIBS += -L/usr/local/lib -lwiringPi

#include<wiringPiI2C.h>
#include<wiringPi.h>

//NAPOMENA2!!!! U .pro fajlu treba dodati red: QT += charts
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
QT_CHARTS_USE_NAMESPACE


//za napon
int fd, adcVal;
double napon;
const char PCF8591 = 0x48;
int t=0;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
	
    //ui->lcdNumber->display((QString)"10.0");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timerUpdate()));
    this->timer->start(1000);
    if(wiringPiSetup () == -1) exit (1);
    fd = wiringPiI2CSetup(PCF8591);
	
	QLineSeries *series = new QLineSeries();
	//series->append(0, 0);
	
	QChart *chart = new QChart();
	 chart->legend()->hide();
	 chart->addSeries(series);
	 chart->createDefaultAxes();
	 //postavljanje opsega
	 chart->axisX()->setMin(0);
	 chart->axisX()->setMax(5);
	 //2. način za postavljanje opsega
	 chart->axisY()->setRange(0,3.3);
	 chart->setTitle("Merenje napona");

	 QChartView *chartView = new QChartView(chart);
	 chartView->setRenderHint(QPainter::Antialiasing);

	 //postavljanje grafika u vertikal layout
	 ui->verticalLayout->addWidget(chartView);

}

Dialog::~Dialog()
{
    delete ui;
}



void Dialog::on_timerUpdate() {

	//napon na potenciometru
    wiringPiI2CReadReg8(fd, PCF8591 + 3);
    adcVal = wiringPiI2CReadReg8(fd, PCF8591 + 3);
    napon = (3.3*adcVal)/ 255;
	if(t==6) 
	{
		//void QXYSeries::removePoints(int index, int count)
		//Removes the number of points specified by count from 
		//the series starting at the position specified by index.
		removePoints(0, 6);
		t=0;
	}
	series->append(t, napon);
	t++;
	
}

//TODO: na pritisak Start dugmeta, startuj tajmer i
//na pritisak Obriši dugmeta zaustaviti merenje i obrisati podatke

/* void YourObjectClass::button_clicked()  //changing the state of the Timer with the button
{
  if (timer->isActive())
  {
    timer->stop();
  }
  else
  {
    timer->start(1000);
  }
} */

