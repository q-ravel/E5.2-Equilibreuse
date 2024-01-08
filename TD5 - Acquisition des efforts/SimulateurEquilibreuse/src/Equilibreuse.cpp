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

#include "Equilibreuse.h"

Equilibreuse* Equilibreuse::anchor = NULL;

Equilibreuse::Equilibreuse() :
    indice(0),
    timer(NULL),
    accumulateur(0),
    incrementPhase(pow(2, 32) / 1000),
    dephase(0),
    attenuation(1),
    accuBruit(0),
    incrementBruit(incrementPhase << 3), // indique le niveau d'harmonique
    bruitA(true),
    bruitO(true),
    dephasageO(true),
    attenuationO(true),
    casse(false)

{
    anchor = this;
    pinMode(INDEX, OUTPUT);
    pinMode(VOIE_A, OUTPUT);
    pinMode(VOIE_B, OUTPUT);
}

void Equilibreuse::beginDds(int _splFreq)
{
    splFreq = _splFreq;
    timer = timerBegin(1, 80, true);
    timerAttachInterrupt(timer, Equilibreuse::marshallDds, true);
    timerAlarmWrite(timer, (1000000 / (_splFreq * 4)), true);
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_2);
    start();
}

void Equilibreuse::AvancerCodeur()
{
    const static byte etat[2][4] PROGMEM = {{HIGH, LOW, LOW, HIGH},{HIGH, HIGH, LOW, LOW}};

    if (indice & 0x01)
    {
        codeurB = etat[B][indice & 0x03];
        digitalWrite(VOIE_B, etat[B][indice & 0x03]); // vitesse
    }
    else
    {
        codeurA = etat[A][indice & 0x03];
        digitalWrite(VOIE_A, etat[A][indice & 0x03]); // echantillonage
    }
    indice++;
    if (indice % 4000 == 0)
    {
        digitalWrite(INDEX, HIGH);
        indice = 0;
    }
    else
    {
        digitalWrite(INDEX, LOW);
    }
}

void Equilibreuse::ReculerCodeur()
{
    const static byte etat[2][4] PROGMEM = {{HIGH, LOW, LOW, HIGH},{HIGH, HIGH, LOW, LOW}};

    if (indice & 0x01)
    {
        codeurB = etat[B][indice & 0x03];
        digitalWrite(VOIE_B, etat[B][indice & 0x03]); // vitesse
    }
    else
    {
        codeurA = etat[A][indice & 0x03];
        digitalWrite(VOIE_A, etat[A][indice & 0x03]); // echantillonage
    }
    indice--;
    if(indice == -1)
        indice = 3999;

    if (indice % 4000 == 0)
    {
        digitalWrite(INDEX, HIGH);
        indice = 0;
    }
    else
    {
        digitalWrite(INDEX, LOW);
    }
    Serial.printf("indice = %d\n",indice);
}

void Equilibreuse::Casser(int _splFreq)
{
    casse = true;
    stop();
    timerAlarmWrite(timer, 1000000 / (_splFreq * 4), true);
    start();
}

void Equilibreuse::stop()
{
    timerAlarmDisable(timer);
}

void Equilibreuse::start()
{
    timerAlarmEnable(timer);
}

Equilibreuse::~Equilibreuse()
{
}

