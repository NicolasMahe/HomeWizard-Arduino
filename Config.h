#ifndef Config
#define Config

#include "Common.h"

/*Donné spécific à chaque arduino */

/* libre */
const char APIKEY[] = "38ME9Q6dvWF83UYY"; //libre
static uint8_t mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x36, 0x2C };
static uint8_t ip[] = { 192, 168, 1, 200 };

/* FIN DONNEE SPECIFIC*/


/*Pilotage des relais*/
const int RELAIS_PIN[7] = { 0, O0, O1, O2, O3, O4, O5 };

/* Sensors */
const int SENSORS_PIN[6] = { I0, I1, I2, I3, I4, I5 };

#endif  
