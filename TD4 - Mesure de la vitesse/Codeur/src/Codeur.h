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

#ifndef CODEUR_H
#define CODEUR_H

#include <Arduino.h>

#define INDEX  15  //15   //normalement la 5 mais ne fonctionne pas
#define VOIE_A 17
#define VOIE_B 2

#define SAMPLING_FREQUENCY 50

class Codeur {
public:
    Codeur();
    void begin(int _splFreq = SAMPLING_FREQUENCY);
    void stop();
    void start();
    void setSampleFrequency(int _splFreq);
    virtual ~Codeur();
private:
    enum {A,B};
    
    int indice;
    static void marshall();
    static Codeur* anchor;
    void  IRAM_ATTR interuption();
    hw_timer_t * timer; 
};

#endif /* CODEUR_H */

