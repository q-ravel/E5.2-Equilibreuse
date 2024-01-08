#ifndef CODEUR_H
#define CODEUR_H

#include "mcculdaq.h"
#include <QObject>
#include "equilibreuse.h"
#include <QTimer>

class Codeur
{
public:
    Codeur(const int _numDio, const MccUldaq _laCarte, const QObject *parent);
    ~Codeur();

    int vitesse;

    void onTimerVitesse_timeout();
    void LancerMesureVitesse();
    void ArreterMesureVitesse();

signals:
    void VitesseChange();

private:
    MccUldaq *laCarte;
    Equilibreuse *leCodeur;
    QTimer *timerVitesse;
};

#endif // CODEUR_H
