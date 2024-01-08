#include "equilibreuse.h"
#include "ui_equilibreuse.h"

Equilibreuse::Equilibreuse(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Equilibreuse)
{
    ui->setupUi(this);

    leCapot = new Capot(laCarte, 0, dynamic_cast<QObject *>(this));

    leMoteur = new Moteur(laCarte, 0, 4);

    ui->statusbar->addPermanentWidget(&labelEtatCapot);

    connect(leCapot, &Capot::EtatCapotChange, this, &Equilibreuse::onCapot_EtatCapotChange);
}

Equilibreuse::~Equilibreuse()
{
    delete leCapot;
    delete leMoteur;
    delete ui;
}

void Equilibreuse::onCapot_EtatCapotChange(const bool _etat)
{
    QPalette palette;
    labelEtatCapot.setAutoFillBackground(true);
    if(_etat)
    {
        palette.setColor(QPalette::WindowText,Qt::black);
        labelEtatCapot.setPalette(palette);
        labelEtatCapot.setText("| Capot fermé |");
    }
    else
    {
        palette.setColor(QPalette::WindowText,Qt::red);
        labelEtatCapot.setPalette(palette);
        labelEtatCapot.setText("| Capot ouvert |");

        ui->dial_ConsigneVitesse->setValue(0);
        leMoteur->FixerConsigneVitesse(0);

        if (ui->pushButton_Lancer->text() != "Lancer Moteur")
        {
            ui->pushButton_Lancer->setText("Lancer Moteur");
        }
    }
}


void Equilibreuse::on_pushButton_Lancer_clicked()
{
    if (ui->pushButton_Lancer->text() == "Lancer Moteur")
    {
        ui->pushButton_Lancer->setText("Fixer Consigne");
    }
    // Code Fixer Consigne
    leMoteur->FixerConsigneVitesse(ui->dial_ConsigneVitesse->value());
}


void Equilibreuse::on_pushButton_Arreter_clicked()
{
    if (ui->pushButton_Lancer->text() != "Lancer Moteur")
    {
        ui->pushButton_Lancer->setText("Lancer Moteur");
    }
    // Code Fixer Consigne
    leMoteur->FixerConsigneVitesse(0);
}


void Equilibreuse::on_dial_ConsigneVitesse_valueChanged(int value)
{
    ui->lcdNumber_consigne->display(value);
}

