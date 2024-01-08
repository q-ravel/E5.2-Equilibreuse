#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&timer,&QTimer::timeout,this,&Widget::on_reglageMesureTension);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="Lancer mesure")
    {
    ui->pushButton->setText("Arrêter Mesure");
    timer.start(500);
    }
    else{
        ui->pushButton->setText("Lancer mesure");
        timer.stop();
    }
}

void Widget::on_reglageMesureTension()
{
     double valeur;
     laCarte.ulAIn(0,valeur);
      ui->lcdNumber->display(valeur);
}

