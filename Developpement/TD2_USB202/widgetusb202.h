#ifndef WIDGETUSB202_H
#define WIDGETUSB202_H

#include <QWidget>
#include "mcculdaq.h"

#include <QTimer>

#include <QtGlobal>
#include "../include/uldaq.h"
#include "../include/UlDaqDeviceManager.h"
#include "../include/UlException.h"
#include "../include/DioDevice.h"
#include "../include/AoDevice.h"
#include "../include/AiDevice.h"
#include "../include/CtrDevice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetUSB202; }
QT_END_NAMESPACE

class WidgetUSB202 : public QWidget
{
    Q_OBJECT

public:
    WidgetUSB202(QWidget *parent = nullptr);
    ~WidgetUSB202();
    enum LED {LED1 = 4, LED2, LED3, LED4};
    enum BP {BP1, BP2, BP3, BP4};

private slots:
    void on_checkBox_LED1_stateChanged(int arg1);

    void on_checkBox_LED2_stateChanged(int arg1);

    void on_checkBox_LED3_stateChanged(int arg1);

    void on_checkBox_LED4_stateChanged(int arg1);

    void onTimerBP_timeOut();

private:
    Ui::WidgetUSB202 *ui;
    QTimer timerBP;
    MccUldaq laCarte;
};
#endif // WIDGETUSB202_H
