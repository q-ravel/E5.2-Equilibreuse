/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Codeur.h
 * Author: pcruchet
 *
 * Created on 22 novembre 2021, 13:35
 */

#ifndef EQUILIBREUSE_H
#define EQUILIBREUSE_H

#include <Arduino.h>
#include <driver/dac.h>

#define LED1 13

#define INDEX  15  
#define VOIE_A 17  //normalement la 5 mais ne fonctionne pas
#define VOIE_B 2


#define SAMPLING_FREQUENCY 50

class Equilibreuse {
public:
    Equilibreuse();
    void beginDds(int _splFreq = SAMPLING_FREQUENCY);
    void AvancerCodeur();
    void ReculerCodeur();
    void Casser(int _splFreq);
    void stop();
    void start();
    void setSampleFrequency(int _splFreq);
    virtual ~Equilibreuse();
    void setBruitA(bool value=true);
    void setBruitO(bool value=true);
    bool getBruitA() const;
    bool getBruitO() const;
    bool getDephasageO() const;
    void setDephasageO(bool value);

    bool getAttenuationO() const;
    void setAttenuationO(bool value);

    bool getCodeurA() const;
    bool getCodeurB() const;
    bool getIndexCodeur() const;

    void ResetIndice();

private:

    enum {
        A, B
    };

    int indice;
    static void marshallDds();
    static Equilibreuse* anchor;
    void IRAM_ATTR interuptionDds();
    hw_timer_t * timer;


    // Dds
    volatile uint32_t accumulateur;
    volatile uint32_t incrementPhase; // Increment de phase courant
    volatile uint32_t dephase; // Valeur du déphasage de la porteuse  
    int splFreq; // Fréquence d'échantillonage
    int attenuation;
    uint8_t sinus1;
    uint8_t sinus2;
    uint8_t bruit;
    
    volatile uint32_t accuBruit;
    volatile uint32_t incrementBruit;

    bool bruitA;
    bool bruitO;
    bool dephasageO;
    bool attenuationO;
    bool casse;

    bool codeurA;
    bool codeurB;
    bool indexCodeur;




};

#endif /* EQUILIBREUSE_H */