void IRAM_ATTR Equilibreuse::interuptionDds()
{
    const static byte sinusTable[512] PROGMEM = {128, 129, 131, 132, 134, 135, 137, 138, 140, 141, 143, 145, 146, 148, 149, 151, 152, 154, 155, 157, 158, 160, 161, 163, 164, 166, 167, 169, 170, 172, 173, 175, 176, 178, 179, 180, 182, 183, 185, 186,
                                                 187, 189, 190, 191, 193, 194, 195, 197, 198, 199, 201, 202, 203, 204, 206, 207, 208, 209, 210, 212, 213, 214, 215, 216, 217, 218, 219, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 230, 231, 232,
                                                 233, 234, 235, 236, 236, 237, 238, 239, 240, 240, 241, 242, 242, 243, 244, 244, 245, 245, 246, 247, 247, 248, 248, 249, 249, 249, 250, 250, 251, 251, 251, 252, 252, 252, 253, 253, 253, 253, 254, 254,
                                                 254, 254, 254, 254, 254, 254, 254, 254, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 252, 252, 252, 251, 251, 251, 250, 250, 249, 249, 249, 248, 248, 247, 247, 246, 245,
                                                 245, 244, 244, 243, 242, 242, 241, 240, 240, 239, 238, 237, 236, 236, 235, 234, 233, 232, 231, 230, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 219, 218, 217, 216, 215, 214, 213, 212, 210, 209,
                                                 208, 207, 206, 204, 203, 202, 201, 199, 198, 197, 195, 194, 193, 191, 190, 189, 187, 186, 185, 183, 182, 180, 179, 178, 176, 175, 173, 172, 170, 169, 167, 166, 164, 163, 161, 160, 158, 157, 155, 154,
                                                 152, 151, 149, 148, 146, 145, 143, 141, 140, 138, 137, 135, 134, 132, 131, 129, 127, 126, 124, 123, 121, 120, 118, 117, 115, 114, 112, 110, 109, 107, 106, 104, 103, 101, 100, 98, 97, 95, 94, 92, 91, 89,
                                                 88, 86, 85, 83, 82, 80, 79, 77, 76, 75, 73, 72, 70, 69, 68, 66, 65, 64, 62, 61, 60, 58, 57, 56, 54, 53, 52, 51, 49, 48, 47, 46, 45, 43, 42, 41, 40, 39, 38, 37, 36, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 25, 24, 23,
                                                 22, 21, 20, 19, 19, 18, 17, 16, 15, 15, 14, 13, 13, 12, 11, 11, 10, 10, 9, 8, 8, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6,
                                                 6, 6, 7, 7, 8, 8, 9, 10, 10, 11, 11, 12, 13, 13, 14, 15, 15, 16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 25, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49, 51, 52, 53, 54, 56, 57,
                                                 58, 60, 61, 62, 64, 65, 66, 68, 69, 70, 72, 73, 75, 76, 77, 79, 80, 82, 83, 85, 86, 88, 89, 91, 92, 94, 95, 97, 98, 100, 101, 103, 104, 106, 107, 109, 110, 112, 114, 115, 117, 118, 120, 121, 123, 124, 126};

    AvancerCodeur();
    if(casse)
    {
        if ((indice & 0x03) == 0)
        {
            accumulateur += (incrementPhase << 6); // accumulateur de phase  (sur 32 bits)

            sinus1 = pgm_read_byte(&(sinusTable[((accumulateur -4) >> 23) & 0x1ff])); //lecture de la valeur du sinus dans la table
            sinus2 = pgm_read_byte(&(sinusTable[((accumulateur >> 23) + 256) & 0x1ff]));

            //sinus = (sinus >> attenuation) - (0x80 >> attenuation) + 0x80; //attenuation de l'amplitude la valeur moyenne reste constante
            dac_output_voltage(DAC_CHANNEL_1, (sinus1 >> 5)); //envoi de la valeur vers le dac
            dac_output_voltage(DAC_CHANNEL_2, (sinus2 >> 6));
        }
    }
    else
    {
        if ((indice & 0x03) == 0)
        {
            accumulateur += incrementPhase; // accumulateur de phase  (sur 32 bits)
            accuBruit += incrementBruit;

            sinus1 = pgm_read_byte(&(sinusTable[(accumulateur >> 23) & 0x1ff])); //lecture de la valeur du sinus dans la table
            sinus2 = pgm_read_byte(&(sinusTable[((accumulateur >> 23) + (dephasageO ? 256 : 0)) & 0x1ff]));
            if (attenuationO)
                sinus2 = (sinus2 >> attenuation) - (0x80 >> attenuation) + 0x80;

            bruit = pgm_read_byte(&(sinusTable[((accuBruit >> 23)+256) & 0x1ff]));
            bruit >>= 5; // amplitude du bruit
            //sinus = (sinus >> attenuation) - (0x80 >> attenuation) + 0x80; //attenuation de l'amplitude la valeur moyenne reste constante
            dac_output_voltage(DAC_CHANNEL_1, (sinus1 >> 1) + (bruitO ? bruit :0)); //envoi de la valeur vers le dac
            dac_output_voltage(DAC_CHANNEL_2, (sinus2 >> 1) + (bruitA ? bruit :0));
        }
    }
}

bool Equilibreuse::getIndexCodeur() const
{
    return (indice == 0 ? true : false);
}

void Equilibreuse::ResetIndice()
{
    indice = 0;
}

bool Equilibreuse::getCodeurB() const
{
    return codeurB;
}

bool Equilibreuse::getCodeurA() const
{
    return codeurA;
}

bool Equilibreuse::getAttenuationO() const
{
    return attenuationO;
}

void Equilibreuse::setAttenuationO(bool value)
{
    attenuationO = value;
}

bool Equilibreuse::getDephasageO() const
{
    return dephasageO;
}

void Equilibreuse::setDephasageO(bool value)
{
    dephasageO = value;
}

bool Equilibreuse::getBruitO() const
{
    return bruitO;
}

bool Equilibreuse::getBruitA() const
{
    return bruitA;
}

void Equilibreuse::setBruitO(bool value)
{
    bruitO = value;
}

void Equilibreuse::setBruitA(bool value)
{
    bruitA = value;
}

void Equilibreuse::marshallDds()
{
    anchor->interuptionDds();
}

void Equilibreuse::setSampleFrequency(int _splFreq)
{
    stop();
    timerAlarmWrite(timer, 1000000 / (_splFreq * 4), true);
    start();
}
