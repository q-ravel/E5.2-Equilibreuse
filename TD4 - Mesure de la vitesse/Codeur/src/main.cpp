#include <Arduino.h>
#include <driver/adc.h>

#include "Codeur.h"

#define CAPOT 33


Codeur leCodeur;
int consignePrec;
int consigne;
bool arret = false;
int prec;

void setup()
{
    Serial.begin(115200);
    Serial.println("Setup Signaux Codeur");
    leCodeur.begin(SAMPLING_FREQUENCY);  
    pinMode(CAPOT,INPUT);
    adc1_config_width(ADC_WIDTH_BIT_12); 
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    consignePrec = adc1_get_raw(ADC1_CHANNEL_4) * 10;
    prec = consignePrec;
    pinMode(VOIE_A, OUTPUT);
}

void loop()
{

    //consigne = map(adc1_get_raw(ADC1_CHANNEL_4), 1, 3300, 10, 33000);
    consigne = adc1_get_raw(ADC1_CHANNEL_4)* 10;
    if (consigne >= 33333)
        consigne = 33333;
    if (consigne != prec)
    {
        Serial.println(consigne);
        prec = consigne;
    }
    if (consigne > 1)
    {

        if(arret == true)
            leCodeur.start();
        
        consigne = (consigne + 10 * consignePrec) / 11;
        // Serial.printf("Consigne : %d\n\r", consigne);
        // Serial.printf("ADC : %d\n\r", adc1_get_raw(ADC1_GPIO32_CHANNEL));
        consignePrec = consigne;
        leCodeur.setSampleFrequency(consigne );
    }
    else 
    {
        leCodeur.stop();
        arret = true;
    }
    if(digitalRead(CAPOT))
    {
        leCodeur.stop();
        arret = true;
    }
    delay(500);   
}
