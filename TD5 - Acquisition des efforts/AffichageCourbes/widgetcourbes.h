#ifndef WIDGETCOURBES_H
#define WIDGETCOURBES_H

#include <QWidget>
#include <QTimer>
#include <QtCharts>

#include "mcculdaq.h"
#include "constantes.h"



QT_BEGIN_NAMESPACE
namespace Ui { class WidgetCourbes; }
QT_END_NAMESPACE

class WidgetCourbes : public QWidget
{
    Q_OBJECT

public:
    WidgetCourbes(QWidget *parent = nullptr);
    ~WidgetCourbes();

private slots:
    void on_pushButton_Lancer_clicked();
    void onTimerMesure_timeout();

private:
    Ui::WidgetCourbes *ui;
    MccUldaq laCarte;
    QTimer timerMesure;

    double *valeursBrutes;

    QChartView *chartView;
    QHBoxLayout *layout;
    QSplineSeries courbeA;
    QSplineSeries courbeO;
    QChart chart;
};
#endif // WIDGETCOURBES_H
