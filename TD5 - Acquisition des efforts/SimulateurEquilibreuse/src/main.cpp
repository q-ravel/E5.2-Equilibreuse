#include <Arduino.h>
#include <driver/adc.h>

#include "Equilibreuse.h"
#include "constantes.h"
#include "clavier.h"



int consignePrec;
int consigne;
bool arret ;
bool ouvert;

Equilibreuse equi;

uint8_t leds[4] = {13,12,14,27};
uint8_t bps[4] = {36,39,34,35};
Clavier leClavier(bps,4);


void setup()
{
    
    Serial.begin(115200);
    Serial.println("Setup Signaux Codeur");
    equi.beginDds(SAMPLING_FREQUENCY);
    pinMode(CAPOT, INPUT);
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    consignePrec = adc1_get_raw(ADC1_CHANNEL_4) * 10;

    arret = false;
    ouvert = false;

    for (int i=0;i < 4 ;i++ )
    {
        pinMode(leds[i],OUTPUT);
    }
}

void loop()
{

    consigne = map(adc1_get_raw(ADC1_CHANNEL_4), 1, 3300, 10, 33000);
    consigne = adc1_get_raw(ADC1_CHANNEL_4) * 10;
    if(consigne > 33300)
    {
        equi.Casser(33300);
    }
    else
    {

        if (consigne >= 33000)
            consigne = 33000;

        if (consigne > 1)
        {
            if (arret == true)
                equi.start();

            consigne = (consigne + 10 * consignePrec) / 11;
            // Serial.printf("Consigne : %d\n\r", consigne);
            // Serial.printf("ADC : %d\n\r", adc1_get_raw(ADC1_GPIO32_CHANNEL));
            consignePrec = consigne;
            equi.setSampleFrequency(consigne);
        }
        else
        {
            equi.stop();
            arret = true;
        }
    }
    if (digitalRead(CAPOT))
    {
        if(!ouvert)
        {
            equi.stop();
            arret = true;
            ouvert = true;
            for(int i=0 ; i <4 ; i++)
            {
                digitalWrite(leds[i],LOW);
            }
            equi.setBruitA(true);
            equi.setBruitO(true);
            equi.setDephasageO(true);
            equi.setAttenuationO(true);
            equi.ResetIndice();
        }
        Clavier::TOUCHES_CLAVIER touche = leClavier.Scruter();
        if(touche != Clavier::AUCUNE)
        {
            //digitalWrite(leds[0],equi.ObtenirEtatCodeurA());
            //digitalWrite(leds[1],equi.ObtenirEtatCodeurB());
            switch(touche)
            {
            case Clavier::BRUIT_A :
                equi.AvancerCodeur();
                Serial.println("avancer");
                break;
            case Clavier::BRUIT_O :
                equi.ReculerCodeur();
                Serial.println("reculer");
                break;
            default:
                break;
            }
            digitalWrite(leds[0],equi.getCodeurA());
            digitalWrite(leds[1],equi.getCodeurB());
            digitalWrite(leds[2],equi.getIndexCodeur());
        }
    }
    else
    {
        if(ouvert)
        {
            ouvert = false;
            for(int i=0 ; i <4 ; i++)
            {
                digitalWrite(leds[i],LOW);
            }

        }
        Clavier::TOUCHES_CLAVIER touche = leClavier.Scruter();
        if(touche != Clavier::AUCUNE)
        {
            digitalWrite(leds[touche],!digitalRead(leds[touche]));
            switch(touche)
            {
            case Clavier::BRUIT_A :
                equi.setBruitA(!equi.getBruitA());
                break;
            case Clavier::BRUIT_O :
                equi.setBruitO(!equi.getBruitO());
                break;
            case Clavier::DEPHASAGE :
                equi.setDephasageO(!equi.getDephasageO());

                break;
            case Clavier::CASSE :
                equi.setAttenuationO((!equi.getAttenuationO()));
                break;
            default:
                break;
            }
        }
        delay(500);
    }
}
