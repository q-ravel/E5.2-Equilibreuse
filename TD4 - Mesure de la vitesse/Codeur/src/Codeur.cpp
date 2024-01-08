/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Codeur.cpp
 * Author: pcruchet
 * 
 * Created on 22 novembre 2021, 13:35
 */

#include "Codeur.h"

Codeur* Codeur::anchor = NULL;

Codeur::Codeur() :
indice(0),
timer(NULL)
{
    anchor = this;
    pinMode(INDEX, OUTPUT);
    pinMode(VOIE_A, OUTPUT);
    pinMode(VOIE_B, OUTPUT);
}

void Codeur::begin(int _splFreq)
{
    timer = timerBegin(1, 80, true);
    timerAttachInterrupt(timer, Codeur::marshall, true);
    timerAlarmWrite(timer,(1000000 / (_splFreq*4)), true);
    start();
}

void Codeur::stop()
{
    timerAlarmDisable(timer);
}

void Codeur::start()
{
    timerAlarmEnable(timer);
}
Codeur::~Codeur()
{
}

void IRAM_ATTR Codeur::interuption()
{
    uint8_t etat[2][4] PROGMEM = {{HIGH,LOW,LOW,HIGH},{HIGH,HIGH,LOW, LOW}};
    
    if(indice & 0x01)
        digitalWrite(VOIE_B, etat[B][indice & 0x03]);
    else
        digitalWrite(VOIE_A, etat[A][indice & 0x03]);

    indice++;
    if(indice % 4000 == 0)
    {
        digitalWrite(INDEX,HIGH);
        indice = 0;       
    }
    else
    {
        digitalWrite(INDEX,LOW);
    }
}

void Codeur::marshall()
{
    anchor->interuption();
}

void Codeur::setSampleFrequency(int _splFreq)
{
    stop();
    timerAlarmWrite(timer,1000000/(_splFreq*4), true);
    start();
}

