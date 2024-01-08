#include "clavier.h"

Clavier::Clavier(const uint8_t _bps[],const int _nbPoussoirs):
    nbPoussoirs(_nbPoussoirs)
{
    if(nbPoussoirs > 4)
        nbPoussoirs = 4;
    if(nbPoussoirs > 0)
    {
        for(int indice = 0 ;indice < nbPoussoirs ; indice++)
        {
            lesboutonPoussoirs[indice] = _bps[indice];
            pinMode(lesboutonPoussoirs[indice],INPUT);
        }
    }
}

Clavier::TOUCHES_CLAVIER Clavier::Scruter()
{
    TOUCHES_CLAVIER retour = AUCUNE;
    for(int indice = 0 ;indice < nbPoussoirs ; indice++)
    {
        if(!digitalRead(lesboutonPoussoirs[indice]))
            retour = static_cast <TOUCHES_CLAVIER> (indice);
    }
    delay(200);
    return retour;
}
