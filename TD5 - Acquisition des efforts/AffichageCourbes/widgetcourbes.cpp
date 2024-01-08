#include <QDebug>

#include "widgetcourbes.h"
#include "ui_widgetcourbes.h"


WidgetCourbes::WidgetCourbes(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetCourbes)
{
    ui->setupUi(this);
    connect(&timerMesure,&QTimer::timeout,this,&WidgetCourbes::onTimerMesure_timeout);


    courbeA.setName("Palier A");

    courbeO.setName("Palier O");


    chartView = new QChartView(&chart);
    layout = new QHBoxLayout(ui->widgetCourbes);
    layout->addWidget(chartView);
    chart.addSeries(&courbeA);
    chart.addSeries(&courbeO);
    chart.createDefaultAxes();

    QList<QAbstractAxis *> axe = chart.axes();
    if(axe.size() == 2)
    {
        QValueAxis *axeY = static_cast<QValueAxis *>(axe.last());
        axeY->applyNiceNumbers();
        axeY->setTickCount(5);
        axeY->setMinorTickCount(2);
        axeY->setLabelFormat("%d");
        axeY->setMin(-10);
        axeY->setMax(+10);

        QValueAxis *axeX = static_cast<QValueAxis *>(axe.first());
        axeX->setLabelFormat("%d");
        axeX->setMax(NB_ECHANTILLONS_PAR_CANAL);
    }
    valeursBrutes = new double[NB_ECHANTILLONS_PAR_CANAL * NB_CANAUX];
}

WidgetCourbes::~WidgetCourbes()
{
    delete layout;
    delete chartView;
    delete ui;

}


void WidgetCourbes::on_pushButton_Lancer_clicked()
{
    double vitesse = 33000;
    if(ui->pushButton_Lancer->text()=="Lancer")
    {
        ScanOption options = static_cast<ScanOption>(SO_SINGLEIO | SO_CONTINUOUS | SO_EXTCLOCK | SO_EXTTRIGGER);
        qDebug() << "scanOption : " << Qt::hex << options;
        AInScanFlag flags = AINSCAN_FF_DEFAULT ;
        UlError erreur = laCarte.ulAInScan(CH0,CH1,NB_ECHANTILLONS_PAR_CANAL,&vitesse,options,flags,valeursBrutes);
        qDebug()<< "ulAInScan " << erreur;
        timerMesure.start(1000);
        ui->pushButton_Lancer->setText("Arrêter");
    }
    else
    {
        timerMesure.stop();
        laCarte.ulAInScanStop();
        ui->pushButton_Lancer->setText("Lancer");
    }
}

void WidgetCourbes::onTimerMesure_timeout()
{
    ScanStatus status;
    TransferStatus transferStatus;
    UlError erreur = laCarte.ulAInScanStatus(status,transferStatus);
    qDebug()<< "ulAInScanStatus " << erreur;
    qDebug() <<"Status " << status;
    if(status == SS_RUNNING)
    {
         QString resultat;
        if(erreur == ERR_NO_ERROR)
        {
            int index = transferStatus.currentIndex;
            int totalEchantillons = transferStatus.currentTotalCount;

            courbeA.clear();
            courbeO.clear();
            if(totalEchantillons > NB_ECHANTILLONS_PAR_CANAL)
                index = NB_ECHANTILLONS_PAR_CANAL;
            for (int i=0;i<index*NB_CANAUX;i+=NB_CANAUX) {
                courbeA << QPointF(i,valeursBrutes[i]);
                courbeO << QPointF(i,valeursBrutes[i+1]);
            }
            chartView->setRenderHint(QPainter::Antialiasing);
        }
    }
}
