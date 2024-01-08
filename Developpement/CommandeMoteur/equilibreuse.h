#ifndef EQUILIBREUSE_H
#define EQUILIBREUSE_H

#include <QMainWindow>
#include <QLabel>

#include "capot.h"
#include "moteur.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Equilibreuse; }
QT_END_NAMESPACE

class Equilibreuse : public QMainWindow
{
    Q_OBJECT

public:
    Equilibreuse(QWidget *parent = nullptr);
    ~Equilibreuse();
    void onCapot_EtatCapotChange(const bool _etat);

private slots:
    void on_pushButton_Lancer_clicked();

    void on_pushButton_Arreter_clicked();

    void on_dial_ConsigneVitesse_valueChanged(int value);

private:
    Ui::Equilibreuse *ui;

    Moteur *leMoteur;
    Capot *leCapot;
    MccUldaq laCarte;

    QLabel labelEtatCapot;
};
#endif // EQUILIBREUSE_H
