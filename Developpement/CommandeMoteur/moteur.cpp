#include "moteur.h"
#include <QDebug>

Moteur::Moteur(MccUldaq &_laCarte, const int _numCanal, const double _tensionMax):
    laCarte(_laCarte),
    numCanal(_numCanal),
    tensionMaxCommande(_tensionMax)
{
    UlError erreur;
    erreur = laCarte.ulAOut(numCanal,0);
    if(erreur != ERR_NO_ERROR)
        qDebug() << "Constructeur moteur Code erreur MccUldaq : " << erreur ;
}

Moteur::~Moteur()
{
    UlError erreur;
    erreur = laCarte.ulAOut(numCanal,0);
    if(erreur != ERR_NO_ERROR)
        qDebug() << "Destructeur moteur Code erreur MccUldaq : " << erreur ;
}

void Moteur::FixerConsigneVitesse(const int _pourcentage)
{
    double valeurTension = tensionMaxCommande * _pourcentage / 100.0 ;
    UlError erreur;
    erreur = laCarte.ulAOut(numCanal,valeurTension);
    if(erreur != ERR_NO_ERROR)
        qDebug() << "Fixer Consigne vitess Code erreur MccUldaq : " << erreur ;
}

