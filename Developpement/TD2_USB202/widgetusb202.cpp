#include "widgetusb202.h"
#include "ui_widgetusb202.h"

WidgetUSB202::WidgetUSB202(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetUSB202)
{
    ui->setupUi(this);

    UlError erreur;

    erreur = laCarte.ulDConfigPort(DD_INPUT);
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Config port en entrée. Code Erreur : " << erreur;
    }


    erreur = laCarte.ulDConfigBit(LED1,DD_OUTPUT);
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Config " << LED1 << " Code Erreur : " << erreur;
    }

    erreur = laCarte.ulDConfigBit(LED2,DD_OUTPUT);
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Config " << LED2 << " Code Erreur : " << erreur;
    }

    erreur = laCarte.ulDConfigBit(LED3,DD_OUTPUT);
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Config " << LED3 << " Code Erreur : " << erreur;
    }

    erreur = laCarte.ulDConfigBit(LED4,DD_OUTPUT);
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Config " << LED4 << " Code Erreur : " << erreur;
    }

    connect(&timerBP, &QTimer::timeout, this, &WidgetUSB202::onTimerBP_timeOut);
    timerBP.start(500);
}

WidgetUSB202::~WidgetUSB202()
{
    delete ui;
}


void WidgetUSB202::on_checkBox_LED1_stateChanged(int arg1)
{
    UlError erreur;
    erreur = laCarte.ulDBitOut(LED1,static_cast<bool>(arg1));
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Ecriture DIO" << LED1 << " Code Erreur : " << erreur;
    }
}


void WidgetUSB202::on_checkBox_LED2_stateChanged(int arg1)
{
    UlError erreur;
    erreur = laCarte.ulDBitOut(LED2,static_cast<bool>(arg1));
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Ecriture DIO" << LED2 << " Code Erreur : " << erreur;
    }
}


void WidgetUSB202::on_checkBox_LED3_stateChanged(int arg1)
{
    UlError erreur;
    erreur = laCarte.ulDBitOut(LED3,static_cast<bool>(arg1));
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Ecriture DIO" << LED3 << " Code Erreur : " << erreur;
    }
}


void WidgetUSB202::on_checkBox_LED4_stateChanged(int arg1)
{
    UlError erreur;
    erreur = laCarte.ulDBitOut(LED4,static_cast<bool>(arg1));
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Ecriture DIO" << LED4 << " Code Erreur : " << erreur;
    }
}

void WidgetUSB202::onTimerBP_timeOut()
{
    quint8 valeur;
    UlError erreur;
    erreur = laCarte.ulDIn(valeur);
    if(erreur != ERR_NO_ERROR)
    {
        qDebug() << "Lecture port" << " Code Erreur : " << erreur;
    }
    if(valeur & 0x01)
    {
        ui->label_BP1->setStyleSheet("background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->label_BP1->setStyleSheet("background-color: rgb(0, 255, 0)");
    }

    if(valeur & 0x02)
    {
        ui->label_BP2->setStyleSheet("background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->label_BP2->setStyleSheet("background-color: rgb(0, 255, 0)");
    }

    if(valeur & 0x04)
    {
        ui->label_BP3->setStyleSheet("background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->label_BP3->setStyleSheet("background-color: rgb(0, 255, 0)");
    }

    if(valeur & 0x08)
    {
        ui->label_BP4->setStyleSheet("background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->label_BP4->setStyleSheet("background-color: rgb(0, 255, 0)");
    }
}
