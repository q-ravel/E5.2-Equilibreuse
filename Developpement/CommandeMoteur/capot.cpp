#include "capot.h"

Capot::Capot(MccUldaq &_laCarte, const int _numDio, QObject *parent):
QObject(parent), numDio(_numDio), laCarte(_laCarte)
{
    laCarte.ulDConfigBit(numDio, DD_INPUT);
    laCarte.ulDBitIn(numDio, etatCapot);
    connect(&timerCapot, &QTimer::timeout, this, &Capot::onTimerCapot_timeout);
    timerCapot.start(500);
}

void Capot::onTimerCapot_timeout()
{
    laCarte.ulDBitIn(numDio, etatCourant);

    if(etatCapot != etatCourant)
    {
        emit EtatCapotChange(etatCourant);
        etatCapot = etatCourant;
    }
}

